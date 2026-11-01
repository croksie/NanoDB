#include "BufferManager.h"
#include <fstream>

BufferManager::BufferManager() {
	m_nextPageId = m_diskManager.getNumberOfPages();
	if (m_nextPageId < 2) {
		m_nextPageId = 2;
	}
}

BufferManager::~BufferManager()
{
	this->flushAll();
}

std::shared_ptr<Page> BufferManager::getPage(int pageId)
{
	m_globalClock++;
	auto it = m_bufferPool.find(pageId);
	if (it != m_bufferPool.end()) {
		m_numberOfUseSinceLoaded[pageId]++;
		m_lastUse[pageId] = m_globalClock;
		return it->second;
	}
	else {
		if (m_bufferPool.size() > MAX_LAODED_PAGE) {
			int bestCandidate = 0;
			double bestCandidateScore = 0.0;
			for (auto const& [id, page] : m_bufferPool) {
				uint64_t age = (m_globalClock - m_lastUse[id]);
				double freqPenalty = 1.0 / (1.0 + m_numberOfUseSinceLoaded[id]);
				double score = static_cast<double>(age) * freqPenalty;
				if (bestCandidateScore < score) {
					bestCandidate = id;
					bestCandidateScore = score;
				}
			}
			this->flushPage(bestCandidate);
		}
		std::shared_ptr<Page> p = m_diskManager.readPage(pageId);
		m_bufferPool[pageId] = p;
		m_numberOfUseSinceLoaded[pageId] = 1;
		m_lastUse[pageId] = m_globalClock;
		return m_bufferPool[pageId];
	}
}

void BufferManager::flushPage(int pageId)
{
	auto it = m_bufferPool.find(pageId);
	if (it != m_bufferPool.end()) {
		m_diskManager.writePage(it->second);
		m_bufferPool.erase(pageId);
		m_numberOfUseSinceLoaded.erase(pageId);
		m_lastUse.erase(pageId);
	}
}

void BufferManager::flushAll()
{
	for (auto const& [id, page] : m_bufferPool) {
		m_diskManager.writePage(page);
	}
	m_bufferPool.clear();
	m_numberOfUseSinceLoaded.clear();
	m_lastUse.clear();
}

void BufferManager::saveNewPage(std::shared_ptr<Page> page)
{
	m_diskManager.writePage(page);
}

int BufferManager::allocatePage()
{
	return m_nextPageId++;
}
