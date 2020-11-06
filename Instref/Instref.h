
#pragma once
#include <vector>
#include <unordered_map>
#include <boost/core/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include "General.h"

namespace SC
{


// Generic data for one instrument
struct Instrument
{
	Instrument() = default;
	Instrument(int id_, const std::string& name_, const std::string &td_, const std::string& md_) : _id(id_), _name(name_)
	{}
        // our numerical instrument id  for quick lookups
        InstrumentId id() const { return _id; }
        // instrument id for us
	const std::string& getName() const { return _name; }
        // instrument id on the exchange
	const std::string& exId() const { return _exId; }
        /// return the name of the exchange of the instref
        const std::string& exchange() const { return _exchg; }
        /// return the multicast address in format of IP:port where IP is as a.b.c.d)
        const std::string& mdAddr() const { return _mdAddr; }
        
        
        void load_from_line(const std::string& line );
private:
	InstrumentId _id{ 0 };
	std::string _name; // our name
	std::string _exId;   // exchange side id (Assuming the same for md and tl here)
        std::string _exchg; // exchange code
        std::string _mdAddr; // market data multicast address
};


// Container class for all instruments.
// As we always expect require its presence, it is not a component even though 
//  it is initialised  by the component manager after the logger.
class Instref final : boost::noncopyable
{
    /// constructed by the factory in component manager
public:
    static Instref& instance() { 
        static Instref inst;
        return inst;
    }    
    
    Instrument* getById(InstrumentId id) const { return _instruments[id]; }
    Instrument& getByIdOrThrow(InstrumentId id) const;
    
    /// return the instruemnt by exchnge id, does not throw
    Instrument* getByExchgId(const std::string& exId)  { return _exIdLookup[exId]; }
    Instrument& getByExchgIdOrThrow(const std::string& exId);
        
    const std::vector<std::unique_ptr<Instrument>>& allInstref() const { return _store; }
private:
    friend class ComponentManager;
    Instref() = default;
    
    // load the instref content from a file
    void load(const std::string& csvfile);    
    // configure the instref
    void configure(const boost::property_tree::ptree& pt);
    
    std::unordered_map<std::string,Instrument*>   _exIdLookup;      // 
    std::array<Instrument*, MAX_INST_ID>  _instruments; // id lookup
    std::vector<std::unique_ptr<Instrument>> _store;
};

inline Instrument& Instref::getByIdOrThrow(InstrumentId id) const 
{ 
    auto ret = _instruments[id]; 
    EXCEPTION_ASSERT(ret, "instrument "+boost::lexical_cast<std::string>(id)+"not found");
    return *ret;
}

inline Instrument& Instref::getByExchgIdOrThrow(const std::string& exId) 
{
    auto loc = _exIdLookup.find(exId);
    EXCEPTION_ASSERT( (loc!=_exIdLookup.end()), "Could not find instrument with exchange id" + exId);
    return *loc->second;
}

}