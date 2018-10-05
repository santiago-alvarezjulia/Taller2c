#include "cache.h"
#include "cache_direct.h"
#include "cache_associative_fifo.h"
#include "cache_associative_lru.h"
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

CacheProtected::CacheProtected(std::string& cache_type, 
	const std::map<std::string, std::string>& map_data) {
	// El tipo de Cache depende de lo que diga la clave 'cache type', 
	// hay 3 tipos posibles.
	if (cache_type == "associative-lru") {
		this->cache = new Cache_Associative_Lru(map_data);
	} else if (cache_type == "direct") {
		this->cache = new Cache_Direct(map_data);
	} else {
		this->cache = new Cache_Associative_Fifo(map_data);
	}
}

void CacheProtected::print_initialization_data() {
	cache->print_initialization_data();
}
	
int CacheProtected::validate_memory_address_and_print_if_invalid(
	bitset<MEMORY_ADDRESS_SIZE>& memory_address, 
	string& hexa_address) {
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
			
void CacheProtected::procces_memory_address(string& binary_address, 
	string& hexa_address) {
	Lock l(mutex_cache_and_cout);
	cache->procces_memory_address(binary_address, hexa_address);
}
	
void CacheProtected::print_after_processing() {
	cache->print_after_processing();
}

CacheProtected::~CacheProtected() {
	delete this->cache;
}
