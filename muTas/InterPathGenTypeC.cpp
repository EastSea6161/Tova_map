// InterPathGenTypeC.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterPathGenTypeC.h"
#include "afxdialogex.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"


// KInterPathGenTypeC 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterPathGenTypeC, KResizeDialogEx)

KInterPathGenTypeC::KInterPathGenTypeC(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterPathGenTypeC::IDD, pParent)
{

}

KInterPathGenTypeC::~KInterPathGenTypeC()
{
}

void KInterPathGenTypeC::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_TIME, m_wmReportControl);
	DDX_Control(pDX, IDC_COMBO_USINGFIELD, m_cboUsingLinkField);
}


BEGIN_MESSAGE_MAP(KInterPathGenTypeC, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, &KInterPathGenTypeC::OnBnClickedRadioTimeMethod)
END_MESSAGE_MAP()


BOOL KInterPathGenTypeC::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wmReportControl, TRUE);
	
	CheckRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, IDC_RADIO_INITTIME);
	OnBnClickedRadioTimeMethod(IDC_RADIO_INITTIME);
	InitComboLinkField();
	InitControls();

	InitReportHeader();
	UpdateTerminalTimeData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterPathGenTypeC::SetTarget( KTarget* a_pTarget )
{
	try
	{
		if (a_pTarget == nullptr)
		{
			return;
		}

		m_pTarget = a_pTarget;
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

void KInterPathGenTypeC::InitComboLinkField()
{
	m_cboUsingLinkField.ResetContent();

	KIOTables*           pIOTables     = m_pTarget->Tables();
	KIOTable*                  pTable              = pIOTables->FindTable(TABLE_LINK);
	const KIOColumns* pIOColumns = pTable->Columns();

	int nColCount = pIOColumns->ColumnCount();

	int nCur		= 0;
	int nCurrentCur = 0;
	for (int i= 0; i< nColCount; i++)
	{
		KIOColumn*       pIOColumn      = pIOColumns->GetColumn(i);
		KEMIOColumnType  emColumnType   = pIOColumn->ColumnType();
		KEMIODataType    emDataType     = pIOColumn->DataType();

		if(KEMIODataTypeDouble == emDataType)
		{
			CString strDisplayName    = pIOColumn->Caption();
			nCur   = m_cboUsingLinkField.AddString(strDisplayName);
			m_cboUsingLinkField.SetItemData( nCur, (DWORD_PTR)pIOColumn );
			if (pIOColumn == m_pIOColumnUseLink)
			{
				nCurrentCur = nCur;
			}
		}
	}

	m_cboUsingLinkField.SetCurSel(nCurrentCur);
}

void KInterPathGenTypeC::OnBnClickedRadioTimeMethod( UINT nID )
{
	if(IDC_RADIO_INITTIME == nID)
	{
		GetDlgItem(IDC_COMBO_USINGFIELD)->EnableWindow(FALSE);
	}
	else if(IDC_RADIO_USINGFIELD == nID)
	{
		GetDlgItem(IDC_COMBO_USINGFIELD)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_USINGFIELD)->SetFocus();
	}
}


void KInterPathGenTypeC::InitAssingRuleValue()
{
	m_oAssignInfo.dDWTimeTaxi			= 5.0;
	m_oAssignInfo.dDWTimeBusstop		= 0.2;
	m_oAssignInfo.dDwTimeRailStation	= 0.5;
	
	m_oAssignInfo.nInvehicle			= 1;
	m_oAssignInfo.nWating				= 1;
	m_oAssignInfo.nTransfer				= 1;
	m_oAssignInfo.nAETime				= 1;

	m_oAssignInfo.dAlpha				= 0.15;
	m_oAssignInfo.dBeta					= 2.0;
	m_oAssignInfo.dFirstPathTime		= 1.5;
	m_oAssignInfo.dSecondePathLength		= 1.5;
}



void KInterPathGenTypeC::InitControls( void )
{
	InitAssingRuleValue();

	CString strValue(_T(""));
	strValue.Format(_T("%.2f"), m_oAssignInfo.dDWTimeTaxi);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TAXITIME);
	SetDlgItemText(IDC_EDIT_TAXITIME, strValue);

	strValue.Format(_T("%.2f"), m_oAssignInfo.dDWTimeBusstop);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BUSTIME);
	SetDlgItemText(IDC_EDIT_BUSTIME, strValue);

	strValue.Format(_T("%.2f"), m_oAssignInfo.dDwTimeRailStation);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SUBWAYTIME);
	SetDlgItemText(IDC_EDIT_SUBWAYTIME, strValue);

	strValue.Format(_T("%.2f"), m_oAssignInfo.dAlpha);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ALPHA);
	SetDlgItemText(IDC_EDIT_ALPHA, strValue);

	strValue.Format(_T("%.2f"), m_oAssignInfo.dBeta);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BETA);
	SetDlgItemText(IDC_EDIT_BETA, strValue);

	strValue.Format(_T("%.2f"), m_oAssignInfo.dFirstPathTime);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHTIME);
	SetDlgItemText(IDC_EDIT_PATHTIME, strValue);

	strValue.Format(_T("%.2f"), m_oAssignInfo.dSecondePathLength);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHLENGTH);
	SetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);

	if (1 == m_oAssignInfo.nInvehicle)
	{
		CheckDlgButton(IDC_CHECK_INVEHICLE, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_INVEHICLE, FALSE);
	}

	if (1 == m_oAssignInfo.nWating)
	{
		CheckDlgButton(IDC_CHECK_WATING, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_WATING, FALSE);
	}

	if (1 == m_oAssignInfo.nTransfer)
	{
		CheckDlgButton(IDC_CHECK_TRANSFER, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_TRANSFER, FALSE);
	}

	if (1 == m_oAssignInfo.nAETime)
	{
		CheckDlgButton(IDC_CHECK_ACCESSEGRESS, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_ACCESSEGRESS, FALSE);
	}

}

