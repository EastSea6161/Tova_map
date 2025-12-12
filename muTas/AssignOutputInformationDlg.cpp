// AssignOutputInformationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AssignOutputInformationDlg.h"
#include "afxdialogex.h"
//^^ #include "IOColumn.h"

// KAssignOutputInformationDlg 대화 상자입니다.
enum
{
	_0_COLUMN_TABLE_NAME = 0,	
	_1_TYPE_NAME,
	_2_COLUMN_NAME
};

#define LINKOUTPUT _T("Link")

IMPLEMENT_DYNAMIC(KAssignOutputInformationDlg, CXTResizeDialog)

KAssignOutputInformationDlg::KAssignOutputInformationDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KAssignOutputInformationDlg::IDD, pParent)
	,m_nModeOutputType(1)
{
	m_strLinkGroupName  = _T("Link");
	m_strAttriGroupName = _T("Attribute");
}

KAssignOutputInformationDlg::~KAssignOutputInformationDlg()
{
}

void KAssignOutputInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,		 m_wndReportCtrl);
	DDX_Radio  (pDX, IDC_RADIO_NEW,      (int&)m_nRadioOutputType);  
	DDX_Control(pDX, IDC_COMBO1,		 m_cboOverwriteInfo);
}


BEGIN_MESSAGE_MAP(KAssignOutputInformationDlg, CXTResizeDialog)
	ON_BN_CLICKED(IDC_RADIO_NEW,       &KAssignOutputInformationDlg::OnBnClickedRadioNew)
	ON_BN_CLICKED(IDC_RADIO_OVERWRITE, &KAssignOutputInformationDlg::OnBnClickedRadioOverwrite)    
	ON_BN_CLICKED(IDOK,                &KAssignOutputInformationDlg::OnBnClickedOk)
    ON_CBN_SELCHANGE(IDC_COMBO1,       &KAssignOutputInformationDlg::OnCbnSelchangeCombo1)	
END_MESSAGE_MAP()

void KAssignOutputInformationDlg::SetMode( KTarget* pTarget, std::vector<CString> a_vecModeName )
{
	m_pTarget = pTarget;
	m_vecModeName = a_vecModeName;

	KIOTable* pLinkTable = m_pTarget->Tables()->FindTable(TABLE_LINK);
	if (nullptr != pLinkTable) {
		m_strLinkGroupName = pLinkTable->Caption();
	}
	KIOTable* pParaODTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
	if (nullptr != pParaODTable) {
		m_strAttriGroupName = pParaODTable->Caption();
	}
}

BOOL KAssignOutputInformationDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    m_nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(m_pTarget);

	KReportCtrlSetting::Default(m_wndReportCtrl);
	m_wndReportCtrl.GetColumns()->Clear();
	m_wndReportCtrl.ResetContent();
    
	SetDefualtDescription();
	InitReportColumn();
	
	if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
	{
		InitReportRecordModeType();
	}
	else
	{
		InitReportRecord();  
	}	   
	m_wndReportCtrl.Populate();   
    
    #pragma region Assign_Result_Info	
	KDBaseAssignment::GetRunHighwayAssignInfo(m_pTarget, m_vecAssignResultInfo);    
	int     nCboIndex = -1;
	CString strMsg;
	 
	for (int i=0; i<(int)m_vecAssignResultInfo.size(); i++)
	{
		TAssignResultInfo& oAssignResultInfo = m_vecAssignResultInfo[i];
		strMsg.Format(_T("%s(%s~%s)"), oAssignResultInfo.TstrDescription, oAssignResultInfo.TstrStartDate, oAssignResultInfo.TstrEndDate);
		nCboIndex = m_cboOverwriteInfo.AddString(strMsg);
		m_cboOverwriteInfo.SetItemData(nCboIndex, (DWORD_PTR)oAssignResultInfo.TnRunSeq);
	}		
    #pragma endregion Assign_Result_Info

	InitOutputInfo();

    SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;  
}

void KAssignOutputInformationDlg::InitReportColumn( void )
{

	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_TABLE_NAME, _T(""), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	//pColumn->SetGroupable(TRUE);

	m_wndReportCtrl.GetColumns()->GetGroupsOrder()->Add(pColumn);
	pColumn->SetVisible(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_TYPE_NAME, _T("Name"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("출력 항목"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Out Field Name"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("결과 저장 컬럼명"));
    }

	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);
}

void KAssignOutputInformationDlg::InitReportRecord( void )
{
	CString strRunSeq;
	strRunSeq.Format(_T("(%d)"), m_nRunSeq);

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	int nNo = 0;
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Time")));
	pItem->SetAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
        pItemTxt->SetValue(_T("통행시간(분)"));
    }

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgTime") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);
	
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C")));
	pItem->SetAlignment(DT_CENTER);

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Total Volume")));
	pItem->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
        pItemTxt->SetValue(_T("총교통량(PCU)"));
    }

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("TotVol") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Trip")));
	pItem->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
        pItemTxt->SetValue(_T("통행량(인)"));
    }

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Trip") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	for (size_t i=0; i<m_vecModeName.size(); i++)
	{
		CString strModeName = m_vecModeName[i];
        		
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
		pItem->SetAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            CString strCaption = strModeName;
            strCaption.Append(_T(" 교통량(PCU)"));
            CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
            pItemTxt->SetValue(strCaption);
        }

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strModeName + strRunSeq));
		pItem->SetAlignment(DT_CENTER);
	}

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Time")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("통행시간(분)"));
	}
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgTime") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	nNo ++;
}

