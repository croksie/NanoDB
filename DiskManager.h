#pragma once
#include "Page.h"
class DiskManager
{
public:
	void writePage(Page& page);

	Page readPage(int pageId);
};

