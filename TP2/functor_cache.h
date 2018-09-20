#ifndef FUNCTOR_CACHE_H
#define FUNCTOR_CACHE_H

#include "Thread.h"
#include "cache_protected.h"

class FunctorCache: public Thread {
	CacheProtected& cache_protected;
	const char* filename;
	
	public :
		FunctorCache(CacheProtected& cache, const char* file_name);
		virtual void run();
		~FunctorCache();
};

#endif //FUNCTOR_CACHE_H
