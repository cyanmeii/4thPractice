#ifndef EGG_H
#define EGG_H

#include "Resource.h"

// Яйца: дороже зерна, дешевле молока.
class Egg : public Resource {
public:
	Egg() : Resource("Egg") {}
	~Egg() override {}
	double baseCost() const override { return 25.0; }
};

#endif
