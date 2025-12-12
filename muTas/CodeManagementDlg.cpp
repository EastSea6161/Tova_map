// CodeManagementDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "CodeManagementDlg.h"
#include "afxdialogex.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "CodeGroupNewDlg.h"
#include "CodeNewDlg.h"
#include "Target.h"

#include "..\QBicCommon\QBicTxtWriter.h"
#include "..\QBicCommon\QBicTxtReader.h"

#define BG_COLOR_DISABLE	RGB(250,250,250)
#define TEXT_COLOR_DISABLE	RGB(145,145,145)

enum
{
	_0_COLUMN_EDITABLE = 0,
	_1_COLUMN_GROUP_KEY,
	_2_COLUMN_GROUP_NAME,
	_3_COLUMN_USING
};

enum
{
	_0_COLUMN_CODE_KEY = 0,
	_1_COLUMN_CODE_NAME
};


// KCodeManagementDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCodeManagementDlg, KResizeDialogEx)

KCodeManagementDlg::KCodeManagementDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KCodeManagementDlg::IDD, pParent)
{
	m_bNewGroup = false;
	m_bNewCode = false;
	//m_pCodeManager = new KCodeManager;
	m_pCodeManager = NULL;
	m_nNewCode = 0;
	m_nNewGroupKey = 0;

    m_bEdited = false;
}


KCodeManagementDlg::~KCodeManagementDlg()
{
	//if(NULL != m_pCodeManager)
	//{
	//	delete m_pCodeManager;
	//}
}


void KCodeManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReportGroup);
	DDX_Control(pDX, IDC_REPORT2, m_wndReportCode);
	DDX_Control(pDX, IDC_BUTTON2, m_btnRemoveGroup);
	DDX_Control(pDX, IDC_BUTTON3, m_btnAddCode);
	DDX_Control(pDX, IDC_BUTTON4, m_btnRemoveCode);
}


BEGIN_MESSAGE_MAP(KCodeManagementDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KCodeManagementDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &KCodeManagementDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &KCodeManagementDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &KCodeManagementDlg::OnBnClickedButton4)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, IDC_REPORT1, &KCodeManagementDlg::OnGroupSelChanged)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, &KCodeManagementDlg::OnGroupValueChanged)
	ON_NOTIFY(XTP_NM_REPORT_EDIT_CANCELED, IDC_REPORT1, &KCodeManagementDlg::OnGroupEditCanceled)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT2, &KCodeManagementDlg::OnCodeValueChanged)
    ON_BN_CLICKED(IDC_BUTTON5, &KCodeManagementDlg::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON6, &KCodeManagementDlg::OnBnClickedButton6)
    ON_BN_CLICKED(IDOK, &KCodeManagementDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KCodeManagementDlg::OnBnClickedCancel)
END_MESSAGE_MAP()



void KCodeManagementDlg::CodeManager(KCodeManager* pCodeManager)
{
	//ASSERT(NULL != m_pCodeManager);
	//if(NULL != pCodeManager)
	//{
	//	*m_pCodeManager = *pCodeManager;
	//}
	m_pCodeManager = pCodeManager;

	std::vector<KCodeGroup*> codeGroups;
	m_pCodeManager->GetCodeGroups(codeGroups);
	std::vector<KCodeGroup*>::iterator itGroup, itGroupEnd = codeGroups.end();
	for(itGroup = codeGroups.begin(); itGroup != itGroupEnd; ++itGroup)
	{
		KCodeGroup* pCodeGroup = *itGroup;
		STCodeGroupManageInfo* pGroupInfo = new STCodeGroupManageInfo;
		pGroupInfo->emManage = EMCodeManageNone;						// emManage
		pGroupInfo->strName = pCodeGroup->CodeGroupName();				// strName
		
		// jyk추가 시작
		pGroupInfo->nKey = pCodeGroup->CodeGroupKey();					// nKey
		bool bUse = false;
		std::list<TUseGroupTblInfo*> useGroupInfoList;
		GetUseGroupInfo( pGroupInfo->nKey, bUse, useGroupInfoList);
		
		pGroupInfo->bUsed = bUse;										// bUsed

		pGroupInfo->strSysCodeYN = pCodeGroup->SystemCodeYN();			// strSysCodeYN 
		
		if(bUse) {
			pGroupInfo->useGroupInfoList = useGroupInfoList;			// useGroupInfoList
		}
		// jyk추가 종료

		pGroupInfo->strNewName = _T("");								// strNewName
		m_CodeGroupInfoList.push_back(pGroupInfo);

		std::list<STCodeManageInfo*> codeInfoList;
		std::vector<int> codes;
		pCodeGroup->GetCodes(codes);
		std::vector<int>::iterator itCode, itCodeEnd = codes.end();
		for(itCode = codes.begin(); itCode != itCodeEnd; ++itCode) {
			STCodeManageInfo* pCodeInfo = new STCodeManageInfo;
			pCodeInfo->emManage = EMCodeManageNone;
			pCodeInfo->nCode = *itCode;
			pCodeInfo->strName = pCodeGroup->SingleCodeValue(*itCode);
			pCodeInfo->strNewName = _T("");
			codeInfoList.push_back(pCodeInfo);
		}

		m_CodeInfoMap.insert(std::make_pair(pGroupInfo, codeInfoList));
	}
}


const KCodeManager* KCodeManagementDlg::CodeManager(void) const
{
	return m_pCodeManager;
}

// jyk추가 시작
void KCodeManagementDlg::SetTarget(KTarget* pTarget)
{
	TxLogDebugStartMsg();
	m_pTarget = pTarget;
	TxLogDebugEndMsg();
}
// jyk추가 종료

