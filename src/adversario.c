#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct adversario{
	lista_t * lista_pokemon;
	lista_t * pokemones_jugador;
	lista_t * pokemones_adversario;
	int puntos;
};

adversario_t *adversario_crear(lista_t *pokemon)
{
	adversario_t *adversario = malloc(sizeof(adversario_t));
	if (!adversario) {
		return NULL;
	}
	adversario->lista_pokemon = NULL;
	return adversario;
}

//comparador de nombres
int comparar_nombres(void *pokemon, void *nombre) {
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

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	if (adversario == NULL || nombre1 == NULL || nombre2 == NULL ||
	    nombre3 == NULL) {
		return false;
	}
	pokemon_t* pokemon_1 = lista_buscar_elemento(adversario->lista_pokemon,comparar_nombres,(void *)(*nombre1));
	pokemon_t* pokemon_2 = lista_buscar_elemento(adversario->lista_pokemon,comparar_nombres,(void *)(*nombre2));
	pokemon_t* pokemon_3 = lista_elemento_en_posicion(adversario->pokemones_jugador,3);
	if (pokemon_1 == NULL || pokemon_2 == NULL || pokemon_3 == NULL) {
		return false; // Si alguno de los Pokémon no existe en la lista
	}
	if ((pokemon_1 == pokemon_2) ||
	    (pokemon_1 == pokemon_3) ||
	    (pokemon_2 == pokemon_3)) {
		return false; // Si hay Pokémon repetidos en la selección
	}
	lista_t * pokemones ;
	pokemones = lista_crear();
	if (!pokemones){
		return false;
	}
	pokemones = lista_insertar(pokemones,pokemon_1);
	pokemones = lista_insertar(pokemones,pokemon_2);
	pokemones = lista_insertar(pokemones,pokemon_3);
	adversario->pokemones_adversario = lista_crear();
	if (!adversario->pokemones_adversario){
		return false;
	}
	adversario->pokemones_adversario = pokemones;

	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	if (adversario == NULL || nombre1 == NULL || nombre2 == NULL ||
	    nombre3 == NULL) {
		return false;
	}
	pokemon_t* pokemon_1 = lista_buscar_elemento(adversario->lista_pokemon,comparar_nombres,(void *)nombre1);
	pokemon_t* pokemon_2 = lista_buscar_elemento(adversario->lista_pokemon,comparar_nombres,(void *)nombre2);
	pokemon_t* pokemon_3 = lista_buscar_elemento(adversario->lista_pokemon,comparar_nombres,(void *)nombre3);
	if (pokemon_1 == NULL || pokemon_2 == NULL || pokemon_3 == NULL) {
		return false; // Si alguno de los Pokémon no existe en la lista
	}
	if ((pokemon_1 == pokemon_2) ||
	    (pokemon_1 == pokemon_3) ||
	    (pokemon_2 == pokemon_3)) {
		return false; // Si hay Pokémon repetidos en la selección
	}
	lista_t * pokemones ;
	pokemones = lista_crear();
	if (!pokemones){
		return false;
	}
	pokemones = lista_insertar(pokemones,pokemon_1);
	pokemones = lista_insertar(pokemones,pokemon_2);
	pokemones = lista_insertar(pokemones,pokemon_3);
	adversario->pokemones_jugador = lista_crear();
	if (!adversario->pokemones_jugador){
		return false;
	}
	adversario->pokemones_jugador = pokemones;

	return true;
}


jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };
	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
}
