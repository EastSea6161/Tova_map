// DlgTransitStyle.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgTransitStyle.h"
#include "afxdialogex.h"
#include "Object.h"
#include "Target.h"
#include "QuickSearchDlg.h"
#include "BulkDBaseTransit.h"
#include "IOTransitTableView.h"
#include "KExRecordItem.h"

IMPLEMENT_DYNAMIC(KDlgTransitStyle, KResizeDialogEx)

KDlgTransitStyle::KDlgTransitStyle(KTarget* a_pTarget, KIOTransitTableView* a_pTransitView, CWnd* pParent /*=NULL*/)
    : KResizeDialogEx(KDlgTransitStyle::IDD, pParent)
    , m_pTarget(a_pTarget)
    , m_pTransitView(a_pTransitView)
    , m_dMinWidth(0.1)
    , m_dMaxWidth(3.0)
    , m_dWidth(0.5)
    , m_dOffset(0.0)
{
    m_clrLine = RGB(0, 162, 232);

    KBulkDBaseTransit::TransitMainInfo(m_pTarget, m_mapTransitInfo);
}

void KDlgTransitStyle::InitID(std::set<Integer> &a_setID)
{
    m_setReportList = a_setID;
    UpdateReport();
}

void KDlgTransitStyle::InitModalID(std::set<Integer> &a_setID)
{
    m_setReportList = a_setID;
}

void KDlgTransitStyle::GetStyleApplyID(OUT std::set<Integer> &a_setApplyID, COLORREF &a_clrLine, double &a_dWidth, double &a_dOffset)
{
    a_setApplyID = m_setApplyID;
    a_clrLine    = m_clrLine;
    a_dWidth     = m_dWidth;
    a_dOffset    = m_dOffset;
}

KDlgTransitStyle::~KDlgTransitStyle()
{
    CloseWindowProc();
}

void KDlgTransitStyle::DoDataExchange(CDataExchange* pDX)
{
    KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
    DDX_Control(pDX, IDC_CLR_LINE, m_cpLineColor);
}

BEGIN_MESSAGE_MAP(KDlgTransitStyle, KResizeDialogEx)
    ON_BN_CLICKED(IDC_BUTTON2,                   &KDlgTransitStyle::OnBnClickedButtonSearchTransit)
    ON_BN_CLICKED(IDC_CHECK1,                    &KDlgTransitStyle::OnBnClickedCheckSelectAll)
    ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KDlgTransitStyle::OnReportCheckItem)

    ON_BN_CLICKED(IDC_BTN_APPLY,                          &KDlgTransitStyle::OnBnClickedBtnApply)
    ON_BN_CLICKED(IDCANCEL,                      &KDlgTransitStyle::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON1, &KDlgTransitStyle::OnBnClickedBtnStyleReset)
    ON_BN_CLICKED(IDC_CHECK2, &KDlgTransitStyle::OnBnClickedCheckUseClr)
END_MESSAGE_MAP()


// KDlgTransitStyle 메시지 처리기입니다.
void KDlgTransitStyle::ResizeComponent()
{
    SetResize(IDC_CHECK1, SZ_TOP_LEFT, SZ_TOP_LEFT);

    SetResize(IDC_BUTTON2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
    SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_SEPRATOR8, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_CHECK2, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_CLR_LINE, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_STATIC22, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_EDIT_SIZE, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_STATIC1, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_SEPRATOR9, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_BUTTON1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_BTN_APPLY, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}

BOOL KDlgTransitStyle::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();

    HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
    SetIcon(hIcon, TRUE);
    SetIcon(hIcon, FALSE);

    KReportCtrlSetting::Default(m_ctrlReport, FALSE, FALSE, FALSE);
    KReportCtrlSetting::AllowColumnsort(m_ctrlReport);

    KColorPickerSetting::DefaultCtrl(m_cpLineColor, m_clrLine);

    CheckDlgButton(IDC_CHECK1, BST_CHECKED); //전체선택
    CheckDlgButton(IDC_CHECK2, BST_CHECKED); //색상변경
    CotrolCheckClr();

    CString strDefaultSize(_T("")); {
        strDefaultSize.Format(_T("%.1f"), m_dWidth);
    }
    SetDlgItemText(IDC_EDIT_SIZE, strDefaultSize);

    if (true) {
        CString strRangeTxt(_T("")); {
            if (KmzSystem::GetLanguage() == KEMKorea) {
                strRangeTxt.Format(_T("(범위 %.1f~%.1f)"), m_dMinWidth, m_dMaxWidth);
            }
            else {
                strRangeTxt.Format(_T("(Range %.1f~%.1f)"), m_dMinWidth, m_dMaxWidth);
            }
        }
        SetDlgItemText(IDC_STATIC1, strRangeTxt);
    }

    InitReportHeader();
    UpdateReport();
    m_ctrlReport.Populate();

    ResizeComponent();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDlgTransitStyle::InitReportHeader()
{
    CXTPReportColumn* pColumn = NULL;

    pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 50, FALSE));
    pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

    pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(1, _T("Line ID"), 45));
    pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("노선 번호"));
    }

    pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(2, _T("Mode"), 55));
    pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시스템 타입"));
    }

    pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(3, _T("Type"), 55));
    pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("타입"));
    }

    pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(4, _T("Name"), 55));
    pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }
}

