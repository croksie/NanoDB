#pragma once
#include <vector>
#include <cstdint>


class Page
{
private:
	int nextPageId;

	std::vector<int> slotDirectory;
	std::vector<uint8_t> data;

public:
	const int pageId= 0;
	const int prevPageId=0;
	Page();

};

