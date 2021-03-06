#ifndef CACHE_DIRECT_H
#define CACHE_DIRECT_H

#include "cache.h"
#include <map>
#include <string>
#include <bitset>
#define MEMORY_ADDRESS_SIZE 32

class Cache_Direct: public Cache {
	std::map<std::string, std::string> cache;
	int index_len;
	
	public:
		explicit Cache_Direct(const std::map<std::string, 
			std::string>& map_data);
		virtual void procces_memory_address(std::string& binary_adress, 
			std::string& hexa_address);
		~Cache_Direct();
};

#endif //CACHE_DIRECT_H