void KInterPathGenTypeC::InitReportHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(1, _T("Access"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(2, _T("Egress"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
}

void KInterPathGenTypeC::InitTerminalModeValue( std::map<int, TTerminalProcessTime>& a_mapTerminalTime )
{
	try
	{
		a_mapTerminalTime.clear();

		std::map<int, int> mapTerminalMatching;
		KDBaseInterModal::GetInterRegionModeDefine(m_pTarget, mapTerminalMatching);

		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
		std::map<int,CString> mapRegionMode;
		pCodeGroup->GetCodes(mapRegionMode);

		AutoType iter = mapRegionMode.begin();
		AutoType end  = mapRegionMode.end();
		while(iter != end)
		{
			TTerminalProcessTime oInfo;
			oInfo.nModeID		= iter->first;
			oInfo.strModeName	= iter->second;

			AutoType find = mapTerminalMatching.find(iter->first);
			AutoType fend  = mapTerminalMatching.end();
			if (find == fend)
			{
				oInfo.dAccess = 20;
				oInfo.dEgress = 10;
			}
			else
			{
				if (find->second == BUS_TRMINAL)
				{
					oInfo.dAccess = 15;
					oInfo.dEgress = 5;
				}
				else if (find->second == INTER_REGIONAL_RAILSTATION)
				{
					oInfo.dAccess = 20;
					oInfo.dEgress = 10;
				}
				else if (find->second == AIR_TERMIANL)
				{
					oInfo.dAccess = 20;
					oInfo.dEgress = 10;
				}
				else if (find->second == MARIN_PORT)
				{
					oInfo.dAccess = 30;
					oInfo.dEgress = 20;
				}
				else
				{
					oInfo.dAccess = 20;
					oInfo.dEgress = 10;
				}
			}
			
			a_mapTerminalTime.insert(std::make_pair(oInfo.nModeID, oInfo));
			++iter;
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

void KInterPathGenTypeC::UpdateTerminalTimeData( void )
{
	try
	{
		std::map<int, TTerminalProcessTime> mapTerminalTime;
		InitTerminalModeValue(mapTerminalTime);

		AutoType iter = mapTerminalTime.begin();
		AutoType end  = mapTerminalTime.end();

		while(iter != end)
		{
			TTerminalProcessTime oInfo = iter->second;

			CXTPReportRecord*		pRecord = m_wmReportControl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strModeName));
			pItem->SetEditable(FALSE);
			pItem->SetItemData(oInfo.nModeID);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dAccess, _T("%.0f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dEgress, _T("%.0f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);
			++iter;
		}

		m_wmReportControl.Populate();
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

void KInterPathGenTypeC::ArgumentRestriction( CStdioFileEx& of )
{
	try
	{
		int nRestriction(0);
		CString strOut(_T(""));

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_TURN))
		{
			nRestriction = 1;
		}

		strOut.Format(_T("TurnRestriction\t%d\r\n"), nRestriction);
		of.WriteString(strOut);
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

void KInterPathGenTypeC::ArgumentTimeMethod( KIOColumn*& a_pColumn, CStdioFileEx& of )
{
	try
	{
		int nTimeMethod		= 0;
		int nDistance		= 0;
		int nCursel			= 0;
		if( IDC_RADIO_INITTIME == GetCheckedRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD) )
		{
			nTimeMethod     = 1;

			KIOTables* pIOTables = m_pTarget->Tables();
			KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_LINK);
			const KIOColumns* pIOColumns = pIOTable->Columns();

			int nColCount = pIOColumns->ColumnCount();
			for (int i= 0; i< nColCount; i++)
			{
				KIOColumn*       pIOColumn      = pIOColumns->GetColumn(i);
				CString          strColumnName  = pIOColumn->Name();

				if(strColumnName.CompareNoCase(COLUMN_LINK_SPEED) == 0)
				{
					a_pColumn = pIOColumn;
				}
			}
		}
		else
		{
			nTimeMethod     = 2;
			nCursel         = m_cboUsingLinkField.GetCurSel();
			a_pColumn = (KIOColumn*)m_cboUsingLinkField.GetItemData(nCursel);
		}

		CString strOut(_T(""));
		strOut.Format(_T("TimeMethod\t%d\r\n"), nTimeMethod);
		of.WriteString(strOut);
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

void KInterPathGenTypeC::ArgumentTime( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));

		CString strDwellTaxi;
		GetDlgItemText(IDC_EDIT_TAXITIME, strDwellTaxi);
		double dTimeTaxi = _ttof(strDwellTaxi);

		CString strDwellbus;
		GetDlgItemText(IDC_EDIT_BUSTIME, strDwellbus);
		double dTimeBus = _ttof(strDwellbus);

		CString strDwellSubway;
		GetDlgItemText(IDC_EDIT_SUBWAYTIME, strDwellSubway);
		double dTimeSub = _ttof(strDwellSubway);

		strOut.Format(_T("TaxiWatingTime\t%.2f\r\n"), dTimeTaxi);
		of.WriteString(strOut);
		strOut.Format(_T("TransitDwellTime\t%.2f\t%.2f\r\n"), dTimeBus, dTimeSub);
		of.WriteString(strOut);
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

void KInterPathGenTypeC::ArgumnetVDF( CStdioFileEx& of )
{
	CString strOut(_T(""));

	CString strAlPha;
	GetDlgItemText(IDC_EDIT_ALPHA, strAlPha);
	double dAlpah = _ttof(strAlPha);

	CString strBeTa;
	GetDlgItemText(IDC_EDIT_BETA, strBeTa);
	double dBeta = _ttof(strBeTa);

	strOut.Format(_T("BusWaitingVDF\t%.2f\t%.2f\r\n"), dAlpah, dBeta);
	of.WriteString(strOut);
}

void KInterPathGenTypeC::ArgumentCondition( CStdioFileEx& of )
{
	CString strOut(_T(""));
	strOut.Format(_T("Constraint\t2\r\n"));
	of.WriteString(strOut);

	CString strtime;
	GetDlgItemText(IDC_EDIT_PATHTIME, strtime);

	CString strlength;
	GetDlgItemText(IDC_EDIT_PATHLENGTH, strlength);

	int nInVehecle = 0;
	int nWating    = 0;
	int nTransfer  = 0;
	int nAETime	   = 0;

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_INVEHICLE))
	{
		nInVehecle = 1;
	}

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_WATING))
	{
		nWating = 1;
	}

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_TRANSFER))
	{
		nTransfer = 1;
	}

	if (TRUE == IsDlgButtonChecked(IDC_CHECK_ACCESSEGRESS))
	{
		nAETime = 1;
	}

	strOut.Format(_T("time\t%s\t%d\t%d\t%d\t%d\r\n"), strtime, nInVehecle, nWating, nTransfer, nAETime);
	of.WriteString(strOut);

	strOut.Format(_T("length\t%s\r\n"), strlength);
	of.WriteString(strOut);
}

