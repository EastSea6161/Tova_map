// ABPRFnDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ABPRFnDlg.h"
#include "afxdialogex.h"

#include "ABPRFnManagerDlg.h"
#include "DBaseConnector.h"
//^^#include "SEDData.h"

using namespace std;

// KABPRFnDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KABPRFnDlg, KDialogEx)

KEMAssignmentBPRFunctionType KABPRFnDlg::m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;

KABPRFnDlg::KABPRFnDlg(CWnd* pParent /*=NULL*/)
	:	KDialogEx(KABPRFnDlg::IDD, pParent),
		m_bUsedDefault(true),
		m_dVOT(1.0),
		m_dAllLinkAlpha(0.15),
		m_dAllLinkBeta(4.0),
		m_nComboFnCursor(0),
		m_nComboLinkAllAlphaCursor(0),
		m_nComboLinkAllBetaCursor(0),
		m_nComboLinkAllACursor(0),
		m_nComboLinkAllBCursor(0),
		m_nFnGroupID(0),
	    m_strUseFunctionGroup(_T(""))
{
	m_pTarget = NULL;
	m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;
}

KABPRFnDlg::~KABPRFnDlg()
{
}

void KABPRFnDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VOT,			m_edtVOT);
	DDX_Control(pDX, IDC_EDIT_ALL_ALPHA,	m_edtAllAlpha);
	DDX_Control(pDX, IDC_EDIT_ALL_BETA,		m_edtAllBeta);
	DDX_Control(pDX, IDC_COMBO1,			m_comboFn);
	DDX_Control(pDX, IDC_REPORT,			m_wndReport);
	DDX_Control(pDX, IDC_COMBO2,			m_comboLinkAlpha);
	DDX_Control(pDX, IDC_COMBO3,			m_comboLinkBeta);
	DDX_Control(pDX, IDC_COMBO4,			m_comboLinkA);
	DDX_Control(pDX, IDC_COMBO6,			m_comboLinkB);
}


BEGIN_MESSAGE_MAP(KABPRFnDlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO3, &KABPRFnDlg::OnRadioClicked)
	ON_BN_CLICKED(IDOK, &KABPRFnDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_SETBPRFN, &KABPRFnDlg::OnBnClickedButtonSetBPRFunction)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KABPRFnDlg::OnSelchangeCombo1)
	ON_BN_CLICKED(IDCANCEL, &KABPRFnDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDDEFAULT, &KABPRFnDlg::OnBnClickedDefault)
	ON_STN_CLICKED(IDC_SEPRATOR3, &KABPRFnDlg::OnStnClickedSeprator3)
END_MESSAGE_MAP()


// KABPRFnDlg 메시지 처리기입니다.


