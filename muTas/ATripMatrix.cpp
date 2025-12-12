// TripMatrix.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ATripMatrix.h"
#include "afxdialogex.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseAssignment.h"
#include "KExRecordItem.h"
#include "Mode.h"
#include "ModeManager.h"
#include "CustomReportRecordItem.h"
#include "AInputInfo.h"

enum ModeInfo
{
	_0_COLUMN_SELECT		= 0,
	_1_COLUMN_MODE,
	_2_COLUMN_MATRIX,
	_3_COLUMN_OCCUPANCY,    
	_4_COLUMN_PCE,			
	_5_COLUMN_NETWORKTYPE,
	_6_COLUMN_PRELOAD
};

// KTripMatrix 대화 상자입니다.

IMPLEMENT_DYNAMIC(KATripMatrix, KResizeDialogEx)

KATripMatrix::KATripMatrix(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KATripMatrix::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KATripMatrix::~KATripMatrix() 
{
}

void KATripMatrix::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT, m_wndModeReport);
	DDX_Control(pDX, IDC_EDIT_TIME, m_edtAnalysisTime);
}

BEGIN_MESSAGE_MAP(KATripMatrix, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KATripMatrix::OnBnClickedOk)
	ON_BN_CLICKED(IDDEFAULT, &KATripMatrix::OnBnClickedDefault)
	ON_BN_CLICKED(IDCANCEL, &KATripMatrix::OnBnClickedCancel)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItemMode)
END_MESSAGE_MAP()

BOOL KATripMatrix::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndModeReport, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndModeReport);

	InitModeReportHeader();

	if (KmzSystem::IsPassengerSystem())
	{
		ModeReportRecord();
	}
	else
	{
		FModeReportRecord();
	}

	ModeSetting();

	ResizeComponent();

	return TRUE;
}

KEMAssignmentODType KATripMatrix::AssignmentODType(void)
{
	return m_emODType;
}

void KATripMatrix::AssignmentODType(KEMAssignmentODType emODType)
{
	m_emODType = emODType;
}

double KATripMatrix::AnalysisTime()
{
	return m_dAnalysisTime;
}

void KATripMatrix::AnalysisTime(double dTime)
{
	m_dAnalysisTime = dTime;
}

void KATripMatrix::SetModel( KEMAssignmentModel a_emAModel )
{
	m_emAModel = a_emAModel;
}

std::list<KAInputInfo*> KATripMatrix::InputInfo()
{
	return m_lstInputInfo;
}

void KATripMatrix::InitModeReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = NULL;
		pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT,		_T("Select"),		20));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_1_COLUMN_MODE,		_T("Mode"),			40));
		pColumn->SetHeaderAlignment(DT_CENTER);			
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("도로통행수단"));
        }

		pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_2_COLUMN_MATRIX,		_T("Matrix"),		40));
		pColumn->SetHeaderAlignment(DT_CENTER);			
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("O/D 데이터 컬럼"));
        }

		pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_3_COLUMN_OCCUPANCY,	_T("Occupancy"),	40));
		pColumn->SetHeaderAlignment(DT_CENTER);		
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("평균 재차인원"));
        }

		pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_4_COLUMN_PCE,			_T("PCE"),			40));
		pColumn->SetHeaderAlignment(DT_CENTER);		
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("승용차 환산계수"));
        }

		pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_5_COLUMN_NETWORKTYPE, _T("Network Type"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);			
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
		pColumn->SetVisible(FALSE);
		 
		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		pEditOptions->AddConstraint(_T("Highway"), 1);
		 
		if (m_emAModel == KEMAssignmentModelMultiModal)
		{
			pEditOptions->AddConstraint(_T("Transit Line"), 2);
			pEditOptions->AddConstraint(_T("Rail"), 3);
		}

		pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_6_COLUMN_PRELOAD, _T("PreLoad"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("사전 배정"));
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

