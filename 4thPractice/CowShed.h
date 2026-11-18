#ifndef COWSHED_H
#define COWSHED_H

#include "AnimalHouse.h"

// Коровник: требует корма, даёт молоко.
class CowShed : public AnimalHouse {
protected:
	int feedNorm() const override { return FEED_NORM; }

public:
	static const int COST = 400;            // цена пустого коровника
	static const int COW_PRICE = 150;       // цена коровы
	static const int FEED_NORM = 2;         // зерна на корову за ход
	static const int MILK_PER_COW = 2;      // молока с коровы за ход

	~CowShed() override {}

	void processTurn(Warehouse& w) override;
	void print(std::ostream& os) const override;
	Producer* clone() const override { return new CowShed(*this); }

	int animalPrice() const override { return COW_PRICE; }
};

#endif
