<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de (Nombre Apellido) - (Padrón) - (Mail)

- Para compilar:

```bash
make pruebas_chanutron
make main
```

- Para ejecutar:

```bash
/pruebas_chanutron
./main

```

- Para ejecutar con valgrind:

```bash
make valgrind-chanutron
valgrind ./main
make
```
---
##  Funcionamiento

En este TP2 se continuo el desarrollo del TP1, en el cual se creo un TDA que permitia cargar un archivo con pokemon y acceder a dicha informacion, que contenia Pokemon, su tipo, y seguidamente 3 ataques que le seguian al mismo.En este TP2 junto a lo hecho en el primer TP y utilizando TDA que vimos en la materia, se desarollo un juego en el que utilizamos dichos pokemones previamente dichos.
Para este juego se pidio 2 jugadores, uno que sera la "maquina" y el otro el usuario, aunque mi implementacion tiene una posibilidad de poder implementar un segundo jugador.Para ello defini un struct en juego.c: 

```c
struct juego {
	lista_t *lista_pokemon;
	informacion_pokemon_t *pokemones_archivo;
	bool estado_juego;
	struct datos_jugador jugador_1;
	struct datos_jugador jugador_2;
	int turnos;
};
```

El struct `juego` guarda una lista_t de los pokemones que se encontraban en el archivo, un `informacion_pokemon_t` que es el mismo archivo, luego un bool `estado_juego` que determina cuando el juego se acaba , un contador de `turnos` para determinar cuando se jugo la cantidad maxima, predeterminada por la cantidad de ataques y pokemon que tenga el usuario.Y por ultimo un `strcut datos_jugador`.

```c
struct datos_jugador {
	int puntos;
	lista_t *pokemones;
	char ataques_utilizados[9][20];
};
```

Guardando los movimientos que utiliza cada jugador en un vector char estatico, el mismo tiene la funcionalidad para poder implementarse otro jugador usuario si se desea (en este caso, `adversario.c` es el otro jugador) con tambien una lista_t que contiene los pokemon del usuario y los puntos que consigue a lo largo de la partida.Determinados por las efectividades de los tipos sobre otros.Los mismos estan definidos en un enum en juego.h.

Por otro lado `adversario.c` tiene el siguiente struct para llevar la informacion de la partida:

```c
struct adversario {
	lista_t *lista_pokemon;
	lista_t *pokemones_jugador;
	lista_t *pokemones_adversario;
	lista_t *ataques_posibles;
	jugada_t jugadas_posibles[9];
	int turnos;
};
```

Guarda en `lista_pokemon` los pokemones con los que puede trabajar, en `pokemones_jugador` los pokemones del usuario, `pokemones_adversario` guarda los pokemones que a seleccionado para el juego,
`ataques_posibles` los char* ataque posibles y en jugada_t `jugadas_posibles[9]` en cada posicion guarda un ataque para realizarlo contra el usuario.Como tambien un contador de turnos.

Explicando por ahora como se conforma cada uno y las condiciones del juego, el juego se desarrolla a lo largo de 9 turnos , donde cada jugador decidira que ataque realizar frente a su contricante, hasta alcanzar el limite de ataques posibles.Prosigo a explicar como se decidio desarrollar en primer caso, el juego, luego el adversario y el main.

### Juego y Jugador :




## Respuestas a las preguntas teóricas
Incluír acá las respuestas a las preguntas del enunciado (si aplica).
