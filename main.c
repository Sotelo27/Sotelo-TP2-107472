#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include "stdlib.h"
#include "src/hash.h"
#include "src/menu.h"
#include <stdio.h>
#include <string.h>

struct estado_juego{
	juego_t * juego;
	menu_t* menu;
	bool continuar;
	bool juego_iniciado;
};

bool verificar_ataque(char *ataques_jugador[], char *ataque_nombre,int limite) {
    for (int i = 0; ataques_jugador[i] != NULL; i++) {
       if(ataque_nombre == ataques_jugador[i]){
	return true;
       }
    }
    return false; 
}

bool mostrar_pokemon(void *p,void * contexto){
	printf("Nombre: %s\n",pokemon_nombre(p));
	return true;
}

bool cargar_archivo(void *e){
	if (!e){
		return false;
	}
	struct estado_juego *estado = e;
	if (!estado){
		return false;
	}
	JUEGO_ESTADO archivo_estado;
	printf("Ingresa el nombre del archivo a continuacion:\n");
	char linea[200];
	if (fgets(linea, sizeof(linea), stdin) != NULL) {
		size_t longitud = strlen(linea);
	
		if (longitud > 0 && linea[longitud - 1] == '\n') {
			linea[longitud - 1] = '\0';
		}
	}
	archivo_estado = juego_cargar_pokemon(estado->juego,linea);
	if(archivo_estado == TODO_OK){
		printf("\nEl archivo se cargo correctamente\n");
		return true;
	}else if(archivo_estado == POKEMON_INSUFICIENTES){
		printf("\nEl archivo tiene pokemones insuficientes para el juego \n");
	}else if(archivo_estado == ERROR_GENERAL){
		printf("\nEL archivo no se cargo correctamente \n");
	}
	return true;
}

bool listar_pokemones(void *e){
	if (!e){
		return false;
	}
	struct estado_juego *estado = e;
	if(!estado){
		return false;
	}
	if(lista_tamanio(juego_listar_pokemon(estado->juego)) == 0){
		printf("\nAun no se a cargado ningun archivo, no hay pokemones por mostrar.\n");
		return true;
	}
	lista_con_cada_elemento(juego_listar_pokemon(estado->juego),mostrar_pokemon,NULL);
	return true;
}

bool seleccionar_pokemones_jugador(char **eleccion_jugador_1,char **eleccion_jugador_2,char **eleccion_jugador_3){;
	char nombre_1[20];
	char nombre_2[20];
	char nombre_3[20];

	printf("Jugador 1, ingresa tu elección:\n");
	fgets(nombre_1, sizeof(nombre_1), stdin);
	nombre_1[strlen(nombre_1) - 1] = '\0'; 

	printf("Jugador 2, ingresa tu elección:\n");
	fgets(nombre_2, sizeof(nombre_2), stdin);
	nombre_2[strlen(nombre_2) - 1] = '\0'; 

	printf("Jugador 3, ingresa tu elección:\n");
	fgets(nombre_3, sizeof(nombre_3), stdin);
	nombre_3[strlen(nombre_3) - 1] = '\0';
	strcpy(*eleccion_jugador_1, nombre_1);
	strcpy(*eleccion_jugador_2, nombre_2);
	strcpy(*eleccion_jugador_3, nombre_3);
	/*
	estado_seleccion = juego_seleccionar_pokemon(estado->juego, JUGADOR1, eleccionJugador1,
				  eleccionJugador2, eleccionJugador3);
	if(estado_seleccion == POKEMON_INEXISTENTE){
		printf("\nHas ingresado un pokemon que no se encuentra en la lista cargada, reintenta nuevamente.\n");
	}else if (estado_seleccion == POKEMON_REPETIDO)
	{
		printf("\nUno de los pokemon que has ingresado se encuentra repetido\n");
	}else if (estado_seleccion == ERROR_GENERAL)
	{
		printf("\nHa habido un error al ingresarse,reintenta nuevamente\n");
	}else if (estado_seleccion == TODO_OK)
	{
		printf("\nSe a cargado correctamente los pokemon\n");
	}
	adversario_t *adversario = adversario_crear(juego_listar_pokemon(estado->juego));
	adversario_pokemon_seleccionado(adversario,eleccionJugador1,eleccionJugador2,eleccionJugador3);
	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
	adversario_seleccionar_pokemon(adversario,&eleccionAdversario1,&eleccionAdversario2,&eleccionAdversario3);
	juego_seleccionar_pokemon(estado->juego,JUGADOR2,eleccionAdversario1,eleccionAdversario2,eleccionAdversario3);
	*/
	return true;
}

