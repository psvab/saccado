#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace SC
{
    
struct checked_ifstream : std::ifstream
{
    checked_ifstream(const char *path) : std::ifstream(path)
    {
        if(!*this)
            throw std::runtime_error(std::string("Cannot open file ") + path);
    }
    checked_ifstream(const std::string& path) : checked_ifstream(path.c_str())
    {}
};

// If we specify separator 0, it will skip any number of whitespace characters    
inline std::vector<std::string> csvline(const std::string& line, char separator=',')    
{
    std::stringstream linestr(line);
    std::vector<std::string> ret;
    if(separator)
    {
        for(std::string col; std::getline(linestr, col, separator); )
            ret.push_back(col);
    }else
    {
        std::string col;
        for(linestr>>col; col.size(); linestr>>col)
        {
            ret.push_back(col);        
            col="";
        }
    }
    return ret;
}        

inline std::vector<std::string> csvline(std::istream& in, char separator=',')    
{
    std::string line;
    if(!std::getline(in,line))
        throw std::runtime_error("empty csv input stream");
    return csvline(line, separator);
}

// Simple class parsing and enabling access to one csv row
struct csvrow 
{
    csvrow(const std::string& line, char separator=',') {
        _cols = csvline(line, separator);
    }
    template<typename T>
    T get(unsigned i) { return boost::lexical_cast<T>(_cols.at(i)); }
    // return the raw string value
    const std::string& operator [] (unsigned i) const  { return _cols.at(i); }
    
    size_t size() const { return _cols.size(); }
private:
    std::vector<std::string> _cols;
};

    
    
    
    
}