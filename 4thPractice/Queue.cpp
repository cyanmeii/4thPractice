#include "Queue.h"

Queue::Queue(int size) : front_(nullptr), rear_(nullptr), items_(0), qsize_(size) {}

Queue::~Queue() {
	while (front_ != nullptr) {
		Node* old = front_;
		front_ = front_->next;
		delete old;
	}
}

bool Queue::enqueue(const std::string& item) {
	if (isfull()) {
		return false;
	}
	Node* node = new Node(item);
	if (rear_ == nullptr) {
		front_ = node;
	} else {
		rear_->next = node;
	}
	rear_ = node;
	++items_;
	return true;
}

bool Queue::dequeue(std::string& item) {
	if (isempty()) {
		return false;
	}
	Node* old = front_;
	item = old->item;
	front_ = front_->next;
	delete old;
	--items_;
	if (front_ == nullptr) {
		rear_ = nullptr;
	}
	return true;
}

void Queue::print(std::ostream& os) const {
	for (const Node* p = front_; p != nullptr; p = p->next) {
		os << "  - " << p->item << "\n";
	}
}
