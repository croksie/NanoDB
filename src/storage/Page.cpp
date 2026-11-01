#include "Page.h"
#include "DatabaseException.h"

Page::Page() { m_data.assign(PAGE_SIZE, 0); }

Page::Page(int pageId, int prevPageId)
    : m_pageId(pageId), m_prevPageId(prevPageId) {
  m_data.assign(PAGE_SIZE, 0);
}

Page::Page(const std::vector<uint8_t> &data) : m_data(std::move(data)) {

  m_pageId = (static_cast<uint32_t>(m_data.at(0)) << 24) |
             (static_cast<uint32_t>(m_data.at(1)) << 16) |
             (static_cast<uint32_t>(m_data.at(2)) << 8) |
             static_cast<uint32_t>(m_data.at(3));

  m_nextPageId = (static_cast<uint32_t>(m_data.at(4)) << 24) |
                 (static_cast<uint32_t>(m_data.at(5)) << 16) |
                 (static_cast<uint32_t>(m_data.at(6)) << 8) |
                 static_cast<uint32_t>(m_data.at(7));

  m_prevPageId = (static_cast<uint32_t>(m_data.at(8)) << 24) |
                 (static_cast<uint32_t>(m_data.at(9)) << 16) |
                 (static_cast<uint32_t>(m_data.at(10)) << 8) |
                 static_cast<uint32_t>(m_data.at(11));

  m_slotCount = (static_cast<uint32_t>(m_data.at(12)) << 24) |
                (static_cast<uint32_t>(m_data.at(13)) << 16) |
                (static_cast<uint32_t>(m_data.at(14)) << 8) |
                static_cast<uint32_t>(m_data.at(15));
}

std::vector<uint8_t> Page::serialize() const {
  std::vector<uint8_t> result = m_data;

  result.at(0) = static_cast<uint8_t>(m_pageId >> 24);
  result.at(1) = static_cast<uint8_t>(m_pageId >> 16);
  result.at(2) = static_cast<uint8_t>(m_pageId >> 8);
  result.at(3) = static_cast<uint8_t>(m_pageId);

  result.at(4) = static_cast<uint8_t>(m_nextPageId >> 24);
  result.at(5) = static_cast<uint8_t>(m_nextPageId >> 16);
  result.at(6) = static_cast<uint8_t>(m_nextPageId >> 8);
  result.at(7) = static_cast<uint8_t>(m_nextPageId);

  result.at(8) = static_cast<uint8_t>(m_prevPageId >> 24);
  result.at(9) = static_cast<uint8_t>(m_prevPageId >> 16);
  result.at(10) = static_cast<uint8_t>(m_prevPageId >> 8);
  result.at(11) = static_cast<uint8_t>(m_prevPageId);

  result.at(12) = static_cast<uint8_t>(m_slotCount >> 24);
  result.at(13) = static_cast<uint8_t>(m_slotCount >> 16);
  result.at(14) = static_cast<uint8_t>(m_slotCount >> 8);
  result.at(15) = static_cast<uint8_t>(m_slotCount);

  return result;
}

size_t Page::calculateNextTupleOffset() const {
  if (m_slotCount == 0) {
    return PAGE_SIZE - 1;
  } else {
    size_t lastSlotOffset =
        HEADER_SIZE + static_cast<size_t>(m_slotCount - 1) * 4;
    uint16_t lastTupleOffset =
        (static_cast<uint16_t>(m_data.at(lastSlotOffset)) << 8) |
        static_cast<uint16_t>(m_data.at(lastSlotOffset + 1));
    uint16_t lastTupleSize =
        (static_cast<uint16_t>(m_data.at(lastSlotOffset + 2)) << 8) |
        static_cast<uint16_t>(m_data.at(lastSlotOffset + 3));
    return static_cast<size_t>(lastTupleOffset) - lastTupleSize;
  }
}

void Page::insertTuple(Tuple &tuple) {
  std::vector<uint8_t> tupleData = tuple.m_data;

  size_t size = tupleData.size();

  // Reserve the first 16 bytes for the header so it start at tupleData.at(15)

  // Slots representation : 2 bytes for the index + 2 bytes for the size
  size_t nextSlot =
      HEADER_SIZE + (static_cast<size_t>(m_slotCount) *
                     4); // number of slots * size of a slot + header size
  size_t nextTupleOffset = this->calculateNextTupleOffset();
  size_t tupleStart = nextTupleOffset - size;
  if (nextTupleOffset - size < nextSlot + 4) {
    throw DatabaseException("la page est pleine");
  }

  uint16_t tupleSize = static_cast<uint16_t>(size);
  uint16_t tupleOffset = static_cast<uint16_t>(nextTupleOffset);
  m_data.at(nextSlot) = static_cast<uint8_t>(tupleOffset >> 8);
  m_data.at(nextSlot + 1) = static_cast<uint8_t>(tupleOffset & 0xFF);
  m_data.at(nextSlot + 2) = static_cast<uint8_t>(tupleSize >> 8);
  m_data.at(nextSlot + 3) = static_cast<uint8_t>(tupleSize & 0xFF);

  for (size_t i = 0; i < size; ++i) {
    m_data.at(i + tupleStart) = tuple.m_data.at(i);
  }

  m_slotCount++;
}

std::vector<uint8_t> Page::getTuple(int slotIndex) const {
  size_t slotOffset = HEADER_SIZE + static_cast<size_t>(slotIndex) * 4;
  uint16_t tupleOffset = (static_cast<uint16_t>(m_data.at(slotOffset)) << 8) |
                         static_cast<uint16_t>(m_data.at(slotOffset + 1));
  uint16_t tupleSize = (static_cast<uint16_t>(m_data.at(slotOffset + 2)) << 8) |
                       static_cast<uint16_t>(m_data.at(slotOffset + 3));

  std::vector<uint8_t> result;
  result.assign(tupleSize, 0);

  size_t tupleStart = static_cast<size_t>(tupleOffset) - tupleSize;
  for (size_t i = 0; i < tupleSize; ++i) {
    result.at(i) = m_data.at(tupleStart + i);
  }
  return result;
}
