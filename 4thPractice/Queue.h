#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>

// Лекция 9: очередь как связный список. Здесь хранит журнал событий игры.
class Queue {
private:
	struct Node {
		std::string item;
		Node* next;
		explicit Node(const std::string& value) : item(value), next(nullptr) {}
	};

	Node* front_;
	Node* rear_;
	int items_;
	const int qsize_;

public:
	explicit Queue(int size = 10);
	~Queue();

	// копировать очередь запрещено (идиома "некопируемый класс")
	Queue(const Queue&) = delete;
	Queue& operator=(const Queue&) = delete;

	bool enqueue(const std::string& item);   // добавить в конец
	bool dequeue(std::string& item);         // достать из начала
	bool isempty() const { return items_ == 0; }
	bool isfull() const { return items_ >= qsize_; }
	int count() const { return items_; }

	void print(std::ostream& os) const;      // вывести всё, не вынимая
};

#endif
