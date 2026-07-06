#pragma once
#include <vector>
#include <fstream>
#include "Page.h" 

class BufferManager {
	std::vector<Page> bufferPool;

public:
	Page& getPage(int pageId);
	void flushPage(int pageId);

private:
	void savePage(int pageId);

};