void KCodeManagementDlg::initCodeGroupList(void)
{
	// Report Header 설정
	//m_reportGroup.GetColumns()->Clear();

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportGroup.AddColumn(new CXTPReportColumn(_0_COLUMN_EDITABLE, _T("편집가능 여부"),25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() != KEMKorea) {
		pColumn->SetCaption(_T("Editability"));
	}

	pColumn = m_wndReportGroup.AddColumn(new CXTPReportColumn(_1_COLUMN_GROUP_KEY, _T("구분Key"),30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() != KEMKorea) {
		pColumn->SetCaption(_T("Code Key"));
	}

	pColumn = m_wndReportGroup.AddColumn(new CXTPReportColumn(_2_COLUMN_GROUP_NAME, _T("코드 그룹 명칭"),70));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() != KEMKorea) {
        pColumn->SetCaption(_T("Name"));
    }

	pColumn = m_wndReportGroup.AddColumn(new CXTPReportColumn(_3_COLUMN_USING, _T("사용여부"),25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() != KEMKorea) {
        pColumn->SetCaption(_T("In Use(Y/N)"));
    }
}


void KCodeManagementDlg::updateCodeGroupList(void)
{
	//std::vector<KCodeGroup*> codeGroups;
	//m_pCodeManager->GetCodeGroups(codeGroups);
	//std::vector<KCodeGroup*>::iterator itCodeGroup, itEnd = codeGroups.end();
	//for(itCodeGroup = codeGroups.begin(); itCodeGroup != itEnd; ++itCodeGroup)
	//{
	//	KCodeGroup* pCodeGroup = *itCodeGroup;
	//	CXTPReportRecord* pRecord = m_reportGroup.AddRecord(new CXTPReportRecord);
	//	CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText(pCodeGroup->CodeGroupName()));
	//	pItem->SetItemData((DWORD_PTR)pCodeGroup);
	//}

	//m_reportGroup.Populate();

	//if(m_reportGroup.GetRecords()->GetCount() > 0)
	//{
	//	m_reportGroup.GetRows()->GetAt(0)->SetSelected(TRUE);
	//}

	m_wndReportGroup.GetRecords()->RemoveAll();

	std::list<STCodeGroupManageInfo*>::iterator itGroup, itEnd = m_CodeGroupInfoList.end();
	
	int nMaxGroupKey = 0;
	for(itGroup = m_CodeGroupInfoList.begin(); itGroup != itEnd; ++itGroup)
	{
		STCodeGroupManageInfo* pInfo = *itGroup;
		if(EMCodeManageRemove == pInfo->emManage)
		{
			continue;
		}
		
		CXTPReportRecord* pRecord = m_wndReportGroup.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = NULL;

		// 1. editable
		bool bEditable = false;
		if(pInfo->strSysCodeYN.MakeUpper() == _T("Y"))
		{
			pRecord->SetEditable(FALSE);

			pItem = pRecord->AddItem( new CXTPReportRecordItemText(_T("N")) );

			if(itGroup == m_CodeGroupInfoList.begin())
			{
				m_btnRemoveGroup.EnableWindow(FALSE);
			}

			bEditable = false;
		}
		else
		{
			pRecord->SetEditable(TRUE);

			pItem = pRecord->AddItem( new CXTPReportRecordItemText(_T("Y")) );

			if(itGroup == m_CodeGroupInfoList.begin())
			{
				m_btnRemoveGroup.EnableWindow(TRUE);
			}

			bEditable = true;
		}

		if(bEditable == false)
		{
			pItem->SetBackgroundColor(BG_COLOR_DISABLE);
		}

		//2. group id
		pItem = pRecord->AddItem( new CXTPReportRecordItemNumber( pInfo->nKey, _T("%.0f")) );

		if(bEditable == false)
		{
			pItem->SetBackgroundColor(BG_COLOR_DISABLE);
			pItem->SetTextColor(TEXT_COLOR_DISABLE);
		}
		
		//3. group name
		if(EMCodeManageNone == pInfo->emManage)
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pInfo->strName));
		}
		else
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pInfo->strNewName));
		}

		if(bEditable == false)
		{
			pItem->SetBackgroundColor(BG_COLOR_DISABLE);
			pItem->SetTextColor(TEXT_COLOR_DISABLE);
		}

		pItem->SetItemData((DWORD_PTR)pInfo);
			
		//4. using info
		if(true == pInfo->bUsed)		
		{
			pItem = pRecord->AddItem( new CXTPReportRecordItemText(_T("Y")) );		// 해당 그룹 사용 중

			std::list<TUseGroupTblInfo*>::iterator itUse, itUseEnd = pInfo->useGroupInfoList.end();
			for(itUse = pInfo->useGroupInfoList.begin(); itUse != itUseEnd; ++itUse)
			{
				TUseGroupTblInfo* pUseInfo = *itUse;

				CString strTblName = pUseInfo->strTblName;
				CString strColName = pUseInfo->strColName;
			}

		}
		else 
		{
			pItem = pRecord->AddItem( new CXTPReportRecordItemText(_T("N")) );		// 해당 그룹 사용 무
		}

		if(bEditable == false)
		{
			pItem->SetBackgroundColor(BG_COLOR_DISABLE);
			pItem->SetTextColor(TEXT_COLOR_DISABLE);
		}

		if(nMaxGroupKey < pInfo->nKey)
		{
			nMaxGroupKey = pInfo->nKey;
		}
	}

	m_nNewGroupKey = nMaxGroupKey + 1;

	if( m_CodeGroupInfoList.size() == 0 )
	{
		m_nNewGroupKey = 1;
	}

	m_wndReportGroup.Populate();

	if(m_wndReportGroup.GetRecords()->GetCount() > 0)
	{
		m_wndReportGroup.GetRows()->GetAt(0)->SetSelected(TRUE);
	}
}

