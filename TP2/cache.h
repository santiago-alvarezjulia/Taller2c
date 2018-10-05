#ifndef CACHE_H
#define CACHE_H

#include <map>
#include <string>
#include <bitset>
#define MEMORY_ADDRESS_SIZE 32

class Cache {
	protected:
		std::map<std::string, std::string> config_data;
		int hits;
		int misses;
		int size;
		int block_size;
		int offset_len;
		bool debug_mode;
	
	public:
		explicit Cache(const std::map<std::string, std::string>& map_data);
		void print_initialization_data();
		virtual void procces_memory_address(std::string& binary_adress, 
			std::string& hexa_address) = 0;
		void print_after_processing();
		virtual ~Cache();
};

#endif //CACHE_H
