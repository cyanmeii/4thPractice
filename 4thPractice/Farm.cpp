#include "Farm.h"

#include "ChickenCoop.h"
#include "CowShed.h"
#include "WheatField.h"

Farm::Farm(int startGold)
	: store_(startGold), market_(), items_(new Producer*[4]), count_(0), capacity_(4) {}

// глубокое копирование: каждого производителя клонируем виртуальным clone()
Farm::Farm(const Farm& other)
	: store_(other.store_), market_(other.market_),
	  items_(new Producer*[other.capacity_]), count_(0), capacity_(other.capacity_) {
	for (const_iterator it = other.begin(); it != other.end(); ++it) {
		items_[count_++] = (*it)->clone();
	}
}

Farm& Farm::operator=(const Farm& other) {
	if (this == &other) {
		return *this;
	}
	Producer** copy = new Producer*[other.capacity_];
	int n = 0;
	for (const_iterator it = other.begin(); it != other.end(); ++it) {
		copy[n++] = (*it)->clone();
	}
	for (iterator it = begin(); it != end(); ++it) {
		delete *it;
	}
	delete[] items_;

	store_ = other.store_;
	market_ = other.market_;
	items_ = copy;
	count_ = n;
	capacity_ = other.capacity_;
	return *this;
}

Farm::~Farm() {
	for (iterator it = begin(); it != end(); ++it) {
		delete *it;        // виртуальный деструктор удалит объект правильного типа
	}
	delete[] items_;
}

void Farm::push(Producer* p) {
	if (count_ == capacity_) {
		int newCapacity = capacity_ * 2;
		Producer** bigger = new Producer*[newCapacity];
		for (int i = 0; i < count_; ++i) {
			bigger[i] = items_[i];
		}
		delete[] items_;
		items_ = bigger;
		capacity_ = newCapacity;
	}
	items_[count_++] = p;
}

// ---------------- шаги хода ----------------

void Farm::feedAll() {
	for (iterator it = begin(); it != end(); ++it) {
		(*it)->feed(store_);
	}
}

void Farm::produceAll() {
	for (iterator it = begin(); it != end(); ++it) {
		(*it)->processTurn(store_);      // полиморфный вызов
	}
}

void Farm::recalculateMarket() {
	market_.recalculate();
}

// ---------------- действия игрока ----------------

bool Farm::build(BuildingType type, std::string& msg) {
	int cost = 0;
	const char* title = "";
	switch (type) {
	case BUILD_FIELD:   cost = WheatField::COST; title = "пшеничное поле"; break;
	case BUILD_COWSHED: cost = CowShed::COST;    title = "коровник";       break;
	case BUILD_COOP:    cost = ChickenCoop::COST; title = "курятник";      break;
	default:
		msg = "Неизвестный тип постройки.";
		return false;
	}

	if (!store_.gold.spend(cost)) {
		msg = std::string("Не хватает золота на постройку (") + title + "): нужно " +
		      std::to_string(cost) + ", есть " + std::to_string(store_.gold.count()) + ".";
		return false;
	}

	Producer* p = nullptr;
	if (type == BUILD_FIELD) {
		p = new WheatField();
	} else if (type == BUILD_COWSHED) {
		p = new CowShed();
	} else {
		p = new ChickenCoop();
	}
	push(p);
	msg = std::string("Построено: ") + title + " за " + std::to_string(cost) + " зол.";
	return true;
}

bool Farm::buyAnimals(int index, int quantity, std::string& msg) {
	if (index < 0 || index >= count_) {
		msg = "Такого объекта нет.";
		return false;
	}
	Producer& p = *items_[index];
	int price = p.animalPrice();
	if (price == 0) {
		msg = "В этом объекте животных держать нельзя.";
		return false;
	}
	if (quantity <= 0) {
		msg = "Количество должно быть положительным.";
		return false;
	}
	if (quantity > p.freeSlots()) {
		msg = "Не хватает места: свободно мест - " + std::to_string(p.freeSlots()) + ".";
		return false;
	}
	int total = price * quantity;
	if (!store_.gold.spend(total)) {
		msg = "Не хватает золота: нужно " + std::to_string(total) + ", есть " +
		      std::to_string(store_.gold.count()) + ".";
		return false;
	}
	p.addAnimals(quantity);
	msg = "Куплено животных: " + std::to_string(quantity) + " за " + std::to_string(total) + " зол.";
	return true;
}

// Общая логика продажи для любого Storage<T> (шаблонная функция, видна только в этом .cpp)
template <typename T>
static bool sellFrom(Storage<T>& storage, Storage<Gold>& gold, Market& market,
                     ResourceKind kind, int quantity, std::string& msg) {
	if (quantity <= 0) {
		msg = "Количество должно быть положительным.";
		return false;
	}
	if (storage.count() < quantity) {
		msg = std::string("На складе недостаточно: ") + resourceTitle(kind) + " - " +
		      std::to_string(storage.count()) + " шт.";
		return false;
	}
	int revenue = static_cast<int>(market.price(kind) * quantity);
	storage.remove(quantity);
	gold.add(revenue);
	market.recordSale(kind, quantity);
	msg = std::string("Продано: ") + resourceTitle(kind) + " x" + std::to_string(quantity) +
	      ", выручка " + std::to_string(revenue) + " зол.";
	return true;
}

bool Farm::sell(ResourceKind kind, int quantity, std::string& msg) {
	switch (kind) {
	case GRAIN: return sellFrom(store_.grain, store_.gold, market_, kind, quantity, msg);
	case MILK:  return sellFrom(store_.milk, store_.gold, market_, kind, quantity, msg);
	case EGG:   return sellFrom(store_.egg, store_.gold, market_, kind, quantity, msg);
	default:
		msg = "Неизвестный ресурс.";
		return false;
	}
}

int Farm::stock(ResourceKind kind) const {
	switch (kind) {
	case GRAIN: return store_.grain.count();
	case MILK:  return store_.milk.count();
	case EGG:   return store_.egg.count();
	default:    return 0;
	}
}

void Farm::printStatus(std::ostream& os) const {
	os << "----- Ферма -----\n";
	os << "Золото: " << store_.gold.count() << "\n";
	os << "Склад: зерно " << store_.grain.count()
	   << ", молоко " << store_.milk.count()
	   << ", яйца " << store_.egg.count() << "\n";
	os << "Объекты (" << count_ << "):\n";
	int number = 1;
	for (const_iterator it = begin(); it != end(); ++it, ++number) {
		os << "  " << number << ". " << **it << "\n";    // operator<< -> print()
	}
	market_.print(os);
}