void KCodeManagementDlg::initCodeList(void)
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCode.AddColumn(new CXTPReportColumn(_0_COLUMN_CODE_KEY, _T("Code"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("코드번호"));
    }

	pColumn = m_wndReportCode.AddColumn(new CXTPReportColumn(_1_COLUMN_CODE_NAME, _T("Name"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }
}


void KCodeManagementDlg::ReloadCodeList(void)
{
	m_wndReportCode.GetRecords()->RemoveAll();

	//KCodeGroup* pCurrentGroup = NULL;

	//CXTPReportRow* pGroupRow = m_reportGroup.GetSelectedRows()->GetAt(0);
	//if(NULL != pGroupRow)
	//{
	//	CXTPReportRecord* pGroupRecord = pGroupRow->GetRecord();
	//	CXTPReportRecordItem* pGroupItem = pGroupRecord->GetItem(_2_COLUMN_GROUP_NAME );
	//	pCurrentGroup = (KCodeGroup*)(pGroupItem->GetItemData());
	//}

	//if(NULL != pCurrentGroup)
	//{
	//	std::vector<int> codes;
	//	pCurrentGroup->GetCodes(codes);
	//	std::vector<int>::iterator itCode, itEnd = codes.end();
	//	for(itCode = codes.begin(); itCode != itEnd; ++itCode)
	//	{
	//		CString strValue = pCurrentGroup->CodeValue(*itCode);

	//		CXTPReportRecord* pRecord = m_reportCode.AddRecord(new CXTPReportRecord);
	//		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(*itCode));
	//		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));
	//	}
	//}
	//m_reportCode.Populate();

	CString strKey;
	STCodeGroupManageInfo* pGroupInfo = NULL;
	CXTPReportRow* pGroupRow = m_wndReportGroup.GetSelectedRows()->GetAt(0);
	if(NULL != pGroupRow)
	{
		CXTPReportRecord* pGroupRecord = pGroupRow->GetRecord();
		CXTPReportRecordItem* pGroupItem = pGroupRecord->GetItem(_2_COLUMN_GROUP_NAME);
		//strKey = pGroupItem->GetCaption(NULL);
		pGroupInfo = (STCodeGroupManageInfo*)(pGroupItem->GetItemData());
	}

	//ASSERT(strKey.IsEmpty() == FALSE);
	ASSERT(NULL != pGroupInfo);

	auto itCodeMap = m_CodeInfoMap.find(pGroupInfo);

	ASSERT(m_CodeInfoMap.end() != itCodeMap);

	BOOL bEditable = FALSE;
	if(pGroupInfo->strSysCodeYN.MakeUpper() == _T("Y")) {
		bEditable = FALSE;
	}
	else {
		bEditable = TRUE;
	}

	std::list<STCodeManageInfo*>& codeInfoList = itCodeMap->second;
	std::list<STCodeManageInfo*>::iterator itCode, itCodeEnd = codeInfoList.end();
	for(itCode = codeInfoList.begin(); itCode != itCodeEnd; ++itCode)
	{
		STCodeManageInfo* pCodeInfo = *itCode;
		if(EMCodeManageRemove == pCodeInfo->emManage) {
			continue;
		}

		m_nNewCode = ( pCodeInfo->nCode ) + 1;

		CXTPReportRecord* pRecord = m_wndReportCode.AddRecord(new CXTPReportRecord);
		pRecord->SetEditable(bEditable);

		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pCodeInfo->nCode));
		if(bEditable == FALSE) {
			pItem->SetBackgroundColor(BG_COLOR_DISABLE);
			pItem->SetTextColor(TEXT_COLOR_DISABLE);
		}

		pItem->SetItemData((DWORD_PTR)pCodeInfo);
		pItem->GetEditOptions(NULL)->m_bSelectTextOnEdit = TRUE;
		pItem->GetEditOptions(NULL)->m_dwEditStyle |= ES_NUMBER;
		
		if(EMCodeManageNone == pCodeInfo->emManage) {
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pCodeInfo->strName));
		}
		else {
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pCodeInfo->strNewName));
		}
		if(bEditable == FALSE) {
			pItem->SetBackgroundColor(BG_COLOR_DISABLE);
			pItem->SetTextColor(TEXT_COLOR_DISABLE);
		}
	}

	if( codeInfoList.size() == 0 ) {
		m_nNewCode = 1;
	}

	if(bEditable == FALSE) {
		m_btnAddCode.EnableWindow(FALSE);
		m_btnRemoveCode.EnableWindow(FALSE);
	}

	m_wndReportCode.Populate();
}


CXTPReportRecord* KCodeManagementDlg::GetCurrentGroupRecord(void)
{
	CXTPReportRecord* pRet = NULL;

	CXTPReportSelectedRows* pRows = m_wndReportGroup.GetSelectedRows();
	ASSERT(pRows->GetCount() > 0);

	CXTPReportRow* pRow = pRows->GetAt(0);

	if(NULL != pRow)
	{
		pRet = pRow->GetRecord();
	}

	return pRet;
}


void KCodeManagementDlg::updateCodeGroupData(void)
{
	std::list<STCodeGroupManageInfo*>::iterator itGroup, itGroupEnd = m_CodeGroupInfoList.end();
	for(itGroup = m_CodeGroupInfoList.begin(); itGroup != itGroupEnd; ++itGroup)
	{
		STCodeGroupManageInfo* pGroupInfo = *itGroup;

		if(EMCodeManageNew == pGroupInfo->emManage)
		{
			m_pCodeManager->AddCodeGroup(pGroupInfo->nKey, pGroupInfo->strNewName);
		}
		else if(EMCodeManageModify == pGroupInfo->emManage)
		{
			m_pCodeManager->RenameCodeGroup(pGroupInfo->nKey, pGroupInfo->strName, pGroupInfo->strNewName);
		}
		else if(EMCodeManageRemove == pGroupInfo->emManage)
		{
			if(pGroupInfo->strSysCodeYN.MakeUpper() == _T("N"))		// 해당 그룹이 System 정의된 그룹일 시 삭제 하지 못하도록 한다. 
			{
				m_pCodeManager->RemoveCodeGroupKey(pGroupInfo->nKey);
			}
		}
	}
}


