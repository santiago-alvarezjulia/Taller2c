#ifndef SERVER_FUNCION_PROTECTED_H
#define SERVER_FUNCION_PROTECTED_H

#include "server_Funcion.h"
#include "common_socket.h"
#include <map>
#include <string>
#include <mutex>

class FuncionesProtected {
	std::map<std::string, Funcion> funciones;
	std::map<std::string, std::mutex*> mutex_funciones;
	
	public:
		FuncionesProtected();
		void agregar_funcion(std::string& id_funcion, Funcion& funcion);
		std::map<std::string, Funcion> send_funciones_dia(std::string& fecha);
		bool reservar_asiento(std::string& id_funcion, std::string& fila,
			std::string& columna);
		Funcion asientos(std::string& id_funcion); 
		~FuncionesProtected();
};

#endif // SERVER_FUNCION_PROTECTED_H
