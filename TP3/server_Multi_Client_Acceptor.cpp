#include "server_Multi_Client_Acceptor.h"
#include "server_ThreadServer.h"
#include "server_thread.h"
#include "server_Funcion.h"
#include "server_Pelicula.h"
#include "common_socket.h"
#include "common_SocketError.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
using std::vector;
using std::multimap;
using std::string;


Multi_Client_Acceptor::Multi_Client_Acceptor(Socket& socket, 
	vector<multimap<string, Pelicula>>& peliculas, 
	FuncionesProtected& funciones) : 
	socket_aceptador(std::move(socket)), peliculas(peliculas), 
	funciones(funciones) {
	this->esta_vivo = true;
	this->threads = vector<ThreadServer>();
}

void Multi_Client_Acceptor::run() {
	try {
		// mientras este vivo, sigo aceptando clientes.
		while (this->esta_vivo) {
			// accept_() puede lanzar SocketError
			Socket asoc = this->socket_aceptador.accept_();
			
			// antes elimino los threads que ya hayan finalizado su ejecucion
			for (size_t i = 0; i < this->threads.size(); i++) {
				if (this->threads[i].ha_terminado() && 
					this->threads[i].joinable()) {
					this->threads[i].join();
				}
			}
		
			this->threads.emplace_back(ThreadServer(asoc, this->peliculas, 
				this->funciones));
			this->threads[this->threads.size() - 1].start();
		}
	} catch (const SocketError& e) {
		// no printeo e.what para que no fallen las pruebas en sercom
	}
}

void Multi_Client_Acceptor::frenar() {
	this->esta_vivo = false;
	// cierro la conexion del socket aceptador
	this->socket_aceptador.shutdown_rw();
}

Multi_Client_Acceptor::~Multi_Client_Acceptor() {
	for (size_t i = 0; i < this->threads.size(); i++) {
		if (this->threads[i].joinable()) {
			this->threads[i].join();
		}
	}
}