#include "MSWBreadBoards.h"


MSWBreadBoards::MSWBreadBoards(DSNFile &a, MSWTools &ToolsIn) : BreadBoards(a,ToolsIn)
{

	MSWBreadBoard *A = new MSWBreadBoard(&ToolsIn);

	Tools=&ToolsIn;

	Boards.push_back(*A);
};