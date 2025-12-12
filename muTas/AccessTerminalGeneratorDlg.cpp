// AccessTerminalGeneratorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AccessTerminalGeneratorDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "ImChampDir.h"
#include "WriteCodeAgrument.h"

#include "Project.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "Launcher.h"
#include "DefineNamesDefaultPara.h"

// KAccessTerminalGeneratorDlg 대화 상자입니다.


enum
{
	_0_COLUMN_ID = 0,
	_1_COLUMN_NAME,
	_2_COLUMN_DISTANCE
};

enum
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_ODNAME
};

IMPLEMENT_DYNAMIC(KAccessTerminalGeneratorDlg, KResizeDialogEx)

KAccessTerminalGeneratorDlg::KAccessTerminalGeneratorDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KAccessTerminalGeneratorDlg::IDD, pParent),
	m_pTarget(a_pTarget)
{

}

KAccessTerminalGeneratorDlg::~KAccessTerminalGeneratorDlg()
{
}

void KAccessTerminalGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrlTerminalType);
}


BEGIN_MESSAGE_MAP(KAccessTerminalGeneratorDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KAccessTerminalGeneratorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KAccessTerminalGeneratorDlg::OnBnClickedCancel)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDDEFAULT, &KAccessTerminalGeneratorDlg::OnBnClickedDefault)
END_MESSAGE_MAP()


// KAccessTerminalGeneratorDlg 메시지 처리기입니다.


BOOL KAccessTerminalGeneratorDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		KReportCtrlSetting::Default(m_wndReportCtrlTerminalType, TRUE);

		GetTerminalInfo();
		InitReportHeaderTType();

		KDBaseDefaultParaAccessTerminalSetting::LoadDefaultAccessTerminal(m_mapDefaultAccessTerminal);

		AutoType iter = m_mapDefaultAccessTerminal.begin();
		AutoType end  = m_mapDefaultAccessTerminal.end();

		if (iter != end)
		{
			AutoType iterDefault = m_mapDefaultAccessTerminal.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if (iterDefault != end)
			{
				DefaultUpdateReportControlTType(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				DefaultUpdateReportControlTType(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}
		else
		{
			UpdateReportControlTType();
		}

		ResizeComponent();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	return TRUE; 
}

void KAccessTerminalGeneratorDlg::ResizeComponent()
{
	try
	{
		SetResize(IDC_REPORT,			      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC10,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

		SetResize(IDDEFAULT,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		
		SetResize(IDOK,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KAccessTerminalGeneratorDlg::InitReportHeaderTType( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlTerminalType.AddColumn(new CXTPReportColumn(_0_COLUMN_ID, _T("Terminal Type"),  20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("터미널 타입"));
    }
    pColumn->SetVisible(FALSE);

	pColumn = m_wndReportCtrlTerminalType.AddColumn(new CXTPReportColumn(_1_COLUMN_NAME, _T("Name"), 45));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("터미널 타입"));
    }

	pColumn = m_wndReportCtrlTerminalType.AddColumn(new CXTPReportColumn(_2_COLUMN_DISTANCE, _T("Boundary Distance(Km)"), 35));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("반경(Km)"));
    }
}

void KAccessTerminalGeneratorDlg::UpdateReportControlTType( void )
{
	try
	{
		m_wndReportCtrlTerminalType.ResetContent(TRUE);

		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);

		AutoType iter = m_mapTerminalType.begin();
		AutoType end =  m_mapTerminalType.end();
		while(iter != end)
		{
			int nCode			= iter->first;
			double dBoundary	= iter->second;
			
			CString strCodeName	= pCodeGroup->SingleCodeValue(nCode); 

			CXTPReportRecord*		pRecord = m_wndReportCtrlTerminalType.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nCode, _T("%.0f")));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCodeName));
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBoundary, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnTextRight);

			++iter;
		}
		
		m_wndReportCtrlTerminalType.Populate();
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

