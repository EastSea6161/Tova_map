#include "stdafx.h"
#include "UtilExcelHandler.h"
#include <comdef.h>
#include <vector>

UtilExcelHandler::UtilExcelHandler()
{
}

UtilExcelHandler::~UtilExcelHandler()
{
}

HRESULT UtilExcelHandler::AutoWrap(WORD wFlags, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs...)
{
    if (!pDisp)
        return E_POINTER;

    va_list marker;
    va_start(marker, cArgs);

    VARIANT* pArgs = new VARIANT[cArgs];
    for (int i = 0; i < cArgs; i++)
    {
        pArgs[i] = va_arg(marker, VARIANT);
    }
    va_end(marker);

    DISPID dispID;
    HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);

    if (FAILED(hr))
    {
        delete[] pArgs;
        return hr;
    }

    DISPPARAMS dp = { 0 };
    dp.cArgs = cArgs;
    dp.rgvarg = pArgs;

    dp.cNamedArgs = 0;

    hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, wFlags, &dp, pvResult, nullptr, nullptr);

    delete[] pArgs;
    return hr;
}

bool UtilExcelHandler::GetExcelSheetNames(const CString& excelPath, std::vector<CString>& outSheetNames)
{
    outSheetNames.clear();

    // 1. 초기화: 모든 포인터를 NULL로 시작 (나중에 Release 여부 판단 기준)
    IDispatch* pExcelApp = nullptr;
    IDispatch* pWorkbooks = nullptr;
    IDispatch* pWorkbook = nullptr;
    IDispatch* pSheets = nullptr;
    bool bResult = false;

    HRESULT hr = CoInitialize(nullptr);
    bool needUninit = SUCCEEDED(hr) && (hr != S_FALSE);

    try {
        // Excel 실행
        CLSID clsid;
        if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsid))) throw 1;
        if (FAILED(CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pExcelApp))) throw 1;

        // 속성 설정 (Visible/DisplayAlerts = false)
        VARIANT x; VariantInit(&x); x.vt = VT_BOOL; x.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"Visible", 1, x);
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"DisplayAlerts", 1, x);

        // Workbooks 가져오기
        VARIANT vWorkbooks; VariantInit(&vWorkbooks);
        if (FAILED(AutoWrap(DISPATCH_PROPERTYGET, &vWorkbooks, pExcelApp, L"Workbooks", 0))) throw 1;
        pWorkbooks = vWorkbooks.pdispVal;

        // Workbook 열기
        VARIANT vWorkbook; VariantInit(&vWorkbook);
        VARIANT arg; VariantInit(&arg); arg.vt = VT_BSTR; arg.bstrVal = _bstr_t(excelPath);
        if (FAILED(AutoWrap(DISPATCH_METHOD, &vWorkbook, pWorkbooks, L"Open", 1, arg))) throw 1;
        pWorkbook = vWorkbook.pdispVal;

        // Sheets 가져오기
        VARIANT vSheets; VariantInit(&vSheets);
        if (FAILED(AutoWrap(DISPATCH_PROPERTYGET, &vSheets, pWorkbook, L"Worksheets", 0))) throw 1;
        pSheets = vSheets.pdispVal;

        // 시트 개수 확인
        VARIANT vCount; VariantInit(&vCount);
        AutoWrap(DISPATCH_PROPERTYGET, &vCount, pSheets, L"Count", 0);
        long sheetCount = vCount.lVal;

        // 2. 루프 내 중단 체크 및 이름 수집
        for (long i = 1; i <= sheetCount; ++i) {
            // [중요] 사용자가 중단했는지 수시로 체크
            if (m_pbAbort && *m_pbAbort) throw 0;

            VARIANT argIdx; VariantInit(&argIdx); argIdx.vt = VT_I4; argIdx.lVal = i;
            VARIANT vSheet; VariantInit(&vSheet);
            if (SUCCEEDED(AutoWrap(DISPATCH_PROPERTYGET, &vSheet, pSheets, L"Item", 1, argIdx))) {
                IDispatch* pSheet = vSheet.pdispVal;
                VARIANT vName; VariantInit(&vName);
                if (SUCCEEDED(AutoWrap(DISPATCH_PROPERTYGET, &vName, pSheet, L"Name", 0))) {
                    outSheetNames.push_back(CString(vName.bstrVal));
                    VariantClear(&vName);
                }
                pSheet->Release();
                VariantClear(&vSheet);
            }
        }
        bResult = true;
    }
    catch (...) {
        bResult = false;
    }

    // 3. 최종 정리: 생성된 역순으로 안전하게 해제
    if (pSheets) pSheets->Release();
    if (pWorkbook) {
        VARIANT save; VariantInit(&save); save.vt = VT_BOOL; save.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_METHOD, nullptr, pWorkbook, L"Close", 1, save);
        pWorkbook->Release();
    }
    if (pWorkbooks) pWorkbooks->Release();
    if (pExcelApp) {
        AutoWrap(DISPATCH_METHOD, nullptr, pExcelApp, L"Quit", 0);
        pExcelApp->Release();
    }

    if (needUninit) CoUninitialize();
    return bResult;
}

