// KASetTransitAssignOptionsDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KASetTransitAssignOptionsDlg.h"
#include "afxdialogex.h"


// KASetTransitAssignOptionsDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KASetTransitAssignOptionsDlg, KDialogEx)

KASetTransitAssignOptionsDlg::KASetTransitAssignOptionsDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KASetTransitAssignOptionsDlg::IDD, pParent)
    , m_nNoOfRoute(5)
    , m_nMaxNoOfZoneConnectors(5)
    , m_dInvehicleTime(-1.0)
    , m_dWaitingTime(-1.0)
    , m_dTransferTime(-1.0)
    , m_dAccessWalkTime(-1.0)
    , m_dEgressWalkTime(-1.0)
    , m_dNoOfTransfers(-1.0)
    , m_dCost(-1.0)
    , m_dFailToBoard(-1.0)
    , m_dSeatOccupancy(-1.0)
    , m_dMaxWalkingTimeToStation(20)
    , m_dTimeOfBoarding(2)
    , m_nRadioAssignRule(1)
{

}

KASetTransitAssignOptionsDlg::~KASetTransitAssignOptionsDlg()
{
}

void KASetTransitAssignOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_nNoOfRoute);
    DDV_MinMaxInt(pDX, m_nNoOfRoute, 2, 10);
    DDX_Control(pDX, IDC_SPIN2, m_spinMaxNoOfRoute);
    DDX_Text(pDX, IDC_EDIT11, m_nMaxNoOfZoneConnectors);
    DDV_MinMaxInt(pDX, m_nMaxNoOfZoneConnectors, 1, 10);
    DDX_Control(pDX, IDC_SPIN3, m_spinMaxNoOfZoneConnectors);

    DDX_Control(pDX, IDC_EDIT2, m_edtInVehicleTime);
    DDX_Text(pDX, IDC_EDIT2, m_dInvehicleTime);

    DDX_Control(pDX, IDC_EDIT3, m_edtWaitingTime);
    DDX_Text(pDX, IDC_EDIT3, m_dWaitingTime);

    DDX_Control(pDX, IDC_EDIT4, m_edtTransferTime);
    DDX_Text(pDX, IDC_EDIT4, m_dTransferTime);

    DDX_Control(pDX, IDC_EDIT5, m_edtAccessWalkTime);
    DDX_Text(pDX, IDC_EDIT5, m_dAccessWalkTime);

    DDX_Control(pDX, IDC_EDIT6, m_edtEgressWalkTime);
    DDX_Text(pDX, IDC_EDIT6, m_dEgressWalkTime);

    DDX_Control(pDX, IDC_EDIT7, m_edtNoOfTransfers);
    DDX_Text(pDX, IDC_EDIT7, m_dNoOfTransfers);

    DDX_Control(pDX, IDC_EDIT8, m_edtCost);
    DDX_Text(pDX, IDC_EDIT8, m_dCost);

    DDX_Control(pDX, IDC_EDIT9, m_edtFailToBoard);
    DDX_Text(pDX, IDC_EDIT9, m_dFailToBoard);

    DDX_Control(pDX, IDC_EDIT10, m_edtSeatOccupancy);
    DDX_Text(pDX, IDC_EDIT10, m_dSeatOccupancy);

    DDX_Control(pDX, IDC_EDIT12, m_edtMaxWalkingTimeToStation);
    DDX_Text(pDX, IDC_EDIT12, m_dMaxWalkingTimeToStation);

    DDX_Control(pDX, IDC_EDIT13, m_editTimeOfBoarding);
    DDX_Text(pDX, IDC_EDIT13, m_dTimeOfBoarding);

    DDX_Radio(pDX, IDC_RADIO1, m_nRadioAssignRule);
}


BEGIN_MESSAGE_MAP(KASetTransitAssignOptionsDlg, KDialogEx)
    ON_BN_CLICKED(IDCANCEL, &KASetTransitAssignOptionsDlg::OnBnClickedCancel)
    ON_EN_CHANGE(IDC_EDIT1, &KASetTransitAssignOptionsDlg::OnEnChangeEdit1)
    ON_EN_CHANGE(IDC_EDIT11, &KASetTransitAssignOptionsDlg::OnEnChangeEdit11)
    ON_BN_CLICKED(IDOK, &KASetTransitAssignOptionsDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_RADIO1, &KASetTransitAssignOptionsDlg::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, &KASetTransitAssignOptionsDlg::OnBnClickedRadio2)
