// ImTasIDFind2Column.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImChampIDFind2Column.h"
#include "afxdialogex.h"
#include "MapView.h"


// KImChampIDFind2Column 대화 상자입니다.

const int	  nZoneSize				= 15;
const int     nStationSize			= 15;
const int	  nZoneCharacterIndex	= 66;
const int	  nStationCharIndex		= 74;
const COLORREF clrZONEDEFAULT			= RGB(76, 76, 76);

IMPLEMENT_DYNAMIC(KImChampIDFind2Column, KResizeDialogEx)

KImChampIDFind2Column::KImChampIDFind2Column(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KImChampIDFind2Column::IDD, pParent)
	, m_nxSelectedID(-1)
	, m_pMapView(NULL)
	, m_nMapSelectType(-1)
	, m_nDrawnMap(false)
	, m_pParentWnd(pParent)
	, m_strTitleName(_T(""))
{

}

KImChampIDFind2Column::~KImChampIDFind2Column()
{
	if (m_nDrawnMap == true && m_pMapView != nullptr)
	{
		m_pMapView->ClearScreenDrawEdit();
	}
}

void KImChampIDFind2Column::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KImChampIDFind2Column, KResizeDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KImChampIDFind2Column::OnReportRowDblClick)
	ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KImChampIDFind2Column::OnReportItemClick)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &KImChampIDFind2Column::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_MAP, &KImChampIDFind2Column::OnBnClickedButtonMap)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &KImChampIDFind2Column::OnEnChangeEditSearch)
	ON_BN_CLICKED(IDOK, &KImChampIDFind2Column::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KImChampIDFind2Column::OnBnClickedCancel)
END_MESSAGE_MAP()


// KImChampIDFind2Column 메시지 처리기입니다.


BOOL KImChampIDFind2Column::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if (m_strTitleName != _T(""))
	{
		SetWindowText(m_strTitleName);
	}

	if (nullptr != m_pParentWnd) // Domodal로 오픈할 경우 m_pParentWnd == null
	{
		CRect rectParent;
		m_pParentWnd->GetWindowRect(&rectParent);

		CRect rect;
		GetWindowRect(rect);
		MoveWindow(rectParent.left + rectParent.Width(), rectParent.top + 0, rect.Width(), rect.Height());
	}

	if (!m_imageListSelectIcon.Create( 16, 16, ILC_MASK | ILC_COLOR24, 0, 1 ))
	{
		AfxMessageBox(_T("Failed to create image list"));
		return -1;
	}

	CBitmap bmp;
	bmp.LoadBitmap( IDB_BITMAP_RADIO );
	m_imageListSelectIcon.Add( &bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0) ) );

	m_wndReport.SetImageList(&m_imageListSelectIcon);

	InitialControl();

	SetDlgItemText(IDC_EDIT_SEARCH, m_strPreSearch);

	SetNativeLanguageMode(IDC_EDIT_SEARCH);
	AddImChampTooltip(IDC_EDIT_SEARCH, _T("검색어를 입력해주세요"));
	AddImChampTooltip(IDC_REPORT,      _T("선택해 주세요"));
	DrawReport(m_strPreSearch);  


	if (m_nxSelectedID > 0)
	{
		DrawSelectNode(m_nxSelectedID);
	}

	GetDlgItem(IDC_EDIT_SEARCH)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KImChampIDFind2Column::InitialControl()
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
			pColumn->SetCaption(_T("이름"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(3, m_strSecondColumnName,   65));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);

		m_wndReport.Populate();

		SetResize(IDC_EDIT_SEARCH,    SZ_TOP_LEFT,     SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON_FIND,    SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON_MAP,     SZ_TOP_RIGHT,    SZ_TOP_RIGHT);

		SetResize(IDC_STATIC_HBar1,   SZ_TOP_LEFT,     SZ_TOP_RIGHT);
		SetResize(IDC_REPORT,         SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);        
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

Integer KImChampIDFind2Column::GetSelectedID()
{
	return m_nxSelectedID;
}

