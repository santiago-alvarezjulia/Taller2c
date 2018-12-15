/*
 *     Ejercicio [5] final 12/12/06
 *     Escribir un algoritmo ANSI C que, sin crear archivos intermedios
 * 	   altere el archivo a.txt reemplazando la secuencia '//' por '*'
 * 	   Excepto si se encuentra entre parentesis	
 */

/*
 *     Compilar con gcc -ansi -std=gnu99 -o prueba inplace-acortar.c
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int leer(FILE* f, long int* seek){
	// muevo el file pointer *seek posiciones desde SEEK_SET (inicio del archivo).
	fseek(f, *seek, SEEK_SET);
	// leo un unsigned char extendido a int , puede ser un EOF. mueve el file pointer.
	int out = fgetc(f);
	// actualizo la posicion del puntero respecto al origen.
	*seek = ftell(f);
	// devuelvo el int leido.
	return out;
}

void escribir(FILE* f, char c, long int* seek) {
	// muevo el file pointer *seek posiciones desde SEEK_SET (inicio del archivo).
	fseek(f, *seek, SEEK_SET);
	// escribo el char c en el archivo f. mueve el file pointer.
	fputc(c, f);
	// actualizo la posicion del puntero respecto al origen.
	*seek = ftell(f);
}

int main(int argc, char* argv[])
{
	FILE *f;
	// el int que leo de f
	int c = 0;
	long int c_final = 0; // contador de caracteres final (para alfinal hacer el truncate)
	// mantengo 2 seeks separados
	long int seek_lectura = 0;
	long int seek_escritura = 0;
	// maquina de estados. indica de alguna manera el pasado, lo que ya lei.
	int estado = 0;
	
	f = fopen("a.txt","r+");
	
	// leo de a 1 int hasta el EOF
	while ((c = leer(f, &seek_lectura)) != EOF) {
		c_final++;
		// estado 0 -> nada
		if (estado == 0) {
			if (c == '/') {
				// estado 2 -> 1 barra. tener en cuenta que como no llamo a escribir()
				// no aumenta el seek_escritura. Si aparece una segunda barra, deberia
				// poner el * en la pos de la primer barra (me la da seek_escritura)
				estado = 2;
			} else {
				// si no es una barra, escribo lo que lei (sobreescribo?)
				escribir(f, c, &seek_escritura);
				if (c == '(') {
					// estado 1 -> abrio paréntesis
					estado = 1;
				}
			}
		} else if (estado == 1) {
			// estado 1 -> abrio parentesis => seguro que escribo.
			escribir(f, c, &seek_escritura);
			if (c == ')') {
				// vengo de abrir parentesis, cambio a nada cuando lo cierro.
				// acordarse que dentro del parentesis no se cambia la doble barra
				estado = 0;
			}
		} else if (estado == 2) {
			// hubo una primera barra. Veo si hay una segunda barra
			if (c == '/'){
				// hay una segunda barra. Bajo el contador final en 1 (por reemplazar 2 barras
				// por un asterisco). 
				c_final--;
				// escribo el asterisco y vuelvo al estado nada.
				escribir(f, '*', &seek_escritura);
				estado = 0;
			} else {
				//no hubo segunda barra. escribo la primera barra. escribo el c leido
				// y me fijo segun c en cual estado queda el programa.
				escribir(f, '/', &seek_escritura);
				escribir(f, c, &seek_escritura);
				estado = c == '(' ? 1 : 0;
			}
		}
	}

	fclose(f);
	truncate("a.txt", sizeof(char) * c_final);
	return 0;
}
