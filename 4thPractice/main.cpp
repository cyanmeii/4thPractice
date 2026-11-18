#include "Game.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);   // чтобы русский текст не превращался в "кракозябры"
#endif
	Game game;
	game.run();
	return 0;
}
