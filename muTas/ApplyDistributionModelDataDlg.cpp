// ApplyDistributionModelDataDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ApplyDistributionModelDataDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "Project.h"
#include "Purpose.h"

// KApplyDistributionModelDataDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KApplyDistributionModelDataDlg, KDialogEx)

KApplyDistributionModelDataDlg::KApplyDistributionModelDataDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KApplyDistributionModelDataDlg::IDD, pParent)
	, m_strColumnDisplayName(_T(""))
{

}

KApplyDistributionModelDataDlg::~KApplyDistributionModelDataDlg()
{
}

void KApplyDistributionModelDataDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_NAME, m_strColumnDisplayName);
	DDX_Control(pDX, IDC_COMBO1, m_cboPurposeInfo);
}


BEGIN_MESSAGE_MAP(KApplyDistributionModelDataDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KApplyDistributionModelDataDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KApplyDistributionModelDataDlg 메시지 처리기입니다.


void KApplyDistributionModelDataDlg::SetTarget( KTarget* a_pTarget )
{
	m_pTarget = a_pTarget;
}


void KApplyDistributionModelDataDlg::SetColumnName( CString a_strColumnName )
{
	m_strColumnName = a_strColumnName;
}


void KApplyDistributionModelDataDlg::SetColumnDisplayName( CString a_strColumnDisplayName)
{
	m_strColumnDisplayName = a_strColumnDisplayName;
}


void KApplyDistributionModelDataDlg::SetProject( KProject* a_pProject )
{
	m_pProject = a_pProject;
}

/**************************************
* Overrides                           *
**************************************/
BOOL KApplyDistributionModelDataDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitialPurposeInfo();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KApplyDistributionModelDataDlg::InitialPurposeInfo(void)
{
	ASSERT(NULL != m_pProject);

	std::vector<KPurpose*> vecPurpose = m_pProject->GetPurpose();

	size_t nPurposeCount = vecPurpose.size();

	if( nPurposeCount <= 0)
	{
		return;
	}

	for(size_t i= 0; i< nPurposeCount; ++i)
	{
		KPurpose* pPurpose = vecPurpose[i];

		int nCur = m_cboPurposeInfo.AddString(pPurpose->PurposeName());
		m_cboPurposeInfo.SetItemData( nCur, (DWORD_PTR)( pPurpose ) );
	}
	m_cboPurposeInfo.SetCurSel(0);
}


bool KApplyDistributionModelDataDlg::UpdateDistributionModel(KPurpose* a_pPurpose, CString &a_strStartTime, CString &a_strEndTime)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format(_T(" Insert Or Replace into Distribution_Model(object_group_code, detail_object_id, purpose_od_column_name, distribution_run_seq, start_date, end_date)  Values ( %d, %d, '%s', %s, '%s', '%s' ) "), 
		a_pPurpose->PurposeGroup(),  a_pPurpose->PurposeID(), m_strColumnName, _T(" (select ifnull(max(distribution_run_seq)+1, 1) from Distribution_Model) " ),  a_strStartTime, a_strEndTime  );

	try
	{   
		spDBaseConnection->BeginTransaction();

		spDBaseConnection->ExecuteUpdate(strSQL);

		spDBaseConnection->Commit();
	}
	catch(...)
	{
		TxLogDebug(strSQL.AllocSysString());
		spDBaseConnection->Commit();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KApplyDistributionModelDataDlg::NewUpdateDistributionModel(KPurpose* a_pPurpose, CString &a_strStartTime, CString &a_strEndTime)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDeleteSQL(_T(""));
		CString strInsertSQL(_T(""));

		strDeleteSQL.Format(_T(" DELETE FROM Distribution_Model where object_group_code = %d and purpose_od_column_name = '%s' "), a_pPurpose->PurposeGroup(), m_strColumnName);

		spDBaseConnection->ExecuteUpdate(strDeleteSQL);

		strInsertSQL.Format(_T(" Insert into Distribution_Model(object_group_code, detail_object_id, purpose_od_column_name, distribution_run_seq, start_date, end_date)  Values ( %d, %d, '%s', %s, '%s', '%s' ) "), 
			a_pPurpose->PurposeGroup(),  a_pPurpose->PurposeID(), m_strColumnName, _T(" (select ifnull(max(distribution_run_seq)+1, 1) from Distribution_Model) " ),  a_strStartTime, a_strEndTime  );

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


bool KApplyDistributionModelDataDlg::DistributionSelect(KPurpose* a_pPurpose)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL = _T("");
	strSQL.Format(_T(" Select count(*) From Distribution_Model ")
		_T(" where object_group_code = %d and detail_object_id = %d and purpose_od_column_name = '%s' "),
		a_pPurpose->PurposeGroup(), a_pPurpose->PurposeID(), m_strColumnName );

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


bool KApplyDistributionModelDataDlg::DistributionSelectOnlyName(KPurpose* a_pPurpose)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL = _T("");
	strSQL.Format(_T(" Select count(*) From Distribution_Model ")
		_T(" where object_group_code = %d and purpose_od_column_name = '%s' "),
		a_pPurpose->PurposeGroup(), m_strColumnName );

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


void KApplyDistributionModelDataDlg::OnBnClickedOk()
{
	int nCur = m_cboPurposeInfo.GetCurSel();
	KPurpose* pPurpose = (KPurpose*)m_cboPurposeInfo.GetItemData(nCur);
	/*
	if( true == DistributionSelect(pPurpose) )
	{
		int nResult = AfxMessageBox(_T("해당 컬럼에 대해 중복된 통행목적 정보가 존재 합니다.\r\n덮어 쓰시겠습니까?"), MB_YESNO);
		if(nResult == IDNO)
		{
			return;
		}
	}*/

	if( true == DistributionSelectOnlyName(pPurpose) )
	{
		int nResult = AfxMessageBox(_T("해당 컬럼 통행목적 정보가 이미 정의되어 있습니다.\r\n재정의 하시겠습니까?"), MB_YESNO);
		if(nResult == IDNO)
		{
			return;
		}
	}

	CTime time = CTime::GetCurrentTime();
	NewUpdateDistributionModel( pPurpose, time.Format(_T("%Y-%m-%d %H:%M:%S")), time.Format(_T("%Y-%m-%d %H:%M:%S")));

	KDialogEx::OnOK();
}