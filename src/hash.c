#include <string.h>
#include <stdlib.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MIN \
	3 // se utilizara al momento de crear segun lo especifica el .h

typedef struct par {
	char *clave;
	void *valor;
	struct par *siguiente;
} par_t;

struct hash {
	size_t capacidad;
	size_t cantidad;
	par_t **elementos;
};

/*
Djb2 hash function
FUNCION DE LAUTARO MARTIN SOTELO
PRE: RECIBE LA CADENA DE CARACTERES A HASHEAR
POST: RETORNA UN SIZE_T QUE REPRESENTA EL HASH QUE SE GENERO POR LA CADENA INGRESADA.
 */
size_t djb2(const char *str)
{
	size_t hash = 5381;
	int c;
	//Va a recorrer los caracteres de la cadena y calcula el hasg
	while ((c = (int)*str++))
		hash = ((hash << 5) + hash) + (size_t)c; /* hash * 33 + c */

	//retorna el valor final del hash
	return hash;
}

/*
FUNCION DE LAUTARO MARTIN SOTELO
PRE: RECIBE UN CHAR DE LA CLAVE
POST: COPIA EL VALOR EN UNA VARIABLE AUXILIAR
*/

char *copiar_clave(const char *clave)
{
	if (!clave) {
		return NULL;
	}
	char *clave_copiada = malloc(strlen(clave) + 1);
	strcpy(clave_copiada, clave);
	return clave_copiada;
}

hash_t *hash_crear(size_t capacidad)
{
	if (capacidad < CAPACIDAD_MIN) {
		capacidad = CAPACIDAD_MIN; // como lo especifa en el .h
	}
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash) {
		return NULL;
	}
	hash->capacidad = capacidad;
	hash->cantidad = 0;
	hash->elementos = calloc(capacidad, sizeof(par_t *));
	if (!(hash->elementos)) {
		free(hash);
		return NULL;
	}
	return hash;
}

//FUNCION DE LAUTARO MARTIN SOTELO
//PRE: RECIBE EL HASH
//POST: DUPLICARA LA CAPACIDAD DEL MISMO, Y REUBICARA LOS ELEMENTOS COMO CORRESPONDE

hash_t *funcion_rehash(hash_t *hash)
{
	if (!hash) {
		return NULL;
	}
	hash_t *rehash = hash_crear(hash->capacidad * 2 + 1);
	if (!rehash) {
		return NULL;
	}
	for (size_t i = 0; i < hash->capacidad; i++) {
		for (par_t *par = hash->elementos[i]; par;
		     par = par->siguiente) {
			hash_insertar(rehash, par->clave, par->valor, NULL);
		}
	}
	hash_t aux = *hash;
	*hash = *rehash;
	*rehash = aux;
	hash_destruir(rehash);
	return hash;
}

/*
FUNCION DE LAUTARO MARTIN SOTELO
PRE RECIBE UNA CLAVE Y UN ELEMENTO, RESERVA MEMORIA PARA PAR_T Y ASIGNA EL ELEMENTO, COPIANDO LA CLAVE EN EL NUEVO LUGAR.
POST: DEVUELVE UN PUNTERO A PAR_T CON LA CLAVE Y EL ELEMENTO
*/

par_t *nuevo_par(const char *clave, void *elemento)
{
	par_t *par = calloc(1, sizeof(par_t));
	if (!par)
		return NULL;

	par->valor = elemento;
	par->clave = copiar_clave(clave);
	if (!par->clave) {
		free(par);
		return NULL;
	}

	return par;
}

/*
FUNCION DE LAUTARO MARTIN SOTELO
//PRE:RECIBE UN HASH, UNA CLAVE, Y EL ELEMENTO O VALOR,UN ELEMENTO ANTERIOR , LA POSICION EN LA TABLA HASH Y REFERENCIAS A LA PAR ACTUAL Y PAR ANTERIOR.
//POST: ACTUALIZA EL VALOR DEL ELEMENTO SI ENCUENTRA COINCIDENCIA CON LA CLAVE EN LA TABLA, CASO APARTE INSERTARA UN NUEVO PAR CLAVE-VALOR EN LA POSICION QUE LE CORRESPONDE.
*/

void actualizar_o_insertar_elemento(hash_t *hash, const char *clave,
				    void *elemento, void **anterior, size_t pos,
				    par_t *par, par_t *prev)
{
	if (par != NULL) {
		if (anterior)
			*anterior = par->valor;
		par->valor = elemento;
	} else {
		par_t *nuevo = nuevo_par(clave, elemento);
		if (!nuevo)
			return;

		if (prev)
			prev->siguiente = nuevo;
		else
			hash->elementos[pos] = nuevo;

		hash->cantidad++;
	}
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	if (anterior)
		*anterior = NULL;

	size_t pos = djb2(clave) % hash->capacidad;

	par_t *prev = NULL;
	par_t *par = hash->elementos[pos];

	while (par != NULL && strcmp(clave, par->clave) != 0) {
		prev = par;
		par = par->siguiente;
	}

	actualizar_o_insertar_elemento(hash, clave, elemento, anterior, pos,
				       par, prev);

	if (((float)hash->cantidad) / ((float)hash->capacidad) >
	    FACTOR_CARGA_MAXIMO) {
		funcion_rehash(hash);
	}

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t pos = djb2(clave) % hash->capacidad;
	par_t *par = hash->elementos[pos];
	par_t *prev = NULL;

	while (par != NULL && strcmp(par->clave, clave) != 0) {
		prev = par;
		par = par->siguiente;
	}

	if (par == NULL)
		return NULL;

	if (prev == NULL)
		hash->elementos[pos] = par->siguiente;
	else
		prev->siguiente = par->siguiente;

	void *valor = par->valor;
	free(par->clave);
	free(par);
	hash->cantidad--;

	return valor;
}

/*
FUNCION DE LAUTARO MARTIN SOTELO
PRE: RECIBE UN PUNTERO A LA TABLA HASH Y LA CLAVE, ASI VERIFICANDO QUE NO SEAN NULAS
POST:SI ENCONTRO UN PAR CON LA CLAVE , SE DEVUELVE EL PUNTERO AL PAR, SINO RETORNARA NULL
*/

par_t *buscar_clave_en_hash(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t pos = djb2(clave) % hash->capacidad;
	par_t *par = hash->elementos[pos];

	while (par != NULL) {
		if (strcmp(clave, par->clave) == 0) {
			return par;
		}
		par = par->siguiente;
	}
	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	par_t *par = buscar_clave_en_hash(hash, clave);
	return par ? par->valor : NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	return buscar_clave_en_hash(hash, clave) != NULL;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;

	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
	return;
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {
		for (par_t *par = hash->elementos[i]; par;) {
			if (destructor) {
				destructor(par->valor);
			}
			par_t *siguiente = par->siguiente;
			free(par->clave);
			free(par);
			par = siguiente;
		}
		hash->elementos[i] = NULL;
	}
	free(hash->elementos);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;
	bool continuar = true;
	par_t *par;
	size_t i = 0;

	while (i < hash->capacidad && continuar) {
		par = hash->elementos[i];
		while (par && continuar) {
			continuar = f(par->clave, par->valor, aux);
			n++;
			par = par->siguiente;
		}
		i++;
	}

	return n;
}