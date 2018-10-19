#ifndef SERVER_SALA_H
#define SERVER_SALA_H

#include <string>

class Sala {
	std::string id;
	std::string pantalla;
	int cantidad_filas;
	int cantidad_columnas;
	
	public:
		Sala(std::string& id, std::string& pantalla, std::string& capacidad);
		const std::string& getId() const;
		int getCantidadFilas() const;
		int getCantidadColumnas() const;
		~Sala();	
};

#endif // SERVER_SALA_H
