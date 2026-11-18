#ifndef GAME_H
#define GAME_H

#include <string>

#include "Farm.h"
#include "Queue.h"

// Игровой цикл: ходы, меню игрока, условия победы и проигрыша.
class Game {
private:
	Farm farm_;
	Queue log_;          // журнал последних событий (очередь на списке)
	int turn_;
	int maxTurns_;
	int goalGold_;

	void log(const std::string& text);
	bool readInt(const std::string& prompt, int lo, int hi, int& out);   // false = ввод закончился

	void startTurn();            // шаги 1-3 хода
	bool menu();                 // шаг 4; false = игрок вышел из игры
	void buildMenu();
	void buyAnimalsMenu();
	void sellMenu();
	void report(const std::string& title) const;

public:
	explicit Game(int goalGold = 10000, int maxTurns = 20);
	void run();
};

#endif
