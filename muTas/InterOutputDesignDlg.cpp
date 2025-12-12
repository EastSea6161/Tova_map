// InterOutputDesingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterOutputDesignDlg.h"
#include "afxdialogex.h"
#include "IOColumn.h"

enum
{
	_0_COLUMN_TABLE_NAME = 0,	
	_1_TYPE_NAME,
	_2_COLUMN_NAME
};

#define LINKOUTPUT _T("Link")

// KInterOutputDesingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterOutputDesignDlg, CXTResizeDialog)

KInterOutputDesignDlg::KInterOutputDesignDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KInterOutputDesignDlg::IDD, pParent)
{

}

KInterOutputDesignDlg::~KInterOutputDesignDlg()
{
}

void KInterOutputDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,		 m_wndReportCtrl);
	DDX_Radio  (pDX, IDC_RADIO_NEW,      (int&)m_nRadioOutputType);  
	DDX_Control(pDX, IDC_COMBO1,		 m_cboOverwriteInfo);
}


BEGIN_MESSAGE_MAP(KInterOutputDesignDlg, CXTResizeDialog)
	ON_BN_CLICKED(IDC_RADIO_NEW,       &KInterOutputDesignDlg::OnBnClickedRadioNew)
	ON_BN_CLICKED(IDC_RADIO_OVERWRITE, &KInterOutputDesignDlg::OnBnClickedRadioOverwrite)
	ON_CBN_SELCHANGE(IDC_COMBO1,	   &KInterOutputDesignDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK,				   &KInterOutputDesignDlg::OnBnClickedOk)
END_MESSAGE_MAP()



BOOL KInterOutputDesignDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	m_wndReportCtrl.GetColumns()->Clear();
	m_wndReportCtrl.ResetContent();
	m_nRunSeq	= KDBaseAssignment::GetNewAssignRunSeq(m_pTarget);

	InitReportColumn();

	InitReportRecord();

	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndReportCtrl.SetGroupRowsBold(TRUE);    
	m_wndReportCtrl.Populate();   

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	#pragma region Assign_Result_Info	
	KDBaseAssignment::GetRunAssignInfo(m_pTarget, m_oVecAssignResultInfo);

	int     nCboIndex = -1;
	CString strMsg;

	for (int i=0; i<(int)m_oVecAssignResultInfo.size(); i++)
	{
		TAssignResultInfo& oAssignResultInfo = m_oVecAssignResultInfo[i];
		strMsg.Format(_T("%s(%s~%s)"), oAssignResultInfo.TstrDescription, oAssignResultInfo.TstrStartDate, oAssignResultInfo.TstrEndDate);
		nCboIndex = m_cboOverwriteInfo.AddString(strMsg);
		m_cboOverwriteInfo.SetItemData(nCboIndex, (DWORD_PTR)oAssignResultInfo.TnRunSeq);
	}		

	 #pragma endregion Assign_Result_Info

	InitOutputInfo();

	return TRUE;
}

void KInterOutputDesignDlg::InitReportColumn( void )
{
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_TABLE_NAME, _T(""), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	m_wndReportCtrl.GetColumns()->GetGroupsOrder()->Add(pColumn);
	pColumn->SetVisible(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_TYPE_NAME, _T("Name"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Out Field Name"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);

}

void KInterOutputDesignDlg::InitReportRecord( void )
{
	CString strRunSeq;
	strRunSeq.Format(_T("(%d)"), m_nRunSeq);

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	int nNo = 0;
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Link")));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average_Time")));
	pItem->SetAlignment(DT_CENTER);
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average_Time") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Link")));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C")));
	pItem->SetAlignment(DT_CENTER);
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Link")));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Sum_Volume")));
	pItem->SetAlignment(DT_CENTER);
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Sum_Volume") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("OD")));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average_Time")));
	pItem->SetAlignment(DT_CENTER);
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average_Time") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	nNo ++;
}

void KInterOutputDesignDlg::SetTarget( KTarget* pTarget )
{
	m_pTarget = pTarget;
}

void KInterOutputDesignDlg::OnBnClickedRadioNew()
{
	m_cboOverwriteInfo.SetCurSel(-1);
	m_cboOverwriteInfo.EnableWindow(FALSE);
	CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
	CString strDesciption = _T("");
	strDesciption.Format(_T("%s(%d)"),_T("InterModal"), m_nRunSeq);
	pEditBox->SetWindowTextW(strDesciption);
}

void KInterOutputDesignDlg::OnBnClickedRadioOverwrite()
{
	m_cboOverwriteInfo.EnableWindow(TRUE);
	ChangeComboBox();
}

