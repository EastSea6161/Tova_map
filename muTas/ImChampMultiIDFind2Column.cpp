#include "stdafx.h"
#include "ImChampMultiIDFind2Column.h"
#include "afxdialogex.h"
#include "MapView.h"

IMPLEMENT_DYNAMIC(KImChampMultiIDFind2Column, KResizeDialogEx)

CString KImChampMultiIDFind2Column::m_strPreSearch = _T("");

KImChampMultiIDFind2Column::KImChampMultiIDFind2Column(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KImChampMultiIDFind2Column::IDD, pParent), m_pMapView(NULL), m_nMapSelectType(-1), m_nDrawnMap(false), m_strTiltleName(_T("")), m_nNecessaryCount(0)
{
	m_strReportTitle = _T("Select ID:");
}

KImChampMultiIDFind2Column::~KImChampMultiIDFind2Column()
{
	if (m_nDrawnMap == true && m_pMapView != nullptr)
	{
		m_pMapView->ClearScreenDrawEdit();
	}
}

void KImChampMultiIDFind2Column::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndReport);
	DDX_Control(pDX, IDC_REPORT2, m_wndReport2);
}

BEGIN_MESSAGE_MAP(KImChampMultiIDFind2Column, KResizeDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KImChampMultiIDFind2Column::OnReportRowDblClick)
	ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KImChampMultiIDFind2Column::OnReportItemClick)

	ON_NOTIFY(NM_CLICK,  IDC_REPORT2, &KImChampMultiIDFind2Column::OnReport2ItemClick)

	ON_BN_CLICKED(IDC_BUTTON_FIND, &KImChampMultiIDFind2Column::OnBnClickedButtonFind)
	ON_EN_CHANGE(IDC_EDIT_SEARCH,  &KImChampMultiIDFind2Column::OnEnChangeEditSearch)

	ON_BN_CLICKED(IDOK, &KImChampMultiIDFind2Column::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_MAP, &KImChampMultiIDFind2Column::OnBnClickedMap)
	ON_BN_CLICKED(IDCANCEL, &KImChampMultiIDFind2Column::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &KImChampMultiIDFind2Column::OnBnClickedCheckSelectAll)
END_MESSAGE_MAP()


// KImChampMultiIDFind2Column 메시지 처리기입니다.
BOOL KImChampMultiIDFind2Column::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if (m_strTiltleName != _T(""))
	{
		SetWindowText(m_strTiltleName);
	}

	if( !m_imageListSelectIcon.Create( 16, 16, ILC_MASK | ILC_COLOR24, 0, 1) )
	{
		AfxMessageBox( _T("Failed to create image list") );
		return -1;
	}

	CBitmap bmp;
	bmp.LoadBitmap( IDB_BITMAP_RADIO );
	m_imageListSelectIcon.Add( &bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0) ) );

	m_wndReport.SetImageList(&m_imageListSelectIcon);

	InitialControl();

	SetDlgItemText(IDC_STATIC5,     m_strReportTitle);
	SetDlgItemText(IDC_EDIT_SEARCH, m_strPreSearch);
	CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);

	SetNativeLanguageMode(IDC_EDIT_SEARCH);
	AddImChampTooltip(IDC_EDIT_SEARCH, _T("검색어를 입력해주세요"));
	AddImChampTooltip(IDC_REPORT,      _T("선택해 주세요"));
	DrawReport (m_strPreSearch);  
	DrawReport2();

	return TRUE;  
}

