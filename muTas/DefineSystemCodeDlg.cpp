// DefineSystemCodeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefineSystemCodeDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
#include "CustomReportRecordItem.h"

#include "KExRecordItem.h"
#include "Scenario.h"
#include "Project.h"


enum
{
	_0_COLUMN_SELECT = 0,
	_1_COLUMN_USER_CODE,
	_2_COLUMN_USER_CODE_NAME,
	_3_COLUMN_SYSTEM_CODE_NAME
};

const int TYPE_NODE		= 0;
const int TYPE_LINK		= 1;

const int USE_TYPE		= 0;
const int NOTUES_TYPE	= 1;

// KDefineSystemCodeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefineSystemCodeDlg, KResizeDialogEx)

KDefineSystemCodeDlg::KDefineSystemCodeDlg(KTarget* a_pTarget, bool a_bUseImport, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefineSystemCodeDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_bUseImport(a_bUseImport),
	m_cSeparator(_T(','))
{

}

KDefineSystemCodeDlg::~KDefineSystemCodeDlg()
{
}

void KDefineSystemCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_ReportNode);
	DDX_Control(pDX, IDC_REPORT2, m_ReportLink);
}


BEGIN_MESSAGE_MAP(KDefineSystemCodeDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KDefineSystemCodeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &KDefineSystemCodeDlg::OnBnClickedOk)
	//ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, &KDefineSystemCodeDlg::OnValueChanged)
	//ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT2, &KDefineSystemCodeDlg::OnValueChanged)
	ON_BN_CLICKED(IDC_BUTTON_ADD_NODE, &KDefineSystemCodeDlg::OnBnClickedButtonAddNode)
	ON_BN_CLICKED(IDC_BUTTON_DEL_NODE, &KDefineSystemCodeDlg::OnBnClickedButtonDelNode)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LINK, &KDefineSystemCodeDlg::OnBnClickedButtonAddLink)
	ON_BN_CLICKED(IDC_BUTTON_DEL_LINK, &KDefineSystemCodeDlg::OnBnClickedButtonDelLink)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_NODE, &KDefineSystemCodeDlg::OnBnClickedButtonExportNode)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_NODE, &KDefineSystemCodeDlg::OnBnClickedButtonImportNode)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_LINK, &KDefineSystemCodeDlg::OnBnClickedButtonExportLink)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_LINK, &KDefineSystemCodeDlg::OnBnClickedButtonImportLink)
END_MESSAGE_MAP()


// KDefineSystemCodeDlg 메시지 처리기입니다.


BOOL KDefineSystemCodeDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		ResizeComponent();

		KReportCtrlSetting::Default(m_ReportNode, TRUE);
		KReportCtrlSetting::Default(m_ReportLink, TRUE);

		GetCodeGrouopData();

		GetUserNodeLinkDataByDB();

		GetDefineSystemCodeData();

		InitNodeReportHeader();
		InitLinkReportHeader();

		UpdateNodeReportControl();
		UpdateLinkReportControl();

		CString strOKCaption(_T("OK"));
		CString strCencleCaption(_T("Close"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strOKCaption = _T("확인");
			strCencleCaption = _T("닫기");
		}

		CButton* pOKButton = (CButton*)GetDlgItem(IDOK);
		CButton* pCANCLEButton = (CButton*)GetDlgItem(IDCANCEL);
		if (true == m_bUseImport)
		{
			strOKCaption = _T("OK");
			strCencleCaption = _T("Cancel");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strOKCaption = _T("확인");
				strCencleCaption = _T("취소");
			}
			pCANCLEButton->EnableWindow(FALSE);
		}

		pOKButton->SetWindowTextW(strOKCaption);
		pCANCLEButton->SetWindowTextW(strCencleCaption);

        CString strCaption;
        strCaption.Format(_T("%s"), m_pTarget->Caption());
        SetDlgItemText(IDC_STATIC_TARGETYEAR, strCaption);
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	catch (KExceptionPtr ex)
	{ 
		TxLogDebug(ex->GetErrorMessage());
		return TRUE;
	}
	catch(...)
	{
		TxLogDebugException();
		return TRUE;
	}

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefineSystemCodeDlg::InitNodeReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_ReportNode.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T(""), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_ReportNode.AddColumn(new CXTPReportColumn(_1_COLUMN_USER_CODE, _T("User_Code_Key"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("노드 타입"));
    }

	pColumn = m_ReportNode.AddColumn(new CXTPReportColumn(_2_COLUMN_USER_CODE_NAME, _T("User_Code_Name"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }

	pColumn = m_ReportNode.AddColumn(new CXTPReportColumn(_3_COLUMN_SYSTEM_CODE_NAME, _T("System_Code_Name"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("타입 설정"));
    }
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

	int nCount = m_vecSystemNodeCode.size();

	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);

	for (int i = 0; i < nCount; i++)
	{
		int nCode = m_vecSystemNodeCode[i];
		CString strCaption = pCodeGroup->SingleCodeValue(nCode);
		pEditOptions->AddConstraint(strCaption, nCode);
	}
}

void KDefineSystemCodeDlg::InitLinkReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_ReportLink.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T(""), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_ReportLink.AddColumn(new CXTPReportColumn(_1_COLUMN_USER_CODE, _T("User_Code_Key"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("링크 타입"));
    }

	pColumn = m_ReportLink.AddColumn(new CXTPReportColumn(_2_COLUMN_USER_CODE_NAME, _T("User_Code_Name"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	 if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }

	pColumn = m_ReportLink.AddColumn(new CXTPReportColumn(_3_COLUMN_SYSTEM_CODE_NAME, _T("System_Code_Name"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("타입 설정"));
    }
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

	int nCount = m_vecSystemLinkCode.size();

	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(SYSTEM_LINKCODE_GROUP_KEY);

	for (int i = 0; i < nCount; i++)
	{
		int nCode = m_vecSystemLinkCode[i];
		CString strCaption = pCodeGroup->SingleCodeValue(nCode);
		pEditOptions->AddConstraint(strCaption, nCode);
	}
}


void KDefineSystemCodeDlg::GetCodeGrouopData()
{
	KCodeManager* pCodeMgr		   = m_pTarget->CodeManager();
	KCodeGroup*	  pSNodeCodeGroup  = pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);
	pSNodeCodeGroup->GetCodes(m_vecSystemNodeCode);

	KCodeGroup*	  pSLinkCodeGroup	= pCodeMgr->FindCodeGroup(SYSTEM_LINKCODE_GROUP_KEY);
	pSLinkCodeGroup->GetCodes(m_vecSystemLinkCode);
}

void KDefineSystemCodeDlg::UpdateNodeReportControl()
{
	m_ReportNode.ResetContent(TRUE);

	AutoType iter = m_mapUserNodeCode.begin();
	AutoType end  = m_mapUserNodeCode.end();

	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(USER_NODECODE_GROUP_KEY);

	while(iter != end)
	{
		CXTPReportRecordItem* pItem = nullptr;
		int nUserCode				= iter->first;
		int nUseType				= iter->second;
		CString strCode(_T(""));
		strCode.Format(_T("%d"), nUserCode);
		CString strCaption		= pCodeGroup->SingleCodeValue(nUserCode);
		
		CXTPReportRecord*		pRecord = m_ReportNode.AddRecord(new CXTPReportRecord);
		
		if (USE_TYPE == nUseType ||  0 == nUserCode)
		{
			pItem	= pRecord->AddItem(new CXTPReportRecordItemText(_T("Used")));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem->SetCaption(_T("씀"));
			}

			pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
		}
		else
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));

		pItem->SetEditable(TRUE);

		pItem = pRecord->AddItem(new CCustomReportRecordItem);

		AutoType iterfind	= m_mapNodeMatchingCode.find(nUserCode);
		AutoType iterend	= m_mapNodeMatchingCode.end();

		if (iterend != iterfind)
		{
			int nSystemCode = iterfind->second;
			pItem->SetItemData(nSystemCode);
		}
		else
		{
			pItem->SetItemData(HIGHWAY_NODE_CODE_KEY);		
		}

		if (nUserCode == ZONE_CODE_KEY)
		{
			pItem->SetItemData(ZONE_CODE_KEY);
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}

		++iter;
	}

	m_ReportNode.Populate();
}

void KDefineSystemCodeDlg::UpdateLinkReportControl()
{
	m_ReportLink.ResetContent(TRUE);

	AutoType iter = m_mapUserLinkCode.begin();
	AutoType end  = m_mapUserLinkCode.end();

	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(USER_LINKCODE_GROUP_KEY);

	while(iter != end)


	{
		int nUserCode				= iter->first;
		int nUseType				= iter->second;

		CString strCode(_T(""));
		strCode.Format(_T("%d"), nUserCode);
		CString strCaption		= pCodeGroup->SingleCodeValue(nUserCode);

		CXTPReportRecord*		pRecord = m_ReportLink.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pItem	=  nullptr;
		
		if (USE_TYPE == nUseType || 0 == nUserCode)
		{
			pItem	= pRecord->AddItem(new CXTPReportRecordItemText(_T("Used")));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem->SetCaption(_T("씀"));
			}

			pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
		}
		else
		{
			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));

		pItem = pRecord->AddItem(new CCustomReportRecordItem);

		AutoType iterfind	= m_mapLinkMatchingCode.find(nUserCode);
		AutoType iterend	= m_mapLinkMatchingCode.end();

		if (iterend != iterfind)
		{
			int nSystemCode = iterfind->second;
			pItem->SetItemData(nSystemCode);
		}
		else
		{
			pItem->SetItemData(HIGHWAY_CODE_KEY);
		}

		if (nUserCode == ZONE_CODE_KEY)
		{
			pItem->SetItemData(ZONE_CODE_KEY);
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}

		++iter;
	}
	m_ReportLink.Populate();
}


