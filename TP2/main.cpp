#include "cache_direct.h"
#include "cache_associative_fifo.h"
#include "cache_associative_lru.h"
#include "cache_protected.h"
#include "functor_cache.h"
#include "cache.h"
#include "Thread.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <bitset>
#include <sstream>
#include <utility>
#include <vector>
#define MEMORY_ADDRESS_SIZE 32
#define DELIMITADOR "="
#define ERROR 1
#define OK 0


int main(int argc, char* argv[]) {
	std::fstream config_file((const char*)argv[1], std::fstream::in);
	if (config_file.fail()) {
		std::cerr << "Error al abrir el archivo" << std::endl;
		return ERROR;
	}
	
	std::map<std::string, std::string> map_config_file;
	std::string line_config_file;
	std::string cache_type;
	do {
		getline(config_file, line_config_file);
		if (line_config_file.empty()) {
			break;
		}
		
		std::size_t pos_delimitador = line_config_file.find(DELIMITADOR);
		std::string key = line_config_file.substr(0, pos_delimitador);
		//el 1 es el largo del delim
		std::string value = line_config_file.substr(pos_delimitador + 1); 
		
		if (key == "cache type") {
			cache_type = value;
		}
		
		map_config_file.insert(std::pair<std::string, std::string>(key, value));
	} while (!line_config_file.empty());
		
	Cache* cache;
	if (cache_type == "associative-lru") {
		cache = new Cache_Associative_Lru ();
	} else if (cache_type == "direct") {
		cache = new Cache_Direct();
	} else {
		cache = new Cache_Associative_Fifo();
	}
	
	CacheProtected cache_protected(cache);
	std::vector<Thread*> threads;
	
	cache_protected.set_data(map_config_file);
	cache_protected.print_initialization_data();
	
	for(int i = 2; i < argc; i++) {
		threads.push_back(new FunctorCache(cache_protected, 
			(const char*)argv[i]));
	}
	
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i]->start();
	}
	
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i]->join();
		delete threads[i];
	}
	
	cache_protected.print_informe();
	
	config_file.close();
	delete cache;
	
	return OK;
}
