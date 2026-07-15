#pragma once
#include <vector>
#include <memory>
#include "Column.h"
#include "Page.h"
#include "DataType.h"
#include "DataValue.h"
#include "Tuple.h"
#include "BufferManager.h"

class Table
{
private:
	//int firstPageIndex = 0;
	std::string name;
	std::vector<Column> columns;

	std::shared_ptr<Page> loadedPage;

	BufferManager& bm;

public:
	Table(std::string name, std::vector<Column> column, BufferManager& bm);
	// TODO : Ajout d'un constructeur pour crée une table déjà existante avec un tableau d'octets


	std::string getName() const;

	void insertTuple(std::vector<std::unique_ptr<DataValue>> data);
	void insertTupleIntoPage(Tuple& tuple);
	Tuple createTuple(int slotIndex);

	void displayTable();
	// TODO : Crée une fonction qui renvoie le premier espaces libres en prenant la taille de l'espace en paramètre
	void nextPage();
	void prevPage();
};