bool UtilExcelHandler::ReadExcelToODData(const CString& excelPath, const CString& sheetName, long startRow, std::vector<ODData>& outData)
{
    outData.clear();

    // 1. 초기화: 모든 인터페이스 포인터를 NULL로 설정
    IDispatch* pExcelApp = nullptr, * pWorkbooks = nullptr, * pWorkbook = nullptr;
    IDispatch* pSheets = nullptr, * pSheet = nullptr, * pRange = nullptr;
    VARIANT vVal; VariantInit(&vVal);
    bool bResult = false;

    HRESULT hr = CoInitialize(nullptr);
    bool needUninit = SUCCEEDED(hr) && (hr != S_FALSE);

    try {
        // Excel 실행 및 초기 설정
        CLSID clsid;
        if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsid))) throw 1;
        if (FAILED(CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pExcelApp))) throw 1;

        VARIANT x; VariantInit(&x); x.vt = VT_BOOL; x.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"Visible", 1, x);
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"DisplayAlerts", 1, x);

        // Workbook 열기
        VARIANT vWBks; VariantInit(&vWBks);
        AutoWrap(DISPATCH_PROPERTYGET, &vWBks, pExcelApp, L"Workbooks", 0);
        pWorkbooks = vWBks.pdispVal;

        VARIANT vWB; VariantInit(&vWB);
        VARIANT argPath; VariantInit(&argPath); argPath.vt = VT_BSTR; argPath.bstrVal = _bstr_t(excelPath);
        if (FAILED(AutoWrap(DISPATCH_METHOD, &vWB, pWorkbooks, L"Open", 1, argPath))) throw 1;
        pWorkbook = vWB.pdispVal;

        // Sheet 가져오기
        VARIANT vSheets; VariantInit(&vSheets);
        AutoWrap(DISPATCH_PROPERTYGET, &vSheets, pWorkbook, L"Worksheets", 0);
        pSheets = vSheets.pdispVal;

        VARIANT vSheet; VariantInit(&vSheet);
        VARIANT argSheet; VariantInit(&argSheet); argSheet.vt = VT_BSTR; argSheet.bstrVal = _bstr_t(sheetName);
        if (FAILED(AutoWrap(DISPATCH_PROPERTYGET, &vSheet, pSheets, L"Item", 1, argSheet))) throw 1;
        pSheet = vSheet.pdispVal;

        // 2. 데이터 범위 계산 (UsedRange)
        long lastRow = 0, lastCol = 0;
        VARIANT vUsed; VariantInit(&vUsed);
        AutoWrap(DISPATCH_PROPERTYGET, &vUsed, pSheet, L"UsedRange", 0);
        if (vUsed.pdispVal) {
            IDispatch* pUsed = vUsed.pdispVal;
            VARIANT vRows, vCols, vRowCount, vColCount, vStartRow, vStartCol;
            VariantInit(&vRows); VariantInit(&vCols); VariantInit(&vRowCount);
            VariantInit(&vColCount); VariantInit(&vStartRow); VariantInit(&vStartCol);

            AutoWrap(DISPATCH_PROPERTYGET, &vRows, pUsed, L"Rows", 0);
            AutoWrap(DISPATCH_PROPERTYGET, &vRowCount, vRows.pdispVal, L"Count", 0);
            AutoWrap(DISPATCH_PROPERTYGET, &vStartRow, pUsed, L"Row", 0);
            lastRow = vStartRow.lVal + vRowCount.lVal - 1;

            AutoWrap(DISPATCH_PROPERTYGET, &vCols, pUsed, L"Columns", 0);
            AutoWrap(DISPATCH_PROPERTYGET, &vColCount, vCols.pdispVal, L"Count", 0);
            AutoWrap(DISPATCH_PROPERTYGET, &vStartCol, pUsed, L"Column", 0);
            lastCol = vStartCol.lVal + vColCount.lVal - 1;

            vRows.pdispVal->Release(); vCols.pdispVal->Release(); pUsed->Release();
        }

        // 3. SafeArray를 통한 데이터 추출 루프
        if (lastRow >= startRow && lastCol > 0) {
            CString rangeStr;
            rangeStr.Format(_T("A%ld:%s%ld"), startRow, ExcelColumnLetterFromIndex((int)lastCol).GetString(), lastRow);

            VARIANT vRange; VariantInit(&vRange);
            VARIANT argRange; VariantInit(&argRange); argRange.vt = VT_BSTR; argRange.bstrVal = _bstr_t(rangeStr);
            AutoWrap(DISPATCH_PROPERTYGET, &vRange, pSheet, L"Range", 1, argRange);
            pRange = vRange.pdispVal;

            if (pRange) {
                AutoWrap(DISPATCH_PROPERTYGET, &vVal, pRange, L"Value2", 0);
                if ((vVal.vt & VT_ARRAY) && (vVal.vt & VT_VARIANT)) {
                    SAFEARRAY* psa = vVal.parray;
                    long lRow, uRow, lCol, uCol;
                    SafeArrayGetLBound(psa, 1, &lRow); SafeArrayGetUBound(psa, 1, &uRow);
                    SafeArrayGetLBound(psa, 2, &lCol); SafeArrayGetUBound(psa, 2, &uCol);

                    for (long r = lRow; r <= uRow; ++r) {
                        // [중요] 사용자가 다이얼로그를 닫았는지 행 단위로 체크
                        if (m_pbAbort && *m_pbAbort) throw 0;

                        ODData rowData;
                        for (long c = lCol; c <= uCol; ++c) {
                            long indices[2] = { r, c };
                            VARIANT element; VariantInit(&element);
                            SafeArrayGetElement(psa, indices, &element);
                            rowData.push_back(VariantToCString(element));
                            VariantClear(&element);
                        }
                        outData.push_back(rowData);
                    }
                }
            }
        }
        bResult = !outData.empty();
    }
    catch (...) {
        bResult = false;
    }

    // 4. 최종 정리: 생성 역순으로 Release 및 엑셀 종료
    VariantClear(&vVal); // SafeArray 메모리 해제
    if (pRange) pRange->Release();
    if (pSheet) pSheet->Release();
    if (pSheets) pSheets->Release();
    if (pWorkbook) {
        VARIANT sc; VariantInit(&sc); sc.vt = VT_BOOL; sc.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_METHOD, nullptr, pWorkbook, L"Close", 1, sc);
        pWorkbook->Release();
    }
    if (pWorkbooks) pWorkbooks->Release();
    if (pExcelApp) {
        AutoWrap(DISPATCH_METHOD, nullptr, pExcelApp, L"Quit", 0);
        pExcelApp->Release();
    }

    if (needUninit) CoUninitialize();
    return bResult;
}

