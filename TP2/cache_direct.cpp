#include "cache_direct.h"
#include <string>
#include <map>
#include <iostream>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <utility>
#define MEMORY_ADDRESS_SIZE 32
#define ERROR 1
#define OK 0

Cache_Direct::Cache_Direct() {}

void Cache_Direct::set_data(std::map<std::string, std::string> map_data) {
	Cache::set_data(map_data);
	
	std::map<std::string, std::string> cache_map;
	this->cache = cache_map;
	this->index_len = log((this->size / this->block_size)) / log(2);
}

void Cache_Direct::print_initialization_data() {
	Cache::print_initialization_data();
}

int Cache_Direct::validate_memory_address(
	std::bitset<MEMORY_ADDRESS_SIZE> memory_address, std::string hexa_address) {	
	return Cache::validate_memory_address(memory_address, hexa_address);
}

void Cache_Direct::procces_memory_address(std::string binary_address, 
	std::string hexa_address) {
	std::string index_hexa = binary_address.substr(MEMORY_ADDRESS_SIZE - 
		this->offset_len - this->index_len, this->index_len);
	
	int index = std::stoi(index_hexa, nullptr, 2);
	
	std::string tag = binary_address.substr(0, MEMORY_ADDRESS_SIZE - 
		this->offset_len);
	

	std::map<std::string, std::string>::iterator it;
	it = this->cache.find(std::to_string(index));
	
	if (it == this->cache.end()) {
		// el bloque esta vacio -> miss. Ademas borro el bitset anterior y agrego
		// el nuevo en ese index
		
		this->cache.insert(std::pair<std::string, std::string> 
			(std::to_string(index), tag));
			
		this->misses += 1;
		
		if (this->debug_mode) {
			std::transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			std::cout << "Miss: " <<  hexa_address << std::endl;
		}
	} else {
		// el bloque no esta vacio. Puede ser el mismo (hit) o un bloque distinto 
		// (miss y reemplazo el bloque). Comparo los tags
		//std::cout << "address almacenado: " << it->second << std::endl;
		bool same_block = false;
		if (tag == it->second) {
			same_block = true;
		}
		
		if (same_block) {
			// HIT
			
			this->hits += 1;
			
			if (this->debug_mode) {
				std::transform(hexa_address.begin(), hexa_address.end(), 
					hexa_address.begin(), ::tolower);
				std::cout << "Hit: " <<  hexa_address << std::endl;
			}
		} else {
			// MISS
			
			this->cache.erase(std::to_string(index));
			this->cache.insert(std::pair<std::string, std::string> 
				(std::to_string(index), tag));
			this->misses += 1;
			
			if (this->debug_mode) {
				std::transform(hexa_address.begin(), hexa_address.end(), 
					hexa_address.begin(), ::tolower);
				std::cout << "Miss: " <<  hexa_address << std::endl;
			}
		}
	}
}



void Cache_Direct::print_informe() {
	Cache::print_informe();
}

Cache_Direct::~Cache_Direct() {}
