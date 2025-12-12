/**
 * @file GenerationModelDlg.cpp
 * @brief KGenerationModelDlg 구현 파일
 * @author 
 * @date 2011.04.28
 * @remark
 */

#include "stdafx.h"
#include "KmzApp.h"
#include "GenerationModelDlg.h"
#include "../kmzModel/KModelInclude.h"
#include "../kmzModel/KDoubleRecord.h"
#include "../kmzModel/KDoubleRecordArray.h"
#include "ExpressionElement.h"
#include "ExpressionElementField.h"
#include "GenerationInfo.h"
#include "GGroupExpressionContainer.h"
#include "GZoneGroupPage.h"
#include "GInputTreeNavigator.h"
#include "GOutputDesignDlg.h"
#include "GRegressionExpression.h"
#include "GSelectModelDlg.h"
#include "GSimpleExpressionContainer.h"
#include "GSimpleExpressionPage.h"
#include "GSimpleRegressionPage.h"
#include "GSimpleUnitPage.h"
#include "GUnitExpression.h"
#include "KGCategoryPage.h"
#include "KGCategoryExpression.h"
#include "IOTableController.h"
#include "PAPurpose.h"
#include "PurposeManager.h"
#include "Target.h"
#include "ZoneGroupColumn.h"
#include "ZoneGroupValue.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "MainFrameWnd.h"
#include "IOTableView.h"
#include "IOView.h"
#include "GBaseExpression.h"

#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "KUIDefGeneration.h"
#include "XmlManager.h"
#include "GDefaultSimpleFuntion.h"
#include "GDefaultZoneGroupFunction.h"
#include "ParseExpression.h"
#include "ImChampFrameWindow.h"
#include "DBaseGenerationParaModel.h"

using namespace UIGeneration;
IMPLEMENT_DYNAMIC(KGenerationModelDlg, KDialogEx)

KGenerationModelDlg::KGenerationModelDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KGenerationModelDlg::IDD, pParent)
{
    TxLogDebugStartMsg();
	m_pTarget = NULL;
	m_pZone = NULL;
    TxLogDebugEndMsg();
}

KGenerationModelDlg::~KGenerationModelDlg()
{
    TxLogDebugStartMsg();
	clearGenerationInfo();
	CleareDefaultData();
    TxLogDebugEndMsg();
}

void KGenerationModelDlg::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MODEL, m_btnModel);
    DDX_Control(pDX, IDC_INPUT, m_btnInput);
    DDX_Control(pDX, IDC_OUTPUTDESIGN, m_btnOutputDesign);
    DDX_Control(pDX, IDC_OUTPUTTABLE, m_btnViewTable);
    DDX_Control(pDX, IDC_RUN, m_btnRun);
    DDX_Control(pDX, IDC_STATIC_ARROW1, m_uiArrow1);    
    DDX_Control(pDX, IDC_STATIC_ARROW2, m_uiArrow2);
    DDX_Control(pDX, IDC_STATIC_BACK, m_uiBackImage);
}


BEGIN_MESSAGE_MAP(KGenerationModelDlg, KDialogEx)
	ON_BN_CLICKED(IDC_MODEL, &KGenerationModelDlg::OnBnClickedModel)
	ON_BN_CLICKED(IDC_INPUT, &KGenerationModelDlg::OnBnClickedInput)
	ON_BN_CLICKED(IDC_OUTPUTDESIGN, &KGenerationModelDlg::OnBnClickedOutputdesign)
	ON_BN_CLICKED(IDC_RUN, &KGenerationModelDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BUTTON1, &KGenerationModelDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_OUTPUTTABLE, &KGenerationModelDlg::OnBnClickedOutputtable)
	//ON_WM_ACTIVATE() - markup button focus 있을때 외부 클릭 시 버튼 사라짐 문제 때문에 기능 삭제
END_MESSAGE_MAP()


void KGenerationModelDlg::KRedrawWindow()
{
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}


void KGenerationModelDlg::SetTarget(KTarget* a_pTarget)
{
    TxLogDebugStartMsg();
	m_pTarget = a_pTarget;
	ASSERT(NULL != a_pTarget);

	KIOTables* pTables = a_pTarget->Tables();
	ASSERT(NULL != pTables);

	m_nTargetYear = a_pTarget->GetTargetYear();
	m_pZone = pTables->FindTable(_T("ZONE"));
	ASSERT(NULL != m_pZone);


	initPurpose();
	initGenerationInfo();
    TxLogDebugEndMsg();
}


bool KGenerationModelDlg::initPurpose(void)
{
    TxLogDebugStartMsg();

	if(NULL != m_pTarget)
	{
		KPurposeManager* pPurposeMgr = m_pTarget->PurposeManager();
		if(NULL == pPurposeMgr)
		{
			return false;
		}

		int nPurposeCount = pPurposeMgr->GetPurposeCount();
		for(int i = 0; i < nPurposeCount; ++i)
		{
			KPAPurpose* pPurpose = pPurposeMgr->GetPurpose(i);
			m_lstPurposeList.push_back(pPurpose);
		}
	}

    TxLogDebugEndMsg();
	return true;
}


bool KGenerationModelDlg::initGenerationInfo(void)
{
    TxLogDebugStartMsg();
	clearGenerationInfo();
	
	std::list<KPAPurpose*>::iterator itPurpose, itEnd = m_lstPurposeList.end();
	for(itPurpose = m_lstPurposeList.begin(); itPurpose != itEnd; ++itPurpose)
	{
		KPAPurpose* pPurpose = *itPurpose;
		KGenerationInfo* pInfo = new KGenerationInfo;
		pInfo->Purpose(pPurpose);
		pInfo->UseGeneration(false);
		m_lstGInfoList.push_back(pInfo);
	}

    TxLogDebugEndMsg();
	return true;
}


void KGenerationModelDlg::clearGenerationInfo(void)
{
    TxLogDebugStartMsg();
	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		KGenerationInfo* pInfo = *itInfo;
		delete pInfo;
	}

	m_lstGInfoList.clear();
    TxLogDebugEndMsg();
}


// KGenerationModelDlg 메시지 처리기입니다.

//Step 0 : 다이얼로그 초기화 
BOOL KGenerationModelDlg::OnInitDialog()
{
    TxLogDebugStartMsg();

	m_KemHaveDB = KEMHaveSed_All;

	((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(FALSE);

	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KBulkDBase::BulkSedPopSelect(m_pTarget->GetDBaseConnection(), m_vecSEDPopRecords);

    UINT nID_GENERATION_INPUT  = 90903;
    XTPImageManager()->SetIcons(IDB_GENERATION_INPUT, &nID_GENERATION_INPUT, 1, 0); 
    
    UINT nID_GENERATION_MODEL  = 90905;
    XTPImageManager()->SetIcons(IDB_GENERATION_MODEL, &nID_GENERATION_MODEL, 1, 0); 

    UINT nID_GENERATION_OUTPUT = 90907;
    XTPImageManager()->SetIcons(IDB_GENERATION_OUTPUT, &nID_GENERATION_OUTPUT, 1, 0); 

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(4, 5);

	m_uiBackImage.MoveWindow(&rcClient);
	m_uiBackImage.SetMarkupText(UIXAML_G_BACKIMAGE);

    m_uiArrow1.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);      
    m_uiArrow2.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);

	InitButtonControl();
	InitDefaultValue();

	m_btnViewTable.EnableWindow(FALSE);
	m_btnRun.EnableWindow(FALSE);    
    
    SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1); // 1, 
    KRedrawWindow();

	return TRUE; 
}

