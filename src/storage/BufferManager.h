#pragma once
#include <unordered_map>
#include <memory>
#include "DiskManager.h"
#include "Page.h" 

constexpr auto MAX_LAODED_PAGE = 10;

class BufferManager {
	DiskManager m_diskManager;

	uint64_t m_globalClock = 0;

	std::unordered_map<int, std::shared_ptr<Page>> m_bufferPool;
	std::unordered_map<int, uint64_t> m_numberOfUseSinceLoaded;
	std::unordered_map<int, uint64_t> m_lastUse;

	int m_nextPageId = 0;


public:
	BufferManager();
	~BufferManager();
	std::shared_ptr<Page> getPage(int pageId);
	void flushPage(int pageId);
	void flushAll();
	void saveNewPage(std::shared_ptr<Page> page);


	int allocatePage();

};

