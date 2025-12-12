// InterPathGenMain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterPathGenMain.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Project.h"
#include "DBaseAssignment.h"
#include "KBulkDBase2File.h"
#include "WriteCodeAgrument.h"
#include "Launcher.h"

// KInterPathGenMain 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterPathGenMain, KResizeDialogEx)

KInterPathGenMain::KInterPathGenMain(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterPathGenMain::IDD, pParent)
	, m_nStep(0)
	, m_pTarget(a_pTarget)
{
}

KInterPathGenMain::~KInterPathGenMain()
{
}

void KInterPathGenMain::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KInterPathGenMain, KResizeDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_BACK, &KInterPathGenMain::OnBnClickedButtonBack)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, &KInterPathGenMain::OnBnClickedButtonNext)
    ON_BN_CLICKED(IDCANCEL, &KInterPathGenMain::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &KInterPathGenMain::OnBnClickedOk)
    ON_WM_SIZE()
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

BOOL KInterPathGenMain::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
        
    CWnd* pWnd = GetDlgItem(IDC_FRAME);
    CRect rect;
    pWnd->GetWindowRect(&rect);
    ScreenToClient(&rect);

	m_dlgPathGenTypeA.SetTarget(m_pTarget);
    m_dlgPathGenTypeA.Create(IDD_6840_INTERMODALPATHGEN_A, this);
    m_dlgPathGenTypeA.ShowWindow(WS_VISIBLE | WS_CHILD);
    m_dlgPathGenTypeA.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
    //초기화 루틴
    
	m_dlgPathGenTypeB.SetTarget(m_pTarget);
    m_dlgPathGenTypeB.Create(IDD_6840_INTERMODALPATHGEN_B, this);
    m_dlgPathGenTypeB.ShowWindow(WS_VISIBLE | WS_CHILD);
    m_dlgPathGenTypeB.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	m_dlgPathGenTypeC.SetTarget(m_pTarget);
    m_dlgPathGenTypeC.Create(IDD_6840_INTERMODALPATHGEN_C, this);
    m_dlgPathGenTypeC.ShowWindow(WS_VISIBLE | WS_CHILD);
    m_dlgPathGenTypeC.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
    
    UIChange();

    return TRUE;  // return TRUE unless you set the focus to a control
}

void KInterPathGenMain::UIChange()
{
    switch(m_nStep)
    {
    case 0:
        m_dlgPathGenTypeA.ShowWindow(SW_SHOW);
        m_dlgPathGenTypeB.ShowWindow(SW_HIDE);
        m_dlgPathGenTypeC.ShowWindow(SW_HIDE);
        break;
    case 1:
        m_dlgPathGenTypeA.ShowWindow(SW_HIDE);
        m_dlgPathGenTypeB.ShowWindow(SW_SHOW);
        m_dlgPathGenTypeC.ShowWindow(SW_HIDE);
        break;
    case 2:
        m_dlgPathGenTypeA.ShowWindow(SW_HIDE);
        m_dlgPathGenTypeB.ShowWindow(SW_HIDE);
        m_dlgPathGenTypeC.ShowWindow(SW_SHOW);
        break;
    default:
        break;
    }

    CButton* pButtonBack = (CButton*)GetDlgItem(IDC_BUTTON_BACK);
    CButton* pButtonNext = (CButton*)GetDlgItem(IDC_BUTTON_NEXT);
    CButton* pButtonRun  = (CButton*)GetDlgItem(IDOK);

    switch(m_nStep)
    {
    case 0:
        pButtonBack->EnableWindow(FALSE);
        pButtonNext->EnableWindow(TRUE);
        pButtonRun->EnableWindow (FALSE);
        break;
    case 1:
        pButtonBack->EnableWindow(TRUE);
        pButtonNext->EnableWindow(TRUE);
        pButtonRun->EnableWindow (FALSE);
        break;
    case 2:
        pButtonBack->EnableWindow(TRUE);
        pButtonNext->EnableWindow(FALSE);
        pButtonRun->EnableWindow (TRUE);
        break;
    default:
        break;
    }
}

