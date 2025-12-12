// ImTasIDFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImChampIDFind.h"
#include "afxdialogex.h"
#include "MapView.h"
// KImChampIDFind 대화 상자입니다.

IMPLEMENT_DYNAMIC(KImChampIDFind, KResizeDialogEx)

CString KImChampIDFind::m_strPreSearch = _T("");

KImChampIDFind::KImChampIDFind(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KImChampIDFind::IDD, pParent), m_nxSelectedID(-1), m_pMapView(NULL), m_nMapSelectType(-1), m_nDrawnMap(false), m_pParentWnd(pParent), m_strTitleName(_T(""))
{

}

KImChampIDFind::~KImChampIDFind()
{
	if (m_nDrawnMap == true && m_pMapView != nullptr)
	{
		m_pMapView->ClearScreenDrawEdit();
	}
}

void KImChampIDFind::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KImChampIDFind, KResizeDialogEx)
    ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KImChampIDFind::OnReportRowDblClick)
    ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KImChampIDFind::OnReportItemClick)
    ON_BN_CLICKED(IDC_BUTTON_FIND, &KImChampIDFind::OnBnClickedButtonFind)
    ON_EN_CHANGE(IDC_EDIT_SEARCH, &KImChampIDFind::OnEnChangeEditSearch)
    ON_BN_CLICKED(IDOK, &KImChampIDFind::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KImChampIDFind::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_MAP, &KImChampIDFind::OnBnClickedButtonMap)
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// KImChampIDFind 메시지 처리기입니다.
BOOL KImChampIDFind::OnInitDialog()
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
        
    SetDlgItemText(IDC_EDIT_SEARCH, m_strPreSearch);    
    SetNativeLanguageMode(IDC_EDIT_SEARCH);
    AddImChampTooltip(IDC_EDIT_SEARCH, _T("검색어를 입력해주세요"));
    AddImChampTooltip(IDC_REPORT,      _T("선택해 주세요"));
    DrawReport(m_strPreSearch);  

    return TRUE;  
}

void KImChampIDFind::OnShowWindow(BOOL bShow, UINT nStatus)
{
    KResizeDialogEx::OnShowWindow(bShow, nStatus);
    GetDlgItem(IDC_EDIT_SEARCH)->SetFocus();
}


void KImChampIDFind::InitialControl()
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
		//if (KmzSystem::GetLanguage() == KEMKorea) {
		//	pColumn->SetCaption(_T("ID"));
		//}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Name"),   65));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("명칭"));
		}
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

void KImChampIDFind::OnReportRowDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
    try
    {
        XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
        if (nullptr == pClickedItem)
            return;

        CXTPReportRow* pRow = pClickedItem->pRow;
        if (nullptr == pRow)
            return;

		CXTPReportRecords*	  pRecords = NULL;
		CXTPReportRecord*     pRecord  = NULL;
		CXTPReportRecordItem* pItem    = NULL;
		pRecords = m_wndReport.GetRecords();

		int nColumnIndex = pClickedItem->pColumn->GetItemIndex();
		if (nColumnIndex == 0)
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

			CString strSelection;
			strSelection.Format(_T("%I64d"), m_nxSelectedID);
			SetDlgItemText(IDC_STATIC_INFO, strSelection);

			if (m_nDrawnMap == true && m_pMapView != nullptr)
			{
				m_pMapView->ClearScreenDrawEdit();
			}

			KResizeDialogEx::OnOK();
		}
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KImChampIDFind::OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult )
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
                    m_pMapView->PanToFeature(KLayerID::Node(), nxID, false);
                }
            }
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }    
}

void KImChampIDFind::DrawReport( CString a_strFilter )
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
            KIDCaption& oIDCaption = m_vecIDCaption[i];
            KIDCaption& oDisplayIDCaption = m_vecDisplayIDCaption[i];

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
        }

        for (size_t i=0; i<nxCount; i++)
        {
            CXTPReportRecord     *pRecord = NULL;
            CXTPReportRecordItem *pItem   = NULL;

            KIDCaption& oIDCaption = m_vecIDCaption[i];
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
            
            KIDCaption& oDisplayIDCaption = m_vecDisplayIDCaption[i];

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

void KImChampIDFind::OnBnClickedButtonFind()
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

void KImChampIDFind::OnBnClickedButtonMap()
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


void KImChampIDFind::OnEnChangeEditSearch()
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

BOOL KImChampIDFind::PreTranslateMessage(MSG* pMsg)
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

void KImChampIDFind::SetIDCaption( std::vector<KIDCaption> a_vecIDCaption )
{
    try
    {
        m_vecIDCaption.clear();
        m_vecDisplayIDCaption.clear();

        size_t nxCount = a_vecIDCaption.size();
        for (size_t i=0; i<nxCount; i++)
        {
            KIDCaption& oIDCaption = a_vecIDCaption[i];
            KIDCaption  oNewIDCaption;

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

void KImChampIDFind::OnBnClickedOk()
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

void KImChampIDFind::OnBnClickedCancel()
{
    if (m_nDrawnMap == true && m_pMapView != nullptr)
    {
        m_pMapView->ClearScreenDrawEdit();
    }

    KResizeDialogEx::OnCancel();
}

bool KImChampIDFind::SelectionCheck()
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

Integer KImChampIDFind::GetSelectedID()
{
    return m_nxSelectedID;
}

void KImChampIDFind::SetSelectedID( Integer a_nxSelectID )
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

void KImChampIDFind::SetMapNodeSelectInfo( KMapView* a_pMapView, std::vector<Integer> a_vecID, TPreferenceSymbol a_SymbolStyle )
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

void KImChampIDFind::SetMapNodeSelectInfoA( KMapView* a_pMapView, const std::map<Integer, CString> &a_mapID, TPreferenceSymbol a_SymbolStyle)
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

void KImChampIDFind::MapSelectionFeedback( Integer a_nxID)
{
    m_nxSelectedID = a_nxID;
    DrawReport(_T(""));

 	SetSelectedID(a_nxID);
 	SetDlgItemText(IDC_EDIT_SEARCH, m_strPreSearch);
}

void KImChampIDFind::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
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

void KImChampIDFind::MapSelectionNoneFeedback()
{
}

void KImChampIDFind::SetTitleName( CString a_strTitleName )
{
	m_strTitleName = a_strTitleName;
}

