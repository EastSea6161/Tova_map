// KMapBackgroundColorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KMapBackgroundColorDlg.h"
#include "afxdialogex.h"


// KMapBackgroundColorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMapBackgroundColorDlg, CDialogEx)

KMapBackgroundColorDlg::KMapBackgroundColorDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(IDD_2201_MAP_BACKGROUND, pParent)
	, m_nColorType(0)
{

}

KMapBackgroundColorDlg::~KMapBackgroundColorDlg()
{
}

void KMapBackgroundColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nColorType);
	DDX_Control(pDX, IDC_SLIDER1, m_sldTransparent);
	DDX_Control(pDX, IDC_EDIT1, m_edtTransparent);
	DDX_Control(pDX, IDC_BUTTON_COLORPICK, m_btnColorPick);
}


BEGIN_MESSAGE_MAP(KMapBackgroundColorDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &KMapBackgroundColorDlg::OnBnClickedOk)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &KMapBackgroundColorDlg::OnNMReleasedcaptureSlider1)
	ON_EN_CHANGE(IDC_EDIT1, &KMapBackgroundColorDlg::OnEnChangeEdit1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KMapBackgroundColorDlg::OnBnClickedRadio)
END_MESSAGE_MAP()


// KMapBackgroundColorDlg 메시지 처리기입니다.


BOOL KMapBackgroundColorDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	UpdateData(false);
	COLORREF cr = m_crBackground.ToCOLORREF();
	int nA = m_crBackground.GetAlpha();
	int pos = 100 - ((double)nA / 255.0 * 100.0);

	// Color Picker button 설정
	m_btnColorPick.SetColor(cr);
	m_btnColorPick.SetWindowText(_T(""));
	m_btnColorPick.ShowText(TRUE);
	m_btnColorPick.ModifyCPStyle(0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE);


	// 슬라이더 설정
	m_sldTransparent.SetRange(0, 100);
	m_sldTransparent.SetRangeMin(0);
	m_sldTransparent.SetRangeMax(100);
	m_sldTransparent.SetPos(pos);
	m_sldTransparent.SetTicFreq(50);
	m_sldTransparent.SetLineSize(1);
	m_sldTransparent.SetPageSize(10);

	// Edit 설정
	CString strTransparent;
	strTransparent.Format(_T("%d"), pos);
	m_edtTransparent.EnableMinMax(0, 100);
	m_edtTransparent.EnableRealNumber(false);
	m_edtTransparent.SetWindowText(strTransparent);

	if (m_nColorType == 0)	// 배경없음
	{
		m_btnColorPick.EnableWindow(FALSE);
		m_sldTransparent.EnableWindow(FALSE);
		m_edtTransparent.EnableWindow(FALSE);
	}
	else if (m_nColorType == 1)	// 단일 색상
	{
		m_btnColorPick.EnableWindow(TRUE);
		m_sldTransparent.EnableWindow(TRUE);
		m_edtTransparent.EnableWindow(TRUE);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMapBackgroundColorDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int pos = m_sldTransparent.GetPos();
	CString strTransparent;
	strTransparent.Format(_T("%d"), pos);
	m_edtTransparent.SetWindowText(strTransparent);

	*pResult = 0;
}


void KMapBackgroundColorDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_nColorType == 0)
	{
		m_crBackground = Gdiplus::Color::White;
	}
	else if(m_nColorType == 1)
	{
		COLORREF cr = m_btnColorPick.GetColor();
		int pos = m_sldTransparent.GetPos();
		pos = 255 - (int)(255 * ((double)pos / 100.0));
		m_crBackground.SetValue(
			Gdiplus::Color::MakeARGB(
				pos, cr & 0x000000ff, (cr & 0x0000ff00) >> 8, (cr & 0x00ff0000) >> 16
			)
		);
	}

	CDialogEx::OnOK();
}


void KMapBackgroundColorDlg::OnEnChangeEdit1()
{
	CString strValue;
	m_edtTransparent.GetWindowText(strValue);
	int nPos = _ttoi(strValue);
	if (nPos < 0 || nPos > 100)
	{
		AfxMessageBox(_T("투명도의 입력 범위는 0 ~ 100 까지 입니다.\n확인 후 다시 입력 해 주세요."));
		m_edtTransparent.SetFocus();
		return;
	}

	m_sldTransparent.SetPos(nPos);
}



void KMapBackgroundColorDlg::OnBnClickedRadio(UINT id)
{
	UpdateData(TRUE);

	if (m_nColorType == 0)	// 배경없음
	{
		m_btnColorPick.EnableWindow(FALSE);
		m_sldTransparent.EnableWindow(FALSE);
		m_edtTransparent.EnableWindow(FALSE);
	}
	else if (m_nColorType == 1)	// 단일 색상
	{
		m_btnColorPick.EnableWindow(TRUE);
		m_sldTransparent.EnableWindow(TRUE);
		m_edtTransparent.EnableWindow(TRUE);
	}

}


int KMapBackgroundColorDlg::GetMapBackgroundColorMode()
{
	return m_nColorType;
}


void KMapBackgroundColorDlg::SetMapBackgroundColorMode(int mode)
{
	if (mode > 1 || mode < 0)
	{
		m_nColorType = 0;
	}
	else
	{
		m_nColorType = mode;
	}
}




Gdiplus::Color KMapBackgroundColorDlg::GetMapBackgroundColor()
{
	return m_crBackground;
}


void KMapBackgroundColorDlg::SetMapBackgroundColor(Gdiplus::Color cr)
{
	m_crBackground = cr;
}



