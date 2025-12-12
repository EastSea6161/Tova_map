// PA2ODModelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PA2ODModelDlg.h"
#include "afxdialogex.h"
#include "Launcher.h"
#include "KUIDefPA2OD.h"
using namespace UIPA2OD;

#include "PA2ODInfo.h"
#include "PA2ODPAImport1Dlg.h"
#include "PA2ODPAImport2Dlg.h"
#include "PA2ODParamImport1Dlg.h"
#include "PA2ODParamImport2Dlg.h"
#include "PA2ODOutputDesignDlg.h"

#include "PurposeInfoCollection.h"
#include "Purpose.h"

#include "Target.h"

#include "KxBulkDbase.h"
#include "ImChampDir.h"


// KPA2ODModelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPA2ODModelDlg, KDialogEx)

KPA2ODModelDlg::KPA2ODModelDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KPA2ODModelDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{
	m_emPA2ODReturnType = KEMPA2ODNotIncludeEachReturn;
}

KPA2ODModelDlg::~KPA2ODModelDlg()
{
}

void KPA2ODModelDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_BACK, 	m_uiBackGround);
	DDX_Control(pDX, IDC_STATIC_MODEL, 	m_uiLblModel);
	DDX_Control(pDX, IDC_BTN_DEFINE,	m_uiBtnSelect);
	DDX_Control(pDX, IDC_BTN_PA,		m_uiBtnPA);
	DDX_Control(pDX, IDC_BTN_PARAM,		m_uiBtnParam);
	DDX_Control(pDX, IDC_BTN_OUTPUT, 	m_uiBtnOutputDesign);
	DDX_Control(pDX, IDC_STATIC_ARROW1,	m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2,	m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3,	m_uiArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW4,	m_uiArrow4);
}


BEGIN_MESSAGE_MAP(KPA2ODModelDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BTN_DEFINE, &KPA2ODModelDlg::OnBnClickedBtnDefine)
	ON_BN_CLICKED(IDC_BTN_PA, &KPA2ODModelDlg::OnBnClickedBtnImportPA)
	ON_BN_CLICKED(IDC_BTN_PARAM, &KPA2ODModelDlg::OnBnClickedBtnImportParam)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, &KPA2ODModelDlg::OnBnClickedBtnOutput)
	ON_BN_CLICKED(IDOK, &KPA2ODModelDlg::OnBnClickedOk)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// KPA2ODModelDlg 메시지 처리기입니다.

void KPA2ODModelDlg::ClearInputInfoList( void )
{
	std::list<KPA2ODInfo*>::iterator iter, end = m_lstHomeBasedPA2ODInfo.end();

	for (iter = m_lstHomeBasedPA2ODInfo.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* pPA2ODInfo = *iter;
		delete 		pPA2ODInfo;
	}

	std::list<KPA2ODInfo*>::iterator iter2, end2 = m_lstNotHBasedPA2ODInfo.end();

	for (iter2 = m_lstNotHBasedPA2ODInfo.begin(); iter2 != end2; ++iter2)
	{
		KPA2ODInfo* pPA2ODInfo = *iter2;
		delete 		pPA2ODInfo;
	}

	std::list<KPA2ODInfo*>::iterator iter3, end3 = m_lstOutput.end();

	for (iter3 = m_lstOutput.begin(); iter3 != end3; ++iter3)
	{
		KPA2ODInfo* pPA2ODInfo = *iter3;
		delete 		pPA2ODInfo;
	}
}

