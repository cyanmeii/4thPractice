#ifndef WHEATFIELD_H
#define WHEATFIELD_H

#include "Producer.h"

// Пшеничное поле: корма не требует, каждый ход даёт зерно.
class WheatField : public Producer {
public:
	static const int COST = 200;           // цена постройки
	static const int GRAIN_PER_TURN = 5;   // урожай за ход

	~WheatField() override {}

	void processTurn(Warehouse& w) override;
	void print(std::ostream& os) const override;
	Producer* clone() const override { return new WheatField(*this); }
};

#endif
