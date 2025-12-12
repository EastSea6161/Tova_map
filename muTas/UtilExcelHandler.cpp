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

    HRESULT hr = CoInitialize(nullptr);
    bool needUninit = SUCCEEDED(hr) && (hr != S_FALSE);

    CLSID clsid;
    hr = CLSIDFromProgID(L"Excel.Application", &clsid);
    if (FAILED(hr))
    {
        if (needUninit) CoUninitialize();
        return false;
    }

    IDispatch* pExcelApp = nullptr;
    hr = CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pExcelApp);
    if (FAILED(hr) || !pExcelApp)
    {
        if (needUninit) CoUninitialize();
        return false;
    }

    // Visible = false
    {
        VARIANT x;
        VariantInit(&x);
        x.vt = VT_BOOL;
        x.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"Visible", 1, x);
    }

    // DisplayAlerts = false
    {
        VARIANT x;
        VariantInit(&x);
        x.vt = VT_BOOL;
        x.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"DisplayAlerts", 1, x);
    }

    // --------------------------------------------------------------------
    // Workbooks
    // --------------------------------------------------------------------
    VARIANT vWorkbooks;
    VariantInit(&vWorkbooks);

    hr = AutoWrap(DISPATCH_PROPERTYGET, &vWorkbooks, pExcelApp, L"Workbooks", 0);
    if (FAILED(hr) || vWorkbooks.vt != VT_DISPATCH || !vWorkbooks.pdispVal)
    {
        VariantClear(&vWorkbooks);
        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }

    IDispatch* pWorkbooks = vWorkbooks.pdispVal;
    pWorkbooks->AddRef();          // 내가 쓸 참조
    VariantClear(&vWorkbooks);     // VARIANT 안의 참조는 정리

    // --------------------------------------------------------------------
    // Workbooks.Open(excelPath)
    // --------------------------------------------------------------------
    VARIANT vWorkbook;
    VariantInit(&vWorkbook);

    {
        VARIANT arg;
        VariantInit(&arg);
        arg.vt = VT_BSTR;
        arg.bstrVal = _bstr_t(excelPath);

        hr = AutoWrap(DISPATCH_METHOD, &vWorkbook, pWorkbooks, L"Open", 1, arg);
        // arg 안의 BSTR은 Variant가 쥐고 있고, AutoWrap 끝나면 알아서 Clear 안 하므로 여기서 따로 Clear 필요 없음
    }

    if (FAILED(hr) || vWorkbook.vt != VT_DISPATCH || !vWorkbook.pdispVal)
    {
        VariantClear(&vWorkbook);
        pWorkbooks->Release();
        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }

    IDispatch* pWorkbook = vWorkbook.pdispVal;
    pWorkbook->AddRef();
    VariantClear(&vWorkbook);

    // --------------------------------------------------------------------
    // Worksheets
    // --------------------------------------------------------------------
    VARIANT vSheets;
    VariantInit(&vSheets);

    hr = AutoWrap(DISPATCH_PROPERTYGET, &vSheets, pWorkbook, L"Worksheets", 0);
    if (FAILED(hr) || vSheets.vt != VT_DISPATCH || !vSheets.pdispVal)
    {
        VariantClear(&vSheets);
        pWorkbook->Release();
        pWorkbooks->Release();
        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }

    IDispatch* pSheets = vSheets.pdispVal;
    pSheets->AddRef();
    VariantClear(&vSheets);

    // --------------------------------------------------------------------
    // Worksheets.Count
    // --------------------------------------------------------------------
    long sheetCount = 0;
    VARIANT vCount;
    VariantInit(&vCount);

    hr = AutoWrap(DISPATCH_PROPERTYGET, &vCount, pSheets, L"Count", 0);
    if (SUCCEEDED(hr) && (vCount.vt == VT_I4 || vCount.vt == VT_INT))
    {
        sheetCount = vCount.lVal;
    }
    VariantClear(&vCount);

    // --------------------------------------------------------------------
    // 1-based 인덱스로 시트 이름들 가져오기
    // --------------------------------------------------------------------
    VARIANT vResult;
    VariantInit(&vResult);

    for (long i = 1; i <= sheetCount; ++i)
    {
        // Sheets.Item(i)
        VARIANT argIndex;
        VariantInit(&argIndex);
        argIndex.vt = VT_I4;
        argIndex.lVal = i;

        VariantClear(&vResult);
        hr = AutoWrap(DISPATCH_PROPERTYGET, &vResult, pSheets, L"Item", 1, argIndex);
        if (FAILED(hr) || vResult.vt != VT_DISPATCH || !vResult.pdispVal)
        {
            // 이번 시트는 스킵하고 계속
            continue;
        }

        IDispatch* pSheet = vResult.pdispVal;
        pSheet->AddRef();
        VariantClear(&vResult);

        // Sheet.Name
        VariantInit(&vResult);
        hr = AutoWrap(DISPATCH_PROPERTYGET, &vResult, pSheet, L"Name", 0);
        if (SUCCEEDED(hr) && vResult.vt == VT_BSTR && vResult.bstrVal != nullptr)
        {
            CString sheetName(vResult.bstrVal);
            outSheetNames.push_back(sheetName);
        }
        VariantClear(&vResult);

        pSheet->Release();
    }

    // --------------------------------------------------------------------
    // Workbook 닫기 (변경사항 저장 X)
    // --------------------------------------------------------------------
    {
        VARIANT saveChanges;
        VariantInit(&saveChanges);
        saveChanges.vt = VT_BOOL;
        saveChanges.boolVal = VARIANT_FALSE;

        AutoWrap(DISPATCH_METHOD, nullptr, pWorkbook, L"Close", 1, saveChanges);
    }

    // Excel 종료
    AutoWrap(DISPATCH_METHOD, nullptr, pExcelApp, L"Quit", 0);

    // Release
    pSheets->Release();
    pWorkbook->Release();
    pWorkbooks->Release();
    pExcelApp->Release();

    if (needUninit) CoUninitialize();

    return !outSheetNames.empty();
}