void KPA2ODModelDlg::InitInputInfoList( void )
{
	ClearInputInfoList();

	KPurposeInfoCollection oPurposeInfos(m_pTarget);
	std::list<KPurpose*>::iterator iter, end = oPurposeInfos.end();

	for (iter = oPurposeInfos.begin(); iter != end; ++iter)
	{
		KPurpose* pPurpose 	   = *iter;
		KPurpose* pNewPurpose  = new KPurpose(
			pPurpose->PurposeGroup(), pPurpose->PurposeID(), pPurpose->PurposeName());
		KPA2ODInfo* pPA2ODInfo = new KPA2ODInfo();

		pPA2ODInfo->Purpose(pNewPurpose);
		m_lstHomeBasedPA2ODInfo.push_back(pPA2ODInfo);
	}

	KPurposeInfoCollection oPurposeInfos2(m_pTarget);
	std::list<KPurpose*>::iterator iter2, end2 = oPurposeInfos2.end();

	for (iter2 = oPurposeInfos2.begin(); iter2 != end2; ++iter2)
	{
		KPurpose* pPurpose 	   = *iter2;
		KPurpose* pNewPurpose  = new KPurpose(
			pPurpose->PurposeGroup(), pPurpose->PurposeID(), pPurpose->PurposeName());
		KPA2ODInfo* pPA2ODInfo = new KPA2ODInfo();

		pPA2ODInfo->Purpose(pNewPurpose);
		m_lstNotHBasedPA2ODInfo.push_back(pPA2ODInfo);
	}

	KPurposeInfoCollection oPurposeInfos3(m_pTarget);
	std::list<KPurpose*>::iterator iter3, end3 = oPurposeInfos3.end();

	for (iter3 = oPurposeInfos3.begin(); iter3 != end3; ++iter3)
	{
		KPurpose* pPurpose 	   = *iter3;
		KPurpose* pNewPurpose  = new KPurpose(
			pPurpose->PurposeGroup(), pPurpose->PurposeID(), pPurpose->PurposeName());
		KPA2ODInfo* pPA2ODInfo = new KPA2ODInfo();

		pPA2ODInfo->Purpose(pNewPurpose);
		m_lstOutput.push_back(pPA2ODInfo);
	}
}

void KPA2ODModelDlg::InitDialogUI( void )
{
	UINT nID_DISTRIBUTION_MODEL = 90915;
	XTPImageManager()->SetIcons(IDB_DISTRIBUTION_MODEL, &nID_DISTRIBUTION_MODEL, 1, 0);

	UINT nID_MODECHOICE_OUTPUT 	= 90927;
	XTPImageManager()->SetIcons(IDB_MODECHOICE_OUTPUT, &nID_MODECHOICE_OUTPUT, 1, 0); 

	//Background
	m_uiBackGround.MoveWindow(9, 5, 595, 330);
	m_uiBackGround.SetMarkupText(UIXAML_PA2OD_BACKIMAGE);

	//Model
	m_uiLblModel.SetMarkupText(UIXAML_PA2OD_MODEL);

	//Select
	m_uiBtnSelect.EnableMarkup(TRUE);
	m_uiBtnSelect.SetWindowText(UIXAML_PA2OD_SELECT_DEFAULT);

	//PA Data
	m_uiBtnPA.EnableMarkup(TRUE);
	m_uiBtnPA.SetWindowText(UIXAML_PA2OD_PA_FALSE);
	m_uiBtnPA.EnableWindow(FALSE);

	//Parameter
	m_uiBtnParam.EnableMarkup(TRUE);
	m_uiBtnParam.SetWindowText(UIXAML_PA2OD_PARAM_FALSE);
	m_uiBtnParam.EnableWindow(FALSE);

	//Output Design
	m_uiBtnOutputDesign.EnableMarkup(TRUE);
	m_uiBtnOutputDesign.SetWindowText(UIXAML_PA2OD_OUTPUT_FALSE);
	m_uiBtnOutputDesign.EnableWindow(FALSE);

	m_uiArrow1.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB);
	m_uiArrow2.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);
	m_uiArrow3.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT);
	m_uiArrow4.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);

	SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1);
}

BOOL KPA2ODModelDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitInputInfoList();

	InitDialogUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void KPA2ODModelDlg::OnBnClickedBtnDefine()
{
	KPA2ODDefinePADlg oDlg(m_lstHomeBasedPA2ODInfo, m_lstNotHBasedPA2ODInfo);
	oDlg.PA2ODHomeBasedReturnType(m_emPA2ODReturnType);
	
	if (IDOK == oDlg.DoModal())
	{
		m_emPA2ODReturnType = oDlg.PA2ODHomeBasedReturnType();

		m_uiBtnSelect.SetWindowText(UIXAML_PA2OD_SELECT_SELECTED);

		m_uiBtnPA.SetWindowText(UIXAML_PA2OD_PA_DEFAULT);
		m_uiBtnPA.EnableWindow(TRUE);
	}
		
}