BOOL KABPRFnDlg::OnInitDialog()
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

	DefaultRadioButton();

	m_edtVOT.EnableRealNumber(true);
	m_edtVOT.EnableMinMax(0.0, 10000.0);

	AutoType beginODDefault  = m_mapDefaultODTime.begin();
	AutoType endODDefault    = m_mapDefaultODTime.end();

	AutoType beginVDFDefault = m_mapDefaultVDFPara.begin();
	AutoType endVDFDefault   = m_mapDefaultVDFPara.end();

	if((beginODDefault != endODDefault) && (beginVDFDefault != endVDFDefault) && m_bUsedDefault == true)
	{
		DefaultVDFSetting();
	}
	else
	{
		CString strValue;
		strValue.Format(_T("%f"), m_dVOT);
		m_edtVOT.SetWindowText(strValue);

		m_edtAllAlpha.EnableRealNumber(true);
		m_edtAllBeta.EnableRealNumber(true);

		strValue.Format(_T("%f"), m_dAllLinkAlpha);
		m_edtAllAlpha.SetWindowText(strValue);

		strValue.Format(_T("%f"), m_dAllLinkBeta);
		m_edtAllBeta.SetWindowText(strValue);
	}

	AutoType beginVDFParaDefault = m_mapDefaultVDFPara.begin();
	AutoType endVDFParaDefault   = m_mapDefaultVDFPara.end();

	if(beginVDFParaDefault!=endVDFParaDefault && m_bUsedDefault == true)
	{
		DefaultComboBoxEachLink();
	}
	else
	{
		InitComboBoxEachLink();
	}

	UpdateCtrlState();

	m_wndReport.GetColumns()->Clear();

	KReportCtrlSetting::Default(m_wndReport);

	InitReportCtrl();

	AutoType beginGroup  = m_mapDefaultVDFFuntionGroup.begin();
	AutoType endGroup  = m_mapDefaultVDFFuntionGroup.end();

	if (beginGroup != endGroup && m_bUsedDefault == true)
	{
		DefaultComboBoxEachFn();
	    DefaultVDFReportCtrl();
	}
	else
	{
		InitComboBoxEachFn();
		UpdateReportCtrl();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KABPRFnDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strNumber;

	m_edtVOT.GetWindowText(strNumber);
	if (strNumber == _T(""))
	{
		m_edtVOT.SetFocus();
		AfxMessageBox(_T("유료도로 통행료 시간가치(VOT) 값이 없습니다."));
		return;
	}
	m_dVOT = _ttof(strNumber);
	
	CString strAlpah(_T(""));
	CString strBeta(_T(""));
	m_edtAllAlpha.GetWindowText(strAlpah);
	m_dAllLinkAlpha = _ttof(strAlpah);

	m_edtAllBeta.GetWindowText(strBeta);
	m_dAllLinkBeta = _ttof(strBeta);

	if (m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
	{
		if (strAlpah == _T(""))
		{
			m_edtAllAlpha.SetFocus();
			AfxMessageBox(_T("지체함수 계수 Alpha 값이 없습니다."));
			return;
		}
		
		if (strBeta == _T(""))
		{
			m_edtAllBeta.SetFocus();
			AfxMessageBox(_T("지체함수 지수 Beta 값이 없습니다."));
			return;
		}
	}

	
	if (m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
	{
		OnSelchangeCombo1();
		size_t nCount = m_vecReportData.size();

		if (nCount == 0)
		{
			AfxMessageBox(_T("함수 형태별 VDF에 설정한 함수가 존재하지 않습니다."));
			return;
		}
	}
	m_nComboFnCursor			= m_comboFn.GetCurSel();
	m_nComboLinkAllAlphaCursor	= m_comboLinkAlpha.GetCurSel();
	m_nComboLinkAllBetaCursor	= m_comboLinkBeta.GetCurSel();
	m_nComboLinkAllACursor		= m_comboLinkA.GetCurSel();
	m_nComboLinkAllBCursor		= m_comboLinkB.GetCurSel();

	GenerateLinkQuery();

	KDialogEx::OnOK();
}


void KABPRFnDlg::OnBnClickedButtonSetBPRFunction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_nComboFnCursor = m_comboFn.GetCurSel();
	int nFnGroupID   = m_comboFn.GetItemData(m_nComboFnCursor);
	
	KABPRFnManagerDlg oDlg(m_pTarget, nFnGroupID);
	if(oDlg.DoModal() == IDOK)
	{
		m_nFnGroupID = oDlg.FnGroupID();

		InitComboBoxEachFn();
		OnSelchangeCombo1();
	}
}

void KABPRFnDlg::InitComboBoxEachFn( void )
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

		int nCursorIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboFn.InsertString(-1, _T("------------선택------------"));
		}
		else {
			nCursorIndex = m_comboFn.InsertString(-1, _T("-----------Select-----------"));
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


void KABPRFnDlg::InitComboBoxEachLink( void )
{
	int nAlphaIndex(0);
	int nBetaIndex (0);
	int nAIndex	   (0);
	int nBIndex	   (0);

	try
	{
		m_comboLinkAlpha.ResetContent();
		m_comboLinkBeta.ResetContent();
		m_comboLinkA.ResetContent();
		m_comboLinkB.ResetContent();

		int nCursorIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("--------선택--------"));
		}
		else {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("-------Select-------"));
		}
		m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("--------선택--------"));
		}
		else {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("-------Select-------"));
		}
		//nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("-----Select-----"));
		m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("--------선택--------"));
		}
		else {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("-------Select-------"));
		}
		//nCursorIndex = m_comboLinkA.InsertString(-1, _T("-----Select-----"));
		m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("--------선택--------"));
		}
		else {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("-------Select-------"));
		}
		//nCursorIndex = m_comboLinkB.InsertString(-1, _T("-----Select-----"));
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

				nCursorIndex = m_comboLinkBeta.InsertString(-1, pColumn->Caption());
				m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				nCursorIndex = m_comboLinkA.InsertString(-1, pColumn->Caption());
				m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				nCursorIndex = m_comboLinkB.InsertString(-1, pColumn->Caption());
				m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				CString strColumnName = pColumn->Name();
				if (strColumnName.CompareNoCase(COLUMN_LINK_ALPHA) == 0)
				{
					nAlphaIndex = nCursorIndex;
					m_comboLinkAlpha.SetCurSel(nAlphaIndex);
				}
				else if (strColumnName.CompareNoCase(COLUMN_LINK_BETA) == 0)
				{
					nBetaIndex = nCursorIndex;
					m_comboLinkBeta.SetCurSel(nBetaIndex);
				}
				else if (strColumnName.CompareNoCase(COLUMN_LINK_A) == 0)
				{
					nAIndex = nCursorIndex;
					m_comboLinkA.SetCurSel(nAIndex);
				}
				else if (strColumnName.CompareNoCase(COLUMN_LINK_B) == 0)
				{
					nBIndex = nCursorIndex;
					m_comboLinkB.SetCurSel(nBIndex);
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

	if (m_nComboLinkAllACursor == 0)
		m_comboLinkAlpha.SetCurSel(nAlphaIndex);
	else
		m_comboLinkAlpha.SetCurSel(m_nComboLinkAllAlphaCursor);
	
	if (m_nComboLinkAllBetaCursor == 0)
		m_comboLinkBeta.SetCurSel(nBetaIndex);
	else
		m_comboLinkBeta.SetCurSel(m_nComboLinkAllBetaCursor);
	
	if (m_nComboLinkAllACursor == 0)
		m_comboLinkA.SetCurSel(nAIndex);
	else
		m_comboLinkA.SetCurSel(m_nComboLinkAllACursor);

	if (m_nComboLinkAllBCursor == 0)
		m_comboLinkB.SetCurSel(nBIndex);
	else
		m_comboLinkB.SetCurSel(m_nComboLinkAllBCursor);
}


