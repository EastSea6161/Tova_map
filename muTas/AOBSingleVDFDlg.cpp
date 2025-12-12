// KAOBSingleVDFDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AOBSingleVDFDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include <fstream>
#include <iostream>
#include "ABPRFnManagerDlg.h"
#include "DBaseConnector.h"

#include "DefineNamesDefaultPara.h"


// KAOBSingleVDFDlg 대화 상자입니다.

using namespace std;

IMPLEMENT_DYNAMIC(KAOBSingleVDFDlg, KDialogEx)

KAOBSingleVDFDlg::KAOBSingleVDFDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KAOBSingleVDFDlg::IDD, pParent)
	,m_pTarget(a_pTarget),
	m_dVOT(1.0),
	m_dAllLinkAlpha(0.15),
	m_dAllLinkBeta(4.0),
	m_nComboFnCursor(0),
	m_nComboLinkAllAlphaCursor(0),
	m_nComboLinkAllBetaCursor(0),
	m_nComboLinkAllACursor(0),
	m_nComboLinkAllBCursor(0),
	m_nFnGroupID(0),
	m_nRadiobtnFunction(0),
	m_bUsedDefault(true)
{

}

KAOBSingleVDFDlg::~KAOBSingleVDFDlg()
{
}

void KAOBSingleVDFDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VOT,			m_edtVOT);
	DDX_Control(pDX, IDC_EDIT_ALL_ALPHA,	m_edtAllAlpha);
	DDX_Control(pDX, IDC_EDIT_ALL_BETA,		m_edtAllBeta);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Control(pDX, IDC_COMBO2, m_comboLinkAlpha);
	DDX_Control(pDX, IDC_COMBO3, m_comboLinkBeta);
	DDX_Control(pDX, IDC_COMBO4, m_comboLinkA);
	DDX_Control(pDX, IDC_COMBO6, m_comboLinkB);
	DDX_Control(pDX, IDC_COMBO1, m_comboFn);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadiobtnFunction);
}

BEGIN_MESSAGE_MAP(KAOBSingleVDFDlg, KDialogEx)
	ON_BN_CLICKED(ID_BUTTON_SETBPRFN, &KAOBSingleVDFDlg::OnBnClickedButtonSetbprfn)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KAOBSingleVDFDlg::OnCbnSelchangeCombo1)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO3, &KAOBSingleVDFDlg::OnBnClickedRadio)
END_MESSAGE_MAP()


// KAOBSingleVDFDlg 메시지 처리기입니다.


BOOL KAOBSingleVDFDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	KDBaseDefaultParaHighwayAssign::LoadDefaultVDFPara(m_mapDefaultVDFPara);
	KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(m_mapDefaultODTime);
	KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntion(m_mapDefaultVDFFuntion);
	KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntionGroup(m_mapDefaultVDFFuntionGroup);

	m_edtVOT.EnableRealNumber(true);
	m_edtVOT.EnableMinMax(0.0, 10000.0);

	CString strValue;

	strValue.Format(_T("%f"), m_dVOT);
	m_edtVOT.SetWindowText(strValue);

	m_edtAllAlpha.EnableRealNumber(true);
	m_edtAllBeta.EnableRealNumber(true);

	strValue.Format(_T("%f"), m_dAllLinkAlpha);
	m_edtAllAlpha.SetWindowText(strValue);

	strValue.Format(_T("%f"), m_dAllLinkBeta);
	m_edtAllBeta.SetWindowText(strValue);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	InitComboBoxEachFn();
	InitComboBoxEachLink();

	InitReportCtrl();
	UpdataReportCtrl();

	SetRadioButton();

	if (m_bUsedDefault == true)
	{
		SetDefaultSetting();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KAOBSingleVDFDlg::InitReportCtrl( void )
{
	m_wndReport.GetColumns()->Clear();
	m_wndReport.ResetContent();

	KReportCtrlSetting::Default(m_wndReport, TRUE);

	CXTPReportColumn* pReportColumn = NULL;
	int nColumnItemIndex(0);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Function No."), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pReportColumn->SetCaption(_T("함수번호"));
    }

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("α"), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);


	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("β"), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("a"), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("b"), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);
}

void KAOBSingleVDFDlg::UpdataReportCtrl( void )
{
	m_wndReport.ResetContent();

	if (m_comboFn.GetCurSel() == 0)
	{
		m_wndReport.Populate();
		return;
	}
	

	int nRecordCount = m_vecReportData.size();
	CXTPReportRecord* pReportRecord = NULL;

	for(int i = 0; i < nRecordCount; ++i)
	{
		std::vector<CString> vecRow = m_vecReportData[i];
		pReportRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pRecordItem = NULL;

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[0]));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[1]));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[2]));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[3]));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[4]));
	}

	m_wndReport.Populate();
}

