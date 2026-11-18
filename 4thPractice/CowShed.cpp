#include "CowShed.h"

// молоко дают только накормленные коровы; голодные не дают ничего
void CowShed::processTurn(Warehouse& w) {
	for (int i = 0; i < fed_; ++i) {
		for (int j = 0; j < MILK_PER_COW; ++j) {
			w.milk.add(Milk());
		}
	}
}

void CowShed::print(std::ostream& os) const {
	os << "Коровник: коров " << animals_ << "/" << CAPACITY
	   << ", сытых в прошлый раз " << fed_;
}
