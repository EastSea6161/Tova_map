// DefineRegionMode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefineRegionMode.h"
#include "afxdialogex.h"
#include "Target.h"
#include "KExRecordItem.h"
#include "CustomReportRecordItem.h"
#include "Scenario.h"
#include "Project.h"


enum
{
	_0_COLUMN_SELECT = 0,
	_1_COLUMN_TRANSITTYPE,
	_2_COLUMN_NAME,
	_3_COLUMN_TERMINALTYPE
};

// KDefineRegionMode 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefineRegionMode, KResizeDialogEx)

KDefineRegionMode::KDefineRegionMode(KTarget* a_pTarget, bool a_bUseImport, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefineRegionMode::IDD, pParent),
	m_pTarget(a_pTarget),
	m_bUseImport(a_bUseImport),
	m_cSeparator(_T(','))
{

}

KDefineRegionMode::~KDefineRegionMode()
{
}

void KDefineRegionMode::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wmReportControl);
}

BEGIN_MESSAGE_MAP(KDefineRegionMode, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LINK, &KDefineRegionMode::OnBnClickedButtonAddLink)
	ON_BN_CLICKED(IDC_BUTTON_DEL_LINK, &KDefineRegionMode::OnBnClickedButtonDelLink)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &KDefineRegionMode::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &KDefineRegionMode::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDOK, &KDefineRegionMode::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefineRegionMode::OnBnClickedCancel)
END_MESSAGE_MAP()


// KDefineRegionMode 메시지 처리기입니다.


BOOL KDefineRegionMode::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	try
	{
		ResizeComponent();

		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		KReportCtrlSetting::Default(m_wmReportControl, TRUE);
		
		GetTerminalTypeCode();
		GetRegionModeDefineDB();
		InitReportHeader();
		UpdateReportControl();

		CButton* pOKButton = (CButton*)GetDlgItem(IDOK);
		CButton* pCANCLEButton = (CButton*)GetDlgItem(IDCANCEL);
		if (true == m_bUseImport)
		{
			pCANCLEButton->EnableWindow(FALSE);
		}

		CString strCaption;
		strCaption.Format(_T("%s"), m_pTarget->Caption());
		SetDlgItemText(IDC_STATIC_TARGETYEAR, strCaption);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return FALSE;
	}
	catch(...)
	{
		TxLogDebugException();
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefineRegionMode::ResizeComponent()
{
	try
	{
		SetResize(IDC_REPORT			, SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
		SetResize(IDC_BUTTON_ADD_LINK	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_BUTTON_DEL_LINK	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

		SetResize(IDC_BUTTON_EXPORT	    , SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDC_BUTTON_IMPORT   	, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC4			, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
		SetResize(IDOK					, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL				, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
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

void KDefineRegionMode::GetTerminalTypeCode()
{
	try
	{
		m_mapRegionMode.clear();

		KCodeManager* pCodeMgr		= m_pTarget->CodeManager();
		KCodeGroup*   pTTypeCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
		pTTypeCodeGroup->GetCodes(m_mapRegionMode);
		
		std::set<int> setDBInCode;
		GetRegionModeValueInTerminalDB(setDBInCode);

		AutoType iter = setDBInCode.begin();
		AutoType end  = setDBInCode.end();
		while(iter != end)
		{
			int nCode = *iter;

			AutoType find = m_mapRegionMode.find(nCode);
			AutoType fend = m_mapRegionMode.end();
			if (find == fend)
			{
				m_mapRegionMode.insert(std::make_pair(nCode, _T(" - ")));
			}
			++iter;
		}

		m_vecTerminalType.clear();
		m_vecTerminalType.push_back(TRANSITCOMPLEX);
		m_vecTerminalType.push_back(INTER_REGIONAL_RAILSTATION);
		m_vecTerminalType.push_back(BUS_TRMINAL);
		m_vecTerminalType.push_back(INTER_REGIONAL_BUS_TRANSFER);
		m_vecTerminalType.push_back(AIR_TERMIANL);
		m_vecTerminalType.push_back(MARIN_PORT);
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

void KDefineRegionMode::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T("Select"), 40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("선택"));
		}

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_TRANSITTYPE, _T("Mode"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("타입"));
		}

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Name"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("수단 명칭"));
		}

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_TERMINALTYPE, _T("Terminal Type"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("타입 설정"));
		}
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;


		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

		int nCount = m_vecTerminalType.size();
		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);
		for (int i = 0; i < nCount; i++)
		{
			int nCode = m_vecTerminalType[i];
			CString strCaption = pCodeGroup->SingleCodeValue(nCode);
			pEditOptions->AddConstraint(strCaption, nCode);
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

void KDefineRegionMode::UpdateReportControl()
{
	try
	{
		KCodeManager* pCodeMgr		= m_pTarget->CodeManager();
		KCodeGroup*	  pCodeGroup	= pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);

		AutoType iter = m_mapRegionMode.begin();
		AutoType end  = m_mapRegionMode.end();
		while(iter != end)
		{
			int nRegionCode = iter->first;

			CXTPReportRecord*		pRecord = m_wmReportControl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			CString strCode(_T(""));
			strCode.Format(_T("%d"), nRegionCode);
			CString strCodeName = pCodeGroup->SingleCodeValue(nRegionCode);

			std::set<int> setDBInCode;
			GetRegionModeValueInTerminalDB(setDBInCode);

			AutoType findCode = setDBInCode.find(nRegionCode);
			AutoType endCode  = setDBInCode.end();

			if (findCode != endCode)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Used")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pItem->SetCaption(_T("씀"));
				}
			}
			else
			{
				pItem = pRecord->AddItem(new KExRecordItemCheck());
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCode));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR)nRegionCode);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCodeName));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CCustomReportRecordItem);

			AutoType iterfind = m_mapResionMathingCode.find(nRegionCode);
			AutoType endFind  = m_mapResionMathingCode.end();
			if (iterfind != endFind)
			{
				pItem->SetItemData(iterfind->second);
			}
			else
			{
				pItem->SetItemData(BUS_TRMINAL);
			}

			++iter;
		}

		m_wmReportControl.Populate();
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

