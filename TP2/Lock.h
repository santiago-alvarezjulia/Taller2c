#ifndef LOCK_H
#define LOCK_H

#include <mutex>

class Lock {
	std::mutex& m;
	Lock(const Lock&) = delete;
	Lock& operator = (const Lock&) = delete;
	Lock(Lock&&) = delete;
	Lock& operator = (Lock&&) = delete;
	
	public:
		explicit Lock(std::mutex& m);
		~Lock();
};

#endif // LOCK_H
