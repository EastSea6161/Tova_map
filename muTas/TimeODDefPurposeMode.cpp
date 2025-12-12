// TimeODDefPurposeMode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TimeODDefPurposeMode.h"

#include "TimeODSurveyImport1Dlg.h"
#include "TimeODSurveyImport2Dlg.h"
#include "TimeODSelectSurveyCodeDlg.h"

/* define user message */
#define UM_SURVEYPURPOSEEDIT	    WM_USER + 1001
#define UM_SURVEYMODEEDIT			WM_USER + 1002

enum KEMTest
{
	_0_COLUMN_NAME = 0,
	_1_COLUMN_SURVEYCODE,
};


// KTimeODDefPurposeMode 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTimeODDefPurposeModeDlg, CXTPResizeDialog)

KTimeODDefPurposeModeDlg::KTimeODDefPurposeModeDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KTimeODDefPurposeModeDlg::IDD, pParent)
{

}

KTimeODDefPurposeModeDlg::~KTimeODDefPurposeModeDlg()
{
}

void KTimeODDefPurposeModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,   m_PurposeReportCtrl);
	DDX_Control(pDX, IDC_REPORT2,  m_ModeReportCtrl);
}


BEGIN_MESSAGE_MAP(KTimeODDefPurposeModeDlg, CXTPResizeDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &KTimeODDefPurposeModeDlg::OnBnClickedButtonImport)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT,  OnItemPurposeButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT2, OnItemModeButtonClick)
	ON_MESSAGE(UM_SURVEYPURPOSEEDIT, OnSurveyPurposeEdit)
	ON_MESSAGE(UM_SURVEYMODEEDIT,    OnSurveyModeEdit)

	ON_BN_CLICKED(IDOK, &KTimeODDefPurposeModeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KTimeODDefPurposeMode 메시지 처리기입니다.

BOOL KTimeODDefPurposeModeDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();    

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	QBicReportCtrlSetting::Default(m_PurposeReportCtrl, FALSE, FALSE, FALSE);
	QBicReportCtrlSetting::AllowColumnsort(m_PurposeReportCtrl);
	QBicReportCtrlSetting::Default(m_ModeReportCtrl, FALSE, FALSE, FALSE);
	QBicReportCtrlSetting::AllowColumnsort(m_ModeReportCtrl);

	InitReportHeader();
	InitReportData();
	m_PurposeReportCtrl.Populate();
	m_ModeReportCtrl.Populate();

	SetResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTimeODDefPurposeModeDlg::InitReportHeader()
{
	InitPurposeReportHeader();
	InitModeReportHeader();
}


void KTimeODDefPurposeModeDlg::InitPurposeReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_PurposeReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_NAME,       _T("목적"),  40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	pColumn->SetSortable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행목적"));
    }

	pColumn = m_PurposeReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SURVEYCODE, _T("Survey Code"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_LEFT);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("조사자료 매칭 코드"));
    }
}


void KTimeODDefPurposeModeDlg::InitModeReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ModeReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_NAME,          _T("수단"),  40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	pColumn->SetSortable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }

	pColumn = m_ModeReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SURVEYCODE,    _T("Survey Code"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_LEFT);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("조사자료 매칭 코드"));
    }
}


void KTimeODDefPurposeModeDlg::InitReportData()
{
	InitPurposeReportData();
	InitModeReportData();
}


void KTimeODDefPurposeModeDlg::InitPurposeReportData()
{
	CXTPReportRecord*            pRecord = NULL;
	CXTPReportRecordItem*        pItem   = NULL;
	CXTPReportRecordItemControl* pButton = NULL;

	size_t nPurposeCount = m_vecSurveyPurpose.size();

	for(size_t i= 0; i< nPurposeCount; i++)
	{
		TSurveyPurpose    &oTSurveyPurpose      = m_vecSurveyPurpose[i];
		CString &strName = oTSurveyPurpose.oTPurpose.strPurposeName;
		KTDSurveyCodeSet  &setPurposeSurveyCode = oTSurveyPurpose.setPurposeSurveyCode;
		CString strCodes = _T("");		

		CodesToStiring(setPurposeSurveyCode, strCodes);

		pRecord = m_PurposeReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strCodes));
		pItem->SetItemData((DWORD_PTR) &setPurposeSurveyCode);
		pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
		if(NULL != pButton)
		{
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("설정"));
                pButton->SetSize(CSize(25, 0));
            }
            else {
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Edit"));
                pButton->SetSize(CSize(22, 0));
            }			
		}
	}
}