void KCodeManagementDlg::updateCodeData(void)
{
	std::map<STCodeGroupManageInfo*, std::list<STCodeManageInfo*>>::iterator itCodeGroup, itCodeGroupEnd = 
		m_CodeInfoMap.end();

	for(itCodeGroup = m_CodeInfoMap.begin(); itCodeGroup != itCodeGroupEnd; ++itCodeGroup)
	{
		STCodeGroupManageInfo* pGroupInfo = itCodeGroup->first;
		std::list<STCodeManageInfo*>& codeList = itCodeGroup->second;

		if(EMCodeManageRemove == pGroupInfo->emManage)
		{
			continue;
		}

		KCodeGroup* pCodeGroup = NULL;
		if(EMCodeManageNone == pGroupInfo->emManage)
		{
			pCodeGroup = m_pCodeManager->FindCodeGroup(pGroupInfo->nKey);
		}
		else
		{
			pCodeGroup = m_pCodeManager->FindCodeGroup(pGroupInfo->nKey);
		}

		if(NULL != pCodeGroup)
		{
			std::list<STCodeManageInfo*>::iterator itCode, itCodeEnd = codeList.end();
			for(itCode = codeList.begin(); itCode != itCodeEnd; ++itCode)
			{
				STCodeManageInfo* pCodeInfo = *itCode;
				if(EMCodeManageNew == pCodeInfo->emManage)
				{
					pCodeGroup->AddCode(pCodeInfo->nCode, pCodeInfo->strNewName);
				}
				else if(EMCodeManageModify == pCodeInfo->emManage)
				{
					pCodeGroup->UpdateCode(pCodeInfo->nCode, pCodeInfo->strNewName);
				}
				else if(EMCodeManageRemove == pCodeInfo->emManage)
				{
					pCodeGroup->RemoveCode(pCodeInfo->nCode);
				}
			}
		}
	}
}

// jyk추가
void KCodeManagementDlg::GetUseGroupInfo(int nGKey, bool &bUse, std::list<TUseGroupTblInfo*> &useGroupInfoList)
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format(
		_T(" SELECT Table_Name, Column_Name ")
		_T(" FROM Column_Description ")
		_T(" WHERE KCode_Group_Key = %d"), nGKey
		);

	try
	{
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{

			TUseGroupTblInfo* pUseGroupTblInfo = new TUseGroupTblInfo;
			pUseGroupTblInfo->strTblName = spResult->GetValueString(0);
			pUseGroupTblInfo->strColName = spResult->GetValueString(1);

			useGroupInfoList.push_back(pUseGroupTblInfo);
			bUse = true;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


// KCodeManagementDlg 메시지 처리기입니다.
BOOL KCodeManagementDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	KReportCtrlSetting::Default(m_wndReportGroup, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_wndReportGroup);
	initCodeGroupList();
	updateCodeGroupList();

	KReportCtrlSetting::Default(m_wndReportCode, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_wndReportCode);
	initCodeList();
	ReloadCodeList();		// jyk추가 시작

	SetResize(IDC_STATIC4,		SZ_TOP_CENTER,  SZ_TOP_RIGHT);				// Edit Text - 'Codes'

	SetResize(IDC_STATIC3,		SZ_TOP_CENTER,  SZ_BOTTOM_CENTER);			// 가운데 세로 라인

	SetResize(IDC_REPORT1,		SZ_TOP_LEFT,     SZ_BOTTOM_CENTER);
	SetResize(IDC_REPORT2,		SZ_TOP_CENTER,   SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON1,      SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON2,      SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON3,      SZ_BOTTOM_CENTER, SZ_BOTTOM_CENTER);
	SetResize(IDC_BUTTON4,      SZ_BOTTOM_CENTER, SZ_BOTTOM_CENTER);

	SetResize(IDC_STATIC2,		SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);			// 가로 라인

    SetResize(IDC_BUTTON5,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_BUTTON6,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDCANCEL,			SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
        
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KCodeManagementDlg::OnBnClickedButton1()
{
	int     nDuplicate   = 0;
	bool    bCancel      = false;
	int     nGroupKey    = m_nNewGroupKey;
	CString strGroupName = _T("");

	do
	{
		KCodeGroupNewDlg dlg;
		dlg.m_nGroupKey = nGroupKey;
		dlg.m_strGroupName = strGroupName;

		if(dlg.DoModal() == IDOK)
		{
			nDuplicate = 0;
			std::list<STCodeGroupManageInfo*>::iterator itGroup, itGroupEnd = m_CodeGroupInfoList.end();
			for(itGroup = m_CodeGroupInfoList.begin(); itGroup != itGroupEnd; ++itGroup)
			{
				STCodeGroupManageInfo* pGroupInfo = *itGroup;
				if(EMCodeManageNone == pGroupInfo->emManage)
				{
					if(pGroupInfo->strName.Compare(dlg.m_strGroupName) == 0)
					{
						nDuplicate = 1;
						break;
					}
					if(pGroupInfo->nKey == dlg.m_nGroupKey)
					{
						nDuplicate = 2;
						break;
					}
				}
				else if((EMCodeManageNew == pGroupInfo->emManage) || (EMCodeManageModify == pGroupInfo->emManage))
				{
					if(pGroupInfo->strNewName.Compare(dlg.m_strGroupName) == 0)
					{
						nDuplicate = 1;
						break;
					}
					if(pGroupInfo->nKey == dlg.m_nGroupKey)
					{
						nDuplicate = 2;
						break;
					}
				}
			}

			if(nDuplicate == 1)
			{
				//AfxMessageBox(_T("입력한 코드 그룹 명칭이 이미 존재 합니다.\n다시 입력해 주세요."));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					AfxMessageBox(_T("입력한 코드 그룹 명칭이 이미 존재 합니다.\n다시 입력해 주세요."));
				}
				else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
					AfxMessageBox(_T("The entered code group name exists.\nPlease enter again."));
				}
			}
			else if(nDuplicate == 2)
			{
				//AfxMessageBox(_T("입력한 구분 Key값이 이미 존재 합니다.\n다시 입력해 주세요."));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					AfxMessageBox(_T("입력한 구분Key 값이 이미 존재 합니다.\n다시 입력해 주세요."));
				}
				else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
					AfxMessageBox(_T("The entered code key exists.\nPlease enter again."));
				}
			}
			else
			{
				nGroupKey = dlg.m_nGroupKey;
				strGroupName = dlg.m_strGroupName;
			}
		}
		else
		{
			bCancel = true;
		}

	} while((false == bCancel) && (nDuplicate > 0));

	if(true == bCancel)
	{
		return;
	}

	STCodeGroupManageInfo* pNewGroupInfo = new STCodeGroupManageInfo;
	pNewGroupInfo->emManage = EMCodeManageNew;
	pNewGroupInfo->strNewName = strGroupName;
	pNewGroupInfo->bUsed = false;		// jyk추가
	pNewGroupInfo->strSysCodeYN = _T("N");
	pNewGroupInfo->nKey = nGroupKey;
	m_CodeGroupInfoList.push_back(pNewGroupInfo);

	std::list<STCodeManageInfo*> codesList;
	m_CodeInfoMap.insert(std::make_pair(pNewGroupInfo, codesList));

	if(m_nNewGroupKey <= (nGroupKey+1))
	{
		m_nNewGroupKey = (nGroupKey+1);
	}

	// add a new record
	CXTPReportRecord* pRecord = m_wndReportGroup.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Y")));

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nGroupKey, _T("%.0f")));

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(strGroupName));
	pItem->SetItemData((DWORD_PTR)pNewGroupInfo);

	pItem = pRecord->AddItem( new CXTPReportRecordItemText(_T("N")) );// jyk추가
	m_wndReportGroup.Populate();

	CXTPReportRow* pRow = m_wndReportGroup.GetRows()->GetAt(m_wndReportGroup.GetRows()->GetCount() - 1);
	m_wndReportGroup.SetFocusedRow(pRow);

    m_bEdited = true;
}


