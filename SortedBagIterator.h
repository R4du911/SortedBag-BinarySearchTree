#pragma once
#include "SortedBag.h"

class SortedBag;

class SortedBagIterator
{
	friend class SortedBag;

private:
	const SortedBag& bag;
	SortedBagIterator(const SortedBag& b);

	int currentnode;
    int currentfrecv;
    BSTNode* stack;

    int nr_keys;

public:
	TComp getCurrent();
	bool valid();
	void next();
	void first();
    void remove_current();
};