void KTimeODDefPurposeModeDlg::InitModeReportData()
{
	CXTPReportRecord*            pRecord = NULL;
	CXTPReportRecordItem*        pItem   = NULL;
	CXTPReportRecordItemControl* pButton = NULL;

	size_t nModeCount = m_vecSurveyMode.size();

	for(size_t i= 0; i< nModeCount; i++)
	{
		TSurveyMode       &oTSurveyMode      = m_vecSurveyMode[i];
		CString &strName = oTSurveyMode.oTMode.strModeName;
		KTDSurveyCodeSet  &setModeSurveyCode = oTSurveyMode.setModeSurveyCode;
		CString strCodes = _T("");		

		CodesToStiring(setModeSurveyCode, strCodes);

		pRecord = m_ModeReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strCodes));
		pItem->SetItemData((DWORD_PTR) &setModeSurveyCode);
		pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
		if(NULL != pButton)
		{
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("설정"));
                pButton->SetSize(CSize(25, 0));
            }
            else {
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Edit"));
                pButton->SetSize(CSize(22, 0));
            }			
		}
	}
}


void KTimeODDefPurposeModeDlg::CodesToStiring(KTDSurveyCodeSet &a_setSurveyCode, CString &a_strCodes)
{
	CString strDevide = _T(", ");

	KTDSurveyCodeSet::iterator iter = a_setSurveyCode.begin();

	while(iter != a_setSurveyCode.end())
	{
		CString strCode = _T("");
		strCode.Format(_T("%d"), *iter);

		a_strCodes += strCode;
		a_strCodes += strDevide;
		++iter;
	}

	a_strCodes = a_strCodes.Left( a_strCodes.GetLength() - strDevide.GetLength() ); // 마지막 (,)제거 하기
}


