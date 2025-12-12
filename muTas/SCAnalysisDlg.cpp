// SCAnalysisDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCAnalysisDlg.h"
#include "afxdialogex.h"
#include "Target.h"


#include "ModeManager.h"
#include "SCAnalysisInfo.h"
#include "SCAnalysisInputDlg.h"
#include "SCAnalysisSelectDlg.h"
#include "SCAnalysisCriteriaDlg.h"
#include "SCAnalysisOutputDesignDlg.h"

#include "BulkDBaseNode.h"
#include "KBulkDBase2File.h"
#include "BulkDBaseScreenCordon.h"

#include "SCMapAnalysis.h"
#include "ImChampFrameWindow.h"

#include "Launcher.h"
#include "KxBulkDbase.h"

#include "ChildFrm.h"
#include "MainFrameWnd.h"
#include "IOView.h"
#include "IOTableController.h"
#include "ImChampDir.h"

#include "KUIDefSCAnalysis.h"
using namespace UISCAnalysis;

// KSCAnalysisDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(KSCAnalysisDlg, KDialogEx)

KSCAnalysisDlg::KSCAnalysisDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KSCAnalysisDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nLineType(0)
	, m_vecSelected(NULL)
	, m_nIteration(100)
	, m_dToleranceError(0.1)
	, m_nTotalNodeCount(0)
{
	m_bChkInput 	   = false;
	m_bChkSelectSCLine = false;
	m_bChkCriteria 	   = false;
	m_bChkOutput 	   = false;
}


KSCAnalysisDlg::~KSCAnalysisDlg()
{
	ClearInputList();
}


void KSCAnalysisDlg::ClearInputList( void )
{
	std::list<KSCAnalysisInfo*>::iterator itInput, itEnd = m_lstSCAnlaysisInfo.end();

	for (itInput = m_lstSCAnlaysisInfo.begin(); itInput != itEnd; ++itInput)
	{
		KSCAnalysisInfo* pSCAnlaysisInfo = *itInput;
		delete pSCAnlaysisInfo;
	}
	m_lstSCAnlaysisInfo.clear();
}


void KSCAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_STATIC_BACK,     m_uiBackGround);
	DDX_Control(pDX, IDC_STATIC_MODEL,    m_uiLblModel);
	DDX_Control(pDX, IDC_BTN_INPUT,	      m_uiBtnInput);
	DDX_Control(pDX, IDC_BTN_SELECT,      m_uiBtnSelect);
	DDX_Control(pDX, IDC_BTN_CRITERIA,    m_uiBtnCriteria);
	DDX_Control(pDX, IDC_BTN_OUTPUT,      m_uiBtnOutput);
	DDX_Control(pDX, IDC_BTN_OUTPUTTABLE, m_uiBtnOutputTable);
	DDX_Control(pDX, IDC_RUN,             m_btnRun);
	DDX_Control(pDX, IDC_STATIC_ARROW1,	  m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2,	  m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3,	  m_uiArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW4,	  m_uiArrow4);
}


BEGIN_MESSAGE_MAP(KSCAnalysisDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BTN_INPUT, &KSCAnalysisDlg::OnBnClickedBtnInput)
	ON_BN_CLICKED(IDC_BTN_SELECT, &KSCAnalysisDlg::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_CRITERIA, &KSCAnalysisDlg::OnBnClickedBtnCriteria)
	ON_BN_CLICKED(IDC_CLOSE, &KSCAnalysisDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, &KSCAnalysisDlg::OnBnClickedBtnOutput)
	ON_BN_CLICKED(IDC_RUN, &KSCAnalysisDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BTN_OUTPUTTABLE, &KSCAnalysisDlg::OnBnClickedBtnOutputtable)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// KSCAnalysisDlg 메시지 처리기입니다.


