#include "BufferManager.h"
#include <fstream>

BufferManager::BufferManager() {
	std::ifstream file("data.db", std::ios::binary | std::ios::ate);
	if (file.is_open()) {
		std::streampos size = file.tellg();
		this->nextPageId = static_cast<int>(size / PAGE_SIZE);
		if (this->nextPageId < 2) {
			this->nextPageId = 2;
		}
	} else {
		this->nextPageId = 2;
	}
}

BufferManager::~BufferManager()
{
	this->flushAll();
}

std::shared_ptr<Page> BufferManager::getPage(int pageId)
{
	this->globalClock++;
	auto it = this->bufferPool.find(pageId);
	if (it != this->bufferPool.end()) {
		this->numberOfUseSinceLoaded[pageId]++;
		this->lastUse[pageId] = this->globalClock;
		return it->second;
	}
	else {
		if (this->bufferPool.size() > MAX_LAODED_PAGE) {
			int bestCandidate = 0;
			double bestCandidateScore = 0.0;
			for (auto const& [id, page] : this->bufferPool) {
				uint64_t age = (globalClock - this->lastUse[id]);
				double freqPenalty = 1.0 / (1.0 + this->numberOfUseSinceLoaded[id]);
				double score = static_cast<double>(age) * freqPenalty;
				if (bestCandidateScore < score) {
					bestCandidate = id;
					bestCandidateScore = score;
				}
			}
			this->flushPage(bestCandidate);
		}
		std::shared_ptr<Page> p = this->dm.readPage(pageId);
		this->bufferPool[pageId] = p;
		this->numberOfUseSinceLoaded[pageId] = 1;
		this->lastUse[pageId] = this->globalClock;
		return this->bufferPool[pageId];
	}
}

void BufferManager::flushPage(int pageId)
{
	auto it = this->bufferPool.find(pageId);
	if (it != this->bufferPool.end()) {
		this->dm.writePage(it->second);
		this->bufferPool.erase(pageId);
		this->numberOfUseSinceLoaded.erase(pageId);
		this->lastUse.erase(pageId);
	}
}

void BufferManager::flushAll()
{
	for (auto const& [id, page] : this->bufferPool) {
		this->dm.writePage(page);
	}
	this->bufferPool.clear();
	this->numberOfUseSinceLoaded.clear();
	this->lastUse.clear();
}

void BufferManager::saveNewPage(std::shared_ptr<Page> page)
{
	this->dm.writePage(page);
}

int BufferManager::allocatePage()
{
	return ++this->nextPageId;
}
