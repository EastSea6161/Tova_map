#pragma once
#include <vector>
#include <map>

struct ExcelColumnRequest
{
	CString colLetter;
	std::vector<CString> data;
};

class UtilExcelHandler
{
public:
	UtilExcelHandler();
	~UtilExcelHandler();

public:
	HRESULT AutoWrap(WORD wFlags, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs...);
	bool GetExcelSheetNames(const CString& excelPath, std::vector<CString>& outSheetNames);
	bool ReadColumnsBatch(const CString& excelPath, const CString& sheetName, long startRow, std::vector<ExcelColumnRequest*>& requests);
	CString VariantToCString(const VARIANT& var);
	bool ReadRowRangeFromExcel(const CString& excelPath, const CString& sheetName, long rowIndex, int maxColumn, std::vector<CString>& outValues);
	CString ExcelColumnLetterFromIndex(int index);
};

