#pragma once
#include "afxwin.h"
#include <fstream>
#include <iterator>
#include <iostream>
#include <sys/stat.h>
//#include "StdioFileEx.h"

class KTarget;

class KWriteCodeAgrument
{
public:
	KWriteCodeAgrument(void);
	~KWriteCodeAgrument(void);


public:

	static void WirteMatchingCodeArument(KTarget* a_pTarget, CString a_strFolderName = DLL_MODEL_FOLDER_NAME);

};

