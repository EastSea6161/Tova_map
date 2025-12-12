// WOTrimpMatrix.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOTrimpMatrix.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MultiModalInfo.h"

#include "CustomReportRecordItem.h"
#include "Mode.h"
#include "DBaseAssignment.h"

#define MAMODE_NAME			0
#define MAMODE_CLOLUMN		1	
#define MAMODE_OCCUPANCY    2 
#define MAMODE_PCU			3
#define MAMODE_NETWORKTYPE  4
#define MAMODE_PRELOAD		5

// KWOTrimpMatrix 대화 상자입니다.

IMPLEMENT_DYNAMIC(KWOTrimpMatrix, KResizeDialogEx)
	
KWOTrimpMatrix::KWOTrimpMatrix(KTarget* a_pTarget, std::list<KMultiModalInfo>& a_lstMaInfo,int& a_nUsingTime,KIOColumn*& a_pLinkColumn,CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KWOTrimpMatrix::IDD, pParent),
	m_pTarget(a_pTarget),
	m_lstMAInfo(a_lstMaInfo),
	m_nUsingLinkTravelTime(a_nUsingTime),
	m_pLinkTimeColumn(a_pLinkColumn)
{
	
}

KWOTrimpMatrix::~KWOTrimpMatrix()
{
}

void KWOTrimpMatrix::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndModeReport);
	DDX_Control(pDX, IDC_COMBO1, m_cboLinkTime);
}


BEGIN_MESSAGE_MAP(KWOTrimpMatrix, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KWOTrimpMatrix::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KWOTrimpMatrix::OnBnClickedCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KWOTrimpMatrix::OnRadioClicked)
END_MESSAGE_MAP()


// KWOTrimpMatrix 메시지 처리기입니다.


BOOL KWOTrimpMatrix::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		if (m_nUsingLinkTravelTime == 1)
		{
			CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO1);
			pRadio->SetCheck(BST_CHECKED);
			m_cboLinkTime.EnableWindow(FALSE);
		}
		else
		{
			CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO2);
			pRadio->SetCheck(BST_CHECKED);
			m_cboLinkTime.EnableWindow(TRUE);
		}

		ResizeComponent();
		InitModeReportControl();
		UpdateModeReportRecord();
		SetLinkTaravelTimeField();

		KReportCtrlSetting::Default(m_wndModeReport, TRUE);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KWOTrimpMatrix::InitModeReportControl( void )
{
	
	// Add Columns
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_NAME, _T("Mode"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_CLOLUMN, _T("Matrix"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_OCCUPANCY, _T("Occupancy"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("평균재차인원"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_PCU, _T("PCE"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("승용차 환산계수"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_NETWORKTYPE, _T("Network"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시스템 타입명"));
    }

	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pEditOptions->AddConstraint(_T("도로"), 1);
		pEditOptions->AddConstraint(_T("버스"), 2);
		pEditOptions->AddConstraint(_T("도시철도"), 3);
		pEditOptions->AddConstraint(_T("버스+도시철도"), 4);
	}
	else {
		pEditOptions->AddConstraint(_T("Highway"), 1);
		pEditOptions->AddConstraint(_T("Line"), 2);
		pEditOptions->AddConstraint(_T("Rail"), 3);
		pEditOptions->AddConstraint(_T("Line,Rail"), 4);
	}
 
	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_PRELOAD, _T("PreLoad"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("사전배정"));
    }
}

void KWOTrimpMatrix::UpdateModeReportRecord( void )
{
	std::list<KMultiModalInfo>::iterator iter = m_lstMAInfo.begin();
	std::list<KMultiModalInfo>::iterator iterend = m_lstMAInfo.end();

	for (iter ; iter != iterend ; iter++)
	{
		KMultiModalInfo &oMulitModalInfo = *iter;


		CXTPReportRecord* pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		if (oMulitModalInfo.TransitHighwayType() == 1)
		{
			//pItem->SetCaption(_T("Bus"));
			KMode* pMode = oMulitModalInfo.Mode();
			pItem->SetCaption(pMode->ModeName());
		}
		else if (oMulitModalInfo.TransitHighwayType() == 2)
		{
			pItem->SetCaption(_T("Subway"));
			KMode* pMode = oMulitModalInfo.Mode();
			pItem->SetCaption(pMode->ModeName());
		}
		else if (oMulitModalInfo.TransitHighwayType() == 3)
		{
			pItem->SetCaption(_T("Bus+Subway"));
			KMode* pMode = oMulitModalInfo.Mode();
			pItem->SetCaption(pMode->ModeName());
		}
		else if (oMulitModalInfo.TransitHighwayType() == 0)
		{
			pItem->SetCaption(_T("Highway"));
			KMode* pMode = oMulitModalInfo.Mode();
			pItem->SetCaption(pMode->ModeName());
		}
		pItem->SetItemData((DWORD_PTR)&oMulitModalInfo);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		KIOColumn* pColumn = oMulitModalInfo.ModeColumn();

		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pColumn->Caption());
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		// Occupancy
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMulitModalInfo.Occupancy(), _T("%f")));

		// PCE
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMulitModalInfo.PCU(), _T("%f")));

		// NetworkType
		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(oMulitModalInfo.NetworkType());
		if (oMulitModalInfo.TransitHighwayType() == 0)
		{
			pItem->SetEditable(FALSE);
		}
		else if (oMulitModalInfo.TransitHighwayType() == 1)
		{
			pItem->SetEditable(FALSE);
		}
		else if (oMulitModalInfo.TransitHighwayType() == 2)
		{
			pItem->SetEditable(FALSE);
		}
		else if (oMulitModalInfo.TransitHighwayType() == 3)
		{
			pItem->SetEditable(FALSE);
		}
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		//PreLoad
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		if(oMulitModalInfo.PreLoad() == 1)
		{
			pItem->SetChecked(TRUE);
		}

		if (oMulitModalInfo.TransitHighwayType() == 1)
		{
			pItem->SetChecked(FALSE);
			pItem->SetEditable(FALSE);

		}
		else if (oMulitModalInfo.TransitHighwayType() == 2)
		{
			pItem->SetChecked(FALSE);
			pItem->SetEditable(FALSE);
		}
		else if (oMulitModalInfo.TransitHighwayType() == 3)
		{
			pItem->SetChecked(FALSE);
			pItem->SetEditable(FALSE);
		}
	}

	m_wndModeReport.Populate();
	m_wndModeReport.ExpandAll(TRUE);
}


