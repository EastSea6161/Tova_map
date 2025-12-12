// ModelProcessInterModal.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ModelProcessInterModal.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
#include "DBaseCheckData.h"
#include "MainFrameWnd.h"
#include "ImChampFrameWindow.h"
#include "KUIDefInterModalProcess.h"

// KModelProcessInterModal 대화 상자입니다.

using namespace UIDefInterModalProcess;

IMPLEMENT_DYNAMIC(KModelProcessInterModal, KResizeDialogEx)

KModelProcessInterModal::KModelProcessInterModal(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KModelProcessInterModal::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
{

}

KModelProcessInterModal::~KModelProcessInterModal()
{
}

void KModelProcessInterModal::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MARKUP_STEP1, m_wndStaticStep1);
	DDX_Control(pDX, IDC_STATIC_MARKUP_STEP2, m_wndStaticStep2);
	DDX_Control(pDX, IDC_STATIC_MARKUP_STEP3, m_wndStaticStep3);
	DDX_Control(pDX, IDC_STATIC_MARKUP_STEP4, m_wndStaticStep4);
	DDX_Control(pDX, IDC_STATIC_MARKUP_STEP5, m_wndStaticStep5);
}


BEGIN_MESSAGE_MAP(KModelProcessInterModal, KResizeDialogEx)
END_MESSAGE_MAP()


// KModelProcessInterModal 메시지 처리기입니다.


BOOL KModelProcessInterModal::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	RunNotifyData();

	m_wndStaticStep1.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &KModelProcessInterModal::OnHyperlinkClickStep1));
	m_wndStaticStep2.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &KModelProcessInterModal::OnHyperlinkClickStep2));
	m_wndStaticStep3.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &KModelProcessInterModal::OnHyperlinkClickStep3));
	m_wndStaticStep4.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &KModelProcessInterModal::OnHyperlinkClickStep4));
	m_wndStaticStep5.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &KModelProcessInterModal::OnHyperlinkClickStep5));

	return TRUE; 
}


void KModelProcessInterModal::RunNotifyData()
{
	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_wndStaticStep1.SetMarkupText(KR_UIXAML_TRUE_STEP1);
	}
	else {
		m_wndStaticStep1.SetMarkupText(UIXAML_TRUE_STEP1);
	}

	if(false == KDBaseCheckData::GetCheckTargetRegionCode(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep2.SetMarkupText(KR_UIXAML_FALSE_FALSE_STEP2);
		}
		else {
			m_wndStaticStep2.SetMarkupText(UIXAML_FALSE_FALSE_STEP2);
		}
	}
	else if (false == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget) )
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep2.SetMarkupText(KR_UIXAML_TRUE_FALSE_STEP2);
		}
		else {
			m_wndStaticStep2.SetMarkupText(UIXAML_TRUE_FALSE_STEP2);
		}
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep2.SetMarkupText(KR_UIXAML_TRUE_TRUE_STEP2);
		}
		else {
			m_wndStaticStep2.SetMarkupText(UIXAML_TRUE_TRUE_STEP2);
		}
	}

	if (false == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget) || false == KDBaseCheckData::GetCheckTargetRegionCode(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep3.SetMarkupText(KR_UIXAML_FALSE_FALSE_STEP3);
		} 
		else {
			m_wndStaticStep3.SetMarkupText(UIXAML_FALSE_FALSE_STEP3);
		}
	}
	else if (true == KDBaseCheckData::GetCheckTerminaNodeData(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep3.SetMarkupText(KR_UIXAML_TRUE_TRUE_STEP3);
		}
		else {
			m_wndStaticStep3.SetMarkupText(UIXAML_TRUE_TRUE_STEP3);
		}
	}
	else if (true == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep3.SetMarkupText(KR_UIXAML_TRUE_FALSE_STEP3);
		}
		else {
			m_wndStaticStep3.SetMarkupText(UIXAML_TRUE_FALSE_STEP3);
		}
	}
	

	if (false == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget) || false == KDBaseCheckData::GetCheckTargetRegionCode(m_pTarget) || false == KDBaseCheckData::GetCheckTerminaNodeData(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep4.SetMarkupText(KR_UIXAML_FALSE_FALSE_STEP4);
		}
		else {
			m_wndStaticStep4.SetMarkupText(UIXAML_FALSE_FALSE_STEP4);
		}
	}
	else if (true == KDBaseCheckData::GetCheckInterModalPahthGenerator(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep4.SetMarkupText(KR_UIXAML_TRUE_TRUE_STEP4);
		}
		else {
			m_wndStaticStep4.SetMarkupText(UIXAML_TRUE_TRUE_STEP4);
		}
	}
	else if (true == KDBaseCheckData::GetCheckTerminaNodeData(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep4.SetMarkupText(KR_UIXAML_TRUE_FALSE_STEP4);
		}
		else {
			m_wndStaticStep4.SetMarkupText(UIXAML_TRUE_FALSE_STEP4);
		}
	}
	

	if (false == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget) || false == KDBaseCheckData::GetCheckTargetRegionCode(m_pTarget) || false == KDBaseCheckData::GetCheckTerminaNodeData(m_pTarget) || false == KDBaseCheckData::GetCheckInterModalPahthGenerator(m_pTarget))
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep5.SetMarkupText(KR_UIXAML_FALSE_STEP5);
		}
		else {
			m_wndStaticStep5.SetMarkupText(UIXAML_FALSE_STEP5);
		}
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_wndStaticStep5.SetMarkupText(KR_UIXAML_TRUE_STEP5);
		}
		else {
			m_wndStaticStep5.SetMarkupText(UIXAML_TRUE_STEP5);
		}
	}
}