void KATripMatrix::ModeReportRecord()
{
	try
	{
		m_wndModeReport.ResetContent(TRUE);

		CXTPReportRecords*			 pRecords	  = m_wndModeReport.GetRecords();
		CXTPReportRecord*			 pRecord	  = nullptr;
		CXTPReportRecordItem*        pItem   	  = nullptr;
		CXTPReportRecordItemComboEx* pItemCombo   = nullptr;
		KMode*						 pMode	      = nullptr;
		KIOColumn*					 pInputColumn = nullptr;
		CString						 strModeName  (_T(""));
		double						 dOccupancy   (1.0);
		double						 dPCE		  (1.0);
		bool						 bPreLoad	  = false;
		bool						 bSelected	  = false;

		KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pTarget, m_mapModeChocieColumns);
		AutoType iterModeColumns = m_mapModeChocieColumns.begin();
		AutoType  endModeColumns = m_mapModeChocieColumns.end();

		std::list<KAInputInfo*>::iterator iterInput; 

		AutoType endInput  = m_lstInputInfo.end();
	
		for(iterInput = m_lstInputInfo.begin(); iterInput != endInput; ++iterInput)
		{
			KAInputInfo* pInput	= *iterInput;
			bSelected			= pInput->Selected();
			pMode				= pInput->Mode();
			strModeName		    = pMode->ModeName();
			pInputColumn		= pInput->InputColumn();
			dOccupancy			= pInput->Occupancy();
			dPCE				= pInput->PCU();
			bPreLoad			= pInput->PreLoad();

			pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);
			
			std::vector<CString> vecModeColumns;
			AutoType findColumn = m_mapModeChocieColumns.find(pMode->ModeID());
			AutoType endColumne = m_mapModeChocieColumns.end();
			if (findColumn != endModeColumns)
			{
				vecModeColumns = findColumn->second;
			}

			size_t nColumnCount = vecModeColumns.size();

			if(bSelected == true)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->SetItemData((DWORD_PTR)pInput);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				if (nColumnCount < 1)
				{
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}
				else
				{
					pItem->SetEditable(TRUE);
					pItem->SetChecked(TRUE);
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
				pItem->SetItemData((DWORD_PTR)pMode);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(TRUE);

				pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
				KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
				const KIOColumns* pColumns	= pTable->Columns();

				int nIndex       = 0;
				int nCurrentIndex   = 0;
				for (size_t i = 0; i < nColumnCount; i++)
				{
					CString strColunmName = vecModeColumns[i];
					KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
					nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);

					CString strCompareColumnName = pColumn->Caption();
					CString strCompareInputName = pInput->InputColumn()->Caption();

					if(strCompareColumnName == strCompareInputName)
					{
						nCurrentIndex = nIndex;
					}
				}

				pItemCombo->SetCurSel(nCurrentIndex);
				pItemCombo->SetAlignment(xtpColumnTextCenter);
				pItemCombo->SetBackgroundColor(RGB(255,255,255));
				pItemCombo->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
				pItem->SetItemData(pInput->NetworkType());

				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetEditable(TRUE);
				if(bPreLoad == true)
				{
					pItem->SetChecked(TRUE);
				}	
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetItemData((DWORD_PTR)pInput);
				pItem->SetAlignment(xtpColumnIconCenter);
				if (nColumnCount < 1)
				{
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}
				else
				{
					pItem->SetEditable(TRUE);
					pItem->SetChecked(FALSE);
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
				pItem->SetItemData((DWORD_PTR)pMode);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
				KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
				const KIOColumns* pColumns	= pTable->Columns();

				int nSelIndex       = 0;
				for (size_t i = 0; i < nColumnCount; i++)
				{
					CString strColunmName = vecModeColumns[i];
					KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
					int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
				}
				pItemCombo->SetCurSel(nSelIndex);
				pItemCombo->SetAlignment(xtpColumnTextCenter);
				pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItemCombo->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
				pItem->SetItemData(pInput->NetworkType());

				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(FALSE);
				pItem->SetEditable(FALSE);
			}
			
		}
		m_wndModeReport.Populate();
		m_wndModeReport.ExpandAll(TRUE);
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

