#include "Resource.h"

#include <cstring>

// static-поле определяется ровно один раз, в .cpp (слово static тут не пишется)
int Resource::instanceCount_ = 0;

Resource::Resource(const char* name) : name_(nullptr) {
	if (name == nullptr) {
		name = "";
	}
	name_ = new char[std::strlen(name) + 1];
	std::strcpy(name_, name);
	++instanceCount_;
}

// глубокое копирование: выделяем СВОЮ память под имя
Resource::Resource(const Resource& other) : name_(new char[std::strlen(other.name_) + 1]) {
	std::strcpy(name_, other.name_);
	++instanceCount_;
}

Resource& Resource::operator=(const Resource& other) {
	if (this == &other) {   // защита от r = r
		return *this;
	}
	char* copy = new char[std::strlen(other.name_) + 1];
	std::strcpy(copy, other.name_);
	delete[] name_;
	name_ = copy;
	return *this;
}

Resource::~Resource() {
	delete[] name_;
	--instanceCount_;
}

std::ostream& operator<<(std::ostream& os, const Resource& r) {
	return os << r.name_ << " (базовая цена " << r.baseCost() << ")";
}