void KTimeODDefPurposeModeDlg::SetResizeComponent()
{
	SetResize(IDC_BUTTON1,            SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR6,          SZ_TOP_LEFT,      SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_PURPOSE,     SZ_TOP_LEFT,      SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,             SZ_TOP_LEFT,      SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_MODE,        SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_REPORT2,            SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);

	SetResize(IDC_SEPRATOR,           SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                   SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,               SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KTimeODDefPurposeModeDlg::OnBnClickedButtonImport()
{
	TxLogDebugStartMsg();
	CString strFileName(_T(""));
	BOOL	bContainHeader(false);
	int		nSeparatorType(0);
	int		nEncodingType(0);
		
	while (true)
	{
		//#1
		KTimeODSurveyImport1Dlg dlg(bContainHeader, nSeparatorType, nEncodingType, strFileName);
		
		if (IDCANCEL == dlg.DoModal())
			return;
		bContainHeader	= dlg.ContainHeader();
		nSeparatorType	= dlg.SeparatorType();
		nEncodingType	= dlg.EncodingType();
		strFileName		= dlg.FileName();

		TCHAR tcSeparator;
		switch (nSeparatorType)
		{
		case 0 : tcSeparator = _T(',');
			break;
		case 1 : tcSeparator = _T(';');
			break;
		case 2 : tcSeparator = _T('|');
			break;
		case 3 : tcSeparator = _T('	'); 
			break;
		default: ASSERT(FALSE);
			break;
		}

		KEMCSVEncoding emCSVEncoding;
		switch( nEncodingType )
		{
		case 0 : emCSVEncoding = KEMCSVEncodingANSI; 
			break;
		case 1 : emCSVEncoding = KEMCSVEncodingUTF8; 
			break;
		case 2 : emCSVEncoding = KEMCSVEncodingUTF16LE; 
			break;
		default : ASSERT( FALSE ); 
			break;
		}

		//#2
		KTimeODSurveyImport2Dlg dlg2(bContainHeader, emCSVEncoding, tcSeparator, strFileName);
		
		if(IDCANCEL == dlg2.DoModal())
			return;
		
		if (dlg2.GoBack())
			continue;

		//update
		m_setTotalPurpseSurveyCode = dlg2.PurposeSet();
		m_setTotalModeSurveyCode   = dlg2.ModeSet();
		ClearPrevSurveyCode();

		break;		//Import2Dlg == IDOK 일시 수행
	}


	
	TxLogDebugEndMsg();
}


void KTimeODDefPurposeModeDlg::ClearPrevSurveyCode()
{
// 	CXTPReportRecords*	  pRecords        = NULL;
// 	CXTPReportRecord*     pRecord         = NULL;
// 	CXTPReportRecordItem* pItem           = NULL;
// 	pRecords = m_PurposeReportCtrl.GetRecords();
// 	for(int i= 0; i< pRecords->GetCount(); i++)
// 	{
// 		pRecord         = pRecords->GetAt(i);
// 		pItem           = pRecord->GetItem(_1_COLUMN_SURVEYCODE);
// 		
// 		KTDSurveyCodeSet* pSetSelectedCode = (KTDSurveyCodeSet*)pItem->GetItemData();
// 		pSetSelectedCode->clear();
// 
// 		pItem->SetCaption(_T(""));
// 	}
// 	/*m_PurposeReportCtrl.Populate();*/
// 	m_PurposeReportCtrl.RedrawControl();
// 
// 	pRecords = m_ModeReportCtrl.GetRecords();
// 	for(int i= 0; i< pRecords->GetCount(); i++)
// 	{
// 		pRecord      = pRecords->GetAt(i);
// 		pItem        = pRecord->GetItem(_1_COLUMN_SURVEYCODE);
// 
// 		KTDSurveyCodeSet* pSetSelectedCode = (KTDSurveyCodeSet*)pItem->GetItemData();
// 		pSetSelectedCode->clear();
// 
// 		pItem->SetCaption(_T(""));
// 	}
// 	/*m_ModeReportCtrl.Populate();*/
// 	m_ModeReportCtrl.RedrawControl();

	for( size_t i= 0; i< m_vecSurveyPurpose.size(); i++ )
	{
		TSurveyPurpose    &oTSurveyPurpose      = m_vecSurveyPurpose[i];
		KTDSurveyCodeSet  &setPurposeSurveyCode = oTSurveyPurpose.setPurposeSurveyCode;
		setPurposeSurveyCode.clear();
	}
	*m_pVecSurveyPurpose = m_vecSurveyPurpose;
	m_PurposeReportCtrl.ResetContent(TRUE);
	InitPurposeReportData();
	m_PurposeReportCtrl.Populate();

	for( size_t i= 0; i< m_vecSurveyMode.size(); i++ )
	{
		TSurveyMode       &oTSurveyMode         = m_vecSurveyMode[i];
		KTDSurveyCodeSet  &setModeSurveyCode    = oTSurveyMode.setModeSurveyCode;
		setModeSurveyCode.clear();
	}
	*m_pVecSurveyMode = m_vecSurveyMode;
	m_ModeReportCtrl.ResetContent(TRUE);
	InitModeReportData();
	m_ModeReportCtrl.Populate();
}


void KTimeODDefPurposeModeDlg::OnBnClickedOk()
{
	CXTPReportRecords*	  pPurposeRecords = NULL;
	CXTPReportRecords*	  pModeRecords    = NULL;
	CXTPReportRecord*     pRecord         = NULL;
	CXTPReportRecordItem* pItem           = NULL;

	pPurposeRecords = m_PurposeReportCtrl.GetRecords();
	pModeRecords    = m_ModeReportCtrl.GetRecords();

	if( pPurposeRecords->GetCount()< 1 )
	{
		AfxMessageBox(_T("목적 통행 데이터가 존재하지 않습니다."));
		return;
	}

	if( pModeRecords->GetCount()< 1 )
	{
		AfxMessageBox(_T("수단 통행 데이터가 존재하지 않습니다."));
		return;
	}

	if( m_setTotalModeSurveyCode.size()< 1 || m_setTotalPurpseSurveyCode.size()< 1 )
	{
		AfxMessageBox(_T("조사자료 매칭 코드가 존재하지 않습니다.\r\n조사자료 불러오기를 먼저 실행해 주세요."));
		return;
	}

	for(int i= 0; i< pPurposeRecords->GetCount(); i++)
	{
		CString strName = _T("");

		pRecord      = pPurposeRecords->GetAt(i);
		pItem        = pRecord->GetItem(_0_COLUMN_NAME);
		strName      = pItem->GetCaption(NULL);

		pItem        = pRecord->GetItem(_1_COLUMN_SURVEYCODE);
		KTDSurveyCodeSet* pSetSelectedCode = (KTDSurveyCodeSet*)pItem->GetItemData();

		if( pSetSelectedCode->size() < 1 )
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("%s에 조사자료 코드를 설정해 주세요."), strName);
			AfxMessageBox(strMsg);

			PostMessage(UM_SURVEYPURPOSEEDIT, (WPARAM)(pRecord));

			return;
		}
	}

	for(int i= 0; i< pModeRecords->GetCount(); i++)
	{
		CString strName = _T("");

		pRecord      = pModeRecords->GetAt(i);
		pItem        = pRecord->GetItem(_0_COLUMN_NAME);
		strName      = pItem->GetCaption(NULL);

		pItem        = pRecord->GetItem(_1_COLUMN_SURVEYCODE);
		KTDSurveyCodeSet* pSetSelectedCode = (KTDSurveyCodeSet*)pItem->GetItemData();

		if( pSetSelectedCode->size() < 1 )
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("%s에 조사자료 코드를 설정해 주세요."), strName);
			AfxMessageBox(strMsg);

			PostMessage(UM_SURVEYMODEEDIT, (WPARAM)(pRecord));

			return;
		}
	}

	CXTPResizeDialog::OnOK();
}


