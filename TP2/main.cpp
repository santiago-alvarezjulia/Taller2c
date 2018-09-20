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
using std::fstream;
using std::map;
using std::cerr;
using std::endl;
using std::string;
using std::size_t;
using std::pair;
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
	string line_config_file;
	string cache_type;
	do {
		getline(config_file, line_config_file);
		if (line_config_file.empty()) {
			break;
		}
		
		size_t pos_delimitador = line_config_file.find(DELIMITADOR);
		string key = line_config_file.substr(0, pos_delimitador);
		//el 1 es el largo del delim
		string value = line_config_file.substr(pos_delimitador + 1); 
		
		if (key == "cache type") {
			cache_type = value;
		}
		
		map_config_file.insert(std::pair<string, string>(key, value));
	} while (!line_config_file.empty());
		
	// El tipo de Cache depende de lo que diga la clave 'cache type' en el 
	// archivo de configuracion, hay 3 tipos posibles.
	Cache* cache;
	if (cache_type == "associative-lru") {
		cache = new Cache_Associative_Lru ();
	} else if (cache_type == "direct") {
		cache = new Cache_Direct();
	} else {
		cache = new Cache_Associative_Fifo();
	}
	
	CacheProtected cache_protected(cache);
	vector<Thread*> threads;
	
	// Cache protected inicializa la informacion que parsee del archivo de 
	// configuracion. Luego la imprime en std::cout
	cache_protected.set_data(map_config_file);
	cache_protected.print_initialization_data();
	
	// Agrego al vector un FunctorCache por archivo de direcciones de memoria
	for(int i = 2; i < argc; i++) {
		threads.push_back(new FunctorCache(cache_protected, 
			(const char*)argv[i]));
	}
	
	// Corro los hilos
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i]->start();
	}
	
	// Espero a que termine la ejecucion de todos los hilos y libero recursos
	// de los hilos que esten en el heap.
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i]->join();
		delete threads[i];
	}
	
	// Cache protected imprime el informe final
	cache_protected.print_informe();
	
	// Cierro archivos y libero recursos del heap
	config_file.close();
	delete cache;
	
	return OK;
}