void KDlgTransitStyle::GetCodes(LPCTSTR strColName, std::map<int, CString> &a_mapCode)
{
    a_mapCode.clear();

    try
    {
        KIOTables*  pIOTables = m_pTarget->Tables();
        KIOTable*   pIOTable  = pIOTables->FindTable(TABLE_TRANSIT);
        KIOColumns* pIOColums = pIOTable->Columns();

        KIOColumn* pIOColumn = pIOColums->GetColumn(strColName);

        bool bCodeType = pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode ? true : false;
        KCodeGroup*      pCodeData = NULL;
        KCodeManager*    pCodeManager = pIOTable->GetOwner()->GetCodeManager();

        if (bCodeType)
        {
            pCodeData = pCodeManager->FindCodeGroup(pIOColumn->CodeGroup());
            pCodeData->GetCodes(a_mapCode);
        }
    }
    catch (int& ex) {
    	
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    }
    catch (...) {
        TxLogDebugException();
    }
}

void KDlgTransitStyle::UpdateReport()
{
    BOOL bSelectAll(FALSE);
    if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
        bSelectAll = TRUE;
    else
        bSelectAll = FALSE;

    m_ctrlReport.ResetContent();

    std::map<int, CString> mapTypeInfo; {
        GetCodes(COLUMN_TRANSIT_TYPE, mapTypeInfo);
    }
    std::map<int, CString> mapDetailTypeInfo; {
        GetCodes(COLUMN_TRANSIT_DETAIL_TYPE, mapDetailTypeInfo);
    }

    try
    {
        auto iter  = m_setReportList.begin();
        auto itEnd = m_setReportList.end();

        CXTPReportRecord*     pRecord = nullptr;
        CXTPReportRecordItem* pItem   = nullptr;

        CString strID(_T(""));
        CString strCodeVal1(_T(""));
        CString strCodeVal2(_T(""));

        for (;iter != itEnd; ++iter)
        {
            Integer nxID = *iter;

            auto itFindNm = m_mapTransitInfo.find(nxID);
            if (itFindNm == m_mapTransitInfo.end()) {
                continue;
            }
            TTransitMainInfo &oTransitInfo = itFindNm->second;

            pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

            pItem = pRecord->AddItem(new KExRecordItemCheck);
            pItem->HasCheckbox(TRUE);
            pItem->SetChecked(bSelectAll);
            pItem->SetItemData((DWORD_PTR)nxID);
            pItem->SetAlignment(xtpColumnIconCenter);

            pItem = pRecord->AddItem(new CXTPReportRecordItem());
            strID.Format(_T("%I64d"), nxID);
            pItem->SetCaption(strID);
            pItem->SetAlignment(DT_CENTER);

            int nCode = oTransitInfo.TType;
            auto itFind1 = mapTypeInfo.find(nCode);
            if (itFind1 != mapTypeInfo.end()) {
                if (nCode != 0)
                {
                    strCodeVal1.Format(_T("%d:%s"), nCode, itFind1->second);
                }
                else
                {
                    strCodeVal1 = itFind1->second;
                }

            }
            else {
                strCodeVal1.Format(_T("%d:%s"), nCode, _T("(x)"));
            }
            pItem = pRecord->AddItem(new CXTPReportRecordItem());
            pItem->SetCaption(strCodeVal1);
            pItem->SetAlignment(DT_CENTER);
            pItem->SetItemData((DWORD_PTR)nCode);

            nCode = oTransitInfo.TDetailType;
            auto itFind2 = mapDetailTypeInfo.find(nCode);
            if (itFind2 != mapDetailTypeInfo.end()) {
                if (nCode != 0)
                {
                    strCodeVal2.Format(_T("%d:%s"), nCode, itFind2->second);
                }
                else
                {
                    strCodeVal2 = itFind2->second;
                }
            }
            else {
                strCodeVal2.Format(_T("%d:%s"), nCode, _T("(x)"));
            }
            pItem = pRecord->AddItem(new CXTPReportRecordItem());
            pItem->SetCaption(strCodeVal2);
            pItem->SetAlignment(DT_CENTER);
            pItem->SetItemData((DWORD_PTR)nCode);

            pItem = pRecord->AddItem(new CXTPReportRecordItem());
            pItem->SetCaption(oTransitInfo.TName);
            pItem->SetAlignment(DT_CENTER);
        }
    }
    catch (...)
    {
        m_ctrlReport.ResetContent();
        TxLogDebugException();
    }

    m_ctrlReport.Populate();
}

