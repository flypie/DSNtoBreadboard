#pragma once

#include "dsntools.h"
#include "DSNFile.h"
#include "BreadBoard.h"




class BreadBoards
{
public:

	~BreadBoards();
	BreadBoards(DSNFile& a, DSNTools &b);

	void Paint();
	void Print() {};

protected:

	std::vector<BreadBoard> Boards;

	DSNFile* PCB;

private:

	DSNTools *Tools;



};

