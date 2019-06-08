#include "MSWBreadBoards.h"


MSWBreadBoards::MSWBreadBoards(DSNFile &a, MSWTools &ToolsIn) : BreadBoards(a,ToolsIn)
{
	Tools=&ToolsIn;

	Tools->SetFileOrientation(FILE_IS_X_GOES_DOWN);
};