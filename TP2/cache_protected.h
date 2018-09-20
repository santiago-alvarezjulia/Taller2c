#ifndef CACHE_PROTECTED_H
#define CACHE_PROTECTED_H

#include "cache.h"
#include "Lock.h"
#include <mutex>
#include <bitset>
#include <string>
#include <map>


class CacheProtected {
	Cache* cache;
	std::mutex mutex_cache_and_cout;
	std::mutex mutex_cerr;
	
	public:
		explicit CacheProtected(Cache* cache);
		void set_data(std::map<std::string, std::string> map_data);
		void print_initialization_data();
		int validate_memory_address_and_print_if_invalid(
			std::bitset<MEMORY_ADDRESS_SIZE> memory_address, 
			std::string hexa_address);
		void procces_memory_address(std::string binary_address, 
			std::string hexa_address);
		void print_informe();
		~CacheProtected();
};

#endif //CACHE_PROTECTED_H
