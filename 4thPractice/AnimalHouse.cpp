#include "AnimalHouse.h"

// Каждое животное пытается забрать свою норму зерна.
// Не хватило зерна -> животное голодное, в этот ход ничего не производит.
void AnimalHouse::feed(Warehouse& w) {
	fed_ = 0;
	for (int i = 0; i < animals_; ++i) {
		if (w.grain.remove(feedNorm())) {
			++fed_;
		}
	}
}
