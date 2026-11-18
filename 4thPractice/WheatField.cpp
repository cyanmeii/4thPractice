#include "WheatField.h"

void WheatField::processTurn(Warehouse& w) {
	for (int i = 0; i < GRAIN_PER_TURN; ++i) {
		w.grain.add(Grain());
	}
}

void WheatField::print(std::ostream& os) const {
	os << "Пшеничное поле (+" << GRAIN_PER_TURN << " зерна за ход)";
}
