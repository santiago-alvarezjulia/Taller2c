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
using std::map;
using std::cout;
using std::endl;
using std::string;
using std::stoi;
using std::to_string;
using std::pair;

Cache_Direct::Cache_Direct(const map<string, string>& map_data) : Cache(map_data) {
	this->cache = map<string, string>();
	this->index_len = log((this->size / this->block_size)) / log(2);
}

void Cache_Direct::procces_memory_address(string& binary_address, 
	string& hexa_address) {
	// calculo el indice a partir de la binary_address. 
	string index_hexa = binary_address.substr(MEMORY_ADDRESS_SIZE - 
		this->offset_len - this->index_len, this->index_len);
	int index = stoi(index_hexa, nullptr, 2);
	
	// separo el tag de la binary_address
	string tag = binary_address.substr(0, MEMORY_ADDRESS_SIZE - 
		this->offset_len);
	
	// me fijo si el bloque esta ocupado.
	map<string, string>::iterator it;
	it = this->cache.find(to_string(index));
	if (it == this->cache.end()) {
		// el bloque esta vacio -> miss. Agrego el nuevo en ese index
		this->cache.insert(pair<string, string>(to_string(index), tag));
			
		this->misses += 1;
		
		if (this->debug_mode) {
			transform(hexa_address.begin(), hexa_address.end(), 
				hexa_address.begin(), ::tolower);
			cout << "Miss: " <<  hexa_address << endl;
		}
	} else {
		// el bloque no esta vacio. Puede ser el mismo (hit) o un bloque 
		// distinto (miss y reemplazo el bloque). Comparo los tags
		bool same_block = false;
		if (tag == it->second) {
			same_block = true;
		}
		
		if (same_block) {
			// El bloque es el mismo -> hit
			this->hits += 1;
			
			if (this->debug_mode) {
				transform(hexa_address.begin(), hexa_address.end(), 
					hexa_address.begin(), ::tolower);
				cout << "Hit: " <<  hexa_address << endl;
			}
		} else {
			// El bloque es distinto -> miss. Elimino el anterior y 
			// agrego el nuevo
			this->cache.erase(to_string(index));
			this->cache.insert(pair<string, string>(to_string(index), tag));
			this->misses += 1;
			
			if (this->debug_mode) {
				transform(hexa_address.begin(), hexa_address.end(), 
					hexa_address.begin(), ::tolower);
				cout << "Miss: " <<  hexa_address << endl;
			}
		}
	}
}

Cache_Direct::~Cache_Direct() {}
