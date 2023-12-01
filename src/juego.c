#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>


struct datos_jugador{
	int puntos;
	lista_t *pokemones;
};

struct juego {
	informacion_pokemon_t *informacion_pokemon;
	lista_t *lista_pokemon;
	bool estado_juego;
	struct datos_jugador jugador_1;
	struct datos_jugador jugador_2;
};



juego_t *juego_crear()
{
	juego_t *juego = malloc(sizeof(juego_t));
	if (!juego) {
		return NULL;
	}
	juego->lista_pokemon = NULL;
	juego->informacion_pokemon = NULL;
	juego->estado_juego = true;
	return juego;
}

//FUNCION DE LAUTARO MARTIN SOTELO
//PRE: RECIBE EL POKEMON Y UN VOID QUE ES LA LISTA POKEMON.
//POST: INSERTA CADA POKEMON EN LA LISTA.
void insertar_pokemon_lista(pokemon_t *pokemon, void *lista_pokemon)
{
	lista_pokemon = lista_insertar(lista_pokemon, pokemon);
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if (juego == NULL || archivo == NULL) {
		return ERROR_GENERAL;
	}

	// Creo una estructura para almacenar la información de los Pokémon
	informacion_pokemon_t *pokemon_info = pokemon_cargar_archivo(archivo);
	if (pokemon_info == NULL) {
		return POKEMON_INSUFICIENTES;
	}

	if (!juego->lista_pokemon) {
		juego->lista_pokemon = lista_crear();
		if (!juego->lista_pokemon) {
			return ERROR_GENERAL;
		}
	}
	int cantidad_pokemon = con_cada_pokemon(
		pokemon_info, insertar_pokemon_lista, juego->lista_pokemon);
	if (cantidad_pokemon < 4) {
		juego_destruir(juego);
		return POKEMON_INSUFICIENTES;
	}
	juego->informacion_pokemon = pokemon_info;
	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if (!juego) {
		return NULL;
	}
	return juego->lista_pokemon;
}

int comparador(void *pokemon, void *nombre) {
    pokemon_t* aux_pokemon = (pokemon_t*)pokemon;
    const char *nombre_pokemon = (const char *)nombre;
    
    const char *nombre_aux_pokemon = pokemon_nombre(aux_pokemon);
    
    if (nombre_aux_pokemon != NULL && nombre_pokemon != NULL) {
        if (strcmp(nombre_aux_pokemon, nombre_pokemon) == 0) {
            return 0;
        }
    }
    
    return -1;
}

bool mostra_pokemon(void *p,void * contexto){
	printf("Nombre: %s\n",pokemon_nombre(p));
	return true;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if (juego == NULL || nombre1 == NULL || nombre2 == NULL ||
	    nombre3 == NULL) {
		return ERROR_GENERAL;
	}
	pokemon_t* pokemon_1 = lista_buscar_elemento(juego->lista_pokemon,comparador,(void *)nombre1);
	pokemon_t* pokemon_2 = lista_buscar_elemento(juego->lista_pokemon,comparador,(void *)nombre2);
	pokemon_t* pokemon_3 = lista_buscar_elemento(juego->lista_pokemon,comparador,(void *)nombre3);
	if (pokemon_1 == NULL || pokemon_2 == NULL || pokemon_3 == NULL) {
		return POKEMON_INEXISTENTE; // Si alguno de los Pokémon no existe en la lista
	}
	if ((pokemon_1 == pokemon_2) ||
	    (pokemon_1 == pokemon_3) ||
	    (pokemon_2 == pokemon_3)) {
		return POKEMON_REPETIDO; // Si hay Pokémon repetidos en la selección
	}
	lista_t * pokemones ;
	pokemones = lista_crear();
	pokemones = lista_insertar(pokemones,pokemon_1);
	pokemones = lista_insertar(pokemones,pokemon_2);
	pokemones = lista_insertar(pokemones,pokemon_3);
	if(jugador == JUGADOR1){
		juego->jugador_1.pokemones = lista_crear();
		juego->jugador_1.pokemones = pokemones;
	}else if (jugador == JUGADOR2){
		juego->jugador_2.pokemones = lista_crear();
		juego->jugador_2.pokemones = pokemones;
	}
	
	lista_con_cada_elemento(juego->jugador_2.pokemones,mostra_pokemon,NULL);
	printf("esta es la prueba\n");
	//guarda_pokemon_seleccionados(juego, jugador, pokemon_1, pokemon_2,
				     //pokemon_3);
	return TODO_OK;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;
	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	return true;
}

void juego_destruir(juego_t *juego)
{
}