BOOL KSCAnalysisDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	RunnableCheck();

	InitDialogUI();
	InitInputList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KSCAnalysisDlg::InitDialogUI( void )
{
	/*BackGround*/
	m_uiBackGround.MoveWindow(9, 5, 595, 330);
	m_uiBackGround.SetMarkupText(UIXAML_SC_BACKIMAGE);

	UINT nID_DISTRIBUTION_MODEL  = 90915;
	XTPImageManager()->SetIcons(IDB_DISTRIBUTION_MODEL, &nID_DISTRIBUTION_MODEL, 1, 0);
	
	UINT nID_MODECHOICE_OUTPUT = 90927;
	XTPImageManager()->SetIcons(IDB_MODECHOICE_OUTPUT, &nID_MODECHOICE_OUTPUT, 1, 0); 

	/*Model*/
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_uiLblModel.SetMarkupText(KR_UIXAML_SC_MODEL);
	}
	else {
		m_uiLblModel.SetMarkupText(UIXAML_SC_MODEL);
	}

	/*Input*/
	m_uiBtnInput.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_uiBtnInput.SetWindowText(KR_UIXAML_SC_INPUT_DEFAULT);
	}
	else {
		m_uiBtnInput.SetWindowText(UIXAML_SC_INPUT_DEFAULT);
	}

	/*Select*/
	m_uiBtnSelect.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_uiBtnSelect.SetWindowText(KR_UIXAML_SC_SELECT_DEFAULT);
	}
	else {
		m_uiBtnSelect.SetWindowText(UIXAML_SC_SELECT_DEFAULT);
	}

	/*Stopping Criteria*/
	m_uiBtnCriteria.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_uiBtnCriteria.SetWindowTextW(KR_UIXAML_SC_CRITERIA_DEFAULT);
	}
	else {
		m_uiBtnCriteria.SetWindowTextW(UIXAML_SC_CRITERIA_DEFAULT);
	}

	/*Output*/
	m_uiBtnOutput.EnableMarkup(TRUE);
 	m_uiBtnOutput.EnableWindow(FALSE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_uiBtnOutput.SetWindowText(KR_UIXAML_SC_OUTPUT_FALSE);
	}
	else {
		m_uiBtnOutput.SetWindowText(UIXAML_SC_OUTPUT_FALSE);
	}

	/*View Table*/
	m_uiBtnOutputTable.EnableWindow(FALSE);

	m_uiArrow1.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB);
	m_uiArrow2.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRT);
	m_uiArrow3.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRB, ZEMArrowPositionRT);
	m_uiArrow4.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);

	SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1);
}


void KSCAnalysisDlg::InitInputList( void )
{
	KModeManager* pModeManager = m_pTarget->ModeManager();
	int nModeCount = pModeManager->GetModeCount();

	for (int i = 0; i < nModeCount; ++i)
	{
		KMode* pMode = pModeManager->GetMode(i);
		KSCAnalysisInfo* pSCAnlaysisInfo = new KSCAnalysisInfo;
		pSCAnlaysisInfo->Mode(pMode);

		m_lstSCAnlaysisInfo.push_back(pSCAnlaysisInfo);		
	}
}


void KSCAnalysisDlg::OnBnClickedBtnInput()
{
	KSCAnalysisInputDlg oDlg(m_pTarget, m_lstSCAnlaysisInfo);
	
	if(IDOK == oDlg.DoModal())
	{
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_uiBtnInput.SetWindowText(KR_UIXAML_SC_INPUT_SELECTED);
			m_uiBtnOutput.SetWindowText(KR_UIXAML_SC_OUTPUT_DEFAULT);
		}
		else {
			m_uiBtnInput.SetWindowText(UIXAML_SC_INPUT_SELECTED);
			m_uiBtnOutput.SetWindowText(UIXAML_SC_OUTPUT_DEFAULT);
		}
		m_uiBtnOutput.EnableWindow(TRUE);

		m_bChkInput  = true;
		m_bChkOutput = false;
	}
	RunnableCheck();
}


void KSCAnalysisDlg::OnBnClickedBtnSelect()
{
	KSCAnalysisSelectDlg oDlg(m_pTarget, m_nLineType, m_vecSelected);
	if(IDOK == oDlg.DoModal())
	{
		m_nLineType		= oDlg.LineType();
		m_vecSelected	= oDlg.Selected();
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_uiBtnSelect.SetWindowText(KR_UIXAML_SC_SELECT_SELECTED);
		}
		else {
			m_uiBtnSelect.SetWindowText(UIXAML_SC_SELECT_SELECTED);
		}

		m_bChkSelectSCLine = true;
	}
	RunnableCheck();
}


void KSCAnalysisDlg::OnBnClickedBtnCriteria()
{
	KSCAnalysisCriteriaDlg oDlg(m_nIteration, m_dToleranceError);
	if(IDOK == oDlg.DoModal())
	{
		m_nIteration		= oDlg.Iteration();
		m_dToleranceError	= oDlg.ToleranceError();
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_uiBtnCriteria.SetWindowText(KR_UIXAML_SC_CRITERIA_SELECTED);
		}
		else {
			m_uiBtnCriteria.SetWindowText(UIXAML_SC_CRITERIA_SELECTED);
		}

		m_bChkCriteria = true;
	}
	RunnableCheck();
}

