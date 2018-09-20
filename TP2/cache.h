#ifndef CACHE_H
#define CACHE_H

#include <map>
#include <string>
#include <bitset>
#define MEMORY_ADDRESS_SIZE 32

class Cache {
	protected:
		std::string fabricante;
		std::string modelo;
		std::string cpu_mhz;
		int hits;
		int misses;
		int size;
		int block_size;
		bool debug_mode;
		int offset_len;
	
	public:
		Cache();
		virtual void set_data(std::map<std::string, std::string> map_data);
		virtual void print_initialization_data();
		virtual void procces_memory_address(std::string binary_adress, 
			std::string hexa_address) = 0;
		virtual void print_informe();
		virtual ~Cache();
};

#endif //CACHE_H
