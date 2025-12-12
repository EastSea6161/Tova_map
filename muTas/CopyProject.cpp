// CopyProject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CopyProject.h"
#include "afxdialogex.h"
#include "KmzApp.h"
#include "ImChampFrameWindow.h"
#include "Scenario.h"
#include "Mode.h"
#include "Purpose.h"

// KCopyProject 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCopyProject, KResizeDialogEx)

KCopyProject::KCopyProject(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KCopyProject::IDD, pParent)
{

}

KCopyProject::~KCopyProject()
{
}

void KCopyProject::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KCopyProject, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KCopyProject::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KCopyProject::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_PRJBTN_LOCATION, &KCopyProject::OnBnClickedPrjbtnLocation)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KCopyProject::OnReportCheckItem)
	ON_BN_CLICKED(IDC_CHECK1, &KCopyProject::OnBnClickedCheck1)
END_MESSAGE_MAP()


// KCopyProject 메시지 처리기입니다.

int CALLBACK KCopyProject::BrowseCallBackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lPData )
{
	switch (uMsg)
	{
	case  BFFM_INITIALIZED:
		{
			::SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)lPData);
		}
		break;
	}
	return 0;
}

BOOL KCopyProject::OnInitDialog()
{
    TxLogDebugStartMsg();
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetResize(IDC_STATIC_TOP,    SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_BOTTOM, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_REPORT,        SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK1,        SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDOK,              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
// 
// 	SetDlgItemText(IDC_EDIT1, _T("temp1"));
// 	SetDlgItemText(IDC_PRJEDT_LOCATION, _T("d:\\db"));

	KReportCtrlSetting::Default(m_wndReport);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReport);

	InitReportHeader();
	InitReportData();

    TxLogDebugEndMsg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KCopyProject::ValidateInput( void )
{
	//#0. ProjectName
	GetDlgItemText(IDC_EDIT1, m_strPrjName);
	m_strPrjName.Trim();

	if (m_strPrjName.IsEmpty())
	{
		GetDlgItem(IDC_EDIT1)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetSel(0, -1);
		ThrowException(_T("프로젝트명을 입력해주세요."));
	}
	
	if (m_strPrjName.FindOneOf(_T("\\/:*?<>|")) != -1)
	{
		GetDlgItem(IDC_EDIT1)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetSel(0, -1); 
		ThrowException(_T("프로젝트명에 사용할 수 없는 문자가 있습니다."));
	}
	
	//#1. ProjectLocation
	GetDlgItemText(IDC_PRJEDT_LOCATION, m_strPrjPath);
	m_strPrjPath.Trim();

	if (m_strPrjPath.IsEmpty())
	{
		GetDlgItem(IDC_PRJBTN_LOCATION)->SetFocus();
		ThrowException(_T("프로젝트 경로를 선택하세요."));
	}

	CFileFind file;
	// 2020.02.09 - Han He : 시스템 root에 프로젝트 생성이 안되는 오류 수정
	bool r = PathIsRoot(m_strPrjPath);

	if (!file.FindFile(m_strPrjPath) && !r)
	{
		GetDlgItem(IDC_PRJEDT_LOCATION)->SetFocus();
		((CEdit*)GetDlgItem(IDC_PRJEDT_LOCATION))->SetSel(0, -1);
		CString tmpstr = _T("");
		tmpstr.Format(_T("올바르지 않은 경로입니다.\n%s"), m_strPrjPath);
		//ThrowException(_T("올바르지 않은 경로입니다."));
		ThrowException(tmpstr);
	}

	m_strFullPath.Format(_T("%s\\%s"), m_strPrjPath, m_strPrjName);
	
	if (file.FindFile(m_strFullPath))
	{
		file.FindNextFile();

		if (file.IsDirectory())
		{
			GetDlgItem(IDC_PRJEDT_LOCATION)->SetFocus();
			((CEdit*)GetDlgItem(IDC_PRJEDT_LOCATION))->SetSel(0, -1);
			ThrowException(_T("선택한 경로에 동일한 프로젝트가 존재합니다."));
		}
	}

	//#2. 타겟 선택했는지 Check;
	int  nRecord   = m_wndReport.GetRecords()->GetCount();
	BOOL bSelected = FALSE;

	for (int i = 0; i < nRecord; ++i)
	{
		if (m_wndReport.GetRecords()->GetAt(i)->GetItem(0)->GetCheckedState())
		{
			bSelected = TRUE;
			break;
		}
	}

	if (!bSelected)
		ThrowException(_T("Copy할 Target을 선택해주세요."));
	
}

void KCopyProject::OnBnClickedOk()
{
    TxLogDebugStartMsg();

	try
	{
		ValidateInput();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ThreadCaller, this, false, nLang);

		if (!m_bThreadOK)
			ThrowException(m_strErrorMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error : KCopyProject::OnBnClickedOk"));
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}
		
	AfxMessageBox(_T("프로젝트 복사가 완료되었습니다."));

    TxLogDebug(_T("프로젝트 복사가 완료되었습니다."));
    TxLogDebugEndMsg();
	KResizeDialogEx::OnOK();
}


void KCopyProject::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KCopyProject::OnBnClickedPrjbtnLocation()
{
	CString strPath = theApp.GetProfileString(_T("Common"), _T("ProjectLocation"));
	PathRemoveFileSpec((LPWSTR)(LPCWSTR)strPath);

	ITEMIDLIST *pidlBrowse;
	TCHAR      szPathName[1024] = {0};
	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = GetSafeHwnd();
	browseInfo.pidlRoot = NULL;

	memset( &browseInfo, 0, sizeof(browseInfo) );
	browseInfo.pszDisplayName = szPathName;
	browseInfo.lpszTitle      = _T("경로 선택");
	browseInfo.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	browseInfo.lParam         = (LPARAM)strPath.GetBuffer( strPath.GetLength() );
	browseInfo.lpfn           = BrowseCallBackProc;
	browseInfo.hwndOwner      = this->GetSafeHwnd();

	pidlBrowse = ::SHBrowseForFolder(&browseInfo);

	if( pidlBrowse )
	{
		::SHGetPathFromIDList(pidlBrowse, szPathName);
		SetDlgItemText(IDC_PRJEDT_LOCATION, szPathName);
	}
}

void KCopyProject::InitReportHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	//pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select Copy Target"), 1));
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select Target"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetTreeColumn(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시나리오 선택"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Data"), 80, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("DB 존재 여부"));
    }
}