void KImChampMultiIDFind2Column::InitialControl()
{
	try
	{
		KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);
		KReportCtrlSetting::AllowColumnsort(m_wndReport);

		CXTPReportColumn* pColumn = NULL;
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("선택"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("ID"),     40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Name"),   65));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("명칭"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(3, m_strSecondColumnName,   65));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);

		m_wndReport.Populate();

		KReportCtrlSetting::Default(m_wndReport2, FALSE, FALSE, FALSE);
		KReportCtrlSetting::AllowColumnsort(m_wndReport2);

		pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(0, _T("Select"), 50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("선택"));
		}

		pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(1, _T("ID"),     40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);

		pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(2, _T("Name"),   65));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("명칭"));
		}

		m_wndReport2.Populate();

		SetResize(IDC_EDIT_SEARCH,    SZ_TOP_LEFT,     SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON_FIND,    SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON_MAP,    SZ_TOP_RIGHT,    SZ_TOP_RIGHT);

		SetResize(IDC_STATIC_HBar1,   SZ_TOP_LEFT,     SZ_TOP_RIGHT);
		SetResize(IDC_CHECK1, SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_REPORT,         SZ_TOP_LEFT,     CXTPResizePoint(1.0, 0.5));

		SetResize(IDC_STATIC_SELECTED, CXTPResizePoint(0.0, 0.5), CXTPResizePoint(1.0, 0.5));       
		SetResize(IDC_REPORT2,         CXTPResizePoint(0.0, 0.5), CXTPResizePoint(1.0, 1.0));
		SetResize(IDC_STATIC_HBar2,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC_INFO,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);    
		SetResize(IDOK,               SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,           SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

		SetDlgItemText(IDC_STATIC_INFO, _T(""));

		if (m_nMapSelectType == 0)
		{
			GetDlgItem(IDC_BUTTON_MAP)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_MAP)->EnableWindow(FALSE);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KImChampMultiIDFind2Column::OnReportRowDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		if (nullptr == pClickedItem)
			return;

		CXTPReportRow* pRow = pClickedItem->pRow;
		if (nullptr == pRow)
			return;
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KImChampMultiIDFind2Column::OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) pNMHDR;

		if (!pClickedItem->pRow || !pClickedItem->pColumn)
			return;

		int nRowIndex    = pClickedItem->pRow->GetIndex();
		int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

		CXTPReportRecords*	  pRecords = NULL;
		CXTPReportRecord*     pRecord  = NULL;
		CXTPReportRecordItem* pItem    = NULL;

		if( 0 == nColumnIndex )
		{
			pRecords = m_wndReport.GetRecords();
			pRecord  = pClickedItem->pRow->GetRecord();
			pItem    = pRecord->GetItem(0); 
			Integer nxID = (Integer)pItem->GetItemData();
			CString strID(_T(""));
			strID.Format(_T("%I64d"), nxID);

			if (pItem->IsChecked() == TRUE)
			{                
				m_setSelectionID.insert(strID);
			}
			else
			{
				std::set<CString>::iterator iter = m_setSelectionID.find(strID);
				std::set<CString>::iterator end  = m_setSelectionID.end();
				if (iter != end)
				{
					m_setSelectionID.erase(iter);
					CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
				}
			}

			DrawReport2();
		}
		else if ( 1 == nColumnIndex )
		{
			pRecords = m_wndReport.GetRecords();
			pRecord  = pClickedItem->pRow->GetRecord();
			pItem    = pRecord->GetItem(0);            
			Integer nxID = (Integer)pItem->GetItemData();
			if (m_pMapView != nullptr)
			{
				if ( m_nMapSelectType == 0 )
				{
					m_pMapView->PanToFeature(KLayerID::Node(), nxID);
				}
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}    
}

