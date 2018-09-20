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

Cache::Cache() {}

void Cache::set_data(std::map<std::string, std::string> map_data) {
	this->hits = 0;
	this->misses = 0;
	
	// default values, en el caso de que no sean especificados
	this->fabricante = "N/A";
	this->modelo = "N/A";
	this->cpu_mhz = "N/A";
	this->debug_mode = false;
	
	for (std::map<std::string, std::string>::iterator it = map_data.begin(); 
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
	std::cout << "# Cache Inicializada" << std::endl;
	std::cout << std::endl;
	
	std::cout << "* Fabricante: " << this->fabricante << std::endl;
	std::cout << "* Modelo: " << this->modelo << std::endl;
	std::cout << "* Cpu MHz: " << this->cpu_mhz << std::endl;
	std::cout << std::endl;
}

void Cache::print_informe() {
	std::cout << std::endl;
	std::cout << "# Informe" << std::endl;
	std::cout << std::endl;
	
	std::cout << "* Total de hits: " << this->hits << std::endl;
	std::cout << "* Total de misses: " << this->misses << std::endl;
}

Cache::~Cache() {}