void KCopyProject::InitReportData( void )
{
	m_wndReport.ResetContent();

	try
	{
		CXTPReportRecord*     pRecord       = nullptr;
		CXTPReportRecordItem* pItem         = nullptr;
		CXTPReportRecordItem* pScenarioItem = nullptr;
		
		KProject* pProject = ImChampFrameWindow::GetProject();

		//Base Scenario
		KScenario*                   pBaseScenario = pProject->GetBaseScenario();
		KTarget*                     pBaseTarget   = nullptr;
		const KTDTargetMap&          mapBaseTarget = pBaseScenario->GetTargetMap();
		KTDTargetMap::const_iterator citTargetMap  = mapBaseTarget.begin();

		if (pBaseScenario == nullptr)
			ThrowException(_T("Base Scenario가 존재하지 않습니다."));

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pBaseScenario->GetName()));
		pItem->SetAlignment(DT_LEFT);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(TRUE);
		pItem->SetItemData((DWORD_PTR)pBaseScenario);
        pItem->SetEditable(FALSE);
		
		pScenarioItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		
		AddTreeRecord(pRecord, pBaseScenario);
		
		//Other Scenario
		const KTDScenarioMap&          scenarioMap = pProject->GetScenarioMap();
		KTDScenarioMap::const_iterator itScenario, itEnd = scenarioMap.end();

		for (itScenario = scenarioMap.begin(); itScenario != itEnd; ++itScenario)
		{
			KScenario* pScenario = itScenario->second;

			if (pScenario == nullptr)
				continue;

			pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pScenario->GetName()));
			pItem->SetAlignment(DT_LEFT);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetItemData((DWORD_PTR)pScenario);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));

			AddTreeRecord(pRecord, pScenario);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		
	}

	m_wndReport.Populate();
	m_wndReport.ExpandAll();

}

