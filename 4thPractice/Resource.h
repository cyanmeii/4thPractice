#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>

// Абстрактный базовый класс ресурса (зерно, молоко, яйца).
// Лекция 9: динамическая память (char*), конструктор копирования,
//           оператор присваивания, статический счётчик, operator<<.
// Лекция 10: чистая виртуальная функция + виртуальный деструктор.
class Resource {
private:
	char* name_;                 // динамическая строка -> нужно ГЛУБОКОЕ копирование
	static int instanceCount_;   // сколько объектов Resource сейчас живо

public:
	explicit Resource(const char* name = "");
	Resource(const Resource& other);              // конструктор копирования
	Resource& operator=(const Resource& other);   // оператор присваивания
	virtual ~Resource();                          // виртуальный деструктор

	// чистая виртуальная функция => Resource создать нельзя (ошибка компиляции)
	virtual double baseCost() const = 0;

	const char* name() const { return name_; }

	static int HowMany() { return instanceCount_; }

	friend std::ostream& operator<<(std::ostream& os, const Resource& r);
};

#endif
