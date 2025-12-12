// PrepareChartNetworkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PrepareChartNetworkDlg.h"
#include "afxdialogex.h"
//^#include "DBaseConnector.h"
#include "KExRecordItem.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"

enum
{
	_0_COLUMN_SELECT     = 0,
	_1_COLUMN_FIELDNAME  = 1
};

// KPrepareChartNetwork 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPrepareChartNetworkDlg, KResizeDialogEx)

KPrepareChartNetworkDlg::KPrepareChartNetworkDlg(KTarget* a_pTarget, CString a_strTableName, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPrepareChartNetworkDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{
	KIOTables* pIOTables = m_pTarget->Tables();

	m_pTable = pIOTables->FindTable(a_strTableName);
	m_strTableName = a_strTableName;
}

KPrepareChartNetworkDlg::~KPrepareChartNetworkDlg()
{
	m_pTable->Unregister(this);
}

void KPrepareChartNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_CHARTCONTROL, m_wndChartControl);
} 


BEGIN_MESSAGE_MAP(KPrepareChartNetworkDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ALL,            &KPrepareChartNetworkDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDCANCEL,                 &KPrepareChartNetworkDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CHART, &KPrepareChartNetworkDlg::OnBnClickedButtonChart)
END_MESSAGE_MAP()

// KPrepareChartNetworkDlg 메시지 처리기입니다.

void KPrepareChartNetworkDlg::ShowInformation()
{
	UpdateReportData();
}


void KPrepareChartNetworkDlg::UpdateReportData()
{
	m_wndReportCtrl.ResetContent();

	BOOL bCheckAll(FALSE);
	if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED) {
		bCheckAll = TRUE;
	}

	CString strTableName = m_pTable->Name();
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	m_pIDColumn                   = nullptr;
	m_pNameColumn                 = nullptr;

	const KIOColumns* pIOColumns   = m_pTable->Columns();
	for (int i= 0; i< pIOColumns->ColumnCount(); i++)
	{
		KIOColumn* pIOColumn  = pIOColumns->GetColumn(i);
		CString    strCaption = pIOColumn->Caption();
		CString    strName    = pIOColumn->Name();
	
		if(strTableName.CompareNoCase(TABLE_ZONE) == 0)
		{
			if(strName.CompareNoCase(COLUMN_ZONE_ID) == 0)
			{
				m_pIDColumn = pIOColumn;
			}
			else if(strName.CompareNoCase(COLUMN_ZONE_FULL_NAME) == 0)
			{
				m_pNameColumn = pIOColumn;
			}
		}
		else if(strTableName.CompareNoCase(TABLE_LINK) == 0)
		{
			if(strName.CompareNoCase(COLUMN_LINK_ID) == 0)
			{
				m_pIDColumn = pIOColumn;
			}
			else if(strName.CompareNoCase(COLUMN_LINK_NAME) == 0)
			{
				m_pNameColumn = pIOColumn;
			}
		}
		else if(strTableName.CompareNoCase(TABLE_NODE) == 0)
		{
			if(strName.CompareNoCase(COLUMN_NODE_ID) == 0)
			{
				m_pIDColumn = pIOColumn;
			}
			else if(strName.CompareNoCase(COLUMN_NODE_NAME) == 0)
			{
				m_pNameColumn = pIOColumn;
			}
		}

		/*if(KEMIOColumnTypeSystem == pIOColumn->ColumnType() || KEMIODataTypeDouble != pIOColumn->DataType())*/
		if(KEMIODataTypeDouble != pIOColumn->DataType())
			continue;

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		pItem = pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetChecked(bCheckAll);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(pIOColumn->Caption()));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetItemData((DWORD_PTR)pIOColumn);
	}

	m_wndReportCtrl.Populate();
}


void KPrepareChartNetworkDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);

		if (strSubjectName.CompareNoCase(m_strTableName) == 0)
		{
			//UpdateReportData();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


BOOL KPrepareChartNetworkDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CString strTargetName = m_pTarget->Caption();
	SetDlgItemText(IDC_STATIC_TARGET, strTargetName);
	
	KReportCtrlSetting::Default(m_wndReportCtrl);
	InitReportHeader();

	ResizeComponent();

// 	KIOTableManager*    pIOTables = m_pTarget->Tables();
// 	m_pPurposeODTable = pIOTables->FindTable(m_strTableName);
// 	m_pModeODTable    = pIOTables->FindTable(TABLE_MODE_OD);
// 	m_pParaODTable    = pIOTables->FindTable(TABLE_PARAMETER_OD);
	m_pTable->Register(this);
// 	m_pModeODTable->Register(this);
// 	m_pParaODTable->Register(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPrepareChartNetworkDlg::InitReportHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT,       _T("선택"),        60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_FIELDNAME,    _T("Field"),       100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	m_wndReportCtrl.Populate();
}


void KPrepareChartNetworkDlg::ResizeComponent()
{
// 	SetResize(IDC_SEPRATOR3,				  SZ_TOP_LEFT, SZ_TOP_RIGHT);
// 	SetResize(IDC_REPORT,       			  SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
// 	SetResize(IDC_SEPRATOR4,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
// 	SetResize(IDC_BUTTON_CHART,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
// 	SetResize(IDCANCEL,						  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);

	SetResize(IDC_REPORT,       			  SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR4,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CHART,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,						  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR5,				  SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHARTCONTROL,       		  SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}


void KPrepareChartNetworkDlg::OnBnClickedCheckAll()
{
	try
	{
		BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_ALL);    

		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		int nCount = pRecords->GetCount();

		for (int i=0; i<nCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
			pItem->SetChecked(bCheck);
		}

		m_wndReportCtrl.RedrawControl();
	}
	catch (...)
	{
		return;
	}
}


