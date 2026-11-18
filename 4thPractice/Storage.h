#ifndef STORAGE_H
#define STORAGE_H

#include "Gold.h"

// Лекция 11: шаблон класса. ВСЯ реализация - только в заголовке (требование ТЗ).
// Хранит "штуки" ресурса одного типа в динамическом массиве.
// Правило трёх: деструктор + конструктор копирования + operator=.
template <typename T>
class Storage {
private:
	T* data_;
	int count_;
	int capacity_;

	void grow() {                       // массив полон -> увеличиваем вдвое
		int newCapacity = capacity_ * 2;
		T* bigger = new T[newCapacity];
		for (int i = 0; i < count_; ++i) {
			bigger[i] = data_[i];
		}
		delete[] data_;
		data_ = bigger;
		capacity_ = newCapacity;
	}

public:
	explicit Storage(int capacity = 16)
		: data_(nullptr), count_(0), capacity_(capacity > 0 ? capacity : 1) {
		data_ = new T[capacity_];
	}

	Storage(const Storage& other)       // глубокое копирование
		: data_(new T[other.capacity_]), count_(other.count_), capacity_(other.capacity_) {
		for (int i = 0; i < count_; ++i) {
			data_[i] = other.data_[i];
		}
	}

	Storage& operator=(const Storage& other) {
		if (this != &other) {
			T* copy = new T[other.capacity_];
			for (int i = 0; i < other.count_; ++i) {
				copy[i] = other.data_[i];
			}
			delete[] data_;
			data_ = copy;
			count_ = other.count_;
			capacity_ = other.capacity_;
		}
		return *this;
	}

	~Storage() { delete[] data_; }

	// добавление
	void add(const T& item) {
		if (count_ == capacity_) {
			grow();
		}
		data_[count_++] = item;
	}

	// извлечение одной штуки (с конца); false, если склад пуст
	bool take(T& out) {
		if (count_ == 0) {
			return false;
		}
		out = data_[--count_];
		return true;
	}

	// списать ровно n штук; если столько нет - не списываем ничего и возвращаем false
	bool remove(int n) {
		if (n < 0 || n > count_) {
			return false;
		}
		count_ -= n;
		return true;
	}

	// подсчёт
	int count() const { return count_; }
};

// Явная (полная) специализация для золота: золото не портится, это просто число,
// поэтому массив не нужен, а интерфейс другой (add(int), spend(int)).
// Динамической памяти нет, значит правило трёх тут не требуется.
template <>
class Storage<Gold> {
private:
	int amount_;

public:
	explicit Storage(int amount = 0) : amount_(amount) {}

	void add(int n) {
		if (n > 0) {
			amount_ += n;
		}
	}

	bool spend(int n) {
		if (n < 0 || n > amount_) {
			return false;
		}
		amount_ -= n;
		return true;
	}

	int count() const { return amount_; }
};

#endif