void KABPRFnDlg::OnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	UpdateReportCtrl();
	UpdateReportCtrlByEachLink();

}

void KABPRFnDlg::InitReportCtrl( void )
{
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

void KABPRFnDlg::UpdateReportCtrl( void )
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
		
		CString strFuntionNo = vecRow[0];
		double dAlpha		 = _ttof(vecRow[1]);
		double dBeta		 = _ttof(vecRow[2]);
		double dA			 = _ttof(vecRow[3]);
		double dB            = _ttof(vecRow[4]);

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(strFuntionNo));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(dAlpha, _T("%.3f")));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(dBeta , _T("%.3f")));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(dA    , _T("%.3f")));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(dB    , _T("%.3f")));
	}

	m_wndReport.Populate();
}

void KABPRFnDlg::UpdateReportCtrlByEachLink( void )
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

bool KABPRFnDlg::WriteInputArgument( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		strOut.Format(_T("vdf\t%d\r\n"), 1);
		of.WriteString(strOut);

		strOut.Format(_T("%d\t%f\r\n"), m_emBPRFnType, m_dVOT);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
	
}

void KABPRFnDlg::OnRadioClicked( UINT nID )
{
	switch(nID)
	{
	case IDC_RADIO1:
		m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;
		break;

	case IDC_RADIO2:
		m_emBPRFnType = KEMAssignmentBPRFunctionEachFn;
		break;

	case IDC_RADIO3:
		m_emBPRFnType = KEMAssignmentBPRFunctionEachLink;
		break;
	}

	UpdateCtrlState();
}

