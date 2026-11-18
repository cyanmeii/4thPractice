#ifndef MARKET_H
#define MARKET_H

#include <iostream>

#include "History.h"

enum ResourceKind { GRAIN = 0, MILK = 1, EGG = 2, KIND_COUNT = 3 };

const char* resourceTitle(ResourceKind kind);   // "зерно", "молоко", "яйца"

// Рынок. Цена зависит от объёма продаж за последние ходы:
// чем больше продали - тем ниже цена (перепроизводство сбивает рынок).
class Market {
private:
	static const int HISTORY_TURNS = 4;       // сколько последних ходов учитываем
	static constexpr double DROP_PER_UNIT = 0.0015;   // -0.15% цены за каждую проданную единицу
	static constexpr double MIN_FACTOR = 0.30;       // цена не ниже 30% от базовой

	double basePrice_[KIND_COUNT];
	double price_[KIND_COUNT];
	int soldNow_[KIND_COUNT];                 // продано в текущем ходу
	History soldHistory_[KIND_COUNT];         // объёмы продаж за последние ходы
	History priceHistory_[KIND_COUNT];        // история цен

public:
	Market();

	double price(ResourceKind kind) const { return price_[kind]; }
	void recordSale(ResourceKind kind, int quantity);
	void recalculate();                       // шаг 3 хода
	void print(std::ostream& os) const;
};

#endif
