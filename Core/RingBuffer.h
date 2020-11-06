#pragma once

#include<memory>
#include<array>
#include<cassert>
#include<vector>
#include<atomic>
#include<mutex>
#include "ThreadUtils.h"

#include <iostream>

namespace SC
{



// Spin lock implementation to be tuned later
template<typename T>
class RingBufferSP
{
public:
	RingBufferSP(unsigned size = 65536) : mods(size - 1), _buffer(size)
	{
		assert((size & mods) == 0); //size is power of 2
	}

	long int size()
	{
		return head - tail;
	}
	void read(T& ret)
	{
		scoped_lock lock(rm);
		while (tail == head)
			std::atomic_thread_fence(std::memory_order_acquire)
			;
		ret = _buffer[tail & mods];
		++tail;
		std::atomic_thread_fence(std::memory_order_release);

	}

	void write(const T& val)
	{
		scoped_lock lock(wm);
		while (head - tail > mods)
			std::atomic_thread_fence(std::memory_order_acquire);
		_buffer[head & mods] = val;
		std::atomic_thread_fence(std::memory_order_release);
		++head;
	}

private:
	mutable mutex wm, rm;
	unsigned mods;
	long unsigned head = 0; // next item place  to write
	long unsigned tail = 0;	// first place to read
	std::vector<T> _buffer;
};


template<typename T>
using RingBuffer = RingBufferSP<T>;


}