void KABPRFnDlg::UpdateCtrlState( void )
{
	if (m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
	{
		m_edtAllAlpha.EnableWindow(TRUE);
		m_edtAllBeta.EnableWindow(TRUE);

		m_comboFn.EnableWindow(FALSE);
		GetDlgItem(ID_BUTTON_SETBPRFN)->EnableWindow(FALSE);

		m_comboLinkAlpha.EnableWindow(FALSE);
		m_comboLinkBeta.EnableWindow(FALSE);
		m_comboLinkA.EnableWindow(FALSE);
		m_comboLinkB.EnableWindow(FALSE);
	}
	else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
	{
		m_edtAllAlpha.EnableWindow(FALSE);
		m_edtAllBeta.EnableWindow(FALSE);

		m_comboFn.EnableWindow(TRUE);
		GetDlgItem(ID_BUTTON_SETBPRFN)->EnableWindow(TRUE);

		m_comboLinkAlpha.EnableWindow(FALSE);
		m_comboLinkBeta.EnableWindow(FALSE);
		m_comboLinkA.EnableWindow(FALSE);
		m_comboLinkB.EnableWindow(FALSE);
	}
	else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
	{
		m_edtAllAlpha.EnableWindow(FALSE);
		m_edtAllBeta.EnableWindow(FALSE);

		m_comboFn.EnableWindow(FALSE);
		GetDlgItem(ID_BUTTON_SETBPRFN)->EnableWindow(FALSE);

		m_comboLinkAlpha.EnableWindow(TRUE);
		m_comboLinkBeta.EnableWindow(TRUE);
		m_comboLinkA.EnableWindow(TRUE);
		m_comboLinkB.EnableWindow(TRUE);
	}
}

void KABPRFnDlg::Target( KTarget* a_pTarget )
{
	m_pTarget = a_pTarget;
}

bool KABPRFnDlg::ExportDB2LinkFile( KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString a_strAddQuery, CString strFileName/*=_T("link.dat")*/ )
{
	if (m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
	{
		TxLogDebugStartMsg();

		ofstream ofs(strFileName, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id");
		strSQL.Append(_T(" From link"));
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			TNewFixedLink fixedLink;

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
				fixedLink.alpha		= m_dAllLinkAlpha;
				fixedLink.beta		= m_dAllLinkBeta;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TNewFixedLink) );

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
	else if (m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
	{
		TxLogDebugStartMsg();

		bool bUseAlpha = true;
		bool bUseBeta  = true;
		bool bUseA     = true;
		bool bUseB     = true;


		//GenerateQuery;
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		CString strQuery, strAddQuery, strTemp;
		strQuery.Format(_T("SELECT alpha_column_name, beta_column_name, a_column_name, b_column_name FROM function_group WHERE function_group_id = %d"), m_nEachFnCombo);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("데이터가 없습니다."));

		//Alpha
		strTemp = spResultSet->GetValueString(0);

		if (strTemp.IsEmpty())
		{
			bUseAlpha = false;
			strTemp   = _T("''");
		}

		strAddQuery.Format(_T(", %s as Alpha"), strTemp);

		//Beta
		strTemp = spResultSet->GetValueString(1);

		if (strTemp.IsEmpty())
		{
			bUseBeta = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as Beta"), strTemp);

		//a
		strTemp = spResultSet->GetValueString(2);

		if (strTemp.IsEmpty())
		{
			bUseA = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as a"), strTemp);

		//b
		strTemp = spResultSet->GetValueString(3);

		if (strTemp.IsEmpty())
		{
			bUseB = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as b"), strTemp);

		//Start
		ofstream ofs(strFileName, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		std::map<int, std::vector<double>>::iterator fIter, eIter = m_mapEachFN.end();

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;
		strSQL.Append(strAddQuery);
		strSQL.Append(_T(" FROM link"));
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			TNewFixedLink fixedLink;

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


				fIter = m_mapEachFN.find(fixedLink.fuNumber);

				if (fIter == eIter)
					ThrowException(_T("정의되지 않은 functionID입니다."));

				std::vector<double> vecEachFn = fIter->second;

				if (bUseAlpha)
				{
					fixedLink.alpha	= spResultSet->GetValueDouble(9);
				}
				else
				{
					fixedLink.alpha = vecEachFn[0];
				}

				if (bUseBeta)
				{
					fixedLink.beta = spResultSet->GetValueDouble(10);
				}
				else
				{
					fixedLink.beta = vecEachFn[1];
				}

				if (bUseA)
				{
					fixedLink.a	= spResultSet->GetValueDouble(11);
				}
				else
				{
					fixedLink.a = vecEachFn[2];
				}

				if (bUseB)
				{
					fixedLink.b	= spResultSet->GetValueDouble(12);
				}
				else
				{
					fixedLink.b = vecEachFn[3];
				}				

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TNewFixedLink) );

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


		TxLogDebugEndMsg();
	}
	else	//KEMAssignmentBPRFunctionEachLink
	{
		TxLogDebugStartMsg();

		ofstream ofs(strFileName, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}


		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;
		strSQL.Append(a_strAddQuery);
		strSQL.Append(_T(" FROM link"));
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			TNewFixedLink fixedLink;

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
				fixedLink.a			= spResultSet->GetValueDouble(11);
				fixedLink.b			= spResultSet->GetValueDouble(12);

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TNewFixedLink) );
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


void KABPRFnDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}


void KABPRFnDlg::GenerateLinkQuery( void )
{
	CString strTemp;

	if (m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
	{
		KIOColumn* pColumn = (KIOColumn*)m_comboLinkAlpha.GetItemData(m_comboLinkAlpha.GetCurSel());
		if (pColumn == NULL)
		{
			m_strAddQuery.Append(_T(", 0 as Alpha"));
		}
		else
		{
			m_strCaptionAlpha = pColumn->Caption();
			strTemp.Format(_T(", %s as Alpha"), pColumn->Name());
			m_strAddQuery.Append( strTemp );
		}

		pColumn = (KIOColumn*)m_comboLinkBeta.GetItemData(m_comboLinkBeta.GetCurSel());
		if (pColumn == NULL)
		{
			m_strAddQuery.Append(_T(", 0 as Beta"));
		}
		else
		{
			m_strCaptionBeta = pColumn->Caption();
			strTemp.Format(_T(", %s as Beta"), pColumn->Name());
			m_strAddQuery.Append( strTemp );
		}

		pColumn = (KIOColumn*)m_comboLinkA.GetItemData(m_comboLinkA.GetCurSel());
		if (pColumn == NULL)
		{
			m_strAddQuery.Append(_T(", 0 as A"));
		}
		else
		{
			m_strCaptionA = pColumn->Caption();
			strTemp.Format(_T(", %s as A"), pColumn->Name());
			m_strAddQuery.Append( strTemp );
		}

		pColumn = (KIOColumn*)m_comboLinkB.GetItemData(m_comboLinkB.GetCurSel());
		if (pColumn == NULL)
		{
			m_strAddQuery.Append(_T(", 0 as B"));
		}
		else
		{
			m_strCaptionB = pColumn->Caption();
			strTemp.Format(_T(", %s as B"), pColumn->Name());
			m_strAddQuery.Append( strTemp );
		}
	}
	else if (m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
	{

	}
}

bool KABPRFnDlg::ExportDB2LinkExcludeType( KDBaseConPtr a_spDBaseConnection, CString a_strAddQuery, std::set<Integer> a_setIncludeLink,CString strFileName/*=_T("link.dat")*/ )
{
	if (m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
	{
		TxLogDebugStartMsg();

		ofstream ofs(strFileName, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id");
		strSQL.Append(_T(" From link"));
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			TNewFixedLink fixedLink;

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
				fixedLink.alpha		= m_dAllLinkAlpha;
				fixedLink.beta		= m_dAllLinkBeta;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if(iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TNewFixedLink) );
				}
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
	else if (m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
	{
		TxLogDebugStartMsg();
		
		bool bUseAlpha = true;
		bool bUseBeta  = true;
		bool bUseA     = true;
		bool bUseB     = true;


		//GenerateQuery;
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		CString strQuery, strAddQuery, strTemp;
		strQuery.Format(_T("SELECT alpha_column_name, beta_column_name, a_column_name, b_column_name FROM function_group WHERE function_group_id = %d"), m_nEachFnCombo);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("데이터가 없습니다."));

		//Alpha
		strTemp = spResultSet->GetValueString(0);

		if (strTemp.IsEmpty())
		{
			bUseAlpha = false;
			strTemp   = _T("''");
		}
		
		strAddQuery.Format(_T(", %s as Alpha"), strTemp);

		//Beta
		strTemp = spResultSet->GetValueString(1);

		if (strTemp.IsEmpty())
		{
			bUseBeta = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as Beta"), strTemp);

		//a
		strTemp = spResultSet->GetValueString(2);

		if (strTemp.IsEmpty())
		{
			bUseA = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as a"), strTemp);

		//b
		strTemp = spResultSet->GetValueString(3);

		if (strTemp.IsEmpty())
		{
			bUseB = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as b"), strTemp);

		//Start
		ofstream ofs(strFileName, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		std::map<int, std::vector<double>>::iterator fIter, eIter = m_mapEachFN.end();

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;
		strSQL.Append(strAddQuery);
		strSQL.Append(_T(" FROM link"));
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			TNewFixedLink fixedLink;

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

				
				fIter = m_mapEachFN.find(fixedLink.fuNumber);

				if (fIter == eIter)
					ThrowException(_T("정의되지 않은 functionID입니다."));
				
				std::vector<double> vecEachFn = fIter->second;

				if (bUseAlpha)
				{
					fixedLink.alpha	= spResultSet->GetValueDouble(9);
				}
				else
				{
					fixedLink.alpha = vecEachFn[0];
				}

				if (bUseBeta)
				{
					fixedLink.beta = spResultSet->GetValueDouble(10);
				}
				else
				{
					fixedLink.beta = vecEachFn[1];
				}
				
				if (bUseA)
				{
					fixedLink.a	= spResultSet->GetValueDouble(11);
				}
				else
				{
					fixedLink.a = vecEachFn[2];
				}
				
				if (bUseB)
				{
					fixedLink.b	= spResultSet->GetValueDouble(12);
				}
				else
				{
					fixedLink.b = vecEachFn[3];
				}				

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if(iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TNewFixedLink) );
				}
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


		TxLogDebugEndMsg();
	}
	else	//KEMAssignmentBPRFunctionEachLink
	{
		TxLogDebugStartMsg();

		ofstream ofs(strFileName, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}


		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;
		strSQL.Append(a_strAddQuery);
		strSQL.Append(_T(" FROM link"));
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			TNewFixedLink fixedLink;
			
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
				fixedLink.a			= spResultSet->GetValueDouble(11);
				fixedLink.b			= spResultSet->GetValueDouble(12);

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if(iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TNewFixedLink) );
				}
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

void KABPRFnDlg::InitButtonContol()
{
	m_nComboLinkAllAlphaCursor = 0;
	m_nComboLinkAllBetaCursor  = 0;
	m_nComboLinkAllACursor	   = 0;
	m_nComboLinkAllBCursor	   = 0;
	m_dAllLinkAlpha = 0.15;
	m_dAllLinkBeta = 4.0;
	m_nComboFnCursor = 0;
	m_nFnGroupID =0;
}

void KABPRFnDlg::DefaultComboBoxEachLink()
{
	try
	{
		m_comboLinkAlpha.ResetContent();
		m_comboLinkBeta.ResetContent();
		m_comboLinkA.ResetContent();
		m_comboLinkB.ResetContent();

		int nAlphaIndex(0);
		int nBetaIndex (0);
		int nAIndex	   (0);
		int nBIndex	   (0);

		//int nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("------ Select ------"));
		int nCursorIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("--------선택--------"));
		}
		else {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("-------Select-------"));
		}
		m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("--------선택--------"));
		}
		else {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("-------Select-------"));
		}
		//nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("------ Select ------"));
		m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("--------선택--------"));
		}
		else {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("-------Select-------"));
		}
		//nCursorIndex = m_comboLinkA.InsertString(-1, _T("------ Select ------"));
		m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("--------선택--------"));
		}
		else {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("-------Select-------"));
		}
		//nCursorIndex = m_comboLinkB.InsertString(-1, _T("------ Select ------"));
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
			CString strSelect           = (_T("-------Select-------"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strSelect = _T("--------선택--------");
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
			KIOTables* pTables = m_pTarget->Tables();
			KIOTable*  pTable = pTables->FindTable(TABLE_LINK);

			const KIOColumns* pColumns = pTable->Columns();

			int nColumnCount = pColumns->ColumnCount();

			for(int i = 0; i<nColumnCount; ++i)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);
				if(pColumn->DataType() == KEMIODataTypeDouble)
				{
					nCursorIndex = m_comboLinkAlpha.InsertString(-1, pColumn->Caption());
					m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					nCursorIndex = m_comboLinkBeta.InsertString(-1, pColumn->Caption());
					m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					nCursorIndex = m_comboLinkA.InsertString(-1, pColumn->Caption());
					m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					nCursorIndex = m_comboLinkB.InsertString(-1, pColumn->Caption());
					m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					CString strColumnName = pColumn->Name();
					if (strColumnName.CompareNoCase(COLUMN_LINK_ALPHA) == 0)
					{
						nAlphaIndex = nCursorIndex;
						m_comboLinkAlpha.SetCurSel(nAlphaIndex);
					}
					else if (strColumnName.CompareNoCase(COLUMN_LINK_BETA) == 0)
					{
						nBetaIndex = nCursorIndex;
						m_comboLinkBeta.SetCurSel(nBetaIndex);
					}
					else if (strColumnName.CompareNoCase(COLUMN_LINK_A) == 0)
					{
						nAIndex = nCursorIndex;
						m_comboLinkA.SetCurSel(nAIndex);
					}
					else if (strColumnName.CompareNoCase(COLUMN_LINK_B) == 0)
					{
						nBIndex = nCursorIndex;
						m_comboLinkB.SetCurSel(nBIndex);
					}
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

void KABPRFnDlg::DefaultComboBoxEachFn()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (spDBaseConnection == NULL)
	{
		TxLogDebugException();
		return;
	}

	try
	{
		AutoType iterDefaultVDFFn = m_mapDefaultVDFPara.find(1);
		AutoType endDefaultVDFFn  = m_mapDefaultVDFPara.end();

		if (iterDefaultVDFFn != endDefaultVDFFn)
		{
			KVDFPara oVDFPara = iterDefaultVDFFn->second;
			m_strUseFunctionGroup	= oVDFPara.strUseFunctionID;
		}
		else
		{
			AutoType iterSystemVDFFn = m_mapDefaultVDFPara.find(0);
			AutoType endSystemVDFFn  = m_mapDefaultVDFPara.end();

			if(iterSystemVDFFn!=endSystemVDFFn)
			{
				KVDFPara oVDFPara = iterSystemVDFFn->second;
				m_strUseFunctionGroup	= oVDFPara.strUseFunctionID;
			}
		}

		AutoType iterGroup = m_mapDefaultVDFFuntionGroup.find(m_strUseFunctionGroup);
		AutoType endGroup  = m_mapDefaultVDFFuntionGroup.end();
		if (iterGroup != endGroup)
		{
			int nUseFunctionID = iterGroup->second;

			CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
			KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

			m_comboFn.ResetContent();
			
			//int nCursorIndex = m_comboFn.InsertString(-1, _T("------ Select --------"));
			int nCursorIndex = 0;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				nCursorIndex = m_comboFn.InsertString(-1, _T("------------선택------------"));
			}
			else {
				nCursorIndex = m_comboFn.InsertString(-1, _T("-----------Select-----------"));
			}
			m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)0 );

			while (oResultSetPtr->Next())
			{
				nCursorIndex = m_comboFn.InsertString(-1, oResultSetPtr->GetValueString(1));
				m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );
			}

			if( nUseFunctionID != 0)
			{
				int nComboCount = m_comboFn.GetCount();

				for (int i = 0; i < nComboCount; ++i)
				{
					int nFnGroupId = m_comboFn.GetItemData(i);

					if ( nFnGroupId == nUseFunctionID )
					{
						m_comboFn.SetCurSel(i);
					}
				}
			}
			else
			{
				m_comboFn.SetCurSel(1);
			}
		}
		else
		{
			m_comboFn.ResetContent();

			//int nZeroCursorIndex = m_comboFn.InsertString(-1, _T("------ Select --------"));
			int nZeroCursorIndex = 0;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				nZeroCursorIndex = m_comboFn.InsertString(-1, _T("------------선택------------"));
			}
			else {
				nZeroCursorIndex = m_comboFn.InsertString(-1, _T("-----------Select-----------"));
			}
			m_comboFn.SetItemData( nZeroCursorIndex, (DWORD_PTR)0 );

			CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
			KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

			while (oResultSetPtr->Next())
			{
				int nCursorIndex = m_comboFn.InsertString(-1, oResultSetPtr->GetValueString(1));
				m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );
			}

			m_comboFn.SetCurSel(nZeroCursorIndex);
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

	UpdateReportCtrl();
}