void KCodeManagementDlg::OnBnClickedButton2()
{
	CXTPReportRow* pGroupRow = m_wndReportGroup.GetSelectedRows()->GetAt(0);
	if(NULL == pGroupRow)
	{
		return;
	}

	if (KmzSystem::GetLanguage() == KEMKorea) {
		if (AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO) == IDNO)
		{
			return;
		}
	}
	else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
		if (AfxMessageBox(_T("Are you sure you want to delete?"), MB_YESNO) == IDNO)
		{
			return;
		}
	}
	

	int nIndex = pGroupRow->GetIndex();

	CXTPReportRecord* pGroupRecord = pGroupRow->GetRecord();
	CXTPReportRecordItem* pGroupItem = pGroupRecord->GetItem(_2_COLUMN_GROUP_NAME);
	STCodeGroupManageInfo* pGroupInfo = (STCodeGroupManageInfo*)(pGroupItem->GetItemData());
	if(EMCodeManageNew == pGroupInfo->emManage)
	{
		// delete code data
		std::map<STCodeGroupManageInfo*, std::list<STCodeManageInfo*>>::iterator itCodeMap = 
			m_CodeInfoMap.find(pGroupInfo);

		if(m_CodeInfoMap.end() != itCodeMap)
		{
			std::list<STCodeManageInfo*>& codeList = itCodeMap->second;
			std::list<STCodeManageInfo*>::iterator itCode, itCodeEnd = codeList.end();
			for(itCode = codeList.begin(); itCode != itCodeEnd; ++itCode)
			{
				STCodeManageInfo* pInfo = *itCode;
				delete pInfo;
			}
			m_CodeInfoMap.erase(itCodeMap);

			m_CodeGroupInfoList.remove(pGroupInfo);
		}

		delete pGroupInfo;
		m_wndReportGroup.GetRecords()->RemoveRecord(pGroupRecord);
		m_wndReportGroup.Populate();
	}
	else
	{
		if(pGroupInfo->strSysCodeYN.MakeUpper() == _T("Y"))
		{
			//AfxMessageBox(_T("해당 코드 그룹은 시스템에서 정의한 그룹이므로 삭제 할 수 없습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("해당 코드 그룹은 시스템에서 정의한 그룹이므로 삭제 할 수 없습니다."));
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("This code group was definited by system.\nYou CANNOT delete."));
			}
			return;
		}

		pGroupInfo->emManage = EMCodeManageRemove;
		updateCodeGroupList();
	}

	if(nIndex > 1)	// 선택한 행을 삭제 한 후 focus는 이전 행으로 셋팅
	{
		SetFocusControl(nIndex -1);
	}
	else
	{
		SetFocusControl(0);
	}

    m_bEdited = true;
}


void KCodeManagementDlg::SetFocusControl(int a_nIndex)
{
	CXTPReportRow* pRow = NULL;

	pRow = m_wndReportGroup.GetRows()->GetAt(a_nIndex);

	pRow->SetSelected(TRUE);
	m_wndReportGroup.SetFocusedRow(pRow);
	m_wndReportGroup.SetTopRow(a_nIndex);


	CXTPReportRecord* pGroupRecord = pRow->GetRecord();
	CXTPReportRecordItem* pGroupItem = pGroupRecord->GetItem(_2_COLUMN_GROUP_NAME);
	STCodeGroupManageInfo* pGroupInfo = (STCodeGroupManageInfo*)(pGroupItem->GetItemData());

	if(pGroupInfo->strSysCodeYN.MakeUpper() == _T("Y"))
	{
		m_btnRemoveGroup.EnableWindow(FALSE);
		m_btnAddCode.EnableWindow(FALSE);
		m_btnRemoveCode.EnableWindow(FALSE);
	}
	else
	{
		m_btnRemoveGroup.EnableWindow(TRUE);
		m_btnAddCode.EnableWindow(TRUE);
		m_btnRemoveCode.EnableWindow(TRUE);
	}
}


