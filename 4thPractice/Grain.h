#ifndef GRAIN_H
#define GRAIN_H

#include "Resource.h"

// Зерно: самый дешёвый ресурс.
class Grain : public Resource {
public:
	Grain() : Resource("Grain") {}
	~Grain() override {}
	double baseCost() const override { return 10.0; }
};

#endif