void KGenerationModelDlg::InitButtonControl()
{
	m_btnModel.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnModel.SetWindowText(KR_UIXAML_G_MODEL_DEFAULT);
	}
	else {
		m_btnModel.SetWindowText(UIXAML_G_MODEL_DEFAULT);
	}

	m_btnInput.EnableWindow(FALSE);
	m_btnInput.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnInput.SetWindowText(KR_UIXAML_G_INPUT_FALSE);
	}
	else {
		m_btnInput.SetWindowText(UIXAML_G_INPUT_FALSE);
	}

	m_btnOutputDesign.EnableWindow(FALSE);
	m_btnOutputDesign.EnableMarkup(TRUE);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		m_btnOutputDesign.SetWindowText(KR_UIXAML_G_OUTPUT_FALSE);
	}
	else {
		m_btnOutputDesign.SetWindowText(UIXAML_G_OUTPUT_FALSE);
	}
}


// Step 1 : 모델 선택
void KGenerationModelDlg::OnBnClickedModel()
{
	KGSelectModelDlg dlg(m_KemHaveDB);
	
	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		dlg.AddGenerationInfo(*itInfo);
	}

	if(dlg.DoModal() == IDOK)
	{        
        if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnModel.SetWindowText(KR_UIXAML_G_MODEL_SELECTED);
			m_btnInput.SetWindowText(KR_UIXAML_G_INPUT_DEFAULT);
		}
		else {
			m_btnModel.SetWindowText(UIXAML_G_MODEL_SELECTED);
			m_btnInput.SetWindowText(UIXAML_G_INPUT_DEFAULT);
		}
        
		m_btnInput.EnableWindow(TRUE);  
		RunButtonControl();
		SetDefaultData();
	}	

    KRedrawWindow();
}


// Step 2 : Input 선택
void KGenerationModelDlg::OnBnClickedInput()
{
	TxLogDebugStartMsg();
	CXTPPropertySheet     oPropertySheet(_T("Input Design"));
	KGInputTreeNavigator* pTreeNav = new KGInputTreeNavigator;

	if(nullptr != m_spKGFormulaDefaultDlg)
	{
		m_spKGFormulaDefaultDlg.reset();
	}

	std::vector<CXTPPropertyPage*> vecPageArray;
	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();

	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		KGenerationInfo* pInfo = *itInfo;

        if (!pInfo->UseGeneration())
        {
            continue;
        }

		KGenerationInfo* pDefaultInfo = GetDefaultData(pInfo);

        pTreeNav->AddGenerationInfo(pInfo);

        if(pInfo->ZoneGroup() == true)
        {
            KGZoneGroupPage* pPage = new KGZoneGroupPage;
            pPage->SetParentSheet   (&oPropertySheet);
            pPage->SetTarget        (m_pTarget);
            pPage->SetGenerationInfo(pInfo);
            pPage->SetZoneTable     (m_pZone);
            pPage->SetCodeManager   (m_pTarget->CodeManager());
			pPage->SetCaption(pInfo->GenerationModelName());
			pPage->SetDefaultPassengerInfo(pDefaultInfo);

            vecPageArray.push_back  (pPage);
            oPropertySheet.AddPage  (pPage);
        }
        else
        {
            if(pInfo->GenerationModel() == KEMGenerationUnit)
            {
                KGSimpleUnitPage* pPage = new KGSimpleUnitPage(&m_spKGFormulaDefaultDlg);
                pPage->SetParentSheet   (&oPropertySheet);
                pPage->SetTarget        (m_pTarget);
                pPage->SetGenerationInfo(pInfo);
				pPage->SetCaption(pInfo->GenerationModelName());
				pPage->SetDefaultPassengerData(pDefaultInfo);

                vecPageArray.push_back  (pPage);
                oPropertySheet.AddPage  (pPage);
            }
            else if(pInfo->GenerationModel() == KEMGenerationRegression)
            {
                KGSimpleRegressionPage* pPage = new KGSimpleRegressionPage(&m_spKGFormulaDefaultDlg);
                pPage->SetParentSheet   (&oPropertySheet);
                pPage->SetTarget        (m_pTarget);
                pPage->SetGenerationInfo(pInfo);
				pPage->SetCaption(pInfo->GenerationModelName());
				pPage->SetDefaultPassengerInfo(pDefaultInfo);

                vecPageArray.push_back  (pPage);
                oPropertySheet.AddPage  (pPage);
            }
            else if (pInfo->GenerationModel() == KEMGenerationCrossClassification)
            {
                KGCategoryPage* pPage = new KGCategoryPage;
                pPage->SetParentSheet   (&oPropertySheet);
                pPage->SetTarget        (m_pTarget);
                pPage->SetGenerationInfo(pInfo);
				pPage->SetCaption(pInfo->GenerationModelName());
				pPage->SetDefaultPassengerInfo(pDefaultInfo);

                vecPageArray.push_back  (pPage);
                oPropertySheet.AddPage  (pPage);
            }
        }
	}

	oPropertySheet.SetNavigator(pTreeNav);
	oPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	oPropertySheet.RecalcLayout();
	oPropertySheet.SetResizable(TRUE);
    oPropertySheet.SetPageSize(CSize(254, 184));

	if(oPropertySheet.DoModal() == IDOK)
	{
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnInput.SetWindowText       (KR_UIXAML_G_INPUT_SELECTED);
			m_btnOutputDesign.SetWindowText(KR_UIXAML_G_OUTPUT_DEFAULT);
		}
		else {
			m_btnInput.SetWindowText       (UIXAML_G_INPUT_SELECTED);
			m_btnOutputDesign.SetWindowText(UIXAML_G_OUTPUT_DEFAULT);
		}
        m_btnOutputDesign.EnableWindow (TRUE);
		RunButtonControl();
		/*((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(FALSE);*/
	}

	std::vector<CXTPPropertyPage*>::iterator itPage, itPageEnd = vecPageArray.end();
	for(itPage = vecPageArray.begin(); itPage != itPageEnd; ++itPage)
	{
		delete *itPage;
	}

    KRedrawWindow();
	TxLogDebugEndMsg();
}


// Step 3 : Output 선택
void KGenerationModelDlg::OnBnClickedOutputdesign()
{
	KGOutputDesignDlg oDlg;
	oDlg.SetTarget(m_pTarget);
	oDlg.SetZoneTable(m_pZone);

	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		KGenerationInfo* pInfo = *itInfo;
		if(pInfo->UseGeneration() == true)
		{
			oDlg.AddGenerationInfo(pInfo);
		}
	}

	INT_PTR nResult = oDlg.DoModal();
	if(IDOK == nResult)
	{      
		if (KmzSystem::GetLanguage()==KEMKorea) {
			m_btnOutputDesign.SetWindowText(KR_UIXAML_G_OUTPUT_SELECTED);
		}
		else {
			m_btnOutputDesign.SetWindowText(UIXAML_G_OUTPUT_SELECTED);
		}
        
		m_btnRun.EnableWindow(TRUE);
		RunButtonControl();
	}
	/*((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(TRUE);*/
    KRedrawWindow();
}


