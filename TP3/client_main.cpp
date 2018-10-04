#include "common_socket.h"
#include "client_Client.h"
#include <iostream>
#include <string>
#define CANT_PARAMETROS 3
#define OK 0
#define ERROR 1
#define POS_HOSTNAME 1
#define POS_PORT 2
#define DELIM_CIN ' '
#define GENERO "GENERO"
#define ASIENTOS "ASIENTOS"
#define RESERVA "RESERVA"
#define IDIOMA "IDIOMA"
#define EDAD "EDAD"
#define FUNCION_DIA "FECHA"
#define FUNCTION_GENERO '1'
#define FUNCTION_IDIOMA '2'
#define FUNCTION_EDAD '3'
#define FUNCTION_FUNCIONES_DIA '4'
#define FUNCTION_RESERVA '5'
#define FUNCTION_ASIENTOS '6'
using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::string;

int main(int argc, char* argv []) {
	if (argc != CANT_PARAMETROS) {
		cerr << 
		"Uso: ./client <ip-servidor> <puerto-servidor>" 
		<< endl;
		return ERROR;
	}
	Socket socket_client;
	Client cliente(socket_client, argv[POS_HOSTNAME], argv[POS_PORT]);
	
	string comando;
	string data;
	string id_funcion;
	string fila;
	string columna;
	while (getline(cin, comando, DELIM_CIN)) {
		if (comando == RESERVA) {
			getline(cin, id_funcion, DELIM_CIN);
			getline(cin, fila, DELIM_CIN);
			getline(cin, columna);
			cliente.reservar_asiento(FUNCTION_RESERVA, id_funcion, fila, 
				columna);
		} else {
			getline(cin, data);
			
			if (comando == ASIENTOS) {
				cliente.asientos_funcion(FUNCTION_ASIENTOS, data);
			} else if (comando == GENERO) {
				cliente.recibo_idioma_edad_genero(FUNCTION_GENERO, data);
			} else if (comando == IDIOMA) {
				cliente.recibo_idioma_edad_genero(FUNCTION_IDIOMA, data);
			} else if (comando == EDAD) {
				cliente.recibo_idioma_edad_genero(FUNCTION_EDAD, data);
			} else if (comando == FUNCION_DIA) {
				cliente.funciones_del_dia(FUNCTION_FUNCIONES_DIA, data);
			}
		}
	}
	
	return OK;
}