//    ON_BN_CLICKED(IDC_BUTTON1, &KASetTransitAssignOptionsDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// KASetTransitAssignOptionsDlg 메시지 처리기입니다.


void KASetTransitAssignOptionsDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KDialogEx::OnCancel();
}


void KASetTransitAssignOptionsDlg::OnEnChangeEdit1()
{
    // TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
    // KDialogEx::OnInitDialog() 함수를 재지정 
    //하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
    // 이 알림 메시지를 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
    int nValue = GetDlgItemInt(IDC_EDIT1);
    if (nValue < 2)
    {
        SetDlgItemText(IDC_EDIT1, _T("2"));
    }

    if (nValue > 10)
    {
        SetDlgItemText(IDC_EDIT1, _T("10"));
    }
}


BOOL KASetTransitAssignOptionsDlg::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    m_spinMaxNoOfRoute.SetRange32(2, 10);       
    m_spinMaxNoOfZoneConnectors.SetRange32(1, 10);

    m_edtInVehicleTime.EnableRealNumber(true);
    m_edtWaitingTime.EnableRealNumber(true);

    m_edtTransferTime.EnableRealNumber(true);
    m_edtAccessWalkTime.EnableRealNumber(true);
    m_edtEgressWalkTime.EnableRealNumber(true);
    m_edtNoOfTransfers.EnableRealNumber(true);
    m_edtCost.EnableRealNumber(true);
    m_edtFailToBoard.EnableRealNumber(true);
    m_edtSeatOccupancy.EnableRealNumber(true);
    m_edtMaxWalkingTimeToStation.EnableRealNumber(true);
    m_editTimeOfBoarding.EnableRealNumber(true);

    if (m_nRadioAssignRule == 0)
    {
        EnableSingleRoute();
    }
    else
    {
        EnableMultipleRoute();
    }

    return TRUE;  
}


void KASetTransitAssignOptionsDlg::OnEnChangeEdit11()
{
    int nValue = GetDlgItemInt(IDC_EDIT11);
    if (nValue < 1)
    {
        SetDlgItemText(IDC_EDIT11, _T("1"));
    }

    if (nValue > 10)
    {
        SetDlgItemText(IDC_EDIT11, _T("10"));
    }
}


void KASetTransitAssignOptionsDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdateData(TRUE);    
    KDialogEx::OnOK();
}

void KASetTransitAssignOptionsDlg::EnableSingleRoute()
{
    CEdit* pEdit = NULL;
    pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT4);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT5);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT6);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT7);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT8);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT9);
    pEdit->EnableWindow(FALSE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT10);
    pEdit->EnableWindow(FALSE);
}

void KASetTransitAssignOptionsDlg::EnableMultipleRoute()
{
    CEdit* pEdit = NULL;
    pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT4);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT5);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT6);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT7);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT8);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT9);
    pEdit->EnableWindow(TRUE);

    pEdit = (CEdit*)GetDlgItem(IDC_EDIT10);
    pEdit->EnableWindow(TRUE);
}
void KASetTransitAssignOptionsDlg::OnBnClickedRadio1()
{
    EnableSingleRoute();
}


void KASetTransitAssignOptionsDlg::OnBnClickedRadio2()
{
    EnableMultipleRoute();
}

bool KASetTransitAssignOptionsDlg::WriteInputArgument(CStdioFileEx& of)
{
    CString strOut;

    if (m_nRadioAssignRule == 0)
    {
        of.WriteString(_T("transit_assign_options\t2\r\n"));        
        strOut.Format(_T("%d\r\n"), m_nRadioAssignRule + 1);
        of.WriteString(strOut);
    }
    else
    {
        of.WriteString(_T("transit_assign_options\t3\r\n"));
        strOut.Format(_T("%d\r\n"), m_nRadioAssignRule + 1);
        of.WriteString(strOut);

        strOut.Format(_T("%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\r\n"), 
            m_nNoOfRoute, m_dInvehicleTime, m_dWaitingTime, m_dTransferTime, m_dAccessWalkTime, m_dEgressWalkTime, 
            m_dNoOfTransfers, m_dCost, m_dFailToBoard, m_dSeatOccupancy);
        of.WriteString(strOut);
        
    }

    strOut.Format(_T("%d\t%f\t%f\r\n"), m_nMaxNoOfZoneConnectors, m_dMaxWalkingTimeToStation, m_dTimeOfBoarding);
    of.WriteString(strOut);

    return true;
}
