// Functions (operations on the player).

// Decreases the body temperature of the player.
void decreaseBodyTemp(Player *player, double temp) {
	player->bodyTemp -= temp;
}

// Increases the body temperature of the player.
void increaseBodyTemp(Player *player, double temp) {
	player->bodyTemp += temp;
}

// Prints the vital stats of the player.
void printStats(Player *currentPlayer) {
	printf("\nWakefulness = %d\n", currentPlayer->wakefulness);
	printf("Body temperature = %.2f\n\n", currentPlayer->bodyTemp);
}

// Function for picking up items.
void pickUp(Player *player, int item) {
	player->itemSlots[item] = item;
}

// Function for making the player drink a red bull.
void drinkRedBull(Player *player) {
	if (player->itemSlots[5] == 5) {
		player->wakefulness += 70;
		printf("\nYou drank a red bull.\n");
		printf("Your wakefulness rose!\n\n");
		player->itemSlots[5] = 0;
	} else {
		printf("You dont have any red bull left.\n\n");
	}
}

// Ignites the fireplace.
void igniteFireplace(Player *player, bool *fireplaceIgnited) {
	if (player->itemSlots[2] == 2) {
		*fireplaceIgnited = true;
		printf("\nYou have ignited the fireplace!\n");
		printf("Walk around the cabin to get warm.\n\n");
	} else {
		printf("\nTo make up a fire, you need something to light it up with!\n\n");
	}
}

// Function for "winning".
void win() {
	printf("**********************************\n");
	printf("	You are winner!		\n");
	printf("**********************************\n");
	exit(0);
}