void KAccessTerminalGeneratorDlg::DefaultUpdateReportControlTType(int a_nMasterCode)
{
	try
	{
		m_wndReportCtrlTerminalType.ResetContent(TRUE);

		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;
		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);

		KDBaseDefaultParaAccessTerminalSetting::MakeTerminalNode(m_mapTerminalNode);
		KDBaseDefaultParaAccessTerminalSetting::NodeCodeMatching(m_pTarget, m_setNodeSystemUserCodeMatching);
		KDBaseDefaultParaAccessTerminalSetting::GetTableCode(a_nMasterCode, m_setAccessTerminalTableCode);
		KDBaseDefaultParaAccessTerminalSetting::GetAccessTerminalTable(a_nMasterCode, m_mapGetAccessTerminalTable);

		AutoType iterCheckTerminal = m_mapTerminalNode.begin();
		AutoType endCheckTerminal  = m_mapTerminalNode.end();
		if(iterCheckTerminal!=endCheckTerminal)
		{
			size_t nCount = m_mapTerminalNode.size();

			for(size_t i = 0; i < nCount; i++)
			{
				int nSystemCode = iterCheckTerminal->first;

				AutoType iterFindNode = m_setNodeSystemUserCodeMatching.find(nSystemCode);
				AutoType endFindNode  = m_setNodeSystemUserCodeMatching.end();

				AutoType iterFindTable = m_setAccessTerminalTableCode.find(nSystemCode);
				AutoType endFindTable  = m_setAccessTerminalTableCode.end();

				if ((iterFindNode != endFindNode) && (iterFindTable != endFindTable))
				{
					AutoType iter = m_mapGetAccessTerminalTable.find(nSystemCode);
					AutoType end  = m_mapGetAccessTerminalTable.end();

					if(iter != end)
					{
						KAccessTerminal oAccessTerminal = iter->second;
						int				nType			= oAccessTerminal.nType;
						CString			strName			= oAccessTerminal.strCodeName;
						double			dBoundaryDist	= oAccessTerminal.dBoundaryDist;

						CString strType(_T(""));
						strType.Format(_T("%d"), nType);

						pRecord = m_wndReportCtrlTerminalType.AddRecord(new CXTPReportRecord());

						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nType, _T("%.0f")));
						pItem->SetAlignment(DT_CENTER);
						pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
						pItem->SetEditable(FALSE);

						pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strName));
						pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
						pItem ->SetAlignment(xtpColumnTextCenter);
						pItem ->SetEditable(FALSE);

						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber (dBoundaryDist, _T("%.2f")));
						pItem ->SetAlignment(xtpColumnTextCenter);
					}
				}
				else if(iterFindNode != endFindNode)
				{
					CString strType(_T(""));
					strType.Format(_T("%d"), nSystemCode);
					CString strName       = pCodeGroup->SingleCodeValue(nSystemCode);
					double  dBoundaryDist = iterCheckTerminal->second;

					pRecord = m_wndReportCtrlTerminalType.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nSystemCode, _T("%.0f")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strName));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber (dBoundaryDist, _T("%.2f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
				}
				iterCheckTerminal++;
			}
		}
		m_wndReportCtrlTerminalType.Populate();
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

