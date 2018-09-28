#ifndef SERVER_SALA_GRANDE_H
#define SERVER_SALA_GRANDE_H

#include "server_Sala.h"
#include <string>
#include <array>
#define FILAS 15
#define COLUMNAS 16

class Sala_Grande : Sala {
	std::string id;
	std::string pantalla;
	std::string capacidad;
	std::array<std::array<char, COLUMNAS>, FILAS> asientos;
	
	public:
		Sala_Grande();
		virtual void set_data_sala(std::string id, std::string pantalla, 
			std::string capacidad);
		std::string getCapacidad();
		~Sala_Grande();	

};

#endif // SERVER_SALA_GRANDE_H