void KGenerationModelDlg::RunButtonControl()
{
	UpdateData(TRUE);

	CString strButtonState = _T(""); 
	BOOL bButtonRun = TRUE;

	m_btnModel.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_G_MODEL_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_G_MODEL_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	m_btnInput.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_G_INPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_G_INPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}

	// 3. output button state
	m_btnOutputDesign.GetWindowText(strButtonState);
	if (KmzSystem::GetLanguage()==KEMKorea) {
		if( strButtonState.CompareNoCase(KR_UIXAML_G_OUTPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	else {
		if( strButtonState.CompareNoCase(UIXAML_G_OUTPUT_SELECTED) != 0 )
		{
			bButtonRun = FALSE;
		}
	}
	
	((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(bButtonRun);
}

// Step 4 : Run 버튼 클릭
void KGenerationModelDlg::OnBnClickedRun()
{
 	TxLogDebugStartMsg();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("통행발생 모형 실행 중..."));
	}
	else {
		AddStatusMessage(_T("Running Generation Model..."));
	}

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	try 
	{
		ThreadPara oPara(this);
		if (true) {
			oPara.TBusiness = 0;

			RThreadInfo.Init();
			QBicSimpleProgressThread::ExecuteThread(GenerationModelThreadCaller, &oPara, false, nLang);

			if (RThreadInfo.IsOK() == false) {
				ThrowException(RThreadInfo.ErrorMsg());
			}
		}
	}
	catch (int& ex) {
		CString strError(_T(""));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strError.Format(_T("오류 : %d"), ex);
		}
		else {
			strError.Format(_T("Error : %d"), ex);
		}
		TxLogDebug((LPCTSTR)strError);
		KRedrawWindow();
		return;
	}
	catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		KRedrawWindow();
		return;
	}
	catch (...) {
		TxLogDebugException();
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			AddStatusMessage(_T("Error Occurred while Running Model."));
		}
		KRedrawWindow();
		return;
	}

	m_pZone->BeginTransaction();
	try
	{
		ThreadPara oPara(this);
		if (true) {
			oPara.TBusiness = 1;
			oPara.TKeyInt[0] = 1; // commit 여부 1:true, 0:false

			RThreadInfo.Init();
			QBicSimpleProgressThread::ExecuteThread(GenerationModelThreadCaller, &oPara, false, nLang);

			if (RThreadInfo.IsOK() == false) {
				ThrowException(RThreadInfo.ErrorMsg());
			}
			else {

				if (1 == oPara.TKeyInt[0]) { // true
					m_pZone->Commit();
				}
				else { 
					m_pZone->Rollback();
				}

				/*m_pTarget->DetachDatabase(_T("sed"));*/
				//m_pTarget->DetachDatabase(_T("iolocal"));

				if (KmzSystem::GetLanguage() == KEMKorea) {
					AddStatusMessage(_T("모형 실행이 성공적으로 완료되었습니다."));
				}
				else {
					AddStatusMessage(_T("Running Complete Successfully."));
				}
			}
			// 성공
			InitButtonControl();

			m_btnViewTable.EnableWindow(TRUE);

			((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(FALSE);
			initGenerationInfo();
		}
	}
	catch (int& ex) {
		m_pZone->Rollback();

		CString strError(_T(""));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strError.Format(_T("오류 : %d"), ex);
		}
		else {
			strError.Format(_T("Error : %d"), ex);
		}
		TxLogDebug((LPCTSTR)strError);
	}
	catch (KExceptionPtr ex) {
		m_pZone->Rollback();

		TxExceptionPrint(ex);
	}
	catch (...) {
		m_pZone->Rollback();

		TxLogDebugException();
	}

	KRedrawWindow();
}


unsigned __stdcall KGenerationModelDlg::GenerationModelThreadCaller(void* a_pParam)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)a_pParam;
	if (nullptr != pParameter) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KGenerationModelDlg* pDlg = (KGenerationModelDlg*)pPara->TWindow;

			if (0 == pPara->TBusiness)
				pDlg->runModel();
			else
				pDlg->runModelPart2(pPara);
		}
	}

	return 1;
}


