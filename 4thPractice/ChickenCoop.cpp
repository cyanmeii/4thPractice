#include "ChickenCoop.h"

// яйца дают только накормленные куры; голодные не дают ничего
void ChickenCoop::processTurn(Warehouse& w) {
	for (int i = 0; i < fed_; ++i) {
		for (int j = 0; j < EGGS_PER_CHICKEN; ++j) {
			w.egg.add(Egg());
		}
	}
}

void ChickenCoop::print(std::ostream& os) const {
	os << "Курятник: кур " << animals_ << "/" << CAPACITY
	   << ", накормлено в этом ходу " << fed_ << " из " << animals_;
}
