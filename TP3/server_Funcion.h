#ifndef SERVER_FUNCION_H
#define SERVER_FUNCION_H

#include <string>

class Funcion {
	std::string id;
	std::string id_sala;
	std::string titulo;
	std::string fecha;
	std::string hora;
	
	public:
		Funcion(std::string id, std::string id_sala, std::string titulo, 
			std::string fecha, std::string hora);
		std::string getTitulo();
		~Funcion();	

};

#endif // SERVER_FUNCION_H
