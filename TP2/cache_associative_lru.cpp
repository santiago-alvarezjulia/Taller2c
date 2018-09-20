#include "cache_associative_lru.h"
#include <string>
#include <deque>
#include <map> 
#include <iostream>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <utility>
#define MEMORY_ADDRESS_SIZE 32
#define ERROR 1
#define OK 0

Cache_Associative_Lru::Cache_Associative_Lru() {}

void Cache_Associative_Lru::set_data(std::map<std::string, 
	std::string> map_data) {
	Cache::set_data(map_data);
	
	std::map<std::string, bool> cache_map;
	this->cache = cache_map;
}

void Cache_Associative_Lru::print_initialization_data() {
	Cache::print_initialization_data();
}

void Cache_Associative_Lru::procces_memory_address(std::string binary_address, 
	std::string hexa_address) {
	std::string tag = binary_address.substr(0, MEMORY_ADDRESS_SIZE - 
			this->offset_len);
				
	std::map<std::string, bool>::iterator it;
	it = this->cache.find(tag);
	if (it != this->cache.end()) {
		// HIT -> elimino el bloque de la cache y lo vuelvo a agregar
		this->cache.erase(it);
		this->cache.insert(std::pair<std::string, bool> (tag, true));
		
		this->hits += 1;
		
		if (this->debug_mode) {
			std::transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			std::cout << "Hit: " <<  hexa_address << std::endl;
		}
	} else {
		// MISS -> primero verifico si esta lleno
		if (this->cache.size() == (size_t)(this->size / this->block_size)) {
			// esta lleno. Saco el menos usado recientemente
			this->cache.erase(this->cache.begin());
		}
		// Agrego el tag al final (indiferente si estaba lleno o no)
		this->cache.insert(std::pair<std::string, bool> (tag, true));
		
		this->misses += 1;
		
		if (this->debug_mode) {
			std::transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			std::cout << "Miss: " <<  hexa_address << std::endl;
		}
	}			
}

void Cache_Associative_Lru::print_informe() {
	Cache::print_informe();
}

Cache_Associative_Lru::~Cache_Associative_Lru() {}