bool UtilExcelHandler::ReadColumnsBatch(const CString& excelPath, const CString& sheetName, long startRow, std::vector<ExcelColumnRequest*>& requests)
{
    if (requests.empty()) return true;

    IDispatch* pExcelApp = nullptr, * pWorkbooks = nullptr, * pWorkbook = nullptr;
    IDispatch* pSheets = nullptr, * pSheet = nullptr;
    bool bResult = false;

    HRESULT hr = CoInitialize(nullptr);
    bool needUninit = SUCCEEDED(hr) && (hr != S_FALSE);

    try {
        CLSID clsid;
        if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsid))) throw 1;
        if (FAILED(CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pExcelApp))) throw 1;

        VARIANT x; VariantInit(&x); x.vt = VT_BOOL; x.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"Visible", 1, x);
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"DisplayAlerts", 1, x);

        AutoWrap(DISPATCH_PROPERTYGET, &x, pExcelApp, L"Workbooks", 0);
        pWorkbooks = x.pdispVal;

        VARIANT vWB; VariantInit(&vWB);
        VARIANT argPath; VariantInit(&argPath); argPath.vt = VT_BSTR; argPath.bstrVal = _bstr_t(excelPath);
        if (FAILED(AutoWrap(DISPATCH_METHOD, &vWB, pWorkbooks, L"Open", 1, argPath))) throw 1;
        pWorkbook = vWB.pdispVal;

        AutoWrap(DISPATCH_PROPERTYGET, &x, pWorkbook, L"Worksheets", 0);
        pSheets = x.pdispVal;

        VARIANT vSheet; VariantInit(&vSheet);
        VARIANT argSheet; VariantInit(&argSheet); argSheet.vt = VT_BSTR; argSheet.bstrVal = _bstr_t(sheetName);
        if (FAILED(AutoWrap(DISPATCH_PROPERTYGET, &vSheet, pSheets, L"Item", 1, argSheet))) throw 1;
        pSheet = vSheet.pdispVal;

        // 마지막 행 계산
        long lastRow = 0;
        VARIANT vUsed; VariantInit(&vUsed);
        AutoWrap(DISPATCH_PROPERTYGET, &vUsed, pSheet, L"UsedRange", 0);
        if (vUsed.pdispVal) {
            IDispatch* pUsed = vUsed.pdispVal;
            VARIANT vRows, vRowCount, vStartRow;
            VariantInit(&vRows); VariantInit(&vRowCount); VariantInit(&vStartRow);
            AutoWrap(DISPATCH_PROPERTYGET, &vRows, pUsed, L"Rows", 0);
            AutoWrap(DISPATCH_PROPERTYGET, &vRowCount, vRows.pdispVal, L"Count", 0);
            AutoWrap(DISPATCH_PROPERTYGET, &vStartRow, pUsed, L"Row", 0);
            lastRow = vStartRow.lVal + vRowCount.lVal - 1;
            vRows.pdispVal->Release(); pUsed->Release();
        }

        if (lastRow >= startRow) {
            for (auto* req : requests) {
                // [컬럼 루프 중단 체크]
                if (m_pbAbort && *m_pbAbort) throw 0;

                req->data.clear();
                if (req->colLetter.IsEmpty()) continue;

                CString rangeStr;
                rangeStr.Format(_T("%s%ld:%s%ld"), req->colLetter.GetString(), startRow, req->colLetter.GetString(), lastRow);

                VARIANT vRange; VariantInit(&vRange);
                VARIANT argRange; VariantInit(&argRange); argRange.vt = VT_BSTR; argRange.bstrVal = _bstr_t(rangeStr);
                if (SUCCEEDED(AutoWrap(DISPATCH_PROPERTYGET, &vRange, pSheet, L"Range", 1, argRange))) {
                    IDispatch* pColRange = vRange.pdispVal;
                    VARIANT vVal; VariantInit(&vVal);
                    if (SUCCEEDED(AutoWrap(DISPATCH_PROPERTYGET, &vVal, pColRange, L"Value2", 0))) {
                        if ((vVal.vt & VT_ARRAY) && (vVal.vt & VT_VARIANT)) {
                            SAFEARRAY* psa = vVal.parray;
                            long lRow, uRow;
                            SafeArrayGetLBound(psa, 1, &lRow); SafeArrayGetUBound(psa, 1, &uRow);
                            for (long r = lRow; r <= uRow; ++r) {
                                // [데이터 루프 중단 체크] 대량 데이터 시 필수
                                if (m_pbAbort && *m_pbAbort) { VariantClear(&vVal); pColRange->Release(); throw 0; }
                                long indices[2] = { r, 1 };
                                VARIANT element; VariantInit(&element);
                                SafeArrayGetElement(psa, indices, &element);
                                req->data.push_back(VariantToCString(element));
                                VariantClear(&element);
                            }
                        }
                        else {
                            req->data.push_back(VariantToCString(vVal));
                        }
                        VariantClear(&vVal);
                    }
                    pColRange->Release();
                }
            }
        }
        bResult = true;
    }
    catch (...) {
        bResult = false;
    }

    // 정리 로직
    if (pSheet) pSheet->Release();
    if (pSheets) pSheets->Release();
    if (pWorkbook) {
        VARIANT sc; VariantInit(&sc); sc.vt = VT_BOOL; sc.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_METHOD, nullptr, pWorkbook, L"Close", 1, sc);
        pWorkbook->Release();
    }
    if (pWorkbooks) pWorkbooks->Release();
    if (pExcelApp) {
        AutoWrap(DISPATCH_METHOD, nullptr, pExcelApp, L"Quit", 0);
        pExcelApp->Release();
    }

    if (needUninit) CoUninitialize();
    return bResult;
}

