// AImportDBDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AImportDBDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "DBaseAssignImport.h"
#include "ModeManager.h"
#include "Mode.h"
#include "DBaseAssignment.h"
#include "CustomReportRecordItem.h"
#include "KExRecordItem.h"
#include "Project.h"
#include "Scenario.h"

// KAImportDBDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAImportDBDlg, KResizeDialogEx)

KAImportDBDlg::KAImportDBDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KAImportDBDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{
	m_pImportTarget = nullptr;
	m_pImportProject = nullptr;
}

KAImportDBDlg::~KAImportDBDlg()
{
	if (m_pImportProject != nullptr)
	{
		QBicDelete(m_pImportProject);
	}
}

void KAImportDBDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndModeReport);
	DDX_Control(pDX, IDC_COMBO_SCENARIO, m_cboScenario);
	DDX_Control(pDX, IDC_COMBO_TARGET, m_cboTargetYear);
}

BEGIN_MESSAGE_MAP(KAImportDBDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &KAImportDBDlg::OnBnClickedButtonImport)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT1, OnReportCheckItemMode)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, OnReportChangeCombo)
	ON_BN_CLICKED(IDOK, &KAImportDBDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KAImportDBDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_SCENARIO, &KAImportDBDlg::OnCbnSelchangeComboScenario)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET, &KAImportDBDlg::OnCbnSelchangeComboTarget)
END_MESSAGE_MAP()


// KAImportDBDlg 메시지 처리기입니다.


BOOL KAImportDBDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndModeReport, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndModeReport);
	ResizeComponet();
	InitReportHeader();
	return TRUE; 
}

void KAImportDBDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(0,		_T("Select"),		20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(1,		_T("Freight_Mode"),	40));
	pColumn->SetHeaderAlignment(DT_CENTER);			

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(2,		_T("Passenger_Mode"),	40));
	pColumn->SetHeaderAlignment(DT_CENTER);			

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(3,		_T("Matrix"),		40));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(4,		_T("Column Name"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
}

void KAImportDBDlg::UpdateReportData()
{
	m_wndModeReport.ResetContent(TRUE);

	CXTPReportRecord*			 pRecord	  = nullptr;
	CXTPReportRecordItem*        pItem   	  = nullptr;
	CXTPReportRecordItemComboEx* pItemCombo   = nullptr;

	m_mapModeChocieColumns.clear();
	KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pImportTarget, m_mapModeChocieColumns);

	std::map<int, KFAModeInfo> mapFAModeInfo;
	KDBaseAssignImport::GetFmodeInfo( m_pTarget, mapFAModeInfo);

	KIOTable* pTable = m_pImportTarget->Tables()->FindTable(TABLE_MODE_OD);
	const KIOColumns* pColumns	= pTable->Columns();

	AutoType findend = m_mapModeChocieColumns.end();

	KModeManager* pModeManager = m_pImportTarget->ModeManager();
	int nModeCount = pModeManager->GetModeCount();
	for (int i = 0; i <nModeCount; i++)
	{
		KMode* pMode = pModeManager->GetMode(i);
		std::vector<CString> vecColumnName;

		AutoType find = m_mapModeChocieColumns.find(pMode->ModeID());
		if (find != findend)
		{
			vecColumnName = find->second;

			pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetChecked(TRUE);

			int nCurrentIndex(0);
			pItemCombo	= (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
			int nIndex	= pItemCombo->AddString(_T("<- New Mode ->"),(DWORD_PTR) 0);
			AutoType iter = mapFAModeInfo.begin();
			AutoType end  = mapFAModeInfo.end();
			while(iter != end)
			{
				KFAModeInfo oInfo = iter->second;
				if (oInfo.nSystemType == KEMPASSENGER_SYSTEM)
				{
					nIndex = pItemCombo->AddString(oInfo.strFomdeName, (DWORD_PTR)oInfo.nFmode_id);
				}
				if (oInfo.strFomdeName.CompareNoCase(pMode->ModeName()) == 0)
				{
					nCurrentIndex = nIndex;
				}
				++iter;
			}
			pItemCombo->SetCurSel(nCurrentIndex);
			pItemCombo->SetAlignment(xtpColumnTextCenter);
			pItemCombo->SetEditable(TRUE);


			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pMode->ModeName()));
			pItem->SetItemData((DWORD_PTR)pMode);
			pItem->SetAlignment(xtpColumnTextCenter);

			int nCboItemData = (int)pItemCombo->GetItemData(pItemCombo->GetCurSel());
			if (nCboItemData == 0)
			{
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);
			}
			else
			{
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}

			int nCurrentSelect(0);
			pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());

			size_t nColumnCount = vecColumnName.size();
			for (size_t i = 0; i <nColumnCount; i++)
			{
				CString strColunmName = vecColumnName[i];
				KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
				int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
				if (i == 0)
				{
					nCurrentSelect = nIndex;
				}
			}

			pItemCombo->SetAlignment(xtpColumnTextCenter);
			pItemCombo->SetBackgroundColor(RGB(255,255,255));
			pItemCombo->SetEditable(TRUE);
			pItemCombo->SetCurSel(nCurrentSelect);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pMode->ModeName()));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(255,255,255));
			pItem->SetEditable(TRUE);

		}
	}

	m_wndModeReport.Populate();
	m_wndModeReport.ExpandAll(TRUE);
}


