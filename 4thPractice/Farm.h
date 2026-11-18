#ifndef FARM_H
#define FARM_H

#include <iostream>
#include <string>

#include "Market.h"
#include "Producer.h"
#include "Warehouse.h"

enum BuildingType { BUILD_FIELD = 1, BUILD_COWSHED = 2, BUILD_COOP = 3 };

// Ферма. Лекция 11: КОМПОЗИЦИЯ - ферма СОДЕРЖИТ склады и рынок (has-a).
// Ферма владеет динамическим массивом указателей на Producer, поэтому
// нужно правило трёх (деструктор, конструктор копирования, operator=).
class Farm {
private:
	Warehouse store_;      // склады (зерно, молоко, яйца, золото)
	Market market_;        // рынок встроен в ферму как объект-член
	Producer** items_;     // динамический массив указателей на производителей
	int count_;
	int capacity_;

	void push(Producer* p);

public:
	// итератор по массиву производителей (обход через итератор, как требует ТЗ)
	typedef Producer** iterator;
	typedef Producer* const* const_iterator;

	explicit Farm(int startGold = 500);
	Farm(const Farm& other);               // глубокая копия через clone()
	Farm& operator=(const Farm& other);
	~Farm();

	iterator begin() { return items_; }
	iterator end() { return items_ + count_; }
	const_iterator begin() const { return items_; }
	const_iterator end() const { return items_ + count_; }

	// --- шаги хода ---
	void feedAll();               // 1. животные едят
	void produceAll();            // 2. все производят (полиморфизм)
	void recalculateMarket();     // 3. рынок пересчитывает цены

	// --- действия игрока: true = выполнено, msg = сообщение для игрока ---
	bool build(BuildingType type, std::string& msg);
	bool buyAnimals(int producerIndex, int quantity, std::string& msg);   // индекс с 0
	bool sell(ResourceKind kind, int quantity, std::string& msg);

	// --- делегирование рынку ---
	double price(ResourceKind kind) const { return market_.price(kind); }
	void printMarket(std::ostream& os) const { market_.print(os); }

	int gold() const { return store_.gold.count(); }
	int stock(ResourceKind kind) const;
	int producerCount() const { return count_; }
	const Producer& producer(int index) const { return *items_[index]; }

	void printStatus(std::ostream& os) const;
};

#endif
