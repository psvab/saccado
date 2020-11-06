
#include "Logger.h"

using namespace std;


namespace SC
{
    
void Logger::logInternal(eSeverity severity, const std::string& msg)
{
    static std::string _severityStr[SeverityLevelsCount] {"", "Error","Info","Debug" };
    auto &out = severity==SeverityError ? cerr : cout;
    out << _severityStr[severity] << ": " <<  msg << std::endl;
}    

void Logger::configure(const boost::property_tree::ptree& pt)
{
    TRY    
    for(const auto& c : pt)
    {
        if(c.second.count("Logger") )            
        {
            const auto& conf= c.second.get_child("Logger");
            _max_severity = static_cast<eSeverity>(conf.get<int>("max_severity"));
            //log("Maximal logging severity is "+ _max_severity);
        }
    }    
    FATAL_CATCH
}

Logger logger;
    
    
}