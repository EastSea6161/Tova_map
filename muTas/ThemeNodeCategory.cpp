// ThemeNodeCategory.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ThemeNodeCategory.h"
#include "afxdialogex.h"

#include "Target.h"
#include "MapView.h"
//^^ #include "TableOwner.h"
//^^ #include "TableOwner.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"

#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "KPointMarkerSelectDlg.h"
#include "ThemeStyleSave.h"
#include "PointStyleTypeEdit.h"
// KThemeNodeCategory 대화 상자입니다.

IMPLEMENT_DYNAMIC(KThemeNodeCategory, KDialogEx)

KThemeNodeCategory::KThemeNodeCategory(CWnd* pParent /*=NULL*/)
	: KDialogEx(KThemeNodeCategory::IDD, pParent)
{
	m_oQbicDefaultMarkerFont.CreatePointFont(120, _T("QBicF"));   
	
	m_strPreSelectedColumnName = _T("");
	m_nPreSelectedStyleID      = -1;
}

KThemeNodeCategory::~KThemeNodeCategory()
{
}

void KThemeNodeCategory::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO_FIELD, m_cboCategory);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_cboStyle);
}


BEGIN_MESSAGE_MAP(KThemeNodeCategory, KDialogEx)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD, &KThemeNodeCategory::OnCbnSelchangeComboField)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &KThemeNodeCategory::OnCbnSelchangeComboStyle)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, &KThemeNodeCategory::OnReportItemClick)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KThemeNodeCategory::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &KThemeNodeCategory::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_SAVE, &KThemeNodeCategory::OnBnClickedButtonStyleSave)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_EDIT, &KThemeNodeCategory::OnBnClickedButtonStyleEdit)
END_MESSAGE_MAP()


// KThemeNodeCategory 메시지 처리기입니다.
BOOL KThemeNodeCategory::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255));
	
	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);  
	m_wndReportCtrl.AllowEdit(TRUE);

	CXTPReportColumn* pColumn = NULL;
	m_wndReportCtrl.SelectionEnable(FALSE);
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Symbol"), 8));
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Size"),   10));
	pColumn->GetEditOptions()->AddSpinButton(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Value"),  10));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Count"),  10));
	pColumn->SetHeaderAlignment(DT_CENTER);

	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(TRUE);
	pHeader->AllowColumnSort(TRUE); 

	m_wndReportCtrl.Populate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HBRUSH KThemeNodeCategory::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void KThemeNodeCategory::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KThemeNodeCategory::SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView)
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;

	LoadInitialData();
}

void KThemeNodeCategory::LoadInitialData()
{
	LoadInitialCategory();
	LoadInitialStyle();	

	FieldSelectChangeAction();	
}

void KThemeNodeCategory::LoadInitialCategory()
{
	m_cboCategory.ResetContent();
	int nPreSelectedIndex = -1;

	KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTable*    pIOTable  = pIOTables->FindTable(TABLE_NODE);
	const 
    KIOColumns*  pIOColumns = pIOTable->Columns();    
	             int nColumnCount = pIOColumns->ColumnCount();

	KIOColumn*   pIOColumn  = NULL;

	for ( int i=0; i<nColumnCount; i++ )
	{
		pIOColumn = pIOColumns->GetColumn(i);
		if ( pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode )
		{
			int nIndex = m_cboCategory.AddString(pIOColumn->Caption());
			m_cboCategory.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

			if (m_strPreSelectedColumnName.CompareNoCase(pIOColumn->Name()) == 0)
			{
				nPreSelectedIndex = nIndex;
			}
		}
	}

	CButton* pButtonApply = (CButton*)GetDlgItem(IDC_BUTTON_APPLY);
	if (nColumnCount > 0)
	{
		if ( nPreSelectedIndex >= 0)
		{
			m_cboCategory.SetCurSel(nPreSelectedIndex);
		}
		else
		{
			m_cboCategory.SetCurSel(0);
		}		
		pButtonApply->EnableWindow(TRUE);
	}    
	else
	{
		pButtonApply->EnableWindow(FALSE);
	}
}

