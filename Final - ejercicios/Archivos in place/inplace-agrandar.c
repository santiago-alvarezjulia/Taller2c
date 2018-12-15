/*
 *     Ejercicio [4] final 20/02/07
 *     Escribir un algoritmo ANSI C que, sin crear archivos intermedios
 * 	   altere el archivo a.bin reemplazando la secuencia '-' por '--'
 */

/*
 *     Compilar con gcc -ansi -std=gnu99 -o prueba inplace-agrandar.c
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int leer_reversa(FILE* f, long int* seek) {
	// muevo el file pointer *seek posiciones desde SEEK_SET (inicio del archivo).
	fseek(f, *seek, SEEK_SET);
	// leo un unsigned char extendido a int , puede ser un EOF. mueve el file pointer.
	int out = fgetc(f);
	// si *seek == 0 => llegue al principio del archivo => le asigno -1 que sería
	// como un EOF.
	if (*seek == 0) {
		*seek = -1;
	} else {
		// como al hacer fgetc se mueve solo el file pointer hacia adelante, 
		// lo corro 2 posiciones hacia atras
		fseek(f, -2, SEEK_CUR);
		*seek = ftell(f);
	}
	return out;
}

void escribir_reversa(FILE* f, char c, long int* seek) {
	// muevo el file pointer *seek posiciones desde SEEK_SET (inicio del archivo).
	fseek(f, *seek, SEEK_SET);
	// escribo el char c en el archivo f. mueve el file pointer.
	fputc(c, f);
	// si *seek == 0 => llegue al principio del archivo => le asigno -1 que sería
	// como un EOF.
	if (*seek == 0) {
		*seek = -1;
	} else {
		// como al hacer fgetc se mueve solo el file pointer hacia adelante, 
		// lo corro 2 posiciones hacia atras
		fseek(f, -2, SEEK_CUR);
		*seek = ftell(f);
	}
}

int main(int argc, char* argv[]) {
	FILE *f;
	int c = 0;
	long int c_final = 0; // cantidad de caracteres del archivo final (ya agrandado)
	long int seek_lectura = 0;
	long int seek_escritura = 0;
	int estado = 0;
	
	// abro el archivo en modo lectura solamente
	f = fopen("a.txt","r");
	while ((c = fgetc(f)) != EOF) {
		c_final++;
		if (c == '-') {
			// por que agrego un char por cada '-'
			c_final++;
		}
	}
	// le saco el EOF
	ungetc(c, f);
	// pongo el seek_lectura al final del archivo
	seek_lectura = ftell(f);
	
	// lo cierro (tal vez necesario para poder truncar)
	fclose(f);
	// "trunco" -> agrando el archivo en este caso
	truncate("a.txt", sizeof(char) * c_final);

	// lo abro de nuevo, ahora en modo lectura/escritura
	f = fopen("a.txt","r+");
	// muevo el file pointer al final del archivo ya agrandado
	fseek(f, 0, SEEK_END);
	// seek_escritura al final del archivo ya agrandado
	seek_escritura = ftell(f);
	while (seek_lectura >= 0) {
		// leo el archivo de nuevo, esta vez desde el ultimo caracter que aparecia
		// en el archivo original. leo de atras hacia adelante
		c = leer_reversa(f, &seek_lectura);
		// escribo al final del archivo agrandado el caracter leido
		escribir_reversa(f, c, &seek_escritura);
		// si aparece un '-', lo escribo otra vez (lo que pide el ejercicio)
		if (c == '-') {
			escribir_reversa(f, c, &seek_escritura);
		}
	}
	fclose(f);
	return 0;
}
