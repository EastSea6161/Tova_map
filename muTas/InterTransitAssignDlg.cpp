// InterTransitAssignDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterTransitAssignDlg.h"
#include "afxdialogex.h"


// KInterTransitAssignDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterTransitAssignDlg, CDialogEx)

KInterTransitAssignDlg::KInterTransitAssignDlg(CWnd* pParent /*=NULL*/)
	:	CDialogEx(KInterTransitAssignDlg::IDD, pParent),
		m_nNoOfRoute(5),
		m_nMaxNoOfZoneConnectors(5),
		m_dblInvehicleTime(-1.0),
		m_dblWaitingTime(-1.0),
		m_dblTransferTime(-1.0),
		m_dblAccessWalkTime(-1.0),
		m_dblEgressWalkTime(-1.0),
		m_dblNoOfTransfers(-1.0),
		m_dblCost(-1.0),
		m_dblFailToBoard(-1.0),
		m_dblSeatOccupancy(-1.0),
		m_dblMaxWalkingTimeToStation(20),
		m_dblTimeOfBoarding(2),
		m_nRadioAssignRule(1)
{
}

KInterTransitAssignDlg::~KInterTransitAssignDlg()
{
}

void KInterTransitAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nNoOfRoute);
	DDV_MinMaxInt(pDX, m_nNoOfRoute, 2, 10);
	DDX_Control(pDX, IDC_SPIN2, m_spinMaxNoOfRoute);
	DDX_Text(pDX, IDC_EDIT11, m_nMaxNoOfZoneConnectors);
	DDV_MinMaxInt(pDX, m_nMaxNoOfZoneConnectors, 1, 10);
	DDX_Control(pDX, IDC_SPIN3, m_spinMaxNoOfZoneConnectors);

	DDX_Control(pDX, IDC_EDIT2, m_edtInVehicleTime);
	DDX_Text(pDX, IDC_EDIT2, m_dblInvehicleTime);

	DDX_Control(pDX, IDC_EDIT3, m_edtWaitingTime);
	DDX_Text(pDX, IDC_EDIT3, m_dblWaitingTime);

	DDX_Control(pDX, IDC_EDIT4, m_edtTransferTime);
	DDX_Text(pDX, IDC_EDIT4, m_dblTransferTime);

	DDX_Control(pDX, IDC_EDIT5, m_edtAccessWalkTime);
	DDX_Text(pDX, IDC_EDIT5, m_dblAccessWalkTime);

	DDX_Control(pDX, IDC_EDIT6, m_edtEgressWalkTime);
	DDX_Text(pDX, IDC_EDIT6, m_dblEgressWalkTime);

	DDX_Control(pDX, IDC_EDIT7, m_edtNoOfTransfers);
	DDX_Text(pDX, IDC_EDIT7, m_dblNoOfTransfers);

	DDX_Control(pDX, IDC_EDIT8, m_edtCost);
	DDX_Text(pDX, IDC_EDIT8, m_dblCost);

	DDX_Control(pDX, IDC_EDIT9, m_edtFailToBoard);
	DDX_Text(pDX, IDC_EDIT9, m_dblFailToBoard);

	DDX_Control(pDX, IDC_EDIT10, m_edtSeatOccupancy);
	DDX_Text(pDX, IDC_EDIT10, m_dblSeatOccupancy);

	DDX_Control(pDX, IDC_EDIT12, m_edtMaxWalkingTimeToStation);
	DDX_Text(pDX, IDC_EDIT12, m_dblMaxWalkingTimeToStation);

	DDX_Control(pDX, IDC_EDIT13, m_editTimeOfBoarding);
	DDX_Text(pDX, IDC_EDIT13, m_dblTimeOfBoarding);

	DDX_Radio(pDX, IDC_RADIO1, m_nRadioAssignRule);
}


BEGIN_MESSAGE_MAP(KInterTransitAssignDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KInterTransitAssignDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &KInterTransitAssignDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT11, &KInterTransitAssignDlg::OnEnChangeEdit11)
	ON_BN_CLICKED(IDOK, &KInterTransitAssignDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &KInterTransitAssignDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &KInterTransitAssignDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// KInterTransitAssignDlg 메시지 처리기입니다.

void KInterTransitAssignDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void KInterTransitAssignDlg::OnEnChangeEdit1()
{
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

BOOL KInterTransitAssignDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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

void KInterTransitAssignDlg::OnEnChangeEdit11()
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

void KInterTransitAssignDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

void KInterTransitAssignDlg::EnableSingleRoute()
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

void KInterTransitAssignDlg::EnableMultipleRoute()
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

void KInterTransitAssignDlg::OnBnClickedRadio1()
{
	EnableSingleRoute();
}

void KInterTransitAssignDlg::OnBnClickedRadio2()
{
	EnableMultipleRoute();
}

bool KInterTransitAssignDlg::WriteInputArgument(CStdioFileEx& of)
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
			m_nNoOfRoute, m_dblInvehicleTime, m_dblWaitingTime, m_dblTransferTime, m_dblAccessWalkTime, m_dblEgressWalkTime, 
			m_dblNoOfTransfers, m_dblCost, m_dblFailToBoard, m_dblSeatOccupancy);
		of.WriteString(strOut);

	}

	strOut.Format(_T("%d\t%f\t%f\r\n"), m_nMaxNoOfZoneConnectors, m_dblMaxWalkingTimeToStation, m_dblTimeOfBoarding);
	of.WriteString(strOut);

	return true;
}

