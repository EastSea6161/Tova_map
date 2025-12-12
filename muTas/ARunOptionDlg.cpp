/**
 * @file ARunOptionDlg.cpp
 * @brief KARunOptionDlg 구현 파일
 * @author 
 * @date 2011.06.20
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "ARunOptionDlg.h"
#include "afxdialogex.h"
#include "KBulkDBaseUtil.h"
#include "AWarmStartDlg.h"
#include "Scenario.h"
#include "DBaseCheckData.h"
#include "..\QBicCommon\QBicWindowSystem.h"

IMPLEMENT_DYNAMIC(KARunOptionDlg, KDialogEx)

KARunOptionDlg::KARunOptionDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KARunOptionDlg::IDD, pParent)
{

}

KARunOptionDlg::~KARunOptionDlg()
{
}

void KARunOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_chkTurn);
	DDX_Control(pDX, IDC_CHECK2, m_chkVolume);
	DDX_Control(pDX, IDC_CHECK3, m_chkPath);
	DDX_Control(pDX, IDC_CHECK4, m_chkIntersection);
	DDX_Control(pDX, IDC_STC_TRUN, m_stcTurn);
	DDX_Control(pDX, IDC_STC_BUS, m_stcVolume);
	DDX_Control(pDX, IDC_STC_PATH, m_stcPath);
	DDX_Control(pDX, IDC_STC_INTER, m_stcIntersection);
	DDX_Control(pDX, IDC_CHECK_WARM, m_chkWarm);
	DDX_Control(pDX, IDC_CHECK5, m_chkTurnVolume);
	DDX_Control(pDX, IDC_COMBO_ITER, m_cboBalanceIter);
}


BEGIN_MESSAGE_MAP(KARunOptionDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KARunOptionDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &KARunOptionDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &KARunOptionDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &KARunOptionDlg::OnBnClickedButton4)
    ON_BN_CLICKED(IDCANCEL, &KARunOptionDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &KARunOptionDlg::OnOK)
	ON_BN_CLICKED(IDC_BUTTON_WARM, &KARunOptionDlg::OnBnClickedButtonWarm)
	ON_BN_CLICKED(IDC_CHECK5, &KARunOptionDlg::OnBnClickedCheckTurnVolumn)
	ON_BN_CLICKED(IDC_CHECK1, &KARunOptionDlg::OnBnClickedCheckTurn)
	ON_BN_CLICKED(IDC_CHECK_WARM, &KARunOptionDlg::OnBnClickedCheckWarm)
	ON_CBN_SELCHANGE(IDC_COMBO_ITER, &KARunOptionDlg::OnCbnSelchangeComboIter)
END_MESSAGE_MAP()



KARunningOptionInfo& KARunOptionDlg::RunningOption(void)
{
	return m_RunningOption;
}


void KARunOptionDlg::RunningOption(KARunningOptionInfo& info)
{
	m_RunningOption = info;
	m_pSelTarget = m_RunningOption.Target();
	m_pSelScnario = m_RunningOption.Scnario();

}


// KARunOptionDlg 메시지 처리기입니다.
BOOL KARunOptionDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CString strNotExists = _T("Data not exists");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strNotExists = _T("데이터 존재하지 않음");
	}
	CString strEmpty     = _T("");

    bool bExistTurn    = KBulkDBaseUtil::ExitstTurnData(m_pTarget->GetDBaseConnection());
    bool bExistTransit = KBulkDBaseUtil::ExistTransitData(m_pTarget->GetDBaseConnection());   

    bool bExistPathAnalysis = true;
    bool bExistIntersection = KBulkDBaseUtil::ExistInterSectionData(m_pTarget->GetDBaseConnection());

	if ( bExistTurn )
	{
        m_chkTurn.SetCheck(m_RunningOption.IncludeTurnRestriction() == true ? TRUE : FALSE);
		m_chkTurnVolume.SetCheck(m_RunningOption.IncludeTurnVolume() == true ? TRUE : FALSE);
		m_stcTurn.SetWindowText(strEmpty);
	}
	else if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || KEMAssignmentModelSegment == m_emAModel)
	{
		m_chkTurn.EnableWindow(FALSE);
		m_stcTurn.SetWindowText(strNotExists);
		m_chkTurnVolume.SetCheck(m_RunningOption.IncludeTurnVolume() == true ? TRUE : FALSE);
	}
	else
	{
        m_chkTurn.EnableWindow(FALSE);
        m_chkTurnVolume.EnableWindow(FALSE);
		m_stcTurn.SetWindowText(strNotExists);
	}

	if (m_emAModel == KEMAssignmentModelUserEquilibriumFW)
	{
		if( bExistIntersection )
		{
			m_chkIntersection.SetCheck(m_RunningOption.IncludeIntersectionDelay() == true ? TRUE : FALSE);
			m_stcIntersection.SetWindowText(strEmpty);
		}
		else
		{
			m_chkIntersection.EnableWindow(FALSE);
			m_stcIntersection.SetWindowText(strNotExists);
		}

	}
	else
	{
		m_chkIntersection.EnableWindow(FALSE);
		m_stcIntersection.SetWindowText(strEmpty);
	}

	if ((m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment) && 1 == m_nVDFCount )
	{
		m_chkPath.SetCheck(m_RunningOption.IncludePathAnalysis() == true ? TRUE : FALSE);
		m_stcPath.SetWindowText(strEmpty);

		m_chkWarm.SetCheck(m_RunningOption.IncludeWarmStart() == true ? TRUE : FALSE);
		m_chkWarm.EnableWindow(TRUE);
		if (m_pSelScnario != nullptr && m_pSelTarget != nullptr)
		{
			CString strCaption(_T(""));
			strCaption.Format(_T("%s_%s"), m_pSelScnario->GetName(), m_pSelTarget->GetName());
			GetDlgItem(IDC_EDIT1)->SetWindowTextW(strCaption);
		}

		if ( bExistTransit )
		{
			m_chkVolume.SetCheck(m_RunningOption.IncludeBusInitialVolume() == true ? TRUE : FALSE);
			m_stcVolume.SetWindowText(strEmpty);
		}
		else
		{
			m_chkVolume.EnableWindow(FALSE);
			m_stcVolume.SetWindowText(strNotExists);
		}

		int nZoneCount(0);
		KDBaseCheckData::GetZoneCount(m_pTarget, nZoneCount);
		if (nZoneCount > 2000 && QBicWindowSystem::ProcessOnWindowsStatus() < 1 )
		{
			m_cboBalanceIter.EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ITER)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_BAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_ITER)->EnableWindow(FALSE);
		}
		else
		{
			m_cboBalanceIter.EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_ITER)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_BAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_ITER)->EnableWindow(TRUE);
		}

		InitComboBoxIter();
	}
	else
	{
		m_chkPath.EnableWindow(FALSE);
		m_stcPath.SetWindowText(strEmpty);

		m_chkVolume.EnableWindow(FALSE);
		m_stcVolume.SetWindowText(strEmpty);
		
		m_chkWarm.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WARM)->EnableWindow(FALSE);

		m_cboBalanceIter.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ITER)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_BAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ITER)->EnableWindow(FALSE);
	}

    // 2015-05-01(nombara) - 명지대 요청사항 
    // - 옵션들이 이해가 가지 않아서 마지막에 다시 처리하는 구성으로.. ㅠㅠ
    if ((m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment) && 1 < m_nVDFCount ) {
        if ( bExistTransit ) {
            m_chkVolume.SetCheck(m_RunningOption.IncludeBusInitialVolume() == true ? TRUE : FALSE);
            m_stcVolume.SetWindowText(strEmpty);
        }
        else {
            m_chkVolume.EnableWindow(FALSE);
            m_stcVolume.SetWindowText(strNotExists);
        }
    }

	if ( bExistPathAnalysis )
	{
		m_chkPath.SetCheck(m_RunningOption.IncludePathAnalysis() == true ? TRUE : FALSE);
		m_stcPath.SetWindowText(strEmpty);
	}
	else
	{
		m_chkPath.EnableWindow(FALSE);
		m_stcPath.SetWindowText(strNotExists);
	}

	// 2019.04.16 웝스타트가 설정되어 있으면 "구간 교통량 균형 분석 과정 반복 횟수 설정"을 비활성화
	if (m_RunningOption.IncludeWarmStart())
	{
		m_cboBalanceIter.SetCurSel(0);
		ContorlComboIter();

		GetDlgItem(IDC_COMBO_ITER)->EnableWindow(FALSE);
	}
	

	return TRUE;  
}


void KARunOptionDlg::OnOK()
{
	m_RunningOption.IncludeTurnRestriction(m_chkTurn.GetCheck() == BST_CHECKED ? true : false);
	m_RunningOption.IncludeBusInitialVolume(m_chkVolume.GetCheck() == BST_CHECKED ? true : false);
	m_RunningOption.IncludePathAnalysis(m_chkPath.GetCheck() == BST_CHECKED ? true : false);
	m_RunningOption.IncludeIntersectionDelay(m_chkIntersection.GetCheck() == BST_CHECKED ? true : false);
	m_RunningOption.IncludeWarmStart(m_chkWarm.GetCheck() == BST_CHECKED ? true : false);
	m_RunningOption.IncludeTurnVolume(m_chkTurnVolume.GetCheck() == BST_CHECKED ? true : false);

	if (m_RunningOption.IncludeWarmStart() == true)
	{
		if (m_pSelScnario != nullptr && m_pSelTarget != nullptr)
		{
			m_RunningOption.Scnario(m_pSelScnario);
			m_RunningOption.Target(m_pSelTarget);
		}
		else
		{
			if (CheckTargetBushFile(m_pTarget) == false)
			{
				AfxMessageBox(_T("현재 활성화된 네트워크에 경로 교통량 분석 파일이 존재하지 않습니다."));
				return;
			}
			else
			{
				m_RunningOption.Target(m_pTarget);
				m_RunningOption.Scnario((KScenario*)m_pTarget->GetParentObject());
			}
		}
	}
	else
	{
		m_RunningOption.Scnario(NULL);
		m_RunningOption.Target(NULL);
	}

	if ((m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment) && 1 == m_nVDFCount )
	{
		int nGetCur     = m_cboBalanceIter.GetCurSel();
		int nIterOption = (int)m_cboBalanceIter.GetItemData(nGetCur);
		
		if (nIterOption == -1)
		{
			m_RunningOption.m_nIteration = -1;
		}
		else if (nIterOption == 0)
		{
			m_RunningOption.m_nIteration = 0;
		}
		else if (nIterOption == 10)
		{
			m_RunningOption.m_nIteration = 10;
		}
		else if (nIterOption == 30)
		{
			m_RunningOption.m_nIteration = 30;
		}
		else if (nIterOption == 50)
		{
			m_RunningOption.m_nIteration = 50;
		}
		else
		{
			CString strIter(_T(""));
			GetDlgItemText(IDC_EDIT_ITER, strIter);
			if (!QBicStringChecker::IsUnSignedInteger(strIter))
			{
				AfxMessageBox(_T("반복 수행 횟수에 숫자가 아닌 값을 입력하였습니다."));
				return;
			}
			if (strIter.CompareNoCase(_T("0")) == 0)
			{
				AfxMessageBox(_T("반복 수행 횟수에 0보다 큰 숫자를 입력해 주세요."));
				return;
			}
			if (strIter.CompareNoCase(_T("")) == 0)
			{
				AfxMessageBox(_T("반복 수행 횟수에 숫자를 입력해 주세요."));
				return;
			}
			m_RunningOption.m_nIteration = _ttoi(strIter);
		}
	}

	KDialogEx::OnOK();
}


void KARunOptionDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KARunOptionDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KARunOptionDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KARunOptionDlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void KARunOptionDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KDialogEx::OnCancel();
}

void KARunOptionDlg::SetAssignMode( KEMAssignmentModel emAModel )
{
	m_emAModel = emAModel;
}

void KARunOptionDlg::SetVDFCount( int a_nVDFCount )
{
	m_nVDFCount = a_nVDFCount;
}


void KARunOptionDlg::OnBnClickedButtonWarm()
{
	KAWarmStartDlg oDlg(m_pTarget, m_lstInputInfo,  m_pSelTarget, m_pSelScnario);

	if (oDlg.DoModal() == IDOK)
	{
		m_pSelTarget = oDlg.GetWarmTarget();
		m_pSelScnario = oDlg.GetWarmScnario();

		CString strCaption(_T(""));
		strCaption.Format(_T("%s_%s"), m_pSelScnario->GetName(), m_pSelTarget->GetName());
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(strCaption);
		// 2018.08.02 명지대 요청사항 수정
        ((CButton*)GetDlgItem(IDC_CHECK_WARM))->SetCheck(TRUE);

        if (nullptr != m_pSelTarget) 
        {
			m_chkTurn.SetCheck(oDlg.isTurnRestriction() ? BST_CHECKED : BST_UNCHECKED);
			m_chkTurnVolume.SetCheck(oDlg.isIncludeTurnVolume() ? BST_CHECKED : BST_UNCHECKED);
			m_chkVolume.SetCheck(oDlg.isBusInitialVolume() ? BST_CHECKED : BST_UNCHECKED);
			m_chkPath.SetCheck(oDlg.isPathAnalysis() ? BST_CHECKED : BST_UNCHECKED);
			m_chkIntersection.SetCheck(oDlg.isIntersectionDelay() ? BST_CHECKED : BST_UNCHECKED);
		}
		// 2018.08.02 명지대 요청사항 수정 끝
	}
	else
	{
		if (m_pSelTarget == nullptr)
		{
			((CButton*)GetDlgItem(IDC_CHECK_WARM))->SetCheck(FALSE);
		}
	}
}


void KARunOptionDlg::OnBnClickedCheckTurnVolumn()
{
	UpdateData(FALSE);

	if (IsDlgButtonChecked(IDC_CHECK5) == TRUE)
	{
		if (m_emAModel != KEMAssignmentModelUserEquilibriumOB && m_emAModel != KEMAssignmentModelSegment)
		{
			m_chkTurn.SetCheck(TRUE);
		}
	}
	else
	{
		if (m_emAModel != KEMAssignmentModelUserEquilibriumOB && m_emAModel != KEMAssignmentModelSegment)
		{
			m_chkTurn.SetCheck(FALSE);
		}
	}

	UpdateData(TRUE);
}


void KARunOptionDlg::OnBnClickedCheckTurn()
{
	if (IsDlgButtonChecked(IDC_CHECK1) == FALSE)
	{
		if (m_emAModel != KEMAssignmentModelUserEquilibriumOB && m_emAModel != KEMAssignmentModelSegment)
		{
			((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(FALSE);
		}
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(TRUE);
	}
}

bool KARunOptionDlg::CheckTargetBushFile( KTarget* a_pTarget )
{
	try
	{
		CString strBushPath(_T(""));
		strBushPath.Format(_T("%s\\%s\\%s"), a_pTarget->GetLocation(),BUSH_PATHVOLUMN_FOLDER_NAME,_T("*.bsh"));

		CFileFind oCFileFinder;
		if (FALSE == oCFileFinder.FindFile(strBushPath))
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

void KARunOptionDlg::SetInputIinfo( std::list<KAInputInfo*> a_lstInputInfo )
{
	m_lstInputInfo = a_lstInputInfo;
}


void KARunOptionDlg::OnBnClickedCheckWarm()
{
	if (IsDlgButtonChecked(IDC_CHECK_WARM) == TRUE)
	{
		KAWarmStartDlg oDlg(m_pTarget, m_lstInputInfo,  m_pSelTarget, m_pSelScnario);
		oDlg.SetRunningOptionPathAnalysis(m_chkPath.GetCheck() == BST_CHECKED ? true : false);

		if (oDlg.DoModal() == IDOK)
		{
			m_pSelTarget = oDlg.GetWarmTarget();
			m_pSelScnario = oDlg.GetWarmScnario();

			CString strCaption(_T(""));
			strCaption.Format(_T("%s_%s"), m_pSelScnario->GetName(), m_pSelTarget->GetName());
			GetDlgItem(IDC_EDIT1)->SetWindowText(strCaption);

            m_cboBalanceIter.SetCurSel(0);
            ContorlComboIter();

            GetDlgItem(IDC_COMBO_ITER)->EnableWindow(FALSE);

			// 2018.08.02 명지대 요청사항 수정
			m_chkTurn.SetCheck(oDlg.isTurnRestriction() ? BST_CHECKED : BST_UNCHECKED);
			m_chkTurnVolume.SetCheck(oDlg.isIncludeTurnVolume() ? BST_CHECKED : BST_UNCHECKED);
			m_chkVolume.SetCheck(oDlg.isBusInitialVolume() ? BST_CHECKED : BST_UNCHECKED);
			m_chkPath.SetCheck(oDlg.isPathAnalysis() ? BST_CHECKED : BST_UNCHECKED);
			m_chkIntersection.SetCheck(oDlg.isIntersectionDelay() ? BST_CHECKED : BST_UNCHECKED);
			// 2018.08.02 명지대 요청사항 수정 끝
		}
		else
		{
			if (m_pSelTarget == nullptr)
			{
				((CButton*)GetDlgItem(IDC_CHECK_WARM))->SetCheck(FALSE);

                GetDlgItem(IDC_COMBO_ITER)->EnableWindow(TRUE);
			}
            else {
                m_cboBalanceIter.SetCurSel(0);
                ContorlComboIter();

                GetDlgItem(IDC_COMBO_ITER)->EnableWindow(FALSE);
            }
		}
	}
    else {
		m_pSelScnario = NULL;
		m_pSelTarget = NULL;
        GetDlgItem(IDC_COMBO_ITER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
    }
}

void KARunOptionDlg::InitComboBoxIter( void )
{
	try
	{
		int nSetCur(0);
		GetDlgItem(IDC_EDIT_ITER)->EnableWindow(FALSE);

		int nIndex0 = 0;
        if (KmzSystem::GetLanguage() == KEMKorea) {
            nIndex0 = m_cboBalanceIter.AddString(_T("설정값(0)"));
        }
        else {
            nIndex0 = m_cboBalanceIter.AddString(_T("Default(0)"));
        }
		m_cboBalanceIter.SetItemData(nIndex0, 0);

		int nIndex10 = m_cboBalanceIter.AddString(_T("10"));
		m_cboBalanceIter.SetItemData(nIndex10, 10);

		int nIndexDef = m_cboBalanceIter.AddString(_T("20"));
        //int nIndexDef = 0;
        //if (KmzSystem::GetLanguage() == KEMKorea) {
        //    nIndexDef = m_cboBalanceIter.AddString(_T("설정값(20)"));
        //}
        //else {
        //    nIndexDef = m_cboBalanceIter.AddString(_T("Default(20)"));
        //}
		m_cboBalanceIter.SetItemData(nIndexDef, -1);

		int nIndex30 = m_cboBalanceIter.AddString(_T("30"));
		m_cboBalanceIter.SetItemData(nIndex30, 30);

		int nIndex50 = m_cboBalanceIter.AddString(_T("50"));
		m_cboBalanceIter.SetItemData(nIndex50, 50);
		
		int nIndexUser = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nIndexUser = m_cboBalanceIter.AddString(_T("사용자 입력"));
		}
		else {
			nIndexUser = m_cboBalanceIter.AddString(_T("User Input"));
		}
		m_cboBalanceIter.SetItemData(nIndexUser, -999);
		
		if (m_RunningOption.m_nIteration == 0)
		{
			m_cboBalanceIter.SetCurSel(nIndex0);
			//GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("0"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("설정값(0)"));
			}
			else {
				GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("Default(0)"));
			}
		}
		else if (m_RunningOption.m_nIteration == -1)
		{
			m_cboBalanceIter.SetCurSel(nIndexDef);
            GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("20"));
			//if (KmzSystem::GetLanguage() == KEMKorea) {
			//	GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("설정값(20)"));
			//}
			//else {
			//	GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("Default(20)"));
			//}
		}
		else if (m_RunningOption.m_nIteration == 10)
		{
			m_cboBalanceIter.SetCurSel(nIndex10);
			GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("10"));
		}
		else if (m_RunningOption.m_nIteration == 30)
		{
			m_cboBalanceIter.SetCurSel(nIndex30);
			GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("30"));
		}
		else if (m_RunningOption.m_nIteration == 50)
		{
			m_cboBalanceIter.SetCurSel(nIndex50);
		}
		else 
		{
			m_cboBalanceIter.SetCurSel(nIndexUser);
			CString strIter(_T(""));
			strIter.Format(_T("%d"), m_RunningOption.m_nIteration);
			GetDlgItem(IDC_EDIT_ITER)->SetWindowText(strIter);
			GetDlgItem(IDC_EDIT_ITER)->EnableWindow(TRUE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KARunOptionDlg::OnCbnSelchangeComboIter()
{
    ContorlComboIter();
}


void KARunOptionDlg::ContorlComboIter()
{
    int nGetCur = m_cboBalanceIter.GetCurSel();
    int nIterOption = (int)m_cboBalanceIter.GetItemData(nGetCur);
    GetDlgItem(IDC_EDIT_ITER)->EnableWindow(FALSE);
    if (nIterOption == 0)
    {
        //GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("0"));
        if (KmzSystem::GetLanguage() == KEMKorea) {
            GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("설정값(0)"));
        }
        else {
            GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("Default(0)"));
        }
    }
    else if (nIterOption == -1)
    {
        GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("20"));
        //if (KmzSystem::GetLanguage() == KEMKorea) {
        //	GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("설정값(20)"));
        //}
        //else {
        //	GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("Default(20)"));
        //}
    }
    else if (nIterOption == 10)
    {
        GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("10"));
    }
    else if (nIterOption == 30)
    {
        GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("30"));
    }
    else if (nIterOption == 50)
    {
        GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T("50"));
    }
    else
    {
        GetDlgItem(IDC_EDIT_ITER)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_ITER)->SetWindowText(_T(""));
    }
}