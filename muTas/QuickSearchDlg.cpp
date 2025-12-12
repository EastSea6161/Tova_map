// QuickSearchDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuickSearchDlg.h"
#include "afxdialogex.h"

#include "DBaseQuickSearch.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"

// KQuickSearchDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KQuickSearchDlg, KResizeDialogEx)

KQuickSearchDlg::KQuickSearchDlg(KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KQuickSearchDlg::IDD, pParent)
	, m_pTable(a_pTable)
{
	m_pColumns   = new KIOColumns;
	m_pKeyColumn = nullptr;
}

KQuickSearchDlg::~KQuickSearchDlg()
{
	if (m_pColumns != nullptr)
	{
		delete m_pColumns;
		m_pColumns = nullptr;
	}
}

void KQuickSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndReport);
	DDX_Control(pDX, IDC_REPORT1, m_wndSelReport);
	DDX_Control(pDX, IDC_COMBO1,   m_cboFindColumn);
}

BEGIN_MESSAGE_MAP(KQuickSearchDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT,  &KQuickSearchDlg::OnReportItemClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, &KQuickSearchDlg::OnReport1ItemClick)
	ON_BN_CLICKED(IDOK, &KQuickSearchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KQuickSearchDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &KQuickSearchDlg::OnChangeEditSearch)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &KQuickSearchDlg::OnBnClickedButtonFind)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KQuickSearchDlg::OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK1, &KQuickSearchDlg::OnBnClickedCheckSelectAll)
END_MESSAGE_MAP()


// KQuickSearchDlg 메시지 처리기입니다.


void KQuickSearchDlg::InitColumns( void )
{
	try
	{
		CString strName = m_pTable->Name();
		CString strFiledNm(_T(""));

        KIOColumns* pColumns = m_pTable->Columns();
        int         nColumn  = pColumns->ColumnCount();

		if (strName.CompareNoCase(TABLE_NODE) == 0 )
		{
			for (int i = 0; i < nColumn; ++i)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);

				if (pColumn->DataType() == KEMIODataTypeDouble)
					continue;
				strFiledNm = pColumn->Name();
				
				if (strFiledNm.CompareNoCase(COLUMN_NODE_ID) == 0)
					m_pKeyColumn = pColumn;

				m_pColumns->AddNewColumn(pColumn);
			}
		}
		else if (strName.CompareNoCase(TABLE_LINK) == 0)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);

				if (pColumn->DataType() == KEMIODataTypeDouble)
					continue;
				strFiledNm = pColumn->Name();

				if (strFiledNm.CompareNoCase(COLUMN_LINK_ID) == 0)
					m_pKeyColumn = pColumn;

				m_pColumns->AddNewColumn(pColumn);
			}
		}
		else if (strName.CompareNoCase(TABLE_ZONE) == 0)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);

				if (pColumn->DataType() == KEMIODataTypeDouble)
					continue;
				strFiledNm = pColumn->Name();

				if (strFiledNm.CompareNoCase(COLUMN_ZONE_ID) == 0)
					m_pKeyColumn = pColumn;

				m_pColumns->AddNewColumn(pColumn);
			}
		}
        else if (strName.CompareNoCase(TABLE_TRANSIT) == 0) {// 노선 주제도 그리기 설정에서 호출 됨
            for (int i = 0; i < nColumn; i++)
            {
                KIOColumn* pColumn = pColumns->GetColumn(i);

                if (pColumn->DataType() == KEMIODataTypeDouble)
                    continue;

                strFiledNm = pColumn->Name();

                if (strFiledNm.CompareNoCase(COLUMN_TRANSIT_ID) == 0)
                    m_pKeyColumn = pColumn;

                if (strFiledNm.CompareNoCase(COLUMN_TRANSIT_ID) == 0 ||
                    strFiledNm.CompareNoCase(COLUMN_TRANSIT_TYPE) == 0 ||
                    strFiledNm.CompareNoCase(COLUMN_TRANSIT_DETAIL_TYPE) == 0 ||
                    strFiledNm.CompareNoCase(COLUMN_TRANSIT_NAME) == 0 ) {

                    m_pColumns->AddNewColumn(pColumn);
                }
                else {
                    if (pColumn->ColumnType() == KEMIOColumnTypeUserDefine) { // 사용자 정의 컬럼 허용(Custom그룹 검색을 위한)
                        m_pColumns->AddNewColumn(pColumn);
                    }
                }
            }
        }
		else
		{
			CString strMsg;
			strMsg.Format(_T("정의되지 않은 테이블입니다 - %s"), strName);
			ThrowException(strMsg);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebug(_T("unexepcted error - KQuickSearchDlg::InitColumns"));
		throw 1;
	}
}

