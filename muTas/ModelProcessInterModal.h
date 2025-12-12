#pragma once

#include "MarkupStatic.h"
class KTarget;
class KMapView;



// KModelProcessInterModal 대화 상자입니다.

class KModelProcessInterModal : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KModelProcessInterModal)

public:
	KModelProcessInterModal(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KModelProcessInterModal();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8202_ProcessInterModal };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*	m_pTarget;
	KMapView*	m_pMapView;

	CMarkupStatic   m_uiBackImage;
	CMarkupStatic	m_wndStaticStep1;
	CMarkupStatic	m_wndStaticStep2;
	CMarkupStatic	m_wndStaticStep3;
	CMarkupStatic	m_wndStaticStep4;
	CMarkupStatic	m_wndStaticStep5;

protected:
	
	void RunNotifyData();

	void OnHyperlinkClickStep1(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);
	void OnHyperlinkClickStep2(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);
	void OnHyperlinkClickStep3(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);
	void OnHyperlinkClickStep4(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);
	void OnHyperlinkClickStep5(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);

public:
	void Initialize();
public:
	virtual BOOL OnInitDialog();
};
typedef std::shared_ptr<KModelProcessInterModal> KModelProcessInterModalPtr;