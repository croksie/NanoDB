#pragma once
#include <vector>
#include <cstdint>
#include "Tuple.h"

constexpr auto PAGE_SIZE = 4096;
constexpr auto HEADER_SIZE = 16;

class Page
{
private:
	std::vector<int> slotDirectory;
	std::vector<uint8_t> data;

public:
	int pageId = 0;
	int nextPageId = 0;
	int prevPageId =0;
	int slotCount = 0;

	Page();
	Page(std::vector<uint8_t> data);
	Page(int pageId, int prevPageId);

	std::vector<uint8_t> serialize() const;

	void insertTuple(Tuple& tuple);
	std::vector<uint8_t> getTuple(int slotIndex) const;
	int calculateNextTupleOffset() const;

};

