// TableExpEmmeDefineModesDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableExpEmmeDefineModesDlg.h"
#include "afxdialogex.h"

#include "Target.h"
//^#include "DBaseConnector.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"

#include "RegularExp.h"

// KTableExpEmmeDefineModesDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableExpEmmeDefineModesDlg, KResizeDialogEx)

KTableExpEmmeDefineModesDlg::KTableExpEmmeDefineModesDlg(
	KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableExpEmmeDefineModesDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_bDefined(false)
	, m_strPreValue(_T(""))
{

}

KTableExpEmmeDefineModesDlg::~KTableExpEmmeDefineModesDlg()
{
}

void KTableExpEmmeDefineModesDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReport);
}


BEGIN_MESSAGE_MAP(KTableExpEmmeDefineModesDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, &KTableExpEmmeDefineModesDlg::OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, &KTableExpEmmeDefineModesDlg::OnReportItemValueChanged)
	ON_BN_CLICKED(IDOK,     &KTableExpEmmeDefineModesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableExpEmmeDefineModesDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KTableExpEmmeDefineModesDlg 메시지 처리기입니다.

void KTableExpEmmeDefineModesDlg::GetDefineModes( map<int, CString>& ar_mapDefineMode )
{
	ar_mapDefineMode = m_mapDefineMode;
}

BOOL KTableExpEmmeDefineModesDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		KReportCtrlSetting::Default(m_wndReport, TRUE);

		InitReportHeader();
		UpdateReportData();
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTableExpEmmeDefineModesDlg::GetUsedLinkType( void )
{
	CString strDefaultMode = _T("a");

	m_mapDefineMode.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strQuery = _T("Select link_type From link Group By link_type");

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			m_mapDefineMode.insert(make_pair(spResultSet->GetValueInt(0), strDefaultMode));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KTableExpEmmeDefineModesDlg::GetDefineSystemCode( void )
{
	m_mapLinkMatchingCode.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strQuery;
		strQuery.Format(_T("Select user_code_key, system_code_key From %s Where user_code_group = %d"),
			TABLE_DEFINE_SYSTEM_CODE, USER_LINKCODE_GROUP_KEY);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			m_mapLinkMatchingCode.insert(make_pair(
				spResultSet->GetValueInt(0), spResultSet->GetValueInt(1)));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KTableExpEmmeDefineModesDlg::InitReportHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nColumnIndex(0);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("User_Code_Key"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("타입 코드"));
	}

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("User_Code_Name"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("타입명"));
	}

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("System_Code_Name"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("시스템 타입명"));
	}

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("EMME Modes"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("EMME Modes"));
	}

	m_wndReport.Populate();
}

void KTableExpEmmeDefineModesDlg::UpdateReportData( void )
{
	GetUsedLinkType();
	GetDefineSystemCode();
	
	AutoType beginIter = m_mapDefineMode.begin();
	AutoType endIter   = m_mapDefineMode.end();

	KCodeManager* pCodeMgr       = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup     = pCodeMgr->FindCodeGroup(USER_LINKCODE_GROUP_KEY);
	KCodeGroup*   pSysCodeGroup  = pCodeMgr->FindCodeGroup(SYSTEM_LINKCODE_GROUP_KEY);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	CString               strValue;

	while (beginIter != endIter)
	{
		int     nUserCode = beginIter->first;
		CString strModes  = beginIter->second;
		
		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		//User_Code_Key
		strValue.Format(_T("%d"), nUserCode);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		//User_Code_Name
		strValue = pCodeGroup->SingleCodeValue(nUserCode);
		pItem    = pRecord->AddItem(new CXTPReportRecordItemText(strValue));

		//System_Code_Name
		AutoType findCodeIter = m_mapLinkMatchingCode.find(nUserCode);
		AutoType endCodeIter  = m_mapLinkMatchingCode.end();

		int nSysCode;

		if (findCodeIter != endCodeIter)
		{
			nSysCode = findCodeIter->second;
		}
		else
		{
			nSysCode = HIGHWAY_CODE_KEY;
		}

		strValue = pSysCodeGroup->SingleCodeValue(nSysCode);
		pItem    = pRecord->AddItem(new CXTPReportRecordItemText(strValue));

		//Modes
		pItem    = pRecord->AddItem(new CXTPReportRecordItemText(strModes));

		++beginIter;
	}

	m_wndReport.Populate();
}

void KTableExpEmmeDefineModesDlg::OnBnClickedOk()
{
	map<int, CString> mapDefineMode;

	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int                nCounts  = pRecords->GetCount();

	CXTPReportRecord*         pRecord  = nullptr;
	CXTPReportRecordItemText* pItem = nullptr;
	CString                   strValue;
	
	for (int i = 0; i < nCounts; ++i)
	{
		pRecord = pRecords->GetAt(i);

		//link_type
		strValue = ((CXTPReportRecordItemText*)pRecord->GetItem(0))->GetValue();
		int nLinkType = _ttoi(strValue);

		//modes
		strValue = ((CXTPReportRecordItemText*)pRecord->GetItem(3))->GetValue();

		mapDefineMode.insert(make_pair(nLinkType, strValue));
	}

	m_mapDefineMode = mapDefineMode;

	KResizeDialogEx::OnOK();
}

void KTableExpEmmeDefineModesDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KTableExpEmmeDefineModesDlg::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	int nRecordIndex = pItemNotify->pRow->GetIndex();
	int nColumnIndex = pItemNotify->pColumn->GetIndex();

	if (nColumnIndex != 3)
		return;
	
	CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pItemNotify->pItem;

	m_strPreValue = pItem->GetValue();
}

void KTableExpEmmeDefineModesDlg::OnReportItemValueChanged( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	int nRecordIndex = pItemNotify->pRow->GetIndex();
	int nColumnIndex = pItemNotify->pColumn->GetIndex();

	if (nColumnIndex != 3)
		return;

	CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pItemNotify->pItem;
	CString strValue = pItem->GetValue();

	bool bIsLower = KRegularExp::IsLowerAlphabet(strValue);

	if (!bIsLower)
	{
		pItem->SetValue(m_strPreValue);

		AfxMessageBox(_T("영어 소문자만 입력 가능합니다."));
	}
}

