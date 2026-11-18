#include "Game.h"

#include <iomanip>

#include "ChickenCoop.h"
#include "CowShed.h"
#include "Resource.h"
#include "WheatField.h"

using std::cout;

Game::Game(int goalGold, int maxTurns)
	: farm_(500), log_(5), turn_(0), maxTurns_(maxTurns), goalGold_(goalGold) {}

void Game::log(const std::string& text) {
	if (log_.isfull()) {
		std::string old;
		log_.dequeue(old);           // выкидываем самое старое событие
	}
	log_.enqueue("Ход " + std::to_string(turn_) + ": " + text);
}

bool Game::readInt(const std::string& prompt, int lo, int hi, int& out) {
	while (true) {
		cout << prompt;
		std::string line;
		if (!std::getline(std::cin, line)) {
			return false;
		}
		try {
			std::size_t pos = 0;
			int value = std::stoi(line, &pos);
			if (pos == line.size() && value >= lo && value <= hi) {
				out = value;
				return true;
			}
		} catch (...) {
			// падаем сюда на нечисловом вводе - просто просим повторить
		}
		cout << "Ошибка: введите целое число от " << lo << " до " << hi << ".\n";
	}
}

void Game::startTurn() {
	int before[KIND_COUNT];
	for (int k = 0; k < KIND_COUNT; ++k) {
		before[k] = farm_.stock(static_cast<ResourceKind>(k));
	}

	farm_.feedAll();             // 1. животные едят
	farm_.produceAll();          // 2. все производят
	farm_.recalculateMarket();   // 3. пересчёт цен

	cout << "\n==================== Ход " << turn_ << " из " << maxTurns_ << " ====================\n";
	if (turn_ == 1) {
		return;      // в самом начале производить ещё нечему
	}
	cout << "За ночь на складе: ";
	for (int k = 0; k < KIND_COUNT; ++k) {
		int diff = farm_.stock(static_cast<ResourceKind>(k)) - before[k];
		cout << resourceTitle(static_cast<ResourceKind>(k)) << " " << (diff >= 0 ? "+" : "") << diff;
		cout << (k + 1 < KIND_COUNT ? ", " : "\n");
	}
}

void Game::buildMenu() {
	cout << "Что построить?\n"
	     << "  1. Пшеничное поле  (" << WheatField::COST << " зол.)\n"
	     << "  2. Коровник        (" << CowShed::COST << " зол.)\n"
	     << "  3. Курятник        (" << ChickenCoop::COST << " зол.)\n"
	     << "  0. Назад\n";
	int choice = 0;
	if (!readInt("Ваш выбор: ", 0, 3, choice) || choice == 0) {
		return;
	}
	std::string msg;
	farm_.build(static_cast<BuildingType>(choice), msg);
	cout << msg << "\n";
	log(msg);
}

void Game::buyAnimalsMenu() {
	bool any = false;
	for (int i = 0; i < farm_.producerCount(); ++i) {
		if (farm_.producer(i).animalPrice() > 0) {
			cout << "  " << (i + 1) << ". " << farm_.producer(i)
			     << " (цена животного " << farm_.producer(i).animalPrice() << " зол.)\n";
			any = true;
		}
	}
	if (!any) {
		cout << "Сначала постройте коровник или курятник.\n";
		return;
	}
	int index = 0;
	if (!readInt("Номер объекта (0 - назад): ", 0, farm_.producerCount(), index) || index == 0) {
		return;
	}
	int quantity = 0;
	if (!readInt("Сколько животных купить (0 - отмена): ", 0, 1000, quantity) || quantity == 0) {
		return;
	}
	std::string msg;
	farm_.buyAnimals(index - 1, quantity, msg);
	cout << msg << "\n";
	log(msg);
}

void Game::sellMenu() {
	cout << "Что продать?\n" << std::fixed << std::setprecision(1);
	for (int k = 0; k < KIND_COUNT; ++k) {
		ResourceKind kind = static_cast<ResourceKind>(k);
		cout << "  " << (k + 1) << ". " << resourceTitle(kind) << " (на складе "
		     << farm_.stock(kind) << ", цена " << farm_.price(kind) << ")\n";
	}
	cout.unsetf(std::ios_base::floatfield);
	cout << "  0. Назад\n";
	int choice = 0;
	if (!readInt("Ваш выбор: ", 0, KIND_COUNT, choice) || choice == 0) {
		return;
	}
	ResourceKind kind = static_cast<ResourceKind>(choice - 1);
	int quantity = 0;
	if (!readInt("Сколько продать (0 - отмена): ", 0, 1000000, quantity) || quantity == 0) {
		return;
	}
	std::string msg;
	farm_.sell(kind, quantity, msg);
	cout << msg << "\n";
	log(msg);
}

bool Game::menu() {
	while (true) {
		cout << "\nЗолото: " << farm_.gold() << " / " << goalGold_ << "\n"
		     << "1. Построить объект\n"
		     << "2. Купить животных\n"
		     << "3. Продать ресурсы\n"
		     << "4. Состояние фермы и рынка\n"
		     << "5. Пропустить ход (завершить ход)\n"
		     << "6. Завершить игру\n";
		int choice = 0;
		if (!readInt("Ваш выбор: ", 1, 6, choice)) {
			return false;
		}
		switch (choice) {
		case 1: buildMenu(); break;
		case 2: buyAnimalsMenu(); break;
		case 3: sellMenu(); break;
		case 4:
			farm_.printStatus(cout);
			cout << "Последние события:\n";
			if (log_.isempty()) {
				cout << "  (пока пусто)\n";
			} else {
				log_.print(cout);
			}
			break;
		case 5: return true;
		case 6: return false;
		}
	}
}

void Game::report(const std::string& title) const {
	cout << "\n******** " << title << " ********\n";
	cout << "Ходов сыграно: " << turn_ << " из " << maxTurns_ << "\n";
	cout << "Золото: " << farm_.gold() << " (цель " << goalGold_ << ")\n";
	cout << "Объектов на ферме: " << farm_.producerCount() << "\n";
	cout << "Ресурсов в памяти (Resource::HowMany): " << Resource::HowMany() << "\n";
}

void Game::run() {
	cout << "Добро пожаловать на ферму!\n"
	     << "Цель: накопить " << goalGold_ << " золота за " << maxTurns_ << " ходов.\n";

	for (turn_ = 1; turn_ <= maxTurns_; ++turn_) {
		startTurn();                      // шаги 1-3
		if (!menu()) {                    // шаг 4
			report("Игра завершена игроком");
			return;
		}
		// шаг 5: ход завершён, проверяем условия
		if (farm_.gold() >= goalGold_) {
			report("ПОБЕДА!");
			return;
		}
	}
	turn_ = maxTurns_;
	report("ПОРАЖЕНИЕ: ходы закончились, цель не достигнута");
}