void KImChampMultiIDFind2Column::OnReport2ItemClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) pNMHDR;

		if (!pClickedItem->pRow || !pClickedItem->pColumn)
			return;

		int nRowIndex    = pClickedItem->pRow->GetIndex();
		int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

		CXTPReportRecords*	  pRecords = NULL;
		CXTPReportRecord*     pRecord  = NULL;
		CXTPReportRecordItem* pItem    = NULL;

		if( 0 == nColumnIndex )
		{
			pRecords = m_wndReport2.GetRecords();
			pRecord  = pClickedItem->pRow->GetRecord();
			pItem    = pRecord->GetItem(0);

			Integer nxID = (Integer)pItem->GetItemData();
			CString strID(_T(""));
			strID.Format(_T("%I64d"), nxID);

			if (pItem->IsChecked() == TRUE)
			{                
				m_setSelectionID.insert(strID);
			}
			else
			{
				std::set<CString>::iterator iter = m_setSelectionID.find(strID);
				std::set<CString>::iterator end  = m_setSelectionID.end();
				if (iter != end)
					m_setSelectionID.erase(iter);
			}

			CString strFilter(_T(""));
			GetDlgItemText(IDC_EDIT_SEARCH, strFilter);
			DrawReport(strFilter);
			DrawReport2();   
		}
		else if ( 1 == nColumnIndex )
		{
			pRecords = m_wndReport2.GetRecords();
			pRecord  = pClickedItem->pRow->GetRecord();
			pItem    = pRecord->GetItem(0);

			Integer nxID = (Integer)pItem->GetItemData();
			if (m_pMapView != nullptr)
			{
				if ( m_nMapSelectType == 0 )
				{
					m_pMapView->PanToFeature(KLayerID::Node(), nxID);
				}
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}    
}

void KImChampMultiIDFind2Column::DrawReport( CString a_strFilter )
{
	try
	{
		SetDlgItemText(IDC_STATIC_INFO, _T(""));
		CString strFilter = a_strFilter.MakeLower();

		m_wndReport.ResetContent(FALSE);

		std::set<CString>::iterator iter = m_setSelectionID.begin();
		std::set<CString>::iterator end  = m_setSelectionID.end();

		BOOL bSelectAll(TRUE);
		size_t  nxCount = m_vecIDCaption.size();                
		for (size_t i=0; i<nxCount; i++)
		{
			CXTPReportRecord     *pRecord = NULL;
			CXTPReportRecordItem *pItem   = NULL;

			KID2ColumnCaption& oIDCaption = m_vecIDCaption[i];

			if (!strFilter.IsEmpty())
			{
				if (oIDCaption.TID.Find(strFilter) < 0)
				{
					if (oIDCaption.TCaption.Find(strFilter) < 0)
					{
						continue;
					}
				}
			}            

			KID2ColumnCaption& oDisplayIDCaption = m_vecDisplayIDCaption[i];
			pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			iter = m_setSelectionID.find(oIDCaption.TID);
			if (iter != end)
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(FALSE);
				bSelectAll = FALSE;
			}

			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)_ttoi64(oIDCaption.TID));

			CString strTemp;
			strTemp.Format(_T("%s"), oDisplayIDCaption.TID);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);	

			strTemp.Format(_T("%s"), oDisplayIDCaption.TCaption);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);	

			double dValue = _ttof(oDisplayIDCaption.TCaption2);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
			pItem->SetAlignment(DT_CENTER);
		}
		m_wndReport.Populate();

		if (TRUE == bSelectAll)
		{
			CheckDlgButton(IDC_CHECK1, BST_CHECKED);
		}
		else
		{
			CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
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
}

void KImChampMultiIDFind2Column::DrawReport2( )
{
	try
	{
		m_wndReport2.ResetContent(FALSE);

		std::set<CString>::iterator iter = m_setSelectionID.begin();
		std::set<CString>::iterator end  = m_setSelectionID.end();

		size_t  nxCount = m_vecIDCaption.size();
		for (size_t i=0; i<nxCount; i++)
		{
			CXTPReportRecord     *pRecord = NULL;
			CXTPReportRecordItem *pItem   = NULL;

			KID2ColumnCaption& oIDCaption = m_vecIDCaption[i];

			iter = m_setSelectionID.find(oIDCaption.TID);
			if (iter == end)
				continue;

			KID2ColumnCaption& oDisplayIDCaption = m_vecDisplayIDCaption[i];
			pRecord = m_wndReport2.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked (TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)_ttoi64(oIDCaption.TID));

			CString strTemp;
			strTemp.Format(_T("%s"), oDisplayIDCaption.TID);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);	

			strTemp.Format(_T("%s"), oDisplayIDCaption.TCaption);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);	
		}

		m_wndReport2.Populate();
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

