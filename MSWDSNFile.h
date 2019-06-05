#pragma once

#include "framework.h"
#include "commdlg.h"

#include "shobjidl_core.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include "MSWTools.h"
#include "DSNFile.h"


class MSWDSNFile : public DSNFile
{
public:
	MSWDSNFile(wstring a, MSWTools &b);


	void Paint(HDC hdc);
	void Print(HDC hdc);

protected:

private:
	MSWTools* Tools;

};