void KInterPathGenTypeC::ArgumentProsessTime( CStdioFileEx& of )
{
	try
	{
		CXTPReportRecords*          pRecords = m_wmReportControl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItemText*   pItemTXT = nullptr;
		CXTPReportRecordItemNumber*   pItemNum = nullptr;

		CString strOut(_T(""));
		int nRecordCount = pRecords->GetCount();
		for (int i =0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			int nModeID			= pItemTXT->GetItemData();
			CString strModeName = pItemTXT->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			double dAccess = pItemNum->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			double dEgress = pItemNum->GetValue(); 

			strOut.Format(_T("%d\t%.2f\t%.2f\r\n"), nModeID, dAccess, dEgress);
			of.WriteString(strOut);
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

void KInterPathGenTypeC::SaveResultTime2DB()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();

	try
	{
		CString strDwellTaxi;
		GetDlgItemText(IDC_EDIT_TAXITIME, strDwellTaxi);
		double dTimeTaxi = _ttof(strDwellTaxi);

		CString strDwellbus;
		GetDlgItemText(IDC_EDIT_BUSTIME, strDwellbus);
		double dTimeBus = _ttof(strDwellbus);

		CString strDwellSubway;
		GetDlgItemText(IDC_EDIT_SUBWAYTIME, strDwellSubway);
		double dTimeSub = _ttof(strDwellSubway);

		CString strAlpha;
		GetDlgItemText(IDC_EDIT_ALPHA, strAlpha);
		double dAlpah = _ttof(strAlpha);

		CString strBeTa;
		GetDlgItemText(IDC_EDIT_BETA, strBeTa);
		double dBeta = _ttof(strBeTa);

		CString strtime;
		GetDlgItemText(IDC_EDIT_PATHTIME, strtime);
		double dtime = _ttof(strtime);

		CString strlength;
		GetDlgItemText(IDC_EDIT_PATHLENGTH, strlength);
		double dlength = _ttof(strlength);

		int nInVehecle = 0;
		int nWating    = 0;
		int nTransfer  = 0;
		int nAETime	   = 0;

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_INVEHICLE))
		{
			nInVehecle = 1;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_WATING))
		{
			nWating = 1;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_TRANSFER))
		{
			nTransfer = 1;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_ACCESSEGRESS))
		{
			nAETime = 1;
		}

		CString strDELSQL(_T(""));
		strDELSQL.Format(_T("delete from %s"), TABLE_INTER_TIME_INFO);
		spDBaseConnection->ExecuteUpdate(strDELSQL);

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T("Insert Into %s (taxi_time, bus_time, subway_time, alpha, beta, constaint_time, constaint_length, invehicle_time, waiting_time, transfer_time, ae_time)"), TABLE_INTER_TIME_INFO);
		strSQL.AppendFormat(_T(" Values (%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %d, %d, %d, %d)")
			,dTimeTaxi, dTimeBus, dTimeSub, dAlpah, dBeta, dtime, dlength, nInVehecle, nWating, nTransfer, nAETime);
		spDBaseConnection->ExecuteUpdate(strSQL);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGenTypeC::SaveResultTerminalTime2DB()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();

	try
	{
		CString strDELSQL(_T(""));
		strDELSQL.Format(_T("delete from %s"), TABLE_INTER_TERMINAL_TIME_INFO);
		spDBaseConnection->ExecuteQuery(strDELSQL);

		CXTPReportRecords*          pRecords = m_wmReportControl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItemText*   pItemTXT = nullptr;
		CXTPReportRecordItemNumber*   pItemNum = nullptr;

		CString strOut(_T(""));
		int nRecordCount = pRecords->GetCount();
		for (int i =0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemTXT = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			int nModeID			= pItemTXT->GetItemData();
			CString strModeName = pItemTXT->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			double dAccess = pItemNum->GetValue(); 

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			double dEgress = pItemNum->GetValue(); 

			CString strSQL(_T(""));
			strSQL.Format(_T(" Insert Into %s (id, name, access_time, egress_time) Values (%d, '%s', %.2f, %.2f )"),
				TABLE_INTER_TERMINAL_TIME_INFO, nModeID, strModeName, dAccess, dEgress);
			spDBaseConnection->ExecuteUpdate(strSQL);
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		throw -1;
	}
}