void KGenerationModelDlg::runModel(void)
{
	TxLogDebugStartMsg();
	
	try
	{
		CString strOutMsg = _T("");
		int nSelectedCount = UserSelectedModelInfo(strOutMsg);
		if ( nSelectedCount < 1 )
		{
			ThrowException(0, strOutMsg);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		if (KmzSystem::GetLanguage() == KEMKorea) {
			RThreadInfo.SetErrorFlag(-1, _T("오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			RThreadInfo.SetErrorFlag(-1, _T("Error Occurred while Running Model."));
		}
	}

	TxLogDebugEndMsg();
}


void KGenerationModelDlg::runModelPart2(ThreadPara* pPara)
{
	TxLogDebugStartMsg();

	try
	{
		std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
		for (itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
		{
			CTime timeStart, timeEnd;
			timeStart = CTime::GetCurrentTime();

			KGenerationInfo* pGInfo = *itInfo;
			if (pGInfo->UseGeneration())
			{
				if (pGInfo->ZoneGroup() == false)
				{
					if (runSimpleGeneration(pGInfo) == false)
					{
						pPara->TKeyInt[0] = 0; // false
						break;
					}
				}
				else
				{
					if (runGroupGeneration(pGInfo) == false)
					{
						pPara->TKeyInt[0] = 0; // false
						break;
					}
				}

				timeEnd = CTime::GetCurrentTime();
				recordGenerationResult(pGInfo,
					timeStart.Format(_T("%Y-%m-%d %H:%M:%S")),
					timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		if (KmzSystem::GetLanguage() == KEMKorea) {
			RThreadInfo.SetErrorFlag(-1, _T("오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			RThreadInfo.SetErrorFlag(-1, _T("Error Occurred while Running Model."));
		}
	}

	TxLogDebugEndMsg();
}


int KGenerationModelDlg::UserSelectedModelInfo( CString& strOutMsg )
{
	/*std::map<int, KPAPurpose*> mapPurpose;*/

	int nSelectedCount = 0;
	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		KGenerationInfo* pGInfo = *itInfo;
		// check - Input info
		if (pGInfo->UseGeneration())
		{
			nSelectedCount++;

			if(pGInfo->ZoneGroup() == true)
			{
				if( false == CheckZoneGroupInputInfo(pGInfo, strOutMsg) )
				{
					return -1;
				}
			}
			else
			{
				if(pGInfo->GenerationModel() == KEMGenerationUnit)
				{
					if( false == CheckUnitInputInfo(pGInfo, strOutMsg) )
					{
						return -1;
					}
				}
				else if(pGInfo->GenerationModel() == KEMGenerationRegression)
				{
					if( false == CheckRegressionInputInfo(pGInfo, strOutMsg) )
					{
						return -1;
					}
				}
				else if (pGInfo->GenerationModel() == KEMGenerationCrossClassification)
				{
					if( false == CheckCategoryExpressionInputInfo(pGInfo, strOutMsg) )
					{
						return -1;
					}
				}
			}

			/*KPAPurpose* pPurpose = pGInfo->Purpose();
			int nID =  pPurpose->PurposeID();
			std::map<int, KPAPurpose*>::iterator iter = mapPurpose.find(nID);
			if(iter != mapPurpose.end())
			{
				// 동일 id 존재 시 
				mapPurpose.erase(iter);
			}
			else
			{
				mapPurpose.insert(std::make_pair(nID, pPurpose));
			}*/
		}
	}

	// check - select Model
	if(nSelectedCount < 1)
	{
		strOutMsg = _T("선택한 정보가 없습니다.");
		return -1;
	}

	/*std::map<int, KPAPurpose*>::iterator iter    = mapPurpose.begin();
	std::map<int, KPAPurpose*>::iterator iterEnd = mapPurpose.end();

	// check - check output
	while( iter != iterEnd )
	{
		KPAPurpose* pPurpose = iter->second;
		KEMPAType   emPAType = pPurpose->PAType();
		CString     strPurposeName  = pPurpose->PurposeName();

		CString strEmptyPAType = _T("");
		// 없는 type 찾기 (반대 타입)
		if(KEMPATypeProduction == emPAType)
		{
			strEmptyPAType = (KmzSystem::IsPassengerSystem() == true) ? ATTRACTION_STRING : CONSUMPTION_STRING;
		}
		else
		{
			strEmptyPAType = PRODUCTION_STRING;
		}

		strOutMsg.Format(_T("%s 의 %s 정보가 존재 하지 않습니다. "), strPurposeName, strEmptyPAType);
		return -1;

		++iter;
	}*/

	return nSelectedCount;
}


bool KGenerationModelDlg::CheckZoneGroupInputInfo(KGenerationInfo* a_pGInfo, CString &a_strOutMsg)
{
	KGGroupExpressionContainer* pContainer = 
	(KGGroupExpressionContainer*)a_pGInfo->ExpressionContainer();
	if(NULL == pContainer)
	{
		pContainer = (KGGroupExpressionContainer*)(a_pGInfo->CreateExpression());
	}

	KZoneGroupColumn* pGroupRoot = pContainer->GetGroupRoot();
	if(NULL == pGroupRoot )
	{
		a_strOutMsg.Format(_T("\'%s\' 에 대한 Zone group 입력 수식을 설정해 주세요."), a_pGInfo->GetName());
		return false;
	}

	// 1. check KZoneGroupColumn
	std::list<TGroupColumnInfo> lstRemainColumns;
	KZoneGroupValue* pValue = pGroupRoot->GetFirstZoneGroupValue();
		
	while( pValue != NULL )
	{
		// sub column을 가지고 있으면 column을 추가
		if(pValue->HasSubColumn() == true)
		{
			TGroupColumnInfo oTGci;
			oTGci.pColumn = pValue->GetSubColumn();
			lstRemainColumns.push_back(oTGci);
		}
		else	// 그렇지 않으면 expression을 설정
		{
			if( false == pValue->DoneExpression() )
			{
				a_strOutMsg.Format(_T("\'%s\' 에 대한 Zone group 입력 수식을 설정해 주세요."), a_pGInfo->GetName());
				return false;
			}
		}
		pValue = pGroupRoot->GetNextZoneGroupValue();
	}
				
	//2. check KZoneGroupValue
	while(lstRemainColumns.empty() == false)
	{
		TGroupColumnInfo g = lstRemainColumns.front();
		KZoneGroupColumn* pColumn = g.pColumn;
		CXTPReportRecord* pRecord = g.pParent;

		KZoneGroupValue* pValue = pColumn->GetFirstZoneGroupValue();
		while(NULL != pValue)
		{
			// sub column을 가지고 있으면 column을 추가
			if(pValue->HasSubColumn() == true)
			{
				TGroupColumnInfo gci;
				gci.pColumn = pValue->GetSubColumn();
				lstRemainColumns.push_back(gci);
			}
			else	// 그렇지 않으면 expression을 설정
			{
				if( false == pValue->DoneExpression() )
				{
					a_strOutMsg.Format(_T("\'%s\' 에 대한 Zone group 입력 수식을 설정해 주세요."), a_pGInfo->GetName());

					return false;
				}
			}
			pValue = pColumn->GetNextZoneGroupValue();
		}
		lstRemainColumns.pop_front();
	}

	return true;
}


bool KGenerationModelDlg::CheckUnitInputInfo(KGenerationInfo* a_pGInfo, CString &a_strOutMsg)
{
	KGSimpleExpressionContainer* pContainer = 
		(KGSimpleExpressionContainer*)a_pGInfo->ExpressionContainer();
	if(NULL == pContainer)
	{
		pContainer = (KGSimpleExpressionContainer*)(a_pGInfo->CreateExpression());
	}

	KGUnitExpression* pExpression = (KGUnitExpression*)(pContainer->ExpressionObject());
	if(NULL == pExpression)
	{
		pExpression = (KGUnitExpression*)(pContainer->CreateExpressionObject());
	}
	
	CString strCaption = pExpression->FieldCaption();
	CString strName    = pExpression->FieldName();
	if( strCaption.Compare(_T("")) == 0 || strName.Compare(_T("")) == 0 )
	{
		a_strOutMsg.Format(_T("\'%s\' 에 대한 Unit 입력 수식이 없습니다."), a_pGInfo->GetName());
		return false;
	}

	return true;
}


bool KGenerationModelDlg::CheckRegressionInputInfo(KGenerationInfo* a_pGInfo, CString &a_strOutMsg)
{
	KGSimpleExpressionContainer* pContainer = 
		(KGSimpleExpressionContainer*)a_pGInfo->ExpressionContainer();
	if(NULL == pContainer)
	{
		pContainer = (KGSimpleExpressionContainer*)(a_pGInfo->CreateExpression());
	}

	KGRegressionExpression* pExpression = (KGRegressionExpression*)(pContainer->ExpressionObject());
	if(NULL == pExpression)
	{
		pExpression = (KGRegressionExpression*)(pContainer->CreateExpressionObject());
	}

	CString strCaption = pExpression->GetCaption();
	if( strCaption.Compare(_T("")) == 0 )
	{
		a_strOutMsg.Format(_T("\'%s\' 에 대한 Regression 입력 수식이 없습니다."), a_pGInfo->GetName());
		return false;
	}

	return true;
}


bool KGenerationModelDlg::CheckCategoryExpressionInputInfo(KGenerationInfo* a_pGInfo, CString &a_strOutMsg)
{
	KGSimpleExpressionContainer* pContainer = 
		(KGSimpleExpressionContainer*)a_pGInfo->ExpressionContainer();
	if(NULL == pContainer)
	{
		pContainer = (KGSimpleExpressionContainer*)(a_pGInfo->CreateExpression());
	}

	KGCategoryExpression* pExpression = (KGCategoryExpression*)(pContainer->ExpressionObject());
	if(NULL == pExpression)
	{
		pExpression = (KGCategoryExpression*)(pContainer->CreateExpressionObject());
	}

	if( 0 == pExpression->m_category_id)
	{
		a_strOutMsg.Format(_T("\'%s\' 에 대한 Cross-Classification 입력 정보가 없습니다."), a_pGInfo->GetName());
		return false;
	}

	return true;
}


bool KGenerationModelDlg::runSimpleGeneration(KGenerationInfo* a_pGInfo)
{
    TxLogDebugStartMsg();

	bool bResult = false;
	KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)(a_pGInfo->ExpressionContainer());

	KGBaseExpression* pBase = pContainer->ExpressionObject();
	if(NULL == pBase)
	{
		return false;
	}

	KEMGenerationModel emGModel = pBase->GenerationModel();
	if(KEMGenerationUnit == emGModel)
	{
		bResult = runUnit(a_pGInfo, pBase);
	}
	else if(KEMGenerationRegression == emGModel)
	{
		bResult = runRegression(a_pGInfo, pBase);
	}
    else if(KEMGenerationCrossClassification == emGModel)
    {        
        bResult = runCrossClassfication(a_pGInfo, pBase, m_vecSEDPopRecords);
    }

    TxLogDebugEndMsg();
	return bResult;
}


bool KGenerationModelDlg::runGroupGeneration(KGenerationInfo* a_pGInfo)
{
	// todo : Zone group generation 구현
    TxLogDebugStartMsg();
	if(NULL == m_pTarget)
	{
		return false;
	}

	KGGroupExpressionContainer* pContainer = 
		(KGGroupExpressionContainer*)(a_pGInfo->ExpressionContainer());
	KZoneGroupColumn* pRoot = pContainer->GetGroupRoot();
	if(NULL == pRoot)
	{
		return false;
	}

	// Table 정의
	CString strTables = _T("zone, sed_vector");

	// Filter 정의
	CString strWhere = _T("zone.zone_id = sed_vector.zone_id");
	std::list<KZoneGroupBase*> lstGroupList;
	lstGroupList.push_back(pRoot);
	KZoneGroupValue* pFirstValue = pRoot->GetFirstZoneGroupValue();
	if(NULL != pFirstValue)
	{
		lstGroupList.push_back(pFirstValue);
	}

	while(lstGroupList.empty() == false)
	{
		KZoneGroupBase* pBase = lstGroupList.back();
		if(pBase->ZoneGroupObjectType() == KEMZoneGroupValue)	// Group Value일 경우
		{
			KZoneGroupValue* pGroupValue = (KZoneGroupValue*)pBase;
			if(pGroupValue->HasSubColumn() == true)
			{
				KZoneGroupColumn* pGroupColumn = pGroupValue->GetSubColumn();
				lstGroupList.push_back(pGroupColumn);
				pFirstValue = pGroupColumn->GetFirstZoneGroupValue();
				lstGroupList.push_back(pFirstValue);
			}
			else
			{
				CString strFilteryear =_T("");
				//strFilteryear.Format(_T(" AND year = '%d'"), m_nTargetYear);
                CString strGroupFilter     = genGroupFilterString(lstGroupList);
				CString strJoinGroupFilter = strWhere + _T(" And ") + strGroupFilter + strFilteryear;

				ASSERT(strGroupFilter.IsEmpty() == FALSE);

				KGBaseExpression* pExpressionBase = pGroupValue->ExpressionObject();
				if(NULL != pExpressionBase)
				{
					KEMGenerationModel emGModel = pExpressionBase->GenerationModel();
					if(KEMGenerationUnit == emGModel)
					{
						if(runUnit(a_pGInfo, strTables, strJoinGroupFilter, pExpressionBase) == false)
						{
							return false;
						}
					}
					else if(KEMGenerationRegression == emGModel)
					{
						if(runRegression(a_pGInfo, strTables, strJoinGroupFilter, pExpressionBase) == false)
						{
							return false;
						}
					}
                    else if(KEMGenerationCrossClassification == emGModel)
                    {
                        if(runCrossClassfication(a_pGInfo, pExpressionBase, m_vecSEDPopRecords, strGroupFilter) == false)
                        {
                            return false;
                        }
                    }
				}
				lstGroupList.pop_back();
			}
		}
		else	// Group Column일 경우
		{
			KZoneGroupColumn* pGroupColumn = (KZoneGroupColumn*)pBase;
			KZoneGroupValue* pGroupValue = pGroupColumn->GetNextZoneGroupValue();
			if(NULL == pGroupValue)
			{
				lstGroupList.pop_back();
				if(lstGroupList.empty() == false)
				{
					// 이 column의 parent가 value라면 value도 삭제
					pBase = lstGroupList.back();
					if(NULL != pBase)
					{
						ASSERT(pBase->ZoneGroupObjectType() == KEMZoneGroupValue);
						lstGroupList.pop_back();
					}
				}
			}
			else
			{
				lstGroupList.push_back(pGroupValue);
			}
		}
	}

    TxLogDebugEndMsg();
	return true;
}


bool KGenerationModelDlg::runUnit(KGenerationInfo* a_pGInfo, KGBaseExpression* a_pExpression)
{
    TxLogDebugStartMsg();
	KGUnitExpression* pUnit = (KGUnitExpression*)a_pExpression;
	
	KIOTables* pTables = m_pTarget->Tables();
	KIOTable* pSEDVector = pTables->FindTable(_T("sed_vector"));
	if(NULL == pSEDVector)
	{
		return false;
	}

	const KIOColumns* pColumns = pSEDVector->Columns();
	KIOColumn* pColumn = NULL;
	KIOColumns reqColumns;

	// ID column 추가
	pColumn = pColumns->GetColumn(_T("zone_id"));
	reqColumns.AddNewColumn(pColumn);

	// unit column 추가
	pColumn = pColumns->GetColumn(pUnit->FieldName());
	if(NULL == pColumn)
	{
		return false;
	}

	reqColumns.AddNewColumn(pColumn);
	CString strFilterYear =_T("");
	//strFilterYear.Format(_T("year = '%d'"), m_nTargetYear);

	KIORecordset* pRecordset = pSEDVector->GetRecordset(KEMIORecordsetTypeStatic, &reqColumns, strFilterYear, _T(""));
	if(NULL == pRecordset)
	{
		return false;
	}
	
	KDoubleRecordArray oSEDArray(1);
	KDoubleRecord* pRecord = NULL;

	int nRowCount = pRecordset->GetRowCount();
	for(int i = 0; i < nRowCount; ++i)
	{
		const KIORow* pRow = pRecordset->GetRow(i);
		KIOItem* pItem = pRow->GetItem(1);
		if(NULL != pItem)
		{
			pRecord = oSEDArray.AddNewRecord();
			pRecord->SetAt(0, pItem->GetValueAsDouble());
		}
	}

	CString strFunction = pUnit->GetExpression();
	KParseVariables oVariables;
	KParseVariableInfo* pVarInfo = oVariables.AddNewVariable();
	pVarInfo->SetVariable(pUnit->FieldName(), 0);

	CAtlArray<double> result;
	KGeneration gen;

	int nRet = gen.MuExecute(strFunction, oVariables, oSEDArray, result, NULL);

	bool bResult = false;
	if(1 == nRet)
	{
		bResult = updateZoneData(a_pGInfo, pRecordset, result);
	}

	delete pRecordset;

    TxLogDebugEndMsg();
	return bResult;
}


bool KGenerationModelDlg::runUnit(KGenerationInfo* a_pGInfo, 
							LPCTSTR a_lpcszTables, LPCTSTR a_lpcszFilter, KGBaseExpression* a_pExpression)
{
    TxLogDebugStartMsg();
	KGUnitExpression* pUnit = (KGUnitExpression*)a_pExpression;

	// SED data 확인
	KIOTables* pTables = m_pTarget->Tables();
	KIOTable* pSEDVector = pTables->FindTable(_T("sed_vector"));
	if(NULL == pSEDVector)
	{
		return false;
	}

	const KIOColumns* pColumns = pSEDVector->Columns();
	KIOColumn* pColumn = NULL;
	KIOColumns oReqColumns;

	// ID column 추가
	pColumn = oReqColumns.AddNewColumn();
	pColumn->Name(_T("sed_vector.zone_id"));

	// unit column 추가
	pColumn = pColumns->GetColumn(pUnit->FieldName());
	if(NULL == pColumn)
	{
		return false;
	}

	oReqColumns.AddNewColumn(pColumn);

	KIORecordset* pRecordset = m_pZone->GetRecordset(KEMIORecordsetTypeStatic, &oReqColumns, a_lpcszTables, a_lpcszFilter, _T(""));
	if(NULL == pRecordset)
	{
		return false;
	}

	KDoubleRecordArray oSEDArray(1);
	KDoubleRecord* pRecord = NULL;

	int nRowCount = pRecordset->GetRowCount();

	for(int i = 0; i < nRowCount; ++i)
	{
		const KIORow* pRow = pRecordset->GetRow(i);
		KIOItem* pItem = pRow->GetItem(1);
		if(NULL != pItem)
		{
			pRecord = oSEDArray.AddNewRecord();
			pRecord->SetAt(0, pItem->GetValueAsDouble());
		}
	}

	CString strFunction = pUnit->GetExpression();
	KParseVariables oVariables;
	KParseVariableInfo* pVarInfo = oVariables.AddNewVariable();
	pVarInfo->SetVariable(pUnit->FieldName(), 0);

	CAtlArray<double> result;
	KGeneration gen;

	int nRet = gen.MuExecute(strFunction, oVariables, oSEDArray, result, NULL);

	bool bResult = false;
	if(1 == nRet)
	{
		bResult = updateZoneData(a_pGInfo, pRecordset, result);
	}

	delete pRecordset;
    TxLogDebugEndMsg();
	return bResult;
}

// nombara.. 2012.05.30
bool KGenerationModelDlg::runRegression(KGenerationInfo* a_pGInfo, KGBaseExpression* a_pExpression)
{
    TxLogDebugStartMsg();

	KGRegressionExpression* pRegression = (KGRegressionExpression*)a_pExpression;	

	KIOTables* pTables    = m_pTarget->Tables();
	KIOTable*        pSEDVector = pTables->FindTable(_T("sed_vector"));
	if(NULL == pSEDVector)
	{
        AfxMessageBox(_T("사회경제지표 데이터 테이블이 존재하지 않습니다."));
		return false;
	}

    KParseExpression oParseExpression;
    oParseExpression.SetTable(pSEDVector);
    oParseExpression.SetCaptionExpression(pRegression->GetCaptionExpression());
    std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
    
    CString strFunction = oParseExpression.GetRealExpression();

	const KIOColumns* pColumns = pSEDVector->Columns();	
    KIOColumns        oReqColumns;
	KIOColumn*                 pColumn  = NULL;

	// ID Column 추가
	pColumn = pColumns->GetColumn(COLUMN_ZONE_ID);
	oReqColumns.AddNewColumn(pColumn);
	
	// SED Column 추가
    size_t nxCount = vecUsedColumnName.size();
    for (size_t i=0; i<nxCount; i++)
    {
        pColumn = oReqColumns.AddNewColumn();
        pColumn->Name(vecUsedColumnName[i]);
    }
	
	CString strFilter =_T("");
	KIORecordset* pRecordset = pSEDVector->GetRecordset(KEMIORecordsetTypeStatic, &oReqColumns, strFilter, _T(""));
	if(NULL == pRecordset)
	{
		return false;
	}

	int nReqColumnCount = oReqColumns.ColumnCount();
	KDoubleRecordArray    oSedArray(nReqColumnCount - 1);
	KDoubleRecord*        pRecord = NULL;
    
	int nRowCount = pRecordset->GetRowCount();

	for(int i=0; i<nRowCount; ++i)
	{
		const KIORow* pRow = pRecordset->GetRow(i);
		pRecord = oSedArray.AddNewRecord();
		for(int j = 1; j < nReqColumnCount; ++j)
		{
			KIOItem* pItem = pRow->GetItem(j);
			if(NULL != pItem)
			{
				pRecord->SetAt(j-1, pItem->GetValueAsDouble());
			}
		}		
	}

    KParseVariables       variables;
    KParseVariableInfo*   pVarInfo = NULL;

    for(int j = 1; j < nReqColumnCount; ++j)
    {
        pVarInfo = variables.AddNewVariable();
        pColumn  = oReqColumns.GetColumn(j);
        pVarInfo->SetVariable(pColumn->Name(), j - 1);
    }		
    	
	CAtlArray<double> result;
	KGeneration gen;

	int nRet = gen.MuExecute(strFunction, variables, oSedArray, result, NULL);

	bool bResult = false;
	if(1 == nRet)
	{
		bResult = updateZoneData(a_pGInfo, pRecordset, result);
	}

	delete pRecordset;

    TxLogDebugEndMsg();
	return bResult;
}


bool KGenerationModelDlg::runRegression(KGenerationInfo* a_pGInfo, LPCTSTR a_lpcszTables, 
										LPCTSTR a_lpcszFilter, KGBaseExpression* a_pExpression)
{
    TxLogDebugStartMsg();
	KGRegressionExpression* pRegression = (KGRegressionExpression*)a_pExpression;
    
	KIOTables*  pTables    = m_pTarget->Tables();
	KIOTable*         pSEDVector = pTables->FindTable(_T("sed_vector"));
	if(NULL == pSEDVector)
	{
        AfxMessageBox(_T("사회경제지표 데이터 테이블이 존재하지 않습니다."));
		return false;
	}

    KParseExpression oParseExpression;
    oParseExpression.SetTable(pSEDVector);
    oParseExpression.SetCaptionExpression(pRegression->GetCaptionExpression());
    std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();

    CString strFunction = oParseExpression.GetRealExpression();

	const KIOColumns* pColumns = pSEDVector->Columns();
	KIOColumns        oReqColumns;
	KIOColumn*        pColumn = NULL;

	// ID Column 추가
	pColumn = oReqColumns.AddNewColumn();
	pColumn->Name(_T("sed_vector.zone_id"));

	// SED Column 추가
    size_t nxCount = vecUsedColumnName.size();
    for (size_t i=0; i<nxCount; i++)
    {
        pColumn = oReqColumns.AddNewColumn();
        pColumn->Name(vecUsedColumnName[i]);
    }

	KIORecordset* pRecordset = m_pZone->GetRecordset(KEMIORecordsetTypeStatic, &oReqColumns, a_lpcszTables, a_lpcszFilter, _T(""));
	if(NULL == pRecordset)
	{
        AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}

	int nReqColumnCount = oReqColumns.ColumnCount();
	KDoubleRecordArray oSEDArray(nReqColumnCount - 1);
	KDoubleRecord* pRecord = NULL;
	
	int nRowCount = pRecordset->GetRowCount();

	for(int i = 0; i < nRowCount; ++i)
	{
		const KIORow* pRow = pRecordset->GetRow(i);
		pRecord = oSEDArray.AddNewRecord();
		for(int j = 1; j < nReqColumnCount; ++j)
		{
			KIOItem* pItem = pRow->GetItem(j);
			if(NULL != pItem)
			{
				pRecord->SetAt(j-1, pItem->GetValueAsDouble());				
			}
		}
	}

    KParseVariables     oVariables;
    KParseVariableInfo* pVarInfo = NULL;

    for(int j = 1; j < nReqColumnCount; ++j)
    {
        pVarInfo = oVariables.AddNewVariable();
        pColumn = oReqColumns.GetColumn(j);
        pVarInfo->SetVariable(pColumn->Name(), j - 1);
    }

	CAtlArray<double> result;
	KGeneration gen;

	int nRet = gen.MuExecute(strFunction, oVariables, oSEDArray, result, NULL);

	bool bResult = false;
	if(1 == nRet)
	{
		bResult = updateZoneData(a_pGInfo, pRecordset, result);
	}

	delete pRecordset;
    TxLogDebugEndMsg();
	return bResult;
}


bool KGenerationModelDlg::runCrossClassfication(KGenerationInfo* a_pGInfo, KGBaseExpression* a_pExpression, std::vector<TFixedSEDPop>& a_vecSEDPopRecords)
{
    TxLogDebugStartMsg();
    KGBaseExpressionContainer* pBaseCont = a_pGInfo->ExpressionContainer();
    if(NULL == pBaseCont)
    {
        return false;
    }

    KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;
    KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
    if(NULL == pBaseExp)
    {
        return false;
    }

    KGCategoryExpression* pCategoryExpression = (KGCategoryExpression*)pBaseExp;

    std::map <KCategoryKey, double>& mapCategoryUnitValues = pCategoryExpression->m_categoryUnitValues;
    bool isSexUse = pCategoryExpression->m_useSex > 0 ? true : false;
    bool isAgeUse = pCategoryExpression->m_useAge > 0 ? true : false;

    std::map<Integer, double > mapZoneRecords;
    KBulkDBase::BulkZoneSelect(m_pZone, _T("0"), mapZoneRecords);

    size_t nSedCount = a_vecSEDPopRecords.size();

    std::map<Integer, double >::iterator iter ;
    std::map<Integer, double >::iterator end  = mapZoneRecords.end();

    CString strOut;

    for (size_t i=0; i<nSedCount; i++)
    {
        TFixedSEDPop& sedRecord = a_vecSEDPopRecords[i];
        iter = mapZoneRecords.find(sedRecord.zone_id);
        if (iter != end)
        {
            double& dValue = iter->second;
            dValue = dValue + sedRecord.value * pCategoryExpression->GetUnitValue(sedRecord.sex, sedRecord.age);
        }
    }
    
    bool bResult = updateZoneData(a_pGInfo, mapZoneRecords);
    TxLogDebugEndMsg();
    return bResult;
}


bool KGenerationModelDlg::runCrossClassfication(KGenerationInfo* a_pGInfo, KGBaseExpression* a_pExpression, std::vector<TFixedSEDPop>& a_vecSEDPopRecords, CString a_strGroupFilter)
{
    TxLogDebugStartMsg();
    
    KGBaseExpression* pBaseExp = a_pExpression;
    if(NULL == pBaseExp)
    {
        return false;
    }

    KGCategoryExpression* pCategoryExpression = (KGCategoryExpression*)pBaseExp;

    std::map <KCategoryKey, double>& mapCategoryUnitValues = pCategoryExpression->m_categoryUnitValues;
    bool isSexUse = pCategoryExpression->m_useSex > 0 ? true : false;
    bool isAgeUse = pCategoryExpression->m_useAge > 0 ? true : false;

    std::map<Integer, double > mapZoneRecords;
    KBulkDBase::BulkZoneSelect(m_pZone, _T("0"), mapZoneRecords, a_strGroupFilter);

    size_t nSedCount = a_vecSEDPopRecords.size();

    std::map<Integer, double >::iterator iter ;
    std::map<Integer, double >::iterator end  = mapZoneRecords.end();

    CString strOut;

    for (size_t i=0; i<nSedCount; i++)
    {
        TFixedSEDPop& sedRecord = a_vecSEDPopRecords[i];
        iter = mapZoneRecords.find(sedRecord.zone_id);
        if (iter != end)
        {
            double& dValue = iter->second;
            dValue = dValue + sedRecord.value * pCategoryExpression->GetUnitValue(sedRecord.sex, sedRecord.age);
        }
    }

    bool bResult = updateZoneData(a_pGInfo, mapZoneRecords);
    TxLogDebugEndMsg();
    return bResult;
}


bool KGenerationModelDlg::updateZoneData(KGenerationInfo* a_pGInfo, KIORecordset* a_pRecordset, CAtlArray<double>& result)
{
    TxLogDebugStartMsg();
	int nRecordCount = a_pRecordset->GetRowCount();
	ASSERT(result.GetCount() == nRecordCount);

	const KIOColumns * pColumns = m_pZone->Columns();
	KIOColumn* pOutputColumn = pColumns->GetColumn(a_pGInfo->OutputColumnName());

	if(NULL == pOutputColumn)
	{
		KIOColumn oColumn;
		oColumn.Name(m_pZone->GetUsableColumnName());
		oColumn.Caption(a_pGInfo->OutputColumnCaption());
		oColumn.CodeType(KEMIOCodeTypeIsNothing);
		oColumn.ColumnType(KEMIOColumnTypeUserDefine);
		oColumn.DataType(KEMIODataTypeDouble);
		oColumn.DisplayOrder(pColumns->ColumnCount()+1);
		oColumn.PrimaryKey(false);
		oColumn.Visible(true);
		pOutputColumn = m_pZone->AddColumn(oColumn);
		if(NULL == pOutputColumn)
		{
			AfxMessageBox(_T("컬럼 추가에 실패하였습니다."));
			return false;
		}
		a_pGInfo->OutputColumnName(pOutputColumn->Name());
	}

	a_pGInfo->OutputColumnCaption(_T(""));
	KIOColumns oColumns;
	oColumns.AddNewColumn(pOutputColumn);

	bool bResult = true;
	for(int i = 0; i < nRecordCount; ++i)
	{
		KIORow resultRow;
		KIOItem* pResultItem = resultRow.AddNewItem();
		pResultItem->SetValue(result.GetAt(i));

		const KIORow* pRow = a_pRecordset->GetRow(i);
		KIOItem* pItem = pRow->GetItem(0);
		CString strFilter;
		strFilter.Format(_T("zone_id = %s"), pItem->GetValueAsString());
		m_pZone->UpdateRow(&oColumns, &resultRow, strFilter);
	}

    TxLogDebugEndMsg();
	return bResult;
}


bool KGenerationModelDlg::updateZoneData(KGenerationInfo* pGInfo, std::map<Integer, double>& a_mapZoneRecords)
{
    TxLogDebugStartMsg();
    const KIOColumns * pColumns = m_pZone->Columns();
    KIOColumn* pOutputColumn = pColumns->GetColumn(pGInfo->OutputColumnName());

    if(NULL == pOutputColumn)
    {
        KIOColumn oColumn;
        oColumn.Name(m_pZone->GetUsableColumnName());
        oColumn.Caption(pGInfo->OutputColumnCaption());
        oColumn.CodeType(KEMIOCodeTypeIsNothing);
        oColumn.ColumnType(KEMIOColumnTypeUserDefine);
        oColumn.DataType(KEMIODataTypeDouble);
        oColumn.DisplayOrder(pColumns->ColumnCount()+1);
        oColumn.PrimaryKey(false);
        oColumn.Visible(true);
        pOutputColumn = m_pZone->AddColumn(oColumn);
        if(NULL == pOutputColumn)
        {
            AfxMessageBox(_T("컬럼 추가에 실패하였습니다."));
            return false;
        }
        pGInfo->OutputColumnName(pOutputColumn->Name());
    }
	pGInfo->OutputColumnCaption(_T(""));
    bool bResult = KBulkDBase::BulkZoneUpdate(m_pZone, pOutputColumn, a_mapZoneRecords, false);
    TxLogDebugEndMsg();
    return bResult;
}


CString KGenerationModelDlg::genGroupFilterString(std::list<KZoneGroupBase*>& a_lstgroupList)
{
    TxLogDebugStartMsg();
	CString strFilter = _T(" ");
	ASSERT(a_lstgroupList.size() % 2 == 0);

    bool isFirst = true;

	std::list<KZoneGroupBase*>::iterator itGroup, itEnd = a_lstgroupList.end();
	for(itGroup = a_lstgroupList.begin(); itGroup != itEnd; ++itGroup)
	{
		KZoneGroupBase* pBase = *itGroup;
		if(pBase->ZoneGroupObjectType() == KEMZoneGroupColumn)
		{
            if (isFirst)
            {
                isFirst = false;
            }
            else
            {
			    strFilter += _T(" AND ");
            }
			strFilter += ((KZoneGroupColumn*)pBase)->ColumnName();
			strFilter += _T(" = ");
		}
		else
		{
			CString strValue;
			strValue.Format(_T("%d"), ((KZoneGroupValue*)pBase)->Value());
			strFilter += strValue;
			strFilter += _T(" ");
		}
	}

    TxLogDebugStartMsg();
	return strFilter;
}


void KGenerationModelDlg::recordGenerationResult(KGenerationInfo* a_pGInfo, LPCTSTR a_lpcszStartDate, LPCTSTR a_lpcszEndDate)
{
    TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" INSERT OR REPLACE INTO generation_model ")
		_T(" (object_group_code, detail_object_id, pa_flag, zone_column_name, start_date, end_date, generation_run_seq) ")
		_T(" VALUES ('%d', '%d', '%c', '%s', '%s', '%s', %s) "),
		a_pGInfo->GetObjectGroupID(), a_pGInfo->GetObjectID(), 
		a_pGInfo->GetPAType()== KEMPATypeProduction ? _T('p') : _T('a'),
		a_pGInfo->OutputColumnName(), 
		a_lpcszStartDate, a_lpcszEndDate, _T(" (select ifnull(max(generation_run_seq)+1, 1) from Generation_Model) ")
		);

	try
	{
		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
        TxLogDebugException();
	}

    TxLogDebugEndMsg();
}


//Step 5 : Table 정보 보기
void KGenerationModelDlg::OnBnClickedOutputtable()
{
	ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_ZONE);
    KRedrawWindow();
}


//Step 6 : 취소 버튼
void KGenerationModelDlg::OnBnClickedCancel()
{
	OnCancel();
}


void KGenerationModelDlg::OnActivate(UINT a_nState, CWnd* a_pWndOther, BOOL a_bMinimized)
{
	TxLogDebugStartMsg();
	KDialogEx::OnActivate(a_nState, a_pWndOther, a_bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KRedrawWindow();
	TxLogDebugEndMsg();
}

void KGenerationModelDlg::InitDefaultValue()
{
	try
	{
		CleareDefaultData();
		KDBaseGenerationParaModel::InitDefaultGenerationData(m_pTarget, m_mapGenerationInfo);
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

void KGenerationModelDlg::CleareDefaultData()
{
	TxLogDebugStartMsg();
	try
	{
		AutoType iter = m_mapGenerationInfo.begin();
		AutoType end  = m_mapGenerationInfo.end();
		while(iter != end)
		{
			std::map<KIntKey, KGenerationInfo*> mapGeneration = iter->second;

			AutoType iterSub = mapGeneration.begin();
			AutoType endSub	 = mapGeneration.end();
			while(iterSub != endSub)
			{
				KGenerationInfo* pInfo = iterSub->second;
				if (pInfo != nullptr)
				{
					QBicDelete(pInfo);
				}
				iterSub++;
			}
			mapGeneration.clear();
			++iter;
		}
		m_mapGenerationInfo.clear();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}

void KGenerationModelDlg::SetDefaultData()
{
	try
	{
		std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
		for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
		{
			KGenerationInfo* pInfo = *itInfo;
			if (pInfo->UseGeneration() == false)
			{
				continue;
			}
			
			int nPurspoeID	= pInfo->Purpose()->PurposeID();
			int nPAType		= pInfo->GetPAType();
			int nModelType	= pInfo->GenerationModel();
			int nZoneGroup(0);
			if (pInfo->ZoneGroup() == true)
			{
				nZoneGroup = 1;
			}

			KIntKey oFirstKey(nPurspoeID, nPAType);
			KIntKey oSecondKey(nZoneGroup, nModelType);

			AutoType Ffind = m_mapGenerationInfo.find(oFirstKey);
			AutoType Fend  = m_mapGenerationInfo.end();
			if (Ffind != Fend)
			{
				std::map<KIntKey, KGenerationInfo*> mapGenerationDefault = Ffind->second;
				AutoType Sfind = mapGenerationDefault.find(oSecondKey);
				AutoType Send  = mapGenerationDefault.end();
				if (Sfind != Send)
				{
					KGenerationInfo* pInitInfo = Sfind->second;
					pInfo->ExpressionContainer();
					SetExpressionData(pInfo, pInitInfo);
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

KGenerationInfo* KGenerationModelDlg::GetDefaultData( KGenerationInfo* a_pGenerationInfo)
{
	try
	{
		if (a_pGenerationInfo->UseGeneration() == false)
		{
			return nullptr;
		}

		int nPurspoeID	= a_pGenerationInfo->Purpose()->PurposeID();
		int nPAType		= a_pGenerationInfo->GetPAType();
		int nModelType	= a_pGenerationInfo->GenerationModel();
		int nZoneGroup(0);
		if (a_pGenerationInfo->ZoneGroup() == true)
		{
			nZoneGroup = 1;
		}

		KIntKey oFirstKey(nPurspoeID, nPAType);
		KIntKey oSecondKey(nZoneGroup, nModelType);

		AutoType Ffind = m_mapGenerationInfo.find(oFirstKey);
		AutoType Fend  = m_mapGenerationInfo.end();
		if (Ffind != Fend)
		{
			std::map<KIntKey, KGenerationInfo*> mapGenerationDefault = Ffind->second;
			AutoType Sfind = mapGenerationDefault.find(oSecondKey);
			AutoType Send  = mapGenerationDefault.end();
			if (Sfind != Send)
			{
				KGenerationInfo* pInfo = Sfind->second;
				return pInfo; 
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

	return nullptr;
}

void KGenerationModelDlg::SetExpressionData( KGenerationInfo* a_pInfo, KGenerationInfo* a_pDefaultInfo )
{
	try
	{
		if (a_pInfo->GenerationModel() == KEMGenerationUnit && a_pInfo->ZoneGroup() == false)
		{
			KGBaseExpressionContainer* pDefCont = a_pDefaultInfo->ExpressionContainer();
			if (nullptr == pDefCont)
			{
				return;
			}

			KGSimpleExpressionContainer* pDefContainer = (KGSimpleExpressionContainer*)pDefCont;
			if (pDefContainer == nullptr)
			{
				return;
			}

			if (pDefContainer->GenerationModel() != KEMGenerationUnit)
			{
				return;
			}

			KGBaseExpression* pDefExp = pDefContainer->ExpressionObject();
			if (pDefExp == nullptr)
			{
				return;
			}

			KGUnitExpression* pDefUnitExp = (KGUnitExpression*)pDefExp;


			KGSimpleExpressionContainer* pContainer = 
				(KGSimpleExpressionContainer*)a_pInfo->ExpressionContainer();
			if(NULL == pContainer)
			{
				pContainer = (KGSimpleExpressionContainer*)(a_pInfo->CreateExpression());
			}

			KGUnitExpression* pExpression = (KGUnitExpression*)(pContainer->ExpressionObject());
			if(NULL == pExpression)
			{
				pExpression = (KGUnitExpression*)(pContainer->CreateExpressionObject());
			}

			*pExpression = *pDefUnitExp;

		}
		else if (a_pInfo->GenerationModel() == KEMGenerationRegression && a_pInfo->ZoneGroup() == false)
		{
			KGBaseExpressionContainer* pBaseCont = a_pDefaultInfo->ExpressionContainer();
			if (pBaseCont == nullptr)
			{
				return;
			}

			KGSimpleExpressionContainer* pDefContainer = (KGSimpleExpressionContainer*)pBaseCont;
			if (nullptr == pDefContainer)
			{
				return;
			}

			KGBaseExpression* pBaseExp = pDefContainer->ExpressionObject();
			if(NULL == pBaseExp)
			{
				return;
			}

			KGRegressionExpression* pDefRegExp	= (KGRegressionExpression*)(pBaseExp);

			KGSimpleExpressionContainer* pContainer = 
				(KGSimpleExpressionContainer*)a_pInfo->ExpressionContainer();
			if(NULL == pContainer)
			{
				pContainer = (KGSimpleExpressionContainer*)(a_pInfo->CreateExpression());
			}

			KGRegressionExpression* pExpression = (KGRegressionExpression*)(pContainer->ExpressionObject());
			if(NULL == pExpression)
			{
				pExpression = (KGRegressionExpression*)(pContainer->CreateExpressionObject());
			}

			*pExpression = *pDefRegExp;


		}
		else if (a_pInfo->GenerationModel() == KEMGenerationCrossClassification && a_pInfo->ZoneGroup() == false)
		{
			KGBaseExpressionContainer* pBaseCont = a_pDefaultInfo->ExpressionContainer();
			if(NULL == pBaseCont)
			{
				return;
			}

			KGSimpleExpressionContainer* pDefContainer = (KGSimpleExpressionContainer*)pBaseCont;
			KGBaseExpression* pBaseExp = pDefContainer->ExpressionObject();
			if(NULL == pBaseExp)
			{
				return;
			}

			KGCategoryExpression* pDefCategoryExp = (KGCategoryExpression*)(pBaseExp);


			KGSimpleExpressionContainer* pContainer = 
				(KGSimpleExpressionContainer*)a_pInfo->ExpressionContainer();
			if(NULL == pContainer)
			{
				pContainer = (KGSimpleExpressionContainer*)(a_pInfo->CreateExpression());
			}

			KGCategoryExpression* pExpression = (KGCategoryExpression*)(pContainer->ExpressionObject());
			if(NULL == pExpression)
			{
				pExpression = (KGCategoryExpression*)(pContainer->CreateExpressionObject());
			}

			*pExpression = *pDefCategoryExp;


		}
		else if (a_pInfo->ZoneGroup() == true)
		{
			KGBaseExpressionContainer* pBaseCont = a_pDefaultInfo->ExpressionContainer();
			if(nullptr == pBaseCont)
			{
				return;
			}
			if(pBaseCont->ContainerType() != KEMGExpressionContainerZoneGroup)
			{
				a_pDefaultInfo->DeleteExpression();
				return;
			}

			KGGroupExpressionContainer* pDefContainer = (KGGroupExpressionContainer*)pBaseCont;
			if (pDefContainer == nullptr)
			{
				return;
			}

			KZoneGroupColumn* pDefGroupRoot           = pDefContainer->GetGroupRoot();
			if(nullptr == pDefGroupRoot)
			{
				return;
			}

			KGGroupExpressionContainer* pContainer = 
				(KGGroupExpressionContainer*)a_pInfo->ExpressionContainer();
			if(NULL == pContainer)
			{
				pContainer = (KGGroupExpressionContainer*)(a_pInfo->CreateExpression());
			}

			KZoneGroupColumn* pGroupRoot = pContainer->GetGroupRoot();
			if(NULL == pGroupRoot)
			{
				pGroupRoot = pContainer->CreateGroupRoot(pDefGroupRoot->ColumnName(), pDefGroupRoot->ColumnCaption());
			}

			*pGroupRoot = *pDefGroupRoot;
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


void KGenerationModelDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if (spProgressWindow) {
		spProgressWindow->SetStatus(a_strMsg);
	}
}