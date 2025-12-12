// ColoredDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ColoredDialog.h"


// ZColoredDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(ZColoredDialog, CDialog)

ZColoredDialog::ZColoredDialog()
{

}

ZColoredDialog::ZColoredDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
	:	CDialog(lpszTemplateName, pParentWnd)
{

}

ZColoredDialog::ZColoredDialog(UINT nIDTemplate, CWnd* pParentWnd)
	:	CDialog(nIDTemplate, pParentWnd)
{

}


ZColoredDialog::~ZColoredDialog()
{
}

void ZColoredDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ZColoredDialog, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// ZColoredDialog 메시지 처리기입니다.

HBRUSH ZColoredDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  여기서 DC의 특성을 변경합니다.

	//// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	//return hbr;
	if(m_brBackground.GetSafeHandle() == NULL)
	{
		m_brBackground.CreateSolidBrush(m_crBackground);
	}
	return m_brBackground;
}
