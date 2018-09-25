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
using std::deque;
using std::map;
using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::pair;
using std::transform;

Cache_Associative_Lru::Cache_Associative_Lru() {}

void Cache_Associative_Lru::set_data(map<string, string> map_data) {
	Cache::set_data(map_data);
	
	deque<string> cache_deque;
	this->cache = cache_deque;
	map<string, deque<string>::iterator> 
		addresses_in_cache_map;
	this->addresses_in_cache = addresses_in_cache_map;
}

void Cache_Associative_Lru::print_initialization_data() {
	Cache::print_initialization_data();
}

void Cache_Associative_Lru::procces_memory_address(string binary_address, 
	string hexa_address) {
	// get tag
	string tag = binary_address.substr(0, MEMORY_ADDRESS_SIZE - 
		this->offset_len);
	
	map<string, deque<string>::iterator>::iterator it;
	it = this->addresses_in_cache.find(tag);
	if (it != this->addresses_in_cache.end()) {
		// HIT -> elimino el bloque de la deque y lo vuelvo a encolar
		this->cache.erase(it->second);

		this->cache.push_front(tag);
		
		this->addresses_in_cache[tag] = this->cache.begin();
		
		this->hits += 1;
		
		if (this->debug_mode) {
			transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			cout << "Hit: " <<  hexa_address << endl;
		}
	} else {
		// MISS, primero verifico si esta lleno
		if (this->addresses_in_cache.size() == 
			(size_t)(this->size / this->block_size)) {
			// esta lleno. Saco el ultimo y pongo este
			string tag_popped = this->cache.back();
			this->cache.pop_back();
			this->addresses_in_cache.erase(tag_popped);
		}
		// agrego al final y agrego al map
		this->cache.push_front(tag);
		this->addresses_in_cache.insert(pair<string, 
			deque<string>::iterator>(tag, this->cache.end()));
		
		this->misses += 1;
		
		if (this->debug_mode) {
			transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			cout << "Miss: " <<  hexa_address << endl;
		}
	} 		
}

void Cache_Associative_Lru::print_informe() {
	Cache::print_informe();
}

Cache_Associative_Lru::~Cache_Associative_Lru() {}
