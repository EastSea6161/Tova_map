// KDefInterUrbanModeChoiceVariableSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KDefInterUrbanModeChoiceVariableSettingDlg.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include <algorithm>
#include <boost\variant.hpp>
//#include "CustomReportRecordItem.h"
#include "KExRecordItem.h"



// KDefInterUrbanModeChoiceVariableSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefInterUrbanModeChoiceVariableSettingDlg, KResizeDialogEx)

KDefInterUrbanModeChoiceVariableSettingDlg::KDefInterUrbanModeChoiceVariableSettingDlg(KTarget* pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(IDD_8511_Default_Inter_ModeChoiceVariableSettingDlg, pParent)
{
	m_pTarget = pTarget;

	m_pCurrentSelectedRecord = NULL;
	m_pLastSelectedCW = NULL;
}

KDefInterUrbanModeChoiceVariableSettingDlg::~KDefInterUrbanModeChoiceVariableSettingDlg()
{
}

void KDefInterUrbanModeChoiceVariableSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReport1);
	DDX_Control(pDX, IDC_REPORT2, m_wndReport2);
}


BEGIN_MESSAGE_MAP(KDefInterUrbanModeChoiceVariableSettingDlg, KResizeDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT2, &KDefInterUrbanModeChoiceVariableSettingDlg::OnReport2ItemChecked)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT2, &KDefInterUrbanModeChoiceVariableSettingDlg::OnReport2ItemValueChanged)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, IDC_REPORT2, &KDefInterUrbanModeChoiceVariableSettingDlg::OnReport2SelChanged)
	ON_BN_CLICKED(IDOK, &KDefInterUrbanModeChoiceVariableSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KDefInterUrbanModeChoiceVariableSettingDlg 메시지 처리기입니다.


void KDefInterUrbanModeChoiceVariableSettingDlg::OnSize(UINT nType, int cx, int cy)
{
	KResizeDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL KDefInterUrbanModeChoiceVariableSettingDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	KReportCtrlSetting::Default(m_wndReport1, TRUE);
	KReportCtrlSetting::Default(m_wndReport2, TRUE);

 
	ResizeComponet();

	InitUserZoneColumn();


	InitReport1();
	InitReport2();
	UpdateReport1();
	UpdateReport2();

	m_wndReport1.ExpandAll(TRUE);
	m_wndReport2.ExpandAll(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void KDefInterUrbanModeChoiceVariableSettingDlg::SetDescriptionVariableInfo(std::map<int, TDescriptionVariableInfo>& mapVariableInfo)
{
	m_mapVariableInfo = mapVariableInfo;
}


void KDefInterUrbanModeChoiceVariableSettingDlg::SetZoneVariableInfo(std::map<int, TZoneVariableInfo>& mapZoneVariableInfo)
{
	m_mapZoneVariableInfo = mapZoneVariableInfo;
}


void KDefInterUrbanModeChoiceVariableSettingDlg::SetUserZoneColumnInfo(std::map<int, TZoneUserColumnInfo>& mapZoneColumn)
{
	KIOTables* pTables = m_pTarget->Tables();
	ASSERT(NULL != pTables);

	KIOTable* pTable = pTables->FindTable(TABLE_ZONE);
	if (NULL == pTable)
	{
		return;
	}
	KIOColumns* pColumns = pTable->Columns();
	int nColCount = pColumns->ColumnCount();

	for (auto &aZUC : mapZoneColumn)
	{
		for (int i = 0; i < nColCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			CString strCaption = pColumn->Caption();
			if (aZUC.second.strDisplayName.CompareNoCase(strCaption) == 0)
			{
				STIOColumnWrapper sc;
				sc.pColumn = pColumn;
				sc.bSelected = false;
				m_mapUserZoneColumn.insert({ strCaption, sc });
			}
		}
	}
}



std::vector<TDescriptionVariableInfo> KDefInterUrbanModeChoiceVariableSettingDlg::GetSelectedVariables()
{
	return m_arrSelectedVariable;
}


std::vector<TZoneVariableInfo> KDefInterUrbanModeChoiceVariableSettingDlg::GetSelectedZoneVariables()
{
	return m_arrSelectedZoneVariable;
}



void KDefInterUrbanModeChoiceVariableSettingDlg::ResizeComponet()
{
	SetResize(IDC_STATIC_MEU, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_REPORT1, SZ_TOP_LEFT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_MEU2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_SEPRATOR1, SZ_TOP_LEFT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_MEU3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_REPORT2, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR2, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);


	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KDefInterUrbanModeChoiceVariableSettingDlg::InitReport1()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReport1.AddColumn(new CXTPReportColumn(0, _T("Variable Type"), 100));
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("변수명"));
		}
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetTreeColumn(TRUE);

		pColumn = m_wndReport1.AddColumn(new CXTPReportColumn(1, _T("Unit"), 30));
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("단위"));
		}
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReport1.AddColumn(new CXTPReportColumn(2, _T("Description"), 200));
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("변수 설명"));
		}
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDefInterUrbanModeChoiceVariableSettingDlg::InitReport2()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		/// Report 2 초기화
		pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(0, _T("Variable Type"), 100));
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("변수명"));
		}
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetTreeColumn(TRUE);

		pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(1, _T("Column"), 50));
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("컬럼 선택"));
		}
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(2, _T("Description"), 200));
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("변수 설명"));
		}
		pColumn->SetHeaderAlignment(DT_CENTER);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDefInterUrbanModeChoiceVariableSettingDlg::UpdateReport1()
{
	CXTPReportRecord* pParentRecord = nullptr;
	CXTPReportRecordItem* pItemOfParent = nullptr;
	CXTPReportRecords* pRecordsOfParent = nullptr;
	CXTPReportRecord* pRecord = nullptr;

	std::map<int, CXTPReportRecord*> mapParentRecord;

	m_wndReport1.ResetContent();

	for(std::map<int, TDescriptionVariableInfo>::iterator it = m_mapVariableInfo.begin(); 
		it != m_mapVariableInfo.end(); ++it)
	{
		TDescriptionVariableInfo& info = it->second;

		std::map<int, CXTPReportRecord*>::iterator itRecord = mapParentRecord.find(info.nType);
		if (mapParentRecord.end() == itRecord)
		{
			pParentRecord = m_wndReport1.AddRecord(new CXTPReportRecord());
			pItemOfParent = pParentRecord->AddItem(new CXTPReportRecordItemText(info.strTypeName));
			pItemOfParent->SetEditable(FALSE);
			mapParentRecord.insert({ info.nType, pParentRecord });

			pParentRecord->AddItem(new CXTPReportRecordItem())->SetEditable(FALSE);;
			pParentRecord->AddItem(new CXTPReportRecordItem())->SetEditable(FALSE);;
		}
		else
		{
			pParentRecord = itRecord->second;
		}
		pRecordsOfParent = pParentRecord->GetChilds();
		pRecord = pRecordsOfParent->Add(new CXTPReportRecord());
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText(info.strVariableName));
		pItem->SetAlignment(DT_LEFT);
		pItem->HasCheckbox(TRUE);
		pItem->SetItemData(info.nId);
		if (info.bSelected)
		{
			pItem->SetChecked(TRUE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(info.strVariableUnit));
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(info.strDescript));
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);

	}
	
	m_wndReport1.Populate();

}

