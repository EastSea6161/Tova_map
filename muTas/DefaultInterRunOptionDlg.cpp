// DefaultInterRunOptionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultInterRunOptionDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Mode.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseConnector.h"

// KDefaultInterRunOptionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultInterRunOptionDlg, KResizeDialogEx)

KDefaultInterRunOptionDlg::KDefaultInterRunOptionDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultInterRunOptionDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KDefaultInterRunOptionDlg::~KDefaultInterRunOptionDlg()
{
}

void KDefaultInterRunOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT4,			m_wndUrbanReport);
	DDX_Control(pDX, IDC_REPORT3,			m_wndRegionalReport);
	DDX_Control(pDX, IDC_REPORT,			m_wndFreightReport);
}


BEGIN_MESSAGE_MAP(KDefaultInterRunOptionDlg, KResizeDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItem)
	ON_BN_CLICKED(IDDEFAULT, &KDefaultInterRunOptionDlg::OnBnClickedDefault)
	ON_BN_CLICKED(IDOK, &KDefaultInterRunOptionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefaultInterRunOptionDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL KDefaultInterRunOptionDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndUrbanReport,		 TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndUrbanReport);

	KReportCtrlSetting::Default(m_wndRegionalReport,		 TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndRegionalReport);

	KReportCtrlSetting::Default(m_wndFreightReport,		 TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndFreightReport);

	InitDuationEditContol();

	InitReportFreight();
	InitReportUrban();
	InitRoportRegional();

	UpdateFreightData();
	UpdateRegionalData();
	UpdateUrbanData();

	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefaultInterRunOptionDlg::InitReportFreight( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndFreightReport.AddColumn(new CXTPReportColumn(0 , _T("Select"), 20));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("선택"));
		}

		pColumn = m_wndFreightReport.AddColumn(new CXTPReportColumn(1,   _T("O/D Column"),    30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("O/D 데이터 컬럼"));
		}

		pColumn = m_wndFreightReport.AddColumn(new CXTPReportColumn(2,		_T("Occupancy"), 30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("평균 재차인원"));
		}

		pColumn = m_wndFreightReport.AddColumn(new CXTPReportColumn(3,      _T("PCE"),       30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("승용차 환산계수"));
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


void KDefaultInterRunOptionDlg::InitReportUrban( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndUrbanReport.AddColumn(new CXTPReportColumn(0,   _T("Mode"),    30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("통행수단"));
		}

		pColumn = m_wndUrbanReport.AddColumn(new CXTPReportColumn(1,	 _T("Occupancy"), 30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("평균 재차인원"));
		}

		pColumn = m_wndUrbanReport.AddColumn(new CXTPReportColumn(2,    _T("PCE"),       30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("승용차 환산계수"));
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

void KDefaultInterRunOptionDlg::InitRoportRegional( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndRegionalReport.AddColumn(new CXTPReportColumn(0,   _T("Mode"),    30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("통행수단"));
		}

		pColumn = m_wndRegionalReport.AddColumn(new CXTPReportColumn(1,	 _T("Occupancy"), 30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("평균 재차인원"));
		}

		pColumn = m_wndRegionalReport.AddColumn(new CXTPReportColumn(2,    _T("PCE"),       30));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("승용차 환산계수"));
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


void KDefaultInterRunOptionDlg::UpdateUrbanData( void )
{
	m_wndUrbanReport.ResetContent(TRUE);
	try
	{
		std::vector<TInterModeDefine> vecUrbanModeDefine;
		KDBaseDefaultInterRunOption::LoadInterUrbanOccupancyPCE(m_pTarget, vecUrbanModeDefine);

		for (size_t i = 0; i < vecUrbanModeDefine.size(); i++)
		{
			TInterModeDefine oInfo = vecUrbanModeDefine[i];

			CXTPReportRecord*		pRecord = m_wndUrbanReport.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem   = nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strModeName));
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetItemData(oInfo.nModeID);
			pItem->SetAlignment(DT_CENTER);

			if (oInfo.nModeID == -3)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dOccupancy, _T("%.2f")));
				pItem->SetAlignment(DT_RIGHT);
				pItem->SetEditable(TRUE);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dPCE, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);
			pItem->SetEditable(TRUE);
		}

		m_wndUrbanReport.Populate();

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

