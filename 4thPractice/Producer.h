#ifndef PRODUCER_H
#define PRODUCER_H

#include <iostream>

#include "Warehouse.h"

// Абстрактный базовый класс производителя (лекция 10).
// Общий интерфейс "работать ход"; каждый наследник производит своё.
class Producer {
public:
	virtual ~Producer() {}

	// чистые виртуальные функции (= 0) => Producer абстрактный
	virtual void processTurn(Warehouse& w) = 0;          // "обработать ход"
	virtual void print(std::ostream& os) const = 0;      // "вывести информацию о себе"
	virtual Producer* clone() const = 0;                 // виртуальное клонирование (для глубокого копирования фермы)

	// кормление: по умолчанию корм не нужен (поле)
	virtual void feed(Warehouse& w) { (void)w; }

	// всё про животных: по умолчанию их тут нет
	virtual int animalPrice() const { return 0; }        // 0 = животных покупать нельзя
	virtual int freeSlots() const { return 0; }
	virtual void addAnimals(int n) { (void)n; }
};

// вывод через оператор <<, внутри - полиморфный вызов print()
std::ostream& operator<<(std::ostream& os, const Producer& p);

#endif