void KThemeNodeCategory::LoadInitialStyle()
{
	m_cboStyle.ResetContent();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection != NULL)
	{
		std::vector<TNodeTemplate> vecNodeTemplate;
		KProjectDBase::NodeTemplate(spDBaseConnection, 0, vecNodeTemplate);
		
		int nPreSelectedIndex = -1;
		for (size_t i=0; i<vecNodeTemplate.size(); i++)
		{
			TNodeTemplate oTemplate = vecNodeTemplate[i];
			CString strName; strName.Format(_T("%d:%s"), oTemplate.TID, oTemplate.TName);
			int nIndex = m_cboStyle.AddString(strName);
			m_cboStyle.SetItemData(nIndex, (DWORD_PTR)oTemplate.TID);

			if (oTemplate.TID == m_nPreSelectedStyleID)
			{
				nPreSelectedIndex = nIndex;
			}
		}

		if (m_cboStyle.GetCount() > 0)
		{
			if (nPreSelectedIndex >= 0)
			{
				m_cboStyle.SetCurSel(nPreSelectedIndex);
			}
			else
			{
				m_cboStyle.SetCurSel(0);
			}			
		}
	}
}

void KThemeNodeCategory::OnCbnSelchangeComboField()
{
	int nCurSelectedIndex = m_cboCategory.GetCurSel();
	KIOColumn* pIOColumn  = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedIndex);
	if (pIOColumn == NULL)
	{
		AfxMessageBox(_T("관련 컬럼 정보를 찾을 수 없습니다."));
		return;
	}

	m_strPreSelectedColumnName = pIOColumn->Name();
	FieldSelectChangeAction();
	ClearNodeThemes();
}

void KThemeNodeCategory::FieldSelectChangeAction()
{
	m_wndReportCtrl.ResetContent();

	int nCurSelectedIndex = m_cboCategory.GetCurSel();
	if (nCurSelectedIndex < 0)
		return;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);
	KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedIndex);

	bool isCodeColumn    = false;
	if ( pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode )
	{
		isCodeColumn = true;
	}

	KCodeGroup*       pCodeData       = NULL;
	if ( isCodeColumn )
	{
		KCodeManager* pCodeManager = NULL;
		pCodeManager = pIOTable->GetOwner()->GetCodeManager();
		pCodeData   = pCodeManager->FindCodeGroup(pIOColumn->CodeGroup());
	}

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strSQL;
	strSQL.Format (_T(" SELECT %s, COUNT(*) AS Data_Count FROM %s GROUP BY %s "), pIOColumn->Name(), TABLE_NODE, pIOColumn->Name());

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return;
	}

	m_wndReportCtrl.ResetContent(TRUE);

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	while( pResult->Next() )
	{
		// Symbol / Size / Value / Count
		Integer nxValue     = pResult->GetValueInt64 (0);
		Integer nxDataCount = pResult->GetValueInt64 (1);
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetEditable(FALSE);
		pItem->SetFont(&m_oQbicDefaultMarkerFont);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(8.0, _T("%.1f")));         
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(xtpColumnTextLeft);

		CString strLabel=_T("");
		if( isCodeColumn )
		{
		 	strLabel = pCodeData->SingleCodeValue((int)nxValue);
		 	pItem = pRecord->AddItem(new CXTPReportRecordItemText(strLabel));
		 	pItem->SetAlignment(xtpColumnTextLeft);
		}
		else
		{
		 	strLabel.Format(_T("%I64d"), nxValue);
		 
		 	pItem = pRecord->AddItem(new CXTPReportRecordItemText(strLabel));
		 	pItem->SetAlignment(xtpColumnTextRight);
		}

		pItem->SetItemData((DWORD_PTR)nxValue);
		pItem->SetEditable(FALSE);
		 
		// Count 
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxDataCount, _T("%.0f")));  
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(xtpColumnTextCenter);
	}

	StyleSelectChangeAction();
	m_wndReportCtrl.Populate();
}

void KThemeNodeCategory::OnCbnSelchangeComboStyle()
{
	StyleSelectChangeAction();
	ClearNodeThemes();
}

