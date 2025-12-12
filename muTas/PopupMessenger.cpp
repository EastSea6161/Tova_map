#include "StdAfx.h"
#include "PopupMessenger.h"

#define ID_POPUP_CLOSE (-1)


KPopupMessenger::KPopupMessenger()
{

}

KPopupMessenger::~KPopupMessenger()
{

}

void KPopupMessenger::Show(CString strMsg, CWnd* pMainWindow)
{
    CXTPPopupControl* pPopup = new CXTPPopupControl(TRUE);
    pPopup->SetTransparency(255);
    pPopup->AllowMove(TRUE);
    pPopup->SetAnimateDelay(250);
    pPopup->SetPopupAnimation(xtpPopupAnimationSlide);
    pPopup->SetShowDelay(1500);
    pPopup->RemoveAllItems();

    int nPopupWidth  = 90;
    int nPopupHeight = 60;
    pPopup->SetPopupSize(CSize(nPopupWidth, nPopupHeight));
    
    int nMargin      = 4;
    int nWidth       = nPopupWidth - nMargin * 2;
    int nY           = 10;
	
	CXTPPopupItem* pItemText = NULL;
    pItemText = (CXTPPopupItem*)pPopup->AddItem(new CXTPPopupItem(CRect(nMargin, 3 * nMargin, nPopupWidth-nMargin, nPopupHeight-nMargin), strMsg));
    
	//pItemText->SetBold(TRUE);
    pItemText->SetTextColor(RGB(0, 61, 178));
    pItemText->SetTextAlignment(DT_LEFT|DT_WORDBREAK);

	pPopup->SetTheme(xtpPopupThemeOffice2003);
	pPopup->RedrawControl();
    CRect rectWindow;
    pMainWindow->GetWindowRect(&rectWindow);
    CPoint ptPosition = rectWindow.BottomRight();
    pPopup->SetPopupPos(ptPosition);
    pPopup->Show(pMainWindow);
}

