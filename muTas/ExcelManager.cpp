#include "StdAfx.h"
#include "ExcelManager.h"

using namespace libxl;

KExcelManager::KExcelManager(void)
{
}


KExcelManager::~KExcelManager(void)
{
}

bool KExcelManager::Loadfile(CString a_filename, Book* &a_pBook)
{
	CString strToken;
	int nPosition;
	nPosition = a_filename.ReverseFind('.');
	strToken = a_filename.Right((a_filename.GetLength() - nPosition) - 1);
	if (strToken == _T("xls"))
	{
		a_pBook = xlCreateBook();
	}
	else if (strToken == _T("xlsx"))
	{
		a_pBook = xlCreateXMLBook();
	}
	else
	{
		AfxMessageBox(_T("EXCEL 파일을 찾을수 없습니다."));
		return false;
	}
	a_pBook->setKey(L"Soo Ho Jung", L"windows-2e2d220c06c1e70662bd6d63a1o1ceu2"); 
	a_pBook->load(a_filename);
	return true;
}