void KCodeManagementDlg::OnBnClickedButton3()
{
	bool bDuplicate = false;
	bool bCancel = false;
	int nCode = m_nNewCode;
	CString strValue = _T("");

	do
	{
		KCodeNewDlg dlg;
		dlg.m_nCode = nCode;
		dlg.m_strValue = strValue;

		if(dlg.DoModal() == IDOK)
		{
			bDuplicate = false;
			
			CXTPReportRecords* pRecords = m_wndReportCode.GetRecords();
			int nRecordCount = pRecords->GetCount();
			for(int i = 0; i < nRecordCount; ++i)
			{
				CXTPReportRecord* pRecord = pRecords->GetAt(i);
				CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(_0_COLUMN_CODE_KEY));
				if(dlg.m_nCode == pNumber->GetValue())
				{
					bDuplicate = true;
					break;
				}
			}

			if(true == bDuplicate)
			{
				if (KmzSystem::GetLanguage() == KEMKorea) {
					AfxMessageBox(_T("입력한 코드번호가 이미 존재 합니다.\n다시 입력해 주세요."));
				}
				else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
					AfxMessageBox(_T("The entered code number exists.\nPlease enter again."));
				}
			}
			else
			{
				nCode = dlg.m_nCode;
				strValue = dlg.m_strValue;
			}
		}
		else 
		{
			bCancel = true;
		}
	} while((false == bCancel) && (true == bDuplicate));

	if(true == bCancel)
	{
		return;
	}

	STCodeManageInfo* pNewCodeInfo = new STCodeManageInfo; {
        pNewCodeInfo->emManage = EMCodeManageNew;
        pNewCodeInfo->nCode = nCode;
        pNewCodeInfo->strNewName = strValue;
    }	

	//m_nNewCode = (nCode+1);
	if(m_nNewCode <= (nCode+1)) {
		m_nNewCode = (nCode+1);
	}

	CXTPReportRecord*      pGroupRecord = GetCurrentGroupRecord();
	CXTPReportRecordItem*  pGroupItem = pGroupRecord->GetItem(_2_COLUMN_GROUP_NAME);
	STCodeGroupManageInfo* pGroupInfo = (STCodeGroupManageInfo*)(pGroupItem->GetItemData());
	//CString strGroupName = pGroupItem->GetCaption(NULL);
	
	std::map<STCodeGroupManageInfo*, std::list<STCodeManageInfo*>>::iterator itCodeMap = m_CodeInfoMap.find(pGroupInfo);
	ASSERT(m_CodeInfoMap.end() != itCodeMap);

	std::list<STCodeManageInfo*>& codeList = itCodeMap->second;
	codeList.push_back(pNewCodeInfo);

	// add a new record
	CXTPReportRecord* pRecord = m_wndReportCode.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nCode));
	pItem->SetItemData((DWORD_PTR)pNewCodeInfo);
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));
	m_wndReportCode.Populate();

	CXTPReportRow* pRow = m_wndReportCode.GetRows()->GetAt(m_wndReportCode.GetRows()->GetCount() - 1);
	m_wndReportCode.SetFocusedRow(pRow);

    m_bEdited = true;
}

// 코드 Delete
void KCodeManagementDlg::OnBnClickedButton4()
{
	CXTPReportRow* pRow = m_wndReportCode.GetSelectedRows()->GetAt(0);
	if(NULL == pRow) {
		return;
	}

	if (KmzSystem::GetLanguage() == KEMKorea) {
		if (AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO) == IDNO)
		{
			return;
		}
	}
	else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
		if (AfxMessageBox(_T("Are you sure you want to delete?"), MB_YESNO) == IDNO)
		{
			return;
		}
	}

	CXTPReportRecord* pRecord = pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_CODE_KEY);
	STCodeManageInfo* pCodeInfo = (STCodeManageInfo*)(pItem->GetItemData());

	// 새로 추가된 Item은 바로 삭제한다.
	if(EMCodeManageNew == pCodeInfo->emManage)
	{
		CXTPReportRecord*      pGroupRecord = GetCurrentGroupRecord();
		CXTPReportRecordItem*  pGroupItem   = pGroupRecord->GetItem(_2_COLUMN_GROUP_NAME);
		STCodeGroupManageInfo* pGroupInfo   = (STCodeGroupManageInfo*)(pGroupItem->GetItemData());
		
        auto itCodeMap = m_CodeInfoMap.find(pGroupInfo);
		if(m_CodeInfoMap.end() != itCodeMap)
		{
			std::list<STCodeManageInfo*>& codeList = itCodeMap->second;
			std::list<STCodeManageInfo*>::iterator itCode, itCodeEnd = codeList.end();
			for(itCode = codeList.begin(); itCode != itCodeEnd; ++itCode)
			{
				if(pCodeInfo == *itCode)
				{
					codeList.erase(itCode);
					break;
				}
			}
		}

		delete pCodeInfo;
		m_wndReportCode.GetRecords()->RemoveRecord(pRecord);
		m_wndReportCode.Populate();
	}
	else 
	{
		pCodeInfo->emManage = EMCodeManageRemove;
		ReloadCodeList();
	}

    m_bEdited = true;
}


void KCodeManagementDlg::OnGroupSelChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	CXTPReportRow* pRow = m_wndReportGroup.GetFocusedRow();
	if(NULL != pRow)
	{
		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItem* pItem = pRecord->GetItem(_2_COLUMN_GROUP_NAME);
		STCodeGroupManageInfo* pEditInfo = (STCodeGroupManageInfo*)(pItem->GetItemData());
		CString strSysCodeYN = pEditInfo->strSysCodeYN;

		if( strSysCodeYN.MakeUpper() == _T("Y") )
		{
			m_btnRemoveGroup.EnableWindow(FALSE);
			m_btnRemoveCode.EnableWindow(FALSE);
			m_btnAddCode.EnableWindow(FALSE);

            GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
		}
		else
		{
			m_btnRemoveGroup.EnableWindow(TRUE);
			m_btnRemoveCode.EnableWindow(TRUE);
			m_btnAddCode.EnableWindow(TRUE);
            
            GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_btnRemoveGroup.EnableWindow(TRUE);
		m_btnRemoveCode.EnableWindow(TRUE);
		m_btnAddCode.EnableWindow(TRUE);

        GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	ReloadCodeList();
}


void KCodeManagementDlg::OnGroupValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	CString strGroup = pItemNotify->pItem->GetCaption(NULL);
	STCodeGroupManageInfo* pEditInfo = (STCodeGroupManageInfo*)(pItemNotify->pItem->GetItemData());

	bool bDuplicate = false;
	std::list<STCodeGroupManageInfo*>::iterator itGroup, itGroupEnd = m_CodeGroupInfoList.end();
	for(itGroup = m_CodeGroupInfoList.begin(); itGroup != itGroupEnd; ++itGroup)
	{
		STCodeGroupManageInfo* pGroupInfo = *itGroup;
		if(EMCodeManageNone == pGroupInfo->emManage)
		{
			if(pGroupInfo->strName.Compare(strGroup) == 0)
			{
				bDuplicate = true;
				break;
			}
		}
		else if((EMCodeManageNew == pGroupInfo->emManage) || (EMCodeManageModify == pGroupInfo->emManage))
		{
			if(pGroupInfo->strNewName.Compare(strGroup) == 0)
			{
				bDuplicate = true;
				break;
			}
		}
	}

	if(true == bDuplicate)
	{
		//AfxMessageBox(_T("입력한 코드 그룹 명칭이 이미 존재 합니다.\n다시 입력해 주세요."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("입력한 코드 그룹 명칭이 이미 존재 합니다.\n다시 입력해 주세요."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("The entered code group name exists.\nPlease enter again."));
		}
		XTP_REPORTRECORDITEM_ARGS itemArgs(&m_wndReportGroup, pItemNotify->pRow, pItemNotify->pColumn);
		m_wndReportGroup.EditItem(&itemArgs);
		CXTPReportInplaceEdit* pEdit = m_wndReportGroup.GetInplaceEdit();
		if(NULL != pEdit)
		{
			pEdit->SetSel(0, -1);
		}		
	}
	else
	{
		pEditInfo->strNewName = strGroup;
		if(EMCodeManageNone == pEditInfo->emManage)
		{
			pEditInfo->emManage = EMCodeManageModify;
		}

        m_bEdited = true;
	}
}

