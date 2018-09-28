#ifndef SERVER_SALA_CHICA_H
#define SERVER_SALA_CHICA_H

#include "server_Sala.h"
#include <string>
#include <array>
#define FILAS 5
#define COLUMNAS 6

class Sala_Chica : Sala {
	std::string id;
	std::string pantalla;
	std::string capacidad;
	std::array<std::array<char, COLUMNAS>, FILAS> asientos;
	
	public:
		Sala_Chica();
		virtual void set_data_sala(std::string id, std::string pantalla, 
			std::string capacidad);
		std::string getCapacidad();
		~Sala_Chica();	

};

#endif // SERVER_SALA_CHICA_H