void KInterPathGenMain::OnBnClickedButtonBack()
{
    try
    {
        if (m_nStep > 0)
        {
            m_nStep = m_nStep - 1;
        }        

        UIChange();
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


void KInterPathGenMain::OnBnClickedButtonNext()
{
    try
    {
        if (m_nStep < 2)
        {
            m_nStep = m_nStep + 1;
        }        

        UIChange();
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


void KInterPathGenMain::OnBnClickedOk()
{
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(InterGeneratorThreadCaller, this, true, nLang);

	if (m_nResultCode == DLL_PLANNED_TERMINATION)
	{
		AfxMessageBox(_T("성공적으로 완료되었습니다."));
	}
	else if (m_nResultCode == DLL_USER_STOPPING)
	{
		AfxMessageBox(_T("복합수단 경로 생성이 중지되었습니다."));
		return;
	}
	else
	{
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
	}

    KResizeDialogEx::OnOK();
}

void KInterPathGenMain::OnBnClickedCancel()
{
    KResizeDialogEx::OnCancel();
}

unsigned __stdcall KInterPathGenMain::InterGeneratorThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("복합수단 경로 생성 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Intermodal Path Generator..."));
	}
	KInterPathGenMain*        pDlg = (KInterPathGenMain*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}

void KInterPathGenMain::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KInterPathGenMain::Run( void )
{
	try
	{

		KDBaseInterModal::ExportDB2TxtXBusRest(m_pTarget);

		CreateArgument();

		CString strGeneratorPath(_T(""));
		strGeneratorPath.Format(_T("%s\\%s"), DLL_MODEL_FOLDER_NAME, _T("Intermodalgenerator"));

		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget, strGeneratorPath);

		m_nResultCode = DLL_UNEXPECTED_ERROR;
		CString strWindowName;
		GetWindowText(strWindowName);

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			m_nResultCode = DLL_USER_STOPPING;
			return;
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("복합수단 경로 생성 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run Intermodal Path Generator Module..."));
		}

		KLauncher::Run(strWindowName, 18);

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("복합수단 경로 생성 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("Intermodal Path Generator Module Complete."));
			}
			TxLogDebug(_T("Model Run Ok.."));
		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			TxLogDebug(_T("Model Stop.."));
			ThrowException(_T("Model Stop..")); 

		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			AddStatusMessage(_T("> InterModal Path Generator  Dll Not Found ..."));
			TxLogDebug(_T("DLL NOT Found.."));
			ThrowException(_T("DLL Not Found.."));
		}
		else
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Running Failed."));
			}
			TxLogDebug(_T("Model Run Error.."));
			ThrowException(_T("InterModal Path Generator End(Error)"));
		}

		SaveResultValue();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}
}

bool KInterPathGenMain::CreateArgument( void )
{
	try
	{
		ClearFiles();

		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(T_ARG_INTERMODAL_PATH_GENERATOR, nOpenMode);

		CString strModelPath(_T(""));
		strModelPath.Format(_T("%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH);

		CreateDirectory(strModelPath,NULL);
		CreateDirectory(ConvertSavePath(_T("")), NULL);

		CString strDllPath = ConvertSavePath(_T(""));
		CString strResultPath = strDllPath + INTERMODAL_GEN_RESULT_PATH;

		DeleteAllFolderFile(strResultPath);
		CreateDirectory(strResultPath, NULL);

		KIOColumns oODColumnColetion;
		KIOColumn* pLinkColumn;
		ArgumentWriteProjectName(of);
		m_dlgPathGenTypeA.ArgumentPurPoseOD(oODColumnColetion, of);
		m_dlgPathGenTypeA.ArgumentUrbanPath(of);
		m_dlgPathGenTypeA.ArgumentRegionalPath(of);
		
		m_dlgPathGenTypeC.ArgumentRestriction(of);
		m_dlgPathGenTypeC.ArgumentTimeMethod(pLinkColumn, of);

		m_dlgPathGenTypeB.ArgumentAutoCost(of);
		m_dlgPathGenTypeB.ArgumentTaxiCost(of);
		m_dlgPathGenTypeB.ArgumentTransitCost(of);

		m_dlgPathGenTypeC.ArgumentTime(of);
		m_dlgPathGenTypeC.ArgumnetVDF(of);
		m_dlgPathGenTypeC.ArgumentCondition(of);
		m_dlgPathGenTypeC.ArgumentProsessTime(of);

		ArgumentWriteNoofClass(of);
		ArgumentWriteFolder(of);

		of.Close();

		if ( !KDBaseInterModal::InterExportDB2NodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("node.dat")) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2LinkFileGeneratorPath(m_pTarget->GetDBaseConnection(), pLinkColumn, ConvertSavePath(T_DAT_LINK) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LinkFileC) 중 오류가 발생하였습니다. !!!"));
		}

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			m_nResultCode = DLL_USER_STOPPING;
			AddStatusMessage(_T("> Stopping.."));
			return false;
		}

		if ( !KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_LINEINFO), ConvertSavePath(T_DAT_LINE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LineFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_PURPOSE_OD, oODColumnColetion, ConvertSavePath(T_DAT_PURPOSEOD) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2AccessStopNodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2AccessNodeStopFile) 중 오류가 발생하였습니다. !!!"));
		}

		if (!KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(),  ConvertSavePath(T_DAT_TURN)))
		{
			ThrowException(_T("Argument 생성(회전정보) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KDBaseInterModal::InterExportDB2TerminalScheulFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_TERMINAL_SCHEDULE)))
		{
			ThrowException(_T("Dat 생성(ExportDB2TerminalFlaeFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KDBaseInterModal::InterExportDb2AccessTerminalNodeAtZone(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_ACCESSTERMINAL_NODE)))
		{
			ThrowException(_T("Dat 생성(AccessTerminalNoeAtZone) 중 오류가 발생하였습니다. !!!"));
		}
		
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
		return false;
	}
	return true;
}

void KInterPathGenMain::ClearFiles( void )
{
	int nResult = -1;
	try
	{
		nResult = remove((CStringA)T_ARG_INTERMODAL_PATH_GENERATOR);
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINEINFO));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TURN));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_PURPOSEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_ACCESSTERMINAL_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TERMINAL_SCHEDULE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE));
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