void KAImportDBDlg::OnBnClickedButtonImport()
{
	try
	{
		m_cboScenario.ResetContent();
		m_cboTargetYear.ResetContent();
		ClearReportData();

		if (m_pImportProject != nullptr)
		{
			QBicDelete(m_pImportProject)
		}

		m_pImportProject = new KProject;

		if (SelectPassengerFile() == false) 
		{
			ClearReportData();
			return;
		}
		CString strRegLocation = m_strFileLocation.Left(m_strFileLocation.ReverseFind('\\'));

		m_pImportProject->SetLocation(strRegLocation);
		m_pImportProject->OpenProject(m_strFileLocation);

		InitScenarioCombo();
		initTargetCombo();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ClearReportData();
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		ClearReportData();
		return;
	}
} 

bool KAImportDBDlg::CreateImportTarget()
{
	try
	{
		m_pImportTarget->SetIODataFilePath(m_strFileLocation);
		
		if (m_pImportTarget->CreateOtherObject() == false)
		{
			AfxMessageBox(_T("데이터 생성 중 오류가 발생하였습니다."));
			return false;
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}
	return true;
}

bool KAImportDBDlg::SelectDBFile()
{
	try
	{
		CString strFilter(_T("Iolocal Files|iolocal.db|"));
		CFileDialog oFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST , strFilter);
		oFileDlg.m_ofn.lpstrTitle = _T("Select Passneger DB");
		if (m_strFileLocation.CompareNoCase(_T("")) == false)
		{
			oFileDlg.m_ofn.lpstrInitialDir = m_strFileLocation;	
		}

		if (oFileDlg.DoModal() == IDOK)
		{
			m_strFileLocation = oFileDlg.GetPathName();
			GetDlgItem(IDC_EDIT_FILE)->SetWindowTextW(m_strFileLocation);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}

	return true;
}

bool KAImportDBDlg::SelectPassengerFile()
{
	try
	{
		CString strFilter(_T("TOVA Project File (tpx)|*.tpx|"));
		CFileDialog oFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST , strFilter);
		oFileDlg.m_ofn.lpstrTitle = _T("Select Passneger File");
		if (m_strFileLocation.CompareNoCase(_T("")) == false)
		{
			oFileDlg.m_ofn.lpstrInitialDir = m_strFileLocation;	
		}

		if (oFileDlg.DoModal() == IDOK)
		{
			m_strFileLocation = oFileDlg.GetPathName();
			GetDlgItem(IDC_EDIT_FILE)->SetWindowTextW(m_strFileLocation);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}

	return true;
}


bool KAImportDBDlg::CheckDataFile()
{
	try
	{
		int nSystemCode(-1);
		KDBaseAssignImport::GetSystemCode(m_pImportTarget, nSystemCode);
		if (KmzSystem::IsFreightSystem())
		{
			if (nSystemCode == KEMFREIGHT_SYSTEM)
			{
				AfxMessageBox(_T("선택한 데이터는 현재 시스템의 데이터가 아닙니다."));
				return false;
			}
		}
		else
		{
			if (nSystemCode == KEMPASSENGER_SYSTEM)
			{
				AfxMessageBox(_T("선택한 데이터는 Freight의 데이터가 아닙니다."));
				return false;
			}
		}
		std::set<Integer> setCurrentZone;
		std::set<Integer> setImportZone;

		KDBaseAssignImport::GetZoneInfo(m_pTarget, setCurrentZone);
		KDBaseAssignImport::GetZoneInfo(m_pImportTarget, setImportZone);

		size_t nCurrentCount = setCurrentZone.size();
		size_t nImportCount  = setImportZone.size();

		if (nCurrentCount != nImportCount)
		{
			AfxMessageBox(_T("선택한 데이터와 기존 데이터의 존 갯수가 다릅니다."));
			return false;
		}

		AutoType iter = setCurrentZone.begin();
		AutoType end  = setCurrentZone.end();

		AutoType findend = setImportZone.end();

		while(iter != end)
		{
			Integer nxZoneID = *iter;

			AutoType find = setImportZone.find(nxZoneID);
			if (find == findend)
			{
				AfxMessageBox(_T("선택한 데이터와 기존 데이터의 존 노드 ID가 일치하지 않습니다."));
				return false;
			}
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}
	return true;
}

void KAImportDBDlg::OnReportCheckItemMode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if(nIndex == 0)
	{
		if(TRUE == pItemNotify->pRow->GetRecord()->GetItem(0)->IsChecked())
		{
			pItemNotify->pRow->GetRecord()->GetItem(2)->SetEditable(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(2)->SetBackgroundColor(RGB(255,255,255));
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(2)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(2)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
	}

	m_wndModeReport.RedrawControl();
}


void KAImportDBDlg::OnBnClickedOk()
{
	m_nResultCode = 1;

	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(AssignImportThreadCaller, this, true, nLang);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	KIOTable* pVehicleTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);
	pVehicleTable->Notify();

	if (m_nResultCode == 0)
	{
		AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));
	}
	else if (m_nResultCode == 1)
	{
		AfxMessageBox(m_strErrMsg);
		return;
	}
	else
	{
		AfxMessageBox(_T("데이터 불러오기에 실패하였습니다."));
		return;
	}

	KResizeDialogEx::OnOK();
}


void KAImportDBDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KAImportDBDlg::SaveData2DB()
{
	m_strErrMsg = _T("Error");
	std::vector<KAImportInfo> vecImportInfo;

	CString strADBName(_T("a"));
	CString strAttachSQL(_T(""));
	CString strDetachSQL(_T(""));
	strAttachSQL.Format(_T(" Attach DataBase '%s' As %s "), m_pImportTarget->GetIODataFilePath(), strADBName);
	strDetachSQL.Format(_T(" Detach DataBase '%s' "), strADBName);

	KDBaseConPtr spCurrentConnection = m_pTarget->GetDBaseConnection();

	std::vector<CString> vecCurrentPassengerColumn;

	try
	{
		if (GetMatchingData(vecImportInfo) == false)
		{
			m_nResultCode = 1;
			return;
		}

		if (vecImportInfo.size() < 1)
		{
			m_nResultCode = 1;
			m_strErrMsg.Format(_T("선택한데이터가 없습니다."));
			return;
		}

		spCurrentConnection->ExecuteUpdate(strAttachSQL);

		spCurrentConnection->BeginTransaction();
		
		KDBaseAssignImport::SetFModeModelTable(spCurrentConnection, vecImportInfo, vecCurrentPassengerColumn);
		KDBaseAssignImport::SetDetailFmodeTable(spCurrentConnection, vecImportInfo);
		KDBaseAssignImport::InsertImportData(spCurrentConnection, m_pImportTarget, vecImportInfo);

		spCurrentConnection->Commit();

		DeleteCurrentPassengerColumn(vecCurrentPassengerColumn);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spCurrentConnection->RollBack();
		DeleteCreateColumn(vecImportInfo);
		m_nResultCode = 1;
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		spCurrentConnection->RollBack();
		DeleteCreateColumn(vecImportInfo);
		m_nResultCode = 1;
		return;
	}

	try
	{
		spCurrentConnection->ExecuteUpdate(strDetachSQL);	
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}

	m_nResultCode = 0;
}