void KPrepareChartNetworkDlg::OnBnClickedButtonChart()
{
	try
	{
		KIOColumns oSelectedColumns;

		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;
		
		int nCount       = pRecords->GetCount();
		int nSelectedCnt = 0;
		for (int i=0; i<nCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
			
			if( !pItem->IsChecked())
				continue;

			pItem   = pRecord->GetItem(_1_COLUMN_FIELDNAME);
			KIOColumn* pIOCoumn = (KIOColumn*)pItem->GetItemData();
			oSelectedColumns.AddNewColumn(pIOCoumn);
			
			++nSelectedCnt;
		}

		if(nSelectedCnt < 1)
			ThrowException(_T("Field를 선택해 주세요."));

		std::vector<TNetworkChartData> vecNetworkChartData;

		SelectData(oSelectedColumns, vecNetworkChartData);

		CXTPChartContent* pContent = CreateBarChart(vecNetworkChartData);
		m_wndChartControl.SetContent(pContent);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		return;
	}
}


void KPrepareChartNetworkDlg::SelectData(KIOColumns &a_oSelectedColumns, std::vector<TNetworkChartData> &a_vecNetworkChartData)
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strQuery(_T(""));
	strQuery.Format(_T( " SELECT %s, %s " ), m_pIDColumn->Name(), m_pNameColumn->Name());

	for (int i= 0; i< a_oSelectedColumns.ColumnCount(); i++)
	{
		KIOColumn* pSelectedColumn = a_oSelectedColumns.GetColumn(i);
		strQuery.AppendFormat(_T(", %s "), pSelectedColumn->Name());

		TNetworkChartData oTNetworkChartData;
		oTNetworkChartData.strFieldName = pSelectedColumn->Caption();

		a_vecNetworkChartData.push_back(oTNetworkChartData);
	}
	strQuery.Append(_T(" FROM "));
	strQuery.AppendFormat(_T(" %s "), m_pTable->Name());

	try
	{        
		KResultSetPtr spResultSet  = spDBaseConnection->ExecuteQuery(strQuery);
		while ( spResultSet->Next() )
		{
			Integer    nXID      = spResultSet->GetValueInt64 (0);
			CString    strIDName = spResultSet->GetValueString(1);

			for (int i= 0; i< a_oSelectedColumns.ColumnCount(); i++)
			{
				std::vector<TNetworkDBData> &vecNetworkDB = a_vecNetworkChartData[i].vecTData;

				TNetworkDBData oTNetworkDBData;
				oTNetworkDBData.nxID    = nXID;
				oTNetworkDBData.strName = strIDName;
				oTNetworkDBData.dValue  = spResultSet->GetValueDouble(2+i);

				vecNetworkDB.push_back(oTNetworkDBData);
			}
		}
	}
	catch (...)
	{
		ThrowException(_T("정보 조회 중 오류가 발생하였습니다."));
	} 

	return;
}


CXTPChartContent* KPrepareChartNetworkDlg::CreateBarChart(std::vector<TNetworkChartData> &a_vecNetworkChartData)
{
	/*CXTPChartContent*          pContent         = m_wndChartControl.GetContent();*/
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent( pContent, _T("") );
	try
	{
		// Series
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;
		CXTPChartDiagram2D*        pDiagram2D        = nullptr;

		for (size_t i= 0; i< a_vecNetworkChartData.size(); i++)
		{
			CString &strFieldName                   = a_vecNetworkChartData[i].strFieldName;
			auto    &vecTNetworkDBData              = a_vecNetworkChartData[i].vecTData;

			pSeries = pSeriesCollection->Add(new CXTPChartSeries());
			KChartCtrlSetting::DefaultSeriesBar(pSeries, strFieldName);

			CString strXAxisID(_T(""));
			for (size_t k= 0; k< vecTNetworkDBData.size(); k++)
			{
				TNetworkDBData  &oTNetworkDBData     = vecTNetworkDBData[k];
				CString         &strName             = oTNetworkDBData.strName;

				if(strName.Compare(_T("")) == 0)
					strXAxisID.Format(_T("%I64d"), oTNetworkDBData.nxID);
				else
					strXAxisID.Format(_T("%s : %I64d"), oTNetworkDBData.strName, oTNetworkDBData.nxID);  // 라벨 각도가 있을 경우 아이디를 뒷쪽으로

				CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisID, oTNetworkDBData.dValue);  
				pSeries->GetPoints()->Add(pChartPoint);
			}
		}

		pDiagram2D = (CXTPChartDiagram2D*)pSeries->GetDiagram();
		KChartCtrlSetting::DefaultDiagram2D(pDiagram2D, GetIDColumnName(), _T("Value"));
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CString KPrepareChartNetworkDlg::GetIDColumnName()
{
	CString strName(_T(""));

	CString strTableName = m_pTable->Name();

	if (strTableName.CompareNoCase(TABLE_NODE) == 0)
		strName = COLUMN_NODE_ID;
	else if (strTableName.CompareNoCase(TABLE_LINK) == 0)
		strName = COLUMN_LINK_ID;
	else if (strTableName.CompareNoCase(TABLE_ZONE) == 0)
		strName = COLUMN_ZONE_ID;
	
	return strName;
}


void KPrepareChartNetworkDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}