void KPA2ODModelDlg::OnBnClickedBtnImportPA()
{
	CString strFileName(_T(""));
	BOOL	bContainHeader(false);
	int		nSeparatorType(0);
	int		nEncodingType(0);

	while (TRUE)
	{
		KPA2ODPAImport1Dlg oDlg(bContainHeader, nSeparatorType, nEncodingType, strFileName);

		if (IDCANCEL == oDlg.DoModal())
			return;

		bContainHeader = oDlg.ContainHeader();
		nSeparatorType = oDlg.SeparatorType();
		nEncodingType  = oDlg.EncodingType();
		strFileName    = oDlg.FileName();

		TCHAR tcSeparator;
		switch (nSeparatorType)
		{
		case 0 : tcSeparator = _T(',');
			break;
		case 1 : tcSeparator = _T(';');
			break;
		case 2 : tcSeparator = _T('|');
			break;
		case 3 : tcSeparator = _T('	'); 
			break;
		default: ASSERT(FALSE);
			break;
		}

		KEMCSVEncoding emCSVEncoding;
		switch( nEncodingType )
		{
		case 0 : emCSVEncoding = KEMCSVEncodingANSI; 
			break;
		case 1 : emCSVEncoding = KEMCSVEncodingUTF8; 
			break;
		case 2 : emCSVEncoding = KEMCSVEncodingUTF16LE; 
			break;
		default : ASSERT( FALSE ); 
			break;
		}
		//#2
		KPA2ODPAImport2Dlg oDlg2(bContainHeader, emCSVEncoding, tcSeparator, strFileName);
		oDlg2.HomeBasedInfo(m_lstHomeBasedPA2ODInfo);
		oDlg2.NotHBasedInfo(m_lstNotHBasedPA2ODInfo);

		if (IDCANCEL == oDlg2.DoModal())
			return;

		if (oDlg2.GoBack())
			continue;

		break;
	}
	m_uiBtnPA.SetWindowText(UIXAML_PA2OD_PA_SELECTED);

	m_uiBtnParam.SetWindowText(UIXAML_PA2OD_PARAM_DEFAULT);
	m_uiBtnParam.EnableWindow(TRUE);
}


void KPA2ODModelDlg::OnBnClickedBtnImportParam()
{
	CString strFileName(_T(""));
	BOOL	bContainHeader(false);
	int		nSeparatorType(0);
	int		nEncodingType(0);

	while (TRUE)
	{
		KPA2ODParamImport1Dlg oDlg(bContainHeader, nSeparatorType, nEncodingType, strFileName);

		if (IDCANCEL == oDlg.DoModal())
			return;

		bContainHeader = oDlg.ContainHeader();
		nSeparatorType = oDlg.SeparatorType();
		nEncodingType  = oDlg.EncodingType();
		strFileName    = oDlg.FileName();

		TCHAR tcSeparator;
		switch (nSeparatorType)
		{
		case 0 : tcSeparator = _T(',');
			break;
		case 1 : tcSeparator = _T(';');
			break;
		case 2 : tcSeparator = _T('|');
			break;
		case 3 : tcSeparator = _T('	'); 
			break;
		default: ASSERT(FALSE);
			break;
		}

		KEMCSVEncoding emCSVEncoding;
		switch( nEncodingType )
		{
		case 0 : emCSVEncoding = KEMCSVEncodingANSI; 
			break;
		case 1 : emCSVEncoding = KEMCSVEncodingUTF8; 
			break;
		case 2 : emCSVEncoding = KEMCSVEncodingUTF16LE; 
			break;
		default : ASSERT( FALSE ); 
			break;
		}
		//#2
		KPA2ODParamImport2Dlg oDlg2(bContainHeader, emCSVEncoding, tcSeparator, strFileName);
		oDlg2.HomeBasedInfo(m_lstHomeBasedPA2ODInfo);
		oDlg2.NotHBasedInfo(m_lstNotHBasedPA2ODInfo);

		if (IDCANCEL == oDlg2.DoModal())
			return;

		if (oDlg2.GoBack())
			continue;

		break;
	}
	m_uiBtnParam.SetWindowText(UIXAML_PA2OD_PARAM_SELECTED);
	m_uiBtnOutputDesign.SetWindowText(UIXAML_PA2OD_OUTPUT_DEFAULT);
	m_uiBtnOutputDesign.EnableWindow(TRUE);
}


