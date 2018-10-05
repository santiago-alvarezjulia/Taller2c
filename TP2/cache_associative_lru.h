#ifndef CACHE_ASSOCIATIVE_LRU_H
#define CACHE_ASSOCIATIVE_LRU_H

#include "cache.h"
#include <deque>
#include <map> 
#include <string>
#include <bitset>

class Cache_Associative_Lru: public Cache {
	std::deque<std::string> cache;
	std::map<std::string, bool> addresses_in_cache;
	
	public:
		Cache_Associative_Lru(const std::map<std::string, std::string>& 
			map_data);
		virtual void procces_memory_address(std::string& binary_address, 
			std::string& hexa_address);
		~Cache_Associative_Lru();
};

#endif //CACHE_ASSOCIATIVE_Lru_H
