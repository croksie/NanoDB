#pragma once
#include <memory>
#include "Page.h"
class DiskManager
{
public:
	void writePage(std::shared_ptr<Page> page);
	std::shared_ptr<Page> readPage(int pageId);
	int getNumberOfPages();
};

