// TxTooltipView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TxTooltipView.h"
#include "afxdialogex.h"


// TxTooltipView 대화 상자입니다.

IMPLEMENT_DYNAMIC(TxTooltipView, CDialogEx)

TxTooltipView::TxTooltipView(CWnd* pParent /*=NULL*/)
	: CDialogEx(TxTooltipView::IDD, pParent)
{
    m_strText = _T("");
    m_nTextWidth  = 0;
    m_nTextHeight = 0;
    m_nMargin = 4;
}

TxTooltipView::~TxTooltipView()
{
}

void TxTooltipView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL TxTooltipView::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetBackgroundColor(RGB(255, 255, 202));
    m_font.CreatePointFont(90, _T("나눔고딕코딩"));	
    m_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    m_brush.CreateSolidBrush(RGB(255, 255, 202));

    if (true) {
        LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
        lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
        SetWindowLong(m_hWnd, GWL_STYLE, lStyle);
    }

    if (true) {
        LONG lExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
        lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
        SetWindowLong(m_hWnd, GWL_EXSTYLE, lExStyle);
    }

    return TRUE;
}

BEGIN_MESSAGE_MAP(TxTooltipView, CDialogEx)
    ON_BN_CLICKED(IDOK, &TxTooltipView::OnBnClickedOk)
    ON_WM_PAINT()
END_MESSAGE_MAP()


void TxTooltipView::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CDialogEx::OnOK();
}

void TxTooltipView::SetSimpleTooltip( CString strTooltip, CPoint point )
{
    m_strText = strTooltip;
    m_mousePt = point;

    CalculateWindowPos();    
    Invalidate();
}

void TxTooltipView::CalculateWindowPos()
{
    m_nTextWidth  = 0;
    m_nTextHeight = 0;

    CDC *pDC = GetDC();
    CFont *pOldfont;
    pOldfont = pDC->SelectObject(&m_font);

    CSize TextSize;
    int nLineCnt(0);
    int nTextLen = m_strText.GetLength();
    TCHAR *token = NULL;
    TCHAR *seps = _T("\n");
    TCHAR *buff = m_strText.GetBuffer(0);
    TCHAR *Textbuff = new TCHAR[nTextLen + 1];

    memset(Textbuff, 0, sizeof(TCHAR) * (nTextLen + 1));
    memcpy(Textbuff, buff, sizeof(TCHAR) * nTextLen);
    token = _tcstok(Textbuff, seps);
    while(token != NULL) {
        ++nLineCnt ;
        CString str(token);
        ::GetTextExtentPoint32(pDC->GetSafeHdc(), str, str.GetLength(), &TextSize);
        if(m_nTextWidth < TextSize.cx)
            m_nTextWidth = TextSize.cx;
        token = _tcstok(NULL, seps);
    }
    delete []Textbuff;

    if(nLineCnt < 1){
        m_nTextHeight = 1;
    }
    else{
        m_nTextHeight = (nLineCnt * 12);
    }

    pDC->SelectObject(pOldfont);
    ReleaseDC(pDC);

    CWnd*  pWnd  = m_pParentWnd;
    CPoint ptWnd = m_mousePt;
        
    pWnd->ClientToScreen(&ptWnd);
    SetWindowPos(pWnd, ptWnd.x, ptWnd.y + 20, m_nMargin*2 + m_nTextWidth, m_nMargin*2 + m_nTextHeight, SWP_SHOWWINDOW|SWP_NOOWNERZORDER|SWP_NOACTIVATE|SWP_NOZORDER);
}

void TxTooltipView::OnPaint()
{
    CPaintDC dc(this); 
    CFont*  pOldFont  = dc.SelectObject(&m_font);
    CPen*   pOldPen   = dc.SelectObject(&m_pen);    
    CBrush* pOldBrush = dc.SelectObject(&m_brush);
    
    RECT rect; {
        ::SetRect(&rect, 0, 0, m_nTextWidth+m_nMargin*2, m_nTextHeight+m_nMargin*2);
        dc.Rectangle(&rect);
    }
    
    dc.SetBkMode(TRANSPARENT);        
    dc.SetTextColor(RGB(0, 0, 0));    

    RECT txtRect; {
        ::SetRect(&txtRect, m_nMargin, m_nMargin, m_nTextWidth+m_nMargin, m_nTextHeight+m_nMargin);
        dc.DrawText(m_strText, &txtRect, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);
    }        

    dc.SelectObject(pOldFont);
    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}
