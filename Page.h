#pragma once
#include <vector>

#include "Tuple.h"

class Page
{
private:
	std::vector<int> slotDirectory;
	std::vector<std::byte> data;

public:
	int pageId = 0;
	int nextPageId = 0;
	int prevPageId =0;
	int slotCount = 0;

	Page();

	void insertTuple(Tuple& tuple);
	std::vector<std::byte> getTuple(int slotIndex) const;
	int calculateNextTupleOffset() const;

};

