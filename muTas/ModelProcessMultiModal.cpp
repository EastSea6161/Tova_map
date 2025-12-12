// ModelProcessMultiModal.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ModelProcessMultiModal.h"
#include "afxdialogex.h"
#include "KUIDefineMultiModelsProess.h"
#include "SetAccessibility.h"
#include "GenerationBusPathStep1Dlg.h"
#include "stdafx.h"
#include "KmzApp.h"
#include "KmzSystem.h"
#include "MainFrameWnd.h"
#include "ImChampFrameWindow.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "IOTableView.h"
#include "IOView.h"
#include "DBaseCheckData.h"


using namespace UIMultimodalModelsProcess;

// KModelProcessMultiModal 대화 상자입니다.

IMPLEMENT_DYNAMIC(KModelProcessMultiModal, CXTPResizeDialog)

KModelProcessMultiModal::KModelProcessMultiModal(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KModelProcessMultiModal::IDD, pParent)
	,m_pTarget(a_pTarget)
	,m_pMapView(a_pMapView)
{

}

KModelProcessMultiModal::~KModelProcessMultiModal()
{
}

void KModelProcessMultiModal::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MARKUP_STEP1, m_wndStaticStep1);
	DDX_Control(pDX, IDC_STATIC_MARKUP_STEP2, m_wndStaticStep2);
	DDX_Control(pDX, IDC_STATIC_MARKUP_STEP3, m_wndStaticStep3);
}


BEGIN_MESSAGE_MAP(KModelProcessMultiModal, CXTPResizeDialog)
END_MESSAGE_MAP()


// KModelProcessMultiModal 메시지 처리기입니다.


BOOL KModelProcessMultiModal::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	RunNotifyData();

	m_wndStaticStep1.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &KModelProcessMultiModal::OnHyperlinkClickStep1));
	m_wndStaticStep2.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &KModelProcessMultiModal::OnHyperlinkClickStep2));
	m_wndStaticStep3.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &KModelProcessMultiModal::OnHyperlinkClickStep3));
	
	return TRUE;
}

void KModelProcessMultiModal::OnHyperlinkClickStep1(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs)
{
	if (pSender->IsKindOf(MARKUP_TYPE(CXTPMarkupHyperlink)))
	{
		CXTPMarkupString* pTag = MARKUP_DYNAMICCAST(CXTPMarkupString, ((CXTPMarkupHyperlink*)pSender)->GetTag());
		TRACE(_T("Tag = %ls"), (LPCWSTR)*pTag);	
		CString strTag  = *pTag;

		if (strTag == "hyp1")
		{
			CMainFrameWnd* pMainFrameWnd = ImChampFrameWindow::GetMainFrameWnd();
			pMainFrameWnd->OnMultiModalSetAccessibility();
		}
		else if (strTag == "hyp2")
		{
			if (true == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget))
			{
				m_pMapView->OnAccessibilityModify();
			}
			else
			{
				AfxMessageBox(_T("경로 만들기 > 대중교통 > 이용가능 역/정류장 생성 기능을 먼저 실행해 주세요."));
			}
		}
		pArgs->SetHandled();
	}

	RunNotifyData();
}

void KModelProcessMultiModal::OnHyperlinkClickStep2(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs)
{
	if (pSender->IsKindOf(MARKUP_TYPE(CXTPMarkupHyperlink)))
	{
		CXTPMarkupString* pTag = MARKUP_DYNAMICCAST(CXTPMarkupString, ((CXTPMarkupHyperlink*)pSender)->GetTag());
		TRACE(_T("Tag = %ls"), (LPCWSTR)*pTag);	
		CString strTag  = *pTag;

		if (strTag == "hyp3")
		{
			if (true == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget))
			{
				CMainFrameWnd* pMainFrameWnd = ImChampFrameWindow::GetMainFrameWnd();
				pMainFrameWnd->OnMultiModalGenerationBusPath();
			}
			else
			{
				AfxMessageBox(_T("경로 만들기 > 대중교통 > 이용가능 역/정류장 생성 기능을 먼저 실행해 주세요."));
			}
		}
		else if (strTag == "hyp4")
		{
			if (true == KDBaseCheckData::GetCheckBusPathInputData(m_pTarget))
			{
				KMapView* pMapView = ImChampFrameWindow::GetMapView(m_pTarget);
				m_pMapView->OnMultiModalTransitPathEdit();
			}
			else
			{
				AfxMessageBox(_T("경로 만들기 > 대중교통 > 대중교통 경로 생성을 먼저 실행해 주세요."));
			}
		}

		pArgs->SetHandled();

	}

	RunNotifyData();
}

void KModelProcessMultiModal::OnHyperlinkClickStep3(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs)
{
	if ((true == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget)) && (true == KDBaseCheckData::GetCheckBusPathInputData(m_pTarget)))
	{
		CMainFrameWnd* pMainFranwWnd = ImChampFrameWindow::GetMainFrameWnd();
		pMainFranwWnd->OnMultiModalModel();
	}
	else
	{
		AfxMessageBox(_T("이용가능 역/정류장 생성 기능과 대중교통 경로 생성 기능을 먼저 실행해 주세요."));
	}

	RunNotifyData();
}

void KModelProcessMultiModal::RunNotifyData()
{
	if(false == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep1.SetMarkupText(KR_UIXAML_TRUE_FALSE_STEP1);
		}
		else {
 			m_wndStaticStep1.SetMarkupText(UIXAML_TRUE_FALSE_STEP1);
 		}
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep1.SetMarkupText(KR_UIXAML_TRUE_TRUE_STEP1);
		}
		else {
 			m_wndStaticStep1.SetMarkupText(UIXAML_TRUE_TRUE_STEP1);
 		}
	}

	if((false == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget)) && (false == KDBaseCheckData::GetCheckBusPathInputData(m_pTarget)))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep2.SetMarkupText(KR_UIXAML_FALSE_FALSE_STEP2);
			m_wndStaticStep3.SetMarkupText(KR_UIXAML_FALSE_STEP3);
		}
		else {
			m_wndStaticStep2.SetMarkupText(UIXAML_FALSE_FALSE_STEP2);
			m_wndStaticStep3.SetMarkupText(UIXAML_FALSE_STEP3);
 		} 
	}
	else if((true == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget)) && (false == KDBaseCheckData::GetCheckBusPathInputData(m_pTarget)))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep2.SetMarkupText(KR_UIXAML_TRUE_FALSE_STEP2);
			m_wndStaticStep3.SetMarkupText(KR_UIXAML_FALSE_STEP3);
		}
		else {
			m_wndStaticStep2.SetMarkupText(UIXAML_TRUE_FALSE_STEP2);
			m_wndStaticStep3.SetMarkupText(UIXAML_FALSE_STEP3);
 		} 
	}
	else if((true == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget)) && (true == KDBaseCheckData::GetCheckBusPathInputData(m_pTarget)))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep2.SetMarkupText(KR_UIXAML_TRUE_TRUE_STEP2);
			m_wndStaticStep3.SetMarkupText(KR_UIXAML_TRUE_STEP3);
		}
		else {
			m_wndStaticStep2.SetMarkupText(UIXAML_TRUE_TRUE_STEP2);
			m_wndStaticStep3.SetMarkupText(UIXAML_TRUE_STEP3);
 		} 
	}
}

void KModelProcessMultiModal::Initialize()
{
	RunNotifyData();
}
