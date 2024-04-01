#define DEBUG

#include <Windows.h>
#include <iostream>
#include "header/Memory.hpp"
#include "header/Trainer.hpp"

int main() {
	trainer::Trainer game;

	game.run();

	return 0;
}