void KCodeManagementDlg::OnGroupEditCanceled(NMHDR*  pNotifyStruct, LRESULT* result)
{
}

void KCodeManagementDlg::OnCodeValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
	
    CXTPReportRecord*     pRecord   = pItemNotify->pItem->GetRecord();
	CXTPReportRecordItem* pItem     = pRecord->GetItem(_0_COLUMN_CODE_KEY);
	STCodeManageInfo*     pCodeInfo = (STCodeManageInfo*)(pItem->GetItemData());
	ASSERT(NULL != pCodeInfo);
	
    if(NULL == pCodeInfo) {
		return;
	}

	if(EMCodeManageNone == pCodeInfo->emManage) {
		pCodeInfo->emManage = EMCodeManageModify;
	}

	CString strValue = pItemNotify->pItem->GetCaption(NULL);
	pCodeInfo->strNewName = strValue;

    m_bEdited = true;
}

void KCodeManagementDlg::OnBnClickedButton5()
{//★ Export
    CXTPReportRecords* pRecords = m_wndReportCode.GetRecords();
    int nCount = pRecords->GetCount();
    
    CFileDialog oFileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL); {
        oFileDlg.m_ofn.lpstrTitle = _T("Export Data");
    }
    
    if (oFileDlg.DoModal() != IDOK)
        return;

    CString strPath = oFileDlg.GetPathName(); {
        strPath.Replace(_T(".txt"), _T(""));
        strPath.Format(_T("%s.txt"), strPath);
    }

    try {
        QBicTxtWriter oWriter;
        if ( oWriter.Open(strPath) == false ) {
            //AfxMessageBox(_T("파일을 생성하는 과정에서 오류가 발생하였습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("파일을 생성하는 과정에서 오류가 발생하였습니다 !"));
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("Error occured while creating file !"));
			}
            return;
        }

        CXTPReportRecordItem* pItem = nullptr;
        for (int i=0; i<nCount; i++) {
            CXTPReportRecord* pRecord = pRecords->GetAt(i);

            CString strOut(_T("")); {
                pItem = pRecord->GetItem(0);
                strOut.Format(_T("%s"), pItem->GetCaption());

                pItem = pRecord->GetItem(1);
                strOut.AppendFormat(_T(",%s"), pItem->GetCaption());
            }
            oWriter.Write(strOut);
        }

        oWriter.Close();

        //AfxMessageBox(_T("완료되었습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("완료되었습니다."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Export complete."));
		}
    }
    catch(...) {
        //AfxMessageBox(_T("파일을 생성하는 과정에서 오류가 발생하였습니다 !"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("파일을 생성하는 과정에서 오류가 발생하였습니다 !"));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occured while creating file !"));
		}
    }    
}