void KAssignOutputInformationDlg::InitReportRecordModeType()
{
	CString strRunSeq;
	strRunSeq.Format(_T("(%d)"), m_nRunSeq);

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	CString m_strLinkGroupName(_T("Link"));
	CString m_strAttriGroupName(_T("Attribute"));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_strLinkGroupName  = _T("링크");
		m_strAttriGroupName = _T("통행속성");
	}
	else {
		m_strLinkGroupName  = _T("Link");
		m_strAttriGroupName = _T("Attribute");
	}

    //* Link - Average Travel Time
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Time(Without Toll)")));
	pItem->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
        pItemTxt->SetValue(_T("평균통행시간(분)(비용제외)"));
    }

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgTime") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

    //* Link - avgTime(w Toll)
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Time(With Toll)")));
	pItem->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
        pItemTxt->SetValue(_T("평균통행시간(분)(비용포함)"));
    }

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgTime+") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	int nIndex = 1;
	if (m_nModeInVDF  > 1)
	{
	//* Link - VDF별 Travel Time
		for (int i=0; i< m_nModeInVDF && m_nModeInVDF > 1; i++)
		{
            CString strTemp(_T(""));
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));

			strTemp.Format(_T("VDF%d Time(Without Toll)"), nIndex);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);

            if (KmzSystem::GetLanguage() == KEMKorea) {
                strTemp.Format(_T("VDF%d 통행시간(분)(비용 제외)"), nIndex);
                CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
                pItemTxt->SetValue(strTemp);
            }

			strTemp.Format(_T("VDF%dTime%s"), nIndex, strRunSeq);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);
			nIndex++;
		}
	}
	
	if (m_nModeInVDF  > 1)
	{
		nIndex = 1;
	//* Link - VDF별 Travel Time
		for (int i=0; i<m_nModeInVDF && m_nModeInVDF > 1; i++)
		{
            CString strTemp(_T(""));
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));

			strTemp.Format(_T("VDF%d Time(With Toll)"), nIndex);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);
            if (KmzSystem::GetLanguage() == KEMKorea) {
                strTemp.Format(_T("VDF%d 통행시간(분)(비용 포함)"), nIndex);
                CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
                pItemTxt->SetValue(strTemp);
            }

			strTemp.Format(_T("VDF%dTime+%s"), nIndex, strRunSeq);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);
			nIndex++;
		}
	}
	
	//* Link - V/C 컬럼
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C")));
	pItem->SetAlignment(DT_CENTER);
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

    //* Link - Sum Volume 
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Total Volume")));
	pItem->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
        pItemTxt->SetValue(_T("총교통량(PCU)"));
    }

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("TotVol") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//* Link - 수단별 Volume
	size_t nCount = m_vecModeName.size();	
	for (size_t  i=0; i< nCount && nCount > 1; i++)
	{
        CString strTemp(_T(""));
		CString strModeName = m_vecModeName[i];
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));

		strTemp.Format(_T("%s Volume"), strModeName);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
		pItem->SetAlignment(DT_CENTER);

        if (KmzSystem::GetLanguage() == KEMKorea) {
            strTemp.Format(_T("%s 교통량(PCU)"), strModeName);
            CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
            pItemTxt->SetValue(strTemp);
        }

		strTemp.Format(_T("%s%s"), strModeName, strRunSeq);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
		pItem->SetAlignment(DT_CENTER);
	}
    	
	if (true == m_bBusInitalVol)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus Initial Volume")));
		pItem->SetAlignment(DT_CENTER);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("BusInitVol") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);
	}

	//* OD - TravelTime
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Time(WithOut Toll)")));
	pItem->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
        pItemTxt->SetValue(_T("평균통행시간(분)(비용 제외)"));
    }

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgTime") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//* OD - 수단별
	if (m_nModeInVDF  > 1)
	{
		nIndex = 1;
		for (int i=0; i<m_nModeInVDF && m_nModeInVDF > 1; i++)
		{
            CString strTemp(_T(""));
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));

			strTemp.Format(_T("VDF%d Time(Without Toll)"), nIndex);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);

            if (KmzSystem::GetLanguage() == KEMKorea) {
                strTemp.Format(_T("VDF%d 통행시간(분)(비용 제외)"), nIndex);
                CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
                pItemTxt->SetValue(strTemp);
            }

			strTemp.Format(_T("VDF%dTime%s"), nIndex, strRunSeq);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);
			nIndex++;
		}
	}
}


