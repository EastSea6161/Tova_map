// TocZoneProperties.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocZoneProperties.h"
#include "afxdialogex.h"

#include "Target.h"
#include "MapView.h"

#include "LayerXmlRenderer.h"

// KTocZoneProperties 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTocZoneProperties, KDialogEx)

KTocZoneProperties::KTocZoneProperties(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTocZoneProperties::IDD, pParent)
{

}

KTocZoneProperties::~KTocZoneProperties()
{
}

void KTocZoneProperties::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FILL_COLOR,    m_btnFillColorPicker);
	DDX_Control(pDX, IDC_BUTTON_OUTLINE_COLOR, m_btnOutLineColorPicker);
}


BEGIN_MESSAGE_MAP(KTocZoneProperties, KDialogEx)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO1, &KTocZoneProperties::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &KTocZoneProperties::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON_MIN_SCALE, &KTocZoneProperties::OnBnClickedButtonMinScale)
	ON_BN_CLICKED(IDC_BUTTON_MAX_SCALE, &KTocZoneProperties::OnBnClickedButtonMaxScale)
	ON_BN_CLICKED(IDC_BUTTON_RESET,  &KTocZoneProperties::OnBnClickedReset)
	ON_BN_CLICKED(IDC_BUTTON_APPLY,  &KTocZoneProperties::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// KTocZoneProperties 메시지 처리기입니다.
BOOL KTocZoneProperties::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(false, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255));
	//SetResize(IDC_BUTTON_MARKER,   SZ_TOP_LEFT,     SZ_TOP_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HBRUSH KTocZoneProperties::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void KTocZoneProperties::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KTocZoneProperties::SetTargetMap( KTarget* a_pTarget, KMapView* a_pMapView )
{
	m_pTarget        = a_pTarget;
	m_pMapView       = a_pMapView;
	Reset();
}

void KTocZoneProperties::OnBnClickedRadio1()
{
	ActionShowLayerAllScaleSelect();
}
void KTocZoneProperties::ActionShowLayerAllScaleSelect()
{
	CButton* pButton = NULL;

	pButton = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(TRUE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(FALSE);

	pButton = (CButton*)GetDlgItem(IDC_EDIT2);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)GetDlgItem(IDC_EDIT3);
	pButton->EnableWindow(FALSE);
}

void KTocZoneProperties::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ActionShowLayerZoomScaleSelect();
}
void KTocZoneProperties::ActionShowLayerZoomScaleSelect()
{
	CButton* pButton = NULL;

	pButton = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(FALSE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_EDIT2);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_EDIT3);
	pButton->EnableWindow(TRUE);
}

void KTocZoneProperties::OnBnClickedButtonMinScale()
{
	CString strScale;
	double  dMapScale = m_pMapView->GetRequestMapScale();
	strScale.Format(_T("%.0f"), dMapScale);
	SetDlgItemText(IDC_EDIT2, strScale);
}

void KTocZoneProperties::OnBnClickedButtonMaxScale()
{
	CString strScale;
	double  dMapScale = m_pMapView->GetRequestMapScale();
	strScale.Format(_T("%.0f"), dMapScale);
	SetDlgItemText(IDC_EDIT3, strScale);
}

void KTocZoneProperties::OnBnClickedReset()
{
	Reset();
}


