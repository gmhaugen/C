#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "player.h"
#include "player_operations.h"
#include "environment_operations.h"


// Global variable declarations
Player player;
Player *playerPntr = &player;
bool gameOver;
bool fireplaceIgnited;
int *buf;

// Non-global variables
// Items
static int elevatorCard = 1;
static int lighter = 2;
static int redBull = 5;

// Method declarations
bool initialize();
void gameLoop();
void printStats();
void bedroom();
void livingroom();
void kitchen();
void outside();
void skiElevator();
void shed();

int main() {

	if (initialize(playerPntr)) {
		gameLoop();
	} else {
		fputs("ERROR: Failed to initialize.\n", stderr);
	}

	return 0;
}

// Method for initializing variables.
bool initialize(Player *currentPlayer) {

	gameOver = false;
	fireplaceIgnited = false;

	buf = malloc(sizeof(int));

	// Player initialization
	currentPlayer->wakefulness = 30;
	currentPlayer->bodyTemp = 36.1;

	return true;
}

// This is where the game starts.
// The gameloop function itself is not really a game loop.
// It is more like a function to start the game.
void gameLoop() {
	printf("You wake up on a Sunday morning after a late night,\n");
	printf("with after-ski and poor music. It's cold in the cabin,\n");
	printf("you have not slept much either. It is cold\n");
	printf("and you have to use your lift card before you freeze to death!\n");
	printf("But before doing so, you must be ready!\n");

	getchar();

	while (!gameOver) {		// This while
		bedroom();		// The room to start in.
	}
}

// This function is the bedroom.
void bedroom() {
	fireplaceCheck(fireplaceIgnited, &player);

	printf("You are now in the bedroom.\n");
	printf("1) Livingroom.\n");
	printf("2) Kitchen.\n");
	printf("9) Stats.\n");

	if (player.itemSlots[redBull] == redBull)
		printf("6) Drink redBull.\n");
	
	scanf("%d", &buf[0]);	// Taking in the input.

	// Evaluating input from the user.
	switch (buf[0]) {
		case 1:
			livingroom();
		break;

		case 2:
			kitchen();
		break;

		case 6:
			drinkRedBull(&player);
		break;

		case 9:
			printStats(&player);
		break;

		default:
			printf("Invalid option.\n");
			getchar();
		break;
	}
	bedroom();// Calls the function recursively (unless another function is called).
}

// This function is the livingroom.
void livingroom() {

	fireplaceCheck(fireplaceIgnited, &player);

	printf("You are now in the livingroom.\n");

	// Presenting the alternatives for the player.
	printf("1) Bedroom.\n");
	printf("2) Kitchen.\n");
	printf("3) Outside.\n");

	if (player.itemSlots[redBull] == redBull)
		printf("6) Drink redBull.\n");

	if (!fireplaceIgnited && player.itemSlots[lighter] == lighter)
		printf("7) Light up fireplace.\n");
	else if (!fireplaceIgnited && player.itemSlots[lighter] != lighter)
		printf("0) Pick up the lighter\n");

	printf("9) Show stats.\n");

	scanf("%d", &buf[0]);	// Taking in the input.
	
	// Evaluating input from the user.
	switch (buf[0]) {

		case 0:
			pickUp(&player, lighter);
		break;

		case 1:
			bedroom();
		break;

		case 2:
			kitchen();
		break;

		case 3:
			outside();
		break;

		case 6:
			drinkRedBull(&player);
		break;

		case 7:
			igniteFireplace(&player, &fireplaceIgnited);
		break;

		case 9:
			printStats(&player);
		break;

		default:
			printf("Invalid option.\n");
			getchar();
		break;
	}
	livingroom();	// Calls the function recursively (unless another function is called).
}

// This function is the kitchen.
void kitchen() {

	fireplaceCheck(fireplaceIgnited, &player);	// Check if the fireplace is ignited.

	printf("You are now in the kitchen.\n");

	// Presenting the alternatives for the player.
	printf("1) Bedroom.\n");
	printf("2) Livingroom.\n");

	if (player.itemSlots[redBull] == redBull)
		printf("6) Drink redBull.\n");
	else 
		printf("5) Pick up red bull from the refridgerator.\n");

	printf("9) Show stats.\n");

	scanf("%d", &buf[0]);	// Taking in the input.

	// Evaluating input from the user.
	switch (buf[0]) {
		case 1:
			bedroom();
		break;

		case 2:
			livingroom();
		break;

		case 5:
			pickUp(&player, redBull);
			printf("\nYou picked up a red bull!\n\n");
		break;

		case 6:
			drinkRedBull(&player);
		break;

		case 9:
			printStats(&player);
		break;

		default:	// "If input is not valid."
			printf("Invalid option.\n");
			getchar();
		break;
	}
	kitchen();	// Calls the function recursively (unless another function is called).
}

// This function is outside the cabin.
// !! The player is automatically sent back inside if the body temperature is to low to be outside.
void outside() {

	if (canPlayerGoOutside(&player)) {	// Checks if the player can go outside.
		decreaseBodyTemp(&player, 0.5);
		
		printf("You are now outside.\n");

		// Presenting the alternatives for the player.
		printf("1) Go inside.\n");
		printf("2) Ski elevator.\n");
		printf("3) Enter the shed.\n");

		if (player.itemSlots[redBull] == redBull)
			printf("6) Drink redBull.\n");

		printf("9) Show stats.\n");

		scanf("%d", &buf[0]);	// Taking in the input.
		
		// Evaluating input from the user.
		switch (buf[0]) {
			case 1:
				livingroom();
			break;

			case 2:
				skiElevator();
			break;

			case 3:
				shed();
			break;

			case 6:
				drinkRedBull(&player);
			break;

			case 9:
				printStats(&player);
			break;

			default:	// "If input is not valid."
				printf("Invalid option.\n");
				getchar();
			break;
		}
		outside();// Calls the function recursively (unless another function is called).
	} else {
		printf("\nYou cannot go outside!\nYou are to cold and or not awake enough.\n\n");
		livingroom();	// The player is sent back inside if not enough warm and awake.
	}
}

// This function is inside the shed.
void shed() {
	printf("You are now in the shed.\n");

	// Presenting the alternatives for the user.
	printf("1) Go outside.\n");
	if (player.itemSlots[elevatorCard] != elevatorCard)
		printf("2) Pick up elevator card.\n");
	
	scanf("%d", &buf[0]);	// Taking in the input.

	// Evaluating input fro the user.
	switch (buf[0]) {
		case 1:
			outside();
		break;

		case 2:
			pickUp(&player, elevatorCard);
			getchar();
		break;

		default:	// "If input is not valid."
			printf("Invald option.\n");
		break;
	}
	shed();	// Calls the function recursively (unless another function is called).
}

// This function is the ski elevator. It is not a room or place,
// just a function that evaluates if the user can win or not.
void skiElevator() {
	if (player.itemSlots[elevatorCard] == elevatorCard) {
		win();
	} else {
		printf("\nYou dont have the elevator card!\n\n");
		outside();	// Sends the user back outside.
	}
}