void KCodeManagementDlg::OnBnClickedButton6()
{//★ Import    
    CXTPReportRecord* pGroupRecord = GetCurrentGroupRecord();
    if (pGroupRecord == nullptr) {
        //AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("The selected code group does not exist."));
		}
        return;
    }

    CXTPReportRecordItem*  pGroupItem = pGroupRecord->GetItem(_2_COLUMN_GROUP_NAME);
    STCodeGroupManageInfo* pGroupInfo = (STCodeGroupManageInfo*)(pGroupItem->GetItemData());
    if(pGroupInfo == nullptr) {
        //AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("The selected code group does not exist."));
		}
        return;
    }

    if ( _tcsicmp(pGroupInfo->strSysCodeYN, _T("Y")) == 0) {
        //AfxMessageBox(_T("시스템에서 사용하는 코드 그룹은 불러오기를 수행할 수 없습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("시스템에서 사용하는 코드 그룹은 불러오기를 수행할 수 없습니다."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Code groups used by the system cannot be imported."));
		}
        return;
    }
        
    auto itCodeMap = m_CodeInfoMap.find(pGroupInfo);
    if (m_CodeInfoMap.end() == itCodeMap)
        return;
        
    TCHAR       szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
    CFileDialog oFileDlg( TRUE, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter );
    if( oFileDlg.DoModal() != IDOK )
        return;

    CString strPath = oFileDlg.GetPathName();    
    
    QBicTxtReader oReader;
    if (oReader.Open(strPath) == false) {
        //AfxMessageBox(_T("파일을 읽는 과정에서 오류가 발생하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("파일을 읽는 과정에서 오류가 발생하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occured while reading file !"));
		}
        return;
    }
    
    //★ 파일을 읽어서 저장한다.
    std::map<int, CString> mapCode;

    std::vector<CString> vecLine;
    while (oReader.ReadLine(_T(','), vecLine)) {
        if (vecLine.size() < 2)
            continue;

        CString strCode = vecLine[0]; {
            strCode = strCode.Trim();
        }

        int     nCode   = _ttoi(strCode);
        CString strName = vecLine[1];

        mapCode.insert(std::make_pair(nCode, strName));
    }
    oReader.Close();

    if (mapCode.size() == 0) {
        //AfxMessageBox(_T("파일에 코드 관련 정보가 존재하지 않습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("파일에 코드 관련 정보가 존재하지 않습니다 !"));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("There is no any information about code group !"));
		}
        return;
    }

    CString strMsg(_T("")); {
        //strMsg.Format(_T("%d개 코드 그룹 내용을 입력하시겠습니까?"), mapCode.size());
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strMsg.Format(_T("%d개 코드 그룹 내용을 입력하시겠습니까?"), mapCode.size());
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			strMsg.Format(_T("Do you want to import %d code group contents?"), mapCode.size());
		}
    }

    if ( AfxMessageBox(strMsg, MB_YESNO) != IDYES)
        return;

    DeleteAllCode();

    int nCode(0);
    for (auto iter = mapCode.begin(); iter != mapCode.end(); ++iter) {
        nCode = iter->first;
        CString strValue = iter->second;

        STCodeManageInfo* pNewCodeInfo = new STCodeManageInfo; {
            pNewCodeInfo->emManage = EMCodeManageNew;
            pNewCodeInfo->nCode = nCode;
            pNewCodeInfo->strNewName = strValue;
        }	

        std::list<STCodeManageInfo*>& codeList = itCodeMap->second; {
            codeList.push_back(pNewCodeInfo);
        }

        CXTPReportRecord*     pRecord = m_wndReportCode.AddRecord(new CXTPReportRecord);
        CXTPReportRecordItem* pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(nCode)); {
            pItem->SetItemData((DWORD_PTR)pNewCodeInfo);
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));
    }
    
    if(m_nNewCode <= (nCode+1)) {
        m_nNewCode = (nCode+1);
    }

    m_wndReportCode.Populate();

    CXTPReportRow* pRow = m_wndReportCode.GetRows()->GetAt(m_wndReportCode.GetRows()->GetCount() - 1);
    m_wndReportCode.SetFocusedRow(pRow);

    m_bEdited = true;

    /*
    int nGroupKey = pGroupInfo->nKey;
    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    spDBase->BeginTransaction();
    
    CString strSQL(_T("")); {
        strSQL.Format(_T("Delete From KCode_Define Where KCode_Group_Key = '%d'"), nGroupKey);
    }

    spDBase->ExecuteUpdate(strSQL);
    
    strSQL.Empty(); {
        strSQL.Append(_T(" Insert Into KCode_Define "));
        strSQL.Append(_T(" (KCode_Group_Key, KCode_Key, KCode_Display) "));
        strSQL.Append(_T(" Values( ?, ?, ?) "));
    }

    KPreparedPtr spPrepared = spDBase->PrepareStatement(strSQL);
    for (auto iter = mapCode.begin(); iter != mapCode.end(); ++iter) {
        int     nCode   = iter->first;
        CString strName = iter->second;

        spPrepared->BindInt (1, nGroupKey);
        spPrepared->BindInt (2, nCode);
        spPrepared->BindText(3, strName);

        spPrepared->ExecuteUpdate();
        spPrepared->Reset();
    }

    spDBase->Commit();
    */

}

void KCodeManagementDlg::DeleteAllCode()
{
    CXTPReportRecord*      pGroupRecord = GetCurrentGroupRecord();
    CXTPReportRecordItem*  pGroupItem   = pGroupRecord->GetItem(_2_COLUMN_GROUP_NAME);
    STCodeGroupManageInfo* pGroupInfo   = (STCodeGroupManageInfo*)(pGroupItem->GetItemData());

    CXTPReportRecords* pRecords = m_wndReportCode.GetRecords();
    for (int i=0; i<pRecords->GetCount(); i++) {
        CXTPReportRecord* pRecord = pRecords->GetAt(i);
        CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_CODE_KEY);
        STCodeManageInfo* pCodeInfo = (STCodeManageInfo*)(pItem->GetItemData());

        if(EMCodeManageNew == pCodeInfo->emManage) {
            auto itCodeMap = m_CodeInfoMap.find(pGroupInfo);
            if(m_CodeInfoMap.end() != itCodeMap) {
                std::list<STCodeManageInfo*>& codeList = itCodeMap->second;
                std::list<STCodeManageInfo*>::iterator itCode, itCodeEnd = codeList.end();
                for(itCode = codeList.begin(); itCode != itCodeEnd; ++itCode) {
                    if(pCodeInfo == *itCode) {
                        codeList.erase(itCode);
                        break;
                    }
                }
            }

            delete pCodeInfo;        
        }
        else {
            pCodeInfo->emManage = EMCodeManageRemove;
        }
    }
    
    m_wndReportCode.ResetContent();
}

void KCodeManagementDlg::OnBnClickedOk()
{
    if (m_bEdited == false) {
        KResizeDialogEx::OnOK();
        return;
    }

	if (KmzSystem::GetLanguage() == KEMKorea) {
		if (AfxMessageBox(_T("설정하신 내역을 저장하시겠습니까?"), MB_YESNO) == IDNO) {
			return;
		}
	}
	else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
		if (AfxMessageBox(_T("Do you want to save your settings?"), MB_YESNO) == IDNO) {
			return;
		}
	}
    

    updateCodeGroupData();
    updateCodeData();

    KResizeDialogEx::OnOK();
}

void KCodeManagementDlg::OnBnClickedCancel()
{
    if (m_bEdited == true) {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			if (AfxMessageBox(_T("변경한 사항이 존재합니다. 그래도 종료하시겠습니까?"), MB_YESNO) == IDNO) {
				return;
			}
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			if (AfxMessageBox(_T("Do you want to exit without saving?"), MB_YESNO) == IDNO) {
				return;
			}
		}
    }

    KResizeDialogEx::OnCancel();
}