void KWOTrimpMatrix::OnBnClickedOk()
{
	ApplyInputData();

	int nCursel = m_cboLinkTime.GetCurSel();

	int  nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO2)
	{
		m_pLinkTimeColumn = (KIOColumn*)m_cboLinkTime.GetItemData(nCursel);

		if (m_pLinkTimeColumn == nullptr)
		{
			AfxMessageBox(_T("링크통행시간 관련 컬럼을 선택해 주세요."));
			return;
		}

	}
	
	KResizeDialogEx::OnOK();
}


void KWOTrimpMatrix::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

bool KWOTrimpMatrix::ApplyInputData( void )
{
	CXTPReportRecords* pRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(MAMODE_NAME);
		KMultiModalInfo* pInfo = (KMultiModalInfo*)(pItem->GetItemData());

		CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(MAMODE_OCCUPANCY));
		pInfo->Occupancy(pNumber->GetValue());

		pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(MAMODE_PCU));
		pInfo->PCU(pNumber->GetValue());

		pItem = pRecord->GetItem(MAMODE_NETWORKTYPE);
		pInfo->NetworkType(pItem->GetItemData());

		pItem = pRecord->GetItem(MAMODE_PRELOAD);
		pInfo->PreLoad((pItem->IsChecked() == TRUE ? true : false));
	}

	return true;
}

void KWOTrimpMatrix::SetLinkTaravelTimeField( void )
{
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*		 pTable		   = pIOTables->FindTable(TABLE_LINK);
	const KIOColumns* pColumns = pTable->Columns();
	
	int nPreSelIndex = 0;
	int nCount = pColumns->ColumnCount();

	if (KmzSystem::GetLanguage() == KEMKorea) {
        m_cboLinkTime.AddString(_T("--------선택--------"));
    }
	else {
		m_cboLinkTime.AddString(_T("-------Select-------"));
	}

	for (int i = 0; i < nCount; i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		CString strColumnName = pColumn->Name();

		if (pColumn->ColumnType()== KEMIOColumnTypeUserDefine)
		{
			int nIndex = m_cboLinkTime.AddString(pColumn->Caption());
			m_cboLinkTime.SetItemData(nIndex, (DWORD_PTR)pColumn);

			if (pColumn == m_pLinkTimeColumn)
			{
				nPreSelIndex = nIndex;
			}
		}
	}

	m_cboLinkTime.SetCurSel(nPreSelIndex);
}

void KWOTrimpMatrix::OnRadioClicked( UINT nID )
{
	int  nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO1)
	{
		m_nUsingLinkTravelTime = 1;
		m_cboLinkTime.EnableWindow(FALSE);
	}
	else
	{
		m_nUsingLinkTravelTime = 0;
		m_cboLinkTime.EnableWindow(TRUE);
	}
}

void KWOTrimpMatrix::ResizeComponent()
{
	SetResize(IDC_STATIC1,		SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR2,	SZ_TOP_LEFT,  SZ_TOP_RIGHT);	
	SetResize(IDC_SEPRATOR1,	SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_REPORT1,		SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDOK,				SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,			SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}
