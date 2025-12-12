// MDefVarSubPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MDefVarSubPage.h"
#include "afxdialogex.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
#include "Target.h"
#include "CustomReportRecordItem.h"
#include "DBaseConnector.h"

// KMDefVarSubPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMDefVarSubPage, KDialogEx)

	KMDefVarSubPage::KMDefVarSubPage(int a_nPageID, int a_nPurposeID, CString a_strPurposeName, TDefVarDefaultODGroup &a_oTDefVarDefaultODGroup, KTarget* a_pTarget, std::map<int, CString> &a_mapFunctionInfo, CWnd* pParent /*=NULL*/)
	: KDialogEx(KMDefVarSubPage::IDD, pParent)
	, m_nPageID(a_nPageID)
	, m_nPurposeID(a_nPurposeID)
	, m_strPurposeName(a_strPurposeName)
	, m_oTDefVarDefaultODGroup(a_oTDefVarDefaultODGroup)
	, m_pTarget(a_pTarget)
	, m_mapFunctionInfo(a_mapFunctionInfo)
	, m_nDefaultFunctionID(-1)
{
	SetDefaultFunctionID();
}

KMDefVarSubPage::~KMDefVarSubPage()
{
}

void KMDefVarSubPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_cboODGroup);
	DDX_Control(pDX, IDC_COMBO_FUNCTION, m_cboFunctionNoneGroup);
}


BEGIN_MESSAGE_MAP(KMDefVarSubPage, KDialogEx)
	ON_BN_CLICKED(IDC_CHECK_GROUP, &KMDefVarSubPage::OnBnClickedCheckGroup)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, &KMDefVarSubPage::OnCbnSelchangeComboGroup)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED,    IDC_REPORT, &KMDefVarSubPage::OnReportValueChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNCTION, &KMDefVarSubPage::OnCbnSelchangeComboFunction)
END_MESSAGE_MAP()


