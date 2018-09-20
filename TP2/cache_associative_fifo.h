#ifndef CACHE_ASSOCIATIVE_FIFO_H
#define CACHE_ASSOCIATIVE_FIFO_H

#include "cache.h"
#include <deque>
#include <map> 
#include <string>
#include <bitset>
#define MEMORY_ADDRESS_SIZE 32

class Cache_Associative_Fifo: public Cache {
	std::deque<std::string> cache;
	std::map<std::string, bool> addresses_in_cache;
	
	public:
		Cache_Associative_Fifo();
		virtual void set_data(std::map<std::string, std::string> map_data);
		virtual void print_initialization_data();
		virtual void procces_memory_address(std::string binary_address, 
			std::string hexa_address);
		virtual void print_informe();
		~Cache_Associative_Fifo();
};

#endif //CACHE_ASSOCIATIVE_FIFO_H