void KImChampMultiIDFind2Column::OnBnClickedButtonFind()
{
	try
	{
		CString strFilter(_T(""));
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

void KImChampMultiIDFind2Column::OnEnChangeEditSearch()
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

BOOL KImChampMultiIDFind2Column::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_EDIT_SEARCH)->m_hWnd)
			{
				OnBnClickedButtonFind();
				return TRUE;
			}            
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}

void KImChampMultiIDFind2Column::SetIDCaption( std::vector<KID2ColumnCaption> a_vecIDCaption )
{
    try
    {
        m_vecIDCaption.clear();
        m_vecDisplayIDCaption.clear();        

        size_t nxCount = a_vecIDCaption.size();
        for (size_t i=0; i<nxCount; i++)
        {
            KID2ColumnCaption& oIDCaption = a_vecIDCaption[i];
            KID2ColumnCaption  oNewIDCaption;

            oNewIDCaption.TID       = oIDCaption.TID.MakeLower();
            oNewIDCaption.TCaption  = oIDCaption.TCaption.MakeLower();

            m_vecIDCaption.push_back(oNewIDCaption);
        }

        m_vecDisplayIDCaption = a_vecIDCaption;
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

void KImChampMultiIDFind2Column::OnBnClickedOk()
{
	try
	{
		if (m_nNecessaryCount > (int)m_setSelectionID.size())
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("최소 %d개 이상의 항목을 선택해 주세요."), m_nNecessaryCount);
			AfxMessageBox(strMsg);
			return;
		}

		CString strFilter(_T(""));
		GetDlgItemText(IDC_EDIT_SEARCH, strFilter);
		m_strPreSearch = strFilter;    

		if (m_nDrawnMap == true && m_pMapView != nullptr)
		{
			m_pMapView->ClearScreenDrawEdit();
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

	KResizeDialogEx::OnOK();
}

void KImChampMultiIDFind2Column::OnBnClickedCancel()
{
	if (m_nDrawnMap == true && m_pMapView != nullptr)
	{
		m_pMapView->ClearScreenDrawEdit();
	}

	KResizeDialogEx::OnCancel();
}

CString KImChampMultiIDFind2Column::GetSelectedID()
{
	CString strReturn(_T(""));

	try
	{
		std::set<CString>::iterator iter = m_setSelectionID.begin();
		std::set<CString>::iterator end  = m_setSelectionID.end();

		bool bFirst(true);
		while(iter != end)
		{
			if (bFirst)
			{
				strReturn.AppendFormat(_T("%s"), *iter);
				bFirst = false;
			}
			else
			{
				strReturn.AppendFormat(_T(",%s"), *iter);
			}
			++iter;
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

	return strReturn;
}

std::set<CString> KImChampMultiIDFind2Column::GetSelectedIDSet()
{
	return m_setSelectionID;
}

void KImChampMultiIDFind2Column::SetSelectedID( CString a_strSelectID )
{
	try
	{
		CString strSelectID = a_strSelectID.Trim();
		if (strSelectID.GetLength() == 0)
			return;

		std::vector<CString> vecSelectID = KParseString::ParseString(strSelectID, _T(','));

		size_t nxCount = vecSelectID.size();
		for (size_t i=0; i<nxCount; i++)
		{
			m_setSelectionID.insert(vecSelectID[i]);
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
}

void KImChampMultiIDFind2Column::MapSelectionFeedback( Integer a_nxID)
{
	try
	{
		CString strID(_T(""));
		strID.Format(_T("%I64d"), a_nxID);
		m_setSelectionID.insert(strID);
		DrawReport(_T(""));
		DrawReport2();   
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

void KImChampMultiIDFind2Column::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
	try
	{
		size_t nxCount = a_vecSelection.size();
		for (size_t i=0; i<nxCount; i++)
		{
			CString strID(_T(""));
			strID.Format(_T("%I64d"), a_vecSelection[i]);
			m_setSelectionID.insert(strID);
		}	
		DrawReport(_T(""));
		DrawReport2();  
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

void KImChampMultiIDFind2Column::MapSelectionNoneFeedback()
{

}

void KImChampMultiIDFind2Column::OnBnClickedMap()
{
	try
	{
		if (m_pMapView != nullptr)
		{
			m_nDrawnMap = true;
			m_pMapView->MapNodeSelectAction(this, m_vecNodeSelectionInfo);
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
}

void KImChampMultiIDFind2Column::SetMapNodeSelectInfo( KMapView* a_pMapView, std::vector<Integer> a_vecID, TPreferenceSymbol a_SymbolStyle )
{
	try
	{
		m_pMapView       = a_pMapView;
		m_nMapSelectType = 0;

		size_t nxCount = a_vecID.size();
		for (size_t i=0; i<nxCount; i++)
		{
			TNodeSelectionInfo oNodeSelectInfo;
			oNodeSelectInfo.TNodeID = a_vecID[i];
			oNodeSelectInfo.TCharacterIndex = a_SymbolStyle.nCharIndex;
			oNodeSelectInfo.TSize           = a_SymbolStyle.dSize;
			oNodeSelectInfo.TColorRGB       = a_SymbolStyle.clrSymbol;

			m_vecNodeSelectionInfo.push_back(oNodeSelectInfo);
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
}


void KImChampMultiIDFind2Column::SetMapNodeSelectInfoA( KMapView* a_pMapView, const std::map<Integer, CString> &a_mapID, TPreferenceSymbol a_SymbolStyle )
{
	try
	{
		m_pMapView       = a_pMapView;
		m_nMapSelectType = 0;

		AutoType iter  = a_mapID.begin();
		AutoType itEnd = a_mapID.end();

		while(iter != itEnd)
		{
			TNodeSelectionInfo oNodeSelectInfo;
			oNodeSelectInfo.TNodeID = iter->first;
			oNodeSelectInfo.TCharacterIndex = a_SymbolStyle.nCharIndex;
			oNodeSelectInfo.TSize           = a_SymbolStyle.dSize;
			oNodeSelectInfo.TColorRGB       = a_SymbolStyle.clrSymbol;

			m_vecNodeSelectionInfo.push_back(oNodeSelectInfo);

			++iter;
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
}

void KImChampMultiIDFind2Column::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
			bSelectAll = TRUE;
		}
		else
		{
			bSelectAll = FALSE;
			m_setSelectionID.clear();
		}

		CXTPReportRecords*	  pRecords = NULL;
		CXTPReportRecord*     pRecord  = NULL;
		CXTPReportRecordItem* pItem    = NULL;

		pRecords         = m_wndReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(0); 
			pItem->SetChecked(bSelectAll);

			Integer nxID = (Integer)pItem->GetItemData();
			CString strID(_T(""));
			strID.Format(_T("%I64d"), nxID);

			if (TRUE == bSelectAll)
			{                
				m_setSelectionID.insert(strID);
			}
		}

		m_wndReport.RedrawControl();

		DrawReport2();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KImChampMultiIDFind2Column::SetTitleName( CString a_strTitleName )
{
	m_strTiltleName = a_strTitleName;
}

void KImChampMultiIDFind2Column::SetSecondColumnname( CString a_strColumnName )
{
	m_strSecondColumnName = a_strColumnName;
}


void KImChampMultiIDFind2Column::SetReportTitle( CString a_strTitleName )
{
	m_strReportTitle = a_strTitleName;
}