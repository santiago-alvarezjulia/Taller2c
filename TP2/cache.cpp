#include "cache.h"
#include <string>
#include <map>
#include <iostream>
#include <cmath>
#include <bitset>
#include <algorithm>
#define MEMORY_ADDRESS_SIZE 32
#define ERROR 1
#define OK 0
using std::map;
using std::string;
using std::cout;
using std::endl;

Cache::Cache() {}

void Cache::set_data(map<string, string> map_data) {
	this->hits = 0;
	this->misses = 0;
	
	// default values, en el caso de que no sean especificados por map_data
	this->fabricante = "N/A";
	this->modelo = "N/A";
	this->cpu_mhz = "N/A";
	this->debug_mode = false;
	
	for (map<string, string>::iterator it = map_data.begin(); 
		it != map_data.end(); ++it) {
		if (it->first.compare("vendor_id") == 0) {
			this->fabricante = it->second;
		} else if (it->first.compare("model name") == 0) {
			this->modelo = it->second;
		} else if (it->first.compare("cpu MHz") == 0) {
			this->cpu_mhz = it->second;
		} else if (it->first.compare("cache size") == 0) {
			this->size = atoi(it->second.c_str());
		} else if (it->first.compare("line size") == 0) {
			this->block_size = atoi(it->second.c_str());
		} else if (it->first.compare("debug") == 0) {
			bool debug = false;
			if (it->second.compare("true") == 0) {
				debug = true;
			}
				
			this->debug_mode = debug;
		}
	}
	
	this->offset_len = log(this->block_size) / log(2);
}

void Cache::print_initialization_data() {
	cout << "# Cache Inicializada" << endl;
	cout << std::endl;
	
	cout << "* Fabricante: " << this->fabricante << endl;
	cout << "* Modelo: " << this->modelo << endl;
	cout << "* Cpu MHz: " << this->cpu_mhz << endl;
	cout << endl;
}

void Cache::print_informe() {
	cout << endl;
	cout << "# Informe" << endl;
	cout << endl;
	
	cout << "* Total de hits: " << this->hits << endl;
	cout << "* Total de misses: " << this->misses << endl;
}

Cache::~Cache() {}