void KDefInterUrbanModeChoiceVariableSettingDlg::UpdateReport2()
{
	CXTPReportRecord* pParentRecord = nullptr;
	CXTPReportRecordItem* pItemOfParent = nullptr;
	CXTPReportRecords* pRecordsOfParent = nullptr;
	CXTPReportRecord* pRecord = nullptr;

	std::map<int, CXTPReportRecord*> mapParentRecord;

	m_wndReport2.ResetContent();

	for (std::map<int, TZoneVariableInfo>::iterator it = m_mapZoneVariableInfo.begin();
		it != m_mapZoneVariableInfo.end(); ++it)
	{
		TZoneVariableInfo& info = it->second;

		std::map<int, CXTPReportRecord*>::iterator itRecord = mapParentRecord.find(info.nType);
		if (mapParentRecord.end() == itRecord)
		{
			pParentRecord = m_wndReport2.AddRecord(new CXTPReportRecord());
			pItemOfParent = pParentRecord->AddItem(new CXTPReportRecordItemText(info.strTypeName));
			pItemOfParent->SetEditable(FALSE);
			mapParentRecord.insert({ info.nType, pParentRecord });
			pParentRecord->AddItem(new CXTPReportRecordItem())->SetEditable(FALSE);
			pParentRecord->AddItem(new CXTPReportRecordItem())->SetEditable(FALSE);
		}
		else
		{
			pParentRecord = itRecord->second;
		}

		pRecordsOfParent = pParentRecord->GetChilds();
		pRecord = pRecordsOfParent->Add(new CXTPReportRecord());
		CString strCaption = _T("");
		CString strDesc = _T("-");
		if (info.strZoneColDispName.IsEmpty())
		{
			strCaption = info.strVariableName + _T(":[-]");
		}
		else
		{
			strCaption = info.strVariableName + _T(":[") + info.strZoneColDispName + _T("]");
		}
		
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
		pItem->SetAlignment(DT_LEFT);
		pItem->HasCheckbox(TRUE);
		pItem->SetItemData(info.nId);

		
		//CXTPReportRecordItem* pColumnItem = pRecord->AddItem(new CCustomReportRecordItem());
		//pColumnItem->SetAlignment(DT_CENTER);
		if (info.bSelected)
		{
			pItem->SetChecked(TRUE);
			CXTPReportRecordItemComboEx* pItemColumn = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
			pItemColumn->SetAlignment(xtpColumnTextCenter);
			int nSelIndex = 0;
			int nIdx = 0;
			for (auto &aZUC : m_mapUserZoneColumn)
			{
				if (!aZUC.second.bSelected)
				{
					pItemColumn->AddString(aZUC.first, (DWORD_PTR)&aZUC.second);
					++nIdx;
				}
				else if (info.strZoneColDispName.CompareNoCase(aZUC.first) == 0)
				{
					nSelIndex = nIdx++;
					pItemColumn->AddString(aZUC.first, (DWORD_PTR)&aZUC.second);
					strDesc = aZUC.second.pColumn->ColumnInfo();
				}
			}
			pItemColumn->SetCurSel(nSelIndex);
		}
		else
		{
			pItem->SetChecked(FALSE);
			CXTPReportRecordItemComboEx* pItemColumn = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
			pItemColumn->AddString(_T("None"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemColumn->AddString(_T("해당 없음"));
			}
			pItemColumn->SetCurSel(0);
			pItemColumn->SetAlignment(xtpColumnTextCenter);
			pItemColumn->SetEditable(FALSE);
		}

		CXTPReportRecordItem* pDescItem = pRecord->AddItem(new CXTPReportRecordItemText(strDesc));
		pDescItem->SetAlignment(DT_CENTER);
		pDescItem->SetEditable(FALSE);
	}

	UpdateReport2CheckboxState();
	m_wndReport2.Populate();
}


void KDefInterUrbanModeChoiceVariableSettingDlg::OnReport2ItemChecked(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	try
	{
		BOOL bChecked = pItemNotify->pItem->IsChecked();
		CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItemComboEx* pItemColumn = (CXTPReportRecordItemComboEx*)pRecord->GetItem(1);
		
		if (NULL != pItemColumn)
		{
			if (TRUE == bChecked)
			{
				pItemColumn->ResetContent();
				for (auto &aZUC : m_mapUserZoneColumn)
				{
					if (!aZUC.second.bSelected)
					{
						pItemColumn->AddString(aZUC.first, (DWORD_PTR)&aZUC.second);
					}
				}
				pItemColumn->SetCurSel(0);
				pItemColumn->SetEditable(TRUE);
				STIOColumnWrapper* pCW = (STIOColumnWrapper*)pItemColumn->GetItemData(pItemColumn->GetCurSel());
				pCW->bSelected = true;
				m_pLastSelectedCW = pCW;
			}
			else
			{
				STIOColumnWrapper* pCW = (STIOColumnWrapper*)pItemColumn->GetItemData(pItemColumn->GetCurSel());
				if (NULL != pCW)
				{
					pCW->bSelected = false;
					m_pLastSelectedCW = pCW;
				}

				pItemColumn->ResetContent();
				pItemColumn->AddString(_T("None"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pItemColumn->AddString(_T("해당 없음"));
				}
				pItemColumn->SetCurSel(0);
				pItemColumn->SetEditable(FALSE);
				pRecord->GetItem(2)->SetCaption(_T("-"));
			}
			UpdateZoneVarName(pRecord);
		}

		UpdateReport2CheckboxState();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDefInterUrbanModeChoiceVariableSettingDlg::OnReport2ItemValueChanged(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == 1)
		{
			if (NULL != m_pLastSelectedCW)
			{
				m_pLastSelectedCW->bSelected = false;
				m_pLastSelectedCW = NULL;
			}

			CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemComboEx* pItemColumn = (CXTPReportRecordItemComboEx*)pRecord->GetItem(1);

			STIOColumnWrapper* pCW = (STIOColumnWrapper*)pItemColumn->GetItemData(pItemColumn->GetCurSel());
			pCW->bSelected = true;
			m_pLastSelectedCW = pCW;

			UpdateZoneVarName(pRecord);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDefInterUrbanModeChoiceVariableSettingDlg::OnReport2SelChanged(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	CXTPReportRow* pRow = m_wndReport2.GetFocusedRow();
	if (NULL != pRow)
	{
		CXTPReportRecord* pRecord = pRow->GetRecord();
		if (m_pCurrentSelectedRecord == pRecord)
		{
			return;
		}

		if (NULL != m_pLastSelectedCW)
		{
			m_pLastSelectedCW = NULL;
		}

		m_pCurrentSelectedRecord = pRecord;
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		if ((pItem->GetHasCheckbox() == TRUE) && (pItem->IsChecked() == TRUE))
		{
			CXTPReportRecordItemComboEx* pItemColumn = (CXTPReportRecordItemComboEx*)pRecord->GetItem(1);
			STIOColumnWrapper* pCW = NULL;
			pCW = (STIOColumnWrapper*)pItemColumn->GetItemData(pItemColumn->GetCurSel());

			pItemColumn->ResetContent();
			int nSelIndex = 0;
			int nIndex = 0;
			for (auto &aZUC : m_mapUserZoneColumn)
			{
				if (aZUC.second.bSelected)
				{
					if ((NULL != pCW) && (pCW->pColumn == aZUC.second.pColumn))
					{
						nSelIndex = nIndex++;
						pItemColumn->AddString(aZUC.first, (DWORD_PTR)&aZUC.second);
					}
				}
				else
				{
					pItemColumn->AddString(aZUC.first, (DWORD_PTR)&aZUC.second);
					nIndex++;
				}
			}
			pItemColumn->SetCurSel(nSelIndex);
			m_pLastSelectedCW = (STIOColumnWrapper*)pItemColumn->GetItemData(pItemColumn->GetCurSel());
			m_pLastSelectedCW->bSelected = true;

		}
	}
}


void KDefInterUrbanModeChoiceVariableSettingDlg::InitUserZoneColumn()
{
	for (auto &aZV : m_mapZoneVariableInfo)
	{
		if (aZV.second.bSelected)
		{
			std::map<CString, STIOColumnWrapper>::iterator itCW = m_mapUserZoneColumn.find(aZV.second.strZoneColDispName);
			if (m_mapUserZoneColumn.end() != itCW)
			{
				itCW->second.bSelected = true;
			}
		}
	}
}


void KDefInterUrbanModeChoiceVariableSettingDlg::MakeSelectedVariables()
{
	m_arrSelectedVariable.clear();

	CXTPReportRecord* pParentRecord = nullptr;
	CXTPReportRecordItem* pItemOfParent = nullptr;
	CXTPReportRecords* pRecordsOfParent = nullptr;
	CXTPReportRecord* pRecord = nullptr;

	CXTPReportRecords* pRoot = m_wndReport1.GetRecords();
	if (nullptr == pRoot)
	{
		return;
	}

	int nRootRecordCount = pRoot->GetCount();
	for (int i = 0; i < nRootRecordCount; ++i)
	{
		pParentRecord = pRoot->GetAt(i);
		pItemOfParent = pParentRecord->GetItem(0);
		pRecordsOfParent = pParentRecord->GetChilds();

		int nRecordCount = pRecordsOfParent->GetCount();
		for (int j = 0; j < nRecordCount; ++j)
		{
			pRecord = pRecordsOfParent->GetAt(j);
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);

			if (pItem->IsChecked())
			{
				int nId = pItem->GetItemData();
				std::map<int, TDescriptionVariableInfo>::iterator it = m_mapVariableInfo.find(nId);
				if (m_mapVariableInfo.end() != it)
				{
					m_arrSelectedVariable.push_back(it->second);
				}
			}
		}
	}
}


void KDefInterUrbanModeChoiceVariableSettingDlg::MakeSelectedZoneVariables()
{
	m_arrSelectedZoneVariable.clear();

	CXTPReportRecord* pParentRecord = nullptr;
	CXTPReportRecordItem* pItemOfParent = nullptr;
	CXTPReportRecords* pRecordsOfParent = nullptr;
	CXTPReportRecord* pRecord = nullptr;

	CXTPReportRecords* pRoot = m_wndReport2.GetRecords();
	if (nullptr == pRoot)
	{
		return;
	}
	pParentRecord = pRoot->GetAt(0);
	pRecordsOfParent = pParentRecord->GetChilds();
	
	int nRecordCount = pRecordsOfParent->GetCount();
	for (int i = 0; i < nRecordCount; ++i)
	{
		pRecord = pRecordsOfParent->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		CString strVarName = pItem->GetCaption();
		
		if (pItem->IsChecked())
		{
			CXTPReportRecordItemComboEx* pColumnName = (CXTPReportRecordItemComboEx*)pRecord->GetItem(1);
			STIOColumnWrapper* pCW = (STIOColumnWrapper*)pColumnName->GetItemData(pColumnName->GetCurSel());
			if (NULL != pCW)
			{
				int nId = pItem->GetItemData();
				std::map<int, TZoneVariableInfo>::iterator it = m_mapZoneVariableInfo.find(nId);
				if (m_mapZoneVariableInfo.end() != it)
				{
					TZoneVariableInfo& info = it->second;
					info.strZoneColName = pCW->pColumn->Name();
					info.strZoneColDispName = pCW->pColumn->Caption();
					info.nZoneColDataType = pCW->pColumn->DataType();
					info.nZoneColCodeType = -1;
					info.strZoneColDesc = pCW->pColumn->ColumnInfo();

					m_arrSelectedZoneVariable.push_back(it->second);
				}
			}
		}
	}
}


void KDefInterUrbanModeChoiceVariableSettingDlg::UpdateZoneVarName(CXTPReportRecord* pRecord)
{
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);
	CXTPReportRecordItemComboEx* pItemColName = (CXTPReportRecordItemComboEx*)pRecord->GetItem(1);
	CXTPReportRecordItem* pItemDesc = pRecord->GetItem(2);

	if (pItem->GetHasCheckbox() == FALSE)
	{
		return;
	}

	CString strVarName;
	CString strDesc = _T("-");

	// 변수명 갱신
	int nVarId = pItem->GetItemData();
	std::map<int, TZoneVariableInfo>::iterator itVar = m_mapZoneVariableInfo.find(nVarId);

	if (m_mapZoneVariableInfo.end() != itVar)
	{
		strVarName = itVar->second.strVariableName + _T(":[");
	}

	STIOColumnWrapper* pCW = (STIOColumnWrapper*)pItemColName->GetItemData(pItemColName->GetCurSel());
	if (NULL != pCW)
	{
		CString strCaption = pCW->pColumn->Caption();
		strVarName += (strCaption + _T("]"));
		strDesc = pCW->pColumn->ColumnInfo();
	}
	else
	{
		strVarName += _T("-]");
	}

	pItem->SetCaption(strVarName);
	pItemDesc->SetCaption(strDesc);
}


//
//
//int KDefInterUrbanModeChoiceVariableSettingDlg::GetZoneColumnId(CString& str)
//{
//	int idx = 0;
//	for (std::map<int, TZoneUserColumnInfo>::iterator it = m_mapUserZoneColumnInfo.begin();
//		it != m_mapUserZoneColumnInfo.end(); ++it)
//	{
//		TZoneUserColumnInfo& info = it->second;
//		if (info.strDisplayName == str)
//		{
//			return info.nId;
//		}
//	}
//
//	return -1;
//}



void KDefInterUrbanModeChoiceVariableSettingDlg::OnBnClickedOk()
{
	MakeSelectedVariables();
	MakeSelectedZoneVariables();	

	KResizeDialogEx::OnOK();
}


//
//
//void KDefInterUrbanModeChoiceVariableSettingDlg::UpdateSelectableColumnMap()
//{
//	for (auto &aZV : m_mapZoneVariableInfo)
//	{
//		std::map<CString, TZoneUserColumnInfo>::iterator itCol = m_mapSelectableZoneColumn.find(aZV.second.strZoneColDispName);
//		if (m_mapSelectableZoneColumn.end() != itCol)
//		{
//			m_mapSelectableZoneColumn.erase(itCol);
//		}
//	}
//}
//
//
//
void KDefInterUrbanModeChoiceVariableSettingDlg::UpdateReport2CheckboxState()
{
	BOOL bEnable = FALSE;
	for (auto &aUZC : m_mapUserZoneColumn)
	{
		if (!aUZC.second.bSelected)
		{
			bEnable = TRUE;
		}
	}

	CXTPReportRecords* pParentRecords = m_wndReport2.GetRecords();
	int nParentCount = pParentRecords->GetCount();
	for (int i = 0; i < nParentCount; ++i)
	{
		CXTPReportRecord* pParentRecord = pParentRecords->GetAt(i);
		CXTPReportRecords* pChildRecords = pParentRecord->GetChilds();
		int nChildCount = pChildRecords->GetCount();
		for (int j = 0; j < nChildCount; ++j)
		{
			CXTPReportRecord* pRecord = pChildRecords->GetAt(j);
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);
			if ((pItem->GetHasCheckbox() == TRUE) && (pItem->IsChecked() == FALSE))
			{
				pItem->SetEditable(bEnable);
			}
		}
	}
}

//void KDefInterUrbanModeChoiceVariableSettingDlg::UpdateColumnComboList()
//{
//	CXTPReportColumn* pColumn = m_wndReport2.GetColumns()->GetAt(1);
//	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
//	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
//	pConstraints->RemoveAll();
//
//	for (auto &aZUC : m_mapSelectableZoneColumn)
//	{
//		pEditOptions->AddConstraint(aZUC.second.strDisplayName, aZUC.second.nId);
//	}
//	m_wndReport2.RedrawControl();
//}