void KCopyProject::AddTreeRecord( CXTPReportRecord* a_pParent, KScenario* a_pScenario )
{
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	const KTDTargetMap& targetMap = a_pScenario->GetTargetMap();
	KTDTargetMap::const_iterator itTarget, itEnd = targetMap.end();

	CString strValue;

	for (itTarget = targetMap.begin(); itTarget != itEnd; ++itTarget)
	{
		KTarget* pTarget = itTarget->second;

		if (pTarget == nullptr)
			continue;

		pRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pTarget->GetName()));
		pItem->SetAlignment(DT_LEFT);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);
		pItem->SetItemData((DWORD_PTR)pTarget);
        if (a_pScenario->IsBaseScenario()) {
            pItem->SetChecked(TRUE);
            pItem->SetEditable(FALSE);
        }

		if (pTarget->IsEmptyData())
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("없음")));
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Empty")));
			}
			//pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Empty")));
			pItem->SetTextColor(RGB(255, 0, 0));
		}
		else
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("있음")));
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Exist")));
			}
			//pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Exist")));
			pItem->SetTextColor(RGB(0, 0, 255));
		}

		pItem->SetAlignment(DT_CENTER);
	}
}

void KCopyProject::OnReportCheckItem( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if (pItemNotify->pColumn->GetItemIndex() != 0)
		return;
	
	try
	{
		CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(0);

		if (pRecord->HasChildren())
		{
			CXTPReportRecords* pChildRecords = pRecord->GetChilds();
			BOOL               bChecked      = pItem->IsChecked();
			int                nRecord       = pChildRecords->GetCount();

			for (int i = 0; i < nRecord; ++i)
			{
				CXTPReportRecord* pChildRecord = pChildRecords->GetAt(i);
				pChildRecord->GetItem(0)->SetChecked(bChecked);
			}
		}
		else
		{
			CXTPReportRecord*  pParentRecord = pRecord->GetParentRecord();
			CXTPReportRecords* pChildRecords = pParentRecord->GetChilds();
			int                nRecord       = pChildRecords->GetCount();
			BOOL               bChecked      = FALSE;//트리Child중에 체크된것이 있는지 확인;

			for (int i = 0; i < nRecord; ++i)
			{
				if (pChildRecords->GetAt(i)->GetItem(0)->IsChecked())
				{
					bChecked = TRUE;
					break;
				}
			}
			pParentRecord->GetItem(0)->SetChecked(bChecked);
		}

		m_wndReport.RedrawControl();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return;
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error - KCopyProject::OnReportCheckItem"));
		return;
	}

}


