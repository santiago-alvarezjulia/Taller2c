#include "client_Client.h"
#include "common_Protocolo.h"
#include <iostream>
#include <string>
#include <cstring>
#define FUNCTION_GENERO '1'
#define FUNCTION_IDIOMA '2'
#define FUNCTION_EDAD '3'
using std::string;
using std::cout;
using std::cerr;
using std::endl;

Client::Client(char* hostname, char* port) : 
	protocolo(Socket(), hostname, port) {}

void Client::recibo_idioma_edad_genero(unsigned char function, string& data) {
	// envio la function
	this->protocolo.send_unsigned_char(function);
	
	// envio la data
	this->protocolo.send_string(data);
	
	// recibo la validez de la operacion
	unsigned int validez_operacion = this->protocolo.receive_unsigned_int();
		
	if (validez_operacion == 1) {
		// operacion inválida
		// me fijo en function para saber cual de las 3 funciones estoy haciendo
		if (function == FUNCTION_GENERO) {
			cerr << "Genero no reconocido" << endl;
		} else if (function == FUNCTION_IDIOMA) {
			cerr << "Idioma no reconocido" << endl;
		} else {
			cerr << "Edad no reconocida" << endl;
		}
		
		return;
	}
	
	// recibo secuencialmente los titulos de las peliculas que cumplen
	string titulo = this->protocolo.receive_string();
	while (!titulo.empty()) {
		cout << titulo << endl;
		
		titulo = this->protocolo.receive_string();
	}
}

void Client::funciones_del_dia(unsigned char function, string& fecha) {
	// envio la function
	this->protocolo.send_unsigned_char(function);
	
	// envio la fecha
	this->protocolo.send_string(fecha);
	
	// recibo en el ciclo while toda la info de la funcion: su id, el titulo
	// de la pelicula, el id de la sala, la hora y el estado de los asientos.
	string id_funcion = this->protocolo.receive_string();
	while (!id_funcion.empty()) {
		// recibo  el titulo
		string titulo = this->protocolo.receive_string();
		
		// recibo la sala
		string id_sala = this->protocolo.receive_string();
		
		// recibo la hora
		string hora = this->protocolo.receive_string();
		
		// recibo el estado de los asientos
		string estado_asientos_funcion = this->protocolo.receive_string();
		
		// si el estado de los asientos dice que esta agotada, agrego al final
		// el texto AGOTADA
		cout << id_funcion << ": <Funcion para \"" << titulo 
			<< "\" en la sala " << id_sala << " con fecha " << fecha << " - " 
			<< hora << ">";	
		
		if (estado_asientos_funcion == "AGOTADA") {
			cout << " " << estado_asientos_funcion;
		}
		cout << endl;
		
		// recibo el id_funcion -> si esta vacio, fin de la ejecucion 
		// del comando
		id_funcion = this->protocolo.receive_string();
	}
}

void Client::asientos_funcion(unsigned char function, string& id_funcion) {
	// envio la function
	this->protocolo.send_unsigned_char(function);
	
	// envio el id_funcion
	this->protocolo.send_string(id_funcion);
	
	// recibo lo mismo que en Client::funciones_del_dia pero solo para 
	// id_funcion y ademas la fecha de la funcion
	// recibo  el titulo
	string titulo = this->protocolo.receive_string();
		
	// recibo la sala
	string id_sala = this->protocolo.receive_string();
	
	// recibo la longitud de la fecha y la fecha
	string fecha = this->protocolo.receive_string();
	
	// recibo la longitud de la hora y la hora
	string hora = this->protocolo.receive_string();
		
	// recibo la longitud del estado de los asientos y el 
	// estado de los asientos
	string estado_asientos_funcion = this->protocolo.receive_string();
		
	// si el estado de los asientos dice que esta agotada, agrego al final
	// el texto AGOTADA
	cout << id_funcion << ": <Funcion para \"" << titulo 
	<< "\" en la sala " << id_sala << " con fecha " << fecha << " - " 
	<< hora << ">";
	
	if (estado_asientos_funcion == "AGOTADA") {
		cout << " " << estado_asientos_funcion;
	}
	cout << endl;
		
	// imprimo por cout "Asientos:" para cumplir con prints de catedra
	cout << "Asientos:" << endl;
	 
	// recibo la cantidad de columnas a imprimir
	unsigned int cantidad_columnas = this->protocolo.receive_unsigned_int();
	for (unsigned int i = 0; i < cantidad_columnas; i++) {
		cout << '\t' << i+1;
	}
	cout << endl;
	
	// recibo la cantidad de filas a imprimir
	unsigned int cantidad_filas = this->protocolo.receive_unsigned_int();
		
	// recibo las filas dentro del ciclo
	for (unsigned int i = 0; i < cantidad_filas; i++) {
		// recibo la fila
		string fila = this->protocolo.receive_string();
		
		cout << fila << endl;
	}
}

void Client::reservar_asiento(unsigned char function, string& id_funcion, 
	string& fila, string& columna) {
	// envio la function
	this->protocolo.send_unsigned_char(function);
	
	// envio el id_funcion
	this->protocolo.send_string(id_funcion);
	
	// envio la fila
	this->protocolo.send_string(fila);
	
	// envio la columna
	this->protocolo.send_string(columna);
	
	// recibo el estado_reserva. (0 si fue reservado el asiento correctamente, 
	// 1 en caso contrario)
	unsigned int estado_reserva = this->protocolo.receive_unsigned_int();
		
	if (estado_reserva == 0) {
		cout << "OK" << endl;
	} else {
		cout << "ERROR: El asiento ya está reservado" << endl;
	}
}


Client::~Client() {
	//this->socket.shutdown_rw();
}
