#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include <thread>
 
class Thread {
    std::thread thread;
 
    public:
		Thread() {}
		void start();
		void join();
		bool joinable();

		virtual void run() = 0;
		virtual ~Thread() {}

		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;

		Thread(Thread&& other);
		Thread& operator=(Thread&& other);
};

#endif // SERVER_THREAD_H