void KPA2ODModelDlg::OnBnClickedBtnOutput()
{
	std::list<KPA2ODInfo*>::iterator homeIter, homeEnd = m_lstHomeBasedPA2ODInfo.end();

	for (homeIter = m_lstHomeBasedPA2ODInfo.begin(); homeIter != homeEnd; ++homeIter)
	{
		KPA2ODInfo* pPA2ODInfo = *homeIter;

		if (!pPA2ODInfo->Selected())
			continue;

		KPurpose* pGoPurpose = pPA2ODInfo->Purpose();
		KPurpose* pReturnPurpose = pPA2ODInfo->ReturnPurpose();

		std::list<KPA2ODInfo*>::iterator outIter, outEnd = m_lstOutput.end();

		for (outIter = m_lstOutput.begin(); outIter != outEnd; ++outIter)
		{
			KPA2ODInfo* pOutPA2ODInfo = *outIter;
			KPurpose*   pOutPurspoe   = pOutPA2ODInfo->Purpose();

			if (pGoPurpose->PurposeID() == pOutPurspoe->PurposeID())
				pOutPA2ODInfo->Selected(true);

			if (pReturnPurpose != NULL && pReturnPurpose->PurposeID() == pOutPurspoe->PurposeID())
				pOutPA2ODInfo->Selected(true);
		}
	}

	std::list<KPA2ODInfo*>::iterator notHIter, notHEnd = m_lstNotHBasedPA2ODInfo.end();

	for (notHIter = m_lstNotHBasedPA2ODInfo.begin(); notHIter != notHEnd; ++notHIter)
	{
		KPA2ODInfo* pPA2ODInfo = *notHIter;

		if (!pPA2ODInfo->Selected())
			continue;

		KPurpose* pGoPurpose = pPA2ODInfo->Purpose();
		KPurpose* pReturnPurpose = pPA2ODInfo->ReturnPurpose();

		std::list<KPA2ODInfo*>::iterator outIter, outEnd = m_lstOutput.end();

		for (outIter = m_lstOutput.begin(); outIter != outEnd; ++outIter)
		{
			KPA2ODInfo* pOutPA2ODInfo = *outIter;
			KPurpose*   pOutPurspoe   = pOutPA2ODInfo->Purpose();

			if (pGoPurpose->PurposeID() == pOutPurspoe->PurposeID())
				pOutPA2ODInfo->Selected(true);

			if (pReturnPurpose != NULL && pReturnPurpose->PurposeID() == pOutPurspoe->PurposeID())
				pOutPA2ODInfo->Selected(true);
		}
	}

	KPA2ODOutputDesignDlg oDlg(m_pTarget, m_lstOutput);
	oDlg.DoModal();

	m_uiBtnOutputDesign.SetWindowText(UIXAML_PA2OD_OUTPUT_SELECTED);
}

void KPA2ODModelDlg::RunnableCheck( void )
{

}

void KPA2ODModelDlg::ReceiveStatusMessageFromDll( void* pParam, int nStatus, char* strMsg )
{

}