void KInterOutputDesignDlg::ChangeComboBox( void )
{
	int nSeq = 0;

	int nCurSel = m_cboOverwriteInfo.GetCurSel();

	if (nCurSel < 0)
	{
		return;
	}

	nSeq = m_cboOverwriteInfo.GetItemData(nCurSel);

	for (int i=0; i<(int)m_oVecAssignResultInfo.size(); i++)
	{
		TAssignResultInfo& oAssignResultInfo = m_oVecAssignResultInfo[i];
		if (nSeq == oAssignResultInfo.TnRunSeq)
		{
			CString strDescription = oAssignResultInfo.TstrDescription;
			SetDlgItemText(IDC_EDIT_TEXT, strDescription);
		}
	}
}

void KInterOutputDesignDlg::InitOutputInfo( void )
{
	CButton* pNewCheck = (CButton*)GetDlgItem(IDC_RADIO_NEW);
	pNewCheck->SetCheck(0);
	CButton* pOverCheck = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	pOverCheck->SetCheck(0);


	if ( m_oAssign.TbUseData == false)
	{
		if (m_oVecAssignResultInfo.size() > 0)
		{
			pNewCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(-1);
			m_cboOverwriteInfo.EnableWindow(FALSE);
			CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
			CString strDesciption = _T("");
			strDesciption.Format(_T("%s(%d)"),_T("InterModal"), m_nRunSeq);
			pEditBox->SetWindowTextW(strDesciption);
		}
		else
		{
			pNewCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(-1);
			m_cboOverwriteInfo.EnableWindow(FALSE);
			CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
			CString strDesciption = _T("");
			strDesciption.Format(_T("%s(%d)"),_T("InterModal"), m_nRunSeq);
			pEditBox->SetWindowTextW(strDesciption);
			pOverCheck->EnableWindow(FALSE);
		}
	
	}
	else
	{
		if (m_bUseOverWirte == false)
		{
			pNewCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(-1);
			m_cboOverwriteInfo.EnableWindow(FALSE);
			CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
			CString strDesciption = _T("");
			strDesciption.Format(_T("%s(%d)"),_T("InterModal"), m_nRunSeq);
			pEditBox->SetWindowTextW(strDesciption);
			//pEditBox->SetWindowTextW(m_oAssign.TDescription);
		}
		else
		{
			pOverCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(m_oAssign.TnRunSeq - 1);
			CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
			pEditBox->SetWindowTextW(m_oAssign.TstrDescription);
		}

	}
}

void KInterOutputDesignDlg::SetOutputInfo( bool a_bUseNew, TAssignResultInfo a_oTARsultInfo )
{
	m_bUseOverWirte = a_bUseNew;
	m_oAssign		= a_oTARsultInfo;
}

void KInterOutputDesignDlg::GetOutputInfo( bool& a_bUseNew, TAssignResultInfo& a_oTARsultInfo )
{
	a_bUseNew = m_bUseOverWirte;
	a_oTARsultInfo = m_oAssign;
}

void KInterOutputDesignDlg::OnBnClickedOk()
{
	UpdateData();
	GetOuputDesignInfo();

	CXTResizeDialog::OnOK();
}

void KInterOutputDesignDlg::GetOuputDesignInfo( void )
{
	m_oAssign.TbUseData = true;

	CString strEditBoxName = _T("");

	GetDlgItemText(IDC_EDIT_TEXT, strEditBoxName);

	bool bCheckRadio = true;

	CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NEW);

	if (pCheck->GetCheck() == 1)
	{
		bCheckRadio = false;
	}

	int nCurSel = m_cboOverwriteInfo.GetCurSel();
	if (nCurSel  <  0   || bCheckRadio == false )
	{
		m_oAssign.TstrDescription	= strEditBoxName;
		m_bUseOverWirte			= false;
		return;
	}

	int nSeq = m_cboOverwriteInfo.GetItemData(nCurSel);

	for (int i=0 ;  i<(int)m_oVecAssignResultInfo.size() ; i++ )
	{
		TAssignResultInfo& oAssignResultInfo = m_oVecAssignResultInfo[i];
		if (nSeq == oAssignResultInfo.TnRunSeq)
		{
			oAssignResultInfo.TstrDescription	= strEditBoxName;
			m_oAssign						= oAssignResultInfo;
			m_bUseOverWirte					= true;

		}
	}
}

void KInterOutputDesignDlg::OnCbnSelchangeCombo1()
{
	ChangeComboBox();
}

