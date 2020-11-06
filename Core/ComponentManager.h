#pragma once

#include "Factory.h"
#include "Component.h"
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <boost/property_tree/ptree.hpp>
#include <boost/core/noncopyable.hpp>

namespace SC
{


// The manager of components - retrieval, creation, loading, starting, stopping etc
// A singleton int he project
// The container first loads and initialises all components, then starts them
// When shutting down/ending, the container sets itself to stopping state, 
//    and then stops all components 
class ComponentManager final : boost::noncopyable
{
    ComponentManager() = default;
public:    
    virtual ~ComponentManager() { tear_down(); }    
    
    static ComponentManager& instance() 
    {
        static ComponentManager mgr; // C++11 standard should guarantee this is ok
        return mgr;
    }
    
    // Run the component manager until stop is requested
    // The first thing it does is to start_all() then it runs, then tears down
    // This method blocks the calling thread 
    // At the end of the it, it call tear_down
    // IFF teardonw==false then tear_down is not called at the end of run
    void run(bool teardown=true);
    
    // load components from a file name/path
    void load_from_json(const std::string& config_path); 
    // load components from already opened config file
    void load(const ptree& config);            
    // return the loaded config
    const ptree& config() const { return _config; }
    
    
    
    // initialise and start all
    void start_all();
    //  stop all
    void stop_all();
    
    // return if the contrainer is in a shutdown mode
    bool stopping() const { return _stopping; }
    
    // start the shutdown of the container
    void shutdown();
    
    // return true iff a component exists
    bool component_exists(const std::string& name) 
    {
        return _components_map.find(name) != _components_map.end();
    }
    
    /// return the component by its name, throw on wrong type
    template<typename T=Component>
    T& get_by_name(const std::string& name)
    {
        return dynamic_cast<T&>( *_components_map.at(name) ); // throw on wrong type
    }
    
    /// return the first component of the given type
    template<typename T=Component>
    T& get_first()
    {
          using namespace std::string_literals;
        for(auto& c : _components)
            if( dynamic_cast<T*>(&*c) ) 
                return *dynamic_cast<T*>(&*c);
        throw std::invalid_argument(typeid(T).name() + " not found in component container"s);
    }

/// return the first component of the given type
    template<typename T=Component>
    std::vector<T*> get_all()
    {
        std::vector<T*> ret;
         for(auto& c : _components)
            if( dynamic_cast<T*>(&*c) ) 
                ret.push_back(dynamic_cast<T*>(&*c));
         return ret;
    }    
    
    // clears the container (stops all containers)
    void tear_down();
    
private:
    ptree _config;
    std::vector<Component_ptr> _components;
    std::map<std::string, Component*> _components_map; // components by name
    bool _stopping{false};
    bool _stop{false};
    std::mutex _mutex;
};
    
    
}