bool UtilExcelHandler::ReadColumnsBatch(const CString& excelPath, const CString& sheetName, long startRow, std::vector<ExcelColumnRequest*>& requests)
{
    if (requests.empty()) return true;

    HRESULT hr = CoInitialize(nullptr);
    bool needUninit = SUCCEEDED(hr) && (hr != S_FALSE);

    // 1. Excel 실행
    CLSID clsid;
    hr = CLSIDFromProgID(L"Excel.Application", &clsid);
    if (FAILED(hr)) { if (needUninit) CoUninitialize(); return false; }

    IDispatch* pExcelApp = nullptr;
    hr = CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pExcelApp);
    if (FAILED(hr) || !pExcelApp) { if (needUninit) CoUninitialize(); return false; }

    // Visible = false, DisplayAlerts = false
    {
        VARIANT x; VariantInit(&x);
        x.vt = VT_BOOL; x.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"Visible", 1, x);
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"DisplayAlerts", 1, x);
    }

    // Workbooks Open
    IDispatch* pWorkbook = nullptr;
    {
        VARIANT vWBks; VariantInit(&vWBks);
        AutoWrap(DISPATCH_PROPERTYGET, &vWBks, pExcelApp, L"Workbooks", 0);
        if (vWBks.pdispVal)
        {
            VARIANT vRet; VariantInit(&vRet);
            VARIANT arg; VariantInit(&arg);
            arg.vt = VT_BSTR; arg.bstrVal = _bstr_t(excelPath);
            AutoWrap(DISPATCH_METHOD, &vRet, vWBks.pdispVal, L"Open", 1, arg);
            pWorkbook = vRet.pdispVal; // RefCount 증가 상태
            vWBks.pdispVal->Release();
        }
    }

    if (!pWorkbook)
    {
        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }

    // Sheet 가져오기
    IDispatch* pSheet = nullptr;
    {
        VARIANT vSheets; VariantInit(&vSheets);
        AutoWrap(DISPATCH_PROPERTYGET, &vSheets, pWorkbook, L"Worksheets", 0);
        if (vSheets.pdispVal)
        {
            VARIANT vRet; VariantInit(&vRet);
            VARIANT arg; VariantInit(&arg);
            arg.vt = VT_BSTR; arg.bstrVal = _bstr_t(sheetName);
            AutoWrap(DISPATCH_PROPERTYGET, &vRet, vSheets.pdispVal, L"Item", 1, arg);
            pSheet = vRet.pdispVal;
            vSheets.pdispVal->Release();
        }
    }

    if (!pSheet)
    {
        // 정리
        VARIANT sc; VariantInit(&sc); sc.vt = VT_BOOL; sc.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_METHOD, nullptr, pWorkbook, L"Close", 1, sc);
        pWorkbook->Release();

        // [수정됨] Quit도 AutoWrap으로 호출해야 합니다.
        AutoWrap(DISPATCH_METHOD, nullptr, pExcelApp, L"Quit", 0);

        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }

    // 2. UsedRange로 마지막 행 계산 (한 번만 수행)
    long lastRow = 0;
    {
        VARIANT vUsed; VariantInit(&vUsed);
        AutoWrap(DISPATCH_PROPERTYGET, &vUsed, pSheet, L"UsedRange", 0);
        if (vUsed.pdispVal)
        {
            IDispatch* pUsed = vUsed.pdispVal;
            VARIANT vRows, vCount, vFirstRow;
            VariantInit(&vRows); VariantInit(&vCount); VariantInit(&vFirstRow);

            AutoWrap(DISPATCH_PROPERTYGET, &vRows, pUsed, L"Rows", 0);
            if (vRows.pdispVal)
            {
                AutoWrap(DISPATCH_PROPERTYGET, &vCount, vRows.pdispVal, L"Count", 0);
                long cnt = vCount.lVal;
                vRows.pdispVal->Release();

                AutoWrap(DISPATCH_PROPERTYGET, &vFirstRow, pUsed, L"Row", 0);
                long first = vFirstRow.lVal;

                lastRow = first + cnt - 1;
            }
            pUsed->Release();
        }
    }

    // 3. 각 컬럼별로 통으로 읽기 (SAFEARRAY 사용)
    if (lastRow >= startRow)
    {
        for (auto* req : requests)
        {
            req->data.clear();
            if (req->colLetter.IsEmpty()) continue;

            // 범위 지정: 예 "A2:A1000"
            CString rangeStr;
            rangeStr.Format(_T("%s%ld:%s%ld"), req->colLetter, startRow, req->colLetter, lastRow);

            VARIANT vRange; VariantInit(&vRange);
            {
                VARIANT arg; VariantInit(&arg);
                arg.vt = VT_BSTR; arg.bstrVal = _bstr_t(rangeStr);
                AutoWrap(DISPATCH_PROPERTYGET, &vRange, pSheet, L"Range", 1, arg);
            }

            if (vRange.pdispVal)
            {
                VARIANT vVal; VariantInit(&vVal);
                // Value2 속성을 가져옴 (속도가 가장 빠름)
                AutoWrap(DISPATCH_PROPERTYGET, &vVal, vRange.pdispVal, L"Value2", 0);
                vRange.pdispVal->Release();

                // 결과가 배열인지 확인
                if ((vVal.vt & VT_ARRAY) && (vVal.vt & VT_VARIANT))
                {
                    SAFEARRAY* psa = vVal.parray;
                    long lBoundRow, uBoundRow, lBoundCol, uBoundCol;
                    SafeArrayGetLBound(psa, 1, &lBoundRow);
                    SafeArrayGetUBound(psa, 1, &uBoundRow);
                    SafeArrayGetLBound(psa, 2, &lBoundCol);
                    SafeArrayGetUBound(psa, 2, &uBoundCol);

                    // 배열 순회 (메모리 접근이라 매우 빠름)
                    for (long r = lBoundRow; r <= uBoundRow; ++r)
                    {
                        long indices[2];
                        indices[0] = r; // Row
                        indices[1] = 1; // Column (범위를 1열만 잡았으므로 항상 1)

                        VARIANT element;
                        VariantInit(&element);
                        // SafeArrayGetElement가 COM 호출보다 월등히 빠름
                        HRESULT hrArr = SafeArrayGetElement(psa, indices, &element);

                        if (SUCCEEDED(hrArr))
                        {
                            CString s = VariantToCString(element);
                            req->data.push_back(s);
                            VariantClear(&element);
                        }
                        else
                        {
                            req->data.push_back(_T(""));
                        }
                    }
                }
                else
                {
                    // 데이터가 1개뿐이거나 배열이 아닌 경우 (단일 셀)
                    CString s = VariantToCString(vVal);
                    req->data.push_back(s);
                }
                VariantClear(&vVal);
            }
        }
    }

    // 4. 정리 (Close & Quit)
    {
        VARIANT sc; VariantInit(&sc); sc.vt = VT_BOOL; sc.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_METHOD, nullptr, pWorkbook, L"Close", 1, sc);
    }
    pWorkbook->Release();

    AutoWrap(DISPATCH_METHOD, nullptr, pExcelApp, L"Quit", 0);
    pExcelApp->Release();

    if (needUninit) CoUninitialize();

    return true;
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

    HRESULT hr = CoInitialize(nullptr);
    bool needUninit = SUCCEEDED(hr) && (hr != S_FALSE);

    // Excel.Application
    CLSID clsid;
    hr = CLSIDFromProgID(L"Excel.Application", &clsid);
    if (FAILED(hr))
    {
        if (needUninit) CoUninitialize();
        return false;
    }

    IDispatch* pExcelApp = nullptr;
    hr = CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER,
        IID_IDispatch, (void**)&pExcelApp);
    if (FAILED(hr) || !pExcelApp)
    {
        if (needUninit) CoUninitialize();
        return false;
    }

    // Visible = false, DisplayAlerts = false
    {
        VARIANT x; VariantInit(&x);
        x.vt = VT_BOOL; x.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"Visible", 1, x);
        AutoWrap(DISPATCH_PROPERTYPUT, nullptr, pExcelApp, L"DisplayAlerts", 1, x);
    }

    // Workbooks
    VARIANT vWBks; VariantInit(&vWBks);
    hr = AutoWrap(DISPATCH_PROPERTYGET, &vWBks, pExcelApp, L"Workbooks", 0);
    if (FAILED(hr) || vWBks.vt != VT_DISPATCH || !vWBks.pdispVal)
    {
        VariantClear(&vWBks);
        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }
    IDispatch* pWorkbooks = vWBks.pdispVal;

    // Workbooks.Open
    VARIANT vWorkbook; VariantInit(&vWorkbook);
    {
        VARIANT arg; VariantInit(&arg);
        arg.vt = VT_BSTR;
        arg.bstrVal = _bstr_t(excelPath);

        hr = AutoWrap(DISPATCH_METHOD, &vWorkbook, pWorkbooks, L"Open", 1, arg);
    }
    if (FAILED(hr) || vWorkbook.vt != VT_DISPATCH || !vWorkbook.pdispVal)
    {
        VariantClear(&vWorkbook);
        pWorkbooks->Release();
        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }
    IDispatch* pWorkbook = vWorkbook.pdispVal;

    // Worksheets
    VARIANT vSheets; VariantInit(&vSheets);
    hr = AutoWrap(DISPATCH_PROPERTYGET, &vSheets, pWorkbook, L"Worksheets", 0);
    if (FAILED(hr) || vSheets.vt != VT_DISPATCH || !vSheets.pdispVal)
    {
        VariantClear(&vSheets);
        pWorkbook->Release();
        pWorkbooks->Release();
        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }
    IDispatch* pSheets = vSheets.pdispVal;

    // 특정 시트
    VARIANT vSheet; VariantInit(&vSheet);
    {
        VARIANT arg; VariantInit(&arg);
        arg.vt = VT_BSTR;
        arg.bstrVal = _bstr_t(sheetName);

        hr = AutoWrap(DISPATCH_PROPERTYGET, &vSheet, pSheets, L"Item", 1, arg);
    }
    if (FAILED(hr) || vSheet.vt != VT_DISPATCH || !vSheet.pdispVal)
    {
        VariantClear(&vSheet);
        pSheets->Release();
        pWorkbook->Release();
        pWorkbooks->Release();
        pExcelApp->Release();
        if (needUninit) CoUninitialize();
        return false;
    }
    IDispatch* pSheet = vSheet.pdispVal;

    // ------------------------------------------------------------
    // 한 번 Excel/Workbook/Sheet 열어놓고,
    // A{rowIndex} ~ [maxColumn열]{rowIndex} 까지 칸을 하나씩 읽는다.
    // (COM 호출은 열 개수 만큼만, workbook은 한 번만 여니까 충분히 빠름)
    // ------------------------------------------------------------
    for (int col = 1; col <= maxColumn; ++col)
    {
        CString colLetter = ExcelColumnLetterFromIndex(col);
        CString cellAddr;
        cellAddr.Format(_T("%s%ld"), colLetter.GetString(), rowIndex);

        // Range("A1") 같은 단일 셀 Range 얻기
        VARIANT vRange; VariantInit(&vRange);
        {
            VARIANT arg; VariantInit(&arg);
            arg.vt = VT_BSTR;
            arg.bstrVal = _bstr_t(cellAddr);

            hr = AutoWrap(DISPATCH_PROPERTYGET, &vRange, pSheet, L"Range", 1, arg);
        }
        if (FAILED(hr) || vRange.vt != VT_DISPATCH || !vRange.pdispVal)
        {
            VariantClear(&vRange);
            // 실패한 경우는 그냥 빈 값으로 채움
            outValues.push_back(CString());
            continue;
        }

        IDispatch* pCellRange = vRange.pdispVal;

        VARIANT vValue; VariantInit(&vValue);
        hr = AutoWrap(DISPATCH_PROPERTYGET, &vValue, pCellRange, L"Value2", 0);
        pCellRange->Release();

        if (FAILED(hr))
        {
            VariantClear(&vValue);
            outValues.push_back(CString());
            continue;
        }

        CString s = VariantToCString(vValue);
        VariantClear(&vValue);

        outValues.push_back(s);
    }

    // Workbook 닫기 / Excel 종료
    {
        VARIANT saveChanges; VariantInit(&saveChanges);
        saveChanges.vt = VT_BOOL;
        saveChanges.boolVal = VARIANT_FALSE;
        AutoWrap(DISPATCH_METHOD, nullptr, pWorkbook, L"Close", 1, saveChanges);
        AutoWrap(DISPATCH_METHOD, nullptr, pExcelApp, L"Quit", 0);
    }

    pSheet->Release();
    pSheets->Release();
    pWorkbook->Release();
    pWorkbooks->Release();
    pExcelApp->Release();

    if (needUninit) CoUninitialize();

    return !outValues.empty();
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