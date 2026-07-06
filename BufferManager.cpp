#include "BufferManager.h"

Page& BufferManager::getPage(int pageId)
{
	for (Page& p : this->bufferPool) {
		if (p.pageId == pageId) {
			return p;
		}
	} 

	return this->bufferPool.at(1);
}
