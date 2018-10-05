#ifndef THREAD_CACHE_H
#define THREAD_CACHE_H

#include "Thread.h"
#include "cache_protected.h"

#include <fstream>

class ThreadCache: public Thread {
	CacheProtected& cache_protected;
	std::fstream archivo_direcciones;
	
	public :
		ThreadCache(CacheProtected& cache, const char* file_name);
		virtual void run();
		ThreadCache(ThreadCache&& other);
		~ThreadCache();
};

#endif //THREAD_CACHE_H
