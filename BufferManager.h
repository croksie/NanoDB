#pragma once
#include <unordered_map>
#include "DiskManager.h"
#include "Page.h" 

constexpr auto MAX_LAODED_PAGE = 10;

class BufferManager {
	DiskManager dm = DiskManager();

	uint64_t globalClock = 0;

	std::unordered_map<int, Page> bufferPool;
	std::unordered_map<int, uint64_t> numberOfUseSinceLoaded;
	std::unordered_map<int, uint64_t> lastUse;

	int nextPageId = 0;


public:
	Page& getPage(int pageId);
	void flushPage(int pageId);
	void saveNewPage(Page& page);


	int allocatePage();

};

