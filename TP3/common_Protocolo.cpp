#include "common_Protocolo.h"
#include "common_socket.h"
#include <string>
#define CHAR_INITIALIZE_STRING '0'
#define FIN_RECIBO_SOCKET 0
using std::string;

Protocolo::Protocolo(Socket socket, char* hostname, char* port) : 
    socket(std::move(socket)) {
    this->socket.connect_(hostname, port); // puede lanzar SocketError
}
		
void Protocolo::send_string(string& data) {
    // envio la data
	unsigned int len_data = data.size();
	this->socket.send_((unsigned char*)&len_data, sizeof(unsigned int));
	this->socket.send_((unsigned char*)data.c_str(), len_data);
}

void Protocolo::send_unsigned_char(unsigned char data) {
    // envio la data
	this->socket.send_(&data, sizeof(unsigned char));
}

string Protocolo::receive_string() {
    // recibo la longitud de la data
    unsigned int len_data;
	this->socket.receive_((unsigned char*)&len_data, sizeof(unsigned int));

    if (len_data == FIN_RECIBO_SOCKET) {
        return std::move(string());
    }

    // recibo la data
    string data(len_data, CHAR_INITIALIZE_STRING);
	this->socket.receive_((unsigned char*) data.c_str(), len_data);

    return std::move(data);
}

unsigned int Protocolo::receive_unsigned_int() {
    unsigned int data;
	this->socket.receive_((unsigned char*)&data, 
		sizeof(unsigned int));

    return data;
}
		
Protocolo::~Protocolo() {}