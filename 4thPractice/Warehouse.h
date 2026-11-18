#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Egg.h"
#include "Gold.h"
#include "Grain.h"
#include "Milk.h"
#include "Storage.h"

// Все склады фермы в одном месте (композиция: склады - члены этой структуры).
// Каждый Storage<...> - это ОТДЕЛЬНЫЙ класс, порождённый компилятором из шаблона.
struct Warehouse {
	Storage<Grain> grain;
	Storage<Milk> milk;
	Storage<Egg> egg;
	Storage<Gold> gold;     // специализация

	explicit Warehouse(int startGold) : grain(), milk(), egg(), gold(startGold) {}
};

#endif