void KAOBSingleVDFDlg::UpdateReportCtrlByEachLink( void )
{
	int nFnGroupID = m_comboFn.GetItemData(m_comboFn.GetCurSel());

	if (nFnGroupID == 0)
		return;

	std::vector<CString> vecColName;

	try
	{
		CString strQuery;
		strQuery.Format(
			_T("SELECT alpha_column_name, beta_column_name, a_column_name, b_column_name FROM function_group WHERE function_group_id = %d"), nFnGroupID);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("No Data..."));

		vecColName.push_back(spResultSet->GetValueString(0));
		vecColName.push_back(spResultSet->GetValueString(1));
		vecColName.push_back(spResultSet->GetValueString(2));
		vecColName.push_back(spResultSet->GetValueString(3));

		ConvertSysToDispColumn(vecColName);

		//ReportUpdate
		CString strValue;
		int     nSize = vecColName.size();

		CXTPReportRecords*        pRecords  = m_wndReport.GetRecords();
		CXTPReportRecordItemText* pTextItem = nullptr;

		int nRecord  = pRecords->GetCount();

		for (int i = 0; i < nSize; ++i)
		{
			strValue = vecColName[i];

			if (strValue.IsEmpty())
				continue;

			for (int j = 0; j < nRecord; ++j)
			{
				pTextItem = (CXTPReportRecordItemText*)pRecords->GetAt(j)->GetItem(i+1);
				pTextItem->SetCaption(strValue);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebug(_T("unpexected error..."));
	}

	m_wndReport.Populate();
}

void KAOBSingleVDFDlg::InitComboBoxEachLink( void )
{	
	try
	{
		m_comboLinkAlpha.ResetContent();
		m_comboLinkBeta.ResetContent();
		m_comboLinkA.ResetContent();
		m_comboLinkB.ResetContent();

		int nCursorIndex = 0;
		//nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("--- Select ---"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("선택"));
		}
		else {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("Select"));
		}
		m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("--- Select ---"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("선택"));
		}
		else {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("Select"));
		}
		m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkA.InsertString(-1, _T("---- 0.0 ----"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("0.0"));
		}
		else {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("0.0"));
		}
		m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkB.InsertString(-1, _T("---- 0.0 ----"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("0.0"));
		}
		else {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("0.0"));
		}
		m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		KIOTables* pTables = m_pTarget->Tables();
		KIOTable* pTable = pTables->FindTable(TABLE_LINK);

		const KIOColumns* pColumns = pTable->Columns();

		int nColumnCount = pColumns->ColumnCount();

		for(int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			if(pColumn->DataType() == KEMIODataTypeDouble)
			{
				nCursorIndex = m_comboLinkAlpha.InsertString(-1, pColumn->Caption());
				m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

                if (_tcsicmp(pColumn->Name(), COLUMN_LINK_ALPHA) == 0)
                {
                    m_nComboLinkAllAlphaCursor = nCursorIndex;
                    m_comboLinkAlpha.SetCurSel(nCursorIndex);
                }

				nCursorIndex = m_comboLinkBeta.InsertString(-1, pColumn->Caption());
				m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

                if (_tcsicmp(pColumn->Name(), COLUMN_LINK_BETA) == 0)
                {
                    m_nComboLinkAllBetaCursor = nCursorIndex;
                    m_comboLinkBeta.SetCurSel(nCursorIndex);
                }

				nCursorIndex = m_comboLinkA.InsertString(-1, pColumn->Caption());
				m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);
                if (_tcsicmp(pColumn->Name(), COLUMN_LINK_A) == 0)
                {
                    m_nComboLinkAllACursor = nCursorIndex;
                    m_comboLinkA.SetCurSel(nCursorIndex);
                }

				nCursorIndex = m_comboLinkB.InsertString(-1, pColumn->Caption());
				m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

                if (_tcsicmp(pColumn->Name(), COLUMN_LINK_B) == 0)
                {
                    m_nComboLinkAllBCursor = nCursorIndex;
                    m_comboLinkB.SetCurSel(nCursorIndex);
                }
			}
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KAOBSingleVDFDlg::InitComboBoxEachFn( void )
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (spDBaseConnection == NULL)
	{
		TxLogDebugException();
		return;
	}

	try
	{
		CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		m_comboFn.ResetContent();

		//int nCursorIndex = m_comboFn.InsertString(-1, _T("------ Select ------"));
		int nCursorIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboFn.InsertString(-1, _T("----------선택----------"));
		}
		else {
			nCursorIndex = m_comboFn.InsertString(-1, _T("---------Select---------"));
		}
		m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		while (oResultSetPtr->Next())
		{
			nCursorIndex = m_comboFn.InsertString(-1, oResultSetPtr->GetValueString(1));
			m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );
		}

		if( m_nFnGroupID != 0)
		{
			int nComboCount = m_comboFn.GetCount();

			for (int i = 0; i < nComboCount; ++i)
			{
				int nFnGroupId = m_comboFn.GetItemData(i);

				if ( nFnGroupId == m_nFnGroupID)
					m_comboFn.SetCurSel(i);
			}
		}
		else
		{
			m_comboFn.SetCurSel(m_nComboFnCursor);
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KAOBSingleVDFDlg::OnBnClickedButtonSetbprfn()
{
	m_nComboFnCursor = m_comboFn.GetCurSel();
	int nFnGroupID   = m_comboFn.GetItemData(m_nComboFnCursor);

	KABPRFnManagerDlg oDlg(m_pTarget, nFnGroupID);

	if(oDlg.DoModal() == IDOK)
	{
		m_nFnGroupID = oDlg.FnGroupID();

		InitComboBoxEachFn();
		OnCbnSelchangeCombo1();
	}
}

void KAOBSingleVDFDlg::OnCbnSelchangeCombo1()
{
	int nFnGroupID = m_comboFn.GetItemData(m_comboFn.GetCurSel());
	m_nEachFnCombo = nFnGroupID;

	m_vecReportData.clear();
	m_mapEachFN.clear();
    	
    KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
    if (spDBaseConnection == NULL)
    {
        TxLogDebugException();
        return;
    }

	try
	{
		CString strQuery;
		strQuery.Format(_T("select Function_ID, Alpha, Beta, A, B from Function where Function_Group_ID = %d"), nFnGroupID);

		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		while (oResultSetPtr->Next())
		{
			std::vector<double> vecEachFn;

			vecEachFn.push_back(oResultSetPtr->GetValueDouble(1));
			vecEachFn.push_back(oResultSetPtr->GetValueDouble(2));
			vecEachFn.push_back(oResultSetPtr->GetValueDouble(3));
			vecEachFn.push_back(oResultSetPtr->GetValueDouble(4));

			m_mapEachFN.insert(std::make_pair(oResultSetPtr->GetValueInt(0), vecEachFn));

			std::vector<CString> vecRow;

			vecRow.push_back( oResultSetPtr->GetValueString(0));
			vecRow.push_back( oResultSetPtr->GetValueString(1));
			vecRow.push_back( oResultSetPtr->GetValueString(2));
			vecRow.push_back( oResultSetPtr->GetValueString(3));
			vecRow.push_back( oResultSetPtr->GetValueString(4));

			m_vecReportData.push_back(vecRow);
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	UpdataReportCtrl();
	UpdateReportCtrlByEachLink();
}

void KAOBSingleVDFDlg::OnBnClickedRadio( UINT nID )
{
	SetRadioButton();
}

void KAOBSingleVDFDlg::SetRadioButton()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_ALL_ALPHA)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ALL_BETA)->EnableWindow(false);
	GetDlgItem(IDC_COMBO1)->EnableWindow(false);
	GetDlgItem(IDC_COMBO2)->EnableWindow(false);
	GetDlgItem(IDC_COMBO3)->EnableWindow(false);
	GetDlgItem(IDC_COMBO4)->EnableWindow(false);
	GetDlgItem(IDC_COMBO6)->EnableWindow(false);
	CButton* pFnBtn = (CButton*)GetDlgItem(ID_BUTTON_SETBPRFN);
	pFnBtn->EnableWindow(FALSE);

	switch (m_nRadiobtnFunction)
	{
	case 0:
		m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;
		GetDlgItem(IDC_EDIT_ALL_ALPHA)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_ALL_BETA)->EnableWindow(true);
		return;
	case 1:
		m_emBPRFnType = KEMAssignmentBPRFunctionEachFn;
		GetDlgItem(IDC_COMBO1)->EnableWindow(true);
		pFnBtn->EnableWindow(TRUE);
		return;
	case 2:
		m_emBPRFnType = KEMAssignmentBPRFunctionEachLink;
		GetDlgItem(IDC_COMBO2)->EnableWindow(true);
		GetDlgItem(IDC_COMBO3)->EnableWindow(true);
		GetDlgItem(IDC_COMBO4)->EnableWindow(true);
		GetDlgItem(IDC_COMBO6)->EnableWindow(true);
		return;
	}
	UpdateData(FALSE);
}

