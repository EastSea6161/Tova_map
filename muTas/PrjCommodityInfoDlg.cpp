// PrjCommodityInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PrjCommodityInfoDlg.h"
#include "afxdialogex.h"
#include "Purpose.h"

// KPrjCommodityInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPrjCommodityInfoDlg, KResizeDialogEx)

KPrjCommodityInfoDlg::KPrjCommodityInfoDlg(std::vector<KPurpose*> &vecPurposeInfo,CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPrjCommodityInfoDlg::IDD, pParent)
	, m_vecPurposeInfo(vecPurposeInfo)
{

}

KPrjCommodityInfoDlg::~KPrjCommodityInfoDlg()
{
}

void KPrjCommodityInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_reportPurpose);
}


BEGIN_MESSAGE_MAP(KPrjCommodityInfoDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KPrjCommodityInfoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KPrjCommodityInfoDlg 메시지 처리기입니다.


BOOL KPrjCommodityInfoDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_reportPurpose, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_reportPurpose);

	IniteReportHeader();
	IniteReportData();

	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPrjCommodityInfoDlg::IniteReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = NULL;

		pColumn = m_reportPurpose.AddColumn(new CXTPReportColumn(0, _T("ID"), 20)); 
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		pColumn->SetAlignment(DT_CENTER);

		pColumn = m_reportPurpose.AddColumn(new CXTPReportColumn(1, _T("Commodity"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetAlignment(DT_LEFT);

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

void KPrjCommodityInfoDlg::IniteReportData()
{
	m_reportPurpose.ResetContent();

	try
	{
		size_t nCount = m_vecPurposeInfo.size();
		for (size_t i = 0; i < nCount;  i++)
		{
			KPurpose* pPurpose = m_vecPurposeInfo[i];

			CXTPReportRecord*     pRecord = m_reportPurpose.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem   = nullptr;

			CString strPurspoeID(_T(""));
			strPurspoeID.Format(_T("%d"), pPurpose->PurposeID());

			pItem  = pRecord->AddItem(new CXTPReportRecordItemText(strPurspoeID));
			pItem->SetEditable(FALSE);
			
			pItem  = pRecord->AddItem(new CXTPReportRecordItemText(pPurpose->PurposeName()));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
		}

		m_reportPurpose.Populate();
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


void KPrjCommodityInfoDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KPrjCommodityInfoDlg::ResizeComponet()
{
	SetResize(IDC_PRJSTC_PURPOSE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_REPORT1, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC2, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}
