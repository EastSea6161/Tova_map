#pragma once

#include "MarkupStatic.h"

class KTarget;
class KMapView;

// KModelProcessMultiModal 대화 상자입니다.

class KModelProcessMultiModal : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KModelProcessMultiModal)

public:
	KModelProcessMultiModal(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KModelProcessMultiModal();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8201_ProcessMultimodal };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void OnHyperlinkClickStep1(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);
	void OnHyperlinkClickStep2(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);
	void OnHyperlinkClickStep3(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


protected:
	void RunNotifyData();

public:
	void Initialize();

protected:
	KTarget*		m_pTarget;
	KMapView*		m_pMapView;

	CMarkupStatic   m_uiBackImage;
	CMarkupStatic	m_wndStaticStep1;
	CMarkupStatic	m_wndStaticStep2;
	CMarkupStatic	m_wndStaticStep3;
};

typedef std::shared_ptr<KModelProcessMultiModal> KModelProcessMultiModalPtr;
