// PolygonStyleEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PolygonStyleEdit.h"
#include "afxdialogex.h"

#include "DBaseConnector.h"
#include "ImTasDBase.h"
//^#include "MunsellColorCircle.h"
// KPolygonStyleEdit 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPolygonStyleEdit, KDialogEx)

KPolygonStyleEdit::KPolygonStyleEdit(CWnd* pParent /*=NULL*/)
	: KDialogEx(KPolygonStyleEdit::IDD, pParent)
{
	m_nPreSelectedStyleID      = -1;
}

KPolygonStyleEdit::~KPolygonStyleEdit()
{
}

void KPolygonStyleEdit::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_cboStyle);
}


BEGIN_MESSAGE_MAP(KPolygonStyleEdit, KDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &KPolygonStyleEdit::OnCbnSelchangeComboStyle)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, &KPolygonStyleEdit::OnReportItemClick)
	ON_EN_SETFOCUS(IDC_EDIT_STYLE_NAME, &KPolygonStyleEdit::OnEnSetfocusEditStyleName)
	ON_CBN_SETFOCUS(IDC_COMBO_STYLE, &KPolygonStyleEdit::OnCbnSetfocusComboStyle)
	ON_BN_CLICKED(IDC_BUTTON_CONTENTS_ADD, &KPolygonStyleEdit::OnBnClickedButtonContentsAdd)
	ON_BN_CLICKED(IDC_BUTTON_CONTENTS_DELETE, &KPolygonStyleEdit::OnBnClickedButtonContentsDelete)
	ON_BN_CLICKED(IDOK, &KPolygonStyleEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_DELETE, &KPolygonStyleEdit::OnBnClickedButtonStyleDelete)
END_MESSAGE_MAP()

BOOL KPolygonStyleEdit::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);  
	m_wndReportCtrl.AllowEdit(TRUE);
	m_wndReportCtrl.SelectionEnable(FALSE);

	CXTPReportColumn* pColumn = NULL;	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Symbol"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Delete"),   10));
	//pColumn->GetEditOptions()->AddSpinButton(TRUE);
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

void KPolygonStyleEdit::UpdateUIState()
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


void KPolygonStyleEdit::OnCbnSelchangeComboStyle()
{
	StyleSelectChangeAction();
}

void KPolygonStyleEdit::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if ( 0 == nIndex )
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetTextColor(), pItemNotify->pItem->GetTextColor()) ;
		if ( IDOK == dlg.DoModal() )
		{
			pItemNotify->pItem->SetTextColor(dlg.GetColor());       
			m_wndReportCtrl.RedrawControl();
		}
	}
}

void KPolygonStyleEdit::LoadInitialStyle()
{
	m_cboStyle.ResetContent();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection != NULL)
	{
		std::vector<TZoneTemplate> vecZoneTemplate;
		KProjectDBase::ZoneTemplate(spDBaseConnection, 0, vecZoneTemplate);

		int nPreSelectedIndex = -1;
		for (size_t i=0; i<vecZoneTemplate.size(); i++)
		{
			TZoneTemplate oTemplate = vecZoneTemplate[i];
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

void KPolygonStyleEdit::StyleSelectChangeAction()
{
	int nCurSelectedIndex = -99;
	int nStyleID          = -99;

	if ( m_cboStyle.GetCount() != 0)
	{
		nCurSelectedIndex = m_cboStyle.GetCurSel();
		nStyleID          = (int)m_cboStyle.GetItemData(nCurSelectedIndex);
	}

	std::vector<TZoneTemplateType> veTemplateType;

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection != NULL)
	{		
		KProjectDBase::ZoneTemplateType(spDBaseConnection, nStyleID, veTemplateType);
	}

	m_wndReportCtrl.ResetContent();

	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	int nTemplateCount = (int)veTemplateType.size();
	for (int i=0; i<nTemplateCount; i++)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetEditable(FALSE);
		pItem->SetTextColor(RGB(veTemplateType[i].TR, veTemplateType[i].TG, veTemplateType[i].TB));
		pItem->SetCaption(_T("■"));

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->SetEditable(TRUE);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);
		pItem->SetAlignment(xtpColumnIconCenter);
	}

	m_wndReportCtrl.Populate();
}


void KPolygonStyleEdit::OnEnSetfocusEditStyleName()
{
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
	pButton->SetCheck(BST_UNCHECKED);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(BST_CHECKED);

	//StyleSelectChangeAction();
}


void KPolygonStyleEdit::OnCbnSetfocusComboStyle()
{
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
	pButton->SetCheck(BST_CHECKED);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(BST_UNCHECKED);

	StyleSelectChangeAction();
}


void KPolygonStyleEdit::OnBnClickedButtonContentsAdd()
{
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);
	pItem->SetAlignment(DT_CENTER);
	pItem->SetEditable(FALSE);

	KMunsellColor    oMunsellColor(5, false);

	pItem->SetTextColor(oMunsellColor.GetColor(m_wndReportCtrl.GetRecords()->GetCount()));
	pItem->SetCaption(_T("■"));
	
	pItem = pRecord->AddItem(new CXTPReportRecordItemText());
	pItem->SetEditable(TRUE);
	pItem->HasCheckbox(TRUE);
	pItem->SetChecked(FALSE);
	pItem->SetAlignment(xtpColumnIconCenter);

	m_wndReportCtrl.Populate();
}


void KPolygonStyleEdit::OnBnClickedButtonContentsDelete()
{
	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for (int i=nRecordCount-1; i>=0; i--)
	{
		pRecord    = pRecords->GetAt(i);
		pItem      = pRecord->GetItem(1);

		if (pItem->IsChecked() == TRUE)
		{
			pRecord->Delete();
		}
	}

	m_wndReportCtrl.Populate();
}

void KPolygonStyleEdit::Apply()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection != NULL)
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
		nID = KProjectDBase::NewLinkTemplateID(spDBaseConnection, 0);
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
			strSQL.Format(_T("INSERT OR REPLACE INTO ZoneTemplate(ID,Type,Name) Values('%d','%d','%s')"), nID,0,strStyleName);
			spDBaseConnection->ExecuteUpdate(strSQL);
		}		

		strSQL.Format(_T("Delete From ZoneTemplateType Where ID = '%d'"), nID);
		spDBaseConnection->ExecuteUpdate(strSQL);

		for (int i=0; i<nRecordCount; i++)
		{
			CString      strRGB;
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(0);

			COLORREF oColor = pItem->GetTextColor();
			strRGB.Format(_T("%d,%d,%d"), GetRValue(oColor), GetGValue(oColor), GetBValue(oColor));

			strSQL.Format(_T("INSERT OR REPLACE INTO ZoneTemplateType(ID,Seq,RGB) Values('%d','%d','%s')"), nID, i, strRGB);

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


void KPolygonStyleEdit::OnBnClickedOk()
{
	Apply();
}


void KPolygonStyleEdit::OnBnClickedButtonStyleDelete()
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
		strSQL.Format(_T("Delete From ZoneTemplate Where ID = '%d' and Type='0'"), nID);
		spDBaseConnection->ExecuteUpdate(strSQL);		

		strSQL.Format(_T("Delete From ZoneTemplateType Where ID = '%d'"), nID);
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
