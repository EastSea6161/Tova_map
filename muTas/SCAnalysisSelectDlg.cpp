// SCAnalysisSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCAnalysisSelectDlg.h"
#include "afxdialogex.h"

#include "Target.h"
//^#include "DBaseConnector.h"

// KSCAnalysisSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSCAnalysisSelectDlg, KDialogEx)

KSCAnalysisSelectDlg::KSCAnalysisSelectDlg(KTarget* a_pTarget, int a_nLineType, std::vector<int> a_vecSelected, CWnd* pParent /*=NULL*/)
	: KDialogEx(KSCAnalysisSelectDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nLineType(a_nLineType)
	, m_vecSelected(a_vecSelected)
{

}

KSCAnalysisSelectDlg::~KSCAnalysisSelectDlg()
{
}

void KSCAnalysisSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM2, m_wndScreenReport);
	DDX_Control(pDX, IDC_CUSTOM4, m_wndCordonReport);
}


BEGIN_MESSAGE_MAP(KSCAnalysisSelectDlg, KDialogEx)
		ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KSCAnalysisSelectDlg::OnRadioClicked)
		ON_BN_CLICKED(IDOK, &KSCAnalysisSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KSCAnalysisSelectDlg 메시지 처리기입니다.


BOOL KSCAnalysisSelectDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if( 0 == m_nLineType)
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));
		pRadio->SetCheck(TRUE);
	}
	else
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO2));
		pRadio->SetCheck(TRUE);
	}
	LoadScreenCordonData();

	InitScreenReportControl();
	UpdateScreenReportControl();

	InitCordonReportControl();
	UpdateCordonReportControl();

	UpdateControlState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int KSCAnalysisSelectDlg::LineType()
{
	return m_nLineType;
}

std::vector<int> KSCAnalysisSelectDlg::Selected()
{
	return m_vecSelected;
}

void KSCAnalysisSelectDlg::OnRadioClicked( UINT nID )
{
	switch (nID)
	{
	case IDC_RADIO1:
		m_nLineType = 0;
		break;
	case IDC_RADIO2:
		m_nLineType = 1;
		break;
	}
	UpdateControlState();
}

void KSCAnalysisSelectDlg::UpdateControlState( void )
{
// 	if (m_nLineType == 0)
// 	{
// 		m_wndScreenReport.EnableWindow(TRUE);
// 		m_wndCordonReport.EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		m_wndScreenReport.EnableWindow(FALSE);
// 		m_wndCordonReport.EnableWindow(TRUE);
// 	}
}

void KSCAnalysisSelectDlg::InitScreenReportControl( void )
{
	KReportCtrlSetting::Default(m_wndScreenReport, FALSE, FALSE, FALSE);
// 	m_wndScreenReport.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_wndScreenReport.GetReportHeader()->AllowColumnSort(FALSE);
// 	m_wndScreenReport.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_wndScreenReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
// 	m_wndScreenReport.SetGridStyle(TRUE, xtpReportGridSmallDots);

	int nColumnItemIndex = 0;
	CXTPReportColumn* pColumn = NULL;
	
	pColumn = new CXTPReportColumn(nColumnItemIndex++, HEADER_ID, 1);
	pColumn = m_wndScreenReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("ID"));
    }

	pColumn = new CXTPReportColumn(nColumnItemIndex++, HEADER_SELECT, 1);
	pColumn = m_wndScreenReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = new CXTPReportColumn(nColumnItemIndex++, HEADER_NAME, 3);
	pColumn = m_wndScreenReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("이름"));
    }
}

void KSCAnalysisSelectDlg::UpdateScreenReportControl( void )
{
	int nScreenCount = m_vecScreenList.size();

	for (int i = 0; i < nScreenCount; ++i)
	{
		TScreenCordonInfo oInfo = m_vecScreenList[i];
		CXTPReportRecord* pRecord = m_wndScreenReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = NULL;		

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TLineID));

		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->HasCheckbox(TRUE);
		
		if (m_nLineType == 0)
		{
			int nSelectedCount = m_vecSelected.size();

			for (int i = 0; i < nSelectedCount; ++i)
			{
				if(m_vecSelected[i] == oInfo.TLineID)
					pItem->SetChecked(TRUE);
			}
		}
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(oInfo.TLineName);
	}

	m_wndScreenReport.Populate();
}

