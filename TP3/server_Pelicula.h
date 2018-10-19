#ifndef SERVER_PELICULA_H
#define SERVER_PELICULA_H

#include <string>

class Pelicula {
	std::string titulo;
	std::string idioma;
	std::string edad;
	std::string genero;
	
	public:
		Pelicula(std::string& titulo, std::string& idioma, std::string& edad, 
			std::string& genero);
		const std::string& getTitulo() const;
		~Pelicula();	
};

#endif // SERVER_PELICULA_H
