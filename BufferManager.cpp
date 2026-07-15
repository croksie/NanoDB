#include "BufferManager.h"

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

void BufferManager::saveNewPage(std::shared_ptr<Page> page)
{
	//std::shared_ptr<Page> prevPage = this->getPage(page->prevPageId);
	//prevPage->nextPageId = page->pageId;
	this->dm.writePage(page);
	//this->flushPage(prevPage->pageId);
}

int BufferManager::allocatePage()
{
	return ++this->nextPageId;
}
