#include "server_End_App.h"
#include <iostream>
#define END_APP 'q'
using std::cin;
using std::cout;
using std::endl;

End_App::End_App() {
	this->is_alive = true;
}

void End_App::run() {
	char input;
	while (true) {
		input = cin.get();
		if (input == END_APP) {
			break;
		}
	}
	this->is_alive = false;
}

bool End_App::isAlive() {
	return this->is_alive;
}

End_App::~End_App() {}