CString UtilExcelHandler::VariantToCString(const VARIANT& v)
{
    CString s;

    switch (v.vt)
    {
    case VT_BSTR:
        s = v.bstrVal;
        break;

    case VT_R8:   // double
    case VT_R4:   // float
    {
        double d = (v.vt == VT_R8) ? v.dblVal : v.fltVal;

        // 소수 포함해서 숫자를 문자열로 (불필요한 0도 자동으로 어느 정도 정리됨)
        s.Format(_T("%.15g"), d);
        break;
    }

    case VT_I1:
    case VT_I2:
    case VT_I4:
    case VT_INT:
    case VT_UI1:
    case VT_UI2:
    case VT_UI4:
    case VT_UINT:
    {
        // 정수형은 그냥 정수로 출력
        long long val = 0;
        switch (v.vt)
        {
        case VT_I1:   val = v.cVal;   break;
        case VT_I2:   val = v.iVal;   break;
        case VT_I4:   val = v.lVal;   break;
        case VT_INT:  val = v.intVal; break;
        case VT_UI1:  val = v.bVal;   break;
        case VT_UI2:  val = v.uiVal;  break;
        case VT_UI4:  val = v.ulVal;  break;
        case VT_UINT: val = v.uintVal; break;
        }

        s.Format(_T("%lld"), val);
        break;
    }

    case VT_EMPTY:
    case VT_NULL:
        s.Empty();
        break;

    default:
        s.Empty();
        break;
    }

    return s;
}

