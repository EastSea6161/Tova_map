// ResultViewModeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ResultViewModeDlg.h"
#include "afxdialogex.h"



#include "Target.h"
#include "Project.h"
#include "Scenario.h"


#include "ViewResultInfo.h"
#include "DBaseResultView.h"
#include "ODResultViewVRecord.h"
//#include "StdioFileEx.h"

// KResultViewModeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KResultViewModeDlg, CXTResizeDialog)

KResultViewModeDlg::KResultViewModeDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KResultViewModeDlg::IDD, pParent)
	, m_pVirtualRecord(NULL)
{
	m_hICon   = AfxGetApp()->LoadIcon(IDR_DLG_ICO);
	m_emModel = MODECHOICE_MODEL;
}

KResultViewModeDlg::~KResultViewModeDlg()
{
}

void KResultViewModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeState);
	DDX_Control(pDX, IDC_CUSTOM1, m_wndReport);
}


BEGIN_MESSAGE_MAP(KResultViewModeDlg, CXTResizeDialog)
	ON_BN_CLICKED(IDOK, &KResultViewModeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TEXT, &KResultViewModeDlg::OnBnClickedButtonSaveText)
END_MESSAGE_MAP()


// KResultViewModeDlg 메시지 처리기입니다.
BOOL KResultViewModeDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::DefaultE(m_wndReport, FALSE, FALSE);

	if (m_emModel == MODECHOICE_MODEL)
	{
		if (KmzSystem::IsPassengerSystem())
		{
			SetWindowText(_T("Compare Mode OD"));//수단 OD 데이터 비교
		}
		else
		{
			SetWindowText(_T("수단 P/C 데이터 비교"));
		}
	}
	else if (m_emModel == CHAIN_OD )
	{
		SetWindowText(_T("수단 O/D 데이터 비교"));
	}
	else if (m_emModel == VEHICLE_OD )
	{
		SetWindowText(_T("Vehicle O/D 데이터 비교"));
	}

	SetResize(IDC_TREE1, SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CUSTOM1, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAVE_TEXT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	SetIcon(m_hICon, TRUE); 

	m_treeState.SetModelChoice(m_emModel);
	m_treeState.UpdateProjectTree();
	m_treeState.SetFont(&XTPAuxData().font);

	TxLogDebugEndMsg();
	return TRUE;
}


void KResultViewModeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWaitCursor wait;

	m_treeState.ClearViewResultInfo();
	m_treeState.GetTreeInfoList();

	m_vecKViewResultInfo = m_treeState.m_vecKResultInfo;

	m_wndReport.GetColumns()->Clear();
	m_wndReport.ResetContent(TRUE);

	InitReportControl();
	UpdateReportControl();
	
	m_wndReport.SetFreezeColumnsCount(2);
	m_wndReport.Populate();

	GetDlgItem(IDC_BUTTON_SAVE_TEXT)->EnableWindow(TRUE);
}

void KResultViewModeDlg::InitReportControl( void )
{
	int nColumnItemIndex	= 0;
	int nSelectedItemCount	= m_vecKViewResultInfo.size();

	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Orgin"), 80);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Dest"), 80);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);

	CString strCaption;

	for (int i = 0; i < nSelectedItemCount; ++i)
	{
		KViewResultInfo oViewResultInfo = m_vecKViewResultInfo[i];

		KIOColumn* pColumn = oViewResultInfo.GetColumnInfo();
		KTarget*   pTarget = oViewResultInfo.GetTargetInfo();

		strCaption = pTarget->Caption() + pColumn->Caption();

		pReportColumn = new CXTPReportColumn(nColumnItemIndex++, strCaption, 110);
		pReportColumn = m_wndReport.AddColumn(pReportColumn);
	}
}



void KResultViewModeDlg::UpdateReportControl( void )
{
	std::map<KODKey, KSDoubleRecord*> mapResultRecord;
	CString strTableName = _T("mode_od");
	if (m_emModel == CHAIN_OD )
	{
		strTableName = _T("chain_od");
	}
	
	if (m_emModel == VEHICLE_OD )
	{
		strTableName = _T("vehicle_mode_od");
	}

	KDBaseResultView::ODResultView(strTableName, m_vecKViewResultInfo, mapResultRecord);

	int nRecordColumnCount = m_vecKViewResultInfo.size() + 3;

	m_pVirtualRecord = new KODResultViewVRecord(mapResultRecord, nRecordColumnCount);
	m_wndReport.SetVirtualMode(m_pVirtualRecord, mapResultRecord.size());
}

BOOL KResultViewModeDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->wParam == VK_RETURN ||
		pMsg->wParam == VK_ESCAPE)
	{
		// don't allow dialog to process return or escape keys.
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);

		return TRUE;
	}

	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void KResultViewModeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hICon);
	}
	else
	{
		CXTResizeDialog::OnPaint();
	}
}

void KResultViewModeDlg::OnBnClickedButtonSaveText()
{		
	size_t nxRecordsCount = m_pVirtualRecord->GetRealRecordsCount();
	if (m_wndReport.GetRecords()->GetCount() < 1)
	{
		AfxMessageBox(_T("검색 결과가 없습니다."));
		return;
	}

	CFileDialog dlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);
	dlg.m_ofn.lpstrTitle = _T("파일을 저장하세요");
	//dlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

	m_strWriteTextFileName = _T("");
	if (dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();
		strFileName.Replace(_T(".txt"), _T(""));
		m_strWriteTextFileName = strFileName + _T(".txt");		
	}
	else
	{
		return;
	}
	
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(WriteTextThreadProc, this, true, nLang);
}

unsigned __stdcall KResultViewModeDlg::WriteTextThreadProc( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;
	KResultViewModeDlg* pDlg = (KResultViewModeDlg*)pParameter->GetParameter();

	CString strFileName = pDlg->GetWriteTextFileName();
	if (strFileName.GetLength() == 0)
		return 0;

	pDlg->WriteTextFile(strFileName, pDlg->GetVirtualRecord()->GetResultRecord());
	return 1;
}

void KResultViewModeDlg::WriteTextFile(CString strFileName, std::map<KODKey, KSDoubleRecord*>& ar_mapResultRecord)
{	
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strFileName, nOpenMode);

	std::map<KODKey, KSDoubleRecord*>::iterator iter = ar_mapResultRecord.begin();
	std::map<KODKey, KSDoubleRecord*>::iterator end  = ar_mapResultRecord.end();

	CString strOut, strData;
	try
	{
		while(iter != end)
		{
			KODKey          oKey    = iter->first;
			KSDoubleRecord* pRecord = iter->second;
			strOut.Format(_T("%I64d,%I64d,"), oKey.OriginID, oKey.DestinationID);

			int nCount = pRecord->GetCount();
			for (int i=0; i<nCount; i++)
			{
				strData.Format(_T("%.2f,"), pRecord->GetAt(i));
				strOut += strData;
			}			

			strOut += _T("\r\n");
			of.WriteString(strOut);
			++iter;

			if (QBicSimpleProgressThread::IsUserStopped())
			{
				break;
			}			
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	of.Close();
}
