// Functions for checking the environment.

// Does a check on the fireplace.
// If ignited, the players body temperature is raised with 0.3 degrees.
// If not, the body temperature is lowered by 0.1 degrees.
void fireplaceCheck(bool fireplaceIgnited, Player *player) {
	if (fireplaceIgnited) {
		increaseBodyTemp(player, 0.3);
	} else {
		decreaseBodyTemp(player, 0.1);
	}
}

// Does a check to see if the player can go outside.
bool canPlayerGoOutside(Player *player) {
	if (player->bodyTemp < 37.5 && player->wakefulness) {
		return false;
	}else {
		return true;
	}
	return false;
}