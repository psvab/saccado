
#pragma once

#include "General.h"
#include <map>

// Simple factory wrapper implementation - for creation of factory classes
// Usage: In header file of the base class Base.h :
// FACTORY_DECLARE(ABFac, Base);
//In Base.cpp add:  
//  FACTORY_DEFINE(ABFac, Base);
//  FACTORY_ADD(ABFac, Base, A);
//  FACTORY_ADD(ABFac, Base, B);
//In file where you use it add: 
//  b = unique_ptr<Base>( FACTORY_CREATE(ABFac,"B") );

namespace SC
{

template<typename Base, typename T>
Base* type_factory_fnc() { return new T; }

template<typename T>
using type_factory = std::map<std::string, T*(*)() >;

}

#define FACTORY_DECLARE(factoryName, Base)    extern SC::type_factory<Base> factoryName;

#define FACTORY_DEFINE(factoryName, Base)     SC::type_factory<Base> factoryName;

#define FACTORY_ADD(factoryName, Base, Type)  auto MAKE_UNIQUE(factoryName) = [](){ factoryName[#Type] = &SC::type_factory_fnc<Base, Type>; return 0; }();

#define FACTORY_CREATE(factoryName, TypeStr)  [&](){ auto val=factoryName[TypeStr]; \
                                               if(!val) throw std::invalid_argument(std::string("Unknown class in factory :")+TypeStr); \
                                               return val(); }()