void KATripMatrix::GetDefaultModeInfo(int a_nMasterCode, std::map<int, std::vector<KOccupancyPCEMode>>& a_mapGetDefaultModeInfo)
{
	try
	{
		std::map<int, std::vector<KOccupancyPCEMode>> mapDefaultOccupancyPCEMode;
		KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(mapDefaultOccupancyPCEMode);

		AutoType iterDefaultMode = mapDefaultOccupancyPCEMode.find(a_nMasterCode);
		AutoType endDefaultMode  = mapDefaultOccupancyPCEMode.end();
		std::vector<KOccupancyPCEMode>& vecOccupancyPCEMode = iterDefaultMode->second;
		if(iterDefaultMode != endDefaultMode)
		{
			size_t nDefaultModeCount = vecOccupancyPCEMode.size();
			for(size_t i = 0; i < nDefaultModeCount; i++)
			{
				KOccupancyPCEMode oOccupancyPCEMode = vecOccupancyPCEMode[i];
				int				  nModeID			= oOccupancyPCEMode.nModeID;
				oOccupancyPCEMode.dOccupancy		= oOccupancyPCEMode.dOccupancy;
				oOccupancyPCEMode.dPCE				= oOccupancyPCEMode.dPCE;

				std::vector<KOccupancyPCEMode> vecModeInfo;
				vecModeInfo.push_back(oOccupancyPCEMode);

				a_mapGetDefaultModeInfo.insert(std::make_pair(nModeID, vecModeInfo));
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

void KATripMatrix::DefaultModeReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndModeReport.ResetContent(TRUE);

		CXTPReportRecords*			 pRecords	  = m_wndModeReport.GetRecords();
		CXTPReportRecord*			 pRecord	  = nullptr;
		CXTPReportRecordItem*        pItem   	  = nullptr;
		CXTPReportRecordItemComboEx* pItemCombo   = nullptr;
		KMode*						 pMode	      = nullptr;
		KIOColumn*					 pInputColumn = nullptr;
		CString						 strModeName  (_T(""));
		double						 dOccupancy   (1.0);
		double						 dPCE		  (1.0);
		bool						 bPreLoad	  = false;
		bool						 bSelected	  = false;

		GetDefaultModeInfo(a_nMasterCode, m_mapGetDefaultModeInfo);

		KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pTarget, m_mapModeChocieColumns);
		AutoType iterModeColumns = m_mapModeChocieColumns.begin();
		AutoType  endModeColumns = m_mapModeChocieColumns.end();

		std::list<KAInputInfo*>::iterator iterInput; 

		AutoType endInput  = m_lstInputInfo.end();

		for(iterInput = m_lstInputInfo.begin(); iterInput != endInput; ++iterInput)
		{
			KAInputInfo* pInput	= *iterInput;
			bSelected			= pInput->Selected();
			pMode				= pInput->Mode();
			strModeName		    = pMode->ModeName();
			pInputColumn		= pInput->InputColumn();
			dOccupancy			= pInput->Occupancy();
			dPCE				= pInput->PCU();
			bPreLoad			= pInput->PreLoad();

			pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);

			if(bSelected == true)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetItemData((DWORD_PTR)pInput);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
				pItem->SetItemData((DWORD_PTR)pMode);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(TRUE);

				pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
				KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
				const KIOColumns* pColumns	= pTable->Columns();

				if(iterModeColumns != endModeColumns)
				{
					std::vector<CString> vecModeColumns = iterModeColumns->second;
					size_t nColumnCount = vecModeColumns.size();
					int nIndex       = 0;
					for (size_t i = 0; i < nColumnCount; i++)
					{
						CString strColunmName = vecModeColumns[i];
						KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
						int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
					}
					pItemCombo->SetCurSel(nIndex);
					pItemCombo->SetAlignment(xtpColumnTextCenter);
					pItemCombo->SetBackgroundColor(RGB(255,255,255));
					pItemCombo->SetEditable(TRUE);
				}
				int nModeID = pMode->ModeID();

				AutoType iterDefaultMode = m_mapGetDefaultModeInfo.find(nModeID);
				AutoType  endDefaultMode = m_mapGetDefaultModeInfo.end();
				if(iterDefaultMode != endDefaultMode)
				{
					std::vector<KOccupancyPCEMode> vecDefaultModeInfo = iterDefaultMode->second;
					KOccupancyPCEMode oOccupancyPCEMode = vecDefaultModeInfo[0];
					dOccupancy							= oOccupancyPCEMode.dOccupancy;
					dPCE								= oOccupancyPCEMode.dPCE;
				}
				else
				{
					dOccupancy = 1.0;
					dPCE	   = 1.0;
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
				pItem->SetItemData(pInput->NetworkType());

				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetEditable(TRUE);
				if(bPreLoad == true)
				{
					pItem->SetChecked(TRUE);
				}	
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetItemData((DWORD_PTR)pInput);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
				pItem->SetItemData((DWORD_PTR)pMode);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
				KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
				const KIOColumns* pColumns	= pTable->Columns();

				if(iterModeColumns != endModeColumns)
				{
					std::vector<CString> vecModeColumns = iterModeColumns->second;
					size_t nColumnCount = vecModeColumns.size();
					int nIndex       = 0;
					for (size_t i = 0; i < nColumnCount; i++)
					{
						CString strColunmName = vecModeColumns[i];
						KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
						int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
					}
					pItemCombo->SetCurSel(nIndex);
					pItemCombo->SetAlignment(xtpColumnTextCenter);
					pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItemCombo->SetEditable(FALSE);
				}

				int nModeID = pMode->ModeID();

				AutoType iterDefaultMode = m_mapGetDefaultModeInfo.find(nModeID);
				AutoType  endDefaultMode = m_mapGetDefaultModeInfo.end();
				if(iterDefaultMode != endDefaultMode)
				{
					std::vector<KOccupancyPCEMode> vecDefaultModeInfo = iterDefaultMode->second;
					KOccupancyPCEMode oOccupancyPCEMode = vecDefaultModeInfo[0];
					dOccupancy							= oOccupancyPCEMode.dOccupancy;
					dPCE								= oOccupancyPCEMode.dPCE;
				}
				else
				{
					dOccupancy = 1.0;
					dPCE	   = 1.0;
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
				pItem->SetItemData(pInput->NetworkType());

				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(FALSE);
				pItem->SetEditable(FALSE);
			}
			iterModeColumns++;
		}
		m_wndModeReport.Populate();
		m_wndModeReport.ExpandAll(TRUE);
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

void KATripMatrix::ModeSetting()
{
	try
	{
		CString strValue(_T(""));
		strValue.Format(_T("%.3f"), m_dAnalysisTime);
		m_edtAnalysisTime.SetWindowText(strValue);
		m_edtAnalysisTime.EnableRealNumber(true);
		m_edtAnalysisTime.EnableMinMax(0.0, 24.0);
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

void KATripMatrix::DefaultModeSetting(int a_nMasterCode)
{
	try
	{
		KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(m_mapDefaultODTime);

		AutoType iterDefault = m_mapDefaultODTime.find(a_nMasterCode);
		AutoType endDefault  = m_mapDefaultODTime.end();
		if(iterDefault != endDefault)
		{
			KODTime oODTime    = iterDefault->second;

			m_dAnalysisTime	   = oODTime.dTimeDuration;

			CString strValue(_T(""));
			strValue.Format(_T("%.3f"), m_dAnalysisTime);
			m_edtAnalysisTime.SetWindowText(strValue);
			m_edtAnalysisTime.EnableRealNumber(true);
			m_edtAnalysisTime.EnableMinMax(0.0, 24.0);
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

void KATripMatrix::InputList(std::list<KAInputInfo*>& lstInpuInfo)
{
	m_lstInputInfo = lstInpuInfo;
}

bool KATripMatrix::ValidateCheck()
{
	bool bResult = false;

	int nSelectCount(0);
	int nPreLoadCount(0);

	CXTPReportRecords* pRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_SELECT);
		KAInputInfo* pInfo = (KAInputInfo*)(pItem->GetItemData());

		if(pItem->IsChecked() == TRUE)
		{
			nSelectCount++;
			bResult = true;
		}
		pItem = pRecord->GetItem(_6_COLUMN_PRELOAD);

		if (TRUE == pItem->GetCheckedState())
		{
			nPreLoadCount++;
		}
	}
	if (false == bResult)
	{
		AfxMessageBox(_T("선택한 정보가 없습니다."));
		return bResult;
	}

	if (0 != nSelectCount && nSelectCount == nPreLoadCount)
	{
		AfxMessageBox(_T("모든 통행수단에 사전 배정 옵션이 선택되어 있습니다."));
		bResult = false;
	}

	return bResult;
}

void KATripMatrix::ApplyInputInfo()
{
	try
	{
		UpdateData(TRUE);
		//m_lstInputInfo.clear();

		CXTPReportRecords* pRecords	= m_wndModeReport.GetRecords();
		int nRecordCount			= pRecords->GetCount();

		for(int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord*     pRecord  = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem	   = pRecord->GetItem(_0_COLUMN_SELECT);
			KAInputInfo*          pInfo	   = (KAInputInfo*)pItem->GetItemData();

			if(pRecord->GetItem(_0_COLUMN_SELECT)->IsChecked() == FALSE)
			{
				pInfo->Selected(false);
				CXTPReportRecordItemText* pTextItem = (CXTPReportRecordItemText*)(pRecord->GetItem(_1_COLUMN_MODE));
				KMode*					  pMode	    = (KMode*)pTextItem->GetItemData();
				pInfo->Mode(pMode);

				CXTPReportRecordItemComboEx* pCombo  = (CXTPReportRecordItemComboEx*)(pRecord->GetItem(_2_COLUMN_MATRIX));
				KIOColumn*					 pColumn = (KIOColumn*)pCombo->GetItemData(pCombo->GetCurSel());
				pInfo->InputColumn(pColumn);

				CXTPReportRecordItemNumber* pNumber   = (CXTPReportRecordItemNumber*)(pRecord->GetItem(_3_COLUMN_OCCUPANCY));
				pInfo->Occupancy(pNumber->GetValue());

				pNumber   = (CXTPReportRecordItemNumber*)(pRecord->GetItem(_4_COLUMN_PCE));
				pInfo->PCU(pNumber->GetValue());

				pItem	  = pRecord->GetItem(_5_COLUMN_NETWORKTYPE);
				pInfo->NetworkType(pItem->GetItemData());			

				pItem	  = pRecord->GetItem(_6_COLUMN_PRELOAD);
				pInfo->PreLoad((pItem->IsChecked() == TRUE ? true : false));		
			}
			else
			{
				pInfo->Selected(true);

				CXTPReportRecordItemText* pTextItem = (CXTPReportRecordItemText*)(pRecord->GetItem(_1_COLUMN_MODE));
				KMode*					  pMode	    = (KMode*)pTextItem->GetItemData();
				pInfo->Mode(pMode);

				CXTPReportRecordItemComboEx* pCombo  = (CXTPReportRecordItemComboEx*)(pRecord->GetItem(_2_COLUMN_MATRIX));
				KIOColumn*					 pColumn = (KIOColumn*)pCombo->GetItemData(pCombo->GetCurSel());
				pInfo->InputColumn(pColumn);

				CXTPReportRecordItemNumber* pNumber   = (CXTPReportRecordItemNumber*)(pRecord->GetItem(_3_COLUMN_OCCUPANCY));
				pInfo->Occupancy(pNumber->GetValue());

				pNumber   = (CXTPReportRecordItemNumber*)(pRecord->GetItem(_4_COLUMN_PCE));
				pInfo->PCU(pNumber->GetValue());

				pItem	  = pRecord->GetItem(_5_COLUMN_NETWORKTYPE);
				pInfo->NetworkType(pItem->GetItemData());			

				pItem	  = pRecord->GetItem(_6_COLUMN_PRELOAD);
				pInfo->PreLoad((pItem->IsChecked() == TRUE ? true : false));		
			}
			//m_lstInputInfo.push_back(pInfo);
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

void KATripMatrix::OnReportCheckItemMode(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if(nIndex == _0_COLUMN_SELECT)
	{
		if(TRUE == pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_SELECT)->IsChecked())
		{
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX)->SetEditable(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX)->SetBackgroundColor(RGB(255,255,255));
			
			if (KmzSystem::IsPassengerSystem())
			{
				//여객의 경우 Trip을 대단위로 변경하기 위하여 Occupancy값을 사용하나, 
				//화물의 경우 소/중/대로 분할된 차량대수로 배정을 하므로 Occupancy를 수정하지 않음
				pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_OCCUPANCY)->SetEditable(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_OCCUPANCY)->SetBackgroundColor(RGB(255,255,255));
			}
			
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_PCE)->SetEditable(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_PCE)->SetBackgroundColor(RGB(255,255,255));
			pItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_PRELOAD)->SetEditable(TRUE);
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_OCCUPANCY)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_OCCUPANCY)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_PCE)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_PCE)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_PRELOAD)->SetEditable(FALSE);
		}
	}
	m_wndModeReport.RedrawControl();
}

