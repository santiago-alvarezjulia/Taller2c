unsigned char function;
	int i = asoc.receive_(&function, sizeof(unsigned char));
	if (i == 0) {
		return OK;
	}
	
	cout << function << endl;
	
	// recibo la longitud del genero y después el genero
	unsigned int len_filename;
	asoc.receive_((unsigned char*)&len_filename, sizeof(unsigned int));
		
	cout << len_filename << endl;
		
	string genero(len_filename, '0');
	asoc.receive_((unsigned char*) genero.c_str(), len_filename);
	
	cout << genero << endl;
	
	multimap<string, Pelicula> peliculas_segun_genero = clasificacion_peliculas[2];
	std::pair<multimap<string, Pelicula>::iterator, multimap<string, Pelicula>::iterator> ret;
    ret = peliculas_segun_genero.equal_range(genero);
    for (multimap<string, Pelicula>::iterator it = ret.first; it != ret.second; ++it) {
		string titulo_pelicula = it->second.getTitulo();
		unsigned int len_titulo_pelicula = titulo_pelicula.size();
		asoc.send_((unsigned char*)&len_titulo_pelicula, sizeof(unsigned int));
		asoc.send_((unsigned char*)titulo_pelicula.c_str(), len_titulo_pelicula);
	}
	unsigned int fin_envio_socket = FIN_ENVIO_SOCKET;
	asoc.send_((unsigned char*)&fin_envio_socket, sizeof(unsigned int));
