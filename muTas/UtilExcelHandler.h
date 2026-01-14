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

	typedef std::vector<CString> ODData;

	volatile bool* m_pbAbort = nullptr;
public:
	HRESULT AutoWrap(WORD wFlags, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs...);
	bool GetExcelSheetNames(const CString& excelPath, std::vector<CString>& outSheetNames);
	bool ReadColumnsBatch(const CString& excelPath, const CString& sheetName, long startRow, std::vector<ExcelColumnRequest*>& requests);
	bool ReadExcelToODData(const CString& excelPath, const CString& sheetName, long startRow, std::vector<ODData>& outData);
	CString VariantToCString(const VARIANT& var);
	bool ReadRowRangeFromExcel(const CString& excelPath, const CString& sheetName, long rowIndex, int maxColumn, std::vector<CString>& outValues);
	CString ExcelColumnLetterFromIndex(int index);
};

