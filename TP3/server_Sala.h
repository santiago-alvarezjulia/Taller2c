#ifndef SERVER_SALA_H
#define SERVER_SALA_H

#include <string>

class Sala {
	std::string id;
	std::string pantalla;
	std::string capacidad;
	
	public:
		Sala(std::string id, std::string pantalla, 
			std::string capacidad);
		std::string getCapacidad();
		~Sala();	
};

#endif // SERVER_SALA_H
