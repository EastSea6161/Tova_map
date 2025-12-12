/// KStatisticsDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KStatisticsDlg.h"
#include "afxdialogex.h"
#include "ExpressionElementField.h"
#include "ExpressionElementFunction.h"
#include "ExpressionElementNumber.h"
#include "ExpressionElementOperator.h"
#include "GRegressionExpression.h"
#include "GenerationInfo.h"
#include "KDBaseStatistics.h"
//^#include "DBaseConnector.h"
//^^ #include "TableOwner.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
//^^ #include "IORecordset.h"
//^^ #include "IORow.h"
//^^ #include "IOItem.h"
//^#include "DevideDataInterval.h"

enum
{
    _0_COLUMN_NAME = 0,    
    _1_COLUMN_SUM,
    _2_COLUMN_AVERAGE,
    _3_COLUMN_MIN,
    _4_COLUMN_MAX,
	_5_COLUMN_VARIANCE,
	_6_COLUMN_STDDEV,
	_7_COLUMN_STDERR
};
#define COLUMN_TOTAL_COUNT 8

// KStatisticsDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KStatisticsDlg, KResizeDialogEx)

KStatisticsDlg::KStatisticsDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KStatisticsDlg::IDD, pParent)
{

}

KStatisticsDlg::~KStatisticsDlg()
{
}

void KStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KStatisticsDlg, KResizeDialogEx)
    ON_BN_CLICKED(IDOK, &KStatisticsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &KStatisticsDlg::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// KStatisticsDlg 메시지 처리기입니다.

BOOL KStatisticsDlg::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();    
    SetDlgItemText(IDC_STATIC_NAME, m_pTable->Caption());

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    InitReportHeader();
	InitReportData();
    m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);    
    KReportCtrlSetting::Default(m_wndReportCtrl, FALSE, FALSE, FALSE);
	m_wndReportCtrl.Populate();
	m_wndReportCtrl.ExpandAll(TRUE);

    SetResize(IDC_REPORT,        SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR1,     SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
    SetResize(IDOK,              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_STATIC_COUNT,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_EXPORT, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KStatisticsDlg::InitReportHeader()
{
	if(NULL == m_pGroupColumn)
	{
		InitReportNormalHeader();
	}
	else
	{
		InitReportGroupHeader();
	}
}


void KStatisticsDlg::InitReportGroupHeader()
{
	CXTPReportPaintManager* pPaint = m_wndReportCtrl.GetPaintManager();
	pPaint->SetColumnStyle(xtpReportColumnFlat);
	pPaint->m_treeStructureStyle = xtpReportTreeStructureSolid;
	//pPaint->SetGridStyle(TRUE, xtpReportGridSolid);
	
	CXTPReportColumn* pColumn = NULL;
	
	m_wndReportCtrl.SelectionEnable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_NAME,     _T("Name"),    20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T(""));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SUM,      _T("SUM"),     15));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("합계"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_AVERAGE,  _T("Average"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("평균"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_MIN,      _T("Min"),     10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("최소"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_MAX,      _T("Max"),     10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("최대"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_5_COLUMN_VARIANCE, _T("Variance"), 15));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("분산"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_6_COLUMN_STDDEV,   _T("Std.Deviation"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("표준편차"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_7_COLUMN_STDERR,   _T("Std.Error"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("표준오차"));
    }

	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);
}


void KStatisticsDlg::InitReportNormalHeader()
{
	CXTPReportColumn* pColumn = NULL;

	//m_wndReportCtrl.FocusSubItems(FALSE);
	m_wndReportCtrl.SelectionEnable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_NAME,     _T("Name"),    15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T(""));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SUM,      _T("SUM"),     15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("합계"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_AVERAGE,  _T("Average"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("평균"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_MIN,      _T("Min"),     11));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("최소"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_MAX,      _T("Max"),     11));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("최대"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_5_COLUMN_VARIANCE, _T("Variance"), 15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("분산"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_6_COLUMN_STDDEV,   _T("Std.Deviation"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("표준편차"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_7_COLUMN_STDERR,   _T("Std.Error"), 11));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("표준오차"));
	}

	/*_7_COLUMN_STDERR*/

	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
}


void KStatisticsDlg::InitReportData()
{
	if(NULL == m_pGroupColumn)
	{
		InitReportNormalData();
	}
	else
	{
		InitReportGroupData();
	}
}


void KStatisticsDlg::InitReportGroupData()
{
	if(NULL == m_pColumns)
	{
		return;
	}

	CXTPReportRecord*     pParentRecord = NULL;
	CXTPReportRecords*    pRecords= NULL;
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	// 일반 데이터
	m_wndReportCtrl.ResetContent(TRUE);
	for (size_t i= 0; i< m_vecGroupInfo.size(); i++)
	{
		TSingleGroupInfo &oTSingleGroupInfo = m_vecGroupInfo[i];

		pParentRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pParentRecord->AddItem(new CXTPReportRecordItemText(oTSingleGroupInfo.strCaption));
		for (int m= 0; m< (COLUMN_TOTAL_COUNT-1); m++)
		{
			pParentRecord->AddItem( new CXTPReportRecordItemText(_T("")));
		}
		pRecords      = pParentRecord->GetChilds();

		std::vector<TSingleNormalInfo> &vecNormalData = oTSingleGroupInfo.vecNormalData;
		for(size_t k= 0; k< vecNormalData.size(); k++)
		{
			TSingleNormalInfo &oTSingleNormalInfo = vecNormalData[k];

			pRecord = pRecords->Add(new CXTPReportRecord());
			
			SetRecordGrid(pRecord, oTSingleNormalInfo.strCaption, oTSingleNormalInfo.oDevideDataInterval );
		}
	}    

	// 수식 데이터
	for(size_t i= 0; i< m_vecGroupExpressionInfo.size(); i++)
	{
		TSingleGroupExpressionInfo &oTSingleGroupExpressionInfo = m_vecGroupExpressionInfo[i];

		std::vector<TSingleExpressionInfo> &vecExpressionData = oTSingleGroupExpressionInfo.vecExpressionData;

		if(vecExpressionData.size() > 0)
		{
			pParentRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pParentRecord->AddItem(new CXTPReportRecordItemText(oTSingleGroupExpressionInfo.strCaption));
			for (int m= 0; m< (COLUMN_TOTAL_COUNT-1); m++)
			{
				pParentRecord->AddItem( new CXTPReportRecordItemText(_T("")));
			}
			pRecords      = pParentRecord->GetChilds();

			for(size_t k= 0; k< vecExpressionData.size(); k++)
			{
				TSingleExpressionInfo &oTSingleExpressionInfo = vecExpressionData[k];

				pRecord = pRecords->Add(new CXTPReportRecord());

				SetRecordGrid(pRecord, oTSingleExpressionInfo.strCaption, oTSingleExpressionInfo.oDevideDataInterval );
			}
		}
		else
		{
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTSingleGroupExpressionInfo.strCaption));
			pItem->SetAlignment(DT_LEFT);
		}
	}

	CString strDataCount;
	strDataCount.Format(_T("Recod(Count): %d, Data(Count): %d"), 0, 0);        
	SetDlgItemText(IDC_STATIC_COUNT, strDataCount);
}