void KImChampIDFind2Column::SetSelectedID( Integer a_nxSelectID )
{
	try
	{
		if (-1 == a_nxSelectID)
		{
			m_strPreSearch = _T("");
		}
		else
		{
			m_strPreSearch.Format(_T("%I64d"), a_nxSelectID);
		}

		m_nxSelectedID = a_nxSelectID;
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

void KImChampIDFind2Column::SetTitleName( CString a_strTitleName )
{
	try
	{
		m_strTitleName = a_strTitleName;
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

void KImChampIDFind2Column::SetIDCaption( std::vector<KID2ColumnCaption> a_vecIDCaption )
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
			oNewIDCaption.TCaption2	= oIDCaption.TCaption2.MakeLower();

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

void KImChampIDFind2Column::DrawReport( CString a_strFilter/*=_T("")*/ )
{
	try
	{
		//m_nxSelectedID = -1;
		SetDlgItemText(IDC_STATIC_INFO, _T(""));
		CString strFilter = a_strFilter.MakeLower();

		m_wndReport.ResetContent(FALSE);
		size_t  nxCount = m_vecIDCaption.size();

		for (size_t i=0; i<nxCount; i++)
		{
			CXTPReportRecord     *pRecord = NULL;
			CXTPReportRecordItem *pItem   = NULL;
			KID2ColumnCaption& oIDCaption = m_vecIDCaption[i];
			KID2ColumnCaption& oDisplayIDCaption = m_vecDisplayIDCaption[i];

			Integer nxID = _ttoi64(oIDCaption.TID);
			if (m_nxSelectedID != nxID)
				continue;

			pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)_ttoi64(oIDCaption.TID));

			pItem->SetIconIndex(1);

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

			Integer nxID = _ttoi64(oIDCaption.TID);
			if (m_nxSelectedID == nxID)
				continue;

			KID2ColumnCaption& oDisplayIDCaption = m_vecDisplayIDCaption[i];

			pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)_ttoi64(oIDCaption.TID));

			pItem->SetIconIndex(0);

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


void KImChampIDFind2Column::OnBnClickedButtonFind()
{
	try
	{
		CString strFilter(_T(""));
		GetDlgItemText(IDC_EDIT_SEARCH, strFilter);
		DrawReport(strFilter);
		Integer nxNodeID = _ttoi64(strFilter);
		DrawSelectNode(nxNodeID);
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


void KImChampIDFind2Column::OnBnClickedButtonMap()
{
	try
	{
		if (m_pMapView != nullptr)
		{
			m_nDrawnMap = true;
			m_pMapView->MapNodeSelectAction(this, m_vecNodeSelectionInfo, false, false);
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


void KImChampIDFind2Column::OnEnChangeEditSearch()
{
	try
	{
		//OnBnClickedButtonFind();

// 		if (m_nDrawnMap == true && m_pMapView != nullptr)
// 		{
// 			m_pMapView->ClearScreenDrawEdit();
// 		}
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


BOOL KImChampIDFind2Column::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_EDIT_SEARCH)->m_hWnd)
			{
				//OnBnClickedButtonFind();

// 				if (m_nDrawnMap == true && m_pMapView != nullptr)
// 				{
// 					m_pMapView->ClearScreenDrawEdit();
// 				}
				CString strFilter(_T(""));
				GetDlgItemText(IDC_EDIT_SEARCH, strFilter);
				DrawReport(strFilter);
				return TRUE;
			}            
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KImChampIDFind2Column::OnBnClickedOk()
{
	try
	{
		CString strFilter(_T(""));
		GetDlgItemText(IDC_EDIT_SEARCH, strFilter);
		m_strPreSearch = strFilter;

		//검색여부 체크 및 정보 저장
		if (m_nxSelectedID <= 0)
		{
			AfxMessageBox(_T("선택한 항목이 없습니다."));
			return;
		}

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


void KImChampIDFind2Column::OnBnClickedCancel()
{
	if (m_nDrawnMap == true && m_pMapView != nullptr)
	{
		m_pMapView->ClearScreenDrawEdit();
	}

	KResizeDialogEx::OnCancel();
}


bool KImChampIDFind2Column::SelectionCheck()
{
	bool bSelection(false);

	try
	{
		CXTPReportRecords*	  pRecords    = NULL;
		CXTPReportRecord*     pRecord     = NULL;
		CXTPReportRecordItem* pItemCheck  = NULL;

		pRecords = m_wndReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i=0; i<nRecordCount; i++)
		{
			pRecord    = pRecords->GetAt(i);
			pItemCheck = pRecord->GetItem(0);

			if (pItemCheck->IsChecked() == TRUE)
			{
				bSelection     = true;
				m_nxSelectedID = (Integer)pItemCheck->GetItemData();
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

	return bSelection;
}

void KImChampIDFind2Column::SetMapNodeSelectInfo( KMapView* a_pMapView, std::vector<Integer> a_vecID, TPreferenceSymbol a_SymbolStyle )
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
			CString strLable(_T(""));
			strLable.Format(_T("%I64d"),a_vecID[i]);
			//oNodeSelectInfo.TLabel	= strLable;
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

void KImChampIDFind2Column::SetMapNodeSelectInfoA( KMapView* a_pMapView, const std::map<Integer, CString> &a_mapID, TPreferenceSymbol a_SymbolStyle )
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

void KImChampIDFind2Column::MapSelectionFeedback( Integer a_nxID )
{
	try
	{
		m_nxSelectedID = a_nxID;
		DrawReport(_T(""));

		SetSelectedID(a_nxID);
		SetDlgItemText(IDC_EDIT_SEARCH, m_strPreSearch);

		if (m_nDrawnMap == true && m_pMapView != nullptr)
		{
			m_pMapView->ClearScreenDrawEdit();
		}

		Integer nxNodeID = _ttoi64(m_strPreSearch);
		DrawSelectNode(nxNodeID);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KImChampIDFind2Column::MapSelectionFeedback(std::vector<Integer> a_vecSelection )
{
	try
	{
		size_t nxCount = a_vecSelection.size();
		for (size_t i=0; i<nxCount; i++)
		{
			m_nxSelectedID = a_vecSelection[i];
		}	
		DrawReport(_T(""));
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

void KImChampIDFind2Column::MapSelectionNoneFeedback()
{

}


void KImChampIDFind2Column::OnReportRowDblClick( NMHDR* pNMHDR, LRESULT* pResult )
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

void KImChampIDFind2Column::OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult )
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
		pRecords = m_wndReport.GetRecords();

		if( 0 == nColumnIndex )
		{            
			int nRecordCount = pRecords->GetCount();

			for (int i=0; i<nRecordCount; i++)
			{
				pRecord    = pRecords->GetAt(i);
				pItem      = pRecord->GetItem(0);
				pItem->SetIconIndex(0);
			}

			pRecord = pClickedItem->pRow->GetRecord();
			pItem   = pRecord->GetItem(0);
			pItem->SetIconIndex(1);

			m_nxSelectedID = (Integer)pItem->GetItemData();
			DrawSelectNode(m_nxSelectedID);
			CString strSelection;
			strSelection.Format(_T("%I64d"), m_nxSelectedID);
			SetDlgItemText(IDC_STATIC_INFO, strSelection);
		}
		else if ( 1 == nColumnIndex )
		{
			pRecord = pRecords->GetAt(nRowIndex);
			pItem   = pRecord->GetItem(0);            
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

void KImChampIDFind2Column::SetSecondColumnname( CString a_strColumnName )
{
	m_strSecondColumnName = a_strColumnName;
}

void KImChampIDFind2Column::DrawSelectNode(Integer a_nxNodeID)
{
	try
	{
		if (m_nDrawnMap == true && m_pMapView != nullptr)
		{
			m_pMapView->ClearScreenDrawEdit();
		}

		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		TDrawNodeInfo oZoneNodeInfo;
		oZoneNodeInfo.TNodeID			= a_nxNodeID;
		oZoneNodeInfo.TSymbol	= nZoneCharacterIndex;
		oZoneNodeInfo.TSize				= nZoneSize;
		oZoneNodeInfo.TColorRGB			= clrZONEDEFAULT;
		CString strLable(_T(""));
		strLable.Format(_T("%I64d"), m_nxSelectedID);
		oZoneNodeInfo.TLabel			= strLable;
		vecDrawNodeInfo.push_back(oZoneNodeInfo);
		m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, true,  false);

		m_pMapView->PanToFeature(KLayerID::Node(), a_nxNodeID, true);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	
}
