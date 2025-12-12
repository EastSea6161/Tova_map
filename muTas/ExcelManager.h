#include "libxl.h"

using namespace libxl;

#pragma once
class KExcelManager
{
public:
	KExcelManager(void);
	~KExcelManager(void);

public:

	bool Loadfile(CString a_filename,Book* &a_pbook);


};