void KTocZoneProperties::Reset()
{
    TZoneDispInfo oInfo; {
	    KLayerXmlRenderer oLayerXmlRenderer(m_pTarget);
	    oLayerXmlRenderer.LoadZoneProperty(oInfo);
    }

	m_btnFillColorPicker.SetColor( oInfo.TFillColor );
	m_btnFillColorPicker.SetWindowText( _T("") );
	m_btnFillColorPicker.ShowText( TRUE );
	m_btnFillColorPicker.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

	m_btnOutLineColorPicker.SetColor( oInfo.TLineColor );
	m_btnOutLineColorPicker.SetWindowText( _T("") );
	m_btnOutLineColorPicker.ShowText( TRUE );
	m_btnOutLineColorPicker.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

	CString strTemp;
	strTemp.Format(_T("%.1f"), oInfo.TLineWidth);
	SetDlgItemText(IDC_EDIT_OUTLINE_WIDTH, strTemp);

    double dMinScale = oInfo.TMinScale;
    double dMaxScale = oInfo.TMaxScale;

	strTemp.Format(_T("%.0f"), dMinScale);
	SetDlgItemText(IDC_EDIT2, strTemp);

	strTemp.Format(_T("%.0f"), dMaxScale);
	SetDlgItemText(IDC_EDIT3, strTemp);

	if (dMinScale == 0.0 && dMaxScale == 0.0) {
		ActionShowLayerAllScaleSelect();
	} else {
		ActionShowLayerZoomScaleSelect();
	}
}

void KTocZoneProperties::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Apply();
}

void KTocZoneProperties::Apply()
{
	CString  strTemp;
	CButton* pButton;

	COLORREF fillColor    = m_btnFillColorPicker.GetColor();	
	COLORREF outLineColor = m_btnOutLineColorPicker.GetColor();	

	GetDlgItemText(IDC_EDIT_OUTLINE_WIDTH, strTemp);
	if (!QBicStringChecker::IsNumeric(strTemp))
	{
		AfxMessageBox(_T("숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_OUTLINE_WIDTH)->SetFocus();
		return;
	}

	double   dOutLineWidth = _ttof(strTemp);
	if (dOutLineWidth <= 0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_OUTLINE_WIDTH)->SetFocus();
		return;
	}

	double   dMinScaleValue(0.0), dMaxScaleValue(0.0);

	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO2)
	{
		GetDlgItemText(IDC_EDIT2, strTemp);
		if (!QBicStringChecker::IsNumeric(strTemp))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT2)->SetFocus();
			return;
		}

		dMinScaleValue = _ttof(strTemp);
		if (dMinScaleValue < 0)
		{
			AfxMessageBox(_T("0보다 크거나 같은 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT2)->SetFocus();
			return;
		}

		GetDlgItemText(IDC_EDIT3, strTemp);
		if (!QBicStringChecker::IsNumeric(strTemp))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT3)->SetFocus();
			return;
		}

		dMaxScaleValue = _ttof(strTemp);
		if (dMaxScaleValue < 0)
		{
			AfxMessageBox(_T("0보다 크거나 같은 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT3)->SetFocus();
			return;
		}

		if (dMinScaleValue == 0.0 && dMaxScaleValue == 0.0)
		{
			pButton = (CButton*)GetDlgItem(IDC_RADIO1);
			pButton->SetCheck(TRUE);

			pButton = (CButton*)GetDlgItem(IDC_RADIO2);
			pButton->SetCheck(FALSE);
		}
		else if (dMinScaleValue >= dMaxScaleValue )
		{
			if (dMinScaleValue != 0)
			{
				strTemp.Format(_T("Minimum Scale(%.0f) 값은 Maximum Scale(%.0f) 보다 작아야 합니다."), dMinScaleValue, dMaxScaleValue);
				AfxMessageBox(strTemp);
				GetDlgItem(IDC_EDIT2)->SetFocus();
				return;
			}	
		}
	}

    int nLayerOn(1); {
        ITxLayerPtr spLayer = m_pMapView->MapGetLayer(KLayerID::Zone());
        if (spLayer->LayerOn() == false)
            nLayerOn = 0;
    }

    TZoneDispInfo oInfo; {
        oInfo.TLayerOn   = nLayerOn;
        oInfo.TFillColor = fillColor;
        oInfo.TLineColor = outLineColor;
        oInfo.TLineWidth = dOutLineWidth;
        oInfo.TMinScale  = dMinScaleValue;
        oInfo.TMaxScale  = dMaxScaleValue;
    }

	KLayerXmlRenderer oLayerXmlRenderer(m_pTarget);
	oLayerXmlRenderer.SaveZoneProperty(oInfo);

	m_pMapView->ReloadRender(true, 3);
	Reset();
}