void KSCAnalysisDlg::OnBnClickedClose()
{
	KDialogEx::OnCancel();
}


void KSCAnalysisDlg::OnBnClickedRun()
{
	RunSCAnalysisModel();
}

void KSCAnalysisDlg::OnBnClickedBtnOutput()
{
	KSCAnalysisOutputDesignDlg oDlg(m_pTarget, m_lstSCAnlaysisInfo);
	
	if (IDOK == oDlg.DoModal())
	{
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_uiBtnOutput.SetWindowText(KR_UIXAML_SC_OUTPUT_SELECTED);
		} 
		else {
			m_uiBtnOutput.SetWindowText(UIXAML_SC_OUTPUT_SELECTED);
		}

		m_bChkOutput = true;
	}
	RunnableCheck();
}

void KSCAnalysisDlg::RunSCAnalysisModel( void )
{
	TxLogDebugStartMsg();
	
	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicDonkeyProgress oDonkeyProgress(this, _T("Screen Cordon Analysis"), TRUE, FALSE, FALSE, nLang);
		CWaitCursor wait;

		oDonkeyProgress.SendTextEvent(_T("Select Node Data"));

		KBulkDBaseNode::ZoneNodeQuery(m_pTarget, m_vecZoneNode);
		KBulkDBaseNode::TotalNodeCount(m_pTarget, m_nTotalNodeCount);

		KMapView* pMapView = ImChampFrameWindow::GetActiveMapView();
		int nSCLineCount = m_vecSelected.size();

		// Step 0. Generate Base Data
		if (0 == m_nLineType)
		{
			for (int i = 0; i < nSCLineCount; ++i)
			{
				CString strSCLineName;
				std::vector<__int64>				vecIntersectLink;
				std::vector<TDrawScreenCordonXY>	vecPoint;
				std::map<__int64, int>				mapNodeDirection;
				__int64 nxSCLineID = m_vecSelected[i];

				KBulkDBaseScreenCordon::ScreenQuery(m_pTarget, nxSCLineID, strSCLineName, vecPoint);
				KSCMapAnalysis::ScreenLineAnalysis(pMapView, m_vecZoneNode, vecPoint, mapNodeDirection);
				KSCMapAnalysis::ScreenLineIntersectLink(pMapView, vecPoint, vecIntersectLink);

				m_mapSCLineZoneNodeDirection.insert(make_pair(nxSCLineID, mapNodeDirection));
				m_mapSCLineInterSectLink.insert(make_pair(nxSCLineID, vecIntersectLink));
			}
		}
		else if (1 == m_nLineType)
		{
			for (int i = 0; i < nSCLineCount; ++i)
			{
				CString strSCLineName;
				std::vector<TDrawScreenCordonXY>	vecPoint;
				std::map<__int64, int>				mapNodeDirection;
				__int64 nxSCLineID = m_vecSelected[i];

				KBulkDBaseScreenCordon::CordonQuery(m_pTarget, nxSCLineID, strSCLineName, vecPoint);
				KSCMapAnalysis::CordonLineAnalysis(pMapView, m_vecZoneNode, vecPoint, mapNodeDirection);

				m_mapSCLineZoneNodeDirection.insert(make_pair(nxSCLineID, mapNodeDirection));
			}
		}

		oDonkeyProgress.SendTextEvent(_T("Write Argument"));

		WriteArguments();

		oDonkeyProgress.SendTextEvent(_T("Write OD Data"));

		WriteBinFileODData(&oDonkeyProgress);

		oDonkeyProgress.SendTextEvent(_T("Write Link Data"));

		WriteBinFileLineZoneData(&oDonkeyProgress);

		WriteBinFileLinkData(&oDonkeyProgress);

		oDonkeyProgress.Close();

		QBicSimpleProgressThread::ExecuteThread(SCAnalysisThreadCaller, this, false, nLang);

	}
	catch (KExceptionPtr ex)
	{	
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}

	m_uiBtnOutputTable.EnableWindow(true);

	try
	{
		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable* pModeODTable = pIOTables->FindTable(TABLE_MODE_OD);

		pModeODTable->Notify();
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	TxLogDebugEndMsg();
}