void ingresar_nombres(char nombre_1[], char nombre_2[], char nombre_3[]) {
    printf("Jugador ingresa el primer pokemon:\n");
    fgets(nombre_1, 20, stdin);
    nombre_1[strlen(nombre_1) - 1] = '\0';

    printf("Jugador ingresa el segundo pokemon:\n");
    fgets(nombre_2, 20, stdin);
    nombre_2[strlen(nombre_2) - 1] = '\0';

    printf("Jugador ingresa el tercer pokemon:\n");
    fgets(nombre_3, 20, stdin);
    nombre_3[strlen(nombre_3) - 1] = '\0';
}

bool mostrar_tipo_pokemon(){
	return true;
}

bool mostrar_ataques_pokemon(){
	return true;
}

void mostrar_error_seleccion(JUEGO_ESTADO estado_seleccion){
	if(estado_seleccion == POKEMON_INEXISTENTE){
		printf("\nHas ingresado un pokemon que no se encuentra en la lista cargada, reintenta nuevamente.\n");
	}else if (estado_seleccion == POKEMON_REPETIDO)
	{
		printf("\nUno de los pokemon que has ingresado se encuentra repetido\n");
	}else if (estado_seleccion == ERROR_GENERAL)
	{
		printf("\nHa habido un error al ingresarse,reintenta nuevamente\n");
	}else if (estado_seleccion == TODO_OK)
	{
		printf("\nSe a cargado correctamente los pokemon\n");
	}
}

bool jugar(void * e){
	if (!e){
		return false;
	}
	struct estado_juego *estado = e;
	if(!estado){
		return false;
	}
	if(lista_tamanio(juego_listar_pokemon(estado->juego)) == 0){
		printf("\nAun no se a cargado ningun archivo, no se puede jugar.\n");
		return true;
	}
	printf("\n---------!Juego iniciado--------\n");
	printf("\n----------Selecciona los pokemon para continuar, a continuacion ingresa el comando------------\n");
	char nombre_1[20],nombre_2[20],nombre_3[20];
	JUEGO_ESTADO estado_seleccion = ERROR_GENERAL;
	while (estado_seleccion != TODO_OK){
		ingresar_nombres(nombre_1, nombre_2, nombre_3);
		estado_seleccion = juego_seleccionar_pokemon(estado->juego, JUGADOR1, nombre_1,nombre_2,nombre_3);
		mostrar_error_seleccion(estado_seleccion);
		
	}
	printf("\nPerfecto que comienze el juego!\n");
	while(!juego_finalizado(estado->juego)){

	}
	return true;
}

bool finalizar_juego(void * e){
	
	struct estado_juego * estado = e;
	estado->continuar = false;
	return true;
}


bool mostrar_comandos(){
    printf("\n");
    printf("\t||~~~~~~~~~~~~~~~~~~LISTA DE COMANDOS~~~~~~~~~~~~~~~~||\n");
    printf("\t|| [v] Ver comandos                                  ||\n");
    printf("\t|| [c] Cargar un archivo                             ||\n");
    printf("\t|| [l] Listar pokemones Disponible                   ||\n");
    printf("\t|| [a] Seleccionar jugada                            ||\n");
    printf("\t|| [j] Jugar                                         ||\n");
    printf("\t|| [m] Mostrar ataques de pokemon                    ||\n");
    printf("\t|| [t] Mostrar tipo de pokemon                       ||\n");
    printf("\t|| [q] Finalizar juego                               ||\n");
    printf("\t||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n");
    printf("\n");
    return true;
}

