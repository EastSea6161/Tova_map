

#include "stdafx.h"
#include "ImChampMultiZoneFind.h"
#include "afxdialogex.h"

//^^ #include "IOColumn.h"
#include "Target.h"
//^#include "DBaseConnector.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"


const int ZFIND_ZONEID		= 1;
const int ZFIND_ZONENAME	= 2;
const int ZFIND_ZONESUB		= 3;
const int ZFIND_ZONEMAIN	= 4;

IMPLEMENT_DYNAMIC(KImChampMultiZoneFind, KResizeDialogEx)

KImChampMultiZoneFind::KImChampMultiZoneFind(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KImChampMultiZoneFind::IDD, pParent),
	m_pTarget(a_pTarget),
	m_strPreSearch(_T("")),
	m_strTitleName(_T("")),
	m_bUseTransitInclude(false),
	m_bChekTransitInclude(false)

{
	
}

KImChampMultiZoneFind::~KImChampMultiZoneFind()
{
}

void KImChampMultiZoneFind::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndReportUp);
	DDX_Control(pDX, IDC_REPORT2, m_wndReportDown);
	DDX_Control(pDX, IDC_COMBO1, m_cboFindColumn);
}


BEGIN_MESSAGE_MAP(KImChampMultiZoneFind, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &KImChampMultiZoneFind::OnBnClickedButtonFind)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &KImChampMultiZoneFind::OnEnChangeEditSearch)
	ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KImChampMultiZoneFind::OnReportItemClickUp)
	ON_NOTIFY(NM_CLICK,  IDC_REPORT2, &KImChampMultiZoneFind::OnReportItemClickDown)
	ON_BN_CLICKED(IDC_CHECK1, &KImChampMultiZoneFind::OnBnClickedCheckSelectAll)
	ON_BN_CLICKED(IDOK, &KImChampMultiZoneFind::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KImChampMultiZoneFind::OnBnClickedCancel)
END_MESSAGE_MAP()



