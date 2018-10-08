#include "client_Client.h"
#include <iostream>
#include <string>
#include <cstring>
#define CHAR_INITIALIZE_STRING '0'
#define FIN_RECIBO_SOCKET 0
#define FUNCTION_GENERO '1'
#define FUNCTION_IDIOMA '2'
#define FUNCTION_EDAD '3'
using std::string;
using std::cout;
using std::cerr;
using std::endl;

Client::Client(Socket& socket_client, char* hostname, char* port) : 
	socket(std::move(socket_client)) {
	this->socket.connect_(hostname, port); // puede lanzar SocketError
}

void Client::recibo_idioma_edad_genero(unsigned char function, string data) {
	// envio la function
	this->socket.send_(&function, sizeof(unsigned char));
	
	// envio la data
	unsigned int len_data = data.size();
	this->socket.send_((unsigned char*)&len_data, sizeof(unsigned int));
	this->socket.send_((unsigned char*)data.c_str(), len_data);
	
	// recibo la validez de la operacion
	unsigned int validez_operacion;
	this->socket.receive_((unsigned char*)&validez_operacion, 
		sizeof(unsigned int));
		
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
	
	// recibo secuencialmente la longitud del titulo y despues el titulo de 
	// todas la peliculas que cumplen con data
	unsigned int len_titulo;
	this->socket.receive_((unsigned char*)&len_titulo, sizeof(unsigned int));
	while (len_titulo != FIN_RECIBO_SOCKET) {
		string titulo(len_titulo, CHAR_INITIALIZE_STRING);
		this->socket.receive_((unsigned char*) titulo.c_str(), len_titulo);
		
		cout << titulo << endl;
		
		this->socket.receive_((unsigned char*)&len_titulo, 
			sizeof(unsigned int));
	}
}

void Client::funciones_del_dia(unsigned char function, string fecha) {
	// envio la function
	this->socket.send_(&function, sizeof(unsigned char));
	
	// envio la fecha
	unsigned int len_fecha = fecha.size();
	this->socket.send_((unsigned char*)&len_fecha, sizeof(unsigned int));
	this->socket.send_((unsigned char*)fecha.c_str(), len_fecha);
	
	// recibo en el ciclo while toda la info de la funcion: su id, el titulo
	// de la pelicula, el id de la sala, la hora y el estado de los asientos.
	unsigned int len_id_funcion;
	this->socket.receive_((unsigned char*)&len_id_funcion, 
		sizeof(unsigned int));
	while (len_id_funcion != FIN_RECIBO_SOCKET) {
		// recibo el id_funcion
		string id_funcion(len_id_funcion, CHAR_INITIALIZE_STRING);
		this->socket.receive_((unsigned char*)id_funcion.c_str(), 
			len_id_funcion);
		
		// recibo la longitud del titulo y el titulo
		unsigned int len_titulo;
		this->socket.receive_((unsigned char*)&len_titulo, 
			sizeof(unsigned int));
		string titulo(len_titulo, CHAR_INITIALIZE_STRING);
		this->socket.receive_((unsigned char*)titulo.c_str(), len_titulo);
		
		// recibo la longitud del id de la sala y el id de la sala
		unsigned int len_id_sala;
		this->socket.receive_((unsigned char*)&len_id_sala, 
			sizeof(unsigned int));
		string id_sala(len_id_sala, CHAR_INITIALIZE_STRING);
		this->socket.receive_((unsigned char*)id_sala.c_str(), len_id_sala);
		
		// recibo la longitud de la hora y la hora
		unsigned int len_hora;
		this->socket.receive_((unsigned char*)&len_hora, sizeof(unsigned int));
		string hora(len_hora, CHAR_INITIALIZE_STRING);
		this->socket.receive_((unsigned char*)hora.c_str(), len_hora);
		
		// recibo la longitud del estado de los asientos y el 
		// estado de los asientos
		unsigned int len_estado_asientos_funcion;
		this->socket.receive_((unsigned char*)&len_estado_asientos_funcion, 
			sizeof(unsigned int));
		string estado_asientos_funcion(len_estado_asientos_funcion, 
			CHAR_INITIALIZE_STRING);
		this->socket.receive_((unsigned char*)estado_asientos_funcion.c_str(), 
			len_estado_asientos_funcion);
		
		// si el estado de los asientos dice que esta agotada, agrego al final
		// el texto AGOTADA
		cout << id_funcion << ": <Funcion para \"" << titulo 
			<< "\" en la sala " << id_sala << " con fecha " << fecha << " - " 
			<< hora << ">";	
		
		if (estado_asientos_funcion == "AGOTADA") {
			cout << " " << estado_asientos_funcion;
		}
		cout << endl;
		
		// recibo el id_funcion -> si es 0, fin dela ejecucion del comando
		this->socket.receive_((unsigned char*)&len_id_funcion, 
			sizeof(unsigned int));
	}
}