bool KAOBSingleVDFDlg::WriteInputArgument( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		if(m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
		{
			of.WriteString(_T("vdf\t3\r\n")); 

			strOut.Format(_T("%d\t%d\t%d\t%d\r\n"), m_emBPRFnType, 1, 0 ,1);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\t\r\n"), 0 ,0 ,0);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%f\t%f\t0\t0\t\r\n"), m_emBPRFnType, m_dAllLinkAlpha, m_dAllLinkBeta);
			of.WriteString(strOut);
		}
		else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
		{
			int nRecordCount = m_vecReportData.size();

			int nVDFRows = nRecordCount + 2;

			strOut.Format(_T("vdf\t%d\r\n"), nVDFRows);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\t%d\r\n"), m_emBPRFnType, 1, 0 ,1);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\t\r\n"), 0 ,0 ,0);
			of.WriteString(strOut);

			int nIndex;
			double dAlpha, dBeta, dA, dB;

			for (int i = 0; i < nRecordCount; ++i)
			{
				std::vector<CString> vecRow = m_vecReportData[i];

				nIndex	= _ttoi(vecRow[0]);
				dAlpha	= _ttof(vecRow[1]);
				dBeta	= _ttof(vecRow[2]);
				dA		= _ttof(vecRow[3]);
				dB		= _ttof(vecRow[4]);

				strOut.Format(_T("%d\t%g\t%g\t%g\t%g\r\n"), nIndex, dAlpha, dBeta, dA, dB);
				of.WriteString(strOut);
			}

		}
		else if (m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
		{
			strOut.Format(_T("vdf\t%d\r\n"), 2);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\t%d\r\n"), m_emBPRFnType, 1, 0 ,1 );
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\r\n"), 0 ,0 ,0);
			of.WriteString(strOut);

			// 			strOut.Format(_T("%d\t%d\t%d\t%d\t%d\r\n"), 1, 0, 0, 0, 0);
			// 			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KAOBSingleVDFDlg::ExportDB2LinkFile(KDBaseConPtr a_spDBaseConnection)
{
	if (m_emBPRFnType != KEMAssignmentBPRFunctionEachLink)
	{
		TxLogDebugStartMsg();

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id") ;
		strSQL = strSQL + _T(" From Link Order By link_id ");

		ofstream ofs(_T("link.dat"), ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

		try
		{
			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= 0.0;
				fixedLink.beta		= 0.0;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
			}
		}
		catch (...)
		{
			TxLogDebugException();
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}
	else
	{
		TxLogDebugStartMsg();

		CString strAlphaCaption = _T("");
		m_comboLinkAlpha.GetLBText(m_comboLinkAlpha.GetCurSel(),strAlphaCaption);
		CString strAlphaColumnName = GetColumnName(strAlphaCaption);

		CString strBetaCaption = _T("");
		m_comboLinkBeta.GetLBText(m_comboLinkBeta.GetCurSel(),strBetaCaption);
		CString strBetaColumnName = GetColumnName(strBetaCaption);

		CString strATollCaption = _T("");
		m_comboLinkA.GetLBText(m_comboLinkA.GetCurSel(),strATollCaption);
		CString strATollColumnName = GetColumnName(strATollCaption);

		CString strBTollCaption = _T("");
		m_comboLinkB.GetLBText(m_comboLinkB.GetCurSel(),strBTollCaption);
		CString strBTollColumnName = GetColumnName(strBTollCaption);

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;
		strSQL  = strSQL + _T(", ") + strAlphaColumnName + _T(", ") + strBetaColumnName;
		
		if (strATollColumnName != _T(""))
		{
			strSQL = strSQL + _T(", ") + strATollColumnName;
		}

		if (strBTollColumnName !=_T(""))
		{
			strSQL = strSQL + _T(", ") + strBTollColumnName;
		}

		strSQL = strSQL + _T(" From Link Order By link_id ");

		ofstream ofs(_T("link.dat"), ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

		try
		{
			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= spResultSet->GetValueDouble(9);
				fixedLink.beta		= spResultSet->GetValueDouble(10);

				if (strATollColumnName == _T(""))
				{
					fixedLink.a		= 0.0;
				}
				else
				{
					fixedLink.a			= spResultSet->GetValueDouble(11);
				}

				if (strBTollColumnName == _T(""))
				{
					fixedLink.b		= 0.0;
				}
				else
				{
					fixedLink.b			= spResultSet->GetValueDouble(12);
				}

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
			}
		}
		catch (...)
		{
			TxLogDebugException();
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}

	return false;
}

CString KAOBSingleVDFDlg::GetColumnName( CString a_strColumnCaption )
{
	CString strColumnName = _T("");

	KIOTables* pTables = m_pTarget->Tables();
	KIOTable* pTable = pTables->FindTable(TABLE_LINK);

	const KIOColumns* pColumns = pTable->Columns();

	int nColumnCount = pColumns->ColumnCount();

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->DataType() == KEMIODataTypeDouble)
		{
			if (pColumn->Caption() == a_strColumnCaption)
			{
				strColumnName = pColumn->Name();

				return strColumnName;
			}
		}
	}
	return strColumnName;
}

bool KAOBSingleVDFDlg::GetOBSingleInfo( TAOBSingleVDF& a_TAOBSingeVDF )
{
	UpdateData(TRUE);

	CString strvalue = _T("");

	m_edtVOT.GetWindowText(strvalue);
	m_dVOT = _ttof(strvalue);

	m_edtAllAlpha.GetWindowText(strvalue);
	m_dAllLinkAlpha = _ttof(strvalue);

	m_edtAllBeta.GetWindowText(strvalue);
	m_dAllLinkBeta = _ttof(strvalue);

	CString strAlphaCaption = _T("");
	m_comboLinkAlpha.GetLBText(m_comboLinkAlpha.GetCurSel(),strAlphaCaption);
	CString strAlphaColumnName = GetColumnName(strAlphaCaption);

	CString strBetaCaption = _T("");
	m_comboLinkBeta.GetLBText(m_comboLinkBeta.GetCurSel(),strBetaCaption);
	CString strBetaColumnName = GetColumnName(strBetaCaption);

	CString strATollCaption = _T("");
	m_comboLinkA.GetLBText(m_comboLinkA.GetCurSel(),strATollCaption);
	CString strATollColumnName = GetColumnName(strATollCaption);

	CString strBTollCaption = _T("");
	m_comboLinkB.GetLBText(m_comboLinkB.GetCurSel(),strBTollCaption);
	CString strBTollColumnName = GetColumnName(strBTollCaption);

	TAOBSingleVDF oTOBSingeInfo;
	oTOBSingeInfo.TdValueofTime		= m_dVOT;
	oTOBSingeInfo.TemFunctionType	= m_emBPRFnType;
	oTOBSingeInfo.TdAllAlpha		= m_dAllLinkAlpha;
	oTOBSingeInfo.TdAllBeta			= m_dAllLinkBeta;
	oTOBSingeInfo.TvecFucntionType	= m_vecReportData;
	oTOBSingeInfo.TstrAlpha			= strAlphaColumnName;
	oTOBSingeInfo.TstrBeta			= strBetaColumnName;
	oTOBSingeInfo.TA				= strATollColumnName;
	oTOBSingeInfo.TB				= strBTollColumnName;
	oTOBSingeInfo.TstrCaptionAlpha  = strAlphaCaption;
	oTOBSingeInfo.TstrCaptionBeta   = strBetaCaption;
	oTOBSingeInfo.TstrCaptionA      = strATollCaption;
	oTOBSingeInfo.TstrCaptionB      = strBTollCaption;

	if (m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
	{
		if (m_vecReportData.size() == 0)
		{
			AfxMessageBox(_T("함수 형태별 VDF에 설정한 함수가 존재하지 않습니다."));
			return false;
		}
		
		oTOBSingeInfo.TnEachFnCombo = m_comboFn.GetItemData(m_comboFn.GetCurSel());
		oTOBSingeInfo.TmapEachFN    = m_mapEachFN;
	}

	if (m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
	{
		if (strAlphaColumnName == _T("") || strBetaColumnName == _T(""))
		{
			AfxMessageBox(_T("지체함수 계수/지수 컬럼이 설정되지 않았습니다."));
			return false;
		}
	}


	a_TAOBSingeVDF = oTOBSingeInfo;
	UpdateData(FALSE);

	return true;
}

void KAOBSingleVDFDlg::SetOBSingleInfo( TAOBSingleVDF& a_TAOBSingeVDF )
{
	//UpdateData(TRUE);
	
	m_dVOT = a_TAOBSingeVDF.TdValueofTime;
	
	switch(a_TAOBSingeVDF.TemFunctionType)
	{
		case KEMAssignmentBPRFunctionAllLink:
			m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;
			m_dAllLinkAlpha = a_TAOBSingeVDF.TdAllAlpha;
			m_dAllLinkBeta	= a_TAOBSingeVDF.TdAllBeta;
			SetRadioButton();
			return;

		case  KEMAssignmentBPRFunctionEachFn:
			m_emBPRFnType = KEMAssignmentBPRFunctionEachFn;
			m_vecReportData = a_TAOBSingeVDF.TvecFucntionType;
			InitComboBoxEachFn();
			OnCbnSelchangeCombo1();
			SetRadioButton();
			return;

		case  KEMAssignmentBPRFunctionEachLink:
			m_emBPRFnType = KEMAssignmentBPRFunctionEachLink;
			SetComboBoxInfo(a_TAOBSingeVDF);
			SetRadioButton();
			return;
	}


}

CString KAOBSingleVDFDlg::GetColumnCaption( CString a_strColumnName )
{
	CString strColumnCaption = _T("");

	KIOTables* pTables = m_pTarget->Tables();
	KIOTable* pTable = pTables->FindTable(TABLE_LINK);

	const KIOColumns* pColumns = pTable->Columns();

	int nColumnCount = pColumns->ColumnCount();

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->DataType() == KEMIODataTypeDouble)
		{
			if (pColumn->Name() == a_strColumnName)
			{
				strColumnCaption = pColumn->Caption();

				return strColumnCaption;
			}
		}
	}
	return strColumnCaption;
}

void KAOBSingleVDFDlg::SetComboBoxInfo( TAOBSingleVDF& a_TAOBSingeVDF)
{
	if (a_TAOBSingeVDF.TstrAlpha == _T(""))
	{
		return;
	}

	int nIndex = m_comboLinkAlpha.GetCount();
	for (int i = 0; i < nIndex ; i++)
	{
		CString strComboName = _T("");
		m_comboLinkAlpha.GetLBText(i,strComboName);

		CString strColumnCaption = GetColumnCaption(a_TAOBSingeVDF.TstrAlpha);
		if (strColumnCaption == strComboName)
		{
			m_comboLinkAlpha.SetCurSel(i);
		}

	}


	if (a_TAOBSingeVDF.TstrBeta == _T(""))
	{
		return;
	}

	nIndex = m_comboLinkBeta.GetCount();
	for (int i = 0; i < nIndex ; i++)
	{
		CString strComboName = _T("");
		m_comboLinkBeta.GetLBText(i,strComboName);

		CString strColumnCaption = GetColumnCaption(a_TAOBSingeVDF.TstrBeta);
		if (strColumnCaption == strComboName)
		{
			m_comboLinkBeta.SetCurSel(i);
		}
	}

	if (a_TAOBSingeVDF.TA != _T(""))
	{
		nIndex = m_comboLinkA.GetCount();
		for (int i = 0; i < nIndex ; i++)
		{
			CString strComboName = _T("");
			m_comboLinkA.GetLBText(i,strComboName);

			CString strColumnCaption = GetColumnCaption(a_TAOBSingeVDF.TA);
			if (strColumnCaption == strComboName)
			{
				m_comboLinkA.SetCurSel(i);
			}
		}

	}

	if (a_TAOBSingeVDF.TB != _T(""))
	{
		nIndex = m_comboLinkB.GetCount();
		for (int i = 0; i < nIndex ; i++)
		{
			CString strComboName = _T("");
			m_comboLinkB.GetLBText(i,strComboName);

			CString strColumnCaption = GetColumnCaption(a_TAOBSingeVDF.TB);
			if (strColumnCaption == strComboName)
			{
				m_comboLinkB.SetCurSel(i);
			}
		}

	}
	UpdateData(FALSE);
}

void KAOBSingleVDFDlg::InitButtonControl( void )
{
	m_dVOT = 1;
	m_dAllLinkAlpha = 0.15;
	m_dAllLinkBeta = 4.0;
	m_nComboFnCursor = 0;
	m_nComboLinkAllAlphaCursor = 0;
	m_nComboLinkAllBetaCursor = 0;
	m_nComboLinkAllACursor = 0;
	m_nComboLinkAllBCursor =0;
	m_nFnGroupID =0;
	m_nRadiobtnFunction = 0;
}

void KAOBSingleVDFDlg::SetDefaultSetting( void )
{
	try
	{
		SetDefaultRadioButton();
		SetDefaultValueofTime();
		SetDefaultAllLink();
		SetDefaultEachLink();
		SetDefaultEachFn();

		SetRadioButton();
		OnCbnSelchangeCombo1();

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

void KAOBSingleVDFDlg::SetDefaultRadioButton( void )
{
	try
	{
		AutoType findUser	= m_mapDefaultVDFPara.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType findSystem	= m_mapDefaultVDFPara.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		AutoType findend	= m_mapDefaultVDFPara.end();

		KVDFPara oVDFPara;
		int nBPRFuntion(0);
		double dAlpa(0.15);
		double dBeta(2);

		if (findUser != findend)
		{
			oVDFPara	= findUser->second;
			nBPRFuntion = oVDFPara.nVDFChoice;
		}
		else if (findSystem != findend)
		{
			oVDFPara = findSystem->second;
			nBPRFuntion = oVDFPara.nVDFChoice;
		}
		
// 		if (KEMAssignmentBPRFunctionAllLink == (nBPRFuntion+ 1) )
// 		{
// 			CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
// 			m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;
// 		}
// 		else if (KEMAssignmentBPRFunctionEachFn == (nBPRFuntion+ 1))
// 		{
// 			CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
// 			m_emBPRFnType = KEMAssignmentBPRFunctionEachFn;
// 		}
// 		else if (KEMAssignmentBPRFunctionEachLink == (nBPRFuntion+ 1))
// 		{
// 			CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
// 			m_emBPRFnType = KEMAssignmentBPRFunctionEachLink;
// 		}

		CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
		m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;

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

void KAOBSingleVDFDlg::SetDefaultValueofTime( void )
{
	try
	{
		AutoType findUser	= m_mapDefaultODTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType findSystem	= m_mapDefaultODTime.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		AutoType findend	= m_mapDefaultODTime.end();

		KODTime oODTime;
		double dTravelTime(1.0);
		if (findUser!= findend)
		{
			oODTime = findUser->second;
			dTravelTime = oODTime.dTravelTime;
		}
		else if (findSystem != findend)
		{
			oODTime = findSystem->second;
			dTravelTime = oODTime.dTravelTime;
		}
		
		CString strValue(_T(""));
		strValue.Format(_T("%.6f"), dTravelTime);
		m_edtVOT.SetWindowText(strValue);
		m_edtVOT.EnableRealNumber(true);
		m_edtVOT.EnableMinMax(0.0, 10000.0);
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

void KAOBSingleVDFDlg::SetDefaultAllLink( void )
{
	try
	{
		AutoType findUser	= m_mapDefaultVDFPara.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType findSystem	= m_mapDefaultVDFPara.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		AutoType findend	= m_mapDefaultVDFPara.end();

		double dAlpa(0.15);
		double dBeta(2);

		if (findUser != findend)
		{
			KVDFPara oVDFPara = findUser->second;
			dAlpa = oVDFPara.dAllLinkAlpha;
			dBeta = oVDFPara.dAllLinkBeta;
		}
		else if (findSystem != findend)
		{
			KVDFPara oVDFPara = findSystem->second;
			dAlpa = oVDFPara.dAllLinkAlpha;
			dBeta = oVDFPara.dAllLinkBeta;
		}

		CString strValue(_T(""));
		strValue.Format(_T("%.6f"), dAlpa);
		m_edtAllAlpha.SetWindowText(strValue);
		m_edtAllAlpha.EnableRealNumber(true);

		strValue.Format(_T("%.6f"), dBeta);
		m_edtAllBeta.SetWindowText(strValue);
		m_edtAllBeta.EnableRealNumber(true);
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

void KAOBSingleVDFDlg::SetDefaultEachLink( void )
{
	try
	{
		m_comboLinkAlpha.ResetContent();
		m_comboLinkBeta.ResetContent();
		m_comboLinkA.ResetContent();
		m_comboLinkB.ResetContent();

		//int nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("-----Select-----"));
		int nCursorIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("------선택------"));
		}
		else {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("-----Select-----"));
		}
		m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("-----Select-----"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("------선택------"));
		}
		else {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("-----Select-----"));
		}
		m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkA.InsertString(-1, _T("-----Select-----"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("------선택------"));
		}
		else {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("-----Select-----"));
		}
		m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkB.InsertString(-1, _T("-----Select-----"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("------선택------"));
		}
		else {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("-----Select-----"));
		}
		m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		AutoType iterB  = m_mapDefaultVDFPara.begin();
		AutoType itEndB = m_mapDefaultVDFPara.end();

		int nMaxMasterCode(0);
		while (iterB != itEndB)
		{
			nMaxMasterCode = iterB->first;
			++iterB;
		}

		AutoType iter = m_mapDefaultVDFPara.find(nMaxMasterCode);
		AutoType end  = m_mapDefaultVDFPara.end();

		if ( iter != end )
		{
			KVDFPara oVDFPara = iter->second;

			CString strAlpha		    = oVDFPara.strLinkColumnAlpha;
			CString strBeta		        = oVDFPara.strLinkColumnBeta;
			CString strA    		    = oVDFPara.strLinkColumnA;
			CString strB		        = oVDFPara.strLinkColumnB;
			CString strSelect           = (_T("-----Select-----"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strSelect = _T("------선택------");
			}

			KIOTables* pTables = m_pTarget->Tables();
			KIOTable* pTable = pTables->FindTable(TABLE_LINK);

			const KIOColumns* pColumns = pTable->Columns();

			int nColumnCount = pColumns->ColumnCount();

			for(int i = 0; i < nColumnCount; ++i)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);
				if(pColumn->DataType() == KEMIODataTypeDouble)
				{
					nCursorIndex = m_comboLinkAlpha.InsertString(-1, pColumn->Caption());
					m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					if (pColumn->Caption() == strAlpha )
					{
						m_comboLinkAlpha.SetCurSel(nCursorIndex);
					}

					nCursorIndex = m_comboLinkBeta.InsertString(-1, pColumn->Caption());
					m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					if (pColumn->Caption() == strBeta)
					{
						m_comboLinkBeta.SetCurSel(nCursorIndex);
					}

					nCursorIndex = m_comboLinkA.InsertString(-1, pColumn->Caption());
					m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);
					if (pColumn->Caption() == strA)
					{
						m_comboLinkA.SetCurSel(nCursorIndex);
					}

					nCursorIndex = m_comboLinkB.InsertString(-1, pColumn->Caption());
					m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					if (pColumn->Caption() == strB)
					{
						m_comboLinkB.SetCurSel(nCursorIndex);
					}
				}
			}
			if (strAlpha == strSelect )
			{
				m_comboLinkAlpha.SetCurSel(0);
			}

			if (strBeta == strSelect )
			{
				m_comboLinkBeta.SetCurSel(0);
			}

			if (strA == strSelect )
			{
				m_comboLinkA.SetCurSel(0);
			}

			if (strB == strSelect )
			{
				m_comboLinkB.SetCurSel(0);
			}
		}
		else
		{
			InitComboBoxEachLink();
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KAOBSingleVDFDlg::SetDefaultEachFn( void )
{
	AutoType findUser	= m_mapDefaultVDFPara.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
	AutoType findSystem = m_mapDefaultVDFPara.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	AutoType findend	= m_mapDefaultVDFPara.end();

	CString strUserFunctionGroup(_T(""));
	if (findUser != findend)
	{
		KVDFPara oVDFPara = findUser->second;
		strUserFunctionGroup = oVDFPara.strUseFunctionID;
	}
	else if (findSystem != findend)
	{
		KVDFPara oVDFPara = findSystem->second;
		strUserFunctionGroup = oVDFPara.strUseFunctionID;
	}

	AutoType iterGroup = m_mapDefaultVDFFuntionGroup.find(strUserFunctionGroup);
	AutoType endGroup  = m_mapDefaultVDFFuntionGroup.end();

	if (iterGroup != endGroup)
	{
		int nFnID = iterGroup->second;
		int nComboCount = m_comboFn.GetCount();
		for (int i = 0; i < nComboCount; i++)
		{
			if (nFnID = (int)m_comboFn.GetItemData(i))
			{
				m_comboFn.SetCurSel(i);
			}

		}
	}
}

void KAOBSingleVDFDlg::UsedDefalut( bool a_bUsedDefalut )
{
	m_bUsedDefault = a_bUsedDefalut;
}

void KAOBSingleVDFDlg::ConvertSysToDispColumn( std::vector<CString>& a_vecColName )
{
	KIOTables*  pIOTables  = m_pTarget->Tables();
	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
	KIOColumns* pIOColumns = pIOTable->Columns();
	KIOColumn*  pIOColumn  = nullptr;

	int     nSize = a_vecColName.size();
	CString strValue;

	for (int i = 0; i < nSize; ++i)
	{
		strValue = a_vecColName[i];

		if (strValue.IsEmpty())
			continue;

		pIOColumn = pIOColumns->GetColumn(strValue);
		strValue  = pIOColumn->Caption();

		a_vecColName[i] = strValue;
	}
}
