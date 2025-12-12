// ProjectInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ProjectInfoDlg.h"
#include "afxdialogex.h"

#include "Project.h"
#include "ImChampFrameWindow.h"
#include "Scenario.h"


// KProjectInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KProjectInfoDlg, KResizeDialogEx)

KProjectInfoDlg::KProjectInfoDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KProjectInfoDlg::IDD, pParent)
{

}

KProjectInfoDlg::~KProjectInfoDlg()
{
}

void KProjectInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KProjectInfoDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KProjectInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KProjectInfoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KProjectInfoDlg 메시지 처리기입니다.

void KProjectInfoDlg::SetResizeCtrl( void )
{
	SetResize(IDC_REPORT,        SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_BOTTOM, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDOK,              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}

BOOL KProjectInfoDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		SetResizeCtrl();
		InitReportHeader();
		InitReportData();
	}
	catch (...)
	{
		AfxMessageBox(_T("Project Info 로드에 실패했습니다."));
		TxLogDebugException();
	}

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KProjectInfoDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnOK();
}


void KProjectInfoDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KProjectInfoDlg::InitReportHeader( void )
{
	KReportCtrlSetting::Default(m_wndReport);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReport);

	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
// 	m_wndReport.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
// 	m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSmallDots);

	CXTPReportColumn* pColumn = nullptr;

	int nIndex = 0;

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Scenario-Target"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시나리오"));
    }
	
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Generation"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행발생 분석"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Distribution"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행분포 분석"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Assignment"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행배정 분석"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Multimodal"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도시 대중교통 분석"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Intermodal"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("복합수단 분석"));
    }
}

void KProjectInfoDlg::InitReportData( void )
{
	try
	{
		m_wndReport.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		bool      bIsEmptyTarget = true;//시나리오 타겟만 있고 데이터가 없는지를 체크함
		KProject* pProject       = ImChampFrameWindow::GetProject();

		//Base
		KScenario* pBaseScenario = pProject->GetBaseScenario();

		if (pBaseScenario != nullptr)
		{
			const KTDTargetMap&          mapBaseTarget = pBaseScenario->GetTargetMap();
			KTDTargetMap::const_iterator bIter, eIter  = mapBaseTarget.end();

			for (bIter = mapBaseTarget.begin(); bIter != eIter; ++bIter)
			{
				KTarget* pTarget = bIter->second;

				if (pTarget->IsEmptyData())
					continue;

				bIsEmptyTarget = false;
			}

			if (!bIsEmptyTarget)
			{
				pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

				//Scenario-Target
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pBaseScenario->GetName()));
				//G
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				//D
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				//A
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				//M
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				//I
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));

				for (bIter = mapBaseTarget.begin(); bIter != eIter; ++bIter)
				{
					KTarget* pTarget = bIter->second;

					if (pTarget->IsEmptyData())
						continue;

					AddChildRecord(pRecord, pTarget);
				}
			}
		}//end if (pBaseScenario != nullptr)

		//exclude base
		const KTDScenarioMap&          mapScenario  = pProject->GetScenarioMap();
		KTDScenarioMap::const_iterator bIter, eIter = mapScenario.end();

		for (bIter = mapScenario.begin(); bIter != eIter; ++bIter)
		{
			bIsEmptyTarget = true;

			KScenario*          pScenario = bIter->second;
			const KTDTargetMap& mapTarget = pScenario->GetTargetMap();

			KTDTargetMap::const_iterator bTargetIter, eTargetIter = mapTarget.end();

			for (bTargetIter = mapTarget.begin(); bTargetIter != eTargetIter; ++bTargetIter)
			{
				KTarget* pTarget = bTargetIter->second;

				if (pTarget->IsEmptyData())
					continue;

				bIsEmptyTarget = false;
			}

			if (!bIsEmptyTarget)
			{
				pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

				//Scenario-Target
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pScenario->GetName()));
				//G
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				//D
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				//A
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				//M
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				//I
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));

				for (bTargetIter = mapTarget.begin(); bTargetIter != eTargetIter; ++bTargetIter)
				{
					KTarget* pTarget = bTargetIter->second;

					if (pTarget->IsEmptyData())
						continue;

					AddChildRecord(pRecord, pTarget);
				}
			}
		}

		m_wndReport.Populate();
		m_wndReport.ExpandAll(TRUE);
	}
	catch (...)
	{
		
	}
}

