
#include "ComponentManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <thread>
#include <signal.h>
// Add all component headers to be include here
#include "MDLink.h"
#include "TradingLink.h"
#include "Logger.h"
#include "Instref.h"
#include "MDSender.h"
#include "MockExchange.h"
#include "EventQ.h"
#include "Strategy.h"

using namespace std;




namespace SC 
{
    
    

FACTORY_DEFINE(sComponentFactory, Component);

// Add all components here
FACTORY_ADD(sComponentFactory, Component, GoMDLink);
FACTORY_ADD(sComponentFactory, Component, GoTLink);
FACTORY_ADD(sComponentFactory, Component, EventQ);
FACTORY_ADD(sComponentFactory, Component, Strategy1);

FACTORY_ADD(sComponentFactory, Component, MockTLink);
FACTORY_ADD(sComponentFactory, Component, MockExchange);
FACTORY_ADD(sComponentFactory, Component, MDSender);

void ComponentManager::start_all()
{
    for(auto& c : _components)
        c->init_component();
    for(auto& c : _components)
        c->start_component();
}
void ComponentManager::stop_all()
{   {
        std::scoped_lock<mutex> lock(mutex);
        if(_stopping) return;
        _stopping = true;
    }
    for(auto it=_components.rbegin(); it!=_components.rend(); ++it)
    {
        auto& c = *it;
        c->stop_component();
    }    
}
void ComponentManager::tear_down()
{
    stop_all();
    _components_map.clear();
    _components.clear();
    _stop = _stopping = false;
}

void ComponentManager::shutdown()
{
    _stop = true;
}


static volatile int sSignalReceived = 0;

void intHandler(int dummy) 
{
    sSignalReceived = 1;
}

void ComponentManager::run(bool teardown)
{
    signal(SIGINT, intHandler);  
    start_all();
    LOG_INFO("All started. Running");
    while(!_stop)
    {
        if( sSignalReceived)
            _stop = true;
        this_thread::sleep_for(300ms);
    }
    if(teardown)
        tear_down();
}


void ComponentManager::load_from_json(const std::string& config_path)
{
    TRY
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(config_path, pt);
    load(pt);
    FATAL_CATCH
}


void ComponentManager::load(const ptree& config)
{
    _config = config;
    logger.configure(config);
    Instref::instance().configure(config);
    TRY
    for (const auto& comp_node_it : config)
    {
        assert(comp_node_it.first=="");
        const auto comp_node = comp_node_it.second;
        if( comp_node.count("class") ==0 ) // not a class element config entry, ignore
            continue;
        auto name = comp_node.get<string>("class");
        Component_ptr component( FACTORY_CREATE(sComponentFactory,name) );
        component->load_component(comp_node);        
        EXCEPTION_ASSERT( _components_map.find(component->name()) == _components_map.end(), 
                "Duplicate component name " + component->name());
        _components_map[component->name()] = &*component;
        _components.push_back( std::move(component) );        
    }
    FATAL_CATCH
}

    
    
    
    
}