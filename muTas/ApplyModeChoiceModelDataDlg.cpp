// ApplyModeChoiceModelDataDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ApplyModeChoiceModelDataDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Mode.h"
#include "Project.h"
//^#include "DBaseConnector.h"


// KApplyModeChoiceModelDataDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KApplyModeChoiceModelDataDlg, KDialogEx)

KApplyModeChoiceModelDataDlg::KApplyModeChoiceModelDataDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KApplyModeChoiceModelDataDlg::IDD, pParent)
	, m_strColumnDisplayName(_T(""))
{

}

KApplyModeChoiceModelDataDlg::~KApplyModeChoiceModelDataDlg()
{
}

void KApplyModeChoiceModelDataDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_NAME, m_strColumnDisplayName);
	DDX_Control(pDX, IDC_COMBO1, m_cboModeInfo);
}


BEGIN_MESSAGE_MAP(KApplyModeChoiceModelDataDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KApplyModeChoiceModelDataDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// KApplyModeChoiceModelDataDlg 메시지 처리기입니다.


void KApplyModeChoiceModelDataDlg::SetTarget( KTarget* a_pTarget )
{
	m_pTarget = a_pTarget;
}


void KApplyModeChoiceModelDataDlg::SetColumnName( CString a_strColumnName )
{
	m_strColumnName = a_strColumnName;
}


void KApplyModeChoiceModelDataDlg::SetColumnDisplayName( CString a_strColumnDisplayName)
{
	m_strColumnDisplayName = a_strColumnDisplayName;
}

void KApplyModeChoiceModelDataDlg::SetProject( KProject* a_pProject )
{
	m_pProject = a_pProject;
}


/**************************************
* Overrides                           *
**************************************/
BOOL KApplyModeChoiceModelDataDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitialModeInfo();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KApplyModeChoiceModelDataDlg::InitialModeInfo(void)
{
	ASSERT(NULL != m_pProject);


	std::vector<KMode*> vecMode = m_pProject->GetMode();

	size_t nModeCount = vecMode.size();

	for(size_t i= 0; i< nModeCount; ++i)
	{
		KMode* pMode = vecMode[i];

		int nCur = m_cboModeInfo.AddString(pMode->ModeName());
		m_cboModeInfo.SetItemData( nCur, (DWORD_PTR)( pMode ) );
	}
	m_cboModeInfo.SetCurSel(0);
}


bool KApplyModeChoiceModelDataDlg::UpdateModeChoiceModel(KMode* a_pMode, CString &a_strStartTime, CString &a_strEndTime)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format(_T(" Insert Or Replace into modechoice_model(mode_group_code, detail_mode_id, mode_od_column_name, mode_run_seq, start_date, end_date)  Values ( %d, %d, '%s', %s, '%s', '%s' ) "), 
		a_pMode->ModeGroup(),  a_pMode->ModeID(), m_strColumnName, _T(" (select ifnull(max(mode_run_seq)+1, 1) from modechoice_model) " ),  a_strStartTime, a_strEndTime  );

	try
	{  
		spDBaseConnection->BeginTransaction();

		spDBaseConnection->ExecuteUpdate(strSQL);

		spDBaseConnection->Commit();
	}
	catch(...)
	{
		TxLogDebug(strSQL.AllocSysString());
		spDBaseConnection->RollBack();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KApplyModeChoiceModelDataDlg::NewUpdateModeChoiceModel(KMode* a_pMode, CString &a_strStartTime, CString &a_strEndTime)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDeleteSQL(_T(""));
		CString strInsertSQL(_T(""));

		strDeleteSQL.Format(_T(" DELETE FROM modechoice_model where mode_group_code = %d and mode_od_column_name = '%s' "), a_pMode->ModeGroup(), m_strColumnName);

		spDBaseConnection->ExecuteUpdate(strDeleteSQL);

		strInsertSQL.Format(_T(" Insert into modechoice_model(mode_group_code, detail_mode_id, mode_od_column_name, mode_run_seq, start_date, end_date)  Values ( %d, %d, '%s', %s, '%s', '%s' ) "), 
			a_pMode->ModeGroup(),  a_pMode->ModeID(), m_strColumnName, _T(" (select ifnull(max(mode_run_seq)+1, 1) from modechoice_model) " ),  a_strStartTime, a_strEndTime  );

		spDBaseConnection->ExecuteUpdate(strInsertSQL);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KApplyModeChoiceModelDataDlg::ModeChoiceSelect(KMode* a_pMode)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL = _T("");
	strSQL.Format(_T(" select count(*) from modechoice_model")
				  _T(" where mode_group_code = %d and detail_mode_id = %d and mode_od_column_name = '%s' "),
				  a_pMode->ModeGroup(), a_pMode->ModeID(), m_strColumnName );

	KResultSetPtr pReultSet = spDBaseConnection->ExecuteQuery(strSQL);
	
	int nCount = 0;
	while (pReultSet->Next())
	{
		nCount = pReultSet->GetValueInt(0);
	}

	if(nCount >0)
	{
		return true;
	}
	else
	{
		return  false;
	}
}


bool KApplyModeChoiceModelDataDlg::ModeChoiceSelectOnlyName(KMode* a_pMode)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL = _T("");
	strSQL.Format(_T(" select count(*) from modechoice_model")
		_T(" where mode_group_code = %d and mode_od_column_name = '%s' "),
		a_pMode->ModeGroup(), m_strColumnName );

	KResultSetPtr pReultSet = spDBaseConnection->ExecuteQuery(strSQL);

	int nCount = 0;
	while (pReultSet->Next())
	{
		nCount = pReultSet->GetValueInt(0);
	}

	if(nCount >0)
	{
		return true;
	}
	else
	{
		return  false;
	}
}


void KApplyModeChoiceModelDataDlg::OnBnClickedOk()
{
	int nCur = m_cboModeInfo.GetCurSel();
	KMode* pMode = (KMode*)m_cboModeInfo.GetItemData(nCur);
	/*
	if( true == ModeChoiceSelect(pMode) )
	{
		int nResult = AfxMessageBox(_T("해당 컬럼에 대해 중복된 통행수단 정보가 존재 합니다.\r\n덮어 쓰시겠습니까?"), MB_YESNO);
		if(nResult == IDNO)
		{
			return;
		}
	}
	*/
	if( true == ModeChoiceSelectOnlyName(pMode) )
	{
		int nResult = AfxMessageBox(_T("해당 컬럼 통행수단 정보가 이미 정의되어 있습니다.\r\n재정의 하시겠습니까?"), MB_YESNO);
		if(nResult == IDNO)
		{
			return;
		}
	}

	CTime time = CTime::GetCurrentTime();
	NewUpdateModeChoiceModel( pMode, time.Format(_T("%Y-%m-%d %H:%M:%S")), time.Format(_T("%Y-%m-%d %H:%M:%S")));

	KDialogEx::OnOK();
}