void KThemeNodeCategory::StyleSelectChangeAction()
{
	int nCurSelectedIndex = -99;
	int nStyleID          = -99;

	if ( m_cboStyle.GetCount() != 0)
	{
		nCurSelectedIndex = m_cboStyle.GetCurSel();
		nStyleID          = (int)m_cboStyle.GetItemData(nCurSelectedIndex);
	}

	std::vector<TNodeTemplateType> veTemplateType;

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection != NULL)
	{				
		try
		{
			KProjectDBase::NodeTemplateType(spDBaseConnection, nStyleID, veTemplateType);
		}
		catch (...)
		{
			AfxMessageBox(_T("Error : LinkTemplateType"));
			return;
		}		
	}
	else
	{
		AfxMessageBox(_T("Error : Connection-Project.db"));
		return;
	}

	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecordItemNumber* pItemSize = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	int nTemplateCount = (int)veTemplateType.size();
	for (int i=0; i<nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);
		pItem      = pRecord->GetItem(0);

		CString strSymbol;
		if (nTemplateCount > i)
		{
			strSymbol.AppendChar(veTemplateType[i].TSymbol);
			pItem->SetTextColor(RGB(veTemplateType[i].TR, veTemplateType[i].TG, veTemplateType[i].TB));
			pItem->SetItemData(veTemplateType[i].TSymbol);
		}
		else
		{
			strSymbol.AppendChar(48);
			pItem->SetTextColor(RGB(255, 0, 0));
			pItem->SetItemData(48);
		}
		
		pItem->SetCaption(strSymbol);

		pItemSize = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
		if (nTemplateCount > i)
		{			
			pItemSize->SetValue(veTemplateType[i].TSize);
			pItemSize->SetItemData((DWORD_PTR)veTemplateType[i].TSize);
		}
		else
		{
			pItemSize->SetValue(8.0);
			pItemSize->SetItemData((DWORD_PTR)8.0);
		}
	}

	m_nPreSelectedStyleID = nStyleID;

	m_wndReportCtrl.Populate();
}

void KThemeNodeCategory::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if ( 0 == nIndex )
	{
		KPointMarkerSelectDlg dlg;
		int      nSymbol = pItemNotify->pItem->GetItemData();
		COLORREF oColor  = pItemNotify->pItem->GetTextColor();
		dlg.SetInitialData(nSymbol, oColor);

		if (IDOK == dlg.DoModal())
		{
			int nSelectMarkerIndex = dlg.GetSelectedMarkerIndex();
			if (nSelectMarkerIndex > 0)
			{
				CString strSymbol;
				strSymbol.AppendChar(nSelectMarkerIndex);        
				pItemNotify->pItem->SetCaption(strSymbol);
				pItemNotify->pItem->SetItemData(nSelectMarkerIndex);

				pItemNotify->pItem->SetTextColor(dlg.GetSelectedMarkerColor());
			}
		}

		m_wndReportCtrl.RedrawControl();
	}
}

void KThemeNodeCategory::OnBnClickedButtonStyleSave()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	int     nCurSelectedIndex = m_cboStyle.GetCurSel();
	CString strExistStyleName;
	int     nID = 0;
	if (nCurSelectedIndex >= 0)
	{
		m_cboStyle.GetLBText(nCurSelectedIndex, strExistStyleName);
		nID = m_cboStyle.GetItemData(nCurSelectedIndex);
	}

	KThemeStyleSave dlg;
	dlg.SetExistStyleName(strExistStyleName);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	CString strSQL;
	bool bOverwrite = dlg.IsOverWrite();	

	try
	{
		spDBaseConnection->BeginTransaction();

		if (!bOverwrite)
		{
			CString strNewStyleName = dlg.GetNewStyleName();
			nID = KProjectDBase::NewNodeTemplateID(spDBaseConnection, 0);	
			strSQL.Format(_T("INSERT OR REPLACE INTO NodeTemplate(ID,Type,Name) Values('%d','%d','%s')"), nID,0,strNewStyleName.Trim());
			spDBaseConnection->ExecuteUpdate(strSQL);
		}		

		for (int i=0; i<nRecordCount; i++)
		{
			CString      strRGB;
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(0);

			int      nSymbol = pItem->GetItemData();
			COLORREF oColor  = pItem->GetTextColor();
			strRGB.Format(_T("%d,%d,%d"), GetRValue(oColor), GetGValue(oColor), GetBValue(oColor));

			pItem        = pRecord->GetItem(1);
			double dSize = _ttof(pItem->GetCaption(NULL));
			strSQL.Format(_T("INSERT OR REPLACE INTO NodeTemplateType(ID,Seq,Symbol,Size,RGB) Values('%d','%d','%d','%.1f','%s')"), nID, i, nSymbol, dSize, strRGB);

			spDBaseConnection->ExecuteUpdate(strSQL);
		}
		spDBaseConnection->Commit();

		m_nPreSelectedStyleID = nID;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
	}	

	LoadInitialStyle();
}


