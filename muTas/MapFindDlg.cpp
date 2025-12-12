// MapFindDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapFindDlg.h"
#include "afxdialogex.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "Target.h"
#include "MapView.h"

#include "DBaseConnector.h"

// KMapFindDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMapFindDlg, KResizeDialogEx)

KMapFindDlg::KMapFindDlg(KMapView* a_pMapView, KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMapFindDlg::IDD, pParent)
{
	m_pTarget  = a_pTarget;
	m_pMapView = a_pMapView;
	m_nType    = -1;
	m_nxID     = -1;
}

KMapFindDlg::~KMapFindDlg()
{
	m_pTarget = NULL;
}

void KMapFindDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KMapFindDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KMapFindDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KMapFindDlg::OnBnClickedCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KMapFindDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &KMapFindDlg::OnBnClickedButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KMapFindDlg::OnReportRowDblClick)
	ON_BN_CLICKED(IDC_CHECK_ALL, &KMapFindDlg::OnBnClickedCheckAll)
END_MESSAGE_MAP()


// KMapFindDlg 메시지 처리기입니다.
BOOL KMapFindDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(TRUE);
    GetDlgItem(IDC_EDIT1)->SetFocus();
    CenterWindow(GetParent());
	
	KReportCtrlSetting::Default(m_wndReport);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReport);

	InitReportHeader();

	SetResize(IDC_STATIC_TOP,    SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,        SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_BOTTOM, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_DEL,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KMapFindDlg::OnBnClickedOk()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);
	if (pButton->GetCheck())
	{
		m_nType = 0;
	}
	else
	{
		m_nType = 1;
	}

	CString strID;
	GetDlgItemText(IDC_EDIT1, strID);
	m_nxID = _ttoi64(strID);
	
	if (m_nType == 0)
	{
		if (!KBulkDBaseNode::ExistNode(m_pTarget, m_nxID))
		{
			AfxMessageBox(_T("노드 검색 결과가 없습니다."));
			return;
		}
	}
	else
	{
		if (!KBulkDBaseLink::ExistLink(m_pTarget, m_nxID))
		{
			AfxMessageBox(_T("링크 검색 결과가 없습니다."));
			return;
		}
	}

	m_pMapView->MapFindDlgNext();
	
	InsertRecentObject();
	InitReportData();
}

void KMapFindDlg::OnBnClickedCancel()
{
    KResizeDialogEx::OnCancel();
}

void KMapFindDlg::GetUserSelectedInfo( int& ar_nType, Integer& ar_nxID )
{
	ar_nType = m_nType;
	ar_nxID  = m_nxID;
}

void KMapFindDlg::InitReportHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("ID"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("ID"));
	}

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Description"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("명칭"));
	}
}

void KMapFindDlg::InitReportData( void )
{
	LoadRecentHistory();
	m_wndReport.ResetContent();

	BOOL bCheckAll(FALSE);
	if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
		bCheckAll = TRUE;
	else
		bCheckAll = FALSE;

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	int nSize = m_vecResult.size();

	for (int i = 0; i < nSize; ++i)
	{
		TFindResult oFindResult = m_vecResult[i];

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		pItem   = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(bCheckAll);
		pItem->SetItemData((DWORD_PTR)oFindResult.TnxSeq);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)oFindResult.TnxObjectID, _T("%.0f")));
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oFindResult.TstrObjectName));
	}

	m_wndReport.Populate();
}

void KMapFindDlg::LoadRecentHistory( void )
{
	m_vecResult.clear();

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);
	
	if (pButton->GetCheck())
	{
		//type node is 0
		LoadRecentNodeHist();
	}
	else
	{
		//type link is 1
		LoadRecentLinkHist();
	}
}