void KAccessTerminalGeneratorDlg::GetTerminalInfo( void )
{

	try
	{
		m_mapTerminalType.clear();

		std::map<int, double> mapTerminalNode;
		mapTerminalNode.insert(std::make_pair(TRANSITCOMPLEX, 10));
		mapTerminalNode.insert(std::make_pair(INTER_REGIONAL_RAILSTATION, 20));
		mapTerminalNode.insert(std::make_pair(BUS_TRMINAL, 20));
		//mapTerminalNode.insert(std::make_pair(INTER_REGIONAL_BUS_TRANSFER, 20));
		mapTerminalNode.insert(std::make_pair(AIR_TERMIANL, 40));
		mapTerminalNode.insert(std::make_pair(MARIN_PORT, 40));

		std::vector<int> vecNodeType;
		std::map<int, int> mapSystemCode;

		KDBaseInterModal::GetNetworkTypePairData(m_pTarget, mapSystemCode);
		KDBaseInterModal::GetUsingNodeType(m_pTarget, vecNodeType);


		size_t nCount = vecNodeType.size();
		for (size_t i = 0; i < nCount; i++)
		{
			int nUsingCode = vecNodeType[i];

			AutoType FindMathing = mapSystemCode.find(nUsingCode);
			AutoType EndMathing	 = mapSystemCode.end();

			if (FindMathing != EndMathing)
			{
				int nSystemCode = FindMathing->second;

				AutoType FindTerminal = mapTerminalNode.find(nSystemCode);
				AutoType EndTerminal  = mapTerminalNode.end();

				if (FindTerminal != EndTerminal)
				{
					m_mapTerminalType.insert(std::make_pair(nSystemCode, FindTerminal->second));
				}

			}
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


void KAccessTerminalGeneratorDlg::OnBnClickedOk()
{
    try
    {
        CXTPReportRecords* pRecords = m_wndReportCtrlTerminalType.GetRecords();
        if ( pRecords->GetCount() == 0 )
        {
            AfxMessageBox(_T("현재 활성화된 네트워크에 터미널 관련 정보가 존재하지 않습니다."));
            return;
        }
    }
    catch(...)
    {
        AfxMessageBox(_T("레코드 정보 조회 중 오류가 발생하였습니다.[KAccessTerminalGeneratorDlg]"));
        return;
    }

	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(AccessThreadCaller, this, true, nLang);

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			CopyFile(T_ARG_ACCESSTERMINALGENERATOR, ConvertSavePath(T_ARG_ACCESSTERMINALGENERATOR), FALSE);
			AfxMessageBox(_T("성공적으로 완료되었습니다."));
		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
			ThrowException(_T("Model Stop.."));
		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			ThrowException(_T("DLL Not Found.."));
		}
		else
		{
			ThrowException(_T("Access Terminal End(Error)"));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}
	KResizeDialogEx::OnOK();
}


void KAccessTerminalGeneratorDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

unsigned __stdcall KAccessTerminalGeneratorDlg::AccessThreadCaller( void* p )
{
	
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("이용가능 터미널 생성 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Access Terminal Generator..."));
	}
	KAccessTerminalGeneratorDlg*  pDlg = (KAccessTerminalGeneratorDlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}


void KAccessTerminalGeneratorDlg::ReceiveStatusMessageFromDll( void* pParam1, int nStatus, char* strMsg )
{
	AddStatusMessage(CString(strMsg));
}


void KAccessTerminalGeneratorDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


CString KAccessTerminalGeneratorDlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s\\%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_ACCESSTERMINAL_FOLDER_NAME, a_strFileName);

	return strSavePath;
}

void KAccessTerminalGeneratorDlg::Run( void )
{
	try
	{
        CString strModelFolder(_T(""));
		strModelFolder.Format(_T("%s%s"), m_pTarget->GetLocation(),DLL_READ_WRITE_PATH);
		CreateDirectory(strModelFolder, NULL);
		CreateDirectory(ConvertSavePath(_T("")), NULL);
		
		CString strCodePath(_T(""));
		strCodePath.Format(_T("%s\\%s"),DLL_MODEL_FOLDER_NAME, INTERMODAL_ACCESSTERMINAL_FOLDER_NAME);

		try 
		{
			KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget, strCodePath);
		} catch (...) {
			TxLogDebugWarning();
			ThrowException(_T("DefineNetworkType.arg"));
		}

		CreateArgument();

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

		m_nResultCode = DLL_UNEXPECTED_ERROR;
		CString strWindowName;
		GetWindowText(strWindowName);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("이용가능 터미널 생성 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run Access Terminal Generator Module..."));
		}

		KLauncher::Run(strWindowName, 17);

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("이용가능 터미널 생성 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("Access Terminal Generator Module Complete."));
			}
			TxLogDebug(_T("Model Run Ok.."));
		}
		else if (DLL_USER_STOPPING == m_nResultCode || QBicSimpleProgressThread::IsUserStopped())
		{
            m_nResultCode = DLL_USER_STOPPING;
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
			AddStatusMessage(_T("> Access Terminal Generator Dll Not Found ..."));
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
			ThrowException(_T("Access Terminal End(Error)"));
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
		}

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			m_nResultCode = DLL_USER_STOPPING;
			ThrowException(_T(">User Stop.."));
		}

		std::vector<TAccessTerminalNodeInfo> vecTerminalNodeInfo;

		ReadOutputFile(vecTerminalNodeInfo);
		KDBaseInterModal::InsertAcessTerminalNodeSetting(m_pTarget, vecTerminalNodeInfo);
		SaveInfo2DB();

		m_pTarget->Tables()->FindTable(TABLE_ACCESS_TERMINAL_NODE)->Notify();
	}
	catch (KExceptionPtr ex)
	{
		m_nResultCode = DLL_UNEXPECTED_ERROR;
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		m_nResultCode = DLL_UNEXPECTED_ERROR;
		TxLogDebugException();
	}
}

