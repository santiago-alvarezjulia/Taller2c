#include "cache.h"
#include <string>
#include <map>
#include <iostream>
#include <cmath>
#include <bitset>
#include <algorithm>
#define MEMORY_ADDRESS_SIZE 32
#define DEFAULT_VALUE "N/A"
#define ERROR 1
#define OK 0
using std::map;
using std::cout;
using std::endl;
using std::string;

Cache::Cache(const map<string, string>& map_data) : config_data(map_data) {
	this->hits = 0;
	this->misses = 0;
	
	std::map<std::string, std::string>::iterator it = 
		this->config_data.find("cache size");
	this->size = atoi(it->second.c_str());
	
	it = this->config_data.find("line size");
	this->block_size = atoi(it->second.c_str());
			
			
	this->offset_len = log(this->block_size) / log(2);
	
	it = this->config_data.find("debug");
	bool debug = false; // default debug mode es false
	if (it != this->config_data.end()) {
		if (it->second.compare("true") == 0) {
			debug = true;
		}
	}
				
	this->debug_mode = debug;
}

void Cache::print_initialization_data() {
	cout << "# Cache Inicializada" << endl;
	cout << std::endl;
	
	
	std::map<std::string, std::string>::iterator it = 
		this->config_data.find("vendor_id");
	string vendor_id = DEFAULT_VALUE;
	if (it != this->config_data.end()) {
		vendor_id = it->second;
	}
	cout << "* Fabricante: " << vendor_id << endl;
	
	
	it = this->config_data.find("model name");
	string model_name = DEFAULT_VALUE;
	if (it != this->config_data.end()) {
		model_name = it->second;
	}
	cout << "* Modelo: " << model_name << endl;
	
	it = this->config_data.find("cpu MHz");
	string cpu_mhz = DEFAULT_VALUE;
	if (it != this->config_data.end()) {
		cpu_mhz = it->second;
	}
	cout << "* Cpu MHz: " << cpu_mhz << endl;
	
	cout << std::endl;
}

void Cache::print_after_processing() {
	cout << endl;
	cout << "# Informe" << endl;
	cout << endl;
	
	cout << "* Total de hits: " << this->hits << endl;
	cout << "* Total de misses: " << this->misses << endl;
}

Cache::~Cache() {}
