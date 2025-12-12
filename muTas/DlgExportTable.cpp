#include "stdafx.h"
#include "KmzApp.h"
#include "DlgExportTable.h"
#include "afxdialogex.h"
#include "ImportTableInfo.h"
#include "..\QBicCommon\QBicTxtWriter.h"
#include "Target.h"
#include "ImChampFrameWindow.h"

namespace CreateCongIndexENUM
{
	enum
	{
		_0_COLUMN_CHECK = 0,
		_1_COLUMN_NAME,
		_2_COLUMN_TYPE,
		_3_COLUMN_INFO
	};
}

using namespace CreateCongIndexENUM;

IMPLEMENT_DYNAMIC(KDlgExportTable, KWhiteDlgEx)

KDlgExportTable::KDlgExportTable(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KWhiteDlgEx(KDlgExportTable::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pTable(a_pTable)
	, m_bSetDataType(false)
{
}

KDlgExportTable::~KDlgExportTable()
{
	KProject* pProject = ImChampFrameWindow::GetProject();

	m_editFile.SetDlgInitialDir(pProject->GetLocation());
}

BOOL KDlgExportTable::OnInitDialog()
{
	KWhiteDlgEx::OnInitDialog();
	KWhiteDlgEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ResizeComponent();

	CheckDlgButton(IDC_CHECK1, BST_CHECKED);
	CheckRadioButton(IDC_RADIO8, IDC_RADIO11, IDC_RADIO11);

	QBicReportCtrlSetting::Default(m_ctrlReport, FALSE, FALSE, FALSE);
	InitReportHeader();
    InitComboEncoding();

	TCHAR szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	m_editFile.Initialize( this, BES_XT_CHOOSEFILE );
	m_editFile.SetDlgOpenFile( FALSE );
	m_editFile.SetReadOnly( TRUE );
	m_editFile.SetDlgTitle( _T("Select file") );
	m_editFile.SetDlgFilter( szFilter );
	m_editFile.SetDlgStyle(OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);

	KProject* pProject = ImChampFrameWindow::GetProject();
	m_editFile.SetDlgInitialDir(pProject->GetLocation());

	//node-based, link-based Select;
	CString strTableName = m_pTable->Name();
	if (strTableName.CompareNoCase(TABLE_TRANSITLINKS) == 0) {
		m_bSetDataType = true;

		GetDlgItem(IDC_STATIC_DATATYPE)->ShowWindow(TRUE);
		GetDlgItem(IDC_COMBO_DATATYPE)->ShowWindow(TRUE);

		InitComboBaseDataType();
	}

	UpdateExportColumnInfo();

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDlgExportTable::DoDataExchange(CDataExchange* pDX)
{
	KWhiteDlgEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,         m_ctrlReport);
	DDX_Control(pDX, IDC_FILENAME,       m_editFile );
	DDX_Control(pDX, IDC_COMBO_DATATYPE, m_cboDataType);
    DDX_Control(pDX, IDC_COMBO4,         m_cboEncoding);
}

BEGIN_MESSAGE_MAP(KDlgExportTable, KWhiteDlgEx)
	ON_BN_CLICKED(IDOK,                   &KDlgExportTable::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK3,             &KDlgExportTable::OnBnClickedCheckSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KDlgExportTable::OnReportCheckItem)
	ON_CBN_SELCHANGE(IDC_COMBO_DATATYPE,  &KDlgExportTable::OnSelchangeComboBaseDataType)
END_MESSAGE_MAP()


void KDlgExportTable::ResizeComponent()
{
	try 
	{
		SetResize(IDC_FILENAME,		SZ_TOP_LEFT,	SZ_TOP_RIGHT);
		SetResize(IDC_STATIC5,      SZ_TOP_LEFT,	SZ_TOP_RIGHT);
		SetResize(IDC_STATIC2,		SZ_TOP_LEFT,	SZ_TOP_RIGHT);

		SetResize(IDC_REPORT,	    SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC6,		SZ_BOTTOM_LEFT,	SZ_BOTTOM_RIGHT);

        SetResize(IDC_STATIC_ENC,   SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
        SetResize(IDC_COMBO4,       SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

		SetResize(IDOK,				SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,			SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgExportTable::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK, _T("Select"), 40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("선택"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_1_COLUMN_NAME, _T("Column Name"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("컬럼 명칭"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_2_COLUMN_TYPE, _T("Data Type"),  70, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("데이터 유형"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_3_COLUMN_INFO, _T("Description"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("데이터 내용"));
		}

		m_ctrlReport.Populate();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgExportTable::InitComboEncoding()
{
    try
    {
        m_cboEncoding.ResetContent();
        int nCur = m_cboEncoding.AddString(_T("ANSI"));
        m_cboEncoding.SetItemData(nCur, (DWORD_PTR)KEMCSVEncodingANSI);

        nCur = m_cboEncoding.AddString(_T("UTF-8"));
        m_cboEncoding.SetItemData(nCur, (DWORD_PTR)KEMCSVEncodingUTF8);

        m_cboEncoding.SetCurSel(0);
    }
    catch (int& ex) {
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    }
    catch (...) {
        TxLogDebugException();
    }
}


void KDlgExportTable::InitComboBaseDataType( void )
{
	m_cboDataType.ResetContent();

	int nIndex;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		nIndex = m_cboDataType.InsertString(-1, _T("노드 기반"));
	}
	else
	{
		nIndex = m_cboDataType.InsertString(-1, _T("NodeBase"));
	}
	m_cboDataType.SetItemData(nIndex, KEMBaseNode);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		nIndex = m_cboDataType.AddString(_T("링크 기반"));
	}
	else
	{
		nIndex = m_cboDataType.AddString(_T("LinkBase"));
	}
	m_cboDataType.SetItemData(nIndex, KEMBaseLink);

	m_cboDataType.SetCurSel(0);
}

void KDlgExportTable::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;

		CXTPReportRecords*	  pRecords = nullptr;
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		pRecords         = m_ctrlReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(0); 
			pItem->SetChecked(bSelectAll);
		}

		m_ctrlReport.RedrawControl();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgExportTable::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	try
	{
		BOOL bChecked = pItemNotify->pItem->IsChecked();
		if (!bChecked) {
			CheckDlgButton(IDC_CHECK3, BST_UNCHECKED);
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgExportTable::UpdateExportColumnInfo( void )
{
	TxLogDebugStartMsg();

	m_ctrlReport.ResetContent();

	BOOL bSelectAll(TRUE);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	const KIOColumns* pColumns     = m_pTable->Columns();
	int               nColumnCount = pColumns->ColumnCount();

	for(int i= 0; i< nColumnCount; i++)	{
		KIOColumn* pColumn       = pColumns->GetColumn(i);
		CString    strColumnName = pColumn->Caption();
		CString    strColumnInfo = pColumn->ColumnInfo();
		CString    strDataType   = _T("");

		//Export Based를 Node, Link 따로 변경하는 경우에만 해당 로직을 탐
		int nIndex = m_cboDataType.GetCurSel();
		if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0 && m_bSetDataType) {
			if (KEMBaseNode == (int)m_cboDataType.GetItemData(nIndex)) {
				strColumnName = _T("Node_ID");
				strColumnInfo = _T("Node_ID");
			}
		}

		if (KEMBaseNode == m_cboDataType.GetItemData(nIndex)) 
        {
            //★ 필터링 TABLE_TRANSITLINKS
			/*if (_tcsicmp(m_pTable->Name(), TABLE_TRANSITLINKS) == 0) {
				CString strColumn = pColumn->Name();
				if (strColumn.CompareNoCase(COLUMN_TRANSIT_ID) != 0 && strColumn.CompareNoCase(COLUMN_TRANSIT_LINK_ID) != 0 
					&& strColumn.CompareNoCase(COLUMN_TRANSIT_LINK_SEQ) != 0 && strColumn.CompareNoCase(COLUMN_TRANSIT_LINK_STATION_YN) != 0) {
						continue;
				}
			}*/
		}

		switch(pColumn->DataType())
		{
		case KEMIODataTypeInteger : strDataType = DATATYPESTRING_INTEGER; break;
		case KEMIODataTypeDouble  : strDataType = DATATYPESTRING_DOUBLE; break;
		case KEMIODataTypeString  : strDataType = DATATYPESTRING_STRING; break;
		}

		BOOL bCheck(FALSE);
		if (KEMIOColumnTypeSystem == pColumn->ColumnType()) {
			bCheck = TRUE;
		} else {
			bCheck     = FALSE;
			bSelectAll = FALSE;
		}

		pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());
		// 선택
		pItem   = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->SetItemData((DWORD_PTR)pColumn);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(bCheck);
		pItem->SetAlignment(xtpColumnIconCenter);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strColumnName));
		pItem->SetAlignment(DT_CENTER);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strDataType));
		pItem->SetAlignment(DT_CENTER);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strColumnInfo));
		pItem->SetAlignment(DT_LEFT);
	}

	m_ctrlReport.Populate();

	if (TRUE == bSelectAll) {
		CheckDlgButton(IDC_CHECK3, BST_CHECKED);
	} else {
		CheckDlgButton(IDC_CHECK3, BST_UNCHECKED);
	}

	TxLogDebugEndMsg();
}

void KDlgExportTable::OnSelchangeComboBaseDataType()
{
	UpdateExportColumnInfo();
}