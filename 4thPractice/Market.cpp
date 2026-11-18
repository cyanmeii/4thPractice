#include "Market.h"

#include <iomanip>

#include "Egg.h"
#include "Grain.h"
#include "Milk.h"

const char* resourceTitle(ResourceKind kind) {
	switch (kind) {
	case GRAIN: return "зерно";
	case MILK:  return "молоко";
	case EGG:   return "яйца";
	default:    return "?";
	}
}

Market::Market() {
	// базовые цены берём у самих ресурсов (виртуальный baseCost)
	Grain grain;
	Milk milk;
	Egg egg;
	const Resource* samples[KIND_COUNT] = { &grain, &milk, &egg };

	for (int k = 0; k < KIND_COUNT; ++k) {
		basePrice_[k] = samples[k]->baseCost();
		price_[k] = basePrice_[k];
		soldNow_[k] = 0;
		soldHistory_[k] = History(HISTORY_TURNS, 0.0);
		priceHistory_[k] = History(HISTORY_TURNS, basePrice_[k]);
	}
}

void Market::recordSale(ResourceKind kind, int quantity) {
	soldNow_[kind] += quantity;
}

// Вызывается в начале хода: учитывает продажи ПРОШЛОГО хода и пересчитывает цены.
void Market::recalculate() {
	for (int k = 0; k < KIND_COUNT; ++k) {
		soldHistory_[k].push(soldNow_[k]);
		soldNow_[k] = 0;

		double volume = soldHistory_[k].sum();            // встроенная операция valarray
		double factor = 1.0 - DROP_PER_UNIT * volume;
		if (factor < MIN_FACTOR) {
			factor = MIN_FACTOR;
		}
		price_[k] = basePrice_[k] * factor;
		priceHistory_[k].push(price_[k]);
	}
}

void Market::print(std::ostream& os) const {
	os << "Рынок (цена за единицу):\n";
	os << std::fixed << std::setprecision(1);
	for (int k = 0; k < KIND_COUNT; ++k) {
		os << "  " << std::left << std::setw(7) << resourceTitle(static_cast<ResourceKind>(k))
		   << std::right << std::setw(6) << price_[k] << " зол."
		   << "  | история: средняя " << priceHistory_[k].average()
		   << ", мин " << priceHistory_[k].min()
		   << ", макс " << priceHistory_[k].max()
		   << "  | продано за последние ходы: " << static_cast<int>(soldHistory_[k].sum())
		   << "\n";
	}
	os.unsetf(std::ios_base::floatfield);
}
