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
	
	
	Client client(argv[POS_HOSTNAME], argv[POS_PORT]);
	
	string comando;
	string data;
	string id_funcion;
	string fila;
	string columna;
	while (getline(cin, comando, DELIM_CIN)) {
		cout << comando << endl;
		if (comando == RESERVA) {
			getline(cin, id_funcion, DELIM_CIN);
			getline(cin, fila, DELIM_CIN);
			getline(cin, columna);
		} else if (comando == ASIENTOS) {
			getline(cin, data);
		} else if (comando == GENERO) {
			getline(cin, data);
		}
	}
	
	return OK;
}
