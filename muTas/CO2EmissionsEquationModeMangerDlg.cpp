// CO2EmissionsEquationModeMangerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CO2EmissionsEquationModeMangerDlg.h"
#include "afxdialogex.h"


// KCO2EmissionsEquationModeMangerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCO2EmissionsEquationModeMangerDlg, KDialogEx)

KCO2EmissionsEquationModeMangerDlg::KCO2EmissionsEquationModeMangerDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KCO2EmissionsEquationModeMangerDlg::IDD, pParent)
	, m_nNextModeID(0)
{

}

KCO2EmissionsEquationModeMangerDlg::~KCO2EmissionsEquationModeMangerDlg()
{
}

void KCO2EmissionsEquationModeMangerDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboMode);
}


BEGIN_MESSAGE_MAP(KCO2EmissionsEquationModeMangerDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KCO2EmissionsEquationModeMangerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KCO2EmissionsEquationModeMangerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &KCO2EmissionsEquationModeMangerDlg::OnBnClickedDel)
	ON_BN_CLICKED(IDC_BUTTON2, &KCO2EmissionsEquationModeMangerDlg::OnBnClickedAdd)
END_MESSAGE_MAP()


// KCO2EmissionsEquationModeMangerDlg 메시지 처리기입니다.


BOOL KCO2EmissionsEquationModeMangerDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitComboCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KCO2EmissionsEquationModeMangerDlg::InitComboCtrl( void )
{
	m_cboMode.ResetContent();

	int nIndex;

	std::map<int, CString>::iterator bIter = m_mapMode.begin();

	while (bIter != m_mapMode.end())
	{
		nIndex = m_cboMode.InsertString(-1, bIter->second);
		m_cboMode.SetItemData(nIndex, bIter->first);

		m_nNextModeID = bIter->first;

		++bIter;
	}

	++m_nNextModeID;

	m_cboMode.SetCurSel(0);
}

void KCO2EmissionsEquationModeMangerDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnOK();
}


void KCO2EmissionsEquationModeMangerDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}


void KCO2EmissionsEquationModeMangerDlg::OnBnClickedDel()
{
	try
	{
		int nModeID = m_cboMode.GetItemData(m_cboMode.GetCurSel());

		if (m_setUsedModeID.find(nModeID) != m_setUsedModeID.end())
		{
			AfxMessageBox(_T("선택한 통행수단 관련 설정 항목을 먼저 삭제해 주세요."));
			return;
		}

		std::map<int, CString>::iterator fIter = m_mapMode.find(nModeID);

		if (fIter == m_mapMode.end())
		{
			AfxMessageBox(_T("데이터에 오류가 존재합니다."));
			return;
		}

		m_mapMode.erase(nModeID);

		InitComboCtrl();

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("삭제되었습니다."));
		}
		else {
			AfxMessageBox(_T("Deleted successfully."));
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}
}


void KCO2EmissionsEquationModeMangerDlg::OnBnClickedAdd()
{
	try
	{
		bool bIsDupl = false;

		CString strAddModeName;
		GetDlgItemText(IDC_EDIT1, strAddModeName);

		strAddModeName.Trim();

		if (strAddModeName.IsEmpty())
		{
			AfxMessageBox(_T("수단명을 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		std::map<int, CString>::iterator bIter = m_mapMode.begin();

		while (bIter != m_mapMode.end())
		{
			if (strAddModeName.CompareNoCase(bIter->second) == 0)
			{
				bIsDupl = true;
				break;
			}

			++bIter;
		}

		if (bIsDupl)
		{
			AfxMessageBox(_T("입력한 이름이 이미 존재합니다."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		m_mapMode.insert(std::make_pair(m_nNextModeID, strAddModeName));

		InitComboCtrl();

		AfxMessageBox(_T("추가하였습니다."));
		SetDlgItemText(IDC_EDIT1, _T(""));
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}
}
