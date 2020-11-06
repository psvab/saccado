#pragma once

#include "Factory.h"
#include <memory>
#include <string>
#include <map>
#include <thread>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/core/noncopyable.hpp>

namespace SC
{
 
typedef boost::property_tree::ptree ptree;



/// One component of the system, this could be anything that is able to do any 
// useful work and we may want to look it up. A base of all components.
// All components are loaded in order (of config file spec)
// Then all init are called in order
// Then all start are called in order
// On shutdown, all stop are called in revese order
class Component : boost::noncopyable
{
    
public:
    virtual ~Component() {}
    virtual std::string class_name() { return typeid(*this).name(); }
    
    // the default load the component things only (name); calls load()
    void load_component(const ptree& config); 
    // initialise what was not initialised in the load; calls init()
    void init_component();
    // start the run of this component and set its start flag; calls start()
    void start_component();
    // stop the component - de-allocate what you can here too; calls stop()
    void stop_component();

    /// return the component name
    const std::string& name() { return _name; }
    
    // return the running state of the component with methods below
    
    bool started() const {  return _started; }
    bool stopping() const {  return _stopping; };
    bool stopped() const {  return _stopped; };

protected:
    //  The methods to be overriden
    
     // the default load the component things only (name), to be overriden
    virtual void load(const ptree& config) {}
    // initialise what was not initialised in the load
    virtual void init() {}
    // start the run of this component and set its start flag
    virtual void start() {}
    // stop the component - de-allocate what you can here too
    virtual void stop() {}
    
    std::string _name; // name of the component
    bool _started{false};
    bool _stopping{false};
    bool _stopped{false};
};

typedef std::unique_ptr<Component> Component_ptr;

FACTORY_DECLARE(sComponentFactory, Component);



/// A component that is thread running
class ThreadRunningComponent : public Component
{
public:
protected:    
    void start() override
    {
        _runner = std::make_unique<std::thread>( [this](){ this->run(); } );    
    }

    void stop() override
    {
        if(_runner)
            _runner->join();    
    }
    
    virtual void run() = 0;
    
    std::unique_ptr<std::thread> _runner;
};

}