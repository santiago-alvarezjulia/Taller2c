#ifndef END_APP_H
#define END_APP_H

#include "common_thread.h"

class End_App : public Thread {
	bool is_alive;
	
	public:
		End_App();
		virtual void run();
		bool isAlive();
		~End_App();
};

#endif // END_APP_H
