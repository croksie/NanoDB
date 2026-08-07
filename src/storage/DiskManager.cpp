#include "DiskManager.h"
#include <fstream>
#include "DatabaseException.h"

void DiskManager::writePage(std::shared_ptr<Page> page)
{
    // use std::fstream because std::ofstream truncates.
    std::fstream dbFile("data.db", std::ios::in | std::ios::out | std::ios::binary);

    if (!dbFile.is_open()) { // if file not exist
        std::ofstream create_file("data.db", std::ios::binary);
        create_file.close();

        dbFile.open("data.db", std::ios::in | std::ios::out | std::ios::binary);
        if (!dbFile.is_open()) {
            throw DatabaseException("Impossible d'ouvrir le fichier data.db");
        }
    }

    size_t offset = static_cast<size_t>(page->pageId) * PAGE_SIZE;

    // check if we need padding
    dbFile.seekp(0, std::ios::end);
    size_t fileSize = dbFile.tellp();
    if (offset + PAGE_SIZE > fileSize) {
        dbFile.seekp(0, std::ios::end);
        std::vector<char> padding(offset + PAGE_SIZE - fileSize, 0);
        dbFile.write(padding.data(), padding.size());
    }

    dbFile.seekp(offset, std::ios::beg);
    dbFile.write(reinterpret_cast<const char*>(page->serialize().data()), PAGE_SIZE);

    if (!dbFile.good()) {
        throw DatabaseException("Erreur lors de l'ecriture de la page");
    }
}

std::shared_ptr<Page> DiskManager::readPage(int pageId)
{
    std::ifstream dbFile("data.db", std::ios::binary);
    if (!dbFile.is_open()) {
        throw DatabaseException("Impossible d'ouvrir le fichier pour lecture");
    }

    size_t offset = static_cast<size_t>(pageId) * PAGE_SIZE;
    dbFile.seekg(offset, std::ios::beg);

    std::vector<uint8_t> buffer(PAGE_SIZE);
    dbFile.read(reinterpret_cast<char*>(buffer.data()), PAGE_SIZE);
    if (dbFile.gcount() < PAGE_SIZE) {
        throw DatabaseException("La page n'existe pas");
    }
    return std::make_shared<Page>(buffer);
}

int DiskManager::getNumberOfPages()
{
    std::ifstream dbFile("data.db", std::ios::binary);
    if (!dbFile.is_open()) {
        return 0;
    }
    else {
        std::streampos fileSize = dbFile.tellg();
        return static_cast<int>(fileSize / PAGE_SIZE);
    }
}
