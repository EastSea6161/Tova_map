// PointStyleTypeEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PointStyleTypeEdit.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "KPointMarkerSelectDlg.h"
// KPointStyleTypeEdit 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPointStyleTypeEdit, KDialogEx)

KPointStyleTypeEdit::KPointStyleTypeEdit(CWnd* pParent /*=NULL*/)
	: KDialogEx(KPointStyleTypeEdit::IDD, pParent)
{
	m_oQbicDefaultMarkerFont.CreatePointFont(120, _T("QBicF"));   
	m_nPreSelectedStyleID      = -1;
}

KPointStyleTypeEdit::~KPointStyleTypeEdit()
{
}

void KPointStyleTypeEdit::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_cboStyle);
}


BEGIN_MESSAGE_MAP(KPointStyleTypeEdit, KDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &KPointStyleTypeEdit::OnCbnSelchangeComboStyle)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, &KPointStyleTypeEdit::OnReportItemClick)
	ON_EN_SETFOCUS(IDC_EDIT_STYLE_NAME, &KPointStyleTypeEdit::OnEnSetfocusEditStyleName)
	ON_CBN_SETFOCUS(IDC_COMBO_STYLE, &KPointStyleTypeEdit::OnCbnSetfocusComboStyle)
	ON_BN_CLICKED(IDC_BUTTON_CONTENTS_ADD, &KPointStyleTypeEdit::OnBnClickedButtonContentsAdd)
	ON_BN_CLICKED(IDC_BUTTON_CONTENTS_DELETE, &KPointStyleTypeEdit::OnBnClickedButtonContentsDelete)
	ON_BN_CLICKED(IDOK, &KPointStyleTypeEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_DELETE, &KPointStyleTypeEdit::OnBnClickedButtonStyleDelete)
END_MESSAGE_MAP()


// KPointStyleTypeEdit 메시지 처리기입니다.
BOOL KPointStyleTypeEdit::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);  
	m_wndReportCtrl.AllowEdit(TRUE);
	m_wndReportCtrl.SelectionEnable(FALSE);

	CXTPReportColumn* pColumn = NULL;	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Symbol"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Size"),   10));
	pColumn->GetEditOptions()->AddSpinButton(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Delete"),   10));
	pColumn->GetEditOptions()->AddSpinButton(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	
	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(TRUE);
	pHeader->AllowColumnSort(FALSE); 

	LoadInitialStyle();
	StyleSelectChangeAction();

	UpdateUIState();
	
	m_wndReportCtrl.Populate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPointStyleTypeEdit::UpdateUIState()
{
	CButton* pButton = NULL;

	if (m_cboStyle.GetCount() > 0)
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
		pButton->SetCheck(BST_CHECKED);
		pButton->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STYLE_DELETE)->EnableWindow(TRUE);

		pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(BST_UNCHECKED);
	}
	else
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
		pButton->SetCheck(BST_UNCHECKED);

		pButton->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STYLE_DELETE)->EnableWindow(FALSE);

		pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(BST_CHECKED);
	}
}


void KPointStyleTypeEdit::OnCbnSelchangeComboStyle()
{
	StyleSelectChangeAction();
}

void KPointStyleTypeEdit::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
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

void KPointStyleTypeEdit::LoadInitialStyle()
{
	m_cboStyle.ResetContent();
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();

	if (spDBaseConnection == NULL)
		return;
	
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

void KPointStyleTypeEdit::StyleSelectChangeAction()
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
		KProjectDBase::NodeTemplateType(spDBaseConnection, nStyleID, veTemplateType);
	}

	m_wndReportCtrl.ResetContent();

	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecordItemNumber* pItemSize = NULL;
		
	int nTemplateCount = (int)veTemplateType.size();
	for (int i=0; i<nTemplateCount; i++)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetEditable(FALSE);
		pItem->SetFont(&m_oQbicDefaultMarkerFont);
		CString strSymbol;strSymbol.AppendChar(veTemplateType[i].TSymbol);
		pItem->SetTextColor(RGB(veTemplateType[i].TR, veTemplateType[i].TG, veTemplateType[i].TB));
		pItem->SetItemData(veTemplateType[i].TSymbol);
		pItem->SetCaption(strSymbol);

		pItemSize = (CXTPReportRecordItemNumber*)pRecord->AddItem(new CXTPReportRecordItemNumber(8.0, _T("%.1f")));         
		pItemSize->SetEditable(TRUE);
		pItemSize->SetAlignment(xtpColumnTextLeft);

		pItemSize->SetValue(veTemplateType[i].TSize);
		pItemSize->SetItemData((DWORD_PTR)veTemplateType[i].TSize);

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->SetEditable(TRUE);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);
		pItem->SetAlignment(xtpColumnIconCenter);
	}

	m_wndReportCtrl.Populate();
}


void KPointStyleTypeEdit::OnEnSetfocusEditStyleName()
{
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
	pButton->SetCheck(BST_UNCHECKED);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(BST_CHECKED);

	//StyleSelectChangeAction();
}


void KPointStyleTypeEdit::OnCbnSetfocusComboStyle()
{
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
	pButton->SetCheck(BST_CHECKED);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(BST_UNCHECKED);

	StyleSelectChangeAction();
}