void KSCAnalysisSelectDlg::InitCordonReportControl( void )
{
	KReportCtrlSetting::Default(m_wndCordonReport, FALSE, FALSE, FALSE);
// 	m_wndCordonReport.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_wndCordonReport.GetReportHeader()->AllowColumnSort(FALSE);
// 	m_wndCordonReport.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_wndCordonReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
// 	m_wndCordonReport.SetGridStyle(TRUE, xtpReportGridSmallDots);

	int nColumnItemIndex = 0;
	CXTPReportColumn* pColumn = NULL;

	pColumn = new CXTPReportColumn(nColumnItemIndex++, HEADER_ID, 1);
	pColumn = m_wndCordonReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("ID"));
    }

	pColumn = new CXTPReportColumn(nColumnItemIndex++, HEADER_SELECT, 1);
	pColumn = m_wndCordonReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = new CXTPReportColumn(nColumnItemIndex++, HEADER_NAME, 3);
	pColumn = m_wndCordonReport.AddColumn(pColumn);
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("이름"));
    }
}

void KSCAnalysisSelectDlg::UpdateCordonReportControl( void )
{
	int nCordonCount = m_vecCordonList.size();

	for (int i = 0; i < nCordonCount; ++i)
	{
		TScreenCordonInfo oInfo = m_vecCordonList[i];
		CXTPReportRecord* pRecord = m_wndCordonReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = NULL;		
		
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TLineID));
		
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->HasCheckbox(TRUE);

		if (m_nLineType == 1)
		{
			int nSelectedCount = m_vecSelected.size();

			for (int i = 0; i < nSelectedCount; ++i)
			{
				if(m_vecSelected[i] == oInfo.TLineID)
					pItem->SetChecked(TRUE);
			}
		}
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(oInfo.TLineName);
	}

	m_wndCordonReport.Populate();
}

void KSCAnalysisSelectDlg::LoadScreenCordonData( void )
{
	m_vecScreenList.clear();
	m_vecCordonList.clear();

	CString strQuery = _T("Select ID, Type, Name From SCREEN_CORDON Order By ID");
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strQuery);

	while (spResult->Next())
	{
		TScreenCordonInfo oScreenCordonInfo;

		oScreenCordonInfo.TLineID			= spResult->GetValueInt(0);
		oScreenCordonInfo.TDirectionType	= spResult->GetValueInt(1);
		oScreenCordonInfo.TLineName			= spResult->GetValueString(2);

		if(1 == oScreenCordonInfo.TDirectionType)
		{
			m_vecScreenList.push_back(oScreenCordonInfo);
		}
		else
		{
			m_vecCordonList.push_back(oScreenCordonInfo);
		}
	}
}


void KSCAnalysisSelectDlg::OnBnClickedOk()
{
	try
	{
		ApplySelectedInfo();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugWarning();
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		TxLogDebugException();
		return;
	}
	KDialogEx::OnOK();
}

void KSCAnalysisSelectDlg::ApplySelectedInfo( void )
{
	m_vecSelected.clear();

	CXTPReportRecords* pReportRecords 	  = NULL;
	int 			   nLineSelectedCount = 0;

	if (0 == m_nLineType)
	{
		pReportRecords = m_wndScreenReport.GetRecords();
		int nRecordCount = pReportRecords->GetCount();

		for (int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord* pReportRecord = pReportRecords->GetAt(i);
			CXTPReportRecordItem* pRecordItem = pReportRecord->GetItem(1);

			if(pRecordItem->IsChecked() == FALSE)
				continue;
			CXTPReportRecordItemNumber* pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(0);
			m_vecSelected.push_back((int)pRecordItemNumber->GetValue());

			nLineSelectedCount++;
		}
	}
	else
	{
		pReportRecords = m_wndCordonReport.GetRecords();
		int nRecordCount = pReportRecords->GetCount();

		for (int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord* pReportRecord = pReportRecords->GetAt(i);
			CXTPReportRecordItem* pRecordItem = pReportRecord->GetItem(1);

			if(pRecordItem->IsChecked() == FALSE)
				continue;
			CXTPReportRecordItemNumber* pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(0);
			m_vecSelected.push_back((int)pRecordItemNumber->GetValue());

			nLineSelectedCount++;
		}
	}

	if (0 == nLineSelectedCount)
		ThrowException(_T("Screen/Cordon Line을 선택하세요."));
}