void Client::asientos_funcion(unsigned char function, string id_funcion) {
	// envio la function
	this->socket.send_(&function, sizeof(unsigned char));
	
	// envio el id_funcion
	unsigned int len_id_funcion = id_funcion.size();
	this->socket.send_((unsigned char*)&len_id_funcion, sizeof(unsigned int));
	this->socket.send_((unsigned char*)id_funcion.c_str(), len_id_funcion);
	
	// recibo lo mismo que en Client::funciones_del_dia pero solo para 
	// id_funcion y ademas la fecha de la funcion
	// recibo la longitud del titulo y el titulo
	unsigned int len_titulo;
	this->socket.receive_((unsigned char*)&len_titulo, 
		sizeof(unsigned int));
	string titulo(len_titulo, CHAR_INITIALIZE_STRING);
	this->socket.receive_((unsigned char*)titulo.c_str(), len_titulo);
	
	// recibo la longitud del id de la sala y el id de la sala
	unsigned int len_id_sala;
	this->socket.receive_((unsigned char*)&len_id_sala, 
		sizeof(unsigned int));
	string id_sala(len_id_sala, CHAR_INITIALIZE_STRING);
	this->socket.receive_((unsigned char*)id_sala.c_str(), len_id_sala);
	
	// recibo la longitud de la fecha y la fecha
	unsigned int len_fecha;
	this->socket.receive_((unsigned char*)&len_fecha, sizeof(unsigned int));
	string fecha(len_fecha, CHAR_INITIALIZE_STRING);
	this->socket.receive_((unsigned char*)fecha.c_str(), len_fecha);
	
	// recibo la longitud de la hora y la hora
	unsigned int len_hora;
	this->socket.receive_((unsigned char*)&len_hora, sizeof(unsigned int));
	string hora(len_hora, CHAR_INITIALIZE_STRING);
	this->socket.receive_((unsigned char*)hora.c_str(), len_hora);
		
	// recibo la longitud del estado de los asientos y el 
	// estado de los asientos
	unsigned int len_estado_asientos_funcion;
	this->socket.receive_((unsigned char*)&len_estado_asientos_funcion, 
		sizeof(unsigned int));
	string estado_asientos_funcion(len_estado_asientos_funcion, 
		CHAR_INITIALIZE_STRING);
	this->socket.receive_((unsigned char*)estado_asientos_funcion.c_str(), 
		len_estado_asientos_funcion);
		
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
	unsigned int cantidad_columnas;
	this->socket.receive_((unsigned char*)&cantidad_columnas, 
		sizeof(unsigned int));
	for (unsigned int i = 0; i < cantidad_columnas; i++) {
		cout << '\t' << i+1;
	}
	cout << endl;
	
	// recibo la cantidad de filas a imprimir
	unsigned int cantidad_filas;
	this->socket.receive_((unsigned char*)&cantidad_filas, 
		sizeof(unsigned int));
		
	// recibo las filas dentro del ciclo
	for (unsigned int i = 0; i < cantidad_filas; i++) {
		// recibo la fila
		unsigned int len_fila;
		this->socket.receive_((unsigned char*)&len_fila, 
			sizeof(unsigned int));
		string fila(len_fila, CHAR_INITIALIZE_STRING);
		this->socket.receive_((unsigned char*)fila.c_str(), len_fila);
		
		cout << fila << endl;
	}
}

void Client::reservar_asiento(unsigned char function, string id_funcion, 
	string fila, string columna) {
	// envio la function
	this->socket.send_(&function, sizeof(unsigned char));
	
	// envio el id_funcion
	unsigned int len_id_funcion = id_funcion.size();
	this->socket.send_((unsigned char*)&len_id_funcion, sizeof(unsigned int));
	this->socket.send_((unsigned char*)id_funcion.c_str(), len_id_funcion);
	
	// envio la fila
	unsigned int len_fila = fila.size();
	this->socket.send_((unsigned char*)&len_fila, sizeof(unsigned int));
	this->socket.send_((unsigned char*)fila.c_str(), len_fila);
	
	// envio la columna
	unsigned int len_columna = columna.size();
	this->socket.send_((unsigned char*)&len_columna, sizeof(unsigned int));
	this->socket.send_((unsigned char*)columna.c_str(), len_columna);
	
	// recibo el estado_reserva. (0 si fue reservado el asiento correctamente, 
	// 1 en caso contrario)
	unsigned int estado_reserva;
	this->socket.receive_((unsigned char*)&estado_reserva, 
		sizeof(unsigned int));
		
	if (estado_reserva == 0) {
		cout << "OK" << endl;
	} else {
		cout << "ERROR: El asiento ya está reservado" << endl;
	}
}


Client::~Client() {
	this->socket.shutdown_rw();
}
