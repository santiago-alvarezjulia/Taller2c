#ifndef SERVER_FUNCION_H
#define SERVER_FUNCION_H

#include <string>
#include <vector>

class Funcion {
	std::string id;
	std::string id_sala;
	std::string titulo;
	std::string fecha;
	std::string hora;
	std::vector<std::vector<char>> asientos;
	int cantidad_filas;
	int cantidad_columnas;
	int asientos_ocupados;
	int cantidad_total_asientos;
	int fila_to_number(std::string fila);
	
	public:
		Funcion(std::string id, std::string id_sala, std::string titulo, 
			std::string fecha, std::string hora, std::string capacidad);
		std::string getTitulo();
		int getCantidadFilas();
		int getCantidadColumnas();
		std::vector<std::vector<char>> getAsientos();
		bool esta_agotada();
		bool reservarAsiento(std::string fila, std::string columna);
		~Funcion();	
};

#endif // SERVER_FUNCION_H
