#pragma once
#include <unordered_map>
#include <memory>
#include "DiskManager.h"
#include "Page.h" 

constexpr auto MAX_LAODED_PAGE = 10;

class BufferManager {
	DiskManager dm = DiskManager();

	uint64_t globalClock = 0;

	std::unordered_map<int, std::shared_ptr<Page>> bufferPool;
	std::unordered_map<int, uint64_t> numberOfUseSinceLoaded;
	std::unordered_map<int, uint64_t> lastUse;

	int nextPageId = 0;


public:
	BufferManager();
	~BufferManager();
	std::shared_ptr<Page> getPage(int pageId);
	void flushPage(int pageId);
	void flushAll();
	void saveNewPage(std::shared_ptr<Page> page);


	int allocatePage();

};

