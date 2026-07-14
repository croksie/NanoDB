#include "DiskManager.h"
#include <fstream>
#include "DatabaseException.h"

void DiskManager::writePage(Page& page)
{
	std::ofstream dbFile("data.db", std::ios::binary);

    if (!dbFile.is_open()) {
        dbFile.open("data.db", std::ios::binary | std::ios::out);
        if (!dbFile.is_open()) {
            throw DatabaseException("Impossible d'ouvrir le fichier");
        }
    }
    size_t offset = static_cast<size_t>(page.pageId) * PAGE_SIZE;
    dbFile.seekp(offset, std::ios::beg);

    dbFile.seekp(0, std::ios::end);
    size_t fileSize = dbFile.tellp();
    if (offset + 4096 > fileSize) {
        dbFile.seekp(0, std::ios::end);
        dbFile.write("\0", offset + PAGE_SIZE - fileSize);
    }

    dbFile.seekp(offset, std::ios::beg);

    dbFile.write(reinterpret_cast<const char*>(page.serialize().data()), 4096);

    if (!dbFile.good()) {
        throw DatabaseException("Impossible d'ouvrir le fichier");
    }
}

Page DiskManager::readPage(int pageId)
{
    std::ifstream dbFile("data.db", std::ios::binary);
    if (!dbFile.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier");
    }

    size_t offset = static_cast<size_t>(pageId) * PAGE_SIZE;
    dbFile.seekg(offset, std::ios::beg);

    std::vector<uint8_t> buffer(PAGE_SIZE);
    if (!dbFile.read(reinterpret_cast<char*>(buffer.data()), PAGE_SIZE)) {
        size_t bytesRead = dbFile.gcount();
        buffer.resize(bytesRead);
    }
	return Page(buffer);
}