void KPA2ODModelDlg::OnBnClickedOk()
{
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(PA2ODModelThreadCaller, this, false, nLang);



// 	try
// 	{
// 		CreateArguments();
// 
// 		CTime timeStart = CTime::GetCurrentTime();
// 
// 		m_nResultCode = DLL_UNEXPECTED_ERROR;
// 		CString strWindowName;
// 		GetWindowText(strWindowName);
// 
// 		AddStatusMessage(_T(">PA2ODModel Run Start ..."));
// 
// 		KLauncher::Run(strWindowName, 40);
// 
// 		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
// 		{
// 			AddStatusMessage(_T("> PA2ODModel End ..."));
// 			TxLogDebug(_T("Model Run Ok.."));
// 			AfxMessageBox(_T("성공적으로 완료되었습니다."));
// 		}
// 		else if (DLL_USER_STOPPING == m_nResultCode)
// 		{
// 			AddStatusMessage(_T("> PA2ODModel Stop ..."));
// 			TxLogDebug(_T("Model Stop.."));
// 
// 			return;
// 		}
// 		else if (DLL_LOAD_ERROR == m_nResultCode)
// 		{
// 			AddStatusMessage(_T("> Dll Not Found ..."));
// 			TxLogDebug(_T("DLL NOT Found.."));
// 
// 			return;
// 		}
// 		else
// 		{
// 			AddStatusMessage(_T("> PA2ODModel End(Error) ..."));
// 			TxLogDebug(_T("PA2ODModel Run Error.."));
// 
// 			return;
// 		}
// 
// 
// 		
// 		if (NULL != m_hPAtoOD)
// 			::FreeLibrary(m_hPAtoOD);
// 
// 		m_hPAtoOD = ::LoadLibrary(_T("FPATOOD"));
// 
// 		if (NULL == m_hPAtoOD)
// 			ThrowException(_T("DLL이 없습니다."));
// 
// 		PAtoODProcPtr = (FPATOOD)GetProcAddress(m_hPAtoOD, "FPATOOD");
// 
// 		if (NULL == PAtoODProcPtr)
// 		{
// 			::FreeLibrary(m_hPAtoOD);
// 			ThrowException(_T("FPATOOD 함수가 없습니다."));
// 		}
// 
// 		TxLogDebug(_T("Load Dll..."));
// 
// 		CTime timeStart = CTime::GetCurrentTime();
// 
// 		int nResult = PAtoODProcPtr((void*)this, (void*)ReceiveStatusMessageFromDll);
// 
// 		if ( 1 != nResult )
// 			ThrowException(_T("Model Run Error..."));
// 
// 		if (NULL != m_hPAtoOD)
// 			::FreeLibrary(m_hPAtoOD);
// 
// 		KIOColumns rPurposeODColumnCollection;
// 		AddPA2ODResultColumn(rPurposeODColumnCollection);
// 
// 		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
// 
// 		try
// 		{
// 			spDBaseConnection->BeginTransaction();
// 
// 			KIOTables* pIOTables = m_pTarget->Tables();
// 			KIOTable* 		 pODTable 	   = pIOTables->FindTable(TABLE_PURPOSE_OD);
// 			int 			 nColumnCount  = rPurposeODColumnCollection.ColumnCount();
// 
// 			if (0 == nColumnCount)
// 				ThrowException(_T("적용 컬럼 정보가 없습니다."));
// 
// 			std::ifstream ifs( _T("ODout.dat"), std::ios::binary );
// 
// 			if( !ifs )
// 				ThrowException(_T("File Not Found"));
// 
// 			KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
// 			__int64 nxOZoneID, nxDZoneID;
// 
// 			KxBulkDbase oBulkDBase(spDBaseConnection, pODTable, &rPurposeODColumnCollection);
// 
// 			while (!ifs.eof())
// 			{
// 				ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
// 				ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
// 				pRecord->ReadBytes(ifs);
// 
// 				if(ifs.eof())
// 					break;
// 				oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
// 			}
// 			oBulkDBase.DeprecatedFnExecute();
// 			delete pRecord;
// 
// 			CTime timeEnd = CTime::GetCurrentTime();
// 			ApplyResult(spDBaseConnection, 
// 				timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), 
// 				timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"))
// 				);
// 			spDBaseConnection->Commit();
// 		}
// 		catch (KExceptionPtr ex)
// 		{
// 			spDBaseConnection->RollBack();
// 			throw ex;
// 		}
// 		catch (...)
// 		{
// 			spDBaseConnection->RollBack();
// 			throw 1;
// 		}
// 	}
// 	catch (KExceptionPtr ex)
// 	{
// 		AfxMessageBox(ex->GetErrorMessage());
// 	}
// 	catch (...)
// 	{
// 		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
// 	}
}

