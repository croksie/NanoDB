#pragma once
#include <vector>


class Page
{
private:
	int nextPageId;

	std::vector<int> slotDirectory;
	std::vector<std::byte> data;

public:
	const int pageId= 0;
	const int prevPageId=0;
	Page();

};