void KQuickSearchDlg::InitResizeCtrl( void )
{
	try
	{
		SetResize(IDC_EDIT_SEARCH,     SZ_TOP_LEFT,               SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON_FIND,     SZ_TOP_RIGHT,              SZ_TOP_RIGHT);
		SetResize(IDC_REPORT,          SZ_TOP_LEFT,               CXTPResizePoint(1.0, 0.5));
		SetResize(IDC_REPORT1,         CXTPResizePoint(0.0, 0.5), CXTPResizePoint(1.0, 1.0));
		SetResize(IDC_STATIC_SELECTED, CXTPResizePoint(0.0, 0.5), CXTPResizePoint(1.0, 0.5));
		SetResize(IDC_STATIC_BOTTOM,   SZ_BOTTOM_LEFT,            SZ_BOTTOM_RIGHT);
		SetResize(IDOK,                SZ_BOTTOM_RIGHT,           SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,            SZ_BOTTOM_RIGHT,           SZ_BOTTOM_RIGHT);
		SetResize(IDC_CHECK1,          SZ_TOP_LEFT,              SZ_TOP_LEFT);
	}
	catch (...)
	{
		TxLogDebug(_T("unexepcted error - KQuickSearchDlg::InitResizeCtrl"));
		throw 1;
	}
}

void KQuickSearchDlg::InitComboCtrl( void )
{
	try
	{
		m_cboFindColumn.ResetContent();

		int         nColumn  = m_pColumns->ColumnCount();
		int         nIndex;

		for (int i = 0; i < nColumn; ++i)
		{
			KIOColumn* pColumn = m_pColumns->GetColumn(i);

			nIndex = m_cboFindColumn.InsertString(-1, pColumn->Caption());
			m_cboFindColumn.SetItemData(nIndex, (DWORD_PTR)pColumn);
		}

		m_cboFindColumn.SetCurSel(0);
	}
	catch (...)
	{
		TxLogDebug(_T("unexepcted error - KQuickSearchDlg::InitComboCtrl"));
		throw 1;	
	}
}

void KQuickSearchDlg::InitReportHeader( void )
{
	try
	{
		KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);
		KReportCtrlSetting::AllowColumnsort(m_wndReport);

		KReportCtrlSetting::Default(m_wndSelReport, FALSE, FALSE, FALSE);
		KReportCtrlSetting::AllowColumnsort(m_wndSelReport);

		int nColumn  = m_pColumns->ColumnCount();

		CXTPReportColumn* pColumn = nullptr;

		//Select Report
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }
		
		//Selected Report
		pColumn = m_wndSelReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		for (int i = 0; i < nColumn; ++i)
		{
			KIOColumn* pIOColumn = m_pColumns->GetColumn(i);

			if (i == 0)
			{
				//Select Report
				pColumn = m_wndReport.AddColumn(new CXTPReportColumn(i + 1, pIOColumn->Caption(), 2));
				pColumn->SetHeaderAlignment(DT_CENTER);
				pColumn->SetSortable(TRUE);

				//Selected Report;
				pColumn = m_wndSelReport.AddColumn(new CXTPReportColumn(i + 1, pIOColumn->Caption(), 2));
				pColumn->SetHeaderAlignment(DT_CENTER);
				pColumn->SetSortable(TRUE);
			}
			else
			{
				//Select Report
				pColumn = m_wndReport.AddColumn(new CXTPReportColumn(i + 1, pIOColumn->Caption(), 3));
				pColumn->SetHeaderAlignment(DT_CENTER);
				pColumn->SetSortable(TRUE);

				//Selected Report;
				pColumn = m_wndSelReport.AddColumn(new CXTPReportColumn(i + 1, pIOColumn->Caption(), 3));
				pColumn->SetHeaderAlignment(DT_CENTER);
				pColumn->SetSortable(TRUE);
			}
		}

		m_wndReport.Populate();
		m_wndSelReport.Populate();
	}
	catch (...)
	{
		TxLogDebug(_T("unexepcted error - KQuickSearchDlg::InitReportHeader"));
		throw 1;
	}
}

