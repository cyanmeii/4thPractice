#ifndef CHICKENCOOP_H
#define CHICKENCOOP_H

#include "AnimalHouse.h"

// Курятник: требует корма, даёт яйца.
class ChickenCoop : public AnimalHouse {
protected:
	int feedNorm() const override { return FEED_NORM; }

public:
	static const int COST = 300;            // цена пустого курятника
	static const int CHICKEN_PRICE = 50;    // цена курицы
	static const int FEED_NORM = 1;         // зерна на курицу за ход
	static const int EGGS_PER_CHICKEN = 1;  // яиц с курицы за ход

	~ChickenCoop() override {}

	void processTurn(Warehouse& w) override;
	void print(std::ostream& os) const override;
	Producer* clone() const override { return new ChickenCoop(*this); }

	int animalPrice() const override { return CHICKEN_PRICE; }
};

#endif