void KPointStyleTypeEdit::OnBnClickedButtonContentsAdd()
{
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecordItemNumber* pItemSize = NULL;

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);
	pItem->SetAlignment(DT_CENTER);
	pItem->SetEditable(FALSE);
	pItem->SetFont(&m_oQbicDefaultMarkerFont);

	CString strSymbol;strSymbol.AppendChar(48);
	pItem->SetTextColor(RGB(255, 0, 0));
	pItem->SetItemData(48);
	pItem->SetCaption(strSymbol);

	pItemSize = (CXTPReportRecordItemNumber*)pRecord->AddItem(new CXTPReportRecordItemNumber(8.0, _T("%.1f")));         
	pItemSize->SetEditable(TRUE);
	pItemSize->SetAlignment(xtpColumnTextLeft);

	pItemSize->SetItemData((DWORD_PTR)8.0);

	pItem = pRecord->AddItem(new CXTPReportRecordItemText());
	
	pItem->SetEditable(TRUE);
	pItem->HasCheckbox(TRUE);
	pItem->SetChecked(FALSE);
	pItem->SetAlignment(xtpColumnIconCenter);

	m_wndReportCtrl.Populate();
}


void KPointStyleTypeEdit::OnBnClickedButtonContentsDelete()
{
	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecordItemNumber* pItemSize = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for (int i=nRecordCount-1; i>=0; i--)
	{
		pRecord    = pRecords->GetAt(i);
		pItem      = pRecord->GetItem(2);
		
		if (pItem->IsChecked() == TRUE)
		{
			pRecord->Delete();
		}
	}

	m_wndReportCtrl.Populate();
}

void KPointStyleTypeEdit::Apply()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	CString strStyleName;
	bool    bOverwrite     = false;
	int     nID = 0;

	int  nCurSelectedIndex = m_cboStyle.GetCurSel();
	int  nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO1)
	{
		bOverwrite = true;
				
		if (nCurSelectedIndex < 0)
		{
			GetDlgItem(IDC_EDIT_STYLE_NAME)->SetFocus();
			return;
		}

		nID = m_cboStyle.GetItemData(nCurSelectedIndex);
	}
	else
	{
		GetDlgItemText(IDC_EDIT_STYLE_NAME, strStyleName);
		strStyleName = strStyleName.Trim();

		if (strStyleName.GetLength() == 0)
		{			
			AfxMessageBox(_T("명칭을 입력해 주세요."));
			GetDlgItem(IDC_EDIT_STYLE_NAME)->SetFocus();
			return;
		}
		nID = KProjectDBase::NewNodeTemplateID(spDBaseConnection, 0);
	}
	
	CXTPReportRecords*	  pRecords = m_wndReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;	
	int nRecordCount = pRecords->GetCount();

	CString strSQL;	

	try
	{
		spDBaseConnection->BeginTransaction();

		if (!bOverwrite)
		{
			strSQL.Format(_T("INSERT OR REPLACE INTO NodeTemplate(ID,Type,Name) Values('%d','%d','%s')"), nID,0,strStyleName);
			spDBaseConnection->ExecuteUpdate(strSQL);
		}		

		strSQL.Format(_T("Delete From NodeTemplateType Where ID = '%d'"), nID);
		spDBaseConnection->ExecuteUpdate(strSQL);

		for (int i=0; i<nRecordCount; i++)
		{
			CString      strRGB;
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(0);

			int nSymbol = pItem->GetItemData();
			COLORREF oColor = pItem->GetTextColor();
			strRGB.Format(_T("%d,%d,%d"), GetRValue(oColor), GetGValue(oColor), GetBValue(oColor));

			pItem        = pRecord->GetItem(1);
			double dSize = _ttof(pItem->GetCaption(NULL));
			strSQL.Format(_T("INSERT OR REPLACE INTO NodeTemplateType(ID,Seq,Symbol,Size,RGB) Values('%d','%d','%d','%.1f','%s')"), nID, i, nSymbol, dSize, strRGB);

			spDBaseConnection->ExecuteUpdate(strSQL);
		}
		spDBaseConnection->Commit();
		m_nPreSelectedStyleID = nID;

		AfxMessageBox(_T("저장하였습니다."));
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		AfxMessageBox(_T("정보 저장에 실패하였습니다."));
	}

	LoadInitialStyle();
}


void KPointStyleTypeEdit::OnBnClickedOk()
{
	Apply();
}


void KPointStyleTypeEdit::OnBnClickedButtonStyleDelete()
{
	int  nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO2)
	{
		return;
	}

	int     nID = 0;

	int  nCurSelectedIndex = m_cboStyle.GetCurSel();
	if (nCurSelectedIndex < 0)
	{
		GetDlgItem(IDC_EDIT_STYLE_NAME)->SetFocus();
		return;
	}

	nID = m_cboStyle.GetItemData(nCurSelectedIndex);

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	try
	{
		CString strSQL;

		spDBaseConnection->BeginTransaction();		
		strSQL.Format(_T("Delete From NodeTemplate Where ID = '%d' and Type='0'"), nID);
		spDBaseConnection->ExecuteUpdate(strSQL);		

		strSQL.Format(_T("Delete From NodeTemplateType Where ID = '%d'"), nID);
		spDBaseConnection->ExecuteUpdate(strSQL);		
		spDBaseConnection->Commit();

		m_nPreSelectedStyleID = 0;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
	}

	LoadInitialStyle();
	StyleSelectChangeAction();
}
