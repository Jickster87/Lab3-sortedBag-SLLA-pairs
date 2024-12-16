#pragma once
#include "SortedBag.h"

class SortedBag;

class SortedBagIterator
{
	friend class SortedBag;

private:
	const SortedBag& bag;
	SortedBagIterator(const SortedBag& b);

	//TODO - Representation
    int currentIndex;
    int currentFreq;
    
public:
	TComp getCurrent();
	bool valid();
	void next();
	void first();
};

