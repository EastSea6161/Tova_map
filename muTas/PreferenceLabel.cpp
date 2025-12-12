// PreferenceLabel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PreferenceLabel.h"
#include "afxdialogex.h"


// KPreferenceLabel 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPreferenceLabel, KDialogEx)

KPreferenceLabel::KPreferenceLabel(CWnd* pParent /*=NULL*/)
	: KDialogEx(KPreferenceLabel::IDD, pParent)
{
    m_strNodeFontName = ImChampMapDefineLabel::NodeLabelFontName;
    m_strLinkFontName = ImChampMapDefineLabel::LinkLabelFontName;

    m_dNodeFontSize   = ImChampMapDefineLabel::NodeLabelSize;
    m_dLinkFontSize   = ImChampMapDefineLabel::LinkLabelSize;

    m_rgbNodeColor    = ImChampMapDefineLabel::NodeLabelColor;
    m_rgbLinkColor    = ImChampMapDefineLabel::LinkLabelColor;
}

KPreferenceLabel::~KPreferenceLabel()
{
}

void KPreferenceLabel::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_FONT_NODE, m_wndComboFontNode);
    DDX_Control(pDX, IDC_COMBO_FONT_LINK, m_wndComboFontLink);

    DDX_Control(pDX, IDC_BUTTON_MARKER_COLOR_NODE, m_btnNodeColorPicker);
    DDX_Control(pDX, IDC_BUTTON_MARKER_COLOR_LINK, m_btnLinkColorPicker);
}


BEGIN_MESSAGE_MAP(KPreferenceLabel, KDialogEx)
    ON_WM_CTLCOLOR()
    ON_CBN_SELENDOK(IDC_COMBO_FONT_NODE, OnSelEndOKFontNode)
    ON_CBN_SELENDOK(IDC_COMBO_FONT_LINK, OnSelEndOKFontLink)

    ON_CPN_XTP_SELENDOK(IDC_BUTTON_MARKER_COLOR_NODE, OnSelEndOKColor)
    ON_CPN_XTP_SELENDOK(IDC_BUTTON_MARKER_COLOR_LINK, OnSelEndOKColor)
END_MESSAGE_MAP()


BOOL KPreferenceLabel::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    SetBackgroundColor(RGB(255,255,255));

    m_wndComboFontNode.InitControl(m_strNodeFontName);
    m_wndComboFontLink.InitControl(m_strLinkFontName);

    m_btnNodeColorPicker.SetColor( m_rgbNodeColor );
    m_btnNodeColorPicker.SetWindowText( _T("") );
    m_btnNodeColorPicker.ShowText( TRUE );
    m_btnNodeColorPicker.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

    m_btnLinkColorPicker.SetColor( m_rgbLinkColor );
    m_btnLinkColorPicker.SetWindowText( _T("") );
    m_btnLinkColorPicker.ShowText( TRUE );
    m_btnLinkColorPicker.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );
    
    CString strTemp(_T(""));
    strTemp.Format(_T("%.f"), m_dNodeFontSize);
    SetDlgItemText(IDC_EDIT_SIZE_NODE, strTemp);

    strTemp.Format(_T("%.f"), m_dLinkFontSize);
    SetDlgItemText(IDC_EDIT_SIZE_LINK, strTemp);

    return TRUE;
}

HBRUSH KPreferenceLabel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  여기서 DC의 특성을 변경합니다.
    pDC->SetBkMode(TRANSPARENT);
    hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

    return hbr;
}

void KPreferenceLabel::OnSelEndOKFontNode()
{
    UpdateData();
    m_wndComboFontNode.GetSelFont(m_lf);
    m_strNodeFontName = m_lf.lfFaceName;
}

void KPreferenceLabel::OnSelEndOKFontLink()
{
    UpdateData();
    m_wndComboFontLink.GetSelFont(m_lf);
    m_strLinkFontName = m_lf.lfFaceName;
}

void KPreferenceLabel::OnSelEndOKColor()
{
    m_rgbNodeColor = m_btnNodeColorPicker.GetColor();
    m_rgbLinkColor = m_btnLinkColorPicker.GetColor();
}

void KPreferenceLabel::UserCommand(int a_nCommand)
{
    ImChampMapDefineLabel::NodeLabelFontName = m_strNodeFontName;
    ImChampMapDefineLabel::LinkLabelFontName = m_strLinkFontName;
    
    CString strTemp(_T(""));
    GetDlgItemText(IDC_EDIT_SIZE_NODE, strTemp);
    ImChampMapDefineLabel::NodeLabelSize = _ttof(strTemp);

    GetDlgItemText(IDC_EDIT_SIZE_LINK, strTemp);
    ImChampMapDefineLabel::LinkLabelSize = _ttof(strTemp);

    ImChampMapDefineLabel::NodeLabelColor = m_rgbNodeColor;
    ImChampMapDefineLabel::LinkLabelColor = m_rgbLinkColor;

    ImChampMapDefineLabel::SavePreferenceLabel();
}


