#pragma once
#include <vector>
#include "Tuple.h"

constexpr auto PAGE_SIZE = 4096;
constexpr auto HEADER_SIZE = 16;

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
	Page(std::vector<std::byte> data);
	Page(int pageId, int prevPageId);

	std::vector<std::byte> serialize() const;

	void insertTuple(Tuple& tuple);
	std::vector<std::byte> getTuple(int slotIndex) const;
	int calculateNextTupleOffset() const;

};