void KTimeODDefPurposeModeDlg::OnItemPurposeButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRow*    pRow    = pItemNotify->pRow;
	CXTPReportRecord* pRecord = pRow->GetRecord();
	//pItemNotify->pItem
	//CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_SURVEYPURPOSEEDIT, (WPARAM)(pRecord));
}


void KTimeODDefPurposeModeDlg::OnItemModeButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRow*    pRow    = pItemNotify->pRow;
	CXTPReportRecord* pRecord = pRow->GetRecord();
	//pItemNotify->pItem
	//CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	PostMessage(UM_SURVEYMODEEDIT, (WPARAM)(pRecord));
}


LRESULT KTimeODDefPurposeModeDlg::OnSurveyPurposeEdit(WPARAM wParam, LPARAM lParam)
{
	if(m_setTotalPurpseSurveyCode.size() < 1)
	{
		AfxMessageBox(_T("조사자료 매칭 코드가 존재하지 않습니다.\r\n조사자료 불러오기를 먼저 실행해 주세요."));
		return 0;
	}

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	KEMSurveyDataType     emType  = KEMSurveyDataTypePurpose;
	CString               strName = _T("");
	
	pRecord = (CXTPReportRecord*)wParam;
	pItem   = pRecord->GetItem(_0_COLUMN_NAME);
	strName = pItem->GetCaption(NULL);

	pItem   = pRecord->GetItem(_1_COLUMN_SURVEYCODE);
	KTDSurveyCodeSet* pSetSelectedCodes = (KTDSurveyCodeSet*)pItem->GetItemData();

	KTimeODSelectSurveyCodeDlg dlg;
	dlg.SetName(strName);
	dlg.SetSurveyDataType(emType); 
	dlg.SetTotalCodes    (m_setTotalPurpseSurveyCode);
	dlg.SelectedCodes    (*pSetSelectedCodes);

	if( IDOK == dlg.DoModal() )
	{
		*pSetSelectedCodes = dlg.SelectedCodes();

		CString strCodes = _T("");		
		CodesToStiring(*pSetSelectedCodes, strCodes);

		pItem->SetCaption(strCodes);
	}

	/*m_PurposeReportCtrl.Populate();*/
	m_PurposeReportCtrl.RedrawControl();
	return 0;
}


LRESULT KTimeODDefPurposeModeDlg::OnSurveyModeEdit(WPARAM wParam, LPARAM lParam)
{
	if(m_setTotalModeSurveyCode.size() < 1)
	{
		AfxMessageBox(_T("조사자료 매칭 코드가 존재하지 않습니다.\r\n조사자료 불러오기를 먼저 실행해 주세요."));
		return 0;
	}

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	KEMSurveyDataType     emType  = KEMSurveyDataTypeMode;
	CString               strName = _T("");

	pRecord = (CXTPReportRecord*)wParam;
	pItem   = pRecord->GetItem(_0_COLUMN_NAME);
	strName = pItem->GetCaption(NULL);

	pItem   = pRecord->GetItem(_1_COLUMN_SURVEYCODE);
	KTDSurveyCodeSet* pSetSelectedCodes = (KTDSurveyCodeSet*)pItem->GetItemData();

	KTimeODSelectSurveyCodeDlg dlg;
	dlg.SetName(strName);
	dlg.SetSurveyDataType(emType); 
	dlg.SetTotalCodes    (m_setTotalModeSurveyCode);
	dlg.SelectedCodes    (*pSetSelectedCodes);

	if( IDOK == dlg.DoModal() )
	{
		*pSetSelectedCodes = dlg.SelectedCodes();

		CString strCodes = _T("");		
		CodesToStiring(*pSetSelectedCodes, strCodes);

		pItem->SetCaption(strCodes);
	}

	/*m_ModeReportCtrl.Populate();*/
	m_ModeReportCtrl.RedrawControl();
	return 0;
}

