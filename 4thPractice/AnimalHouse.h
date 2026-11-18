#ifndef ANIMALHOUSE_H
#define ANIMALHOUSE_H

#include "Producer.h"

// Общая часть коровника и курятника: животные, кормление, вместимость.
// Тоже абстрактный (норму корма и продукцию задают наследники).
class AnimalHouse : public Producer {
protected:                       // protected: видно наследникам, скрыто от внешнего мира
	int animals_;                // сколько животных живёт в постройке
	int fed_;                    // сколько из них накормлено в этом ходу

	AnimalHouse() : animals_(0), fed_(0) {}
	virtual int feedNorm() const = 0;   // зерна на одно животное за ход

public:
	static const int CAPACITY = 20;     // максимум животных в одной постройке

	~AnimalHouse() override {}

	void feed(Warehouse& w) override;   // шаг 1 хода: животные едят со склада

	int freeSlots() const override { return CAPACITY - animals_; }
	void addAnimals(int n) override { animals_ += n; }
};

#endif