void KCopyProject::OnBnClickedCheck1()
{
	try
	{
		CButton* pButton  = (CButton*)GetDlgItem(IDC_CHECK1);
		BOOL     bChecked = pButton->GetCheck();

		CXTPReportRecords* pRecords = m_wndReport.GetRecords();
		CXTPReportRecord*  pRecord  = nullptr;
		int                nRecord  = pRecords->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pRecord = pRecords->GetAt(i);
			pRecord->GetItem(0)->SetChecked(bChecked);

			if (pRecord->HasChildren())
			{
				CXTPReportRecords* pChildRecords = pRecord->GetChilds();
				int                nChildRecord  = pChildRecords->GetCount();

				for (int j = 0; j < nChildRecord; ++j)
				{
					pChildRecords->GetAt(j)->GetItem(0)->SetChecked(bChecked);
				}
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return;
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error - KCopyProject::OnBnClickedCheck1"));
		return;
	}

	m_wndReport.RedrawControl();
}

void KCopyProject::RunCopyProject( void )
{
	m_bThreadOK   = false;
	m_strErrorMsg = _T("");

	try
	{
		KProject* pParentProject = ImChampFrameWindow::GetProject();
		CString   strParentPath  = pParentProject->GetLocation();
		CString   strValue;

		//폴더생성
		QBicDir::CreateDirectory(m_strFullPath);

		//파라미터 복사
		CopyDefaultParameter(strParentPath, m_strFullPath);
		CopyDefaultProjectDB(strParentPath, m_strFullPath);

		//프로젝트 파일 생성
		CreateProjectFile();
		CreateScenarioFile();

		m_bThreadOK = true;
	}
	catch (...)
	{
		m_strErrorMsg = _T("복사에 실패했습니다.");
	}
}

void KCopyProject::CreateProjectFile( void )
{
	CString strFile, strValue;

	try
	{
		//프로젝트 파일 생성 시작
		strFile = m_strFullPath;
		strFile.AppendFormat(_T("\\%s"), m_strPrjName);

		if (KmzSystem::IsPassengerSystem())
		{
			strFile.Append(_T(".tpx"));
		}
		else
		{
			strFile.Append(_T(".kpf"));
		}

		remove((CStringA)strFile);
		::WritePrivateProfileString( _T("APP"), _T("DESC"), _T("TOVA Project File"), strFile );
		
		KProject* pProject = ImChampFrameWindow::GetProject();
		CString   pszApp   = _T("Project");

		::WritePrivateProfileString( pszApp, _T("Name"),         m_strPrjName, strFile );
		::WritePrivateProfileString( pszApp, _T("Investigator"), pProject->getInvestigator(), strFile );
		::WritePrivateProfileString( pszApp, _T("Organization"), pProject->SetOrganization(), strFile );
		::WritePrivateProfileString( pszApp, _T("Description"),  pProject->GetDescription(),  strFile );
		
		//- base year
		if (!m_wndReport.GetRecords()->GetAt(0)->GetItem(0)->GetCheckedState()
			|| !m_wndReport.GetRecords()->GetAt(0)->HasChildren())
		{
			strValue = _T("");
		}
		else
		{
			CXTPReportRecords* pChildRecords = m_wndReport.GetRecords()->GetAt(0)->GetChilds();
			KTarget*           pTarget       = (KTarget*)pChildRecords->GetAt(0)->GetItem(0)->GetItemData();
				
			strValue.Format(_T("%d"), pTarget->GetTargetYear());
		}
		
		::WritePrivateProfileString( pszApp, _T("BaseYear"), strValue, strFile );

		//- target year
		strValue = _T("");
		std::vector<int> vecTargetYear = pProject->GetTargetYear();
		std::vector<int>::iterator itTargets = vecTargetYear.begin();

		while (vecTargetYear.end() != itTargets)
		{
			CString strYear;
			strYear.Format( _T("%d"), *itTargets );
			strValue += strYear;
			strValue += _T(", ");
			++itTargets;
		}

		if( strValue.IsEmpty() == false )
		{
			strValue.Truncate( strValue.GetLength() - 2 );
			::WritePrivateProfileString( pszApp, _T("Targets"), strValue, strFile );
		}

		//- purpose name
		strValue = _T("");
		std::vector<CString> vecPurposeName = pProject->GetPurposeName();
		std::vector<CString>::iterator itPurposeName = vecPurposeName.begin();
		while( vecPurposeName.end() != itPurposeName )
		{
			CString strPurposeName;
			strPurposeName = *itPurposeName;
			strValue += strPurposeName;
			strValue += _T(", ");
			++itPurposeName;
		}

		if( strValue.IsEmpty() == false )
		{
			strValue.Truncate( strValue.GetLength() -2);
			::WritePrivateProfileString( pszApp, _T("Purpose"), strValue, strFile );
		}

		//- purpose id, group
		strValue          = _T("");
		CString strValue2 = _T("");
		CString strValue3 = _T("");
		std::vector<KPurpose*> vecPurpose = pProject->GetPurpose();
		std::vector<KPurpose*>::iterator itPurpose = vecPurpose.begin();
		while( vecPurpose.end() != itPurpose)
		{
			KPurpose* pPurpose = *itPurpose;
			CString strPurposeId;
			CString strPurposeGroup;

			strPurposeId.Format(_T("%d"),	 pPurpose->PurposeID());
			strPurposeGroup.Format(_T("%d"), pPurpose->PurposeGroup());

			strValue2 += strPurposeId;
			strValue2 += _T(", ");

			strValue3 = strPurposeGroup;

			++itPurpose;
		}

		if( strValue2.IsEmpty() == false )
		{
			strValue2.Truncate( strValue2.GetLength() -2);
			::WritePrivateProfileString( pszApp, _T("PurposeId"), strValue2, strFile);
		}

		if( strValue3.IsEmpty() == false )
		{
			::WritePrivateProfileString( pszApp, _T("PurposeGroup"), strValue3, strFile);
		}

		//- mode name
		strValue = _T("");
		std::vector<CString> vecModeName = pProject->GetModeName();
		std::vector<CString>::iterator itModeName = vecModeName.begin();
		while( vecModeName.end() != itModeName )
		{
			CString strMode;
			strMode = *itModeName;
			strValue += strMode;
			strValue += _T(", ");
			++itModeName;
		}

		if( strValue.IsEmpty() == false )
		{
			strValue.Truncate( strValue.GetLength() -2);
			::WritePrivateProfileString( pszApp, _T("Mode"), strValue, strFile );
		}

		//- mode id, group
		strValue  = _T("");
		strValue2 = _T("");
		strValue3 = _T("");
		std::vector<KMode*> vecMode = pProject->GetMode();
		std::vector<KMode*>::iterator itMode = vecMode.begin();
		while( vecMode.end() != itMode)
		{
			KMode* pMode = *itMode;
			CString strModeId;
			CString strModeGroup;

			strModeId.Format(_T("%d"),	  pMode->ModeID());
			strModeGroup.Format(_T("%d"), pMode->ModeGroup());

			strValue2 += strModeId;
			strValue2 += _T(", ");

			strValue3 = strModeGroup;

			++itMode;
		}

		if( strValue2.IsEmpty() == false )
		{
			strValue2.Truncate( strValue2.GetLength() -2);
			::WritePrivateProfileString( pszApp, _T("ModeId"), strValue2, strFile);
		}

		if( strValue3.IsEmpty() == false )
		{
			::WritePrivateProfileString( pszApp, _T("ModeGroup"), strValue3, strFile);
		}

		//- Analysis Info
		strValue = _T("");
		strValue.Format(_T("%d"), pProject->GetAnalysisAreaId());
		::WritePrivateProfileString( pszApp, _T("AnalysisId"), strValue, strFile );
		::WritePrivateProfileString( pszApp, _T("AnalysisName"), pProject->GetAnalysisAreaName(), strFile );
		::WritePrivateProfileString( pszApp, _T("AnalysisFolderName"), pProject->GetAnalysisFolderName(), strFile );

		//-KEMDataUse
		CString strDataUse = _T("");
// 		if( pProject->GetProjectType() == KEMKTDBProject)
// 		{
// 			strDataUse = _T("1");
// 		}
// 		else
// 		{
			strDataUse = _T("2");
//		}
		::WritePrivateProfileString( pszApp, _T("ProjectDataUse"), strDataUse, strFile );
		
		//프로젝트 파일 하단 시나리오 정의부분;
		strValue = _T("");
		int                   nRecord = m_wndReport.GetRecords()->GetCount();
		CXTPReportRecordItem* pItem   = nullptr;
		
		strValue = _T("");
		for (int i = 1; i < nRecord; ++i)	//i = 1은 Base를 제외한 다른 시나리오에 대해서만 해당기능을 수행하므로.
		{
			pItem = m_wndReport.GetRecords()->GetAt(i)->GetItem(0);

			if (pItem->IsChecked())
			{
				KScenario* pScenario = (KScenario*)pItem->GetItemData();

				CString strScenarioPath = pScenario->GetLocation();
				int     nIndex          = strScenarioPath.ReverseFind(_T('\\'));
				
				if( -1 != nIndex )
				{
					strScenarioPath = strScenarioPath.Right( strScenarioPath.GetLength() - nIndex );
					strScenarioPath.Trim( _T("\\") );
					CString strKey;
					strKey.Format( _T("Scenario_%s"), pScenario->GetName() );
					::WritePrivateProfileString( pszApp, strKey, strScenarioPath, strFile );
				} 

				strValue.AppendFormat(_T("%s;"), pScenario->GetName());
			}
		}

		if (!strValue.IsEmpty())
		{
			strValue.TrimRight(_T(";"));
			::WritePrivateProfileString(pszApp, _T("Scenario"), strValue, strFile);
		}
		//프로젝트 파일 생성 종료		
	}
	catch (KExceptionPtr ex)
	{
	
	}
	catch (...)
	{
		
	}
}

void KCopyProject::CreateScenarioFile( void )
{
	CString pszApp = _T("Scenario");
	CString strValue;
	CString strValue1;
	CString strName;
	CString strFile;

	int     nIndex;

	KProject* pProject = ImChampFrameWindow::GetProject();
	
	//Base;
	strName = _T("BASE");
	strValue.Format(_T("%s\\%s"), m_strFullPath, strName);
	QBicDir::CreateDirectory(strValue);

	strFile.Format(_T("%s\\%s\\%s.ksx"), m_strFullPath, strName, strName);

	if (!m_wndReport.GetRecords()->GetAt(0)->GetItem(0)->IsChecked())
	{
		::WritePrivateProfileString(pszApp, _T("Name"), strName, strFile);
	}
	else
	{
		CString strSrcDir;
		strSrcDir.Format(_T("%s\\%s"), pProject->GetLocation(), strName);
		strValue.Format(_T("%s\\%s"), m_strFullPath, strName);
	
		QBicDir::CopyDirectory(strSrcDir, strValue, TRUE);
	}

	//Base를 제외한 나머지 Scenario;
	
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	int                   nRecord = m_wndReport.GetRecords()->GetCount();

	for (int i = 1; i < nRecord; ++i)	//Base에 대해선 수행하지 않으므로 i = 1
	{
		pRecord = m_wndReport.GetRecords()->GetAt(i);
		pItem   = pRecord->GetItem(0);

		if (!pItem->IsChecked())
			continue;

		//시나리오 디렉토리 생성
		KScenario* pScenario       = (KScenario*)pItem->GetItemData();
		CString    strScenarioPath = pScenario->GetLocation();
		
		nIndex = strScenarioPath.ReverseFind(_T('\\'));

		if (nIndex == -1)
			ThrowException(_T("시나리오 경로가 잘못되었습니다."));

		strScenarioPath = strScenarioPath.Right(strScenarioPath.GetLength() - nIndex);
		strScenarioPath.Trim( _T("\\"));


		strValue.Format(_T("%s\\%s"), m_strFullPath, strScenarioPath);
		QBicDir::CreateDirectory(strValue);

		strFile.Format(_T("%s\\%s\\%s.ksx"), m_strFullPath, strScenarioPath, pScenario->GetName());

		//ksx 생성
		::WritePrivateProfileString(pszApp, _T("Name"), pScenario->GetName(), strFile);

		if (pRecord->HasChildren())
		{
			strValue  = _T("");
			strValue1 = _T("");

			CXTPReportRecords* pChildRecords = pRecord->GetChilds();
			CXTPReportRecord*  pChildRecord  = nullptr;
			int                nChildRecord  = pChildRecords->GetCount();

			for (int i = 0; i < nChildRecord; ++i)
			{
				pChildRecord = pChildRecords->GetAt(i);
				pItem        = pChildRecord->GetItem(0);

				if (!pItem->IsChecked())
					continue;

				KTarget* pTarget = (KTarget*)pItem->GetItemData();
				pTarget->SaveTargetInfo(strFile);

				CString strTargetPath = pTarget->GetLocation();
				int     nIndex        = strTargetPath.ReverseFind(_T('\\'));
				
				if (-1 != nIndex)
				{
					strTargetPath = strTargetPath.Right( strTargetPath.GetLength() - nIndex );
					strTargetPath.Trim( _T("\\") );
					CString strKey;
					strKey.Format( _T("Target_%s"), pTarget->GetName() );
					::WritePrivateProfileString( pszApp, strKey, strTargetPath, strFile );

					//CopyTarget;
					if (!pTarget->IsEmptyData())
					{
						CString strSrcDir;
						strSrcDir.Format(_T("%s\\%s\\%s"), pProject->GetLocation(), strScenarioPath, strTargetPath);
						strValue.Format(_T("%s\\%s\\%s"), m_strFullPath, strScenarioPath, strTargetPath);

						QBicDir::CopyDirectory(strSrcDir, strValue, TRUE);
					}
				}

				strValue1 += pTarget->GetName();
				strValue1 += _T(";");
			}

			if( strValue1.IsEmpty() == false )
			{
				strValue1.TrimRight( _T(";") );
				::WritePrivateProfileString( pszApp, _T("Target"), strValue1, strFile );
			}
		}
	}
}

void KCopyProject::CopyDefaultParameter( CString a_strSource, CString a_strTarget )
{
	bool bSubDirCopy = true;//하위폴더 정보 복사

	a_strSource.Trim();
	a_strTarget.Trim();

	if (a_strSource.IsEmpty())
		ThrowException(_T("복사할 프로젝트의 경로정보가 없습니다."));

	if (a_strTarget.IsEmpty())
		ThrowException(_T("저장할 프로젝트의 경로정보가 없습니다."));

	CFileFind finder;
	
	if (!finder.FindFile(a_strSource))
		ThrowException(_T("복사할 프로젝트의 경로정보가 잘못되었습니다."));

	a_strSource.Append(_T("\\Parameter"));
	a_strTarget.Append(_T("\\Parameter"));
	
	if (finder.FindFile(a_strSource))
	{
		finder.FindNextFile();

		if (finder.IsDirectory())
			QBicDir::CopyDirectory(a_strSource, a_strTarget, bSubDirCopy);
	}
	else
	{
		//Src Project에 Parameter 폴더가 없으면 Skip;
	}

}

void KCopyProject::CopyDefaultProjectDB( CString a_strSource, CString a_strTarget )
{
	bool bSubDirCopy = true;//하위폴더 정보 복사

	a_strSource.Trim();
	a_strTarget.Trim();

	if (a_strSource.IsEmpty())
		ThrowException(_T("복사할 프로젝트의 경로정보가 없습니다."));

	if (a_strTarget.IsEmpty())
		ThrowException(_T("저장할 프로젝트의 경로정보가 없습니다."));

	CFileFind finder;

	if (!finder.FindFile(a_strSource))
		ThrowException(_T("복사할 프로젝트의 경로정보가 잘못되었습니다."));

	a_strSource.Append(_T("\\project.db"));
	a_strTarget.Append(_T("\\project.db"));

	CopyFile(a_strSource, a_strTarget, FALSE);
}

unsigned __stdcall KCopyProject::ThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();
	
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("프로젝트 복사 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Copy Project..."));
	}

	KCopyProject* pDlg = (KCopyProject*)pParameter->GetParameter();
	pDlg->RunCopyProject();

	return 0;
}
