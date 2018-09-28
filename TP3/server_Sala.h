#ifndef SERVER_SALA_H
#define SERVER_SALA_H

#include <string>
#include <vector>

class Sala {
	std::string id;
	std::string pantalla;
	std::string capacidad;
	std::vector<std::vector<char>> asientos;
	int cantidad_filas;
	int cantidad_columnas;
	
	
	public:
		Sala(std::string id, std::string pantalla, 
			std::string capacidad);
		~Sala();	

};

#endif // SERVER_SALA_H