void KDefineSystemCodeDlg::GetDefineSystemCodeData()
{
	m_mapNodeMatchingCode.clear();
	m_mapLinkMatchingCode.clear();
	
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{
		CString strNodeSQL(_T(""));
		strNodeSQL.Format(_T("Select user_code_key, system_code_key From %s Where user_code_group = %d"),
			TABLE_DEFINE_SYSTEM_CODE, USER_NODECODE_GROUP_KEY);

		KResultSetPtr pNodeResult = spDBaseConnection->ExecuteQuery(strNodeSQL);

		while(pNodeResult->Next())
		{
			int nUserCode	= pNodeResult->GetValueInt(0);
			int nSystemCode = pNodeResult->GetValueInt(1);

			m_mapNodeMatchingCode.insert(std::make_pair(nUserCode, nSystemCode));
		}

		CString strLinkSQL(_T(""));
		strLinkSQL.Format(_T("Select user_code_key, system_code_key From %s Where user_code_group = %d"),
			                                                    TABLE_DEFINE_SYSTEM_CODE, USER_LINKCODE_GROUP_KEY);

		KResultSetPtr pLinkResult = spDBaseConnection->ExecuteQuery(strLinkSQL);

		while(pLinkResult->Next())
		{
			int nUserCode	= pLinkResult->GetValueInt(0);
			int nSystemCode = pLinkResult->GetValueInt(1);

			m_mapLinkMatchingCode.insert(std::make_pair(nUserCode, nSystemCode));
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KDefineSystemCodeDlg::OnBnClickedCancel()
{
	if (true == m_bUseImport)
	{
		AfxMessageBox(_T("네트워크 데이터 불러오기를 수행 한 후 네트워크 타입 설정을 해야 합니다."));
		return;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}


void KDefineSystemCodeDlg::OnBnClickedOk()
{
	int     nRecord, nCodeIndex, nUserCode;
	CString strValue, strErr, strCaption, strColumn;

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	//UserCodeKey가 0이 아닌 경우 Node의 Zone_Centorid
	strColumn = m_ReportNode.GetColumns()->GetAt(3)->GetCaption();
	nRecord   = m_ReportNode.GetRecords()->GetCount();

	for (int i = 0; i < nRecord; ++i)
	{
		pRecord = m_ReportNode.GetRecords()->GetAt(i);
		
		//GetSystemCode
		pItem      = pRecord->GetItem(3);
		nCodeIndex = pItem->GetItemData();
		strCaption = pItem->GetCaption();

		if (nCodeIndex == ZONE_CODE_KEY)
		{
			pItem     = pRecord->GetItem(1);
			strValue  = ((CXTPReportRecordItemText*)pItem)->GetValue();
			nUserCode = _ttoi(strValue);

			if (nUserCode != ZONE_CODE_KEY)
			{
				pItem    = pRecord->GetItem(2);
				strValue = ((CXTPReportRecordItemText*)pItem)->GetValue();

				//strErr.Format(_T("Node Type Define(%s)의 %s은 %s를 선택할 수 없습니다."),
				//	strValue, strColumn, strCaption);
				strErr.Format(_T("해당 노드 타입은 선택한 시스템 타입(%s)으로 설정할 수 없습니다."),
					strCaption);
				
				AfxMessageBox(strErr);
				return;
			}
		}
	}

	//UserCodeKey가 0이 아닌 경우 Link의 Zone_Connector를 선택못하게 함.
	strColumn = m_ReportLink.GetColumns()->GetAt(3)->GetCaption();
	nRecord   = m_ReportLink.GetRecords()->GetCount();

	for (int i = 0; i < nRecord; ++i)
	{
		pRecord = m_ReportLink.GetRecords()->GetAt(i);

		//GetSystemCode
		pItem      = pRecord->GetItem(3);
		nCodeIndex = pItem->GetItemData();
		strCaption = pItem->GetCaption();

		if (nCodeIndex == ZONE_CODE_KEY)
		{
			pItem     = pRecord->GetItem(1);
			strValue  = ((CXTPReportRecordItemText*)pItem)->GetValue();
			nUserCode = _ttoi(strValue);

			if (nUserCode != ZONE_CODE_KEY)
			{
				pItem    = pRecord->GetItem(2);
				strValue = ((CXTPReportRecordItemText*)pItem)->GetValue();

				//strErr.Format(_T("Link Type Define(%s)의 %s은 %s를 선택할 수 없습니다."),
				//	strValue, strColumn, strCaption);
				strErr.Format(_T("해당 링크 타입은 선택한 시스템 타입(%s)으로 설정할 수 없습니다."),
					strCaption);

				AfxMessageBox(strErr);
				return;
			}
		}
	}


	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();
	try
	{

		CString strDeleteSQL = _T("");
		strDeleteSQL.Format(_T("delete from %s"), TABLE_DEFINE_SYSTEM_CODE);
		spDBaseConnection->ExecuteUpdate(strDeleteSQL);

		UpdateNodeResultCode(spDBaseConnection);
		UpdateLinkResultCode(spDBaseConnection);

		spDBaseConnection->Commit();

        KCodeManager* pCodeManager = m_pTarget->CodeManager();
        pCodeManager->LoadCodeGroups();

		KIOTable* pLinkTable = m_pTarget->Tables()->FindTable(TABLE_LINK);
		pLinkTable->Notify();
		
		KIOTable* pNodeTable = m_pTarget->Tables()->FindTable(TABLE_NODE);
		pNodeTable->Notify();

		QBicMessageBox::MessageBox(_T("적용되었습니다."), _T("Information"));
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		return;
	}

	KResizeDialogEx::OnOK();
}


void KDefineSystemCodeDlg::UpdateNodeResultCode(KDBaseConPtr spDBaseConnection)
{
	try
	{
		CXTPReportRecords*           pRecords      = m_ReportNode.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportColumn*			 pColumn	   = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;

		std::map<int, int> mapResultMatchingCode;
		std::map<int, CString> mapResultDefineCode;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_1_COLUMN_USER_CODE);
			pColumn = m_ReportNode.GetColumns()->GetAt(_1_COLUMN_USER_CODE);
			
            CString strUserCodeType = pItem->GetCaption(pColumn);
			int nUserCode = _ttoi(strUserCodeType);
            
            if (false == QBicStringChecker::IsUnSignedInteger(strUserCodeType))
            {
                CString strMsg;
                strMsg.Format(_T("User Code Key : '%s' 의 값이 잘못되었습니다. \n0보다 큰 값을 입력해 주세요"), strUserCodeType);
                ThrowException(strMsg);
            }
 
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_USER_CODE_NAME);
			CString strUserCodeName = pItemText->GetValue();

			pItem = pRecord->GetItem(_3_COLUMN_SYSTEM_CODE_NAME);
			int nSystemCode = pItem->GetItemData();
            
			if (strUserCodeName == _T("") || strUserCodeName == _T("'"))
			{
				CString strMsg;
				strMsg.Format(_T("User Code Key : '%s' 의 이름이 없습니다."), strUserCodeType);
				ThrowException(strMsg);
			}
            else if (strUserCodeName.Find(_T(" ")) != -1)
            {
                CString strMsg;
                strMsg.Format(_T("User Code Key : '%s' 이름에 공백이 포함되어 있습니다."), strUserCodeType);
                ThrowException(strMsg);
            }

			if (nSystemCode < 0)
			{
				CString strMsg;
				strMsg.Format(_T("User Code Key : '%s' 의 매칭되는 코드가 없습니다."), strUserCodeType);
				ThrowException(strMsg);
			}

			AutoType iterfind = mapResultMatchingCode.find(nUserCode);
			AutoType iterend  = mapResultMatchingCode.end();
			if (iterfind == iterend)
			{
				mapResultMatchingCode.insert(std::make_pair(nUserCode, nSystemCode));
				mapResultDefineCode.insert(std::make_pair(nUserCode, strUserCodeName));
			}
			else
			{
				CString strMsg;
				strMsg.Format(_T("> User Code Key : '%d' 값이 중복되어 사용되고 있습니다."), nUserCode);
				ThrowException(strMsg);
			}
		}
	
		UpdateDB2SystemMatchingCode(mapResultMatchingCode, mapResultDefineCode, TYPE_NODE, spDBaseConnection);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KDefineSystemCodeDlg::UpdateLinkResultCode(KDBaseConPtr spDBaseConnection)
{
	try
	{
		CXTPReportRecords*           pRecords      = m_ReportLink.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportColumn*			 pColumn	   = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;

		std::map<int, int> mapResultMatchingCode;
		std::map<int, CString> mapResultDefineCode;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_1_COLUMN_USER_CODE);
			pColumn = m_ReportNode.GetColumns()->GetAt(_2_COLUMN_USER_CODE_NAME);
			
            CString strUserCodeType = pItem->GetCaption(pColumn);
			int nUserCode = _ttoi(strUserCodeType);

            if (false == QBicStringChecker::IsUnSignedInteger(strUserCodeType))
            {
                CString strMsg;
                strMsg.Format(_T("User Code Key : '%s' 의 값이 잘못되었습니다. \n0보다 큰 값을 입력해 주세요"), strUserCodeType);
                ThrowException(strMsg);
            }

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_USER_CODE_NAME);
			CString strUserCodeName = pItemText->GetValue();

			pItem = pRecord->GetItem(_3_COLUMN_SYSTEM_CODE_NAME);
			int nSystemCode = pItem->GetItemData();

			if (strUserCodeName == _T("") || strUserCodeName == _T("'"))
			{
				CString strMsg;
				strMsg.Format(_T("User Code Key : '%s' 의 이름이 없습니다."), strUserCodeType);
				ThrowException(strMsg);
			}
            else if (strUserCodeName.Find(_T(" ")) != -1)
            {
                CString strMsg;
                strMsg.Format(_T("User Code Key : '%s' 이름에 공백이 포함되어 있습니다."), strUserCodeType);
                ThrowException(strMsg);
            }

			if (nSystemCode < 0)
			{
				CString strMsg;
				strMsg.Format(_T("User Code Key: '%s' 의 매칭되는 코드가 없습니다."), strUserCodeType);
				ThrowException(strMsg);
			}

			AutoType iterfind = mapResultMatchingCode.find(nUserCode);
			AutoType iterend  = mapResultMatchingCode.end();
			if (iterfind == iterend)
			{
				mapResultMatchingCode.insert(std::make_pair(nUserCode, nSystemCode));
				mapResultDefineCode.insert(std::make_pair(nUserCode, strUserCodeName));
			}
			else
			{
				CString strMsg;
				strMsg.Format(_T("User Code Key : '%d' 값이 중복되어 사용되고 있습니다."), nUserCode);
				ThrowException(strMsg);
			}
		}

		UpdateDB2SystemMatchingCode(mapResultMatchingCode, mapResultDefineCode, TYPE_LINK, spDBaseConnection);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}	
}


