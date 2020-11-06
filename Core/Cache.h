#pragma once


#include <vector>
#include <cassert>

namespace SC
{
    
/// Simple preallocation cache to provide elements fast
// We have to allocate enough at the start to not exceed as this does not do
// range checks to run fast
template<typename T>
struct Cache
{
    Cache(size_t maxElements, const T& init=T()) : _items(maxElements,init)
    {
        _free.reserve(_items.size());
        for(auto& i : _items)
            _free.push_back(&i);
    }
    // return a pre-cached item for use - equivalent to new ()
    T& get() 
    {  
        assert( _nextFree<_free.size() );
        return *_free[_nextFree++];
    }
    // replace the pre-cached item back - equivalent to delete ()
    void give_back(T& item)
    {
        assert( _nextFree>0);
        _free[--_nextFree] = &item;
    }
    
 private:     
     std::vector<T> _items;    // pre-allcated elements
     std::vector<T*> _free;    // free elements
     std::size_t _nextFree{0}; // grows as we allocate
};

    
    
}

