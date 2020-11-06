
#include "Component.h"
#include <boost/property_tree/ptree.hpp>
#include "Logger.h"

using namespace std;

namespace SC 
{
    
    


void Component::load_component(const ptree& config)
{
    _name = config.get<string>("name");
    LOG_INFO("Loading " + _name);
    load(config);
}
void Component::init_component()
{
    init();
}
void Component::start_component()
{
    LOG_INFO("Starting " + name());
    if(started())
        return;
    start();
    _started = true;
}
void Component::stop_component()
{
    LOG_INFO("Stopping " + name());
    if(!started() || stopped() || stopping() )
        return;
    _stopping = true;
    stop();    
    _stopped = true;
}


    
}