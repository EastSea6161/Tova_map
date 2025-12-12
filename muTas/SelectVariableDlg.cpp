// SelectVariableDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SelectVariableDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
#include "IOTableController.h"
//^^ #include "IOTables.h"

// KSelectVariableDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSelectVariableDlg, KDialogEx)

KSelectVariableDlg::KSelectVariableDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KSelectVariableDlg::IDD, pParent)
{

}

KSelectVariableDlg::~KSelectVariableDlg()
{
}

void KSelectVariableDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KSelectVariableDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KSelectVariableDlg::OnBnClickedOk)
END_MESSAGE_MAP()



BOOL KSelectVariableDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitReport();
	UpdateReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KSelectVariableDlg::InitReport()
{
	TxLogDebugStartMsg();
	CXTPReportColumn* pColumn = NULL;
	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Variable(Applied)"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("% of Change"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	//pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Infomation"), 50));
	//pColumn->SetHeaderAlignment(DT_CENTER);

	m_wndReportCtrl.AllowEdit(TRUE);
	m_wndReportCtrl.EditOnClick(TRUE);
	m_wndReportCtrl.SetMultipleSelection(FALSE);
	m_wndReportCtrl.Populate();
	TxLogDebugEndMsg();
}


void KSelectVariableDlg::ParameterInfo( std::vector<TUseColumnInfo> vecUseColumnInfo )
{
	TxLogDebugStartMsg();
	m_mapUseColumnInfo.clear();

	size_t nCount = vecUseColumnInfo.size();

	for (size_t i = 0; i < nCount; i ++)
	{
		TUseColumnInfo oColumnInfo = vecUseColumnInfo[i];
		std::vector<CString> vecColumnName = oColumnInfo.vecUsedColumnName;
		CString strPurposeName	= oColumnInfo.strPurposeName;
		CString strModeName		= oColumnInfo.strModeNodeName;

		CString strInfo = _T("");
		strInfo.Format(_T("%s(%s)"), strPurposeName, strModeName);

		size_t nIndex = vecColumnName.size();
		for (size_t j = 0; j < nIndex; j++)
		{
			CString strColumnName = vecColumnName[j];
			CString strColumnCaption = FindColumnCaption(strColumnName);

			std::map<CString, CString>::iterator iter = m_mapUseColumnInfo.find(strColumnName);
			std::map<CString, CString>::iterator end  = m_mapUseColumnInfo.end();
			if (iter == end)
			{
				m_mapUseColumnInfo.insert(std::make_pair(strColumnCaption, strInfo));
			}
			else
			{
				CString strInformation = iter->second;
				CString strAddInfo = _T("");
				strAddInfo.Format(_T(", %s"), strInfo);
				strInformation += strAddInfo;
				iter->second = strInformation;
			}
		}
	}
	TxLogDebugEndMsg();
}

void KSelectVariableDlg::UpdateReport()
{
	TxLogDebugStartMsg();
	m_wndReportCtrl.ResetContent(TRUE);

	CXTPReportColumns* pColumns = m_wndReportCtrl.GetColumns();

	std::map<CString, CString>::iterator iter = m_mapUseColumnInfo.begin();
	std::map<CString, CString>::iterator end  = m_mapUseColumnInfo.end();
	while(iter != end)
	{
		CString strColumnName = iter->first;
		CString strInfomation = iter->second;

		CXTPReportRecord* pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetCaption(strColumnName);
		TxLogDebug((LPCTSTR)strColumnName);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(100.0, _T("%.1f")));

		//pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		//pItem->SetAlignment(DT_CENTER);
		//pItem->SetCaption(strInfomation);
		++iter;
	}

	m_wndReportCtrl.Populate();
	m_wndReportCtrl.ExpandAll(TRUE);
	TxLogDebugEndMsg();
}

CString KSelectVariableDlg::FindColumnCaption(CString a_strColumnName)
{
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable        = pIOTables->FindTable(TABLE_PARAMETER_OD);
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
	int nColumnCount = pColumns->ColumnCount();
	int nIndex = 0;

	CString strColumnCaption = _T("");

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->DataType() != KEMIODataTypeDouble)
		{
			continue;
		}

		CString strColumnName = pColumn->Name();
		if (a_strColumnName == strColumnName)
		{
			strColumnCaption = pColumn->Caption();
			return strColumnCaption;
		}
	}

	return strColumnCaption;

}

CString KSelectVariableDlg::FindColumnName( CString a_strColumnCaption )
{
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable        = pIOTables->FindTable(TABLE_PARAMETER_OD);
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
	int nColumnCount = pColumns->ColumnCount();
	int nIndex = 0;

	CString strColumnName = _T("");

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->DataType() != KEMIODataTypeDouble)
		{
			continue;
		}

		CString strColumnCaption = pColumn->Caption();
		if (a_strColumnCaption == strColumnCaption)
		{
			strColumnName = pColumn->Name();
			return strColumnName;
		}
	}
	return strColumnName;
}


void KSelectVariableDlg::OnBnClickedOk()
{
	KDialogEx::OnOK();
}

void KSelectVariableDlg::GetChangedFunction(std::map<CString, CString>& a_mapFunction)
{
	TxLogDebugStartMsg();
	a_mapFunction.clear();

	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	
	int nRecordCount   = pRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord   = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = NULL;

		pItem = pRecord->GetItem(0);
		CString strOriginColumn = pItem->GetCaption(NULL);
		CString strColumnName = FindColumnName(strOriginColumn);

		pItem = pRecord->GetItem(1);
		CString strRatio = pItem->GetCaption(NULL);
		double dValue = _wtof(strRatio) / 100;
		CString strChangeColumn = _T("");
		strChangeColumn.Format(_T("%s*%f"), strColumnName, dValue);

		a_mapFunction.insert(std::make_pair(strColumnName, strChangeColumn));
	}
	TxLogDebugEndMsg();
	
}
