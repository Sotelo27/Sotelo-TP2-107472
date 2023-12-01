#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include "stdlib.h"

struct juego {
	informacion_pokemon_t * info_pokemon;
	lista_t *lista_pokemon;
};

juego_t *juego_crear()
{
	juego_t *juego = malloc(sizeof(juego_t));
	if (!juego) {
		return NULL;
	}
	juego->lista_pokemon = NULL;
	return juego;
}


//FUNCION DE LAUTARO MARTIN SOTELO
//PRE: RECIBE EL POKEMON Y UN VOID QUE ES LA LISTA POKEMON.
//POST: INSERTA CADA POKEMON EN LA LISTA.
void insertar_pokemon_lista(pokemon_t * pokemon,void* lista_pokemon){
	lista_pokemon = lista_insertar(lista_pokemon,pokemon);
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

	if(!juego->lista_pokemon){
		juego->lista_pokemon = lista_crear();
		if(!juego->lista_pokemon){
			return ERROR_GENERAL;
		}
	}
	int cantidad_pokemon = con_cada_pokemon(pokemon_info,insertar_pokemon_lista,juego->lista_pokemon);
	if(cantidad_pokemon < 4){
		juego_destruir(juego);
		return POKEMON_INSUFICIENTES;
	}
	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if(!juego){
		return NULL;
	}
	return juego->lista_pokemon;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
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