// KMDefVarSubPage 메시지 처리기입니다.
void KMDefVarSubPage::SetDefaultFunctionID()
{
	try
	{
		m_nDefaultFunctionID = -1;

		AutoType iter = m_mapFunctionInfo.begin();
		if (iter != m_mapFunctionInfo.end())
		{
			m_nDefaultFunctionID = iter->first;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefVarSubPage::UpdateFunctionInfo()
{
	SetDefaultFunctionID();

	InitComboFunctionNoneGroup();

	m_ctrlReport.ResetContent();
	CXTPReportColumns* pColumns = m_ctrlReport.GetColumns();
	pColumns->Clear();
	InitReportHeader();

	try
	{
		AutoType iter  = m_mapODGroupFunctionID.begin();
		AutoType itEnd = m_mapODGroupFunctionID.end();

		while (iter != itEnd)
		{
			std::map<int, int> &mapFunctionID = iter->second;

			AutoType iterB  = mapFunctionID.begin();
			AutoType itEndB = mapFunctionID.end();

			while (iterB != itEndB)
			{
				int &nFunctionID = iterB->second;

				if (m_mapFunctionInfo.find(nFunctionID) == m_mapFunctionInfo.end()) // 삭제 여부 확인
				{
					nFunctionID = m_nDefaultFunctionID;
				}

				++iterB;
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	ControlComboGroup();
}


void KMDefVarSubPage::UsedFunctionIDSet( std::set<int> &a_setUsedFunctionID )
{
	try
	{
		if (m_bUseODGroup)
		{
			CXTPReportRecords*    pRecords = m_ctrlReport.GetRecords();
			CXTPReportRecord*     pRecord  = nullptr;
			CXTPReportRecordItem* pItem    = nullptr;

			int nRecordCnt = pRecords->GetCount();

			for (int i= 0; i< nRecordCnt; i++)
			{
				pRecord  = pRecords->GetAt(i);
				pItem    = pRecord->GetItem(2);

				int nFunctionID  = (int)pItem->GetItemData();

				a_setUsedFunctionID.insert(nFunctionID);
			}
		}
		else
		{
			a_setUsedFunctionID.insert(m_nNoneGroupFunctionID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefVarSubPage::GetResultInfo(TDefVarDefaultODGroup &a_oTDefVarGroupResultInfo)
{
	try
	{
		a_oTDefVarGroupResultInfo.nPurposeID  = m_nPurposeID;
		a_oTDefVarGroupResultInfo.bUseODGroup = m_bUseODGroup;

		if (m_bUseODGroup)
		{
			int nCur         = m_cboODGroup.GetCurSel();
			KCodeGroup* pCodeGroup = (KCodeGroup*)m_cboODGroup.GetItemData(nCur);

			AutoType itFind  = m_mapODGroupFunctionID.find(pCodeGroup);
			if (itFind != m_mapODGroupFunctionID.end())
			{
				a_oTDefVarGroupResultInfo.nODGroupCode  = pCodeGroup->CodeGroupKey();
				a_oTDefVarGroupResultInfo.mapFunctionID = itFind->second;
			}
		}
		else
		{
			a_oTDefVarGroupResultInfo.nNoneGroupFunctionID = m_nNoneGroupFunctionID;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefVarSubPage::ExternalInitialize( TDefVarDefaultODGroup &a_oTDefVarDefaultODGroup )
{
	m_oTDefVarDefaultODGroup = a_oTDefVarDefaultODGroup;
	m_cboFunctionNoneGroup.ResetContent();
	InitOpen();
}


BOOL KMDefVarSubPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE, FALSE);
	InitReportHeader();

	InitOpen();

	SetBackgroundColor(RGB(255,255,255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMDefVarSubPage::InitOpen()
{
	try
	{
		CWaitCursor cw;

		std::set<int> setODGroupKey;

		CString strSQL(_T(" SELECT OD_Group_Key FROM Group_Generation_OD "));

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		try
		{
			KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
			while (spResultSet->Next())
			{
				int nODGroupKey = spResultSet->GetValueInt(0);
				setODGroupKey.insert(nODGroupKey);
			}
		}
		catch (...)
		{
			TxLogDebugException();
		}

		m_cboODGroup.ResetContent();

		AutoType iter  = setODGroupKey.begin();
		AutoType itEnd = setODGroupKey.end();

		int nInitCur(0);
		while (iter != itEnd)
		{
			int nODGroupKey = *iter;

			KCodeGroup* pCodeGroup  = m_pTarget->CodeManager()->FindCodeGroup(nODGroupKey);
			if (nullptr != pCodeGroup)
			{

				int nIndex = m_cboODGroup.AddString(pCodeGroup->CodeGroupName());
				m_cboODGroup.SetItemData(nIndex, (DWORD_PTR) pCodeGroup);

				if (m_oTDefVarDefaultODGroup.bUseODGroup)
				{
					int nCodeGroupKey = pCodeGroup->CodeGroupKey();
					if (nCodeGroupKey == m_oTDefVarDefaultODGroup.nODGroupCode)
					{
						nInitCur = nIndex;
					}
				}
			}

			++iter;
		}

		if (m_cboODGroup.GetCount() > 0)
		{
			m_cboODGroup.SetCurSel(nInitCur);
		}

		m_mapODGroupFunctionID.clear();
		for (int i= 0; i< m_cboODGroup.GetCount(); i++)
		{
			std::map<int, CString> mapODGroupName;
			KCodeGroup* pCodeGroup = (KCodeGroup*)m_cboODGroup.GetItemData(i);
			pCodeGroup->GetCodes(mapODGroupName);

			AutoType iter  = mapODGroupName.begin();
			AutoType itEnd = mapODGroupName.end();

			std::map<int, int> mapFunctionID;

			if (pCodeGroup->CodeGroupKey() == m_oTDefVarDefaultODGroup.nODGroupCode)
			{
				while (iter != itEnd)
				{
					int nCode = iter->first;
					AutoType itFind = m_oTDefVarDefaultODGroup.mapFunctionID.find(nCode);
					if (itFind != m_oTDefVarDefaultODGroup.mapFunctionID.end())
					{
						mapFunctionID.insert(std::make_pair(nCode, itFind->second));
					}
					else
					{
						mapFunctionID.insert(std::make_pair(nCode, m_nDefaultFunctionID));
					}

					++iter;
				}
			}
			else
			{
				while (iter != itEnd)
				{
					int nCode = iter->first;
					mapFunctionID.insert(std::make_pair(nCode, m_nDefaultFunctionID));
					++iter;
				}
			}

			m_mapODGroupFunctionID.insert(std::make_pair(pCodeGroup, mapFunctionID));
		}

		if (m_oTDefVarDefaultODGroup.bUseODGroup)
		{
			CheckDlgButton(IDC_CHECK_GROUP, BST_CHECKED);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_GROUP, BST_UNCHECKED);
		}
		ControlCheckGroup();
		ControlComboGroup();

		InitComboFunctionNoneGroup();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefVarSubPage::InitComboFunctionNoneGroup()
{
	try
	{
		int nDefaultFunctionID(0);

		if (m_cboFunctionNoneGroup.GetCount() > 0)
		{
			int nCur           = m_cboFunctionNoneGroup.GetCurSel();
			nDefaultFunctionID = (int)m_cboFunctionNoneGroup.GetItemData(nCur); // PreSelFunctionID
		}
		else
		{
			nDefaultFunctionID = m_oTDefVarDefaultODGroup.nNoneGroupFunctionID;
		}

		m_cboFunctionNoneGroup.ResetContent();

		AutoType iter  = m_mapFunctionInfo.begin();
		AutoType itEnd = m_mapFunctionInfo.end();

		int nIndex(0);
		int nInitIndex(nIndex);

		while (iter != itEnd)
		{
			int     nFunctionID = iter->first;
			CString strName     = iter->second;

			nIndex = m_cboFunctionNoneGroup.AddString(strName);
			m_cboFunctionNoneGroup.SetItemData(nIndex, (DWORD_PTR)nFunctionID);

			if (nFunctionID == nDefaultFunctionID)
			{
				nInitIndex = nIndex;
			}

			++iter;
		}

		if (m_cboFunctionNoneGroup.GetCount() > 0)
		{
			m_cboFunctionNoneGroup.SetCurSel(nInitIndex);
			ControlComboNoneGroupFunction();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefVarSubPage::InitReportHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(0, _T("Code"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("그룹 번호"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(1, _T("Name"),     50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("그룹명"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(2, _T("Function"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("적용 모형식"));
		}
		if (m_mapFunctionInfo.size() > 0)
		{
			CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
			pEditOptions->AddComboButton(TRUE);
			pEditOptions->m_bAllowEdit = TRUE;
			pEditOptions->m_bExpandOnSelect = TRUE;

			AutoType iter  = m_mapFunctionInfo.begin();
			AutoType itEnd = m_mapFunctionInfo.end();
			while (iter != itEnd)
			{
				int     nFunctionID = iter->first;
				CString strName     = iter->second;

				pEditOptions->AddConstraint(strName, (DWORD_PTR)nFunctionID);

				++iter;
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


void KMDefVarSubPage::OnBnClickedCheckGroup()
{
	ControlCheckGroup();
}


void KMDefVarSubPage::ControlCheckGroup()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_GROUP) == BST_CHECKED)
		{
			GetDlgItem(IDC_COMBO_GROUP)->EnableWindow(TRUE);
			GetDlgItem(IDC_REPORT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STATIC_FUNC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_FUNCTION)->ShowWindow(SW_HIDE);

			m_bUseODGroup = true;
		}
		else
		{
			GetDlgItem(IDC_COMBO_GROUP)->EnableWindow(FALSE);
			GetDlgItem(IDC_REPORT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STATIC_FUNC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_FUNCTION)->ShowWindow(SW_SHOW);

			m_bUseODGroup = false;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMDefVarSubPage::OnCbnSelchangeComboGroup()
{
	ControlComboGroup();
}


void KMDefVarSubPage::ControlComboGroup()
{
	try
	{
		m_ctrlReport.ResetContent();

		int nCur = m_cboODGroup.GetCurSel();
		if (nCur < 0)
			return;

		std::map<int, CString> mapODGroupName;

		KCodeGroup* pCodeGroup = (KCodeGroup*)m_cboODGroup.GetItemData(nCur);
		pCodeGroup->GetCodes(mapODGroupName);

		std::map<int, int> mapODGroupTempFunctionID;
		AutoType itFind = m_mapODGroupFunctionID.find(pCodeGroup);
		if (itFind != m_mapODGroupFunctionID.end())
		{
			mapODGroupTempFunctionID = itFind->second;
		}

		size_t nxFunctionCnt = m_mapFunctionInfo.size();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = mapODGroupName.begin();
		AutoType itEnd = mapODGroupName.end();

		while (iter != itEnd)
		{
			int     nCode        = iter->first;
			CString strGroupName = iter->second;

			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

			CString strCode(_T(""));
			strCode.Format(_T("%d"), nCode);
			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->SetCaption(strCode);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR) nCode);
			pItem->SetEditable(FALSE);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strGroupName));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);

			if (nxFunctionCnt > 0)
			{
				pItem   = pRecord->AddItem(new CCustomReportRecordItem);

				if (mapODGroupTempFunctionID.size() > 0)
				{
					AutoType itFind = mapODGroupTempFunctionID.find(nCode);
					if (itFind != mapODGroupTempFunctionID.end())
						pItem->SetItemData((DWORD_PTR) itFind->second);
				}
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
			}

			++iter;
		}

		m_ctrlReport.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KMDefVarSubPage::InvalidateInputInfo()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_GROUP) == BST_CHECKED)
		{
			// Function이 없다면, 상위단에서 필터링 되고, Function이 있다면, 그 중 하나는 선택되어 있기 때문에, 여기서 필터링할 대상은 없다.
		}
		else
		{
			int nCurSel = m_cboFunctionNoneGroup.GetCurSel();
			if (nCurSel < 0)
			{
				CString strMsg(_T(""));
				strMsg.Format(_T("%s에 함수를 선택해 주세요."), m_strPurposeName);
				AfxMessageBox(strMsg);
				m_cboFunctionNoneGroup.SetFocus();

				return false;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


void KMDefVarSubPage::OnReportValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if ( 2 == nIndex)
	{
		try
		{
			CXTPReportRecord*       pRecord      = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem*   pItemCode    = pRecord->GetItem(0);
			CXTPReportRecordItem*   pItemFunc    = pRecord->GetItem(2);

			int nCode        = (int)pItemCode->GetItemData();
			int nFunctionID  = (int)pItemFunc->GetItemData();

			int nCur         = m_cboODGroup.GetCurSel();
			KCodeGroup* pCodeGroup = (KCodeGroup*)m_cboODGroup.GetItemData(nCur);

			AutoType itFind = m_mapODGroupFunctionID.find(pCodeGroup);
			if (itFind == m_mapODGroupFunctionID.end())
				return;

			std::map<int, int> &mapFunctionID = itFind->second;
			AutoType itFindB = mapFunctionID.find(nCode);
			if (itFindB == mapFunctionID.end())
				return;

			int &nSavedFunctionID = itFindB->second;
			nSavedFunctionID = nFunctionID;
		}
		catch (...)
		{
			return;
		}
	}
}


void KMDefVarSubPage::OnCbnSelchangeComboFunction()
{
	ControlComboNoneGroupFunction();
}


void KMDefVarSubPage::ControlComboNoneGroupFunction()
{
	try
	{
		int nCurSel            = m_cboFunctionNoneGroup.GetCurSel();
		m_nNoneGroupFunctionID = (int)m_cboFunctionNoneGroup.GetItemData(nCurSel);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


HBRUSH KMDefVarSubPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(WHITE_BRUSH);

	return hbr;
} 
