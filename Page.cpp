#include "Page.h"
#include "DatabaseException.h"


Page::Page() {
	this->data.assign(PAGE_SIZE, std::byte{ 0 });
}

Page::Page(int pageId, int prevPageId)
	: pageId(pageId), prevPageId(prevPageId) 
{
	this->data.assign(PAGE_SIZE, std::byte{ 0 });
}

Page::Page(std::vector<std::byte> data){
	this->data.assign(PAGE_SIZE, std::byte{ 0 });

	this->pageId = (static_cast<uint32_t>(data.at(0)) << 24) |
				   (static_cast<uint32_t>(data.at(1)) << 16) |
				   (static_cast<uint32_t>(data.at(2)) << 8) |
					static_cast<uint32_t>(data.at(3));

	this->nextPageId = (static_cast<uint32_t>(data.at(4)) << 24) |
					   (static_cast<uint32_t>(data.at(5)) << 16) |
					   (static_cast<uint32_t>(data.at(6)) << 8) |
						static_cast<uint32_t>(data.at(7));

	this->prevPageId = (static_cast<uint32_t>(data.at(8)) << 24) |
					   (static_cast<uint32_t>(data.at(9)) << 16) |
					   (static_cast<uint32_t>(data.at(10)) << 8) |
					    static_cast<uint32_t>(data.at(11));

	this->slotCount = (static_cast<uint32_t>(data.at(12)) << 24) |
					  (static_cast<uint32_t>(data.at(13)) << 16) |
					  (static_cast<uint32_t>(data.at(14)) << 8) |
					   static_cast<uint32_t>(data.at(15));

	for (int i = HEADER_SIZE; i < PAGE_SIZE; ++i) {
		this->data.at(i) = data.at(i);
	}
}


std::vector<std::byte> Page::serialize() const
{
	std::vector<std::byte> result;
	result.assign(PAGE_SIZE, std::byte{ 0 });

	result.at(0) = static_cast<std::byte>(pageId >> 24);
	result.at(1) = static_cast<std::byte>(pageId >> 16);
	result.at(2) = static_cast<std::byte>(pageId >> 8);
	result.at(3) = static_cast<std::byte>(pageId);

	result.at(4) = static_cast<std::byte>(nextPageId >> 24);
	result.at(5) = static_cast<std::byte>(nextPageId >> 16);
	result.at(6) = static_cast<std::byte>(nextPageId >> 8);
	result.at(7) = static_cast<std::byte>(nextPageId);

	result.at(8) = static_cast<std::byte>(prevPageId >> 24);
	result.at(9) = static_cast<std::byte>(prevPageId >> 16);
	result.at(10) = static_cast<std::byte>(prevPageId >> 8);
	result.at(11) = static_cast<std::byte>(prevPageId);

	result.at(12) = static_cast<std::byte>(slotCount >> 24);
	result.at(13) = static_cast<std::byte>(slotCount >> 16);
	result.at(14) = static_cast<std::byte>(slotCount >> 8);
	result.at(15) = static_cast<std::byte>(slotCount);

	for (int i = HEADER_SIZE; i < PAGE_SIZE; ++i) {
		result.at(i) = this->data.at(i);
	}

	return result;
}




int Page::calculateNextTupleOffset() const
{
	if (slotCount == 0) {
		return PAGE_SIZE - 1;
	}
	else {
		int lastSlotOffset = HEADER_SIZE + (this->slotCount - 1) * 4;
		uint16_t lastTupleOffset = (static_cast<uint16_t>(this->data.at(lastSlotOffset)) << 8) |
			static_cast<uint16_t>(this->data.at(lastSlotOffset + 1));
		uint16_t lastTupleSize = (static_cast<uint16_t>(this->data.at(lastSlotOffset + 2)) << 8) |
			static_cast<uint16_t>(this->data.at(lastSlotOffset + 3));
		return lastTupleOffset - lastTupleSize;
	}
}

void Page::insertTuple(Tuple& tuple)
{
	std::vector<std::byte> tupleData = tuple.data;

	size_t size = tupleData.size();

	// Réserver les 16 premiers octets pour l'entête donc début = tupleData.at(15)

	// Représentation des slots : 2 octes pour index + 2 octets pour la taille
	int nextSlot = HEADER_SIZE + (this->slotCount * 4); // number of slots * size of a slot + header size
	int nextTupleOffset = this->calculateNextTupleOffset();
	size_t tupleStart = nextTupleOffset - size;
	if (nextTupleOffset - size < nextSlot + 4) {
		throw DatabaseException("la page est pleine");
	}

	uint16_t tupleSize = static_cast<uint16_t>(size);
	uint16_t tupleOffset = static_cast<uint16_t>(nextTupleOffset);
	this->data.at(nextSlot) = static_cast<std::byte>(tupleOffset >> 8);
	this->data.at(nextSlot + 1) = static_cast<std::byte>(tupleOffset & 0xFF);
	this->data.at(nextSlot + 2) = static_cast<std::byte>(tupleSize >> 8);
	this->data.at(nextSlot + 3) = static_cast<std::byte>(tupleSize & 0xFF);

	for (int i = 0; i < size; ++i) {
		this->data.at(i + tupleStart) = tuple.data.at(i);
	}

	this->slotCount++;
}

std::vector<std::byte> Page::getTuple(int slotIndex) const
{
	size_t slotOffset = HEADER_SIZE + static_cast<size_t>(slotIndex * 4);
	uint16_t tupleOffset = (static_cast<uint16_t>(this->data.at(slotOffset)) << 8) |
		static_cast<uint16_t>(this->data.at(slotOffset + 1));
	uint16_t tupleSize = (static_cast<uint16_t>(this->data.at(slotOffset + 2)) << 8) |
		static_cast<uint16_t>(this->data.at(slotOffset + 3));

	std::vector<std::byte> result;
	result.assign( tupleSize, std::byte{ 0 });

	uint16_t tupleStart = tupleOffset - tupleSize;
	for (int i = 0; i < tupleSize; ++i) {
		result.at(i) = this->data.at(static_cast<size_t>(tupleStart + i));
	}
	return result;
}