BOOL KQuickSearchDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    if (true) {
        CString strName = m_pTable->Name();
        if (strName.CompareNoCase(TABLE_TRANSIT) == 0) {

            if (KmzSystem::GetLanguage() == KEMKorea) {
                SetWindowText(_T("노선검색"));
            }
            else {
                SetWindowText(_T("Search Line"));
            }
        }
    }

	try
	{
		InitColumns();
		InitComboCtrl();
		InitResizeCtrl();
		InitReportHeader();
		UpdateReportHeader();
		
		KDBaseQuickSearch::GetReportData(m_pTable, m_pColumns, m_mapData);
		ConvertCodeData();

		DrawReport();
        DrawReport2();
		
		AddImChampTooltip(IDC_EDIT_SEARCH, _T("검색어를 입력해주세요"));
		AddImChampTooltip(IDC_REPORT,      _T("선택해 주세요"));
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
		KResizeDialogEx::OnCancel();
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error - KQuickSearchDlg::OnInitDialog"));
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		KResizeDialogEx::OnCancel();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KQuickSearchDlg::OnBnClickedOk()
{
	
	if (m_setSelection.size() == 0)
	{
		m_strFilter = _T("");
	}
	else
	{
		CString strFilter = _T("rowid IN (");
		
		std::set<Integer>::iterator bIter, eIter = m_setSelection.end();

		for (bIter = m_setSelection.begin(); bIter != eIter; ++bIter)
		{
			strFilter.AppendFormat(_T("%I64d, "), *bIter);
		}

		strFilter.Truncate(strFilter.GetLength() - 2);
		strFilter.Append(_T(")"));

		m_strFilter = strFilter;
	}
	
	KResizeDialogEx::OnOK();
}

void KQuickSearchDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KQuickSearchDlg::OnChangeEditSearch()
{
	try
	{
		OnBnClickedButtonFind();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KQuickSearchDlg::OnBnClickedButtonFind()
{
	try
	{
		CString strFilter = _T("");
		GetDlgItemText(IDC_EDIT_SEARCH, strFilter);
		DrawReport(strFilter);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KQuickSearchDlg::DrawReport( CString a_strFilter )
{
	try
	{
		m_wndReport.ResetContent(FALSE);

		int nIndexCbo = m_cboFindColumn.GetCurSel();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		std::set<Integer>::iterator eSetIter = m_setSelection.end();
		std::map<Integer, std::vector<CString>>::iterator bIter, eIter = m_mapData.end();
		
		Integer nxRowID;
		std::vector<CString> vecRow;

		for (bIter = m_mapData.begin(); bIter != eIter; ++bIter)
		{
			nxRowID = bIter->first;
			vecRow  = bIter->second;
			
			if (!a_strFilter.IsEmpty() && vecRow[nIndexCbo].Find(a_strFilter) < 0)
				continue;

			pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->HasCheckbox(TRUE);
			pItem->SetItemData((DWORD_PTR)nxRowID);

			if (m_setSelection.find(nxRowID) != eSetIter)
				pItem->SetChecked(TRUE);
			
			int nRow = vecRow.size();

			for (int j = 0; j < nRow; ++j)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(vecRow[j]));
				pItem->SetAlignment(DT_CENTER);
			}
		}

		m_wndReport.Populate();
	}
	catch (...)
	{
		TxLogDebug(_T("unexepcted error - KQuickSearchDlg::DrawReport"));
		throw 1;
	}
}

void KQuickSearchDlg::DrawReport2( void )
{
	try
	{
		m_wndSelReport.ResetContent(FALSE);
		
		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

        auto iterSel = m_setSelection.begin();
        auto itEdSel = m_setSelection.end();
        for (; iterSel != itEdSel; ++iterSel)
        {
            const Integer &nxID = *iterSel;

            auto itFind = m_mapData.find(nxID);
            if (itFind == m_mapData.end()) {
                continue;
            }

            std::vector<CString> &vecRow = itFind->second;

            pRecord = m_wndSelReport.AddRecord(new CXTPReportRecord);

            pItem = pRecord->AddItem(new CXTPReportRecordItem);
            pItem->SetAlignment(xtpColumnIconCenter);
            pItem->HasCheckbox(TRUE);
            pItem->SetChecked(TRUE);
            pItem->SetItemData((DWORD_PTR)nxID);

            int nRow = vecRow.size();

            for (int j = 0; j < nRow; ++j)
            {
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(vecRow[j]));
                pItem->SetAlignment(DT_CENTER);
            }
        }

		m_wndSelReport.Populate();
	}
	catch (...)
	{
		TxLogDebug(_T("unexepcted error - KQuickSearchDlg::DrawReport2"));
		throw 1;
	}
}

void KQuickSearchDlg::OnReportItemClick( NMHDR* a_pNMHDR, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)a_pNMHDR;

		if (!pClickedItem->pRow || !pClickedItem->pColumn)
			return;

		int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

		if (nColumnIndex != 0) 
			return;

		CXTPReportRecord*     pRecord = pClickedItem->pRow->GetRecord();
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);
		Integer               nxRowID = (Integer)pItem->GetItemData();

		if (pItem->IsChecked())
		{
			m_setSelection.insert(nxRowID);
		}
		else
		{
			CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
			m_setSelection.erase(nxRowID);
		}		

		DrawReport2();		
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KQuickSearchDlg::OnReport1ItemClick( NMHDR* a_pNMHDR, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)a_pNMHDR;

		if (!pClickedItem->pRow || !pClickedItem->pColumn)
			return;

		int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

		if (nColumnIndex != 0) 
			return;

		CXTPReportRecord*     pRecord = pClickedItem->pRow->GetRecord();
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);
		Integer               nxRowID = (Integer)pItem->GetItemData();

		if (!pItem->IsChecked())
			m_setSelection.erase(nxRowID);

		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
		OnBnClickedButtonFind();
		DrawReport2();		
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KQuickSearchDlg::OnSelchangeCombo1()
{
	try
	{
		UpdateReportHeader();
		OnBnClickedButtonFind();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KQuickSearchDlg::OnBnClickedCheckSelectAll( void )
{
	try
	{
		BOOL bSelectedAll = IsDlgButtonChecked(IDC_CHECK1);
		
		CXTPReportRecords*    pRecords = m_wndReport.GetRecords();
		CXTPReportRecordItem* pItem    = nullptr;
		int                   nRecord  = pRecords->GetCount();

		if (bSelectedAll)
		{
			for (int i = 0; i < nRecord; ++i)
			{
				pItem = pRecords->GetAt(i)->GetItem(0);
				pItem->SetChecked(TRUE);

				m_setSelection.insert((Integer)pItem->GetItemData());
			}
		}
		else
		{
			for (int i = 0; i < nRecord; ++i)
			{
				pItem = pRecords->GetAt(i)->GetItem(0);
				pItem->SetChecked(FALSE);

				m_setSelection.erase((Integer)pItem->GetItemData());
			}
		}

		m_wndReport.RedrawControl();
		DrawReport2();
	}
	catch (...)
	{
		
	}
}

void KQuickSearchDlg::UpdateReportHeader( void )
{
	CXTPReportColumn* pColumn       = nullptr;
	KIOColumn*        pSelColumn    = (KIOColumn*)m_cboFindColumn.GetItemData(m_cboFindColumn.GetCurSel());
	CString           strSelColName = pSelColumn->Caption();
	CString           strTblName    = m_pTable->Name();
	CString           strValue;
	

	if (strTblName.CompareNoCase(TABLE_NODE) == 0)
	{
		//m_wndReport
		int nColumn = m_wndReport.GetColumns()->GetCount();

		for (int i = 2; i < nColumn; ++i)	//CheckBox와, 첫번째 Key Column은 제외함
		{
			pColumn = m_wndReport.GetColumns()->GetAt(i);

			if (strSelColName.CompareNoCase(pColumn->GetCaption()) == 0)
			{
				pColumn->SetVisible(TRUE);
			}
			else
			{
				pColumn->SetVisible(FALSE);
			}
		}

		//단 ID를 선택했을 경우 Name을 강제로 추가함
		if (strSelColName.CompareNoCase(m_pKeyColumn->Caption()) == 0)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				pColumn  = m_wndReport.GetColumns()->GetAt(i);
				strValue = pColumn->GetCaption();
				
				if (strValue.CompareNoCase(COLUMN_NODE_NAME) == 0)
					pColumn->SetVisible(TRUE);
			}
		}

		//m_wndSelReport
		nColumn = m_wndSelReport.GetColumns()->GetCount();

		for (int i = 2; i < nColumn; ++i)	//CheckBox와, 첫번째 Key Column은 제외함
		{
			pColumn = m_wndSelReport.GetColumns()->GetAt(i);

			if (strSelColName.CompareNoCase(pColumn->GetCaption()) == 0)
			{
				pColumn->SetVisible(TRUE);
			}
			else
			{
				pColumn->SetVisible(FALSE);
			}
		}

		//단 ID를 선택했을 경우 Name을 강제로 추가함
		if (strSelColName.CompareNoCase(m_pKeyColumn->Caption()) == 0)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				pColumn  = m_wndSelReport.GetColumns()->GetAt(i);
				strValue = pColumn->GetCaption();

				if (strValue.CompareNoCase(COLUMN_NODE_NAME) == 0)
					pColumn->SetVisible(TRUE);
			}
		}
	}	
	else if (strTblName.CompareNoCase(TABLE_LINK) == 0)
	{
		//m_wndReport
		int nColumn = m_wndReport.GetColumns()->GetCount();

		for (int i = 2; i < nColumn; ++i)	//CheckBox와, 첫번째 Key Column은 제외함
		{
			pColumn = m_wndReport.GetColumns()->GetAt(i);

			if (strSelColName.CompareNoCase(pColumn->GetCaption()) == 0)
			{
				pColumn->SetVisible(TRUE);
			}
			else
			{
				pColumn->SetVisible(FALSE);
			}
		}

		//단 ID를 선택했을 경우 Name을 강제로 추가함
		if (strSelColName.CompareNoCase(m_pKeyColumn->Caption()) == 0)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				pColumn  = m_wndReport.GetColumns()->GetAt(i);
				strValue = pColumn->GetCaption();

				if (strValue.CompareNoCase(COLUMN_LINK_NAME) == 0)
					pColumn->SetVisible(TRUE);
			}
		}

		//m_wndSelReport
		nColumn = m_wndSelReport.GetColumns()->GetCount();

		for (int i = 2; i < nColumn; ++i)	//CheckBox와, 첫번째 Key Column은 제외함
		{
			pColumn = m_wndSelReport.GetColumns()->GetAt(i);

			if (strSelColName.CompareNoCase(pColumn->GetCaption()) == 0)
			{
				pColumn->SetVisible(TRUE);
			}
			else
			{
				pColumn->SetVisible(FALSE);
			}
		}

		//단 ID를 선택했을 경우 Name을 강제로 추가함
		if (strSelColName.CompareNoCase(m_pKeyColumn->Caption()) == 0)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				pColumn  = m_wndSelReport.GetColumns()->GetAt(i);
				strValue = pColumn->GetCaption();

				if (strValue.CompareNoCase(COLUMN_LINK_NAME) == 0)
					pColumn->SetVisible(TRUE);
			}
		}
	}
	else if (strTblName.CompareNoCase(TABLE_ZONE) == 0)
	{
		//m_wndReport
		int nColumn = m_wndReport.GetColumns()->GetCount();

		for (int i = 2; i < nColumn; ++i)	//CheckBox와, 첫번째 Key Column은 제외함
		{
			pColumn = m_wndReport.GetColumns()->GetAt(i);

			if (strSelColName.CompareNoCase(pColumn->GetCaption()) == 0)
			{
				pColumn->SetVisible(TRUE);
			}
			else
			{
				pColumn->SetVisible(FALSE);
			}
		}

		//단 ID를 선택했을 경우 Name을 강제로 추가함
		if (strSelColName.CompareNoCase(m_pKeyColumn->Caption()) == 0)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				pColumn  = m_wndReport.GetColumns()->GetAt(i);
				strValue = pColumn->GetCaption();

				if (strValue.CompareNoCase(COLUMN_ZONE_NAME) == 0)
					pColumn->SetVisible(TRUE);
			}
		}

		//m_wndSelReport
		nColumn = m_wndSelReport.GetColumns()->GetCount();

		for (int i = 2; i < nColumn; ++i)	//CheckBox와, 첫번째 Key Column은 제외함
		{
			pColumn = m_wndSelReport.GetColumns()->GetAt(i);

			if (strSelColName.CompareNoCase(pColumn->GetCaption()) == 0)
			{
				pColumn->SetVisible(TRUE);
			}
			else
			{
				pColumn->SetVisible(FALSE);
			}
		}

		//단 ID를 선택했을 경우 Name을 강제로 추가함
		if (strSelColName.CompareNoCase(m_pKeyColumn->Caption()) == 0)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				pColumn  = m_wndSelReport.GetColumns()->GetAt(i);
				strValue = pColumn->GetCaption();

				if (strValue.CompareNoCase(COLUMN_ZONE_NAME) == 0)
					pColumn->SetVisible(TRUE);
			}
		}
	}
}

