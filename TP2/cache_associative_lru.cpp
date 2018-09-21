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
	
	std::deque<std::string> cache_deque;
	this->cache = cache_deque;
	std::map<std::string, std::deque<std::string>::iterator> 
		addresses_in_cache_map;
	this->addresses_in_cache = addresses_in_cache_map;
}

void Cache_Associative_Lru::print_initialization_data() {
	Cache::print_initialization_data();
}

void Cache_Associative_Lru::procces_memory_address(std::string binary_address, 
	std::string hexa_address) {
	// get tag
	std::string tag = binary_address.substr(0, MEMORY_ADDRESS_SIZE - 
		this->offset_len);
	
	std::map<std::string, std::deque<std::string>::iterator>::iterator it;
	it = this->addresses_in_cache.find(tag);
	if (it != this->addresses_in_cache.end()) {
		// HIT -> elimino el bloque de la deque y lo vuelvo a encolar
		this->cache.erase(it->second);

		this->cache.push_front(tag);
		
		this->addresses_in_cache[tag] = this->cache.begin();
		
		this->hits += 1;
		
		if (this->debug_mode) {
			std::transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			std::cout << "Hit: " <<  hexa_address << std::endl;
		}
	} else {
		// MISS, primero verifico si esta lleno
		if (this->addresses_in_cache.size() == 
			(size_t)(this->size / this->block_size)) {
			// esta lleno. Saco el ultimo y pongo este
			std::string tag_popped = this->cache.back();
			this->cache.pop_back();
			this->addresses_in_cache.erase(tag_popped);
		}
		// agrego al final y agrego al map
		this->cache.push_front(tag);
		this->addresses_in_cache.insert(std::pair<std::string, 
			std::deque<std::string>::iterator>(tag, this->cache.end()));
		
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