void KPA2ODModelDlg::CreateArguments( void )
{
	TxLogDebugStartMsg();
	CString strOut;

	ImChampDir::SetCurrentDirectoryToApp();

	CString strFileName = _T("patood.arg");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strFileName, nOpenMode);

	int nPurposeCount = 0;

	std::list<KPA2ODInfo*>::iterator homeIter, homeEnd = m_lstHomeBasedPA2ODInfo.end();

	for (homeIter = m_lstHomeBasedPA2ODInfo.begin(); homeIter != homeEnd; ++homeIter)
	{
		KPA2ODInfo* pPA2ODInfo = *homeIter;

		if (pPA2ODInfo->Selected())
			++nPurposeCount;
	}

	std::list<KPA2ODInfo*>::iterator notHIter, notHEnd = m_lstNotHBasedPA2ODInfo.end();

	for (notHIter = m_lstNotHBasedPA2ODInfo.begin(); notHIter != notHEnd; ++notHIter)
	{
		KPA2ODInfo* pPA2ODInfo = *notHIter;

		if (pPA2ODInfo->Selected())
			++nPurposeCount;
	}

	strOut.Format(_T("nPurpose\t%d\r\n"), nPurposeCount);
	of.WriteString(strOut);

	int nPurposeIndex = 0;
	int nGoIndex, nReturnIndex;
	
	for (homeIter = m_lstHomeBasedPA2ODInfo.begin(); homeIter != homeEnd; ++homeIter)
	{
		KPA2ODInfo* pPA2ODInfo = *homeIter;

		if (!pPA2ODInfo->Selected())
			continue;

		KPurpose* pGOPurpose     = pPA2ODInfo->Purpose();
		KPurpose* pReturnPurpose = pPA2ODInfo->ReturnPurpose();
		
		nGoIndex     = pGOPurpose->PurposeID();
		nReturnIndex = pReturnPurpose->PurposeID();

		strOut.Format(_T("%d\t%d\t%d\r\n"), ++nPurposeIndex, nGoIndex, nReturnIndex);
		of.WriteString(strOut);
	}

	for (notHIter = m_lstNotHBasedPA2ODInfo.begin(); notHIter != notHEnd; ++notHIter)
	{
		KPA2ODInfo* pPA2ODInfo = *notHIter;

		if (!pPA2ODInfo->Selected())
			continue;

		KPurpose* pGOPurpose     = pPA2ODInfo->Purpose();
		KPurpose* pReturnPurpose = pPA2ODInfo->ReturnPurpose();

		nGoIndex     = pGOPurpose->PurposeID();

		if (NULL == pReturnPurpose)
		{
			nReturnIndex = 0;
		}
		else
		{
			nReturnIndex = pReturnPurpose->PurposeID();
		}
		
		strOut.Format(_T("%d\t%d\t%d\r\n"), ++nPurposeIndex, nGoIndex, nReturnIndex);
		of.WriteString(strOut);
	}

	of.Close();
	TxLogDebugEndMsg();
}

void KPA2ODModelDlg::AddPA2ODResultColumn( KIOColumns& a_rODColumnCollection )
{
	a_rODColumnCollection.Clear();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* 		 pODTable 	   = pIOTables->FindTable(TABLE_PURPOSE_OD);

	const KIOColumns* pColumns = pODTable->Columns();

	std::list<KPA2ODInfo*>::iterator iter, end = m_lstOutput.end();

	for (iter = m_lstOutput.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* pPA2ODInfo = *iter;
		KPurpose* pPurpose = pPA2ODInfo->Purpose();
				
		if (!pPA2ODInfo->Selected())
			continue;

		int nColumnCount = pColumns->ColumnCount();

		for (int i = 0 ; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			CString    strCaption = pColumn->Caption();

			if (strCaption.CompareNoCase(pPurpose->PurposeName()) == 0)
			{
				a_rODColumnCollection.AddNewColumn(pColumn);
			}
		}
	}
}

