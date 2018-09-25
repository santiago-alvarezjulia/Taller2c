#include "Thread.h"
#include "cache_protected.h"
#include "functor_cache.h"
#include <string>
#include <fstream>
#include <bitset>
#include <sstream>
#define MEMORY_ADDRESS_SIZE 32
#define ERROR 1
#define OK 0
using std::fstream;
using std::stringstream;
using std::hex;
using std::bitset;
using std::string;

FunctorCache::FunctorCache(CacheProtected& cache, const char* file_name) : 
	cache_protected(cache), filename(file_name) {}
	
void FunctorCache::run() {
	fstream cpu_file(this->filename, fstream::in | fstream::binary);
	string line_cpu_file;
	
	do {
		getline(cpu_file, line_cpu_file);
		if (line_cpu_file.empty()) {
			break;
		}
		
		// conversion de hexa a binario para almacenar en std::bitset
		std::stringstream ss;
		ss << hex << line_cpu_file;
		unsigned n;
		ss >> n;
		
		bitset<MEMORY_ADDRESS_SIZE> memory_address(n);
		
		string binary_address = memory_address.to_string();
		
		// delego en cache_protected verificar que sea una direccion valida
		int result = this->cache_protected
			.validate_memory_address_and_print_if_invalid(memory_address, 
			line_cpu_file);
		if (result == ERROR) {
			// la direccion es invalida -> termino el procesamiento 
			// del archivo
			cpu_file.close();
			return;
		}
		
		// delego en cache_protected el procesamiento de la direccion de memoria
		this->cache_protected.procces_memory_address(binary_address, 
			line_cpu_file);
	} while (!line_cpu_file.empty());
	
	cpu_file.close();
}

FunctorCache::~FunctorCache() {}

