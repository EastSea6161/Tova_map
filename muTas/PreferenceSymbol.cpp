// PreferenceSymbol.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PreferenceSymbol.h"
#include "afxdialogex.h"
#include "KPointMarkerSelectDlg.h"
//#include "StringChecker.h"

// KPreferenceSymbol 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPreferenceSymbol, KDialogEx)

KPreferenceSymbol::KPreferenceSymbol(CWnd* pParent /*=NULL*/)
	: KDialogEx(KPreferenceSymbol::IDD, pParent)
{
	m_oSymbolNode = ImChampMapDefineSymbol::NodeSymbol;
	m_oSymbolZone = ImChampMapDefineSymbol::ZoneSymbol;

	m_clrNode = m_oSymbolNode.clrSymbol;
	m_clrZone = m_oSymbolZone.clrSymbol;

	m_oQbicDefaultMarkerFont.CreatePointFont(120, _T("QBicF"));

	m_nMarkerNodeIndex = m_oSymbolNode.nCharIndex;
	m_nMarkerZoneIndex = m_oSymbolZone.nCharIndex; 
}

KPreferenceSymbol::~KPreferenceSymbol()
{
}

void KPreferenceSymbol::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_MARKER_COLOR_NODE, m_cpNodeColor);
	DDX_Control(pDX, IDC_BUTTON_MARKER_COLOR_ZONE, m_cpZoneColor);
	DDX_Control(pDX, IDC_BUTTON_MARKER_NODE,       m_btnMarkerNode);
	DDX_Control(pDX, IDC_BUTTON_MARKER_ZONE,       m_btnMarkerZone);
}


BEGIN_MESSAGE_MAP(KPreferenceSymbol, KDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BUTTON_MARKER_NODE, &KPreferenceSymbol::OnBnClickedButtonMarkerNode)
	ON_BN_CLICKED(IDC_BUTTON_MARKER_ZONE, &KPreferenceSymbol::OnBnClickedButtonMarkerZone)
	ON_CPN_XTP_SELENDOK(IDC_BUTTON_MARKER_COLOR_NODE, &KPreferenceSymbol::OnSelEndOKColor)
	ON_CPN_XTP_SELENDOK(IDC_BUTTON_MARKER_COLOR_ZONE, &KPreferenceSymbol::OnSelEndOKColor)
END_MESSAGE_MAP()


// KPreferenceSymbol 메시지 처리기입니다.


BOOL KPreferenceSymbol::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_cpNodeColor.SetColor( m_clrNode );
	m_cpNodeColor.SetWindowText( _T("") );
	m_cpNodeColor.ShowText( TRUE );
	m_cpNodeColor.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

	m_cpZoneColor.SetColor( m_clrZone );
	m_cpZoneColor.SetWindowText( _T("") );
	m_cpZoneColor.ShowText( TRUE );
	m_cpZoneColor.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

	CString strTemp(_T(""));
	strTemp.Format(_T("%.f"), m_oSymbolNode.dSize);
	SetDlgItemText(IDC_EDIT_SIZE_NODE, strTemp);

	strTemp.Format(_T("%.f"), m_oSymbolZone.dSize);
	SetDlgItemText(IDC_EDIT_SIZE_ZONE, strTemp);

	strTemp = _T("");
	strTemp.AppendChar((TCHAR)m_nMarkerNodeIndex);    
	SetDlgItemText(IDC_BUTTON_MARKER_NODE, strTemp);

	strTemp = _T("");
	strTemp.AppendChar((TCHAR)m_nMarkerZoneIndex);    
	SetDlgItemText(IDC_BUTTON_MARKER_ZONE, strTemp);

	m_btnMarkerNode.SetFont(&m_oQbicDefaultMarkerFont);
	m_btnMarkerZone.SetFont(&m_oQbicDefaultMarkerFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HBRUSH KPreferenceSymbol::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	return hbr;
}


void KPreferenceSymbol::OnSelEndOKColor()
{
	m_clrNode = m_cpNodeColor.GetColor();
	m_clrZone = m_cpZoneColor.GetColor();
}


void KPreferenceSymbol::UserCommand(int a_nCommand)
{
	try
	{
		if (!InvalidateInputInfo())
		{
			return;
		}

		CWaitCursor cw;

		TPreferenceSymbol &oTNodeSymbol = ImChampMapDefineSymbol::NodeSymbol;
		TPreferenceSymbol &oTZoneSymbol = ImChampMapDefineSymbol::ZoneSymbol;

		CString strTemp(_T(""));

		GetDlgItemText(IDC_EDIT_SIZE_NODE, strTemp);
		oTNodeSymbol.dSize      = _ttof(strTemp);
		oTNodeSymbol.nCharIndex = m_nMarkerNodeIndex;
		oTNodeSymbol.clrSymbol  = m_clrNode;

		GetDlgItemText(IDC_EDIT_SIZE_ZONE, strTemp);
		oTZoneSymbol.dSize      = _ttof(strTemp);
		oTZoneSymbol.nCharIndex = m_nMarkerZoneIndex;
		oTZoneSymbol.clrSymbol  = m_clrZone;
		
		ImChampMapDefineSymbol::SavePreferenceSymbol();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KPreferenceSymbol::InvalidateInputInfo()
{
	try
	{
		CString strTemp(_T(""));
		double  dSize(0.0);

		//node
		GetDlgItemText(IDC_EDIT_SIZE_NODE, strTemp);
		if (QBicStringChecker::IsNumeric(strTemp) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_SIZE_NODE)->SetFocus();
			return false;
		}

		dSize = _ttof(strTemp);
		if (dSize <= 0.0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_SIZE_NODE)->SetFocus();
			return false;
		}

		//zone
		GetDlgItemText(IDC_EDIT_SIZE_ZONE, strTemp);
		if (QBicStringChecker::IsNumeric(strTemp) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_SIZE_ZONE)->SetFocus();
			return false;
		}

		dSize = _ttof(strTemp);
		if (dSize <= 0.0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_SIZE_ZONE)->SetFocus();
			return false;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return true;
}


void KPreferenceSymbol::OnBnClickedButtonMarkerNode()
{
	//TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KPointMarkerSelectDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		int nSelectMarkerIndex = dlg.GetSelectedMarkerIndex();
		if (nSelectMarkerIndex > 0)
		{
			CString strSymbol;
			strSymbol.AppendChar(nSelectMarkerIndex);     
			m_nMarkerNodeIndex = nSelectMarkerIndex;
			SetDlgItemText(IDC_BUTTON_MARKER_NODE, strSymbol);
		}
	}
}


void KPreferenceSymbol::OnBnClickedButtonMarkerZone()
{
	//TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KPointMarkerSelectDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		int nSelectMarkerIndex = dlg.GetSelectedMarkerIndex();
		if (nSelectMarkerIndex > 0)
		{
			CString strSymbol;
			strSymbol.AppendChar(nSelectMarkerIndex);     
			m_nMarkerZoneIndex = nSelectMarkerIndex;
			SetDlgItemText(IDC_BUTTON_MARKER_ZONE, strSymbol);
		}
	}
}