void KProjectInfoDlg::GetGenerationRunTime( KTarget* a_pTarget, CString& a_strDate )
{
	try
	{
		a_strDate = _T("");

		CString strQuery = _T("SELECT MAX(End_Date) FROM Generation_Model");

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			a_strDate = spResultSet->GetValueString(0);
		}

		if (a_strDate.IsEmpty())
			a_strDate = _T("-");
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;	
	}

	
}

void KProjectInfoDlg::GetDistributionRunTime( KTarget* a_pTarget, CString& a_strDate )
{
	try
	{
		a_strDate = _T("");

		CString strQuery = _T("SELECT MAX(End_Date) FROM Distribution_Model");

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			a_strDate = spResultSet->GetValueString(0);
		}

		if (a_strDate.IsEmpty())
			a_strDate = _T("-");
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;	
	}	
}

void KProjectInfoDlg::GetAssignRunTime( KTarget* a_pTarget, CString& a_strDate )
{
	try
	{
		a_strDate = _T("");

		CString strQuery = _T("SELECT MAX(End_Date) FROM assign_model where type between 1 and 4");

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			a_strDate = spResultSet->GetValueString(0);
		}

		if (a_strDate.IsEmpty())
			a_strDate = _T("-");
	}
	catch (...)
	{	
		TxLogDebugException();
		throw 1;	
	}	
}

void KProjectInfoDlg::GetMultiModalRunTime( KTarget* a_pTarget, CString& a_strDate )
{
	try
	{
		a_strDate = _T("");

		CString strQuery = _T("SELECT MAX(End_Date) FROM assign_model where type = 5");

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			a_strDate = spResultSet->GetValueString(0);
		}

		if (a_strDate.IsEmpty())
			a_strDate = _T("-");
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;	
	}
}

void KProjectInfoDlg::GetInterModalRunTime( KTarget* a_pTarget, CString& a_strDate )
{
	try
	{
		a_strDate = _T("");

		CString strQuery = _T("SELECT MAX(End_Date) FROM assign_model where type = 6");

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			a_strDate = spResultSet->GetValueString(0);
		}

		if (a_strDate.IsEmpty())
			a_strDate = _T("-");
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;	
	}	
}

void KProjectInfoDlg::AddChildRecord( CXTPReportRecord* a_pParent, KTarget* a_pTarget )
{
	int     nValue;
	CString strValue;
	
	CXTPReportRecord*     pChildRecord = nullptr;
	CXTPReportRecordItem* pItem        = nullptr;
	
	pChildRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);

	//Scenario-Target
	nValue  = a_pTarget->GetTargetYear();
	strValue.Format(_T("%d"), nValue);
	pItem   = pChildRecord->AddItem(new CXTPReportRecordItemText(strValue));

	//G
	GetGenerationRunTime(a_pTarget, strValue);
	pItem   = pChildRecord->AddItem(new CXTPReportRecordItemText(strValue));

	//D
	GetDistributionRunTime(a_pTarget, strValue);
	pItem   = pChildRecord->AddItem(new CXTPReportRecordItemText(strValue));

	//A
	GetAssignRunTime(a_pTarget, strValue);
	pItem   = pChildRecord->AddItem(new CXTPReportRecordItemText(strValue));

	//M
	GetMultiModalRunTime(a_pTarget, strValue);
	pItem   = pChildRecord->AddItem(new CXTPReportRecordItemText(strValue));

	//I
	GetInterModalRunTime(a_pTarget, strValue);
	pItem   = pChildRecord->AddItem(new CXTPReportRecordItemText(strValue));
}

