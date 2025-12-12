// DropdownEnterCombo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DropdownEnterCombo.h"


// KDropdownEnterCombo

IMPLEMENT_DYNAMIC(KDropdownEnterCombo, CComboBox)

KDropdownEnterCombo::KDropdownEnterCombo()
{

}

KDropdownEnterCombo::~KDropdownEnterCombo()
{
}


BEGIN_MESSAGE_MAP(KDropdownEnterCombo, CComboBox)
END_MESSAGE_MAP()

BOOL KDropdownEnterCombo::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message != WM_KEYDOWN 
		&& LOWORD(pMsg->wParam) != VK_RETURN)
		return CComboBox::PreTranslateMessage(pMsg);

	//GetParent()->SendMessage(WM_KEYDOWN, pMsg->wParam, (LPARAM)m_hWnd);
	pMsg->hwnd = m_hWnd;
	GetParent()->PreTranslateMessage(pMsg);

	return TRUE;
}



// KDropdownEnterCombo 메시지 처리기입니다.