void KModelProcessInterModal::OnHyperlinkClickStep1( CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs )
{

	if (pSender->IsKindOf(MARKUP_TYPE(CXTPMarkupHyperlink)))
	{
		CXTPMarkupString* pTag = MARKUP_DYNAMICCAST(CXTPMarkupString, ((CXTPMarkupHyperlink*)pSender)->GetTag());
		TRACE(_T("Tag = %ls"), (LPCWSTR)*pTag);	
		CString strTag  = *pTag;

		if (strTag == "hyp1")
		{
			m_pMapView->OnInterModalTargetRegionSetting();
		}
		pArgs->SetHandled();
	}

	RunNotifyData();
}

void KModelProcessInterModal::OnHyperlinkClickStep2( CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs )
{

	if (pSender->IsKindOf(MARKUP_TYPE(CXTPMarkupHyperlink)))
	{
		CXTPMarkupString* pTag = MARKUP_DYNAMICCAST(CXTPMarkupString, ((CXTPMarkupHyperlink*)pSender)->GetTag());
		TRACE(_T("Tag = %ls"), (LPCWSTR)*pTag);	
		CString strTag  = *pTag;

		if (strTag == "hyp2")
		{
			CMainFrameWnd* pMainFrameWnd = ImChampFrameWindow::GetMainFrameWnd();
			pMainFrameWnd->OnMultiModalSetAccessibility();
		}
		else if (strTag == "hyp2-1")
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

void KModelProcessInterModal::OnHyperlinkClickStep3( CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs )
{

	if (pSender->IsKindOf(MARKUP_TYPE(CXTPMarkupHyperlink)))
	{
		CXTPMarkupString* pTag = MARKUP_DYNAMICCAST(CXTPMarkupString, ((CXTPMarkupHyperlink*)pSender)->GetTag());
		TRACE(_T("Tag = %ls"), (LPCWSTR)*pTag);	
		CString strTag  = *pTag;

		if (strTag == "hyp3")
		{
			CMainFrameWnd* pMainFrameWnd = ImChampFrameWindow::GetMainFrameWnd();
			pMainFrameWnd->OnInterAccessTerminalGenerator();
		}
		else if (strTag == "hyp3-1")
		{
			m_pMapView->OnInterModalAccessTerminalEditor();
		}
		pArgs->SetHandled();
	}

	RunNotifyData();
}

void KModelProcessInterModal::OnHyperlinkClickStep4( CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs )
{
	if (pSender->IsKindOf(MARKUP_TYPE(CXTPMarkupHyperlink)))
	{
		CXTPMarkupString* pTag = MARKUP_DYNAMICCAST(CXTPMarkupString, ((CXTPMarkupHyperlink*)pSender)->GetTag());
		TRACE(_T("Tag = %ls"), (LPCWSTR)*pTag);	
		CString strTag  = *pTag;

		if (strTag == "hyp4")
		{
			CMainFrameWnd* pMainFrameWnd = ImChampFrameWindow::GetMainFrameWnd();
			pMainFrameWnd->OnInterPathGenerator();
		}
		else if (strTag == "hyp4-1")
		{
			m_pMapView->OnInterModalPathEdit();
		}
		pArgs->SetHandled();
	}

	RunNotifyData();
}

void KModelProcessInterModal::OnHyperlinkClickStep5( CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs )
{

	if (pSender->IsKindOf(MARKUP_TYPE(CXTPMarkupHyperlink)))
	{
		CXTPMarkupString* pTag = MARKUP_DYNAMICCAST(CXTPMarkupString, ((CXTPMarkupHyperlink*)pSender)->GetTag());
		TRACE(_T("Tag = %ls"), (LPCWSTR)*pTag);	
		CString strTag  = *pTag;

		if (strTag == "hyp5")
		{
			CMainFrameWnd* pMainFrameWnd = ImChampFrameWindow::GetMainFrameWnd();
			pMainFrameWnd->OnInterModalMain();
		}
		pArgs->SetHandled();
	}

	RunNotifyData();
}

void KModelProcessInterModal::Initialize()
{
	RunNotifyData();
}