CString KInterPathGenMain::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH,INTERMODAL_GENERATOR_PATH ,a_strFileName);

	return strSavePath;
}

void KInterPathGenMain::DeleteAllFolderFile( CString a_strFolderName )
{
	CFileFind finder;
	BOOL bContinue = TRUE;

	if(a_strFolderName.Right(1) != _T("\\"))
		a_strFolderName += _T("\\");

	a_strFolderName += _T("*.*");
	bContinue = finder.FindFile(a_strFolderName);
	while(bContinue)
	{
		bContinue = finder.FindNextFile();
		if(finder.IsDots()) // Ignore this item.
		{
			continue;
		}
		else if(finder.IsDirectory()) // Delete all sub item.
		{
			DeleteAllFolderFile(finder.GetFilePath());
			::RemoveDirectory((LPCTSTR)finder.GetFilePath());
		}
		else // Delete file.
		{
			::DeleteFile((LPCTSTR)finder.GetFilePath());
		}
	}
}

void KInterPathGenMain::ArgumentWriteProjectName( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();
		strOut.Format(_T("%s_%s_%s\r\n"), pProject->GetName(), m_pTarget->CaptionScenario(), m_pTarget->CaptionTargetYear());
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KInterPathGenMain::ArgumentWriteNoofClass( CStdioFileEx& of )
{
	CString strOut;
	std::map<CString, double> mapNoofClass;

	try
	{
		KDBaseAssignment::GetNoClass(m_pTarget, mapNoofClass);

		int nCount = mapNoofClass.size();

		strOut.Format(_T("noClass\t%d\r\n"), nCount);
		of.WriteString(strOut);

		std::map<CString, double>::iterator iter = mapNoofClass.begin();
		std::map<CString, double>::iterator end  = mapNoofClass.end();

		while(iter != end)
		{
			strOut.Format(_T("%s\t%f\r\n"), iter->first, iter->second);
			of.WriteString(strOut);
			++iter;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KInterPathGenMain::ArgumentWriteFolder( CStdioFileEx& of )
{
	CString strFolder;
	try
	{
		strFolder.Format(_T("%s"),m_pTarget->GetLocation());
		strFolder = strFolder + DLL_READ_WRITE_PATH + INTERMODAL_GENERATOR_PATH;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


BOOL KInterPathGenMain::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	try
	{
		int     nMsgCode = pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return KResizeDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void KInterPathGenMain::SaveResultValue( void )
{
	try
	{
		m_dlgPathGenTypeA.SaveResultOD2DB();
		m_dlgPathGenTypeA.SaveResultUrban2DB();
		m_dlgPathGenTypeA.SaveResultRegion2DB();

		m_dlgPathGenTypeB.SaveResultAutoCost2DB();
		m_dlgPathGenTypeB.SaveResultTaxiCost2DB();
		m_dlgPathGenTypeB.SaveResultTransitCost2DB();

		m_dlgPathGenTypeC.SaveResultTime2DB();
		m_dlgPathGenTypeC.SaveResultTerminalTime2DB();

		CopyFile(T_ARG_INTERMODAL_PATH_GENERATOR, ConvertSavePath(T_ARG_INTERMODAL_PATH_GENERATOR), FALSE);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}
