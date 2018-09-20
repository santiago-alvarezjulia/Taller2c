#ifndef CACHE_ASSOCIATIVE_LRU_H
#define CACHE_ASSOCIATIVE_LRU_H

#include "cache.h"
#include <deque>
#include <map> 
#include <string>
#include <bitset>
#define MEMORY_ADDRESS_SIZE 32

class Cache_Associative_Lru: public Cache {
	std::map<std::string, bool> cache;
	
	public:
		Cache_Associative_Lru();
		virtual void set_data(std::map<std::string, std::string> map_data);
		virtual void print_initialization_data();
		virtual void procces_memory_address(std::string binary_address, 
			std::string hexa_address);
		virtual void print_informe();
		~Cache_Associative_Lru();
};

#endif //CACHE_ASSOCIATIVE_Lru_H