void KDefineRegionMode::GetRegionModeDefineDB()
{
	try
	{
		m_mapResionMathingCode.clear();

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format(_T("Select regional_mode, Terminal_type From %s "), TABLE_REGIONAL_MODE_DEFINE);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			int nRegionalCode = spResultSet->GetValueInt(0);
			int nTerminalCode = spResultSet->GetValueInt(1);
			m_mapResionMathingCode.insert(std::make_pair(nRegionalCode, nTerminalCode));
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

void KDefineRegionMode::OnBnClickedButtonAddLink()
{
	int nTotalRowCount = m_wmReportControl.GetRows()->GetCount();

	CXTPReportRecord*     pRecord = m_wmReportControl.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem   = NULL;

	pItem = pRecord->AddItem(new CXTPReportRecordItem());
	pItem->HasCheckbox(TRUE);
	pItem->SetAlignment(xtpColumnIconCenter);

	int nMaxCode(0);

	GetMaxCode(nMaxCode);

	nMaxCode = nMaxCode + 1;
	CString strCode(_T(""));
	strCode.Format(_T("%d"), nMaxCode);

	pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem	= pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem = pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetItemData(BUS_TRMINAL);

	m_wmReportControl.Populate();

	nTotalRowCount = m_wmReportControl.GetRows()->GetCount();
	CXTPReportRow* pRow = m_wmReportControl.GetRows()->GetAt(nTotalRowCount - 1);

	XTP_REPORTRECORDITEM_ARGS args(&m_wmReportControl, pRow, m_wmReportControl.GetColumns()->GetAt(_2_COLUMN_NAME));
	m_wmReportControl.EditItem(&args);
}


void KDefineRegionMode::OnBnClickedButtonDelLink()
{
	std::vector<CXTPReportRecord*> vecRecrod;
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItem*        pExtItemCheck = nullptr;

	try
	{
		pRecords			= m_wmReportControl.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pExtItemCheck    = pRecord->GetItem(_0_COLUMN_SELECT);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
			{
				vecRecrod.push_back(pRecord);
			}
		}

		int nvecCount = vecRecrod.size();
		for (int ivec = 0; ivec < nvecCount; ivec++)
		{
			pRecord = vecRecrod[ivec];
			m_wmReportControl.GetRecords()->RemoveRecord(pRecord);
			m_wmReportControl.Populate();
			pRecord = nullptr;
		}

		if (nvecCount == 0)
		{
			AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
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

void KDefineRegionMode::GetMaxCode( int& a_nMaxCode )
{
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItemText*    pItemText	   = nullptr;

	try
	{
		pRecords      = m_wmReportControl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemText   = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_TRANSITTYPE);
			CString strValue = pItemText->GetValue();
			int nUserCode = _ttoi(strValue);

			if (nUserCode > a_nMaxCode)
			{
				a_nMaxCode = nUserCode;
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

void KDefineRegionMode::OnBnClickedOk()
{
	try
	{
		UpdateResultCode();
		AfxMessageBox(_T("적용되었습니다."));

		KIOTable* pScheduleTable = m_pTarget->Tables()->FindTable(TABLE_TERMINAL_SCHEDULE);
		pScheduleTable->Notify();
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
		AfxMessageBox(_T("Error.."));
		return;
	}

	KResizeDialogEx::OnOK();
}

void KDefineRegionMode::UpdateResultCode()
{
	try
	{
		CXTPReportRecords*           pRecords      = m_wmReportControl.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportColumn*			 pColumn	   = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;
		CXTPReportRecordItemNumber*	 pItemNum	   = nullptr;

		std::map<int, CString> mapRegionModeCode;
		std::map<int, int>mapRegionMatching;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItemText		= (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_TRANSITTYPE);
			CString strCode = pItemText->GetValue();
			int nRegionModeCode		= _ttoi(strCode);
			if (false == QBicStringChecker::IsUnSignedInteger(strCode))
			{
				CString strMsg;
				//strMsg.Format(_T("User Code Key : %s 의 값이 잘못되었습니다."), strCode);
                strMsg.Format(_T("Mode : '%s' 의 값이 잘못되었습니다. \n0보다 큰 값을 입력해 주세요"), strCode);
				ThrowException(strMsg);
			}

			pItemText		= (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_NAME);
			CString strCodeName = pItemText->GetValue();
			if (strCodeName == _T("") || strCodeName == _T("'"))
			{
				CString strMsg;
				//strMsg.Format(_T("User Code Key : %s 의 이름이 없습니다."), strCode);
                strMsg.Format(_T("Mode : '%s' 의 이름이 없습니다."), strCode);
				ThrowException(strMsg);
			}
			else if (strCodeName.Find(_T(" ")) != -1)
			{
				CString strMsg;
				//strMsg.Format(_T("User Code Key : %s에 공백이 있습니다."), strCode);
                strMsg.Format(_T("Mode : '%s' 의 이름에 공백이 포함되어 있습니다."), strCode);
				ThrowException(strMsg);
			}

			pItem = pRecord->GetItem(_3_COLUMN_TERMINALTYPE);
			int nTerminalCode = pItem->GetItemData();

			mapRegionModeCode.insert(std::make_pair(nRegionModeCode, strCodeName));
			mapRegionMatching.insert(std::make_pair(nRegionModeCode, nTerminalCode));
		}
        
		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup* pRegionCodeGroup	= pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
		pRegionCodeGroup->RemoveAllCode();

		AutoType iterCode = mapRegionModeCode.begin();
		AutoType endCode  = mapRegionModeCode.end();
		while(iterCode != endCode)
		{
			int	nCode		= iterCode->first;
			CString strCode = iterCode->second;
			pRegionCodeGroup->AddCode(nCode, strCode);
			iterCode++;
		}

		UpdateRegionModeTable(mapRegionMatching);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ThrowException(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		ThrowException(_T("Error"));
	}
}

void KDefineRegionMode::UpdateRegionModeTable(std::map<int, int> a_mapRegionMatting)
{
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strDelSQL = _T("");
		strDelSQL.Format(_T("Delete From %s"), TABLE_REGIONAL_MODE_DEFINE);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		AutoType iter = a_mapRegionMatting.begin();
		AutoType end  = a_mapRegionMatting.end();
		while(iter != end)
		{
			CString strSQL(_T(""));
			strSQL.Format(_T(" Insert Into %s(regional_mode , Terminal_type) Values (%d, %d)"), 
				TABLE_REGIONAL_MODE_DEFINE, iter->first, iter->second);
			spDBaseConnection->ExecuteUpdate(strSQL);
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ThrowException(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		ThrowException(_T("SQL Error"));
	}
}

void KDefineRegionMode::GetRegionModeValueInTerminalDB( std::set<int>& a_setCode )
{
	a_setCode.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select mode from %s group by mode "), TABLE_TERMINAL_SCHEDULE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next()) {
			int nCode = spResult->GetValueInt(0);
			a_setCode.insert(nCode);
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

void KDefineRegionMode::OnBnClickedButtonExport()
{
	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle = _T("Export Data");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strExportFile(_T("")); {
			CString strTempFile(_T(""));
			strTempFile = fileDlg.GetPathName();
			strTempFile.Replace(_T(".txt"), _T(""));
			strExportFile.Format(_T("%s.txt"), strTempFile);
		}

		ThreadPara oPara(this);
		if (true) {
			oPara.TBusiness = 0;
			oPara.TKeyStr[0] = strExportFile;
		}

		RThreadInfo.Init();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ThreadRun, &oPara, false, nLang);

		if (RThreadInfo.IsOK() == false) {
			ThrowException(RThreadInfo.ErrorMsg());
		} else {
			AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KDefineRegionMode::OnBnClickedButtonImport()
{
	m_setDetailID.clear();

	try
	{
		TCHAR       szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		CFileDialog filedlg( TRUE, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter );

		KScenario* pScenario   = (KScenario*)m_pTarget->GetParentObject();
		KProject*  pProject    = (KProject*)pScenario->GetParentObject();
		CString    strLocation = pProject->GetLocation();

		if (!strLocation.IsEmpty())
			filedlg.m_ofn.lpstrInitialDir = strLocation;

		if( filedlg.DoModal() != IDOK )
			return;

		CString strImportFile = filedlg.GetPathName();


		ThreadPara oPara(this);
		if (true) {
			oPara.TBusiness  = 1;
			oPara.TKeyStr[0] = strImportFile;
		}

		RThreadInfo.Init();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ThreadRun, &oPara, false, nLang);

		if (RThreadInfo.IsOK() == false) {
			ThrowException(RThreadInfo.ErrorMsg());
		} else {
			UpdateImportData();
			AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

unsigned __stdcall KDefineRegionMode::ThreadRun( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	if (pParameter != nullptr) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KDefineRegionMode* pDlg = (KDefineRegionMode*)pPara->TWindow;

			if (0 == pPara->TBusiness)
				pDlg->Export(pPara);
			else
				pDlg->Import(pPara);
		}
	}

	return 1;
}

void KDefineRegionMode::Export( ThreadPara* pPara )
{
	CString strExportFile = pPara->TKeyStr[0];

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(strExportFile, nOpenMode);

	try
	{
		CString strLine;

		//WriteHeader
		strLine = _T("###Mode, Name, Terminal Type\r\n");
		outFile.WriteString(strLine);

		//WriteData
		CXTPReportRecords* pRecords = m_wmReportControl.GetRecords();
		int                nRecord  = pRecords->GetCount();

		int     nMode(0);
		CString strName(_T(""));
		int     nTerminalType(0);

		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord*           pRecord   = pRecords->GetAt(i);
			CXTPReportRecordItem*       pItem     = nullptr;
			CXTPReportRecordItemText*   pItemText = nullptr;

			pItem         = pRecord->GetItem(1);
			nMode         = (int)pItem->GetItemData();

			pItemText     = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			strName       = pItemText->GetValue();

			pItem         = pRecord->GetItem(3);
			nTerminalType = (int)pItem->GetItemData();

			strLine.Format(_T("%d, %s, %d\r\n"), 
				nMode, strName, nTerminalType);

			outFile.WriteString(strLine);
		}		

		outFile.Close();
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	}
	catch (...)
	{
		outFile.Close();
		TxLogDebugException();
		RThreadInfo.SetErrorFlag(-1, _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}


void KDefineRegionMode::Import( ThreadPara* pPara )
{
	CString strImportFile = pPara->TKeyStr[0];

	m_mapImportData.clear();

	UINT         nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT         nCodePage = 949;
	CStdioFileEx csvFile;

	try
	{
		CString strLine, strTemp, strErr;

		csvFile.SetCodePage(nCodePage);

		if (csvFile.Open(strImportFile, nOpenMode) == FALSE)
			return;

		int nLine(0);

		while (csvFile.ReadString(strLine))
		{
			++nLine;

			strLine.Trim();

			if (strLine.IsEmpty())
				continue;

			strTemp = strLine.Left(1);

			if (strTemp.CompareNoCase(_T("#")) == 0)
				continue;

			strLine.Replace(_T(" "),  _T(""));
			strLine.Replace(_T("\""), _T(""));
			strLine.Replace(_T("'"),  _T(""));

			CSVRow row;
			ParseCSVLineString(strLine, row);

			if (row.size() != 3)
			{
				strErr.Format(_T("Line : %d 데이터 개수 불일치"), nLine);
				ThrowException(strErr);
			}

			TDefineData oDefineData = {};

			oDefineData.nMode         = _ttoi(row[0]);
			oDefineData.strName       = row[1];
			oDefineData.nTerminalType = _ttoi(row[2]);

// 			if (!VerifyImportData(oDefineData, strTemp))
// 			{
// 				strErr.Format(_T("Line : %d %s"), nLine, strTemp);
// 				ThrowException(strErr);
// 			}

			if (m_mapImportData.find(oDefineData.nMode) != m_mapImportData.end())
			{
				strErr.Format(_T("Line : %d 중복된 Mode가 존재합니다."), nLine);
				ThrowException(strErr);
			}

			m_mapImportData.insert(std::make_pair(oDefineData.nMode, oDefineData));
			m_setDetailID.insert(oDefineData.nMode);
		}

		csvFile.Close();
	}
	catch (KExceptionPtr ex)
	{
		csvFile.Close();
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	}
	catch (...)
	{
		csvFile.Close();
		TxLogDebugException();
		RThreadInfo.SetErrorFlag(-1, _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KDefineRegionMode::ParseCSVLineString( CString strLine, CSVRow& row )
{
	bool bQuote = false;
	int nSub = 0;
	CString strRow, strQuote;

	while( AfxExtractSubString( strRow, strLine, nSub++, m_cSeparator ) == TRUE )
	{
		if( true == bQuote )
		{
			strQuote += strRow;
			if( strRow.GetAt( strRow.GetLength() - 1 ) == _T('"') )
			{
				strQuote.Remove( _T('"') );
				strRow = strQuote;
				bQuote = false;
				strQuote = _T("");
				row.push_back(strRow);
			}
		}
		else
		{
			TCHAR chFirst, chLast;
			if( strRow.IsEmpty() == TRUE )
			{
				row.push_back( strRow );
			}
			else if( strRow.GetLength() == 1 )
			{
				chFirst = strRow.GetAt( 0 );
				if( chFirst == _T('"') )
				{
					bQuote = true;
				}
				else
				{
					row.push_back(strRow);
				}
			}
			else
			{
				chFirst = strRow.GetAt( 0 );
				chLast = strRow.GetAt( strRow.GetLength() - 1 );

				if( (chFirst == _T('"')) && (chLast == _T('"')) )
				{
					strRow.Remove( _T('"') );
					row.push_back( strRow );
				}
				else if( chFirst == _T('"') )
				{
					bQuote = true;
					strQuote = strRow;
				}
				else
				{
					row.push_back( strRow );
				}
			}
		}
	}
}


bool KDefineRegionMode::VerifyImportData( TDefineData a_oDefineData, CString& a_strErr )
{
	a_strErr = _T("");

	try
	{
		//Verify Mode
		if (m_mapResionMathingCode.find(a_oDefineData.nMode) == m_mapResionMathingCode.end()) {
			ThrowException(_T("정의되지 않은 ModeID입니다."));
		}

		//Verify TerminalType
		bool bCheckTerminal(false);
		for (size_t i = 0; i < m_vecTerminalType.size(); i++)
		{
			const int nTerminalType = m_vecTerminalType[i];
			if (nTerminalType == a_oDefineData.nTerminalType) {
				bCheckTerminal = true;
				break;
			}
		}

		if (false == bCheckTerminal) {
			ThrowException(_T("정의되지 않은 Terminal Type입니다."));
		}
	}
	catch (KExceptionPtr ex)
	{
		a_strErr = ex->GetErrorMessage();
		return false;
	}
	catch (...)
	{
		a_strErr = _T("데이터에 오류가 존재합니다.");
		return false;
	}

	return true;	
}


void KDefineRegionMode::UpdateImportData( void )
{
	//ReportCtrl 값을 m_mapReportData에 담음
	m_mapReportData.clear();

	try
	{
		int nRecord = m_wmReportControl.GetRecords()->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord*           pRecord  = m_wmReportControl.GetRecords()->GetAt(i);
			CXTPReportRecordItem*       pItem    = nullptr;
			CXTPReportRecordItemText*   pItemText = nullptr;

			TDefineData oDefineData = {};

			pItem = pRecord->GetItem(1);
			oDefineData.nMode = (int)pItem->GetItemData();

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			oDefineData.strName = pItemText->GetValue();

			pItem = pRecord->GetItem(3);
			oDefineData.nTerminalType = (int)pItem->GetItemData();

			m_mapReportData.insert(std::make_pair(oDefineData.nMode, oDefineData));
			m_setDetailID.insert(oDefineData.nMode);

			pItem = pRecord->GetItem(0);

			if (!pItem->GetHasCheckbox())
				m_setUsedDetailID.insert(oDefineData.nMode);
		}

		//ReportCtrl Redraw
		m_wmReportControl.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem	  = nullptr;

		std::set<int>::iterator bSetIter, eSetIter = m_setDetailID.end();

		std::map<int, TDefineData>::iterator fImportIter, fReportIter;

		for (bSetIter = m_setDetailID.begin(); bSetIter != eSetIter; ++bSetIter)
		{
			int nDetailModeID = *bSetIter;

			fImportIter = m_mapImportData.find(nDetailModeID);
			fReportIter = m_mapReportData.find(nDetailModeID);

			pRecord = m_wmReportControl.AddRecord(new CXTPReportRecord);

			if (fImportIter != m_mapImportData.end())
			{
				AddReportData(true, pRecord, fImportIter->second);
			}
			else if (fReportIter != m_mapReportData.end())
			{
				fReportIter = m_mapReportData.find(nDetailModeID);
				AddReportData(false, pRecord, fReportIter->second);			
			}
			else
			{
				ThrowException(_T("unexepcted error..."));
			}
		}

		m_wmReportControl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		ThrowException(ex->GetErrorMessage());
	}
	catch (...)
	{
		ThrowException(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}	
}

void KDefineRegionMode::AddReportData( bool a_bImport, CXTPReportRecord* a_pRecord, TDefineData a_oDefineData )
{
	std::set<int>::iterator fSetIter = m_setUsedDetailID.find(a_oDefineData.nMode);

	CXTPReportRecordItem* pItem = nullptr;

	if (fSetIter != m_setUsedDetailID.end())//Used일 경우
	{
		pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(_T("Used")));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pItem->SetCaption(_T("씀"));
		}
	}
	else
	{
		pItem = a_pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
	}

	int nRegionCode = a_oDefineData.nMode;

	KCodeManager* pCodeMgr		= m_pTarget->CodeManager();
	KCodeGroup*	  pCodeGroup	= pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);

	CString strCode(_T(""));
	strCode.Format(_T("%d"), nRegionCode);

	CString strCodeName(_T(""));
	if (a_bImport) {
		strCodeName = a_oDefineData.strName;
	}
	else {
		strCodeName = pCodeGroup->SingleCodeValue(nRegionCode);
	}

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(strCode));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);
	pItem->SetItemData((DWORD_PTR)nRegionCode);

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(strCodeName));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem = a_pRecord->AddItem(new CCustomReportRecordItem);

	bool bCheckTermial(false);
	for (size_t i= 0; i< m_vecTerminalType.size(); i++) {
		if (m_vecTerminalType[i] == a_oDefineData.nTerminalType) {
			bCheckTermial = true;
			break;
		}
	}

	if (bCheckTermial) {
		pItem->SetItemData(a_oDefineData.nTerminalType);
	} else {
		pItem->SetItemData(BUS_TRMINAL);
	}

}

void KDefineRegionMode::OnBnClickedCancel()
{
	if (true == m_bUseImport)
	{
		AfxMessageBox(_T("대중교통 데이터 불러오기를 수행 한 후 대중교통 비용 설정을 해야 합니다."));
		return;
	}
	KResizeDialogEx::OnCancel();
}
