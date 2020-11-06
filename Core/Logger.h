
#pragma once

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include "General.h"


namespace SC
{

enum eSeverity
{
    SeverityReserved,
    SeverityError,
    SeverityInfo,        
    SeverityDebug,
    SeverityLevelsCount
};

#define LOG_MSG(severity, msg,...) {if(logger.maxSeverity() >= severity) { \
    std::stringstream str; str << msg;  \
    logger.log(severity, str.str());\
    }}

#define LOG_ERROR(msg,...)  LOG_MSG(SeverityError, msg)
#define LOG_INFO(msg,...)   LOG_MSG(SeverityInfo, msg)
#define LOG_DEBUG(msg,...)  LOG_MSG(SeverityDebug, msg)




// The main logging class for logging into log file/cout (based on config)
// We always require the presence of this class so it is not a component but
// it is also configured by the ComponentManager from the config
//
// We can create more loggers and configure them differently for different purposes, 
// but the default logger always exists and is initialised before any component or instref.
class Logger
{
public:
    //maximal displayed severity
    eSeverity maxSeverity() { return _max_severity; }
    
    // General logging function
    void log(eSeverity severity, const std::string& msg)
    {
        if(severity>_max_severity)
            return;
        logInternal(severity, msg);
    }
    // configure the logger
    void configure(const boost::property_tree::ptree& pt);
private:
    
    // This i the internal logging method
    // TODO: add offloading of writes using ringbuffer and file output
    void logInternal(eSeverity severity, const std::string& msg);
        
    eSeverity _max_severity{SeverityLevelsCount};    
};
    

// Default/main logger class
extern Logger logger; 
    
}