#include "Producer.h"

std::ostream& operator<<(std::ostream& os, const Producer& p) {
	p.print(os);
	return os;
}