void KAssignOutputInformationDlg::OnBnClickedRadioNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	//UpdateData();
	m_cboOverwriteInfo.SetCurSel(-1);
	m_cboOverwriteInfo.EnableWindow(FALSE);
	CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
	CString strDesciption = _T("");
	strDesciption.Format(_T("%s(%d)"),m_strDescription, m_nRunSeq);
	pEditBox->SetWindowTextW(strDesciption);
}

void KAssignOutputInformationDlg::OnBnClickedRadioOverwrite()
{
	m_cboOverwriteInfo.EnableWindow(TRUE);
	ChangeComboBox();
}

void KAssignOutputInformationDlg::SetModelType( int nModeInVDF , KEMAssignmentModel a_emMoel  )
{
	m_nModeInVDF = nModeInVDF;

	m_emAModel   = a_emMoel;

	if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
	{
		m_nModeOutputType = 1;
	}
	else
	{
		m_nModeOutputType = 0;
	}
}

void KAssignOutputInformationDlg::OnBnClickedOk()
{
	UpdateData();
	GetOutputDesignInfo();
	
	CXTResizeDialog::OnOK();
}

void KAssignOutputInformationDlg::GetOutputDesignInfo()
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

	for (int i=0 ;  i<(int)m_vecAssignResultInfo.size() ; i++ )
	{
		TAssignResultInfo& oAssignResultInfo = m_vecAssignResultInfo[i];
		if (nSeq == oAssignResultInfo.TnRunSeq)
		{
			oAssignResultInfo.TstrDescription	= strEditBoxName;
			m_oAssign						= oAssignResultInfo;
			m_bUseOverWirte					= true;			
		}
	}

}

void KAssignOutputInformationDlg::GetOutputInfo( bool& a_bUseNew, TAssignResultInfo& a_oTARsultInfo )
{
	a_bUseNew = m_bUseOverWirte;
	a_oTARsultInfo = m_oAssign;
}

void KAssignOutputInformationDlg::ChangeComboBox()
{
	int nSeq = 0;

	int nCurSel = m_cboOverwriteInfo.GetCurSel();
	
	if (nCurSel < 0)
	{
		nCurSel = 0;
	}
	
	m_cboOverwriteInfo.SetCurSel(nCurSel);

	nSeq = m_cboOverwriteInfo.GetItemData(nCurSel);

	for (int i=0; i<(int)m_vecAssignResultInfo.size(); i++)
	{
		TAssignResultInfo& oAssignResultInfo = m_vecAssignResultInfo[i];
		if (nSeq == oAssignResultInfo.TnRunSeq)
		{
			CString strDesciption = _T("");
			strDesciption.Format(_T("%s(%d)"),m_strDescription, m_nRunSeq);
			SetDlgItemText(IDC_EDIT_TEXT, strDesciption);
		}
	}
}

void KAssignOutputInformationDlg::OnCbnSelchangeCombo1()
{
	ChangeComboBox();
}

void KAssignOutputInformationDlg::SetOutputInfo( bool a_bUseNew, TAssignResultInfo a_oTARsultInfo )
{
	m_bUseOverWirte = a_bUseNew;
	m_oAssign		= a_oTARsultInfo;
}

void KAssignOutputInformationDlg::InitOutputInfo( void )
{
	CButton* pNewCheck = (CButton*)GetDlgItem(IDC_RADIO_NEW);
	pNewCheck->SetCheck(0);
	CButton* pOverCheck = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	pOverCheck->SetCheck(0);


	if ( m_oAssign.TbUseData == false)
	{
		if (m_vecAssignResultInfo.size() > 0)
		{
			pOverCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(0);
			m_cboOverwriteInfo.EnableWindow(TRUE);
			ChangeComboBox();
		}
		else
		{
			pNewCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(-1);
			m_cboOverwriteInfo.EnableWindow(FALSE);
			CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
			CString strDesciption = _T("");
			strDesciption.Format(_T("%s(%d)"),m_strDescription, m_nRunSeq);
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
			strDesciption.Format(_T("%s(%d)"),m_strDescription, m_nRunSeq);
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

void KAssignOutputInformationDlg::SetDefualtDescription()
{
	switch(m_emAModel)
	{
		case KEMAssignmentModelAllorNothing:
			m_strDescription = _T("All-or-Nothing");
			break;
		case KEMAssignmentModelIncremental:
			m_strDescription = _T("Incremental");
			break;
		case KEMAssignmentModelUserEquilibriumFW:
			m_strDescription = _T("UserEquilibrium_FW");
			break;
		case KEMAssignmentModelUserEquilibriumOB:
			m_strDescription = _T("UserEquilibrium_OB");
			break;
		case KEMAssignmentModelSegment:
			m_strDescription = _T("Segment");
			break;
		case KEMAssignmentModelMultiModal:
			m_strDescription = _T("Multi-Modal");
			break;
		default:
			m_strDescription = _T("");
			break;
	}

}

void KAssignOutputInformationDlg::SetBusInitalVolume(bool a_bUseInitalVolume )
{
	m_bBusInitalVol = a_bUseInitalVolume;
}