void KMapFindDlg::LoadRecentNodeHist( void )
{
	std::map<Integer, CString> mapNodeName;
	KBulkDBaseNode::Nodes(m_pTarget, mapNodeName);

	std::map<Integer, CString>::iterator fIter, eIter = mapNodeName.end();

	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		if (spDBaseConnection == nullptr)
			ThrowException(_T("Project.db Not Found"));

		CString strQuery = _T("Select seq, obj_id From find_history Where type = 0");

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			TFindResult oFindResult = {};

			oFindResult.TnxSeq         = spResultSet->GetValueInt64(0);
			oFindResult.TnxObjectID    = spResultSet->GetValueInt64(1);

			fIter = mapNodeName.find(oFindResult.TnxObjectID);

			if (fIter == eIter)
				continue;

			oFindResult.TstrObjectName = fIter->second;

			m_vecResult.push_back(oFindResult);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error : KMapFindDlg::LoadRecentNodeHist"));
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KMapFindDlg::LoadRecentLinkHist( void )
{
	std::map<Integer, CString> mapLinkName;
	KBulkDBaseLink::Links(m_pTarget, mapLinkName);

	std::map<Integer, CString>::iterator fIter, eIter = mapLinkName.end();

	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		if (spDBaseConnection == nullptr)
			ThrowException(_T("Project.db Not Found"));

		CString strQuery = _T("Select seq, obj_id From find_history Where type = 1");

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			TFindResult oFindResult = {};

			oFindResult.TnxSeq         = spResultSet->GetValueInt64(0);
			oFindResult.TnxObjectID    = spResultSet->GetValueInt64(1);

			fIter = mapLinkName.find(oFindResult.TnxObjectID);

			if (fIter == eIter)
				continue;

			oFindResult.TstrObjectName = fIter->second;

			m_vecResult.push_back(oFindResult);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error : KMapFindDlg::LoadRecentNodeHist"));
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KMapFindDlg::InsertRecentObject( void )
{
	//중복체크	m_nType, m_nxID
	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		if (spDBaseConnection == nullptr)
			ThrowException(_T("Project.db Not Found"));

		CString strQuery;
		strQuery.Format(_T("Select COUNT(*) From find_history Where Type = %d And Obj_ID = %I64d"),
			m_nType, m_nxID);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			if (spResultSet->GetValueInt(0) > 0)
				return;	//중복 있을 시 Return
		}
			
		strQuery.Format(_T("Insert Into find_history Values((Select IFNULL(max(seq) + 1, 1) From find_history), %d, %I64d)"),
			m_nType, m_nxID);

		if (spDBaseConnection->ExecuteUpdate(strQuery) == SQLITE_ERROR)
			ThrowException(_T("데이터 업데이트에 실패했습니다."));


	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error : KMapFindDlg::InsertRecentObject"))
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KMapFindDlg::Initialize( void )
{
	InitReportData();
	SetDlgItemText(IDC_EDIT1, _T(""));
}

void KMapFindDlg::OnBnClickedRadio( UINT nID )
{
	try
	{
		switch (nID)
		{
		case IDC_RADIO1:
			InitReportData();
			break;
		case IDC_RADIO2:
			InitReportData();
			break;
		default:
			ThrowException(_T("undefined Id..."));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error - KMapFindDlg::OnBnClickedRadio"));
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KMapFindDlg::DeleteSelObject( std::vector<Integer> a_vecObject )
{
	CString strQuery;
	int     nTypeObject; //node = 0, link = 1;

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	
	try
	{
		int nSize = a_vecObject.size();

		if (nSize == 0)
		{
			TxLogDebugWarning();
			return;
		}

		nTypeObject = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == TRUE ? 0 : 1;
		strQuery.Format(_T("Delete From find_history Where Type = %d And Seq = ?"), nTypeObject);
				
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strQuery);
		
		for (int i = 0; i < nSize; ++i)
		{
			spPrepareStatement->BindInt64(1, a_vecObject[i]);
			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}		

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(_T("unexpected error : KMapFindDlg::DeleteSelectObject"));
		throw 1;
	}
}




void KMapFindDlg::OnBnClickedButtonDel()
{
	try
	{
		std::vector<Integer> vecDeleteObj;

		CXTPReportRecords*    pRecords = m_wndReport.GetRecords();
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecord  = pRecords->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pItem = pRecords->GetAt(i)->GetItem(0);

			if (pItem->IsChecked())
				vecDeleteObj.push_back((Integer)pItem->GetItemData());
		}

		DeleteSelObject(vecDeleteObj);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error : KMapFindDlg::OnBnClickedButtonDel"));
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}

	InitReportData();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		AfxMessageBox(_T("삭제되었습니다."));
	}
	else {
		AfxMessageBox(_T("Deleted successfully."));
	}
}

void KMapFindDlg::OnReportRowDblClick( NMHDR* a_pNMHDR, LRESULT* a_pResult )
{
	XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) a_pNMHDR;

	if (!pClickedItem->pRow || !pClickedItem->pColumn)
		return;

	CXTPReportRow* pRow = pClickedItem->pRow;
	
	if (nullptr == pRow)
		return;

	int nRowIndex    = pClickedItem->pRow->GetIndex();
	int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

	if (nColumnIndex == 0)
		return;

	CXTPReportRecord*           pRecord = pRow->GetRecord();
	CXTPReportRecordItemNumber* pNumber =(CXTPReportRecordItemNumber*)pRecord->GetItem(1);
	
	Integer nxObjectID = (Integer)pNumber->GetValue();
	CString strValue;
	strValue.Format(_T("%I64d"), nxObjectID);

	SetDlgItemText(IDC_EDIT1, strValue);
	OnBnClickedOk();
}


void KMapFindDlg::OnBnClickedCheckAll()
{
	try
	{
		BOOL bCheckAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bCheckAll = TRUE;
		else
			bCheckAll = FALSE;

		CXTPReportRecords* pRecords = m_wndReport.GetRecords();
		int nRecordCnt = pRecords->GetCount();

		for (int i= 0; i< nRecordCnt; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);

			CXTPReportRecordItem* pItem   = pRecords->GetAt(i)->GetItem(0);
			pItem->SetChecked(bCheckAll);
		}

		m_wndReport.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
