#include "DiskManager.h"
#include <fstream>
#include <filesystem>
#include "DatabaseException.h"

DiskManager::DiskManager()
{
    // use std::fstream because std::ofstream truncates.
    m_dbFile.open("data.db", std::ios::in | std::ios::out | std::ios::binary);
    if (!m_dbFile.is_open()) { // if file not exist
        m_dbFile.clear();
        std::ofstream create_file("data.db", std::ios::binary);
        create_file.close();

        m_dbFile.open("data.db", std::ios::in | std::ios::out | std::ios::binary);
        if (!m_dbFile.is_open()) {
            throw DatabaseException("Can't the read the database file");
        }
    }
}

DiskManager::~DiskManager()
{
    if (m_dbFile.is_open()) {
        m_dbFile.flush();
        m_dbFile.close();
    }
}

void DiskManager::writePage(std::shared_ptr<Page> page)
{
    m_dbFile.clear();
    if (!m_dbFile.is_open()) {
        throw DatabaseException("Can't the read the database file");
    }

    size_t offset = static_cast<size_t>(page->m_pageId) * PAGE_SIZE;

    // check if we need padding
    m_dbFile.seekp(0, std::ios::end);
    std::streampos endPos = m_dbFile.tellp();
    size_t fileSize = (endPos < 0) ? 0 : static_cast<size_t>(endPos);
    if (offset + PAGE_SIZE > fileSize) {
        m_dbFile.seekp(0, std::ios::end);
        std::vector<char> padding(offset + PAGE_SIZE - fileSize, 0);
        m_dbFile.write(padding.data(), padding.size());
    }

    m_dbFile.seekp(offset, std::ios::beg);
    m_dbFile.write(reinterpret_cast<const char*>(page->serialize().data()), PAGE_SIZE);
    m_dbFile.flush();
    if (!m_dbFile.good()) {
        throw DatabaseException("Error when trying to write page");
    }
}

std::shared_ptr<Page> DiskManager::readPage(int pageId)
{
    m_dbFile.clear();
    if (!m_dbFile.is_open()) {
        throw DatabaseException("Can't the read the database file");
    }

    size_t offset = static_cast<size_t>(pageId) * PAGE_SIZE;
    m_dbFile.seekg(offset, std::ios::beg);

    std::vector<uint8_t> buffer(PAGE_SIZE);
    m_dbFile.read(reinterpret_cast<char*>(buffer.data()), PAGE_SIZE);
    if (m_dbFile.gcount() < PAGE_SIZE) {
         m_dbFile.clear();
        throw DatabaseException("Page doesn't exist");
    }
    return std::make_shared<Page>(buffer);
}

int DiskManager::getNumberOfPages()
{
    m_dbFile.clear();
    if (!m_dbFile.is_open()) {
        return 0;
    }
    else {
        m_dbFile.seekg(0, std::ios::end);
        std::streampos fileSize = m_dbFile.tellg();
        if (fileSize < 0) return 0;

        return static_cast<int>(fileSize / PAGE_SIZE);
    }
}
