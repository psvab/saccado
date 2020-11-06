

#pragma once

#include "General.h"
#include "Logger.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h> 
#include <unistd.h> 


namespace SC
{

// Network address as IP:port[:if]    
struct NetAddr
{
    NetAddr() = default;
    NetAddr(const std::string addr);
    
    // return as IP:port[:if]
    const std::string asStr() const;

    const std::string& ip() const { return _ip; }
    int port() const { return _port; }
    const std::string& iface() const { return _if; }
    const struct sockaddr_in& sockaddr() const { return _sockaddr;  }
private:        
    std::string _ip;
    int _port{0};
    std::string _if;
    struct sockaddr_in _sockaddr;  
};
    
// Simple socket wrapper that closes open sockets at the end of lifetime
struct Socket : boost::noncopyable
{
    Socket() = default;
    
    virtual ~Socket()   {  close();  }
               
    void close()   
    { 
        if(_fd) ::close(_fd); 
        _fd = 0; 
    }
    int fd() const { return _fd; }
    
    int read(char* buffer, size_t size)
    {
        return ::read(fd(), buffer, size);
    }
    // send data 
    int send(const char * data, size_t size)
    {
        return ::send(fd(), data, size, 0);
    }

protected:
    int _fd{0};
    friend struct TCPSocket;
};
    


struct McastSocketBase : Socket
{
    McastSocketBase() {
        _fd = socket(AF_INET, SOCK_DGRAM, 0);
        EXCEPTION_ASSERT( _fd!=0, "Socket() failed");
    }
    
    virtual void setAddr(const NetAddr& addr) 
    {
        _addr = addr;
    }
    const NetAddr& addr() const { return _addr; }
protected:
    NetAddr _addr;    
};

struct McastSendSocket : McastSocketBase
{
    // Create mcst socket for sending ont he given address
    McastSendSocket() = default;
    McastSendSocket(const NetAddr& addr) 
    {
        setAddr(addr);
    }    
    unsigned sendto(const char* data, size_t len, const NetAddr& group)
    {
        size_t sent = ::sendto(fd(), data, len, 0, (struct sockaddr *)&group.sockaddr(), sizeof(struct sockaddr_in));
        if (sent != len)
            throw std::runtime_error("sendto() sent a different number of bytes than expected");
        return sent;
    }
    void sendtoOrThrow(const char* data, size_t len, const NetAddr& group)
    {
        size_t sent = sendto(data, len, group);
        if (sent != len)
            throw std::runtime_error("sendto() sent a different number of bytes than expected");
    }        
};


struct McastRecvSocket : McastSocketBase
{
    // Create mdcaset socket bound to the given address port, joining the given mcast group
    McastRecvSocket() 
    {
        u_int yes = 1;
        EXCEPTION_ASSERT( setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes))==0,
                "Reuse addr failed on socket");                        
    }
    McastRecvSocket(const NetAddr& addr) : McastRecvSocket()
    {
        setAddr(addr);
    }
    // Bind the socket to the given local port
    void setAddr(const NetAddr& addr_)
    {       
        McastSocketBase::setAddr(addr_);
        struct sockaddr_in& addr = _bindaddr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
        addr.sin_port = htons(addr_.port());
        EXCEPTION_ASSERT(bind(fd(), (struct sockaddr*) &addr, sizeof(addr)) ==0, 
                "could not bind socket to " + _addr.asStr());                                
        
        // enable breaking out for endloop condition check
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10 * 1000; // 10 milliseconds
        EXCEPTION_ASSERT(setsockopt(fd(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))==0, "set timeout failed");
}
    
    // Join mcast address
    void join(const NetAddr& group)
    {
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(group.ip().c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        EXCEPTION_ASSERT( setsockopt(_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) ==0,
            "Could not add mcast membership to " + group.asStr());        
    }    
    void leave(const NetAddr& group)
    {
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(group.ip().c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        EXCEPTION_ASSERT( setsockopt(_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) ==0,
            "Could not drop mcast membership to " + group.asStr());        
    }    
    
    int recvfrom(char* buffer, size_t len)
    {
        return ::recvfrom(fd(), buffer, len, 0, (struct sockaddr *) &_bindaddr, &_addrlen);
    }
private:    
    struct sockaddr_in _bindaddr;
    unsigned int _addrlen = sizeof(struct sockaddr_in);
};


struct TCPSocket : Socket
{
    TCPSocket() 
    {
        _fd = socket(AF_INET, SOCK_STREAM, 0);
        EXCEPTION_ASSERT( _fd!=0, "TCP socket() failed");
    }
    TCPSocket(const NetAddr& addr) : TCPSocket()
    {
        setAddr(addr);
    }
    void setAddr(const NetAddr& addr)
    {
        _addr = addr;
    }    
    // client connect    
    void connect(const NetAddr& addr)
    {
        setAddr(addr);
        connect();
    }
    //client connect to address stored via setAddr
    void connect()
    { 
        struct sockaddr_in _serv_addr; 
        memset(&_serv_addr, 0, sizeof(struct sockaddr_in));
        _serv_addr.sin_family = AF_INET; 
        _serv_addr.sin_port = htons(_addr.port());         
        // Convert IPv4 and IPv6 addresses from text to binary form 
        EXCEPTION_ASSERT(inet_pton(AF_INET, _addr.ip().c_str(), &_serv_addr.sin_addr) >0,
                "Invalid address/ Address not supported :" + _addr.asStr());  
        EXCEPTION_ASSERT(::connect(fd(), (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) >=0,
            "TCP Connection Failed to : " + _addr.asStr());        
    }
        
    
    // Run as a server :
    // Create, server_bind, accept - >run with accepted socket    
    void server_bind()
    {
        int opt = 1;
        
        EXCEPTION_ASSERT(setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)) ==0, "Coult not set opt");
        
        struct sockaddr_in& address = _server_address;
        memset(&address, 0, sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( _addr.port() );

        // Forcefully attaching socket to the port 8080
        EXCEPTION_ASSERT(bind(fd(), (struct sockaddr *)&address,  sizeof(address)) >=0,
                " Could not bind to TCP port");        
        EXCEPTION_ASSERT(listen(fd(), 3) >=0, "Listen failed on " + _addr.asStr());
    }
     // Run as a server and return new socket
    void accept_connection(Socket&  sock)
    {
        unsigned addrlen = sizeof(_server_address);
        int new_socket = accept(fd(), (struct sockaddr *)&_server_address, (socklen_t*)&addrlen);
        if(new_socket<0)
            throw std::runtime_error("Accept failed on " + _addr.asStr());
        sock._fd = new_socket;
    }
        
    const NetAddr& addr() const { return _addr; }
        
private:    
    NetAddr _addr;
    struct sockaddr_in _server_address;
        
};



    
}




#include "Network.h"
#include <sstream>

using namespace std;

namespace SC
{

inline NetAddr::NetAddr(const std::string addr_)
{
    stringstream str(addr_);
    EXCEPTION_ASSERT(getline(str,_ip, ':') && _ip.size(), "No IP part in addr " + addr_);
    string port;
    getline(str,port, ':');
    EXCEPTION_ASSERT( port.size(), "No port part in addr " + addr_);
    _port = std::atoi(port.c_str());    
    getline(str,_if, ':');   
    
    struct sockaddr_in& addr = _sockaddr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(_ip.c_str());
    addr.sin_port = htons(_port);
}

inline const std::string NetAddr::asStr() const
{
    stringstream str;
    str << _ip << ':' << _port;
    if( !_if.empty() ) str<<':' << _if;
    return str.str();
}
    
    
    
}