void KStatisticsDlg::InitReportNormalData()
{
	if(NULL == m_pColumns)
	{
		return;
	}

    CXTPReportRecord*     pRecord = NULL;
    CXTPReportRecordItem* pItem   = NULL;

	// 일반 데이터
	m_wndReportCtrl.ResetContent(TRUE);
    for (size_t i=0; i<m_vecNormalInfo.size(); i++)
    {
		TSingleNormalInfo &oTSingleNormalInfo = m_vecNormalInfo[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		SetRecordGrid(pRecord, oTSingleNormalInfo.strCaption, oTSingleNormalInfo.oDevideDataInterval );
    }    

	// 수식 데이터
	for(size_t i= 0; i< m_vecExpressionInfo.size(); i++)
	{
		TSingleExpressionInfo &oTSingleExpressionInfo = m_vecExpressionInfo[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		if(oTSingleExpressionInfo.oDevideDataInterval.GetDataCount() > 0)
		{
			SetRecordGrid(pRecord, oTSingleExpressionInfo.strCaption, oTSingleExpressionInfo.oDevideDataInterval );
		}
		else
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTSingleExpressionInfo.strCaption));
			pItem->SetAlignment(DT_LEFT);
		}
	}

    CString strDataCount;
    strDataCount.Format(_T("Recod(Count): %d, Data(Count): %d"), 0, 0);        
    SetDlgItemText(IDC_STATIC_COUNT, strDataCount);
}


void KStatisticsDlg::SetRecordGrid(CXTPReportRecord* a_pRecord, LPCTSTR a_lpcszName, KDevideDataInterval &a_oDevideDataInterval)
{
	CXTPReportRecordItem* pItem   = NULL;

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText   ( a_lpcszName ));
	pItem->SetAlignment(DT_LEFT);

	pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_oDevideDataInterval.GetSum(),      _T("%.3f")) );
	pItem->SetAlignment(DT_RIGHT);

	pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_oDevideDataInterval.GetAverage(),  _T("%.3f")) );
	pItem->SetAlignment(DT_RIGHT);

	pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_oDevideDataInterval.GetMinValue(), _T("%.3f")) );
	pItem->SetAlignment(DT_RIGHT);

	pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_oDevideDataInterval.GetMaxValue(), _T("%.3f")) );
	pItem->SetAlignment(DT_RIGHT);

	pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_oDevideDataInterval.GetVariance(), _T("%.3f")) );
	pItem->SetAlignment(DT_RIGHT);

	pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_oDevideDataInterval.GetStdDev(),   _T("%.3f")) );
	pItem->SetAlignment(DT_RIGHT);

	pItem = a_pRecord->AddItem( new CXTPReportRecordItemNumber( a_oDevideDataInterval.GetStdErr(),   _T("%.3f")) );
	pItem->SetAlignment(DT_RIGHT);
}


void KStatisticsDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KResizeDialogEx::OnOK();
}


void KStatisticsDlg::OnBnClickedButtonExport()
{
	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle  = _T("Export Statistics Result");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strExportFile = fileDlg.GetPathName();
		strExportFile.Replace(_T(".txt"), _T(""));
		m_strFile.Format(_T("%s.txt"), strExportFile);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ExportThreadCaller, this, false, nLang);

		if (!m_strErrMsg.IsEmpty())
		{
			TxLogDebugException();
			AfxMessageBox(m_strErrMsg);
			return;
		}

		AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("데이터 불러오기에 실패하였습니다."));
	}
}

unsigned __stdcall KStatisticsDlg::ExportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	KStatisticsDlg* pDlg = (KStatisticsDlg*)pParameter->GetParameter();
	pDlg->ExportData();

	return 0;
}

void KStatisticsDlg::ExportData( void )
{
	m_strErrMsg = _T("");

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(m_strFile, nOpenMode);
	CString      strLine;

	CXTPReportRecord*           pRecord = nullptr;
	CXTPReportRecordItem*       pItem   = nullptr;
	CXTPReportRecordItemText*   pText   = nullptr;
	CXTPReportRecordItemNumber* pNumber = nullptr;
	CXTPReportColumn*           pColumn = nullptr;

	CXTPReportRecords*          pChilds  = nullptr;
	CXTPReportRecord*           pCRecord = nullptr;

	try
	{
		//Header
		int nColumn = m_wndReportCtrl.GetColumns()->GetCount();

		for (int i = 0; i < nColumn; ++i)
		{
			pColumn = m_wndReportCtrl.GetColumns()->GetAt(i);
			pColumn->GetCaption();

			if (i == 0)
			{
				strLine = pColumn->GetCaption();
			}
			else
			{
				strLine.AppendFormat(_T("\t%s"), pColumn->GetCaption());
			}
		}

		if (strLine.IsEmpty())
			ThrowException(_T("ReportControl 설정이 잘못되었습니다."));

		outFile.WriteString(strLine);
		outFile.WriteString(_T("\r\n"));

		//Data
		int nRecord = m_wndReportCtrl.GetRecords()->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pRecord = m_wndReportCtrl.GetRecords()->GetAt(i);

			WriteRecord(pRecord, outFile);
			
			if (!pRecord->HasChildren())
				continue;			

			pChilds = pRecord->GetChilds();
			
			int nChildRecord = pChilds->GetCount();

			for (int j = 0; j < nChildRecord; ++j)
			{
				pCRecord = pChilds->GetAt(j);
				WriteRecord(pCRecord, outFile);
			}
		}

		outFile.Close();
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		outFile.Close();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KStatisticsDlg::WriteRecord( CXTPReportRecord* a_pRecord, CStdioFileEx& of )
{
	CString strLine;

	CXTPReportRecordItemText*   pText   = nullptr;
	CXTPReportRecordItemNumber* pNumber = nullptr;

	try
	{
		pText = (CXTPReportRecordItemText*)a_pRecord->GetItem(_0_COLUMN_NAME);
		strLine = pText->GetValue();

		if (!a_pRecord->HasChildren())
		{
			pNumber = (CXTPReportRecordItemNumber*)a_pRecord->GetItem(_1_COLUMN_SUM);
			strLine.AppendFormat(_T("\t%.3f"), pNumber->GetValue());

			pNumber = (CXTPReportRecordItemNumber*)a_pRecord->GetItem(_2_COLUMN_AVERAGE);
			strLine.AppendFormat(_T("\t%.3f"), pNumber->GetValue());

			pNumber = (CXTPReportRecordItemNumber*)a_pRecord->GetItem(_3_COLUMN_MIN);
			strLine.AppendFormat(_T("\t%.3f"), pNumber->GetValue());

			pNumber = (CXTPReportRecordItemNumber*)a_pRecord->GetItem(_4_COLUMN_MAX);
			strLine.AppendFormat(_T("\t%.3f"), pNumber->GetValue());

			pNumber = (CXTPReportRecordItemNumber*)a_pRecord->GetItem(_5_COLUMN_VARIANCE);
			strLine.AppendFormat(_T("\t%.3f"), pNumber->GetValue());

			pNumber = (CXTPReportRecordItemNumber*)a_pRecord->GetItem(_6_COLUMN_STDDEV);
			strLine.AppendFormat(_T("\t%.3f"), pNumber->GetValue());

			pNumber = (CXTPReportRecordItemNumber*)a_pRecord->GetItem(_7_COLUMN_STDERR);
			strLine.AppendFormat(_T("\t%.3f"), pNumber->GetValue());
		}
		
		of.WriteString(strLine);
		of.WriteString(_T("\r\n"));
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}