BOOL KImChampMultiZoneFind::OnInitDialog()
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

	GetFindZoneColumn();
	GetZoneInfo();

	InitComboBox();
	//GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);

	if (m_bUseTransitInclude == false)
	{
		GetDlgItem(IDC_CHECK2)->ShowWindow(FALSE);
	}

	InitialReportControl();

	SetDlgItemText(IDC_EDIT_SEARCH, m_strPreSearch);
	CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);

	SetNativeLanguageMode(IDC_EDIT_SEARCH);
	AddImChampTooltip(IDC_EDIT_SEARCH, _T("검색어를 입력해주세요"));
	AddImChampTooltip(IDC_REPORT,      _T("선택해 주세요"));
	UpdateReportControlUp(m_strPreSearch);
	UpdateReportControlDown();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KImChampMultiZoneFind::InitialReportControl()
{
	try
	{
		KReportCtrlSetting::Default(m_wndReportUp, FALSE, FALSE, FALSE);
		KReportCtrlSetting::AllowColumnsort(m_wndReportUp);

		CXTPReportColumn* pColumn = NULL;
		pColumn = m_wndReportUp.AddColumn(new CXTPReportColumn(0, _T("Select"), 40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("선택"));
		}

		AutoType iter = m_mapZoneColumn.begin();
		AutoType end  = m_mapZoneColumn.end();
		int nIndex = 1;
		while (iter != end)
		{
			KIOColumn* pZoneColumn = iter->second;
			pColumn = m_wndReportUp.AddColumn(new CXTPReportColumn(nIndex, pZoneColumn->Caption(),  40));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetSortable(TRUE);
			nIndex++;
			++iter;
		}
		m_wndReportUp.Populate();

		KReportCtrlSetting::Default(m_wndReportDown, FALSE, FALSE, FALSE);
		KReportCtrlSetting::AllowColumnsort(m_wndReportDown);
        		
		pColumn = m_wndReportDown.AddColumn(new CXTPReportColumn(0, _T("Select"), 40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("선택"));
		}

		nIndex = 1;
		iter = m_mapZoneColumn.begin();
		while (iter != end)
		{
			KIOColumn* pZoneColumn = iter->second;
			pColumn = m_wndReportDown.AddColumn(new CXTPReportColumn(nIndex, pZoneColumn->Caption(),  40));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetSortable(TRUE);
			nIndex++;
			++iter;
		}
		
		m_wndReportDown.Populate();

		SetResize(IDC_EDIT_SEARCH,    SZ_TOP_LEFT,     SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON_FIND,    SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
		//SetResize(IDC_BUTTON_MAP,    SZ_TOP_RIGHT,    SZ_TOP_RIGHT);

		SetResize(IDC_STATIC_HBar1,   SZ_TOP_LEFT,     SZ_TOP_RIGHT);
		SetResize(IDC_CHECK1, SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_REPORT,         SZ_TOP_LEFT,     CXTPResizePoint(1.0, 0.5));

		SetResize(IDC_STATIC_SELECTED, CXTPResizePoint(0.0, 0.5), CXTPResizePoint(1.0, 0.5));       
		SetResize(IDC_REPORT2,         CXTPResizePoint(0.0, 0.5), CXTPResizePoint(1.0, 1.0));
		SetResize(IDC_STATIC_HBar2,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC_INFO,    SZ_MIDDLE_LEFT,  SZ_MIDDLE_RIGHT);    
		SetResize(IDC_CHECK2,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);    
		SetResize(IDOK,               SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,           SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

		SetDlgItemText(IDC_STATIC_INFO, _T(""));
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KImChampMultiZoneFind::GetFindZoneColumn( void )
{
	m_mapZoneColumn.clear();

	KIOTable* pZoneTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
	const KIOColumns* pColumns = pZoneTable->Columns();

	KIOColumn* pColumn =  pColumns->GetColumn(_T("zone_id"));
	if (pColumn != nullptr)
	{
		m_mapZoneColumn.insert(std::make_pair(ZFIND_ZONEID, pColumn));
	}

	pColumn = pColumns->GetColumn(_T("name"));
	if (pColumn != nullptr)
	{
		m_mapZoneColumn.insert(std::make_pair(ZFIND_ZONENAME, pColumn));
	}

	pColumn = pColumns->GetColumn(_T("sub_category"));
	if (pColumn != nullptr)
	{
		m_mapZoneColumn.insert(std::make_pair(ZFIND_ZONESUB, pColumn));
	}

	pColumn = pColumns->GetColumn(_T("main_category"));
	if (pColumn != nullptr)
	{
		m_mapZoneColumn.insert(std::make_pair(ZFIND_ZONEMAIN, pColumn));
	}
}

void KImChampMultiZoneFind::InitComboBox()
{
	try
	{
		m_cboFindColumn.Clear();

		int nCur(0);
		AutoType iter = m_mapZoneColumn.begin();
		AutoType end  = m_mapZoneColumn.end();
		while(iter != end)
		{
			KIOColumn* pColumn = iter->second;
			nCur = m_cboFindColumn.AddString(pColumn->Caption());
			m_cboFindColumn.SetItemData(nCur, iter->first);
			++iter;
		}
		m_cboFindColumn.SetCurSel(nCur);

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

void KImChampMultiZoneFind::GetZoneInfo( void )
{
	CString strSQL(_T(""));
	try
	{
		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pMainCodeGroup = pCodeMgr->FindCodeGroup(ZONE_MAINCODE_GROUP_KEY);
		KCodeGroup*   pSubCodeGroup = pCodeMgr->FindCodeGroup(ZONE_SUBCODE_GROUP_KEY);

		m_mapZoneInfo.clear();

		KDBaseConPtr spDBConnection = m_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select zone_id, name, main_category, sub_category From %s Order by zone_id"), TABLE_ZONE);
		
		KResultSetPtr spResultset = spDBConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			TIDZoneCaption oZoneInfo ={};
			oZoneInfo.TnxZoneID		= spResultset->GetValueInt64(0);
			oZoneInfo.TstrZoneName	= spResultset->GetValueString(1);

			int nMainCode			= spResultset->GetValueInt(2);
			int nSubCode			= spResultset->GetValueInt(3);
			CString strMainCodeName = pMainCodeGroup->SingleCodeValue(nMainCode);
			CString strSubCodeName  = pSubCodeGroup->SingleCodeValue(nSubCode);

			CString strMain(_T(""));
			CString strSub(_T(""));
			strMain.Format(_T("%d[%s]"), nMainCode, strMainCodeName);
			strSub.Format(_T("%d[%s]"),nSubCode, strSubCodeName);

			oZoneInfo.TstrMainCode = strMain;
			oZoneInfo.TstrSubCode  = strSub;

			m_mapZoneInfo.insert(std::make_pair(oZoneInfo.TnxZoneID, oZoneInfo));
		}

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

void KImChampMultiZoneFind::UpdateReportControlUp(CString a_strFilter)
{
	try
	{
		int nCurSel = m_cboFindColumn.GetCurSel();
		int nIndex = m_cboFindColumn.GetItemData(nCurSel);
		CString strFilter = a_strFilter.MakeLower();

		m_wndReportUp.ResetContent(FALSE);
		BOOL bSelectAll(TRUE);

		AutoType iter = m_mapZoneInfo.begin();
		AutoType end  = m_mapZoneInfo.end();
		while(iter != end)
		{
			CXTPReportRecord     *pRecord = NULL;
			CXTPReportRecordItem *pItem   = NULL;

			TIDZoneCaption oInfo = iter->second;

			if (!strFilter.IsEmpty())
			{
				if (nIndex == ZFIND_ZONEID)
				{
					CString strZoneID(_T(""));
					strZoneID.Format(_T("%I64d"), oInfo.TnxZoneID);
					CString strBase = strZoneID.MakeLower();

					if (!strFilter.IsEmpty())
					{
						if (strBase.Find(strFilter) < 0)
						{
							++iter;
							continue;
						}
					}       
				}
				else if (nIndex == ZFIND_ZONENAME)
				{
					CString strName = oInfo.TstrZoneName;
					CString strBase = strName.MakeLower();

					if (!strFilter.IsEmpty())
					{
						if (strBase.Find(strFilter) < 0)
						{
							++iter;
							continue;
						}
					}       
				}
				else if (nIndex == ZFIND_ZONEMAIN)
				{
					CString strBase = oInfo.TstrMainCode.MakeLower();

					if (!strFilter.IsEmpty())
					{
						if (strBase.Find(strFilter) < 0)
						{
							++iter;
							continue;
						}
					}       
				}
				else if (nIndex == ZFIND_ZONESUB)
				{
					
					CString strBase = oInfo.TstrSubCode.MakeLower();

					if (!strFilter.IsEmpty())
					{
						if (strBase.Find(strFilter) < 0)
						{
							++iter;
							continue;
						}
					}            

				}
			}

			pRecord = m_wndReportUp.AddRecord(new CXTPReportRecord());
			pItem	= pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)oInfo.TnxZoneID);

			AutoType findSel = m_setSelectedZone.find(oInfo.TnxZoneID);
			AutoType endSel  = m_setSelectedZone.end();
			if (findSel != endSel)
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(FALSE);
				bSelectAll = FALSE;
			}

			CString strTemp(_T(""));
			strTemp.Format(_T("%I64d"), oInfo.TnxZoneID);
			pItem = pRecord->AddItem(new  CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);

			strTemp = _T("");
			strTemp.Format(_T("%s"), oInfo.TstrZoneName);
			pItem = pRecord->AddItem(new  CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new  CXTPReportRecordItemText(oInfo.TstrSubCode));
			pItem->SetAlignment(DT_CENTER);

            pItem = pRecord->AddItem(new  CXTPReportRecordItemText(oInfo.TstrMainCode));
            pItem->SetAlignment(DT_CENTER);

			++iter;
		}
		m_wndReportUp.Populate();

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
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KImChampMultiZoneFind::UpdateReportControlDown()
{
	try
	{
		m_wndReportDown.ResetContent(FALSE);
		BOOL bSelectAll(TRUE);

		AutoType iter = m_mapZoneInfo.begin();
		AutoType end  = m_mapZoneInfo.end();
		while(iter != end)
		{
			CXTPReportRecord     *pRecord = NULL;
			CXTPReportRecordItem *pItem   = NULL;

			TIDZoneCaption oInfo = iter->second;

			AutoType findSel = m_setSelectedZone.find(oInfo.TnxZoneID);
			AutoType endSel  = m_setSelectedZone.end();
			if (findSel == endSel)
			{
				++iter;
				continue;
			}

			pRecord = m_wndReportDown.AddRecord(new CXTPReportRecord());
			pItem	= pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)oInfo.TnxZoneID);
			pItem->SetChecked(TRUE);

			CString strTemp(_T(""));
			strTemp.Format(_T("%I64d"), oInfo.TnxZoneID);
			pItem = pRecord->AddItem(new  CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);

			strTemp = _T("");
			strTemp.Format(_T("%s"), oInfo.TstrZoneName);
			pItem = pRecord->AddItem(new  CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new  CXTPReportRecordItemText(oInfo.TstrMainCode));
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new  CXTPReportRecordItemText(oInfo.TstrSubCode));
			pItem->SetAlignment(DT_CENTER);

			++iter;
		}
		m_wndReportDown.Populate();
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


BOOL KImChampMultiZoneFind::PreTranslateMessage(MSG* pMsg)
{
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


void KImChampMultiZoneFind::OnBnClickedButtonFind()
{
	try
	{
		CString strFilter(_T(""));
		GetDlgItemText(IDC_EDIT_SEARCH, strFilter);
		UpdateReportControlUp(strFilter);
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

void KImChampMultiZoneFind::OnReportItemClickUp( NMHDR* pNMHDR, LRESULT* pResult )
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
			pItem = pClickedItem->pItem;			
			Integer nxID = (Integer)pItem->GetItemData();

			if (pItem->IsChecked() == TRUE)
			{                
				m_setSelectedZone.insert(nxID);
			}
			else
			{
				AutoType iter = m_setSelectedZone.find(nxID);
				AutoType end  = m_setSelectedZone.end();
				if (iter != end)
				{
					m_setSelectedZone.erase(iter);
					CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
				}
			}
			UpdateReportControlDown();
		}
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

void KImChampMultiZoneFind::OnReportItemClickDown( NMHDR* pNMHDR, LRESULT* pResult )
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
		pItem = pClickedItem->pItem;
		Integer nxID = (Integer)pItem->GetItemData();

		if (pItem->IsChecked() == TRUE)
		{                
			m_setSelectedZone.insert(nxID);
		}
		else
		{
			AutoType iter = m_setSelectedZone.find(nxID);
			AutoType end  = m_setSelectedZone.end();
			if (iter != end)
				m_setSelectedZone.erase(iter);
		}

		CString strFilter(_T(""));
		GetDlgItemText(IDC_EDIT_SEARCH, strFilter);
		UpdateReportControlUp(strFilter);
		UpdateReportControlDown();  
	}
}

void KImChampMultiZoneFind::OnBnClickedCheckSelectAll()
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
			m_setSelectedZone.clear();
		}

		CXTPReportRecords*	  pRecords = NULL;
		CXTPReportRecord*     pRecord  = NULL;
		CXTPReportRecordItem* pItem    = NULL;

		pRecords         = m_wndReportUp.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(0); 
			pItem->SetChecked(bSelectAll);

			Integer nxID = (Integer)pItem->GetItemData();

			if (TRUE == bSelectAll)
			{                
				m_setSelectedZone.insert(nxID);
			}
		}

		m_wndReportUp.RedrawControl();

		UpdateReportControlDown();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KImChampMultiZoneFind::OnBnClickedOk()
{
	BOOL Bchk = IsDlgButtonChecked(IDC_CHECK2);
	if (Bchk == TRUE)
	{
		m_bChekTransitInclude= true;
	}

	  KResizeDialogEx::OnOK();
}

void KImChampMultiZoneFind::OnBnClickedCancel()
{
	 KResizeDialogEx::OnCancel();
}

void KImChampMultiZoneFind::SetSelectedID( std::set<Integer> a_setSelectedZone )
{
	m_setSelectedZone.clear();
	m_setSelectedZone = a_setSelectedZone;
}

void KImChampMultiZoneFind::GetSelectedID( std::set<Integer>& a_setSeletedZone )
{
	a_setSeletedZone = m_setSelectedZone;
}

void KImChampMultiZoneFind::SetTitleName( CString a_strTitleName )
{
	m_strTitleName = a_strTitleName;
}

void KImChampMultiZoneFind::SetUseTransitInclude( bool a_bTransitUsed )
{
	m_bUseTransitInclude = a_bTransitUsed;
}

bool KImChampMultiZoneFind::GetTransitInclude()
{
	return m_bChekTransitInclude;
}

void KImChampMultiZoneFind::OnEnChangeEditSearch()
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
