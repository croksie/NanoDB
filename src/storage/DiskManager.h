#pragma once
#include <memory>
#include <fstream>
#include <filesystem>
#include "Page.h"
class DiskManager
{
private : 
	std::fstream m_dbFile;
public:
	DiskManager();
	~DiskManager();
	void writePage(std::shared_ptr<Page> page);
	std::shared_ptr<Page> readPage(int pageId);
	int getNumberOfPages();
};