void motrar_reglas(){
	printf("\n------------ Bienvenido al juego de pokemon! -------------------\n");
	printf("\n");
	printf("\nReglas del juego:\n");
	printf("----------------------");
	printf("\n||- Se necesitan 2 jugadores.                                                               ||");
	printf("\n||- Cada jugador elige 3 Pokémon de una lista.                                              ||");
	printf("\n||- Los primeros 2 Pokémon seleccionados son propios, el tercero se asigna del otro jugador.||");
	printf("\n||- El juego consta de 9 rondas.                                                            ||");
	printf("\n||- Cada ronda: un Pokémon y un ataque por jugador.                                         ||");
	printf("\n||- Los puntajes se basan en el tipo de ataque y el Pokémon atacado.                        ||");
	printf("\n||- Al final, gana el jugador con más puntos.                                               ||");
	printf("\n");
	printf("\n¡A disfrutar el juego!\n");
	printf("\n");
}


int main(int argc, char *argv[])
{
	juego_t *j = juego_crear();
	struct estado_juego estado = {.juego = j , .continuar = true , .juego_iniciado = false};
	menu_t *menu = menu_crear();
	menu_agregar_comando(menu,"v","Ver comandos",mostrar_comandos);
	menu_agregar_comando(menu,"l","Listar pokemones",listar_pokemones);
	menu_agregar_comando(menu,"c","Cargar un archivo",cargar_archivo);
	menu_agregar_comando(menu,"t","Mostrar tipo de pokemon",mostrar_tipo_pokemon);
	menu_agregar_comando(menu,"m","Mostrar ataque pokemon",mostrar_ataques_pokemon);
	menu_agregar_comando(menu,"j","Iniciar la partida",jugar);
	menu_agregar_comando(menu,"q","Finalizar juego",finalizar_juego);
	mostrar_comandos();
	while(estado.continuar){
		printf("Ingresa un comando a continuacion\n");
		char linea[200];
		char* leido;
		leido = fgets(linea,200,stdin);
		if(leido == NULL)
			continue;
		linea[strlen(linea)-1] = 0;
		MENU_RESULTADO resultado = menu_ejecutar_comando(menu,linea,&estado);
		if(resultado == MENU_INEXISTENTE){
			printf("\nEse comando no existe.\n");
		}else if(resultado == MENU_ERROR){
			printf("Hubo un error al ejecutar el menu, reintente nuevamente.\n");
		}
	}
	menu_destruir(menu);
}





/*
JUGADOR jugador = JUGADOR1;
		resultado_jugada_t resultado;
		resultado.jugador1 = ATAQUE_ERROR;
		resultado.jugador2 = ATAQUE_ERROR;
		char *nombre1 = "Pikachu";
		char *nombre2 = "Charmander";
		char *nombre3 = "Floatzel";
		JUGADOR jugador2 = JUGADOR2;
		juego_seleccionar_pokemon(j,jugador,nombre1,nombre2,nombre3);
		jugada_t jugada_jugador2 = {"Cacnea", "Absorber"};
		jugada_t jugada_jugador1 = {"Charmander", "Ascuas"};
		char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
		adversario_pokemon_seleccionado(adversario,nombre1,nombre2,nombre3);
		adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
				       &eleccionAdversario2,
				       &eleccionAdversario3);
		juego_seleccionar_pokemon(j,jugador2,eleccionAdversario1,eleccionAdversario2,eleccionAdversario3);
		resultado = juego_jugar_turno(j,jugada_jugador1,jugada_jugador2);
		if(resultado.jugador1 == ATAQUE_EFECTIVO){
			printf("\nNo se cual es el error\n");
			}
		jugada_t jugada_jugador3 = {"Pikachu", "Rayo"};
		jugada_t jugada_jugador4 = {"Floatzel", "Cascada"};
		resultado = juego_jugar_turno(j,jugada_jugador3,jugada_jugador4);
		if(resultado.jugador1 == ATAQUE_EFECTIVO){
			printf("\nNo se cual es el error\n");
			}
		
*/