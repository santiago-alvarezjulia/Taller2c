#include "common_Protocolo.h"
#include "common_socket.h"
#include <iostream>
#include <string>
#include <vector>
#define CHAR_INITIALIZE_STRING '0'
#define FIN_RECIBO_SOCKET 0
using std::string;
using std::vector;

Protocolo::Protocolo(Socket socket, char* hostname, char* port) : 
    socket(std::move(socket)) {
    this->socket.connect_(hostname, port); // puede lanzar SocketError
}

Protocolo::Protocolo(Socket& socket) : socket(std::move(socket)) {}
		
Protocolo::Protocolo(Protocolo&& other) : socket(std::move(other.socket)){}

void Protocolo::send_string(string& data) {
    // envio primero el largo del string la data
    this->send_unsigned_int(data.size());
    // para obtener el puntero al inicio de data utilizo std::vector::data
    vector<uint8_t> data_vector(data.begin(), data.end());
    this->socket.send_(data_vector.data(), data.size());
}

void Protocolo::send_unsigned_char(uint8_t data) {
    // envio la data
	this->socket.send_(&data, sizeof(uint8_t));
}

void Protocolo::send_unsigned_int(uint32_t data) {
	// convierto la data a network y la envio
    uint32_t network_data = htonl(data);
    this->socket.send_((uint8_t*)&network_data, 
		sizeof(uint32_t));
}

string Protocolo::receive_string() {
    // recibo la longitud de la data
    uint32_t len_data = this->receive_unsigned_int();

    if (len_data == FIN_RECIBO_SOCKET) {
        return std::move(string());
    }

    // recibo la data
    string data(len_data, CHAR_INITIALIZE_STRING);
    // para obtener el puntero al inicio de data utilizo std::vector::data
	this->socket.receive_((uint8_t*) data.c_str(), len_data);

    return std::move(data);
}

unsigned int Protocolo::receive_unsigned_int() {
    // recibo la data
    uint32_t data;
	this->socket.receive_((uint8_t*)&data, 
		sizeof(uint32_t));
    // devuelvo la data recibida convertida a host
    return ntohl(data);
}
		
unsigned char Protocolo::receive_unsigned_char() {
    // recibo la data
    uint8_t data;
	int i = this->socket.receive_(&data, sizeof(uint8_t));
    if (i == 0) {
        return '0';
    }
    return data;
}

Protocolo::~Protocolo() {
    this->socket.shutdown_rdwr();
}
