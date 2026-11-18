#ifndef MILK_H
#define MILK_H

#include "Resource.h"

// Молоко: дороже зерна.
class Milk : public Resource {
public:
	Milk() : Resource("Milk") {}
	~Milk() override {}
	double baseCost() const override { return 40.0; }
};

#endif
