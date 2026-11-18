#ifndef HISTORY_H
#define HISTORY_H

#include <cstddef>
#include <valarray>

// Лекция 11: ЗАКРЫТОЕ наследование + using.
// История чисел (цены или объёмы продаж) - это valarray<double>,
// но наружу мы открываем только нужные операции: sum, min, max, size.
class History : private std::valarray<double> {
public:
	explicit History(std::size_t n = 4, double fill = 0.0)
		: std::valarray<double>(fill, n) {}

	// открываем выбранные методы базового класса (using без скобок)
	using std::valarray<double>::sum;
	using std::valarray<double>::min;
	using std::valarray<double>::max;
	using std::valarray<double>::size;

	// сдвинуть историю на шаг влево и записать новое значение в конец
	void push(double value) {
		std::valarray<double>& base = *this;
		base = base.cshift(1);
		base[base.size() - 1] = value;
	}

	double average() const {
		return size() > 0 ? sum() / static_cast<double>(size()) : 0.0;
	}
};

#endif
