#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_fin;
	size_t cantidad;
};

struct lista_iterador {
	nodo_t *nodo_iterador;
	lista_t *lista;
};

// FUNCION DE LAUTARO MARTIN SOTELO
//PRE: recibe el nodo, junto a la posicion que se esta buscando.
//POST: devuelve el nodo en la posicion que se esta buscando, caso contrario, devuelve el ultimo nodo.
nodo_t *buscar_nodo(nodo_t *nodo_buscado, size_t posicion_buscada)
{
	if (!nodo_buscado->siguiente || (posicion_buscada == 0)) {
		return nodo_buscado;
	}
	return buscar_nodo(nodo_buscado->siguiente, posicion_buscada - 1);
}

lista_t *lista_crear()
{
	lista_t *lista = calloc(1, sizeof(lista_t));
	if (!lista) {
		return NULL;
	}
	lista->nodo_inicio = NULL;
	lista->nodo_fin = NULL;
	lista->cantidad = 0;
	return lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista) {
		return NULL;
	}
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = NULL;
	if (!lista->nodo_inicio) {
		lista->nodo_inicio = nuevo_nodo;
	} else {
		lista->nodo_fin->siguiente = nuevo_nodo;
	}
	lista->nodo_fin = nuevo_nodo;
	lista->cantidad++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return NULL;
	}
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}
	nuevo_nodo->elemento = elemento;
	if (posicion >= lista->cantidad) {
		free(nuevo_nodo);
		return lista_insertar(lista, elemento);
	} else if (posicion == (size_t)0) {
		nuevo_nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo_nodo;
	} else {
		nodo_t *anterior_nodo =
			buscar_nodo(lista->nodo_inicio, posicion - 1);
		nuevo_nodo->siguiente = anterior_nodo->siguiente;
		anterior_nodo->siguiente = nuevo_nodo;
	}
	lista->cantidad++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}
	if (lista->cantidad == 0) {
		return NULL;
	}
	nodo_t *elemento_eliminado = lista->nodo_fin->elemento;
	if (lista->cantidad == 1) {
		free(lista->nodo_fin);
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
	} else {
		nodo_t *nodo_anterior =
			buscar_nodo(lista->nodo_inicio, lista->cantidad - 2);
		free(lista->nodo_fin);
		nodo_anterior->siguiente = NULL;
		lista->nodo_fin = nodo_anterior;
	}
	lista->cantidad--;
	return elemento_eliminado;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista->cantidad == 0) {
		return NULL;
	}
	if (posicion == 0) {
		nodo_t *elemento_eliminado = lista->nodo_inicio->elemento;
		nodo_t *nodo_eliminado = lista->nodo_inicio;
		lista->nodo_inicio = nodo_eliminado->siguiente;
		free(nodo_eliminado);
		lista->cantidad--;
		return elemento_eliminado;
	} else if (posicion >= lista->cantidad - 1) {
		return lista_quitar(lista);
	}
	nodo_t *nodo_anterior = buscar_nodo(lista->nodo_inicio, posicion - 1);
	nodo_t *nodo_eliminado = nodo_anterior->siguiente;
	nodo_t *elemento_eliminado = nodo_eliminado->elemento;
	nodo_anterior->siguiente = nodo_eliminado->siguiente;
	free(nodo_eliminado);
	lista->cantidad--;
	return elemento_eliminado;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista) || (posicion >= lista->cantidad)) {
		return NULL;
	}
	nodo_t *posicion_de_nodo = buscar_nodo(lista->nodo_inicio, posicion);
	return posicion_de_nodo->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || !comparador)
		return NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;
	bool encontrado = false;
	while (nodo_actual && !encontrado) {
		encontrado = (comparador(nodo_actual->elemento, contexto) == 0);
		if (!encontrado) {
			nodo_actual = nodo_actual->siguiente;
		}
	}
	if (!nodo_actual)
		return NULL;
	return nodo_actual->elemento;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || !lista->nodo_inicio)
		return NULL;
	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || !lista->nodo_fin)
		return NULL;
	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return true;
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return 0;
	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;
	while (lista->cantidad > 0) {
		lista_quitar_de_posicion(lista, 0);
	}
	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista)
		return;
	if (!funcion) {
		lista_destruir(lista);
		return;
	}
	nodo_t *nodo_actual = lista->nodo_inicio;
	if (!nodo_actual) {
		free(lista);
		return;
	}
	nodo_t *nodo_siguiente = nodo_actual->siguiente;
	while (nodo_siguiente) {
		funcion(nodo_actual->elemento);
		free(nodo_actual);
		nodo_actual = nodo_siguiente;
		nodo_siguiente = nodo_actual->siguiente;
	}
	funcion(nodo_actual->elemento);
	free(nodo_actual);
	free(lista);
	return;
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;
	lista_iterador_t *iterador = calloc(1, sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;
	iterador->lista = lista;
	if (!lista_vacia(lista)) {
		iterador->nodo_iterador = lista->nodo_inicio;
	}
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador) {
		return false;
	}
	return iterador->nodo_iterador;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (iterador == NULL || iterador->nodo_iterador == NULL)
		return false;
	if ((iterador->nodo_iterador = iterador->nodo_iterador->siguiente) ==
	    NULL)
		return false;
	return true;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo_iterador)
		return NULL;
	return iterador->nodo_iterador->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (iterador) {
		free(iterador);
	}
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista) {
		return 0;
	}
	if (!funcion) {
		return 0;
	}
	size_t contador = 0;
	nodo_t *nodo_actual = lista->nodo_inicio;
	bool continuar = true;
	while ((nodo_actual) && (continuar)) {
		continuar = funcion(nodo_actual->elemento, contexto);
		nodo_actual = nodo_actual->siguiente;
		contador++;
	}
	return contador;
}