void KSCAnalysisDlg::WriteArguments( void )
{
	TxLogDebugStartMsg();

	ImChampDir::SetCurrentDirectoryToApp();

	CString strFileName = _T("scline.arg");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strFileName, nOpenMode);

	if (!WriteArgCommon(of))
	{
		of.Close();
		TxLogDebugException();
		ThrowException(_T("Argument 생성(WriteArguments) 중 오류가 발생하였습니다."));
		return;
	}

	if (!WriteArgInput(of))
	{
		of.Close();
		TxLogDebugException();
		ThrowException(_T("Argument 생성(WriteArgInput) 중 오류가 발생하였습니다."));
		return;
	}

	if (!WriteArgSelect(of))
	{
		of.Close();
		TxLogDebugException();
		ThrowException(_T("Argument 생성(WriteArgSelect) 중 오류가 발생하였습니다."));
		return;
	}

	if (!WriteArgCriteria(of))
	{
		of.Close();
		TxLogDebugException();
		ThrowException(_T("Argument 생성(WriteArgCriteria) 중 오류가 발생하였습니다."));
		return;
	}
	of.Close();
	TxLogDebugEndMsg();
}

bool KSCAnalysisDlg::WriteArgCommon( CStdioFileEx& of )
{
	CString strOut;

	try
	{
		strOut.Format(_T("noZone_noNode\t%d\t%d\r\n"), m_vecZoneNode.size(), m_nTotalNodeCount);
		of.WriteString(strOut);

		int nModeODCount = m_lstSCAnlaysisInfo.size();
		int nSelectedModeODCount(0);

		std::list<KSCAnalysisInfo*>::iterator iter	= m_lstSCAnlaysisInfo.begin();
		std::list<KSCAnalysisInfo*>::iterator end	= m_lstSCAnlaysisInfo.end();

		while (iter != end)
		{
			KSCAnalysisInfo* pSCAnalysisInfo = *iter;

			if(pSCAnalysisInfo->Selected())
				++nSelectedModeODCount;
			++iter;
		}
		strOut.Format(_T("noOD\t%d\r\n"), nSelectedModeODCount);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KSCAnalysisDlg::WriteArgInput( CStdioFileEx& of )
{
	CString strOut;

	try
	{
		std::list<KSCAnalysisInfo*>::iterator iter	= m_lstSCAnlaysisInfo.begin();
		std::list<KSCAnalysisInfo*>::iterator end	= m_lstSCAnlaysisInfo.end();

		int nIndex(1);

		while (iter != end)
		{
			KSCAnalysisInfo* pSCAnalysisInfo = *iter;

			if(pSCAnalysisInfo->Selected())
			{
				strOut.Format(_T("ODIndex_SurveyVolIndex_Occ_PCUOD_PCUVol\t%d\t%d\t%f\t%f\t%f\r\n"),					nIndex, nIndex, pSCAnalysisInfo->Occupancy(), 
					pSCAnalysisInfo->ODPCU(), pSCAnalysisInfo->VolumePCU());
				of.WriteString(strOut);
				++nIndex;
			}
			++iter;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KSCAnalysisDlg::WriteArgSelect( CStdioFileEx& of )
{
	CString strOut;

	try
	{
		strOut.Format(_T("noSCLine\t%d\r\n"), m_vecSelected.size());
		of.WriteString(strOut);

		std::map<__int64, std::map<__int64, int>>::iterator iter = m_mapSCLineZoneNodeDirection.begin();
		std::map<__int64, std::map<__int64, int>>::iterator end = m_mapSCLineZoneNodeDirection.end();
		while (iter != end)
		{
			__int64 nxSCLineID = iter->first;
			std::map<__int64, int> mapZoneNodeDIR = iter->second;

			std::map<__int64, int>::iterator inIter = mapZoneNodeDIR.begin();
			std::map<__int64, int>::iterator inEnd	= mapZoneNodeDIR.end();

			int nFromRTCount(0), nFromLTCount(0);

			while (inIter != inEnd)
			{
				int nDirection = inIter->second;

				if (1 == nDirection)
				{
					++nFromLTCount;
				}
				else if (2 == nDirection)
				{
					++nFromRTCount;
				}
				++inIter;
			}
			strOut.Format(_T("SCID_Dir_noZone\t%I64d\t%d\t%d\r\n"), nxSCLineID, 1, nFromRTCount);
			of.WriteString(strOut);

			strOut.Format(_T("SCID_Dir_noZone\t%I64d\t%d\t%d\r\n"), nxSCLineID, 2, nFromLTCount);
			of.WriteString(strOut);

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KSCAnalysisDlg::WriteArgCriteria( CStdioFileEx& of )
{
	CString strOut;

	try
	{
		strOut.Format(_T("Iterations\t%d\r\n"), m_nIteration);
		of.WriteString(strOut);

		strOut.Format(_T("tolerance_Error\t%f\r\n"), m_dToleranceError);
		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

void KSCAnalysisDlg::WriteBinFileODData( QBicDonkeyProgress* a_pDonkey )
{
	TxLogDebugStartMsg();
	KIOColumns modeColumnCollection;

	std::list<KSCAnalysisInfo*>::iterator iter	= m_lstSCAnlaysisInfo.begin();
	std::list<KSCAnalysisInfo*>::iterator end	= m_lstSCAnlaysisInfo.end();

	while (iter != end)
	{
		KSCAnalysisInfo* pSCAnalysisInfo = *iter;

		if (pSCAnalysisInfo->Selected())
		{
			KIOColumn* pColumn = pSCAnalysisInfo->InputColumn();

			if (pColumn)
				modeColumnCollection.AddNewColumn(pColumn);
		}
		++iter;
	}
	bool bResult = KBulkDBase2File::ExportDB2ODFile(
		m_pTarget->GetDBaseConnection(), _T("mode_od"), modeColumnCollection, _T("OriginalOD.dat"));

	if (!bResult)
	{
		TxLogDebugException();
		ThrowException(_T("Binary 생성(WriteBinFileODData) 중 오류가 발생하였습니다."));
	}
	TxLogDebugEndMsg();
}

void KSCAnalysisDlg::WriteBinFileLineZoneData( QBicDonkeyProgress* a_pDonkey )
{
	TxLogDebugStartMsg();
	std::ofstream ofs(_T("LineZoneList.dat"), std::ios::binary);

	std::map<__int64, std::map<__int64, int>>::iterator iter = m_mapSCLineZoneNodeDirection.begin();
	std::map<__int64, std::map<__int64, int>>::iterator end	 = m_mapSCLineZoneNodeDirection.end();

	while (iter != end)
	{
		int nZoneListIndex(0);
		__int64 nxSCLineID = iter->first;
		std::map<__int64, int> mapZoneNodeDIR = iter->second;

		std::map<__int64, int>::iterator innerIter	= mapZoneNodeDIR.begin();
		std::map<__int64, int>::iterator innerEnd	= mapZoneNodeDIR.end();
		
		while (innerIter != innerEnd)
		{
			__int64 nxZoneNodeID	= innerIter->first;
			int		nDirection		= innerIter->second;

			ofs.write(reinterpret_cast<char*>(&nxSCLineID), sizeof(__int64));
			ofs.write(reinterpret_cast<char*>(&nDirection), sizeof(int));
			ofs.write(reinterpret_cast<char*>(&nZoneListIndex), sizeof(int));
			ofs.write(reinterpret_cast<char*>(&nxZoneNodeID), sizeof(__int64));

			++nZoneListIndex;
			++innerIter;
		}
		++iter;
	}
	ofs.close();
	TxLogDebugEndMsg();
}

void KSCAnalysisDlg::WriteBinFileLinkData( QBicDonkeyProgress* a_pDonkey )
{
	TxLogDebugStartMsg();
	std::map<__int64, __int64>			mapLinkFNode;
	std::vector<__int64>				vecAllNode;
	std::map<__int64, KSDoubleRecord*>	mapLinkSurveyVolumn;
	
	KBulkDBaseScreenCordon::LinkFNode(m_pTarget, mapLinkFNode);

	KIOColumns linkColumnCollection;

	std::list<KSCAnalysisInfo*>::iterator iter	= m_lstSCAnlaysisInfo.begin();
	std::list<KSCAnalysisInfo*>::iterator end	= m_lstSCAnlaysisInfo.end();

	while (iter != end)
	{
		KSCAnalysisInfo* pSCAnalysisInfo = *iter;

		if (pSCAnalysisInfo->Selected())
		{
			KIOColumn* pColumn = pSCAnalysisInfo->SurveyColumn();

			if (pColumn)
				linkColumnCollection.AddNewColumn(pColumn);
		}
		++iter;
	}

	KBulkDBaseScreenCordon::LinkSurveyVolume(m_pTarget, linkColumnCollection, mapLinkSurveyVolumn);
	KBulkDBaseScreenCordon::ALLNode(m_pTarget, vecAllNode);

	int nSCLineCount = m_vecSelected.size();

	std::vector<__int64> vecIntersectLinkID;
	KMapView* pMapView = ImChampFrameWindow::GetActiveMapView();
	
	std::ofstream ofs(_T("linkData.dat"), std::ios::binary);

	if (m_nLineType == 0)
	{
		for (int i = 0; i < nSCLineCount; ++i)
		{
			CString strSCLineName;
			std::vector<__int64>				vecIntersectLink;
			std::vector<TDrawScreenCordonXY>	vecPoint;
			std::map<__int64, int>				mapNodeDirection;
			__int64 nxSCLineID = m_vecSelected[i];

			KBulkDBaseScreenCordon::ScreenQuery(m_pTarget, nxSCLineID, strSCLineName, vecPoint);
			KSCMapAnalysis::ScreenLineIntersectLink(pMapView, vecPoint, vecIntersectLink);
			KSCMapAnalysis::ScreenLineAnalysis(pMapView, vecAllNode, vecPoint, mapNodeDirection);
			int nIntersectLinkCount = vecIntersectLink.size();


			//Direction Type - 1
			int nloopCount = 0;

			for (int i = 0; i < nIntersectLinkCount; ++i)
			{
				__int64 nxIntersectLinkID = vecIntersectLink[i];
				__int64 nxIntersectFNodeID;
				int nDirection(-1);
				//std::map<__int64, __int64>			mapLinkFNode;
				std::map<__int64, __int64>::iterator findIter = mapLinkFNode.find(nxIntersectLinkID);

				if (findIter != mapLinkFNode.end())
				{
					nxIntersectFNodeID = findIter->second;
				}
				
				std::map<__int64, int>::iterator findIter2 = mapNodeDirection.find(nxIntersectFNodeID);
				
				if (findIter2 != mapNodeDirection.end())
				{
					nDirection = findIter2->second;
				}

				if (nDirection != 1)
					continue;
				
				nloopCount++;

				ofs.write(reinterpret_cast<char*>(&nxSCLineID), sizeof(__int64));
				ofs.write(reinterpret_cast<char*>(&nDirection), sizeof(int));
				ofs.write(reinterpret_cast<char*>(&nloopCount), sizeof(int));

				std::map<__int64, KSDoubleRecord*>::iterator surveyIter = mapLinkSurveyVolumn.find(nxIntersectLinkID); 

				if (surveyIter != mapLinkSurveyVolumn.end())
				{
					KSDoubleRecord* pRecord = surveyIter->second;
					pRecord->WriteBytes(ofs);
				}
			}

			//Direction Type - 2
			nloopCount = 0;

			for (int i = 0; i < nIntersectLinkCount; ++i)
			{
				__int64 nxIntersectLinkID = vecIntersectLink[i];
				__int64 nxIntersectFNodeID;
				int nDirection(-1);
				//std::map<__int64, __int64>			mapLinkFNode;
				std::map<__int64, __int64>::iterator findIter = mapLinkFNode.find(nxIntersectLinkID);

				if (findIter != mapLinkFNode.end())
				{
					nxIntersectFNodeID = findIter->second;
				}

				std::map<__int64, int>::iterator findIter2 = mapNodeDirection.find(nxIntersectFNodeID);

				if (findIter2 != mapNodeDirection.end())
				{
					nDirection = findIter2->second;
				}

				if (nDirection != 2)
					continue;

				nloopCount++;

				ofs.write(reinterpret_cast<char*>(&nxSCLineID), sizeof(__int64));
				ofs.write(reinterpret_cast<char*>(&nDirection), sizeof(int));
				ofs.write(reinterpret_cast<char*>(&nloopCount), sizeof(int));

				std::map<__int64, KSDoubleRecord*>::iterator surveyIter = mapLinkSurveyVolumn.find(nxIntersectLinkID); 

				if (surveyIter != mapLinkSurveyVolumn.end())
				{
					KSDoubleRecord* pRecord = surveyIter->second;
					pRecord->WriteBytes(ofs);
				}
			}
		}
	}
	else if (1 == m_nLineType)
	{
		for (int i = 0; i < nSCLineCount; ++i)
		{
			CString strSCLineName;
			std::vector<__int64>				vecIntersectLink;
			std::vector<TDrawScreenCordonXY>	vecPoint;
			std::map<__int64, int>				mapNodeDirection;
			__int64 nxSCLineID = m_vecSelected[i];

			KBulkDBaseScreenCordon::CordonQuery(m_pTarget, nxSCLineID, strSCLineName, vecPoint);
			KSCMapAnalysis::CordonLineIntersectLink(pMapView, vecPoint, vecIntersectLink);
			KSCMapAnalysis::CordonLineAnalysis(pMapView, vecAllNode, vecPoint, mapNodeDirection);
			int nIntersectLinkCount = vecIntersectLink.size();


			//Direction Type - 1
			int nloopCount = 0;

			for (int i = 0; i < nIntersectLinkCount; ++i)
			{
				__int64 nxIntersectLinkID = vecIntersectLink[i];
				__int64 nxIntersectFNodeID;
				int nDirection(-1);
				//std::map<__int64, __int64>			mapLinkFNode;
				std::map<__int64, __int64>::iterator findIter = mapLinkFNode.find(nxIntersectLinkID);

				if (findIter != mapLinkFNode.end())
				{
					nxIntersectFNodeID = findIter->second;
				}

				std::map<__int64, int>::iterator findIter2 = mapNodeDirection.find(nxIntersectFNodeID);

				if (findIter2 != mapNodeDirection.end())
				{
					nDirection = findIter2->second;
				}

				if (nDirection != 1)
					continue;

				nloopCount++;

				ofs.write(reinterpret_cast<char*>(&nxSCLineID), sizeof(__int64));
				ofs.write(reinterpret_cast<char*>(&nDirection), sizeof(int));
				ofs.write(reinterpret_cast<char*>(&nloopCount), sizeof(int));

				std::map<__int64, KSDoubleRecord*>::iterator surveyIter = mapLinkSurveyVolumn.find(nxIntersectLinkID); 

				if (surveyIter != mapLinkSurveyVolumn.end())
				{
					KSDoubleRecord* pRecord = surveyIter->second;
					pRecord->WriteBytes(ofs);
				}
			}

			//Direction Type - 2
			nloopCount = 0;

			for (int i = 0; i < nIntersectLinkCount; ++i)
			{
				__int64 nxIntersectLinkID = vecIntersectLink[i];
				__int64 nxIntersectFNodeID;
				int nDirection(-1);
				//std::map<__int64, __int64>			mapLinkFNode;
				std::map<__int64, __int64>::iterator findIter = mapLinkFNode.find(nxIntersectLinkID);

				if (findIter != mapLinkFNode.end())
				{
					nxIntersectFNodeID = findIter->second;
				}

				std::map<__int64, int>::iterator findIter2 = mapNodeDirection.find(nxIntersectFNodeID);

				if (findIter2 != mapNodeDirection.end())
				{
					nDirection = findIter2->second;
				}

				if (nDirection != 2)
					continue;

				nloopCount++;

				ofs.write(reinterpret_cast<char*>(&nxSCLineID), sizeof(__int64));
				ofs.write(reinterpret_cast<char*>(&nDirection), sizeof(int));
				ofs.write(reinterpret_cast<char*>(&nloopCount), sizeof(int));

				std::map<__int64, KSDoubleRecord*>::iterator surveyIter = mapLinkSurveyVolumn.find(nxIntersectLinkID); 

				if (surveyIter != mapLinkSurveyVolumn.end())
				{
					KSDoubleRecord* pRecord = surveyIter->second;
					pRecord->WriteBytes(ofs);
				}
			}
		}
	}
	ofs.close();
	TxLogDebugEndMsg();
}

void KSCAnalysisDlg::RunnableCheck( void )
{
	if (m_bChkInput && m_bChkSelectSCLine && m_bChkCriteria && m_bChkOutput)
	{
		m_btnRun.EnableWindow(TRUE);
	}
	else
	{
		m_btnRun.EnableWindow(FALSE);
	}
}

void KSCAnalysisDlg::ReceiveStatusMessageFromDll( void* pParam, int nStatus, char* strMsg )
{
}

void KSCAnalysisDlg::AddSCAnalysisResultColumn( KIOColumns& a_rODColumnCollection )
{
	a_rODColumnCollection.Clear();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pODTable  = pIOTables->FindTable(TABLE_MODE_OD);

	std::list<KSCAnalysisInfo*>::iterator itInput, itEnd = m_lstSCAnlaysisInfo.end();

	for (itInput = m_lstSCAnlaysisInfo.begin(); itInput != itEnd; ++itInput)
	{
		KSCAnalysisInfo* pSCAnlaysisInfo = *itInput;

		if(!pSCAnlaysisInfo->Selected())
			continue;

		KIOColumn* pColumn = pSCAnlaysisInfo->OutputColumn();

		if (NULL == pColumn)
        {
            KIOColumn column;
            column.Name(pODTable->GetUsableColumnName());
            column.Caption(pSCAnlaysisInfo->OutputColumnName());
            column.CodeType(KEMIOCodeTypeIsNothing);
            column.ColumnType(KEMIOColumnTypeUserDefine);
            column.DataType(KEMIODataTypeDouble);
            column.DisplayOrder(pODTable->Columns()->ColumnCount()+1);
            column.PrimaryKey(false);
            column.Visible(true);
            column.ColumnInfo(pSCAnlaysisInfo->OutputColumnName());

            pColumn = pODTable->AddColumn(column);			
        }

		pSCAnlaysisInfo->ColumnName(pColumn->Name());
		
		a_rODColumnCollection.AddNewColumn(pColumn);
	}
}

void KSCAnalysisDlg::RollBackResultColumn( void )
{
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pODTable  = pIOTables->FindTable(TABLE_MODE_OD);

	std::list<KSCAnalysisInfo*>::iterator itInput, itEnd = m_lstSCAnlaysisInfo.end();

	for (itInput = m_lstSCAnlaysisInfo.begin(); itInput != itEnd; ++itInput)
	{
		KSCAnalysisInfo* pSCAnlaysisInfo = *itInput;

		if(!pSCAnlaysisInfo->Selected())
			continue;

		KIOColumn* pColumn = pSCAnlaysisInfo->OutputColumn();

		if (NULL == pColumn)
			pODTable->RemoveColumn(pSCAnlaysisInfo->OutputColumnName());
	}
}


void KSCAnalysisDlg::OnBnClickedBtnOutputtable()
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pModeODTable = pIOTables->FindTable(TABLE_MODE_OD);
	
	pModeODTable->Notify();

	ASSERT(NULL != pModeODTable);

	CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	pChildFrame->ShowIOView( true );
	KIOView* pIOView = pChildFrame->GetIOView();

	if(NULL != pIOView)
	{
		KIOTableView* pTableView = pIOView->FindTableView(pModeODTable->Name());
		if(NULL == pTableView)
		{
			pTableView = pIOView->CreateNewTableView(pModeODTable);
			KIOTableController* pController = new KIOTableController(m_pTarget);
			pController->Attach(pModeODTable, pTableView);
		}
		pIOView->SetActiveTableView(pModeODTable->Name());
	}
	KRedrawWindow();
	TxLogDebugEndMsg();
}

void KSCAnalysisDlg::KRedrawWindow( void )
{
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}

unsigned __stdcall KSCAnalysisDlg::SCAnalysisThreadCaller( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("페쇄선 분석 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Screen & Cordon Line Analysis Module..."));
	}

	KSCAnalysisDlg* pDlg = (KSCAnalysisDlg*)pParameter->GetParameter();;

	pDlg->RunModel();
	return 0;
}

void KSCAnalysisDlg::RunModel()
{
	
	CTime timeStart = CTime::GetCurrentTime();

	m_nResultCode = DLL_UNEXPECTED_ERROR;
	CString strWindowName;
	GetWindowText(strWindowName);

	KLauncher::Run(strWindowName, 32);

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("페쇄선 분석 모듈 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Screen & Cordon Line Analysis Module Complete."));
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
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Running Failed."));
		}
		TxLogDebug(_T("Model Run Error.."));

		return;
	}

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Saving Output Data..."));
	}
	//결과정보저장.
	KIOColumns rModeODColumnCollection;
	AddSCAnalysisResultColumn( rModeODColumnCollection );

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable* 		 pODTable 	   = pIOTables->FindTable(TABLE_MODE_OD);
		int 			 nColumnCount  = rModeODColumnCollection.ColumnCount();

		if (0 == nColumnCount)
			ThrowException(_T("적용 컬럼 정보가 없습니다."));

		std::ifstream ifs( _T("NewOD.dat"), std::ios::binary );

		if( !ifs )
			ThrowException(_T("File Not Found"));

		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;

		KxBulkDbase oBulkDBase(spDBaseConnection, pODTable, &rModeODColumnCollection);

		while (!ifs.eof())
		{
			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
			pRecord->ReadBytes(ifs);

			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}

		oBulkDBase.ExecuteFullOuterJoin();
		delete pRecord;

		CTime timeEnd = CTime::GetCurrentTime();
		KBulkDBaseScreenCordon::ApplyModeChoiceResult(spDBaseConnection, 
			m_lstSCAnlaysisInfo, 
			timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), 
			timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"))
			);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		RollBackResultColumn();
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch (...)
	{
		RollBackResultColumn();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KSCAnalysisDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

BOOL KSCAnalysisDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
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
