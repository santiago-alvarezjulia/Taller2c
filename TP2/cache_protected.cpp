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
using std::map;
using std::cerr;
using std::endl;
using std::string;
using std::bitset;
using std::transform;

CacheProtected::CacheProtected(Cache* cache) : cache(cache) {}
		
void CacheProtected::set_data(map<string, string> map_data) {
	cache->set_data(map_data);
}
		
void CacheProtected::print_initialization_data() {
	cache->print_initialization_data();
}
	
int CacheProtected::validate_memory_address_and_print_if_invalid(
	bitset<MEMORY_ADDRESS_SIZE> memory_address, 
	string hexa_address) {
	Lock l(mutex_cerr);
	int mod = memory_address.to_ulong() % 4;
	if (mod != 0) {
		transform(hexa_address.begin(), hexa_address.end(), 
			hexa_address.begin(), ::tolower);
		cerr << "Direccion invalida: " <<  hexa_address << endl;
		cerr << "Abortando" << endl;
		return ERROR;
	}
	return OK;
}
			
void CacheProtected::procces_memory_address(string binary_address, 
	string hexa_address) {
	Lock l(mutex_cache_and_cout);
	cache->procces_memory_address(binary_address, hexa_address);
}
	
void CacheProtected::print_informe() {
	cache->print_informe();
}

CacheProtected::~CacheProtected() {}
