#pragma once
#include <vector>
#include <cstdint>
#include "Tuple.h"

constexpr auto PAGE_SIZE = 4096;
constexpr auto HEADER_SIZE = 16;

class Page
{
private:
	std::vector<uint8_t> m_data;

public:
	int m_pageId = 0;
	int m_nextPageId = 0;
	int m_prevPageId =0;
	int m_slotCount = 0;

	Page();
	Page(const std::vector<uint8_t>& data);
	Page(int pageId, int prevPageId);

	std::vector<uint8_t> serialize() const;

	void insertTuple(Tuple& tuple);
	std::vector<uint8_t> getTuple(int slotIndex) const;
	size_t calculateNextTupleOffset() const;

};