void KDlgTransitStyle::OnBnClickedButtonSearchTransit()
{
    try
    {
        std::set<Integer> setSelReportID;
        GetSelectedReportID(OUT setSelReportID);

        KIOTables* pIOTables = m_pTarget->Tables();
        KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_TRANSIT);

        KQuickSearchDlg oDlg(pIOTable);
        oDlg.SetSelectedID(IN setSelReportID);

        if (oDlg.DoModal() == IDOK) {
            oDlg.GetSelectedID(OUT setSelReportID);

            /*Append의 개념이라 처리 상황이 복잡함
             검색창에서 검색한 내용이 그리기 설정창에 추가가 되어야 하고,
             검색창에서 검색된 내용만이 체크상태가 되어야 한다.*/

            auto iter = setSelReportID.begin();
            auto itEd = setSelReportID.end();
            for (; iter != itEd; ++iter)
            {
                m_setReportList.insert(*iter);
            }
            UpdateReport(); // 새내용이 추가된 리스트로 리셋하고

            bool bAllSelect(true);

            // 체크 현황 업데이트
            CXTPReportRecords*    pRecords = m_ctrlReport.GetRecords();
            CXTPReportRecordItem* pItem    = nullptr;
            int nCount = pRecords->GetCount();
            for (int i=0; i< nCount; i++)
            {
                CXTPReportRecord* pRecord = pRecords->GetAt(i);
                pItem = pRecord->GetItem(0);
                Integer nxID = (Integer)pItem->GetItemData();

                auto itFind = setSelReportID.find(nxID);
                if (itFind != setSelReportID.end()) {
                    pItem->SetChecked(TRUE);
                }
                else {
                    pItem->SetChecked(FALSE);
                    bAllSelect = false;
                }
            }

            if (bAllSelect) {
                CheckDlgButton(IDC_CHECK1, BST_CHECKED);
            }
            else {
                CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
            }
             
            m_ctrlReport.RedrawControl();
        }
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KDlgTransitStyle::OnBnClickedCheckSelectAll()
{
    try
    {
        BOOL bSelectAll(FALSE);
        if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
            bSelectAll = TRUE;
        else
            bSelectAll = FALSE;

        CXTPReportRecords*    pRecords = m_ctrlReport.GetRecords();
        CXTPReportRecord*     pRecord = nullptr;
        CXTPReportRecordItem* pItem = nullptr;

        int nRecordCnt = pRecords->GetCount();
        for (int i = 0; i < nRecordCnt; i++)
        {
            pRecord = pRecords->GetAt(i);
            pItem = pRecord->GetItem(0);

            pItem->SetChecked(bSelectAll);
        }

        m_ctrlReport.RedrawControl();
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KDlgTransitStyle::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
    XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

    if (!pItemNotify->pRow || !pItemNotify->pColumn)
        return;

    int nIndex = pItemNotify->pColumn->GetItemIndex();

    if (0 == nIndex)
    {
        try
        {
            CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
            CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecord->GetItem(nIndex);

            if (pItem->IsChecked() == FALSE)
            {
                CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
            }
        }
        catch (...)
        {
            TxLogDebugException();
        }
    }
}

void KDlgTransitStyle::OnBnClickedBtnApply()
{
    if (false == InvalidateInputInfo()) {
        return;
    }

    GetSelectedReportID(OUT m_setApplyID);
    if (m_setApplyID.size() < 1) {

        CString strMsg(_T(""));
        if (KmzSystem::GetLanguage() == KEMKorea) {
            AfxMessageBox(_T("노선을 선택해 주세요."));
        }
        else {
            AfxMessageBox(_T("Please Select Line"));
        }
        return;
    }

    // 결과 저장
    CString strValue(_T("")); 
    GetDlgItemText(IDC_EDIT_SIZE, strValue);
    m_dWidth  = _ttof(strValue);

    bool bUseCustColor(false); {
        if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED) {
            bUseCustColor = true;
        }
    }
    
    m_clrLine = m_cpLineColor.GetColor();

    if (nullptr != m_pTransitView) {
//         auto iter = m_setApplyID.begin();
//         auto end  = m_setApplyID.end();
// 
//         for (; iter != end; ++iter) {
//             const Integer &nxID = *iter;
// 
//             m_pTransitView->RegisterTransitStyle(m_setApplyID, bUseCustColor, m_clrLine, m_dWidth, m_dOffset); // Register 개념
//         }
        m_pTransitView->RegisterTransitStyle(m_setApplyID, bUseCustColor, m_clrLine, m_dWidth, m_dOffset); // Register 개념
        
        m_pTransitView->RfreshTransitStyle();
    }
}

bool KDlgTransitStyle::InvalidateInputInfo()
{
    try
    {
        CString strValue(_T(""));

        //두께 체크
        GetDlgItemText(IDC_EDIT_SIZE, strValue);
        if (QBicStringChecker::IsNumeric(strValue) == false)
        {
            CString strMsg(_T(""));
            if (KmzSystem::GetLanguage() == KEMKorea) {
                strMsg.Format(_T("숫자를 입력해 주세요. (%.1f~%.1f)"), m_dMinWidth, m_dMaxWidth);
            }
            else {
                strMsg.Format(_T("Please enter a number (%.1f~%.1f)"), m_dMinWidth, m_dMaxWidth);
            }

            AfxMessageBox(strMsg);
            GetDlgItem(IDC_EDIT_SIZE)->SetFocus();
            return false;
        }
        else {
            double dSize = _ttof(strValue);
            if (dSize < m_dMinWidth || dSize > m_dMaxWidth) {
                CString strMsg(_T(""));
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    strMsg.Format(_T("숫자를 입력해 주세요. (%.1f~%.1f)"), m_dMinWidth, m_dMaxWidth);
                }
                else {
                    strMsg.Format(_T("Please enter a number (%.1f~%.1f)"), m_dMinWidth, m_dMaxWidth);
                }

                AfxMessageBox(strMsg);
                GetDlgItem(IDC_EDIT_SIZE)->SetFocus();
                return false;
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
    return true;
}

void KDlgTransitStyle::GetSelectedReportID(std::set<Integer> &a_setSelected)
{
    try
    {
        a_setSelected.clear();

        CXTPReportRecords*    pRecords = m_ctrlReport.GetRecords();
        CXTPReportRecord*     pRecord  = nullptr;
        CXTPReportRecordItem* pItem    = nullptr;

        int nRecordCnt = pRecords->GetCount();
        for (int i = 0; i < nRecordCnt; i++)
        {
            pRecord = pRecords->GetAt(i);
            pItem = pRecord->GetItem(0);

            if (pItem->IsChecked() == FALSE)
            {
                continue;
            }

            Integer nxTransitID = (Integer)pItem->GetItemData();

            a_setSelected.insert(nxTransitID);
        }
    }
    catch (...)
    {
        a_setSelected.clear();
        TxLogDebugException();
    }
}

void KDlgTransitStyle::OnBnClickedCancel()
{
    CloseWindowProc();
    KResizeDialogEx::OnCancel();
}

void KDlgTransitStyle::CloseWindowProc()
{
   //sp reset
}

void KDlgTransitStyle::OnBnClickedBtnStyleReset()
{
    if (nullptr != m_pTransitView) {
        m_pTransitView->InitTransitStyle();
    }
}


void KDlgTransitStyle::OnBnClickedCheckUseClr()
{
    CotrolCheckClr();
}

void KDlgTransitStyle::CotrolCheckClr()
{
    if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED) {
        GetDlgItem(IDC_CLR_LINE)->EnableWindow(TRUE);
    }
    else {
        GetDlgItem(IDC_CLR_LINE)->EnableWindow(FALSE);
    }
}