void KAccessTerminalGeneratorDlg::CreateArgument( void )
{
	try
	{
		ImChampDir::SetCurrentDirectoryToApp();

		ClearFiles();

		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(T_ARG_ACCESSTERMINALGENERATOR, nOpenMode);

		ArumentWriteProjectName(of);
		ArumentWriteTypeDist(of);
		ArumentWriteFolder(of);

		of.Close();

		if ( !KDBaseInterModal::InterExportDB2NodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("node.dat")) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		}

		KIOColumns oODColumnCollection;
		GetSelectODColumnCollection(oODColumnCollection);

		if ( !KDBaseInterModal::InterExportDB2PurposeODSum(m_pTarget->GetDBaseConnection(), oODColumnCollection, ConvertSavePath(_T("Purpose_OD.dat"))))
		{
			ThrowException(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KDBaseInterModal::InterExportDB2TerminalScheulFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("TerminalSchedule.dat"))))
		{
			ThrowException(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
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

void KAccessTerminalGeneratorDlg::ClearFiles( void )
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove((CStringA)T_ARG_ACCESSTERMINALGENERATOR);
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TERMINAL_SCHEDULE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_PURPOSEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_ACCESSTERMINAL_NODE));
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}

void KAccessTerminalGeneratorDlg::ArumentWriteProjectName( CStdioFileEx& of )
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

void KAccessTerminalGeneratorDlg::ArumentWriteTypeDist(CStdioFileEx& of)
{
	try
	{
		m_mapSaveTerminalType.clear();
		CString strTerminalList(_T("Termianl_List\t5\r\n"));
		CString strOutTC(_T("TComplex_Dist\t4\t0.0\r\n"));
		CString strOutIRR(_T("Rail_Dist\t5\t0.0\r\n"));
		CString strOutXBUS(_T("XBus_Dist\t6\t0.0\r\n"));
		CString strOutAIR(_T("AIR_Dist\t8\t0.0\r\n"));
		CString strOutMARIN(_T("Marine_Dist\t9\t0.0\r\n"));

		CXTPReportRecords*          pRecords = m_wndReportCtrlTerminalType.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;

		int nRecordCount = pRecords->GetCount();

		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(_0_COLUMN_ID);
			int nCodeID		= (int)pItemNum->GetValue();

			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_DISTANCE);
			double dBDist	= (double)pItemNum->GetValue();

			CString strWriteName(_T(""));
			if (TRANSITCOMPLEX == nCodeID)
			{
				strWriteName = _T("TComplex_Dist");
				strOutTC.Format(_T("%s\t4\t%0.1f\r\n"), strWriteName, dBDist);
			}
			else if (INTER_REGIONAL_RAILSTATION == nCodeID)
			{
				strWriteName = _T("Rail_Dist");
				strOutIRR.Format(_T("%s\t5\t%0.1f\r\n"), strWriteName, dBDist);
			}
			else if (BUS_TRMINAL == nCodeID)
			{
				strWriteName = _T("XBus_Dist");
				strOutXBUS.Format(_T("%s\t6\t%0.1f\r\n"), strWriteName, dBDist);
			}
			else if (AIR_TERMIANL == nCodeID)
			{
				strWriteName = _T("Air_Dist");
				strOutAIR.Format(_T("%s\t8\t%0.1f\r\n"), strWriteName, dBDist);
			}
			else if (MARIN_PORT == nCodeID)
			{
				strWriteName = _T("Marine_Dist");
				strOutMARIN.Format(_T("%s\t9\t%0.1f\r\n"), strWriteName, dBDist);
			}

			m_mapSaveTerminalType.insert(std::make_pair(nCodeID, dBDist));
		}

		of.WriteString(strTerminalList);
		of.WriteString(strOutTC);
		of.WriteString(strOutIRR);
		of.WriteString(strOutXBUS);
		of.WriteString(strOutAIR);
		of.WriteString(strOutMARIN);
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