bool KAImportDBDlg::GetMatchingData( std::vector<KAImportInfo>& ar_vecImportInfo)
{
	ar_vecImportInfo.clear();

	try
	{
		int nMaxID(0);
		std::map<int, KFAModeInfo> mapFAModeInfo;
		KDBaseAssignImport::GetFmodeInfo( m_pTarget, mapFAModeInfo);
		AutoType iter = mapFAModeInfo.begin();
		AutoType end  = mapFAModeInfo.end();
		while(iter != end)
		{
			int nModeID = iter->first;
			if (nModeID > nMaxID)
			{
				nMaxID = nModeID;
			}
			++iter;
		}

		nMaxID = nMaxID + 1;
		
		if (nMaxID < 1000)
		{
			nMaxID = 1001;
		}


		KIOTable* pVehicleTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);


		CXTPReportRecords* pRecodes = m_wndModeReport.GetRecords();
		int nRecordCount = pRecodes->GetCount();

		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord	= pRecodes->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);
			if (pItem->IsChecked() == TRUE)
			{

				CXTPReportRecordItemComboEx* pCombo  = (CXTPReportRecordItemComboEx*)(pRecord->GetItem(1));
				int nModeID = (int)pCombo->GetItemData(pCombo->GetCurSel());

				CXTPReportRecordItemText* pTextItem = (CXTPReportRecordItemText*)(pRecord->GetItem(2));
				KMode*	pMode	    = (KMode*)pTextItem->GetItemData();
				CString strModeName = pTextItem->GetValue();

				pCombo  = (CXTPReportRecordItemComboEx*)(pRecord->GetItem(3));
				KIOColumn*	pImportColumn = (KIOColumn*)pCombo->GetItemData(pCombo->GetCurSel());

				CXTPReportRecordItemText* pCTextItem = (CXTPReportRecordItemText*)pRecord->GetItem(4);
				CString strColumnName = pCTextItem->GetValue();

				KIOColumn* pColumn = nullptr;
				CString strDiscription(_T(""));
				strDiscription.Format(_T("ImportColumn(%s)"), pMode->ModeName());
				if (CreateResultColumn(strColumnName, pColumn, pVehicleTable, strDiscription) == false)
				{
					return false;
				}
				
				KAImportInfo oInfo;
				oInfo.pCurrentColumn	= pColumn;
				oInfo.pImportColumn		= pImportColumn;
				oInfo.pImportMode		= pMode;
				if (nModeID == 0)
				{
					oInfo.bIsNewMode = true;
					oInfo.nModeID = nMaxID;

					AutoType iterName = mapFAModeInfo.begin();
					AutoType endName  = mapFAModeInfo.end();
					while(iterName != endName)
					{
						KFAModeInfo oFAModeInfo = iterName->second;
						if (strModeName.CompareNoCase(oFAModeInfo.strFomdeName) == 0)
						{
							m_strErrMsg.Format(_T("%s의 모드 이름이 이미 있습니다."), strModeName);
							return false;
						}
						iterName++;
					}

					oInfo.strModeName = strModeName;

				}
				else
				{
					oInfo.bIsNewMode = false;
					oInfo.nModeID = nMaxID;
				}

				nMaxID = nMaxID + 1;
				ar_vecImportInfo.push_back(oInfo);
			}
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