bool UtilExcelHandler::ReadRowRangeFromExcel(const CString& excelPath, const CString& sheetName, long rowIndex, int maxColumn, std::vector<CString>& outValues)
{
    outValues.clear();

    IDispatch* pExcelApp = nullptr, * pWorkbooks = nullptr, * pWorkbook = nullptr;
    IDispatch* pSheets = nullptr, * pSheet = nullptr;
    bool bResult = false;

    HRESULT hr = CoInitialize(nullptr);
    bool needUninit = SUCCEEDED(hr) && (hr != S_FALSE);

    try {
        // Excel 실행 및 설정
        CLSID clsid;
        if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsid))) throw 1;
        if (FAILED(CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pExcelApp))) throw 1;

        VARIANT x; VariantInit(&x); x.vt = VT_BOOL; x.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"Visible", 1, x);
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"DisplayAlerts", 1, x);

        // Workbook 열기
        VARIANT vWBks; VariantInit(&vWBks);
        AutoWrap(DISPATCH_PROPERTYGET, &vWBks, pExcelApp, L"Workbooks", 0);
        pWorkbooks = vWBks.pdispVal;

        VARIANT vWB; VariantInit(&vWB);
        VARIANT argPath; VariantInit(&argPath); argPath.vt = VT_BSTR; argPath.bstrVal = _bstr_t(excelPath);
        if (FAILED(AutoWrap(DISPATCH_METHOD, &vWB, pWorkbooks, L"Open", 1, argPath))) throw 1;
        pWorkbook = vWB.pdispVal;

        // Sheet 가져오기
        VARIANT vSheets; VariantInit(&vSheets);
        AutoWrap(DISPATCH_PROPERTYGET, &vSheets, pWorkbook, L"Worksheets", 0);
        pSheets = vSheets.pdispVal;

        VARIANT vSheet; VariantInit(&vSheet);
        VARIANT argSheet; VariantInit(&argSheet); argSheet.vt = VT_BSTR; argSheet.bstrVal = _bstr_t(sheetName);
        if (FAILED(AutoWrap(DISPATCH_PROPERTYGET, &vSheet, pSheets, L"Item", 1, argSheet))) throw 1;
        pSheet = vSheet.pdispVal;

        // 열 순회 읽기
        for (int col = 1; col <= maxColumn; ++col) {
            // [중단 체크] 사용자가 취소를 눌렀는지 체크
            if (m_pbAbort && *m_pbAbort) throw 0;

            CString cellAddr;
            cellAddr.Format(_T("%s%ld"), ExcelColumnLetterFromIndex(col).GetString(), rowIndex);

            VARIANT vRange; VariantInit(&vRange);
            VARIANT argAddr; VariantInit(&argAddr); argAddr.vt = VT_BSTR; argAddr.bstrVal = _bstr_t(cellAddr);

            if (SUCCEEDED(AutoWrap(DISPATCH_PROPERTYGET, &vRange, pSheet, L"Range", 1, argAddr))) {
                IDispatch* pCell = vRange.pdispVal;
                VARIANT vVal; VariantInit(&vVal);
                if (SUCCEEDED(AutoWrap(DISPATCH_PROPERTYGET, &vVal, pCell, L"Value2", 0))) {
                    outValues.push_back(VariantToCString(vVal));
                    VariantClear(&vVal);
                }
                else {
                    outValues.push_back(_T(""));
                }
                pCell->Release();
                VariantClear(&vRange);
            }
            else {
                outValues.push_back(_T(""));
            }
        }
        bResult = !outValues.empty();
    }
    catch (...) {
        bResult = false;
    }

    // 일괄 정리
    if (pSheet) pSheet->Release();
    if (pSheets) pSheets->Release();
    if (pWorkbook) {
        VARIANT sc; VariantInit(&sc); sc.vt = VT_BOOL; sc.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_METHOD, nullptr, pWorkbook, L"Close", 1, sc);
        pWorkbook->Release();
    }
    if (pWorkbooks) pWorkbooks->Release();
    if (pExcelApp) {
        AutoWrap(DISPATCH_METHOD, nullptr, pExcelApp, L"Quit", 0);
        pExcelApp->Release();
    }

    if (needUninit) CoUninitialize();
    return bResult;
}

CString UtilExcelHandler::ExcelColumnLetterFromIndex(int index)
{
    CString res;
    int col = index;

    while (col > 0)
    {
        int rem = (col - 1) % 26;
        WCHAR ch = L'A' + rem;
        res.Insert(0, ch);
        col = (col - 1) / 26;
    }

    return res;
}