bool KAccessTerminalGeneratorDlg::GetSelectODColumnCollection( KIOColumns& a_ColumnCollection )
{
	try
	{
		std::map<CString, int>   mapPurposeOD;
		KDBaseInterModal::GetPurposeODData(m_pTarget, mapPurposeOD);

		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PURPOSE_OD);
		const KIOColumns* pIOColumns = pTable->Columns();
		int nColumnCount = pIOColumns->ColumnCount();
		KIOColumn* pUColumn = nullptr;

		AutoType iter = mapPurposeOD.begin();
		AutoType end  = mapPurposeOD.end();
		while(iter != end)
		{
			CString strColumnName = iter->first;
			KIOColumn* pColumn = pIOColumns->GetColumn(strColumnName);
			if (pColumn != nullptr)
			{
				a_ColumnCollection.AddNewColumn(pColumn);
				m_mapSelectPurposeOD.insert(std::make_pair(pColumn->Name(), iter->second));
			}
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

BOOL KAccessTerminalGeneratorDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
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

bool KAccessTerminalGeneratorDlg::ReadOutputFile( std::vector<TAccessTerminalNodeInfo>& a_vecTerminalNodeInfo )
{
	std::ifstream ifs( ConvertSavePath(T_DAT_ACCESSTERMINAL_NODE), std::ios::binary );

	try
	{
		if (ifs)
		{
			TxLogDebug(_T("--- start insert result"));
			a_vecTerminalNodeInfo.clear();
			while(! ifs.eof())
			{
				TAccessTerminalNodeInfo oTerminalNodeInfo;
				ifs.read( reinterpret_cast<char*>( &(oTerminalNodeInfo.nxZoneID)),       sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oTerminalNodeInfo.nType)),			 sizeof(int) );
				ifs.read( reinterpret_cast<char*>( &(oTerminalNodeInfo.nxTerminalID)),   sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oTerminalNodeInfo.dLength)),		 sizeof(double) );

				if(ifs.eof())break;
				a_vecTerminalNodeInfo.push_back(oTerminalNodeInfo);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

void KAccessTerminalGeneratorDlg::SaveInfo2DB()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDelDistanceInfo(_T(""));
		strDelDistanceInfo.Format(_T("delete from %s"), TABLE_ACCESS_TERMINAL_INFO);
		spDBaseConnection->ExecuteUpdate(strDelDistanceInfo);

		AutoType iterDist = m_mapSaveTerminalType.begin();
		AutoType endDist  = m_mapSaveTerminalType.end();
		while(iterDist != endDist)
		{
			CString strSQL(_T(""));
			strSQL.Format(_T("insert Into %s(type, boundary_dist) values (%d, %f)"), TABLE_ACCESS_TERMINAL_INFO, iterDist->first, iterDist->second);
			spDBaseConnection->ExecuteUpdate(strSQL);
			iterDist++;
		}

		CString strDelSelOD(_T(""));
		strDelSelOD.Format(_T("delete from %s"), TABLE_INTER_PURPOSE_OD_INFO);
		spDBaseConnection->ExecuteUpdate(strDelSelOD);

		AutoType iterOD = m_mapSelectPurposeOD.begin();
		AutoType endOD  = m_mapSelectPurposeOD.end();
		while(iterOD != endOD)
		{
			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s(purpose_id, purpose_column) values (%d, '%s')"), TABLE_INTER_PURPOSE_OD_INFO, iterOD->second, iterOD->first);
			spDBaseConnection->ExecuteUpdate(strSQL);
			iterOD++;
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw -1;
	}
}

void KAccessTerminalGeneratorDlg::ArumentWriteFolder( CStdioFileEx& of )
{
	CString strFolder;
	try
	{
		strFolder.Format(_T("%s"), m_pTarget->GetLocation());
		strFolder = strFolder + DLL_READ_WRITE_PATH + INTERMODAL_ACCESSTERMINAL_FOLDER_NAME;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);
	}
	catch(...)
	{
		return;
	}

	return;
}


void KAccessTerminalGeneratorDlg::OnBnClickedDefault()
{
	try
	{
		AutoType iter = m_mapDefaultAccessTerminal.begin();
		AutoType end  = m_mapDefaultAccessTerminal.end();

		if (iter != end)
		{
			AutoType iterDefault = m_mapDefaultAccessTerminal.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if (iterDefault != end)
			{
				DefaultUpdateReportControlTType(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				DefaultUpdateReportControlTType(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}
		else
		{
			UpdateReportControlTType();
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
