#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "ataque.h"
#include "pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct adversario {
	lista_t *lista_pokemon;
	lista_t *pokemones_jugador;
	lista_t *pokemones_adversario;
	lista_t *ataques_posibles;
	jugada_t jugadas_posibles[10];
	int turnos;
};

adversario_t *adversario_crear(lista_t *pokemon)
{
	adversario_t *adversario = malloc(sizeof(adversario_t));
	if (!adversario) {
		return NULL;
	}
	adversario->lista_pokemon = pokemon;
	adversario->pokemones_adversario = NULL;
	adversario->ataques_posibles = NULL;
	adversario->pokemones_jugador = NULL;
	adversario->turnos = 0;
	for (int i = 0; i < 10; i++) {
		strcpy(adversario->jugadas_posibles->pokemon, "");
		strcpy(adversario->jugadas_posibles->ataque, "");
	}
	return adversario;
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE EL POKEMON Y UN NOMBRE
//POST: EVALUA SI SON IGUALES O NO SUS NOMBRES.
int comparar_nombres(void *pokemon, void *nombre)
{
	pokemon_t *aux_pokemon = (pokemon_t *)pokemon;
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
	size_t indice = 0;
	pokemon_t *buscar_pokemon =
		lista_elemento_en_posicion(adversario->lista_pokemon, indice);
	pokemon_t *pokemon_1 =
		lista_elemento_en_posicion(adversario->lista_pokemon, 0);
	pokemon_t *pokemon_2 =
		lista_elemento_en_posicion(adversario->lista_pokemon, 1);
	pokemon_t *pokemon_3 =
		lista_elemento_en_posicion(adversario->pokemones_jugador, 2);
	*nombre1 = (char *)pokemon_nombre(pokemon_1);
	*nombre2 = (char *)pokemon_nombre(pokemon_2);
	char *nombre_aux = (char *)pokemon_nombre(buscar_pokemon);
	while (lista_elemento_en_posicion(adversario->lista_pokemon, 0) ==
		       buscar_pokemon ||
	       lista_elemento_en_posicion(adversario->lista_pokemon, 1) ==
		       buscar_pokemon ||
	       lista_elemento_en_posicion(adversario->pokemones_jugador, 0) ==
		       buscar_pokemon ||
	       lista_elemento_en_posicion(adversario->pokemones_jugador, 1) ==
		       buscar_pokemon) {
		indice += 1;
		buscar_pokemon = lista_elemento_en_posicion(
			adversario->lista_pokemon, indice);
		nombre_aux = (char *)pokemon_nombre(buscar_pokemon);
	}
	*nombre3 = nombre_aux;
	lista_t *pokemones = lista_crear();
	lista_insertar(pokemones, pokemon_1);
	lista_insertar(pokemones, pokemon_2);
	lista_insertar(pokemones, pokemon_3);
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
	pokemon_t *pokemon_1 = lista_buscar_elemento(
		adversario->lista_pokemon, comparar_nombres, (void *)nombre1);
	pokemon_t *pokemon_2 = lista_buscar_elemento(
		adversario->lista_pokemon, comparar_nombres, (void *)nombre2);
	pokemon_t *pokemon_3 = lista_buscar_elemento(
		adversario->lista_pokemon, comparar_nombres, (void *)nombre3);
	if (pokemon_1 == NULL || pokemon_2 == NULL || pokemon_3 == NULL) {
		return false; // Si alguno de los Pokémon no existe en la lista
	}
	if ((pokemon_1 == pokemon_2) || (pokemon_1 == pokemon_3) ||
	    (pokemon_2 == pokemon_3)) {
		return false; // Si hay Pokémon repetidos en la selección
	}
	lista_t *pokemones;
	pokemones = lista_crear();
	if (!pokemones) {
		lista_destruir(pokemones);
		return false;
	}
	lista_insertar(pokemones, pokemon_1);
	lista_insertar(pokemones, pokemon_2);
	lista_insertar(pokemones, pokemon_3);
	adversario->pokemones_jugador = pokemones;

	return true;
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE UN STRUCT ATAQUE Y LA LISTA DE ATAQUES QUE SE PUEDEN REALIZAR.
//POST: CONSIGUE EL NOMBRE DEL ATAQUE Y LO INSERTA EN LA LISTA DE LOS ATAQUES QUE SE PUEDEN REALIZAR.
void insertar_ataques(const struct ataque *ataque, void *lista_ataques_posibles)
{
	char *ataque_nombre = (char *)ataque->nombre;
	lista_insertar(lista_ataques_posibles, ataque_nombre);
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE EL ADVERSARIO CREADO
//POST : ASIGNA AL ADVERSARIO LOS POSIBLES ATAQUES QUE PUEDE HACER.
void crear_ataques_posibles(adversario_t *adversario)
{
	if (!adversario->ataques_posibles) {
		adversario->ataques_posibles = lista_crear();
		for (size_t i = 0; i < 3; i++) {
			pokemon_t *pokemon_adversario =
				lista_elemento_en_posicion(
					adversario->pokemones_adversario, i);
			con_cada_ataque(pokemon_adversario, insertar_ataques,
					adversario->ataques_posibles);
		}
		adversario->jugadas_posibles[0].pokemon[0] = '\0';
		adversario->jugadas_posibles[0].pokemon[0] = '\0';
	}
	if (strcmp(adversario->jugadas_posibles[0].pokemon, "") == 0) {
		int indice = 0;
		adversario->turnos = 0;
		for (size_t i = 0; i < 3; i++) {
			pokemon_t *pokemon_adversario =
				lista_elemento_en_posicion(
					adversario->pokemones_adversario, i);
			for (int j = 0; j < 3; j++) {
				strcpy(adversario->jugadas_posibles[indice]
					       .pokemon,
				       pokemon_nombre(pokemon_adversario));
				strcpy(adversario->jugadas_posibles[indice]
					       .ataque,
				       lista_elemento_en_posicion(
					       adversario->ataques_posibles,
					       (size_t)indice));
				indice++;
			}
		}
	}
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t jugada_adversario = { .ataque = "", .pokemon = "" };
	if (!adversario) {
		return jugada_adversario;
	}
	crear_ataques_posibles(adversario);
	strcpy(jugada_adversario.pokemon,
	       adversario->jugadas_posibles[adversario->turnos].pokemon);
	strcpy(jugada_adversario.ataque,
	       adversario->jugadas_posibles[adversario->turnos].ataque);
	adversario->turnos++;
	return jugada_adversario;
}
void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
	lista_destruir(adversario->ataques_posibles);
	lista_destruir(adversario->pokemones_jugador);
	lista_destruir(adversario->pokemones_adversario);
	free(adversario);
}