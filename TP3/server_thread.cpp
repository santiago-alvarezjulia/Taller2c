#include "server_thread.h"

Thread::Thread(Thread&& other) {
	this->thread = std::move(other.thread);
}

Thread& Thread::operator=(Thread&& other) {
	this->thread = std::move(other.thread);
	return *this;
}

void Thread::start() {
	this->thread = std::thread(&Thread::run, this);
}

bool Thread::joinable() {
	return this->thread.joinable();
}

void Thread::join() {
	this->thread.join();
}
