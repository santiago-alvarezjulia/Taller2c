#include "cache_associative_lru.h"
#include "cache.h"
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

Cache_Associative_Lru::Cache_Associative_Lru(const map<string, 
	string>& map_data) : Cache(map_data) {
	this->cache = deque<string>();
	this->addresses_in_cache = map<string, bool>();
}

void Cache_Associative_Lru::procces_memory_address(string& binary_address, 
	string& hexa_address) {
	// get tag
	string tag = binary_address.substr(0, MEMORY_ADDRESS_SIZE - 
			this->offset_len);
				
	if (this->addresses_in_cache.find(tag) != this->addresses_in_cache.end()) {
		// HIT
		// elimino el tag de la deque
		deque<string>::iterator it;
		for (it = this->cache.begin(); it != this->cache.end(); it++) {
			if (*it == tag) {
				this->cache.erase(it);
				break;
			}
		}
		// vuelvo a encolar el tag eliminado
		this->cache.push_back(tag);
		
		this->hits += 1;
		
		// si esta en modo debug imprimo el HIT
		if (this->debug_mode) {
			transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			cout << "Hit: " <<  hexa_address << endl;
		}
	} else {
		// MISS
		// verifico si esta lleno
		if (this->addresses_in_cache.size() == 
			(size_t)(this->size / this->block_size)) {
			// esta lleno. Saco el ultimo del deque y del map 
			string tag_popped = this->cache.front();
			this->cache.pop_front();
			this->addresses_in_cache.erase(tag_popped);
			
			// encolo el tag nuevo en el deque y lo agrego al map
			this->cache.push_back(tag);
			this->addresses_in_cache.emplace(tag, true);
		} else {
			// no esta lleno, simplemente encolo el tag nuevo
			this->cache.push_back(tag);
			this->addresses_in_cache.emplace(tag, true);
		}
		
		this->misses += 1;
		
		// si esta en modo debug imprimo el MISS
		if (this->debug_mode) {
			transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			cout << "Miss: " <<  hexa_address << endl;
		}
	}			
}

Cache_Associative_Lru::~Cache_Associative_Lru() {}
