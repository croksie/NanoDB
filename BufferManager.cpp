#include "BufferManager.h"

Page& BufferManager::getPage(int pageId)
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
			for (size_t i = 0; i < this->bufferPool.size(); ++i) {
				uint64_t age = (globalClock - this->lastUse[static_cast<int>(i)]);
				double freqPenalty = 1.0 / (1.0 + this->numberOfUseSinceLoaded[static_cast<int>(i)]);
				double score = static_cast<double>(age) * freqPenalty;
				if (bestCandidateScore < score) {
					bestCandidate = static_cast<int>(i);
					bestCandidateScore = score;
				}
			}
			this->flushPage(bestCandidate);
		}
		Page p = this->dm.readPage(pageId);
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

void BufferManager::saveNewPage(Page& page)
{
	Page& prevPage = this->getPage(page.prevPageId);
	prevPage.nextPageId = page.pageId;
	this->dm.writePage(page);
	this->flushPage(prevPage.pageId);
}

int BufferManager::allocatePage()
{
	return ++this->nextPageId;
}
