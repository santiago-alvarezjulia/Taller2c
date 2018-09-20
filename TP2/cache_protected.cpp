#include "cache.h"
#include "Lock.h"
#include "cache_protected.h"
#include <mutex>
#include <bitset>
#include <string>
#include <map>
#include <iostream>
#include <cmath>
#include <algorithm>
#define ERROR 1
#define OK 0

CacheProtected::CacheProtected(Cache* cache) : cache(cache) {}
		
void CacheProtected::set_data(std::map<std::string, std::string> map_data) {
	cache->set_data(map_data);
}
		
void CacheProtected::print_initialization_data() {
	cache->print_initialization_data();
}
	
int CacheProtected::validate_memory_address_and_print_if_invalid(
	std::bitset<MEMORY_ADDRESS_SIZE> memory_address, 
	std::string hexa_address) {
	Lock l(mutex_cerr);
	int mod = memory_address.to_ulong() % 4;
	if (mod != 0) {
		std::transform(hexa_address.begin(), hexa_address.end(), 
			hexa_address.begin(), ::tolower);
		std::cerr << "Direccion invalida: " <<  hexa_address << std::endl;
		std::cerr << "Abortando" << std::endl;
		return ERROR;
	}
	return OK;
}
			
void CacheProtected::procces_memory_address(std::string binary_address, 
	std::string hexa_address) {
	Lock l(mutex_cache_and_cout);
	cache->procces_memory_address(binary_address, hexa_address);
}
	
void CacheProtected::print_informe() {
	cache->print_informe();
}

CacheProtected::~CacheProtected() {}
