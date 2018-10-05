#include "cache_protected.h"
#include "thread_cache.h"
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
#define DELIMITADOR '='
#define ERROR 1
#define OK 0
using std::fstream;
using std::map;
using std::cerr;
using std::endl;
using std::string;
using std::vector;


int main(int argc, char* argv[]) {
	// Abro el archivo de configuracion.
	fstream config_file((const char*)argv[1], fstream::in);
	if (config_file.fail()) {
		cerr << "Error al abrir el archivo" << endl;
		return ERROR;
	}
	
	
	// Parseo del archivo de configuracion. Guardo la info en un map.
	map<string, string> map_config_file;
	string key;
	string value;
	string cache_type;
	while (getline(config_file, key, DELIMITADOR) && getline(config_file, value)) {
		if (key == "cache type") {
			cache_type = value;
		}
		map_config_file.emplace(key, value);
	}
		
	CacheProtected cache_protected(cache_type, map_config_file);
	vector<ThreadCache> threads;
	
	// Cache protected imprime los datos de configuracion de cache
	cache_protected.print_initialization_data();
	
	// Agrego al vector un FunctorCache por archivo de direcciones de memoria
	for(int i = 2; i < argc; i++) {
		threads.emplace_back(ThreadCache(cache_protected, (const char*)argv[i]));
	}
	
	// Corro los hilos
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i].start();
	}
	
	// Espero a que termine la ejecucion de todos los hilos y libero recursos
	// de los hilos que esten en el heap.
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
	
	// Cache protected imprime el informe final
	cache_protected.print_after_processing();
	
	return OK;
}