void KQuickSearchDlg::ConvertCodeData()
{
	int nColumn = m_pColumns->ColumnCount();
	
	KCodeManager* pCodeMgr  = m_pTable->GetOwner()->GetCodeManager();
	KCodeGroup*   pCodeData = nullptr;
	KIOColumn*    pColumn   = nullptr;

	std::map<Integer, std::vector<CString>>::iterator bIter, eIter = m_mapData.end();

	for (int i = 0; i < nColumn; ++i)
	{
		KIOColumn* pColumn = m_pColumns->GetColumn(i);

		if (pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			pCodeData = pCodeMgr->FindCodeGroup(pColumn->CodeGroup());

			for (bIter = m_mapData.begin(); bIter != eIter; ++bIter)
			{
				std::vector<CString>& vecData = bIter->second;

				if (vecData.size() != nColumn)
					ThrowException(_T("데이터 변환중 오류가 발생했습니다."));

				int nCode = _ttoi(vecData[i]);

				vecData[i] = pCodeData->SingleCodeValue(nCode, 1);
			}
		}
		else if (pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
		{
			pCodeData = pCodeMgr->FindCodeGroup(pColumn->CodeGroup());

			for (bIter = m_mapData.begin(); bIter != eIter; ++bIter)
			{
				std::vector<CString>& vecData = bIter->second;

				if (vecData.size() != nColumn)
					ThrowException(_T("데이터 변환중 오류가 발생했습니다."));

				CString strValue = vecData[i];

				vecData[i] = pCodeData->MultiCodeValue(strValue, 1);
			}
		}
	}
}
