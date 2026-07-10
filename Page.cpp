#include "Page.h"
#include "DatabaseException.h"

#define PAGE_SIZE 4096
#define HEADER_SIZE 16

Page::Page() {
	this->data.assign(PAGE_SIZE, std::byte{ 0 });
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
	try {
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
	catch (const DatabaseException& e){
		std::cerr << e.what();
	}
}

std::vector<std::byte> Page::getTuple(int slotIndex) const
{
	int slotOffset = HEADER_SIZE + (slotIndex * 4);
	uint16_t tupleOffset = (static_cast<uint16_t>(this->data.at(slotOffset)) << 8) |
		static_cast<uint16_t>(this->data.at(slotOffset + 1));
	uint16_t tupleSize = (static_cast<uint16_t>(this->data.at(slotOffset + 2)) << 8) |
		static_cast<uint16_t>(this->data.at(slotOffset + 3));

	std::vector<std::byte> result;
	result.assign( tupleSize, std::byte{ 0 });

	int tupleStart = tupleOffset - tupleSize;
	for (int i = 0; i < tupleSize; ++i) {
		result.at(i) = this->data.at(tupleStart + i);
	}
	return result;
}