void KATripMatrix::OnBnClickedOk()
{
	try
	{
		if(!ValidateCheck())
		{
			return;
		}

		ApplyInputInfo();

		CString strNumber;
		m_edtAnalysisTime.GetWindowText(strNumber);
		m_dAnalysisTime = _ttof(strNumber);

		KResizeDialogEx::OnOK();
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


void KATripMatrix::OnBnClickedDefault()
{
	try
	{
		ApplyInputInfo();

		KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(m_mapDefaultODTime);

		AutoType iterODTime = m_mapDefaultODTime.begin();
		AutoType endODTime  = m_mapDefaultODTime.end();
		if(iterODTime != endODTime)
		{
			AutoType findDeODTime = m_mapDefaultODTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			AutoType findSyODTime = m_mapDefaultODTime.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			if(findDeODTime != endODTime)
			{
				DefaultModeSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else if (findSyODTime != endODTime)
			{
				DefaultModeSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}

		if (KmzSystem::IsPassengerSystem())
		{
			KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(m_mapDefaultOccupancyPCEMode);

			AutoType iterMode = m_mapDefaultOccupancyPCEMode.begin();
			AutoType endMode  = m_mapDefaultOccupancyPCEMode.end();
			if(iterMode != endMode)
			{
				AutoType findDeMode = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
				AutoType findSyMode = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
				if(findDeMode != endMode)
				{
					DefaultModeReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
				}
				else if (findSyMode != endMode)
				{
					DefaultModeReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
				}
			}
		}
		else
		{
			DefaultFModeReportRecord();
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


void KATripMatrix::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KATripMatrix::FModeReportRecord()
{
	try
	{
		m_wndModeReport.ResetContent(TRUE);

		CXTPReportRecords*			 pRecords	  = m_wndModeReport.GetRecords();
		CXTPReportRecord*			 pRecord	  = nullptr;
		CXTPReportRecordItem*        pItem   	  = nullptr;
		CXTPReportRecordItemComboEx* pItemCombo   = nullptr;
		KMode*						 pMode	      = nullptr;
		KIOColumn*					 pInputColumn = nullptr;
		CString						 strModeName  (_T(""));
		double						 dOccupancy   (1.0);
		double						 dPCE		  (1.0);
		bool						 bPreLoad	  = false;
		bool						 bSelected	  = false;

		KDBaseAssignment::GetFModeChoiceODAtGroupingMode(m_pTarget, m_mapModeChocieColumns);

		std::list<KAInputInfo*>::iterator iterInput; 

		AutoType endInput  = m_lstInputInfo.end();
		for(iterInput = m_lstInputInfo.begin(); iterInput != endInput; ++iterInput)
		{
			KAInputInfo* pInput	= *iterInput;
			bSelected			= pInput->Selected();
			pMode				= pInput->Mode();
			strModeName		    = pMode->ModeName();
			pInputColumn		= pInput->InputColumn();
			dOccupancy			= pInput->Occupancy();
			dPCE				= pInput->PCU();
			bPreLoad			= pInput->PreLoad();

			std::vector<CString> vecModeColumns;
			AutoType  findColumns = m_mapModeChocieColumns.find(pMode->ModeID());
			AutoType  endColumns = m_mapModeChocieColumns.end();
			if (findColumns != endColumns)
			{
				vecModeColumns = findColumns->second;
			}

			size_t nColumncount = vecModeColumns.size();

			pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);

			if(bSelected == true)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->SetItemData((DWORD_PTR)pInput);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				if (nColumncount < 1)
				{
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}
				else
				{
					pItem->SetEditable(TRUE);
					pItem->SetChecked(TRUE);
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
				pItem->SetItemData((DWORD_PTR)pMode);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(TRUE);

				pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
				KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);
				const KIOColumns* pColumns	= pTable->Columns();

				int nIndex       = 0;
				int nCurrentIndex   = 0;
				for (size_t i = 0; i < nColumncount; i++)
				{
					CString strColunmName = vecModeColumns[i];
					KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
					int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);

					CString strCompareColumnName = pColumn->Caption();
					CString strCompareInputName = pInput->InputColumn()->Caption();

					if(strCompareColumnName == strCompareInputName)
					{
						nCurrentIndex = nIndex;
					}
				}
				pItemCombo->SetCurSel(nCurrentIndex);
				pItemCombo->SetAlignment(xtpColumnTextCenter);
				pItemCombo->SetBackgroundColor(RGB(255,255,255));
				pItemCombo->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				//여객의 경우 Trip을 대단위로 변경하기 위하여 Occupancy값을 사용하나, 
				//화물의 경우 소/중/대로 분할된 차량대수로 배정을 하므로 Occupancy를 수정하지 않음
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
				pItem->SetItemData(pInput->NetworkType());

				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetEditable(TRUE);
				if(bPreLoad == true)
				{
					pItem->SetChecked(TRUE);
				}	
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetItemData((DWORD_PTR)pInput);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(FALSE);
				if (nColumncount < 1)
				{
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
				pItem->SetItemData((DWORD_PTR)pMode);
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
				KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);
				const KIOColumns* pColumns	= pTable->Columns();

				int nSelIndex       = 0;
				for (size_t i = 0; i < nColumncount; i++)
				{
					CString strColunmName = vecModeColumns[i];
					KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
					int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
				}

				pItemCombo->SetCurSel(nSelIndex);
				pItemCombo->SetAlignment(xtpColumnTextCenter);
				pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItemCombo->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
				pItem->SetItemData(pInput->NetworkType());

				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(FALSE);
				pItem->SetEditable(FALSE);
			}
		}
		m_wndModeReport.Populate();
		m_wndModeReport.ExpandAll(TRUE);
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

void KATripMatrix::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC_ODTIME,			  SZ_TOP_LEFT,    SZ_TOP_LEFT);
		SetResize(IDC_EDIT_TIME,				  SZ_TOP_LEFT,    SZ_TOP_LEFT);	
		SetResize(IDC_STATIC_FUNCTION,		      SZ_TOP_LEFT,  SZ_TOP_LEFT);

		SetResize(IDC_SEPRATOR2,				  SZ_TOP_LEFT,    SZ_TOP_RIGHT);
		SetResize(IDC_STATIC_SELECT,			  SZ_TOP_LEFT,    SZ_TOP_LEFT);
		SetResize(IDC_REPORT,					  SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR1,				  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_RIGHT);

		SetResize(IDDEFAULT,					  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

		SetResize(IDOK,							  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KATripMatrix::DefaultFModeReportRecord()
{
	try
	{
		KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(m_mapDefaultOccupancyPCEMode);
		AutoType iter = m_lstInputInfo.begin();
		AutoType end = m_lstInputInfo.end();
		while(iter != end)
		{
			KAInputInfo* oInfo = *iter;
			KMode* pMode = oInfo->Mode();

			std::vector<KOccupancyPCEMode> vecOccupancy;
			AutoType ufind = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			AutoType sfind = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			AutoType end   = m_mapDefaultOccupancyPCEMode.end();
			if (ufind != end)
			{
				vecOccupancy = ufind->second;
			}
			else if (sfind != end)
			{
				vecOccupancy = sfind->second;
			}

			for (size_t i = 0; i < vecOccupancy.size(); i++)
			{
				KOccupancyPCEMode oOccuInfo = vecOccupancy[i];
				if (oOccuInfo.nModeID == pMode->ModeID())
				{
					oInfo->Occupancy(oOccuInfo.dOccupancy);
					oInfo->PCU(oOccuInfo.dPCE);
				}
			}
			++iter;
		}

		FModeReportRecord();
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