void KABPRFnDlg::DefaultVDFSetting()
{
	try
	{
		AutoType iterODDefault = m_mapDefaultODTime.find(1);
		AutoType endODDefault  = m_mapDefaultODTime.end();

		AutoType iterVDFDefault = m_mapDefaultVDFPara.find(1);
		AutoType endVDFDefault  = m_mapDefaultVDFPara.end();

		if ( (iterVDFDefault != endVDFDefault) && (iterODDefault != endODDefault) )
		{
			KODTime oODTime		    = iterODDefault->second;

			double  dTravelTime 	= oODTime.dTravelTime;

			KVDFPara oVDFPara       = iterVDFDefault->second;

			double  dAllLinkAlpha	= oVDFPara.dAllLinkAlpha;
			double	dAllLinkBeta	= oVDFPara.dAllLinkBeta;

			CString strValue(_T(""));
			strValue.Format(_T("%.3f"), dTravelTime);
			m_edtVOT.SetWindowText(strValue);
			m_edtVOT.EnableRealNumber(true);
			m_edtVOT.EnableMinMax(0.0, 10000.0);

			strValue.Format(_T("%.4f"), dAllLinkAlpha);
			m_edtAllAlpha.SetWindowText(strValue);
			m_edtAllAlpha.EnableRealNumber(true);

			strValue.Format(_T("%.4f"), dAllLinkBeta);
			m_edtAllBeta.SetWindowText(strValue);
			m_edtAllBeta.EnableRealNumber(true);
		}
		else
		{
			AutoType iterODDefault = m_mapDefaultODTime.find(0);
			AutoType endODDefault  =m_mapDefaultODTime.end();

			AutoType iterVDFDefault = m_mapDefaultVDFPara.find(0);
			AutoType endVDFDefault  = m_mapDefaultVDFPara.end();
		    if ( (iterVDFDefault != endVDFDefault) && (iterODDefault != endODDefault) )
			{
				KODTime oODTime		    = iterODDefault->second;

				double  dTravelTime 	= oODTime.dTravelTime;

				KVDFPara oVDFPara       = iterVDFDefault->second;

				double  dAllLinkAlpha	= oVDFPara.dAllLinkAlpha;
				double	dAllLinkBeta	= oVDFPara.dAllLinkBeta;

				CString strValue(_T(""));
				strValue.Format(_T("%.3f"), dTravelTime);
				m_edtVOT.SetWindowText(strValue);
				m_edtVOT.EnableRealNumber(true);
				m_edtVOT.EnableMinMax(0.0, 10000.0);

				strValue.Format(_T("%.4f"), dAllLinkAlpha);
				m_edtAllAlpha.SetWindowText(strValue);
				m_edtAllAlpha.EnableRealNumber(true);

				strValue.Format(_T("%.4f"), dAllLinkBeta);
				m_edtAllBeta.SetWindowText(strValue);
				m_edtAllBeta.EnableRealNumber(true);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return; 
}

void KABPRFnDlg::DefaultVDFReportCtrl()
{
	KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntion(m_mapDefaultVDFFuntion);

	int nFnGroupID = m_comboFn.GetItemData(m_comboFn.GetCurSel());
	CString strFnGroupName = (_T(""));

	if( 0 == nFnGroupID )
	{
		return;
	}

	try
	{
		m_wndReport.ResetContent();

		CXTPReportRecords*		pRecords		 = m_wndReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		AutoType findVDFFn = m_mapDefaultVDFFuntion.find(nFnGroupID);
		AutoType endVDFFn  = m_mapDefaultVDFFuntion.end();

		if (findVDFFn != endVDFFn)
		{
			std::vector<KVDFFunction>& vecVDFFunction = findVDFFn->second;
			size_t nUserCount   = vecVDFFunction.size();

			for (size_t i =0; i < nUserCount; i++)
			{
				KVDFFunction oVDFFunction = vecVDFFunction[i];
				int nFn					  = oVDFFunction.nFn;
				double dAlpha			  = oVDFFunction.dAlpha;
				double dBeta			  = oVDFFunction.dBeta;
				double dA				  = oVDFFunction.dA;
				double dB				  = oVDFFunction.dB;

				pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

				pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(nFn));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextRight);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dAlpha, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextRight);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBeta, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextRight);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dA, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextRight);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dB, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextRight);
				pItem ->SetEditable(TRUE);
			}
		}
		m_wndReport.Populate();
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

