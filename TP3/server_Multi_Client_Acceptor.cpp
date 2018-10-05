#include "server_Multi_Client_Acceptor.h"
#include "server_ThreadServer.h"
#include "server_thread.h"
#include "server_Funcion.h"
#include "server_Pelicula.h"
#include "common_socket.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
using std::vector;
using std::multimap;
using std::string;


Multi_Client_Acceptor::Multi_Client_Acceptor(Socket& socket, 
	vector<multimap<string, Pelicula>>& peliculas, 
	vector<multimap<string, Funcion>>& funciones) : 
	socket_aceptador(std::move(socket)), peliculas(peliculas), 
	funciones(funciones) {
	this->is_alive = true;
	this->threads = vector<ThreadServer>();
}

void Multi_Client_Acceptor::run() {
	while (this->is_alive) {
		// mientras este vivo, sigo aceptando clientes.
		Socket asoc = this->socket_aceptador.accept_();
		if (!this->is_alive) {
			// se cerraron las comunicaciones
			break;
		}
		this->threads.emplace_back(ThreadServer(asoc, this->peliculas, 
			this->funciones));
		this->threads[this->threads.size() - 1].start();
		
		for (size_t i = 0; i < this->threads.size(); i++) {
			if (this->threads[i].has_ended()) {
				this->threads[i].join();
			}
		}
	}
}

void Multi_Client_Acceptor::stop() {
	this->is_alive = false;
	this->socket_aceptador.shutdown_rw();
}

Multi_Client_Acceptor::~Multi_Client_Acceptor() {
	for (size_t i = 0; i < this->threads.size(); i++) {
		this->threads[i].join();
	}
}
