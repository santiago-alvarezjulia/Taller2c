// imprimo titulos con idioma SUB
	multimap<string, Pelicula> pelicula_segun_idioma = clasificacion_peliculas[0];
	string idioma = "SUB";
	std::pair<multimap<string, Pelicula>::iterator, multimap<string, Pelicula>::iterator> ret;
    ret = pelicula_segun_idioma.equal_range(idioma);
    for (multimap<string, Pelicula>::iterator it = ret.first; it != ret.second; ++it) {
      cout << it->second.getTitulo() << endl;
	}
	
	cout << endl;
	
	// imprimo titulos del genero Drama
	multimap<string, Pelicula> pelicula_segun_genero = clasificacion_peliculas[2];
	string genero = "Drama";
	std::pair<multimap<string, Pelicula>::iterator, multimap<string, Pelicula>::iterator> ret2;
    ret2 = pelicula_segun_genero.equal_range(genero);
    for (multimap<string, Pelicula>::iterator it = ret2.first; it != ret2.second; ++it) {
      cout << it->second.getTitulo() << endl;
	}
	 
	cout << endl;
	
	// imprimo titulos de FECHA 25/09/2018
	multimap<string, Funcion> funciones_segun_fecha = clasificacion_funciones[0];
	string fecha = "25/09/2018";
	std::pair<multimap<string, Funcion>::iterator, multimap<string, Funcion>::iterator> ret3;
    ret3 = funciones_segun_fecha.equal_range(fecha);
    for (multimap<string, Funcion>::iterator it = ret3.first; it != ret3.second; ++it) {
		cout << it->second.getTitulo() << endl;
	}
	
	// imprimo sala de funcion 1
	int cantidad_columnas = clasificacion_funciones[1].find("1")->second.getCantidadColumnas();
	for (int i = 0; i < cantidad_columnas; i++) {
		cout << '\t' << i+1;
	}
	cout << endl;
	
	
	std::vector<std::vector<char>> asientos = 
		clasificacion_funciones[1].find("1")->second.getAsientos();
	int col = 0;
	std::string columnas_posibles = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (std::vector<std::vector<char>>::iterator it = asientos.begin() ; it != asientos.end(); ++it) {
		cout << columnas_posibles[col];
		for (std::vector<char>::iterator it2 = (*it).begin() ; it2 != (*it).end(); ++it2) {
			cout << '\t' << *it2;
		}
		
		col++;
		cout << endl;
	}
	cout << endl;
	cout << endl;
	cout << endl;
	// reservo asiento A-3 en funcion 1 y despues la imprimo
	bool reservar_asiento = 
		clasificacion_funciones[1].find("1")->second.reservarAsiento("A", "3");
	cout << reservar_asiento << endl;
	for (int i = 0; i < cantidad_columnas; i++) {
		cout << '\t' << i+1;
	}
	cout << endl;
	
	std::vector<std::vector<char>> asientos2 = 
		clasificacion_funciones[1].find("1")->second.getAsientos();
	col = 0;
	for (std::vector<std::vector<char>>::iterator it2 = asientos2.begin() ; it2 != asientos2.end(); ++it2) {
		cout << columnas_posibles[col];
		for (std::vector<char>::iterator it3 = (*it2).begin() ; it3 != (*it2).end(); ++it3) {
			cout << '\t' << *it3;
		}
		
		col++;
		cout << endl;
	}