void KABPRFnDlg::DefaultRadioButton()
{
	try
	{
// 		AutoType iterDefault = m_mapDefaultVDFPara.find(1);
// 		AutoType endDefault  = m_mapDefaultVDFPara.end();
// 
// 		if ( iterDefault != endDefault )
// 		{
// 			KVDFPara oVDFPara = iterDefault->second;
// 
// 			int nVDFChoice		    = oVDFPara.nVDFChoice;
// 
// 			if((nVDFChoice +1) == KEMAssignmentBPRFunctionAllLink)
// 			{
// 				CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
// 				m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;
// 			}
// 			else if((nVDFChoice +1) == KEMAssignmentBPRFunctionEachFn)
// 			{
// 				CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
// 				m_emBPRFnType = KEMAssignmentBPRFunctionEachFn;
// 			}
// 			else if((nVDFChoice +1) == KEMAssignmentBPRFunctionEachLink)
// 			{
// 				CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
// 				m_emBPRFnType = KEMAssignmentBPRFunctionEachLink;
// 			}
// 		}
// 		else
// 		{
// 			AutoType iterSystem = m_mapDefaultVDFPara.find(0);
// 			AutoType endSystem  = m_mapDefaultVDFPara.end();
// 
// 			if ( iterSystem != endSystem )
// 			{
// 				KVDFPara oVDFPara = iterSystem->second;
// 
// 				int nVDFChoice		    = oVDFPara.nVDFChoice;
// 
// 				if((nVDFChoice +1) == KEMAssignmentBPRFunctionAllLink)
// 				{
// 					CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
// 					m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;
// 				}
// 				else if((nVDFChoice +1) == KEMAssignmentBPRFunctionEachFn)
// 				{
// 					CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
// 					m_emBPRFnType = KEMAssignmentBPRFunctionEachFn;
// 				}
// 				else if((nVDFChoice +1) == KEMAssignmentBPRFunctionEachLink)
// 				{
// 					CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
// 					m_emBPRFnType = KEMAssignmentBPRFunctionEachLink;
// 				}
// 			}
// 			else
// 			{
// 				if(m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
// 				{
// 					CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
// 				}
// 				else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
// 				{
// 					CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
// 				}
// 				else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
// 				{
// 					CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
// 				}
// 			}
// 		}

		if(m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
		{
			CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
		}
		else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
		{
			CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
		}
		else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
		{
			CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
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

void KABPRFnDlg::OnBnClickedDefault()
{
	try
	{
		//DefaultRadioButton();

		AutoType beginODDefault  = m_mapDefaultODTime.begin();
		AutoType endODDefault    = m_mapDefaultODTime.end();

		AutoType beginVDFDefault = m_mapDefaultVDFPara.begin();
		AutoType endVDFDefault   = m_mapDefaultVDFPara.end();

		if((beginODDefault != endODDefault) && (beginVDFDefault != endVDFDefault))
		{
			DefaultVDFSetting();

			AutoType beginVDFParaDefault = m_mapDefaultVDFPara.begin();
			AutoType endVDFParaDefault   = m_mapDefaultVDFPara.end();

			if(beginVDFParaDefault!=endVDFParaDefault)
			{
				DefaultComboBoxEachLink();
				UpdateCtrlState();

				m_wndReport.GetColumns()->Clear();

				KReportCtrlSetting::Default(m_wndReport);

				InitReportCtrl();

				AutoType beginGroup  = m_mapDefaultVDFFuntionGroup.begin();
				AutoType endGroup  = m_mapDefaultVDFFuntionGroup.end();

				if (beginGroup != endGroup)
				{
					DefaultComboBoxEachFn();
					DefaultVDFReportCtrl();
				}
				else
				{
					//AfxMessageBox(_T("Default 정보가 없습니다. "));
				}
			}
			else
			{
				//AfxMessageBox(_T("Default 정보가 없습니다. "));
			}
		}
		else
		{
			//AfxMessageBox(_T("Default 정보가 없습니다. "));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
		return;
	}	
}

void KABPRFnDlg::UsedDefault( bool a_bUsedDefault )
{
	m_bUsedDefault = a_bUsedDefault;
}


void KABPRFnDlg::OnStnClickedSeprator3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void KABPRFnDlg::ConvertSysToDispColumn( std::vector<CString>& a_vecColName )
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

void KABPRFnDlg::SaveInterModalLinkOption(CString a_strSQL)
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Delete From inter_link_option where type = 1"));
		spDBaseConnection->ExecuteUpdate(strSQL);

		int nVDF(0), nFunctionID(0);

		if (m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
		{
			nVDF = 1;
		}
		else if (m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
		{
			nVDF = 2;
			nFunctionID = m_nEachFnCombo;
		}
		else if (m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
		{
			nVDF = 3;
		}

		strSQL.Format(_T(" Insert Into inter_link_option ( type, VDFChoice, alpha, beta, VDFFunction, LinkFn_Sql)"));
		strSQL.Append(_T(" Values "));
		strSQL.AppendFormat(_T(" ( '%d', '%d', '%f', '%f', '%d', '%s' ) "), 1, nVDF, m_dAllLinkAlpha, m_dAllLinkBeta, nFunctionID, a_strSQL);
		spDBaseConnection->ExecuteUpdate(strSQL);

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

CString KABPRFnDlg::MultiScenarioLinkQuery( KTarget* a_pRunTarget )
{
	TxLogDebugStartMsg();
	CString strAddQuery;
	CString strValue;
	CString strName;

	try
	{
		KIOTables*  pTables  = a_pRunTarget->Tables();
		KIOTable*   pTable   = pTables->FindTable(TABLE_LINK);
		KIOColumns* pColumns = pTable->Columns();

		int nColumns = pColumns->ColumnCount();		
		
		//Alpha
		strName   = m_strCaptionAlpha;
		strValue  = _T("");

		for (int i = 0; i < nColumns; ++i)
		{
			if (strName.CompareNoCase(pColumns->GetColumn(i)->Caption()) == 0)
			{
				strValue.Format(_T(", %s as Alpha"), pColumns->GetColumn(i)->Name());
				break;
			}		
		}

		if (strValue.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("'%s - %s'에 %s 관련 컬럼 정보가 존재하지 않습니다."), a_pRunTarget->CaptionScenario(), a_pRunTarget->CaptionTargetYear(), strName);
			AfxMessageBox(strMsg);

			throw 1;
		}

		strAddQuery = strValue;

		//Beta
		strName   = m_strCaptionBeta;
		strValue  = _T("");

		for (int i = 0; i < nColumns; ++i)
		{
			if (strName.CompareNoCase(pColumns->GetColumn(i)->Caption()) == 0)
			{
				strValue.Format(_T(", %s as Beta"), pColumns->GetColumn(i)->Name());
				break;
			}		
		}

		if (strValue.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("'%s - %s'에 %s 관련 컬럼 정보가 존재하지 않습니다."), a_pRunTarget->CaptionScenario(), a_pRunTarget->CaptionTargetYear(), strName);
			AfxMessageBox(strMsg);

			throw 1;
		}

		strAddQuery.Append(strValue);

		//a
		strName   = m_strCaptionA;
		strValue  = _T("");

		for (int i = 0; i < nColumns; ++i)
		{
			if (strName.CompareNoCase(pColumns->GetColumn(i)->Caption()) == 0)
			{
				strValue.Format(_T(", %s as a"), pColumns->GetColumn(i)->Name());
				break;
			}		
		}

		if (strValue.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("'%s - %s'에 %s 관련 컬럼 정보가 존재하지 않습니다."), a_pRunTarget->CaptionScenario(), a_pRunTarget->CaptionTargetYear(), strName);
			AfxMessageBox(strMsg);

			throw 1;
		}

		strAddQuery.Append(strValue);

		//b
		strName   = m_strCaptionB;
		strValue  = _T("");

		for (int i = 0; i < nColumns; ++i)
		{
			if (strName.CompareNoCase(pColumns->GetColumn(i)->Caption()) == 0)
			{
				strValue.Format(_T(", %s as b"), pColumns->GetColumn(i)->Name());
				break;
			}		
		}

		if (strValue.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("'%s - %s'에 %s 관련 컬럼 정보가 존재하지 않습니다."), a_pRunTarget->CaptionScenario(), a_pRunTarget->CaptionTargetYear(), strName);
			AfxMessageBox(strMsg);

			throw 1;
		}

		strAddQuery.Append(strValue);
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;	
	}

	TxLogDebugEndMsg();
	return strAddQuery;
}