void KDefineSystemCodeDlg::UpdateDB2SystemMatchingCode(std::map<int, int> a_mapResult, std::map<int, CString> a_mapDefineCode, int a_nType, KDBaseConPtr spDBaseConnection)
{
	try
	{
		CString strInsetSQL = _T("");
		
		AutoType iter	= a_mapResult.begin();
		AutoType end	= a_mapResult.end();

		while(iter != end )
		{
			int nUserCode	= iter->first;
			int nSystemCode = iter->second;
			if (TYPE_NODE == a_nType)
			{
				strInsetSQL.Format(_T("Insert Into %s Values (%d , %d, %d, %d) "), 
					TABLE_DEFINE_SYSTEM_CODE, USER_NODECODE_GROUP_KEY, nUserCode, SYSTEM_NODECODE_GROUP_KEY, nSystemCode );

				spDBaseConnection->ExecuteUpdate(strInsetSQL);
			}
			else
			{
				strInsetSQL.Format(_T("Insert Into %s Values (%d , %d, %d, %d) "), 
					TABLE_DEFINE_SYSTEM_CODE, USER_LINKCODE_GROUP_KEY, nUserCode, SYSTEM_LINKCODE_GROUP_KEY, nSystemCode );

				spDBaseConnection->ExecuteUpdate(strInsetSQL);
			}

			++iter;
		}

		CString strSQL(_T(""));
		int nCodeGroupKey(0);

		if (TYPE_NODE == a_nType)
		{
			nCodeGroupKey = USER_NODECODE_GROUP_KEY;		
		}
		else
		{
			nCodeGroupKey = USER_LINKCODE_GROUP_KEY;
		}

		strSQL.Format( _T(" DELETE FROM %s ")
                       _T(" WHERE kcode_group_key = %d "), 
                       TABLE_KCODE_DEFINE, nCodeGroupKey
                     );

		spDBaseConnection->ExecuteUpdate(strSQL);

		AutoType iterDefine = a_mapDefineCode.begin();
		AutoType endDefine	= a_mapDefineCode.end();

		while(iterDefine != endDefine)
		{
			int nCode			= iterDefine->first;
			CString strCodeName = iterDefine->second;
			strSQL.Format(
				_T(" INSERT INTO %s(kcode_group_key, kcode_key, kcode_display) ")
				_T(" VALUES(%d, %d, '%s') "),
				TABLE_KCODE_DEFINE, nCodeGroupKey, nCode, strCodeName
				);
			spDBaseConnection->ExecuteUpdate(strSQL);
			iterDefine++;
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KDefineSystemCodeDlg::ResizeComponent()
{
	SetResize(IDC_STATIC_TARGETYEAR	, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC1	, SZ_TOP_LEFT, SZ_TOP_LEFT);
	
	SetResize(IDC_REPORT1			, CXTPResizePoint(0, 0),     CXTPResizePoint(1, 0.4));
	SetResize(IDC_BUTTON_ADD_NODE	, CXTPResizePoint(0, 0.4),     CXTPResizePoint(0, 0.4));
	SetResize(IDC_BUTTON_DEL_NODE	, CXTPResizePoint(0, 0.4),     CXTPResizePoint(0, 0.4));
	SetResize(IDC_STATIC2			, CXTPResizePoint(0, 0.4),     CXTPResizePoint(1, 0.4));

	SetResize(IDC_STATIC3			, CXTPResizePoint(0, 0.4),     CXTPResizePoint(0, 0.4));
	SetResize(IDC_REPORT2			, CXTPResizePoint(0, 0.4),   CXTPResizePoint(1, 1));
	SetResize(IDC_STATIC4			, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_BUTTON_ADD_LINK	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DEL_LINK	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDOK					, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL				, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_EXPORT_NODE, CXTPResizePoint(1, 0.4),     CXTPResizePoint(1, 0.4));
	SetResize(IDC_BUTTON_IMPORT_NODE, CXTPResizePoint(1, 0.4),     CXTPResizePoint(1, 0.4));

	SetResize(IDC_BUTTON_EXPORT_LINK, SZ_BOTTOM_RIGHT,             SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_IMPORT_LINK, SZ_BOTTOM_RIGHT,             SZ_BOTTOM_RIGHT);
}


void KDefineSystemCodeDlg::GetUserNodeLinkDataByDB()
{
	m_mapUserNodeCode.clear();
	m_mapUserLinkCode.clear();
    
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strNodeSQL(_T(""));
		strNodeSQL.Format(_T("Select node_type From node Group By node_type"));

		KResultSetPtr pNodeResult = spDBaseConnection->ExecuteQuery(strNodeSQL);

		while(pNodeResult->Next())
		{
			int nNodeType = pNodeResult->GetValueInt(0);
			m_mapUserNodeCode.insert(std::make_pair(nNodeType, USE_TYPE));
		}

		std::map<int, CString> mapNodeCodeGroup;
		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(USER_NODECODE_GROUP_KEY);
		pCodeGroup->GetCodes(mapNodeCodeGroup);

		AutoType iterNCodeGrop = mapNodeCodeGroup.begin();
		AutoType endNCodeGrop  = mapNodeCodeGroup.end();
		while(iterNCodeGrop != endNCodeGrop)
		{
			int nNodeCode = iterNCodeGrop->first;

			AutoType findUserCode = m_mapUserNodeCode.find(nNodeCode);
			AutoType endUserCode   =m_mapUserNodeCode.end();

			if (findUserCode == endUserCode)
			{
				m_mapUserNodeCode.insert(std::make_pair(nNodeCode, NOTUES_TYPE));
			}
			iterNCodeGrop++;
		}


		CString strLinkSQL(_T(""));
		strLinkSQL.Format(_T("Select link_type From link Group By link_type"));

		KResultSetPtr pLinkResult = spDBaseConnection->ExecuteQuery(strLinkSQL);

		while(pLinkResult->Next())
		{
			int nlinkType = pLinkResult->GetValueInt(0);
			m_mapUserLinkCode.insert(std::make_pair(nlinkType, USE_TYPE));
		}

		std::map<int, CString> mapLinkCodeGroup;
		pCodeMgr   = m_pTarget->CodeManager();
		pCodeGroup = pCodeMgr->FindCodeGroup(USER_LINKCODE_GROUP_KEY);
		pCodeGroup->GetCodes(mapLinkCodeGroup);

		AutoType iterLCodeGrop = mapLinkCodeGroup.begin();
		AutoType endLCodeGrop  = mapLinkCodeGroup.end();
		while(iterLCodeGrop != endLCodeGrop)
		{
			int nLinkCode = iterLCodeGrop->first;

			AutoType findLUserCode = m_mapUserLinkCode.find(nLinkCode);
			AutoType endLUserCode   =m_mapUserLinkCode.end();

			if (findLUserCode == endLUserCode)
			{
				m_mapUserLinkCode.insert(std::make_pair(nLinkCode, NOTUES_TYPE));
			}
			iterLCodeGrop++;
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


void KDefineSystemCodeDlg::OnBnClickedButtonAddNode()
{
	int nTotalRowCount = m_ReportNode.GetRows()->GetCount();

	CXTPReportRecord*     pRecord = m_ReportNode.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem   = NULL;

	pItem = pRecord->AddItem(new CXTPReportRecordItem());
	pItem->HasCheckbox(TRUE);
	pItem->SetAlignment(xtpColumnIconCenter);

	int nMaxCode(0);
	GetMaxCodeValue(nMaxCode, TYPE_NODE);

	nMaxCode = nMaxCode + 1;
	CString strCode(_T(""));
	strCode.Format(_T("%d"), nMaxCode);
	
	pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
	pItem->SetEditable(TRUE);

	pItem = pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetItemData(HIGHWAY_CODE_KEY);

	m_ReportNode.Populate();

	nTotalRowCount = m_ReportNode.GetRows()->GetCount();
	CXTPReportRow* pRow = m_ReportNode.GetRows()->GetAt(nTotalRowCount - 1);
	//pRow->SetSelected(TRUE);


	//추가된 컬럼에 Focus주기
	XTP_REPORTRECORDITEM_ARGS args(&m_ReportNode, pRow, m_ReportNode.GetColumns()->GetAt(_2_COLUMN_USER_CODE_NAME));
	m_ReportNode.EditItem(&args);
}


void KDefineSystemCodeDlg::OnBnClickedButtonDelNode()
{

	std::vector<CXTPReportRecord*> vecRecrod;
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItem*        pExtItemCheck = nullptr;
	
	try
	{
		pRecords			= m_ReportNode.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pExtItemCheck    = pRecord->GetItem(_0_COLUMN_SELECT);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
			{
				vecRecrod.push_back(pRecord);
			}
		}

		int nvecCount = vecRecrod.size();
		for (int ivec = 0; ivec < nvecCount; ivec++)
		{
			pRecord = vecRecrod[ivec];
			m_ReportNode.GetRecords()->RemoveRecord(pRecord);
			m_ReportNode.Populate();
			pRecord = nullptr;
		}
		
		if (nvecCount == 0)
		{
			AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
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


void KDefineSystemCodeDlg::OnBnClickedButtonAddLink()
{
	int nTotalRowCount = m_ReportLink.GetRows()->GetCount();

	CXTPReportRecord*     pRecord = m_ReportLink.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem   = NULL;

	pItem = pRecord->AddItem(new KExRecordItemCheck());
	pItem->HasCheckbox(TRUE);
	pItem->SetAlignment(xtpColumnIconCenter);

	int nMaxCode(0);
	GetMaxCodeValue(nMaxCode, TYPE_LINK);

	nMaxCode = nMaxCode + 1;
	CString strCode(_T(""));
	strCode.Format(_T("%d"), nMaxCode);

	pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
	pItem->SetEditable(TRUE);

	pItem = pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetItemData(HIGHWAY_CODE_KEY);

	m_ReportLink.Populate();

	nTotalRowCount = m_ReportLink.GetRows()->GetCount();
	CXTPReportRow* pRow = m_ReportLink.GetRows()->GetAt(nTotalRowCount - 1);
	//pRow->SetSelected(TRUE);


	//추가된 컬럼에 Focus주기
	XTP_REPORTRECORDITEM_ARGS args(&m_ReportLink, pRow, m_ReportNode.GetColumns()->GetAt(1));
	m_ReportLink.EditItem(&args);
}


void KDefineSystemCodeDlg::OnBnClickedButtonDelLink()
{
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItem*        pExtItemCheck = nullptr;
	int nCount(0);
	try
	{
		std::vector<CXTPReportRecord*> vecRecrod;
		pRecords			= m_ReportLink.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pExtItemCheck    = pRecord->GetItem(_0_COLUMN_SELECT);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
			{
				vecRecrod.push_back(pRecord);
			}
		}

		int nvecCount = vecRecrod.size();
		for (int ivec = 0; ivec < nvecCount; ivec++)
		{
			pRecord = vecRecrod[ivec];
			m_ReportLink.GetRecords()->RemoveRecord(pRecord);
			m_ReportLink.Populate();
			pRecord = nullptr;
		}

		if (nvecCount == 0)
		{
			AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
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


void KDefineSystemCodeDlg::GetMaxCodeValue( int& a_nMaxCode, int a_nType )
{
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItemText*    pItemText	   = nullptr;

	try
	{
		if (TYPE_NODE == a_nType)
		{
			pRecords      = m_ReportNode.GetRecords();
		}
		else
		{
			pRecords      = m_ReportLink.GetRecords();
		}
		
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemText   = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_USER_CODE);
			CString strValue = pItemText->GetValue();
			int nUserCode = _ttoi(strValue);

			if (nUserCode > a_nMaxCode)
			{
				a_nMaxCode = nUserCode;
			}
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

void KDefineSystemCodeDlg::DeleteKCodeGroupKey( int a_nCodeGroup )
{
	try
	{

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

void KDefineSystemCodeDlg::OnBnClickedButtonExportNode()
{
	m_strFile = _T("");

	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle = _T("Export Data");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strExportFile = fileDlg.GetPathName();
		strExportFile.Replace(_T(".txt"), _T(""));
		m_strFile.Format(_T("%s.txt"), strExportFile);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ExportNodeThreadCaller, this, false, nLang);

		if (m_bThreadOK)
			AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
		else
			AfxMessageBox(m_strErrMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

unsigned __stdcall KDefineSystemCodeDlg::ExportNodeThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefineSystemCodeDlg* pDefineSystemCodeDlg = (KDefineSystemCodeDlg*)pParameter->GetParameter();
	pDefineSystemCodeDlg->ExportNode();

	return 0;
}

void KDefineSystemCodeDlg::ExportNode( void )
{
	m_bThreadOK = false;

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(m_strFile, nOpenMode);

	try
	{
		CString strLine;

		//WriteHeader
		strLine = _T("###USER_CODE_KEY, USER_CODE_NAME, SYSTEM_CODE_ID\r\n");
		outFile.WriteString(strLine);

		//WriteData
		CXTPReportRecords* pRecords = m_ReportNode.GetRecords();
		int                nRecord  = pRecords->GetCount();

		int     nUserCodeKey, nSystemCodeKey;
		CString strUserCodeName;

		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord*           pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem*       pItem   = nullptr;
			CXTPReportRecordItemNumber* pNumber = nullptr;

			pItem = pRecord->GetItem(1);
			nUserCodeKey = _ttoi(pItem->GetCaption());

			pItem = pRecord->GetItem(2);
			strUserCodeName = pItem->GetCaption();

			pItem = pRecord->GetItem(3);
			nSystemCodeKey = (int)pItem->GetItemData();
			
			strLine.Format(_T("%d, '%s', %d\r\n")
				, nUserCodeKey, strUserCodeName, nSystemCodeKey);

			outFile.WriteString(strLine);
		}		

		outFile.Close();
		m_bThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		TxExceptionPrint(ex);
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		outFile.Close();
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KDefineSystemCodeDlg::OnBnClickedButtonImportNode()
{
	try
	{
		TCHAR       szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		CFileDialog filedlg( TRUE, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter );

		KScenario* pScenario   = (KScenario*)m_pTarget->GetParentObject();
		KProject*  pProject    = (KProject*)pScenario->GetParentObject();
		CString    strLocation = pProject->GetLocation();

		if (!strLocation.IsEmpty())
			filedlg.m_ofn.lpstrInitialDir = strLocation;

		if( filedlg.DoModal() != IDOK )
			return;

		m_strFile = filedlg.GetPathName();


		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ImportNodeThreadCaller, this, false, nLang);

		if (m_bThreadOK)
		{
			UpdateImportNodeData();
			AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));
		}
		else
		{
			AfxMessageBox(m_strErrMsg);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

unsigned __stdcall KDefineSystemCodeDlg::ImportNodeThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefineSystemCodeDlg* pDefineSystemCodeDlg = (KDefineSystemCodeDlg*)pParameter->GetParameter();
	pDefineSystemCodeDlg->ImportNode();

	return 0;
}

void KDefineSystemCodeDlg::ImportNode( void )
{
	m_setAllUserCodeKey.clear();
	m_mapImportData.clear();

	m_bThreadOK = false;

	UINT         nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT         nCodePage = 949;
	CStdioFileEx csvFile;

	try
	{
		CString strLine, strTemp, strErr;

		csvFile.SetCodePage(nCodePage);

		if (csvFile.Open(m_strFile, nOpenMode) == FALSE)
			return;

		int nLine(0);

		while (csvFile.ReadString(strLine))
		{
			++nLine;

			strLine.Trim();

			if (strLine.IsEmpty())
				continue;

			strTemp = strLine.Left(1);

			if (strTemp.CompareNoCase(_T("#")) == 0)
				continue;

			strLine.Replace(_T(" "),  _T(""));
			strLine.Replace(_T("\""), _T(""));
			strLine.Replace(_T("'"),  _T(""));

			CSVRow row;
			ParseCSVLineString(strLine, row);

			if (row.size() != 3)
			{
				strErr.Format(_T("Line : %d 데이터 개수 불일치"), nLine);
				ThrowException(strErr);
			}
			TDefineNetwork oDefineNetwork = {};
			oDefineNetwork.nUserCodeKey    = _ttoi(row[0]);
			oDefineNetwork.strUserCodeName = row[1];
			oDefineNetwork.nSystemCodeKey  = _ttoi(row[2]);
			
			if (!VerifyImportNodeData(oDefineNetwork, strTemp))
			{
				strErr.Format(_T("Line : %d %s"), nLine, strTemp);
				ThrowException(strErr);
			}

			if (m_mapImportData.find(oDefineNetwork.nUserCodeKey) != m_mapImportData.end())
			{
				strErr.Format(_T("Line : %d 중복된 User_Code_Key가 있습니다."), nLine);
				ThrowException(strErr);
			}
						
			m_mapImportData.insert(std::make_pair(oDefineNetwork.nUserCodeKey, oDefineNetwork));
			m_setAllUserCodeKey.insert(oDefineNetwork.nUserCodeKey);
		}

		csvFile.Close();
		m_bThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		csvFile.Close();
		TxExceptionPrint(ex);
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		csvFile.Close();
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}


void KDefineSystemCodeDlg::OnBnClickedButtonExportLink()
{
	m_strFile = _T("");

	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle = _T("Export Data");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strExportFile = fileDlg.GetPathName();
		strExportFile.Replace(_T(".txt"), _T(""));
		m_strFile.Format(_T("%s.txt"), strExportFile);


		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ExportLinkThreadCaller, this, false, nLang);

		if (m_bThreadOK)
			AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
		else
			AfxMessageBox(m_strErrMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

unsigned __stdcall KDefineSystemCodeDlg::ExportLinkThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefineSystemCodeDlg* pDefineSystemCodeDlg = (KDefineSystemCodeDlg*)pParameter->GetParameter();
	pDefineSystemCodeDlg->ExportLink();

	return 0;
}

void KDefineSystemCodeDlg::ExportLink( void )
{
	m_bThreadOK = false;

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(m_strFile, nOpenMode);

	try
	{
		CString strLine;

		//WriteHeader
		strLine = _T("###USER_CODE_KEY, USER_CODE_NAME, SYSTEM_CODE_ID\r\n");
		outFile.WriteString(strLine);

		//WriteData
		CXTPReportRecords* pRecords = m_ReportLink.GetRecords();
		int                nRecord  = pRecords->GetCount();

		int     nUserCodeKey, nSystemCodeKey;
		CString strUserCodeName;

		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord*           pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem*       pItem   = nullptr;
			CXTPReportRecordItemNumber* pNumber = nullptr;

			pItem = pRecord->GetItem(1);
			nUserCodeKey = _ttoi(pItem->GetCaption());

			pItem = pRecord->GetItem(2);
			strUserCodeName = pItem->GetCaption();

			pItem = pRecord->GetItem(3);
			nSystemCodeKey = (int)pItem->GetItemData();

			strLine.Format(_T("%d, '%s', %d\r\n")
				, nUserCodeKey, strUserCodeName, nSystemCodeKey);

			outFile.WriteString(strLine);
		}		

		outFile.Close();
		m_bThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		TxExceptionPrint(ex);
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		outFile.Close();
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KDefineSystemCodeDlg::OnBnClickedButtonImportLink()
{
	try
	{
		TCHAR       szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		CFileDialog filedlg( TRUE, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter );

		KScenario* pScenario   = (KScenario*)m_pTarget->GetParentObject();
		KProject*  pProject    = (KProject*)pScenario->GetParentObject();
		CString    strLocation = pProject->GetLocation();

		if (!strLocation.IsEmpty())
			filedlg.m_ofn.lpstrInitialDir = strLocation;

		if( filedlg.DoModal() != IDOK )
			return;

		m_strFile = filedlg.GetPathName();


		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ImportLinkThreadCaller, this, false, nLang);

		if (m_bThreadOK)
		{
			UpdateImportLinkData();
			AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));
		}
		else
		{
			AfxMessageBox(m_strErrMsg);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

unsigned __stdcall KDefineSystemCodeDlg::ImportLinkThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefineSystemCodeDlg* pDefineSystemCodeDlg = (KDefineSystemCodeDlg*)pParameter->GetParameter();
	pDefineSystemCodeDlg->ImportLink();

	return 0;
}

void KDefineSystemCodeDlg::ImportLink( void )
{
	m_setAllUserCodeKey.clear();
	m_mapImportData.clear();

	m_bThreadOK = false;

	UINT         nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT         nCodePage = 949;
	CStdioFileEx csvFile;

	try
	{
		CString strLine, strTemp, strErr;

		csvFile.SetCodePage(nCodePage);

		if (csvFile.Open(m_strFile, nOpenMode) == FALSE)
			return;

		int nLine(0);

		while (csvFile.ReadString(strLine))
		{
			++nLine;

			strLine.Trim();

			if (strLine.IsEmpty())
				continue;

			strTemp = strLine.Left(1);

			if (strTemp.CompareNoCase(_T("#")) == 0)
				continue;

			strLine.Replace(_T(" "),  _T(""));
			strLine.Replace(_T("\""), _T(""));
			strLine.Replace(_T("'"),  _T(""));

			CSVRow row;
			ParseCSVLineString(strLine, row);

			if (row.size() != 3)
			{
				strErr.Format(_T("Line : %d 데이터 개수 불일치"), nLine);
				ThrowException(strErr);
			}
			TDefineNetwork oDefineNetwork = {};
			oDefineNetwork.nUserCodeKey    = _ttoi(row[0]);
			oDefineNetwork.strUserCodeName = row[1];
			oDefineNetwork.nSystemCodeKey  = _ttoi(row[2]);

			if (!VerifyImportLinkData(oDefineNetwork, strTemp))
			{
				strErr.Format(_T("Line : %d %s"), nLine, strTemp);
				ThrowException(strErr);
			}

			if (m_mapImportData.find(oDefineNetwork.nUserCodeKey) != m_mapImportData.end())
			{
				strErr.Format(_T("Line : %d 중복된 User_Code_Key가 있습니다."), nLine);
				ThrowException(strErr);
			}

			m_mapImportData.insert(std::make_pair(oDefineNetwork.nUserCodeKey, oDefineNetwork));
			m_setAllUserCodeKey.insert(oDefineNetwork.nUserCodeKey);
		}

		csvFile.Close();
		m_bThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		csvFile.Close();
		TxExceptionPrint(ex);
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		csvFile.Close();
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KDefineSystemCodeDlg::ParseCSVLineString( CString strLine, CSVRow& row )
{
	bool bQuote = false;
	int nSub = 0;
	CString strRow, strQuote;

	while( AfxExtractSubString( strRow, strLine, nSub++, m_cSeparator ) == TRUE )
	{
		if( true == bQuote )
		{
			strQuote += strRow;
			if( strRow.GetAt( strRow.GetLength() - 1 ) == _T('"') )
			{
				strQuote.Remove( _T('"') );
				strRow = strQuote;
				bQuote = false;
				strQuote = _T("");
				row.push_back(strRow);
			}
		}
		else
		{
			TCHAR chFirst, chLast;
			if( strRow.IsEmpty() == TRUE )
			{
				row.push_back( strRow );
			}
			else if( strRow.GetLength() == 1 )
			{
				chFirst = strRow.GetAt( 0 );
				if( chFirst == _T('"') )
				{
					bQuote = true;
				}
				else
				{
					row.push_back(strRow);
				}
			}
			else
			{
				chFirst = strRow.GetAt( 0 );
				chLast = strRow.GetAt( strRow.GetLength() - 1 );

				if( (chFirst == _T('"')) && (chLast == _T('"')) )
				{
					strRow.Remove( _T('"') );
					row.push_back( strRow );
				}
				else if( chFirst == _T('"') )
				{
					bQuote = true;
					strQuote = strRow;
				}
				else
				{
					row.push_back( strRow );
				}
			}
		}
	}
}


void KDefineSystemCodeDlg::UpdateImportNodeData( void )
{
	m_setUsedUserCodeKey.clear();

	try
	{
		m_mapReportData.clear();

		int nRecord = m_ReportNode.GetRecords()->GetCount();

		for (int i = 0;i < nRecord; ++i)
		{
			CXTPReportRecord*     pRecord = m_ReportNode.GetRecords()->GetAt(i);
			CXTPReportRecordItem* pItem   = nullptr;

			TDefineNetwork oDefineNetwork = {};
			
			pItem = pRecord->GetItem(1);
			oDefineNetwork.nUserCodeKey = _ttoi(pItem->GetCaption());

			pItem = pRecord->GetItem(2);
			oDefineNetwork.strUserCodeName = pItem->GetCaption();

			pItem = pRecord->GetItem(3);
			oDefineNetwork.nSystemCodeKey = (int)pItem->GetItemData();

			m_mapReportData.insert(std::make_pair(oDefineNetwork.nUserCodeKey, oDefineNetwork));
			m_setAllUserCodeKey.insert(oDefineNetwork.nUserCodeKey);

			pItem = pRecord->GetItem(0);

			if (!pItem->GetHasCheckbox())
				m_setUsedUserCodeKey.insert(oDefineNetwork.nUserCodeKey);
		}

		//reportctrl redraw
		m_ReportNode.ResetContent();

		CXTPReportRecord* pRecord = nullptr;
		int               nUserCodeKey;

		std::set<int>::iterator bSetIter, eSetIter = m_setAllUserCodeKey.end();
		std::map<int, TDefineNetwork>::iterator fImportIter, fReportIter;

		for (bSetIter = m_setAllUserCodeKey.begin(); bSetIter != eSetIter; ++bSetIter)
		{
			nUserCodeKey = *bSetIter;
			pRecord      = m_ReportNode.AddRecord(new CXTPReportRecord);

			fImportIter = m_mapImportData.find(nUserCodeKey);
			fReportIter = m_mapReportData.find(nUserCodeKey);

			if (fImportIter != m_mapImportData.end())
			{
				AddReportData(pRecord, fImportIter->second);
			}
			else if (fReportIter != m_mapReportData.end())
			{
				AddReportData(pRecord, fReportIter->second);
			}
			else
			{
				ThrowException(_T("업데이트에 실패했습니다."));
			}
		}

		m_ReportNode.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		m_ReportNode.ResetContent();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		m_ReportNode.ResetContent();
		ThrowException(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KDefineSystemCodeDlg::UpdateImportLinkData( void )
{
	m_setUsedUserCodeKey.clear();

	try
	{
		m_mapReportData.clear();

		int nRecord = m_ReportLink.GetRecords()->GetCount();

		for (int i = 0;i < nRecord; ++i)
		{
			CXTPReportRecord*     pRecord = m_ReportLink.GetRecords()->GetAt(i);
			CXTPReportRecordItem* pItem   = nullptr;

			TDefineNetwork oDefineNetwork = {};
			
			pItem = pRecord->GetItem(1);
			oDefineNetwork.nUserCodeKey = _ttoi(pItem->GetCaption());

			pItem = pRecord->GetItem(2);
			oDefineNetwork.strUserCodeName = pItem->GetCaption();

			pItem = pRecord->GetItem(3);
			oDefineNetwork.nSystemCodeKey = (int)pItem->GetItemData();

			m_mapReportData.insert(std::make_pair(oDefineNetwork.nUserCodeKey, oDefineNetwork));
			m_setAllUserCodeKey.insert(oDefineNetwork.nUserCodeKey);

			pItem = pRecord->GetItem(0);

			if (!pItem->GetHasCheckbox())
				m_setUsedUserCodeKey.insert(oDefineNetwork.nUserCodeKey);
		}

		//reportctrl redraw
		m_ReportLink.ResetContent();

		CXTPReportRecord* pRecord = nullptr;
		int               nUserCodeKey;

		std::set<int>::iterator bSetIter, eSetIter = m_setAllUserCodeKey.end();
		std::map<int, TDefineNetwork>::iterator fImportIter, fReportIter;

		for (bSetIter = m_setAllUserCodeKey.begin(); bSetIter != eSetIter; ++bSetIter)
		{
			nUserCodeKey = *bSetIter;
			pRecord      = m_ReportLink.AddRecord(new CXTPReportRecord);

			fImportIter = m_mapImportData.find(nUserCodeKey);
			fReportIter = m_mapReportData.find(nUserCodeKey);

			if (fImportIter != m_mapImportData.end())
			{
				AddReportData(pRecord, fImportIter->second);
			}
			else if (fReportIter != m_mapReportData.end())
			{
				AddReportData(pRecord, fReportIter->second);
			}
			else
			{
				ThrowException(_T("업데이트에 실패했습니다."));
			}
		}

		m_ReportLink.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		m_ReportLink.ResetContent();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		m_ReportLink.ResetContent();
		ThrowException(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KDefineSystemCodeDlg::AddReportData( CXTPReportRecord* a_pRecord, TDefineNetwork a_oDefineNetwork )
{
	std::set<int>::iterator fSetIter = m_setUsedUserCodeKey.find(a_oDefineNetwork.nUserCodeKey);

	CXTPReportRecordItem* pItem = nullptr;
	CString strValue;

 	if (fSetIter != m_setUsedUserCodeKey.end())	//Used일 경우
	{
		pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(_T("Used")));
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pItem->SetCaption(_T("씀"));
		}

		strValue.Format(_T("%d"), a_oDefineNetwork.nUserCodeKey);
		pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(strValue));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);
	}
	else
	{
		pItem = a_pRecord->AddItem(new CXTPReportRecordItem());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);

		strValue.Format(_T("%d"), a_oDefineNetwork.nUserCodeKey);
		pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(strValue));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);
	}

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(a_oDefineNetwork.strUserCodeName));
	pItem->SetEditable(TRUE);

	pItem = a_pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetItemData(a_oDefineNetwork.nSystemCodeKey);

	if (a_oDefineNetwork.nUserCodeKey == 0)
		pItem->SetEditable(FALSE);
}

bool KDefineSystemCodeDlg::VerifyImportNodeData( TDefineNetwork a_oDefineNetwork, CString& a_strErr )
{
	bool bResult = false;

	try
	{
		if (a_oDefineNetwork.nUserCodeKey ==  0 && a_oDefineNetwork.nSystemCodeKey != 0)
		{
			ThrowException(_T("존 노드의 시스템코드가 맞지 않습니다."));
		}
		
		int nSize = m_vecSystemNodeCode.size();

		for (int i = 0; i < nSize; ++i)
		{
			if (m_vecSystemNodeCode[i] == a_oDefineNetwork.nSystemCodeKey)
			{
				bResult = true;
			}
		}

		if (!bResult)
			ThrowException(_T("매칭되는 시스템 코드가 없습니다."));
	}
	catch (KExceptionPtr ex)
	{
		a_strErr = ex->GetErrorMessage();
		bResult = false;
	}
	catch (...)
	{
		a_strErr = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
		bResult = false;
	}

	return bResult;
}

bool KDefineSystemCodeDlg::VerifyImportLinkData( TDefineNetwork a_oDefineNetwork, CString& a_strErr )
{
	bool bResult = false;

	try
	{
		if (a_oDefineNetwork.nUserCodeKey ==  0 && a_oDefineNetwork.nSystemCodeKey != 0)
		{
			ThrowException(_T("존 노드의 시스템코드가 맞지 않습니다."));
		}

		int nSize = m_vecSystemLinkCode.size();

		for (int i = 0; i < nSize; ++i)
		{
			if (m_vecSystemLinkCode[i] == a_oDefineNetwork.nSystemCodeKey)
			{
				bResult = true;
			}
		}

		if (!bResult)
			ThrowException(_T("매칭되는 시스템 코드가 없습니다."));
	}
	catch (KExceptionPtr ex)
	{
		a_strErr = ex->GetErrorMessage();
		bResult = false;
	}
	catch (...)
	{
		a_strErr = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
		bResult = false;
	}

	return bResult;
}