void KDefaultInterRunOptionDlg::UpdateRegionalData( void )
{
	m_wndRegionalReport.ResetContent(TRUE);
	try
	{
		std::vector<TInterModeDefine> vecRegionalModeDefine;
		KDBaseDefaultInterRunOption::LoadInterRegionalOccupancyPCE(m_pTarget, vecRegionalModeDefine);

		for (size_t i = 0; i < vecRegionalModeDefine.size(); i++)
		{
			TInterModeDefine oInfo = vecRegionalModeDefine[i];

			CXTPReportRecord*		pRecord = m_wndRegionalReport.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem   = nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strModeName));
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetItemData(oInfo.nModeID);
			pItem->SetAlignment(DT_CENTER);

			if (oInfo.nModeID == -1)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dOccupancy, _T("%.2f")));
				pItem->SetAlignment(DT_RIGHT);
				pItem->SetEditable(TRUE);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dPCE, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);
			pItem->SetEditable(TRUE);
		}

		m_wndRegionalReport.Populate();

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

void KDefaultInterRunOptionDlg::UpdateFreightData( void )
{
	m_wndFreightReport.ResetContent();

	try
	{
		std::map<int, KInterOccupancePCEMode> mapOccupancyPCE;
		KDBaseDefaultInterRunOption::GetModeInfo(m_pTarget, mapOccupancyPCE);

		AutoType iter = mapOccupancyPCE.begin();
		AutoType end  = mapOccupancyPCE.end();
		while(iter != end)
		{
			KInterOccupancePCEMode oInfo = iter->second;

			CXTPReportRecord*		pRecord = m_wndFreightReport.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem   = nullptr;
						
			if (oInfo.nUse == 1)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.pMode->ModeName()));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetItemData(DWORD_PTR(oInfo.pMode));

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dOccupancy, _T("%.2f")));
				pItem->SetAlignment(DT_RIGHT);
				pItem->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dPCE, _T("%.2f")));
				pItem->SetAlignment(DT_RIGHT);
				pItem->SetEditable(TRUE);

			}
			else {
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.pMode->ModeName()));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetItemData(DWORD_PTR(oInfo.pMode));

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dOccupancy, _T("%.2f")));
				pItem->SetAlignment(DT_RIGHT);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dPCE, _T("%.2f")));
				pItem->SetAlignment(DT_RIGHT);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			}
			++iter;
		}

		m_wndFreightReport.Populate();
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