bool KAImportDBDlg::CreateResultColumn( CString a_strColumnName, KIOColumn*& a_pColumn, KIOTable* a_pTable , CString a_strDescription)
{
	try
	{
		const KIOColumns* pColumCollection = a_pTable->Columns();

		int nMaxDisplayCount(0);
		int nCount = pColumCollection->ColumnCount();

		if (0 == nCount)
		{
			nMaxDisplayCount = 1;
		}
		else
		{
			for (int i = 0; i < nCount; i++)
			{
				KIOColumn* pColumn = pColumCollection->GetColumn(i);
				int nDisplayCount = pColumn->DisplayOrder();
				if (nMaxDisplayCount < nDisplayCount)
				{
					nMaxDisplayCount = nDisplayCount;
				}

				if (a_strColumnName.CompareNoCase(pColumn->Caption()) == 0)
				{
					m_strErrMsg.Format(_T("%s의 컬럼 이름이 이미 있습니다."), a_strColumnName);
					return false;
				}
			}
		}

		a_pColumn = nullptr;

		KIOColumn oColumn;
		oColumn.Name(a_pTable->GetUsableColumnName());
		oColumn.Caption(a_strColumnName);
		oColumn.CodeType(KEMIOCodeTypeIsNothing);
		oColumn.ColumnType(KEMIOColumnTypeUserDefine);
		oColumn.DataType(KEMIODataTypeDouble);
		oColumn.DisplayOrder(nMaxDisplayCount+1);
		oColumn.ColumnInfo(a_strDescription);
		oColumn.PrimaryKey(false);
		oColumn.Visible(true);

		a_pColumn = a_pTable->AddColumn(oColumn);
		if (a_pColumn == nullptr)
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

void KAImportDBDlg::ClearReportData()
{
	m_wndModeReport.ResetContent(TRUE);
}

void KAImportDBDlg::DeleteCreateColumn( std::vector<KAImportInfo>& a_vecImportInfo )
{
	try
	{
		KIOTable* pVehicleTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);

		size_t nCount = a_vecImportInfo.size();
		for (size_t i =0; i <nCount; i++)
		{
			KAImportInfo oInfo = a_vecImportInfo[i];

			pVehicleTable->RemoveColumn(oInfo.pCurrentColumn->Name());
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

void KAImportDBDlg::DeleteCurrentPassengerColumn( std::vector<CString> a_vecPassengerColumn )
{
	try
	{
		KIOTable* pVehicleTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);

		size_t nCount = a_vecPassengerColumn.size();
		for (size_t i = 0; i <nCount; i++)
		{
			CString strColumn = a_vecPassengerColumn[i];
			pVehicleTable->RemoveColumn(strColumn);
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
		throw -1;
	}
}

void KAImportDBDlg::ResizeComponet()
{
	try
	{
		SetResize(IDC_STATIC_FILE,				  SZ_TOP_LEFT,    SZ_TOP_LEFT);
		SetResize(IDC_EDIT_FILE,				  SZ_TOP_LEFT,    SZ_TOP_LEFT);	
		SetResize(IDC_BUTTON_IMPORT,		      SZ_TOP_LEFT,  SZ_TOP_LEFT);

		SetResize(IDC_STATIC_SCENARIO,			  SZ_TOP_LEFT,  SZ_TOP_LEFT);
		SetResize(IDC_COMBO_SCENARIO,			  SZ_TOP_LEFT,   CXTPResizePoint(0.5, 0));
		SetResize(IDC_STATIC_TARGET,			  CXTPResizePoint(0.5, 0),  CXTPResizePoint(0.5, 0));
		SetResize(IDC_COMBO_TARGET,				  CXTPResizePoint(0.5, 0),   SZ_TOP_RIGHT);

		SetResize(IDC_SEPRATOR2,				  SZ_TOP_LEFT,    SZ_TOP_RIGHT);
		SetResize(IDC_REPORT1,					  SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR1,				  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_RIGHT);

		SetResize(IDOK,							  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);

	}
	catch(...)
	{
		TxLogDebugException();
	}
}

unsigned __stdcall KAImportDBDlg::AssignImportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 저장 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Saving Data..."));
	}
	KAImportDBDlg*        pDlg = (KAImportDBDlg*)pParameter->GetParameter();
	pDlg->SaveData2DB();

	return 0;
}

void KAImportDBDlg::AddStatusMessage( CString a_strMsg )
{

}

void KAImportDBDlg::InitScenarioCombo()
{
	m_cboScenario.ResetContent();
	try
	{
		int nIndex(0);
		int nfirstCur(0);

		KScenario* pBScenario = m_pImportProject->GetBaseScenario();
		nfirstCur = m_cboScenario.InsertString(nIndex, pBScenario->GetName());
		m_cboScenario.SetItemData(nfirstCur, (DWORD_PTR)pBScenario);
		nIndex++;

		const KTDScenarioMap& mapScenario = m_pImportProject->GetScenarioMap();
		KTDScenarioMap::const_iterator iter = mapScenario.begin();
		KTDScenarioMap::const_iterator end = mapScenario.end();
		while (iter != end)
		{
			KScenario* pSenairo = iter->second;
			CString strName = pSenairo->GetName();

			int nCur = m_cboScenario.InsertString(nIndex, strName);
			m_cboScenario.SetItemData(nCur, (DWORD_PTR)pSenairo);

			nIndex++;
			++iter;
		}

		m_cboScenario.SetCurSel(nfirstCur);
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

void KAImportDBDlg::initTargetCombo()
{
	m_cboTargetYear.ResetContent();

	try
	{
		int nScIndex = m_cboScenario.GetCurSel();
		KScenario* pScenario = (KScenario*)m_cboScenario.GetItemData(nScIndex);
		
		int nIndex(0);
		int nfirstCur(0);

		const KTDTargetMap& mapTarget = pScenario->GetTargetMap();
		KTDTargetMap::const_iterator iter = mapTarget.begin();
		KTDTargetMap::const_iterator end  = mapTarget.end();
		while (iter != end)
		{
			KTarget* pTarget = iter->second;
			CString strName = pTarget->GetName();

			int nCur = m_cboTargetYear.InsertString(nIndex, strName);
			m_cboTargetYear.SetItemData(nCur, (DWORD_PTR)pTarget);

			if (nIndex == 0)
			{
				nfirstCur = nCur;
			}

			nIndex++;
			++iter;
		}

		m_cboTargetYear.SetCurSel(nfirstCur);

		if (InitImportTarget() == true)
		{
			UpdateReportData();
		}
		else
		{
			ClearReportData();
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

bool KAImportDBDlg::InitImportTarget()
{
	try
	{
		int nScIndex = m_cboScenario.GetCurSel();
		if (nScIndex == -1)
		{
			return false;
		}

		int nTarIndex = m_cboTargetYear.GetCurSel();
		if (nTarIndex == -1)
		{
			return false;
		}

		m_pImportTarget = (KTarget*)m_cboTargetYear.GetItemData(nTarIndex);
		if (m_pImportTarget == nullptr)
		{
			return false;
		}

		if (CheckZoneData() == false)
		{
			return false;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KAImportDBDlg::CheckZoneData()
{
	try
	{
		std::set<Integer> setCurrentZone;
		std::set<Integer> setImportZone;

		KDBaseAssignImport::GetZoneInfo(m_pTarget, setCurrentZone);
		KDBaseAssignImport::GetZoneInfo(m_pImportTarget, setImportZone);

		size_t nCurrentCount = setCurrentZone.size();
		size_t nImportCount  = setImportZone.size();

		if (nCurrentCount != nImportCount)
		{
			AfxMessageBox(_T("선택한 데이터와 기존 데이터의 존 갯수가 다릅니다."));
			return false;
		}

		AutoType iter = setCurrentZone.begin();
		AutoType end  = setCurrentZone.end();

		AutoType findend = setImportZone.end();

		while(iter != end)
		{
			Integer nxZoneID = *iter;

			AutoType find = setImportZone.find(nxZoneID);
			if (find == findend)
			{
				AfxMessageBox(_T("선택한 데이터와 기존 데이터의 존 노드 ID가 일치하지 않습니다."));
				return false;
			}
			++iter;
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


void KAImportDBDlg::OnCbnSelchangeComboScenario()
{
	try
	{
		initTargetCombo();
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


void KAImportDBDlg::OnCbnSelchangeComboTarget()
{
	try
	{
		if (InitImportTarget() == true)
		{
			UpdateReportData();
		}
		else
		{
			ClearReportData();
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

void KAImportDBDlg::OnReportChangeCombo( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (nIndex == 1)
	{
		CXTPReportRecordItemComboEx* pItemCobo = (CXTPReportRecordItemComboEx*)pItemNotify->pItem;
		int nModeID = pItemCobo->GetItemData(pItemCobo->GetCurSel());
		CXTPReportRecord*           pRecordChanged   = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecordChanged->GetItem(2);

		if (nModeID == 0)
		{
			pItemText->SetBackgroundColor(RGB(255,255,255));
			pItemText->SetEditable(TRUE);
		}
		else
		{
			KMode* pMode = (KMode*)pItemText->GetItemData();
			pItemText->SetValue(pMode->ModeName());
			pItemText->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemText->SetEditable(FALSE);
		}
	}
}
