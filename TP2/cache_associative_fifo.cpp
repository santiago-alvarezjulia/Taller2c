#include "cache_associative_fifo.h"
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
using std::map;
using std::string;
using std::deque;
using std::transform;
using std::cout;
using std::endl;
using std::pair;

Cache_Associative_Fifo::Cache_Associative_Fifo() {}

void Cache_Associative_Fifo::set_data(map<string, string> map_data) {
	Cache::set_data(map_data);
	
	deque<string> cache_deque;
	this->cache = cache_deque;
	map<string, bool> addresses_in_cache_map;
	this->addresses_in_cache = addresses_in_cache_map;
}

void Cache_Associative_Fifo::print_initialization_data() {
	Cache::print_initialization_data();
}

void Cache_Associative_Fifo::procces_memory_address(string binary_address, 
	string hexa_address) {
	string tag = binary_address.substr(0, MEMORY_ADDRESS_SIZE - 
			this->offset_len);
			
	if (this->addresses_in_cache.find(tag) != this->addresses_in_cache.end()) {
		// HIT
		this->hits += 1;
		
		if (this->debug_mode) {
			transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			cout << "Hit: " <<  hexa_address << endl;
		}
	} else {
		// MISS, primero verifico si esta lleno
		// .size() es O(1) -> aclararlo en informe
		if (this->addresses_in_cache.size() == (size_t)(this->size / 
			this->block_size)) {
			// esta lleno. Saco el ultimo y pongo este
			string tag_popped = this->cache.front();
			this->cache.pop_front();
			this->addresses_in_cache.erase(tag_popped);
			
			this->cache.push_back(tag);
			this->addresses_in_cache.insert(pair<string, bool> (tag, true));
		} else {
			// no esta lleno
			this->cache.push_back(tag);
			this->addresses_in_cache.insert(pair<std::string, bool> 
				(tag, true));
		}
		
		this->misses += 1;
		
		if (this->debug_mode) {
			transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			cout << "Miss: " <<  hexa_address << endl;
		}
	}		
}

void Cache_Associative_Fifo::print_informe() {
	Cache::print_informe();
}

Cache_Associative_Fifo::~Cache_Associative_Fifo() {}
