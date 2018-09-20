#include "cache.h"
#include "Lock.h"
#include "cache_protected.h"
#include <mutex>
#include <bitset>
#include <string>
#include <map>

CacheProtected::CacheProtected(Cache* cache) : cache(cache) {}
		
void CacheProtected::set_data(std::map<std::string, std::string> map_data) {
	cache->set_data(map_data);
}
		
void CacheProtected::print_initialization_data() {
	Lock l(mutex_cout);
	cache->print_initialization_data();
}
	
int CacheProtected::validate_memory_address_and_print_if_invalid(
	std::bitset<MEMORY_ADDRESS_SIZE> memory_address, 
	std::string hexa_address) {
	Lock l(mutex_cerr);
	return cache->validate_memory_address(memory_address, 
		hexa_address);
}
			
void CacheProtected::procces_memory_address(std::string binary_address, 
	std::string hexa_address) {
	Lock l(mutex_cache);
	cache->procces_memory_address(binary_address, hexa_address);
}
	
void CacheProtected::print_informe() {
	Lock l(mutex_cout);
	cache->print_informe();
}

CacheProtected::~CacheProtected() {}