void KDefaultInterRunOptionDlg::OnReportCheckItem( NMHDR* pNotifyStruct, LRESULT* pResult )
{

	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	if (pItemNotify == nullptr)
	{
		return;
	}

	try
	{
		if (TRUE == pItemNotify->pRow->GetRecord()->GetItem(0)->IsChecked())
		{
			CXTPReportRecordItem* pItem = nullptr;
			pItem = pItemNotify->pRow->GetRecord()->GetItem(2);
			pItem->SetEditable(TRUE);
			pItem->SetBackgroundColor(RGB(255,255,255));

			pItem = pItemNotify->pRow->GetRecord()->GetItem(3);
			pItem->SetEditable(TRUE);
			pItem->SetBackgroundColor(RGB(255,255,255));
		}
		else
		{
			CXTPReportRecordItem* pItem = nullptr;
			pItem = pItemNotify->pRow->GetRecord()->GetItem(2);
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			pItem = pItemNotify->pRow->GetRecord()->GetItem(3);
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
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

	m_wndFreightReport.RedrawControl();
}


void KDefaultInterRunOptionDlg::OnBnClickedDefault()
{
	UpdateFreightData();
	UpdateRegionalData();
	UpdateUrbanData();
	InitDuationEditContol();
}


void KDefaultInterRunOptionDlg::OnBnClickedOk()
{
	try
	{
		std::map<int, KInterOccupancePCEMode> mapInterOccupanncePCEMode;

		InvalidData(mapInterOccupanncePCEMode);
		SaveFreightData(mapInterOccupanncePCEMode);
		SaveRegionalData();
		SaveUrbanData();
		SaveDuation();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}
	KResizeDialogEx::OnOK();
}


void KDefaultInterRunOptionDlg::OnBnClickedCancel()
{
	

	KResizeDialogEx::OnCancel();
}

void KDefaultInterRunOptionDlg::InvalidData( std::map<int, KInterOccupancePCEMode>& a_mapModeOPInfo )
{

	a_mapModeOPInfo.clear();

	try
	{
		CXTPReportRecords* pRecords	= m_wndFreightReport.GetRecords();
		int nRecordCount			= pRecords->GetCount();


		for(int i = 0; i < nRecordCount; i++)
		{
			KInterOccupancePCEMode oInfo;

			CXTPReportRecord*     pRecord  = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem	   = pRecord->GetItem(0);

            if (pItem->IsChecked() == TRUE) {
                oInfo.nUse = 1;
            }
            else {
                oInfo.nUse = 0;
            }

			pItem = pRecord->GetItem(1);
			KMode* pMode = (KMode*)pItem->GetItemData();
			oInfo.nModeID = pMode->ModeID();

			CXTPReportRecordItemNumber* pNumber   = (CXTPReportRecordItemNumber*)(pRecord->GetItem(2));
			oInfo.dOccupancy = pNumber->GetValue();

			pNumber   = (CXTPReportRecordItemNumber*)(pRecord->GetItem(3));
			oInfo.dPCE = pNumber->GetValue();

			a_mapModeOPInfo.insert(std::make_pair(oInfo.nModeID, oInfo));
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

void KDefaultInterRunOptionDlg::SaveFreightData( std::map<int, KInterOccupancePCEMode> a_mapModeOPInfo )
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	spDBaseConnection->BeginTransaction();

	CString strDelSQL(_T(""));
	CString strInsertSQL(_T(""));
	try
	{
		strDelSQL.Format(_T(" Delete From %s Where Master_Code = %d"),KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_PCE_MODE, KDefaultParameterMasterCode::USER_MASETER_CODE);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		strInsertSQL.Format(_T("Insert Into %s( Master_Code, Use, detail_mode_id, Occupancy, PCE ) Values ( ?, ?, ?, ?, ? ) "),KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_PCE_MODE);
		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strInsertSQL);

		AutoType iter = a_mapModeOPInfo.begin();
		AutoType end  = a_mapModeOPInfo.end();
		while(iter != end)
		{
			KInterOccupancePCEMode oInfo = iter->second;
			spPreparedStatement->BindInt(1, KDefaultParameterMasterCode::USER_MASETER_CODE);
			spPreparedStatement->BindInt(2, oInfo.nUse);
			spPreparedStatement->BindInt(3, oInfo.nModeID);
			spPreparedStatement->BindDouble(4, oInfo.dOccupancy);
			spPreparedStatement->BindDouble(5, oInfo.dPCE);

			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
			++iter;
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw -1;
	}
}

void KDefaultInterRunOptionDlg::ResizeComponet( void )
{
	try
	{
		SetResize(IDC_STATIC_4, SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_EDIT1, SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_STATIC33, SZ_TOP_LEFT, SZ_TOP_RIGHT);

		SetResize(IDC_STATIC_3, SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_REPORT4, SZ_TOP_LEFT, CXTPResizePoint(1, .3));
		SetResize(IDC_STATIC24, CXTPResizePoint(0, .3), CXTPResizePoint(1, .3));

		SetResize(IDC_STATIC_2, CXTPResizePoint(0, .3), CXTPResizePoint(0, .3));
		SetResize(IDC_REPORT3, CXTPResizePoint(0, .3), CXTPResizePoint(1, .6));
		SetResize(IDC_STATIC11, CXTPResizePoint(0, .6), CXTPResizePoint(1, .6));

		SetResize(IDC_STATIC1, CXTPResizePoint(0, .6), CXTPResizePoint(0, .6));
		SetResize(IDC_REPORT, CXTPResizePoint(0, .6), SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC10, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

		SetResize(IDDEFAULT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

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

void KDefaultInterRunOptionDlg::InitDuationEditContol( void )
{
	try
	{
		double dDuation(0);
		KDBaseDefaultInterRunOption::LoadInterODDuration(m_pTarget, dDuation);

		CString strDuation(_T(""));
		strDuation.Format(_T("%.2f"), dDuation);
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(strDuation);
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

void KDefaultInterRunOptionDlg::SaveUrbanData()
{
	std::vector<TInterModeDefine> vecInterUrbanData;
	
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	spDBaseConnection->BeginTransaction();

	CString strDelSQL(_T(""));
	CString strInsertSQL(_T(""));

	try
	{
		CXTPReportRecords* pRecords	= m_wndUrbanReport.GetRecords();
		int nRecordCount			= pRecords->GetCount();

		for(int i = 0; i < nRecordCount; i++)
		{
			TInterModeDefine oInfo;

			CXTPReportRecord*			pRecord  = pRecords->GetAt(i);
			CXTPReportRecordItemText*	pItemTxt = nullptr;
			CXTPReportRecordItemNumber* pItemNum = nullptr;

			pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oInfo.strModeName = pItemTxt->GetCaption(NULL);
			oInfo.nModeID	  = pItemTxt->GetItemData();

			if (oInfo.nModeID == -3 )
			{
				oInfo.dOccupancy = 0;
			}
			else
			{
				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
				oInfo.dOccupancy = pItemNum->GetValue();
			}
			
			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oInfo.dPCE = pItemNum->GetValue();

			vecInterUrbanData.push_back(oInfo);
		}

		strDelSQL.Format(_T(" Delete From Default_Intermodal_Highway_Para_Urban Where Master_Code = %d"), KDefaultParameterMasterCode::USER_MASETER_CODE);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		strInsertSQL.Format(_T("Insert Into Default_Intermodal_Highway_Para_Urban ( Master_Code, detail_mode_id, mode_name, Occupancy, PCE ) Values ( ?, ?, ?, ?, ? ) "));
		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strInsertSQL);


		for (size_t i = 0; i < vecInterUrbanData.size(); i++)
		{
			TInterModeDefine oInfo = vecInterUrbanData[i];
			spPreparedStatement->BindInt(1,KDefaultParameterMasterCode::USER_MASETER_CODE );
			spPreparedStatement->BindInt(2, oInfo.nModeID);
			spPreparedStatement->BindText(3, oInfo.strModeName);
			spPreparedStatement->BindDouble(4, oInfo.dOccupancy);
			spPreparedStatement->BindDouble(5, oInfo.dPCE);

			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();

		}
		spDBaseConnection->Commit();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw -1;
	}
}

void KDefaultInterRunOptionDlg::SaveRegionalData()
{
	std::vector<TInterModeDefine> vecInterRegionalData;

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	spDBaseConnection->BeginTransaction();

	CString strDelSQL(_T(""));
	CString strInsertSQL(_T(""));

	try
	{
		CXTPReportRecords* pRecords	= m_wndRegionalReport.GetRecords();
		int nRecordCount			= pRecords->GetCount();

		for(int i = 0; i < nRecordCount; i++)
		{
			TInterModeDefine oInfo;

			CXTPReportRecord*			pRecord  = pRecords->GetAt(i);
			CXTPReportRecordItemText*	pItemTxt = nullptr;
			CXTPReportRecordItemNumber* pItemNum = nullptr;

			pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oInfo.strModeName = pItemTxt->GetCaption(NULL);
			oInfo.nModeID	  = pItemTxt->GetItemData();

			if (oInfo.nModeID == -1 )
			{
				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
				oInfo.dOccupancy = pItemNum->GetValue();
			}
			else
			{
				oInfo.dOccupancy = 0;
			}

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oInfo.dPCE = pItemNum->GetValue();

			vecInterRegionalData.push_back(oInfo);
		}

		strDelSQL.Format(_T(" Delete From Default_Intermodal_Highway_Para_Regional Where Master_Code = %d"), KDefaultParameterMasterCode::USER_MASETER_CODE);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		strInsertSQL.Format(_T("Insert Into Default_Intermodal_Highway_Para_Regional ( Master_Code, detail_mode_id, mode_name, Occupancy, PCE ) Values ( ?, ?, ?, ?, ? ) "));
		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strInsertSQL);

		for (size_t i = 0; i < vecInterRegionalData.size(); i++)
		{
			TInterModeDefine oInfo = vecInterRegionalData[i];
			spPreparedStatement->BindInt(1,KDefaultParameterMasterCode::USER_MASETER_CODE );
			spPreparedStatement->BindInt(2, oInfo.nModeID);
			spPreparedStatement->BindText(3, oInfo.strModeName);
			spPreparedStatement->BindDouble(4, oInfo.dOccupancy);
			spPreparedStatement->BindDouble(5, oInfo.dPCE);

			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();

		}
		spDBaseConnection->Commit();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw -1;
	}
}

void KDefaultInterRunOptionDlg::SaveDuation()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	spDBaseConnection->BeginTransaction();

	try
	{
		CString strDuation(_T(""));
		double dDuration(0);
		GetDlgItemText(IDC_EDIT1, strDuation);
		if (QBicStringChecker::IsNumeric(strDuation) == false)
		{
			AfxMessageBox(_T("1일 용량 환산계수에 숫자를 입력해 주세요."));
			throw -1;
		}
		dDuration = _ttof(strDuation);

		CString strDelSQL(_T(""));
		strDelSQL.Format(_T(" Delete From Default_Intermodal_Highway_Para_Duration Where Master_Code = %d"), KDefaultParameterMasterCode::USER_MASETER_CODE);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		CString strInsertSQL(_T(""));
		strInsertSQL.Format(_T(" Insert Into Default_Intermodal_Highway_Para_Duration ( Master_Code, Time_Duration ) Values ( '%d', '%f' ) "), KDefaultParameterMasterCode::USER_MASETER_CODE , dDuration);
		spDBaseConnection->ExecuteUpdate(strInsertSQL);

		spDBaseConnection->Commit();
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