void KPA2ODModelDlg::ApplyResult( KDBaseConPtr spDBaseConnection, 
	CString &a_strStartTime, CString &a_strEndTime )
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables      = m_pTarget->Tables();
	KIOTable* 		 pODTable 	        = pIOTables->FindTable(TABLE_PURPOSE_OD);
	const KIOColumns* pColumns = pODTable->Columns();
	int nColumnCount = pColumns->ColumnCount();

	std::list<KPA2ODInfo*>::iterator iter, end = m_lstOutput.end();

	for (iter = m_lstOutput.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* pOutPA2ODInfo = *iter;
		KPurpose*   pOutPurspoe   = pOutPA2ODInfo->Purpose();

		if (!pOutPA2ODInfo->Selected())
			continue;

		CString strColName;

		for (int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);

			CString strCaption = pColumn->Caption();

			if (strCaption.CompareNoCase(pOutPurspoe->PurposeName()) == 0)
			{
				strColName = pColumn->Name();
			}
		}

		CString strQuery;
		strQuery.Format(
			_T(" Insert Or Replace into Distribution_Model ")
			_T(" (object_group_code, detail_object_id, ")
			_T(" purpose_od_column_name, distribution_run_seq, start_date, end_date)  ")
			_T(" Values( %d, %d, '%s', '%s', '%s', '%s') "),
			pOutPurspoe->PurposeGroup(),  pOutPurspoe->PurposeID(),  strColName,
			_T(" (select ifnull(max(distribution_run_seq)+1, 1) from Distribution_Model) " ),			a_strStartTime, a_strEndTime  );

		spDBaseConnection->ExecuteUpdate(strQuery);

	}
	TxLogDebugEndMsg();
}





unsigned __stdcall KPA2ODModelDlg::PA2ODModelThreadCaller( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("모형 구동 .... "));

	KPA2ODModelDlg* pDlg = (KPA2ODModelDlg*)pParameter->GetParameter();;

	pDlg->RunModel();

	return 0;
}


void KPA2ODModelDlg::RunModel( void )
{
	try
	{
		CreateArguments();

		CTime timeStart = CTime::GetCurrentTime();

		m_nResultCode = DLL_UNEXPECTED_ERROR;
		CString strWindowName;
		GetWindowText(strWindowName);

		AddStatusMessage(_T(">PA2ODModel Run Start ..."));

		KLauncher::Run(strWindowName, 38);

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			AddStatusMessage(_T("> PA2ODModel End ..."));
			TxLogDebug(_T("Model Run Ok.."));
		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
			AddStatusMessage(_T("> PA2ODModel Stop ..."));
			TxLogDebug(_T("Model Stop.."));

			return;
		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			AddStatusMessage(_T("> Dll Not Found ..."));
			TxLogDebug(_T("DLL NOT Found.."));

			return;
		}
		else
		{
			AddStatusMessage(_T("> PA2ODModel End(Error) ..."));
			TxLogDebug(_T("PA2ODModel Run Error.."));

			return;
		}

		KIOColumns rPurposeODColumnCollection;
		AddPA2ODResultColumn(rPurposeODColumnCollection);

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		try
		{
			spDBaseConnection->BeginTransaction();

			KIOTables* pIOTables = m_pTarget->Tables();
			KIOTable* 		 pODTable 	   = pIOTables->FindTable(TABLE_PURPOSE_OD);
			int 			 nColumnCount  = rPurposeODColumnCollection.ColumnCount();

			if (0 == nColumnCount)
				ThrowException(_T("적용 컬럼 정보가 없습니다."));

			std::ifstream ifs( _T("ODout.dat"), std::ios::binary );

			if( !ifs )
				ThrowException(_T("File Not Found"));

			KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
			__int64 nxOZoneID, nxDZoneID;

			KxBulkDbase oBulkDBase(spDBaseConnection, pODTable, &rPurposeODColumnCollection);

			while (!ifs.eof())
			{
				ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
				pRecord->ReadBytes(ifs);

				if(ifs.eof())
					break;
				oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
			}
			oBulkDBase.DeprecatedFnExecute();
			delete pRecord;

			CTime timeEnd = CTime::GetCurrentTime();
			ApplyResult(spDBaseConnection, 
				timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), 
				timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"))
				);
			spDBaseConnection->Commit();
		}
		catch (KExceptionPtr ex)
		{
			spDBaseConnection->RollBack();
			throw ex;
		}
		catch (...)
		{
			spDBaseConnection->RollBack();
			throw 1;
		}
	}
	catch (KExceptionPtr ex)
	{
		//AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}


void KPA2ODModelDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


BOOL KPA2ODModelDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
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
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}   

	return KDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}