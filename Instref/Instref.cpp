#include "Instref.h"
#include "Util.h"
#include <vector>
#include <iostream>

using namespace std;

namespace SC
{

void Instrument::load_from_line(const std::string& line )
{
    csvrow r(line);
    EXCEPTION_ASSERT(r.size()==5, "wrong number of cols in csv file"); 
    _id = r.get<int>(0);
    _name = r[1];
    _exId = r[2];
    _exchg = r[3];    
    _mdAddr = r[4];    
}
 

void Instref::load(const std::string& csvfile)
{
    TRY
    std::ifstream f(csvfile);
    EXCEPTION_ASSERT(f, "Csv instref file "+ csvfile + " cannot be opened");        
    auto header = csvline(f);
    for(string line; getline(f,line) && line.size();)
    {
        unique_ptr<Instrument> i(new Instrument);
        i->load_from_line(line);        
        _exIdLookup[i->exId()] = &*i;
        _instruments[i->id()] = &*i;
        _store.push_back( std::move(i) );                
    }    
    FATAL_CATCH
}

void Instref::configure(const boost::property_tree::ptree& pt)
{
    TRY    
    for(const auto& c : pt)
    {
        if(c.second.count("Instref") )            
        {
            const auto& conf= c.second.get_child("Instref");
            string csvfile = conf.get<string>("csvfile");
            load(csvfile);
        }
    }    
    FATAL_CATCH
}


}