void KThemeNodeCategory::OnBnClickedButtonStyleEdit()
{
	KPointStyleTypeEdit dlg;
	dlg.DoModal();

	LoadInitialStyle();
	StyleSelectChangeAction();
}


void KThemeNodeCategory::OnBnClickedButtonClear()
{
	ClearNodeThemes();
}

void KThemeNodeCategory::ClearNodeThemes()
{
	m_pMapView->ClearNodeThemes();
}

void KThemeNodeCategory::OnBnClickedButtonApply()
{
	//ClearNodeThemes();

	int nCurSelectedIndex = m_cboCategory.GetCurSel();
	if ( nCurSelectedIndex < 0 )
	{
		AfxMessageBox(_T("컬럼을 선택해 주세요."));
		m_cboCategory.SetFocus();
		return;
	}    

	KIOColumn* pColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedIndex);
	if (pColumn == NULL)
	{
		AfxMessageBox(_T("관련 컬럼 정보를 찾을 수 없습니다."));
		return;
	}

	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	if (nRecordCount < 1)
	{
		return;
	}

	std::map<Integer, TNodeTypeRecord> mapTypeRecord;
	for (int i=0; i<nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);
		pItem      = pRecord->GetItem(0);

		TNodeTypeRecord oTypeRecord;
		oTypeRecord.TCharacterIndex = pItem->GetItemData();
		oTypeRecord.TColorRGB       = pItem->GetTextColor();

		pItem      = pRecord->GetItem(1);
		oTypeRecord.TSize = _ttof(pItem->GetCaption(NULL));

		pItem      = pRecord->GetItem(2);
		oTypeRecord.TLabel = pItem->GetCaption(NULL);
		Integer nxKey   = (Integer)pItem->GetItemData();

		mapTypeRecord.insert(std::make_pair(nxKey, oTypeRecord));
	}

	std::map<Integer, TNodeTypeRecord>::iterator iter = mapTypeRecord.begin();
	std::map<Integer, TNodeTypeRecord>::iterator end  = mapTypeRecord.end();

	std::vector<TDrawNodeInfo> vecDrawNodeInfo;
	TDrawNodeInfo              oTDrawNodeInfo;

	CString strSQL;
	strSQL.Format (_T(" Select Node_Id, %s FROM %s "), pColumn->Name(), TABLE_NODE);

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

	while( pResult->Next() )
	{
		Integer nxNodeId	= pResult->GetValueInt64 (0);
		Integer nxValue     = pResult->GetValueInt64 (1);

		iter = mapTypeRecord.find(nxValue);
		if (iter != end)
		{
			TNodeTypeRecord& oTypeRecord = iter->second;

			oTDrawNodeInfo.TNodeID = nxNodeId;
			oTDrawNodeInfo.TSymbol = oTypeRecord.TCharacterIndex;
			oTDrawNodeInfo.TColorRGB       = oTypeRecord.TColorRGB;			
			oTDrawNodeInfo.TSize           = oTypeRecord.TSize;
			oTDrawNodeInfo.TLabel 		   = oTypeRecord.TLabel;

			vecDrawNodeInfo.push_back(oTDrawNodeInfo);
		}		
	}
	m_pMapView->DrawNodeThemes(vecDrawNodeInfo);
}


