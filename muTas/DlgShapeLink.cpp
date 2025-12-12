// LinkShpaeImportDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgShapeLink.h"
#include "afxdialogex.h"
#include "Target.h"
#include "ImChampFrameWindow.h"
#include "CustomReportRecordItem.h"
#include "KExRecordItem.h"
#include "DefineSystemCodeDlg.h"
#include "BulkDBaseNode.h"
#include "NetworkEditLog.h"
#include "ImportCommon.h"
#include "DBaseImportTable.h"

#include "../QBicCommon/QBicTxtWriter.h"

// KDlgShapeLink 대화 상자입니다.

namespace ShapeLinkENUM
{
	enum KEMGeoType
	{
		KEMGeoNull     = 0,
		KEMGeoPoint    = 1,
		KEMGeoPolyLine = 3,
		KEMGeoPolyGon  = 5
	};
}

using namespace ShapeLinkENUM;

IMPLEMENT_DYNAMIC(KDlgShapeLink, KResizeDialogEx)

KDlgShapeLink::KDlgShapeLink(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDlgShapeLink::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nGeoType(KEMGeoNull)
	, m_dOneWayOffset(5.0)
    , m_spFeatureLayer(nullptr)
{
	m_pTable = m_pTarget->Tables()->FindTable(TABLE_LINK);

	CString strTargetLocation = m_pTarget->GetLocation();
	m_strErrFile.Format(_T("%s\\%s"), strTargetLocation, IMPORT_ERROR_FILE_NAME);
}

KDlgShapeLink::~KDlgShapeLink()
{
}

void KDlgShapeLink::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_FOR,	m_wndReportForward);
	DDX_Control(pDX, IDC_REPORT_REV,	m_wndReportReverse);
	DDX_Control(pDX, IDC_REPORT_DRAG,	m_wndReportDrag);
	DDX_Control(pDX, IDC_REPORT_PRE,    m_wndReportPreview);
	DDX_Control(pDX, IDC_EDIT_BROWSE,	m_editShapeFile);
	DDX_Control(pDX, IDC_COMBO1,		m_cboOneField);
	DDX_Control(pDX, IDC_COMBO4, m_cboEncoding);
}


BEGIN_MESSAGE_MAP(KDlgShapeLink, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KDlgShapeLink::OnBnClickedRadioType)
	ON_EN_CHANGE(IDC_EDIT_BROWSE, &KDlgShapeLink::OnEnChangeEditBrowse)
	ON_BN_CLICKED(IDC_CHECK_ID, &KDlgShapeLink::OnBnClickedCheckId)
	ON_NOTIFY(XTP_NM_REPORT_RECORDS_DROPPED,      IDC_REPORT_FOR,  &KDlgShapeLink::OnReportRecordsDroppedFor)
	ON_NOTIFY(XTP_NM_REPORT_RECORDS_DROPPED,      IDC_REPORT_REV,  &KDlgShapeLink::OnReportRecordsDroppedRev)
    ON_NOTIFY(XTP_NM_REPORT_DROP,                 IDC_REPORT_DRAG, &KDlgShapeLink::OnReportRecordsDroping)
	ON_CBN_SELCHANGE(IDC_COMBO4, &KDlgShapeLink::OnCbnSelchangeCombo4)
	ON_BN_CLICKED(IDOK, &KDlgShapeLink::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDlgShapeLink::OnBnClickedCancel)
END_MESSAGE_MAP()


// KDlgShapeLink 메시지 처리기입니다.


BOOL KDlgShapeLink::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	TCHAR szFilter[] = _T("Shp Files (*.shp)|*.shp|");
	m_editShapeFile.Initialize( this, BES_XT_CHOOSEFILE );
	m_editShapeFile.SetDlgOpenFile( TRUE );
	m_editShapeFile.SetReadOnly( TRUE );
	m_editShapeFile.SetDlgTitle( _T("Select Shape File") );
	m_editShapeFile.SetDlgFilter( szFilter );

	KReportCtrlSetting::Default(m_wndReportForward, TRUE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_wndReportReverse, TRUE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_wndReportDrag, TRUE);
	KReportCtrlSetting::Default(m_wndReportPreview, FALSE, FALSE, FALSE);
	KReportCtrlSetting::AutoColumnSizeFalse(m_wndReportPreview); 

	m_wndReportForward.EnableDragDrop (_T("Drop"), xtpReportAllowDrop, xtpReportDropSelect);
	m_wndReportReverse.EnableDragDrop (_T("Drop"), xtpReportAllowDrop, xtpReportDropSelect);
	m_wndReportDrag.EnableDragDrop(_T("Drop"), xtpReportAllowDragCopy | xtpReportAllowDrop, xtpReportDropSelect);

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
	OnBnClickedRadioType(IDC_RADIO2);

	CheckDlgButton(IDC_CHECK_ID, BST_CHECKED);
	GetDlgItem(IDC_CHECK_GENERLINE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ID)->SetWindowTextW(_T("1"));
	GetDlgItem(IDC_EDIT_ID)->EnableWindow(TRUE);

	IniteReportHeaderForward();
	IniteReportHeaderReverse();
	IniteReportHeaderDrag();

	InitComboEncoding();

	ResizeComponent();

	KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
	KDBaseImportTable::NodeRecord(spDBase, m_mapNodeRecord);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDlgShapeLink::InitComboEncoding()
{
	try
	{
		m_cboEncoding.ResetContent();
		int nCur = m_cboEncoding.AddString(_T("ANSI"));
		m_cboEncoding.SetItemData(nCur, (DWORD_PTR)1);

		nCur = m_cboEncoding.AddString(_T("UTF-8"));
		m_cboEncoding.SetItemData(nCur, (DWORD_PTR)2);

		m_cboEncoding.SetCurSel(0);
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

void KDlgShapeLink::OnBnClickedRadioType( UINT nID )
{
	int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2);
	if (nCheck == IDC_RADIO1)
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		CheckDlgButton(IDC_CHECK_ID, BST_UNCHECKED);
		GetDlgItem(IDC_EDIT_ID)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		CheckDlgButton(IDC_CHECK_ID, BST_CHECKED);
		GetDlgItem(IDC_EDIT_ID)->EnableWindow(TRUE);
	}

	if (m_cboOneField.GetCount() > 0) {
		m_cboOneField.SetCurSel(0);
	}

	UpdateReportForward();
	UpdateReportReverse();
}

void KDlgShapeLink::OnBnClickedCheckId()
{
	if (IsDlgButtonChecked(IDC_CHECK_ID) == BST_CHECKED)
	{
		GetDlgItem(IDC_EDIT_ID)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_ID)->EnableWindow(FALSE);
	}

	UpdateReportForward();
	UpdateReportReverse();
}


void KDlgShapeLink::OnEnChangeEditBrowse()
{
	if (LoadPreViewShapeField() == false)
	{
		RefreshInfo();
		return;
	} else {
		if (KEMGeoPolyLine != m_nGeoType) {
			RefreshInfo();
			//AfxMessageBox(_T("선택한 Shape 파일(PolyLine)이 유효하지 않습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("선택한 Shape 파일(PolyLine)이 유효하지 않습니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("The shape file you selected(PolyLine) is not valid."));
			}
			return;
		}
	}

	IniteOneWayCombo();

	UpdateReportForward();
	UpdateReportReverse();
	UpdateReportDrag();

	IniteReportHeaderPreView();
	UpdateReportPreview();
}

void KDlgShapeLink::RefreshInfo()
{
	m_mapShapeField.clear();
	m_nGeoType = KEMGeoNull;
	m_cboOneField.ResetContent();

	m_wndReportForward.ResetContent();
	m_wndReportReverse.ResetContent();
	m_wndReportDrag.ResetContent();

	m_wndReportPreview.ResetContent();
	m_wndReportPreview.GetColumns()->Clear();
	m_wndReportPreview.Populate();
}

bool KDlgShapeLink::LoadPreViewShapeField()
{
	m_mapShapeField.clear();
	m_nGeoType = KEMGeoNull;

	try
	{
		CString strShapeFile(_T(""));
		m_editShapeFile.GetWindowText(strShapeFile);

		ShapeFileReader oShapeFile; {
			int nEncoding = GetSelectedEncoding();
			oShapeFile.SetEncoding(nEncoding);
			OpenShapeFile(strShapeFile, oShapeFile);
		}

		std::vector<DbfFieldInfo> vecFieldInfo = oShapeFile.FieldInfo();

		int nSizeVec = (int)vecFieldInfo.size();
		for (int i= 0; i< nSizeVec; i++)
		{
			const DbfFieldInfo &oDbfFieldInfo = vecFieldInfo[i];
			m_mapShapeField.insert(std::make_pair(i, oDbfFieldInfo.m_strFieldName));
		}

		if (m_mapShapeField.size() < 1)
			return false;

		m_nGeoType = oShapeFile.ShapeType();

	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);

		m_mapShapeField.clear();
		m_nGeoType = KEMGeoNull;
		return false;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());

		m_mapShapeField.clear();
		m_nGeoType = KEMGeoNull;
		return false;
	} catch (...) {
		TxLogDebugException();

		m_mapShapeField.clear();
		m_nGeoType = KEMGeoNull;
		return false;
	}

	return true;
}

void KDlgShapeLink::OpenShapeFile(CString a_strShapeFile, ShapeFileReader &a_oShapeFile)
{
	a_strShapeFile = a_strShapeFile.Trim();

	try
	{
		if (a_strShapeFile.GetLength() < 1)
			ThrowException(_T("File Not Found")); 

		if( PathFileExists( a_strShapeFile ) == FALSE )
			ThrowException(_T("File Not Found"));  

		int     nIndex       = a_strShapeFile.ReverseFind(_T('\\'));
		CString strFilePath  = a_strShapeFile.Left(nIndex);
		CString strFileTitle = a_strShapeFile.Mid (nIndex+1, a_strShapeFile.GetLength());

		if (a_oShapeFile.Open(strFilePath, strFileTitle) != 1)
			ThrowException(_T("It failed to open file"));  

		int nEntityCount = a_oShapeFile.EntityCount();
		int nRecordCount = a_oShapeFile.RecordCount();
		int nFieldCount  = a_oShapeFile.FieldCount();

		if (nEntityCount != nRecordCount) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("Shape 정보와 속성(dbf) 정보의 개수가 일치하지 않습니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("The number of shape and attribute (dbf) information does not match."));
			}
		}
			//ThrowException(_T("Shape 정보와 속성(dbf) 정보의 개수가 일치하지 않습니다."));

		if (0 == nEntityCount) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("Shape 정보와 개수가 0개 입니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("There is no any shape information in data file."));
			}
		}
			//ThrowException(_T("Shape 정보와 개수가 0개 입니다."));
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		a_oShapeFile.Close();
		ThrowException(_T("Failed to open shape file"));
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		a_oShapeFile.Close();
		throw ex;
	} catch (...) {
		TxLogDebugException();
		a_oShapeFile.Close();
		ThrowException(_T("Failed to open shape file"));
	}
}

void KDlgShapeLink::IniteOneWayCombo()
{
	try
	{
		m_cboOneField.ResetContent();

		int nNullIndex;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nNullIndex = m_cboOneField.AddString(_T("선택해주세요."));
		}
		else
		{
			nNullIndex = m_cboOneField.AddString(_T("Please Select."));
		}
		m_cboOneField.SetItemData(nNullIndex, -1);

		AutoType iter = m_mapShapeField.begin();
		AutoType end  = m_mapShapeField.end();
		while(iter != end)
		{
			int nIndex = m_cboOneField.AddString(iter->second);
			m_cboOneField.SetItemData(nIndex, DWORD_PTR(iter->first));
			++iter;
		}

		m_cboOneField.SetCurSel(nNullIndex);

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


void KDlgShapeLink::IniteReportHeaderForward()
{
	try
	{
		CXTPReportColumn* pColumn = m_wndReportForward.AddColumn( new CXTPReportColumn( 0, _T("System Column"), 100 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("시스템 컬럼"));
		}

		pColumn = m_wndReportForward.AddColumn( new CXTPReportColumn( 1, _T("Type"), 40 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("데이터 유형"));
		}

		pColumn = m_wndReportForward.AddColumn( new CXTPReportColumn( 2, _T("Prerequisite"), 50 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		pColumn->SetVisible(FALSE);

		pColumn = m_wndReportForward.AddColumn( new CXTPReportColumn( 3, _T("File Column"), 100 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("파일 컬럼"));
		}

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

void KDlgShapeLink::IniteReportHeaderReverse()
{
	try
	{
		CXTPReportColumn* pColumn = m_wndReportReverse.AddColumn( new CXTPReportColumn( 0, _T("System Column"), 100 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("시스템 컬럼"));
		}

		pColumn = m_wndReportReverse.AddColumn( new CXTPReportColumn( 1, _T("Type"), 40 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("데이터 유형"));
		}

		pColumn = m_wndReportReverse.AddColumn( new CXTPReportColumn( 2, _T("Prerequisite"), 50 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		pColumn->SetVisible(FALSE);

		pColumn = m_wndReportReverse.AddColumn( new CXTPReportColumn( 3, _T("File Column"), 100 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("파일 컬럼"));
		}

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

void KDlgShapeLink::IniteReportHeaderDrag()
{
	try
	{
		CXTPReportColumn* pColumn = m_wndReportDrag.AddColumn( new CXTPReportColumn( 0, _T("Drag & Drop"), 100 ) );
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("끌어 놓기"));
		}
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

void KDlgShapeLink::IniteReportHeaderPreView()
{
	try
	{
		m_wndReportPreview.GetColumns()->Clear();
		CXTPReportColumn* pColumn = nullptr;
		int nIndex = 0;

		AutoType iter = m_mapShapeField.begin();
		AutoType end  = m_mapShapeField.end();
		while(iter != end)
		{
			pColumn = m_wndReportPreview.AddColumn(new CXTPReportColumn (nIndex, iter->second, 50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetSortable(FALSE);
			pColumn->SetEditable(FALSE);

			nIndex++;
			++iter;
		}
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

void KDlgShapeLink::UpdateReportForward()
{
	m_wndReportForward.ResetContent(TRUE);

	if (m_mapShapeField.size() < 1)
	{
		return;
	}

	try
	{
		const KIOColumns* pColumns = m_pTable->Columns();
		int nColumnCount = pColumns->ColumnCount();

		for (int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pColumn		= pColumns->GetColumn(i);
			CString strColumnName	= pColumn->Name();

			if (IsDlgButtonChecked(IDC_CHECK_ID) == BST_CHECKED && strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0)
			{
				continue;
			}

			CXTPReportRecord*     pRecord	= m_wndReportForward.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem		= nullptr;

			bool bPK(false);
			if (_tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_LINK_TYPE) == 0 || 
				_tcsicmp(pColumn->Name(), COLUMN_FNODE_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_TNODE_ID) == 0 )
			{
				bPK = true;
			}

			CString	strColumnCaption	= pColumn->Caption();
			if (bPK) {
				CString strCaptionPK(_T(""));
				strCaptionPK.Format(_T("* %s"), strColumnCaption);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strCaptionPK));
			}
			else {
				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strColumnCaption));
			}
			pItem->SetItemData((DWORD_PTR)pColumn);
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			if (bPK) {
				pItem->SetTextColor(RGB(255,0,0));
			}

			CString strDataType(_T(""));
			switch(pColumn->DataType())
			{
			case KEMIODataTypeInteger : strDataType = DATATYPESTRING_INTEGER; break;
			case KEMIODataTypeDouble  : strDataType = DATATYPESTRING_DOUBLE;  break;
			case KEMIODataTypeString  : strDataType = DATATYPESTRING_STRING;  break;
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDataType));
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			CString strPK = _T("N");
			if (true == bPK) {
				strPK = _T("Y");
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strPK));
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			int nIndex			= 0;
			int nCurrentIndex	= 0;

			CXTPReportRecordItemComboEx* pItemCombo   = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());

			nIndex = pItemCombo->AddString(_T("Null"), (DWORD_PTR)-1);

			AutoType iter = m_mapShapeField.begin();
			AutoType end  = m_mapShapeField.end();
			while(iter != end)
			{
				int nFieldIndex		= (int)iter->first;
				CString strName		= iter->second;

				nIndex = pItemCombo->AddString(strName, (DWORD_PTR)nFieldIndex);

				if (strColumnCaption.CompareNoCase(strName) == 0 || strColumnName.CompareNoCase(strName) == 0)
				{
					nCurrentIndex = nIndex;
				}

				++iter;
			}

			pItemCombo->SetCurSel(nCurrentIndex);
			pItemCombo->SetAlignment(DT_CENTER);
			pItemCombo->SetEditable(TRUE);
		}

		m_wndReportForward.FocusSubItems( TRUE );
		m_wndReportForward.Populate();
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

void KDlgShapeLink::UpdateReportReverse()
{
	m_wndReportReverse.ResetContent(TRUE);

	int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2);
	if (nCheck == IDC_RADIO1)
	{
		return;
	}

	if (m_mapShapeField.size() < 1)
	{
		return;
	}

	try
	{
		const KIOColumns* pColumns = m_pTable->Columns();
		int nColumnCount = pColumns->ColumnCount();

		for (int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pColumn		= pColumns->GetColumn(i);
			CString strColumnName	= pColumn->Name();

			if (IsDlgButtonChecked(IDC_CHECK_ID) == BST_CHECKED && strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0)
			{
				continue;
			}

			CXTPReportRecord*     pRecord	= m_wndReportReverse.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem		= nullptr;

			bool bPK(false);
			if (_tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_LINK_TYPE) == 0 || 
				_tcsicmp(pColumn->Name(), COLUMN_FNODE_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_TNODE_ID) == 0 )
			{
				bPK = true;
			}

			CString	strColumnCaption	= pColumn->Caption();
			if (bPK) {
				CString strCaptionPK(_T(""));
				strCaptionPK.Format(_T("* %s"), strColumnCaption);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strCaptionPK));
			}
			else {
				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strColumnCaption));
			}
			pItem->SetItemData((DWORD_PTR)pColumn);
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			if (bPK) {
				pItem->SetTextColor(RGB(255,0,0));
			}

			CString strDataType(_T(""));
			switch(pColumn->DataType())
			{
			case KEMIODataTypeInteger : strDataType = DATATYPESTRING_INTEGER; break;
			case KEMIODataTypeDouble  : strDataType = DATATYPESTRING_DOUBLE;  break;
			case KEMIODataTypeString  : strDataType = DATATYPESTRING_STRING;  break;
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDataType));
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			CString strPK = _T("N");
			if (true == bPK) {
				strPK = _T("Y");
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strPK));
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			int nIndex			= 0;
			int nCurrentIndex	= 0;

			CXTPReportRecordItemComboEx* pItemCombo   = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());

			nIndex = pItemCombo->AddString(_T("Null"), (DWORD_PTR)-1);

			AutoType iter = m_mapShapeField.begin();
			AutoType end  = m_mapShapeField.end();
			while(iter != end)
			{
				int nFieldIndex		= (int)iter->first;
				CString strName		= iter->second;

				nIndex = pItemCombo->AddString(strName, (DWORD_PTR)nFieldIndex);

				if (strColumnCaption.CompareNoCase(strName) == 0 || strColumnName.CompareNoCase(strName) == 0)
				{
					nCurrentIndex = nIndex;
				}

				++iter;
			}

			pItemCombo->SetCurSel(nCurrentIndex);
			pItemCombo->SetAlignment(DT_CENTER);
			pItemCombo->SetEditable(TRUE);
		}

		m_wndReportReverse.FocusSubItems( TRUE );
		m_wndReportReverse.Populate();
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

void KDlgShapeLink::UpdateReportDrag()
{
	m_wndReportDrag.ResetContent(TRUE);

	try
	{
		CXTPReportRecord*     pRecord	= m_wndReportDrag.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem		= nullptr;

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Null")));
		pItem->SetItemData((DWORD_PTR)-1);
		pItem->SetAlignment(DT_CENTER);
		//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		AutoType iter = m_mapShapeField.begin();
		AutoType end  = m_mapShapeField.end();
		while(iter != end)
		{

			int		nFieldIndex			= (int)iter->first;
			CString	strColumnCaption	= iter->second;

			pRecord	= m_wndReportDrag.AddRecord(new CXTPReportRecord);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strColumnCaption));
			pItem->SetItemData((DWORD_PTR)nFieldIndex);
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			++iter;
		}

		m_wndReportDrag.Populate();
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

void KDlgShapeLink::UpdateReportPreview()
{
	m_wndReportPreview.ResetContent(TRUE);

	try
	{
		const int nShapeFieldCnt = (int)m_mapShapeField.size();
		if (nShapeFieldCnt < 1)
			return;

		CString strShapeFile(_T(""));
		m_editShapeFile.GetWindowText(strShapeFile);

		ShapeFileReader oShapeFile; {
			int nEncoding = GetSelectedEncoding();
			oShapeFile.SetEncoding(nEncoding);
			OpenShapeFile(strShapeFile, oShapeFile);
		}

		const int nLimitPreview(100);

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		int     nGridSeq(1);
		CString strGridSeq(_T(""));

		int nEntityCount = oShapeFile.EntityCount();
		for (int i= 0; i< nEntityCount; i++)
		{
			if (i >= nLimitPreview)
				break;

			std::vector<DbfFieldValue> vecFieldValue = oShapeFile.ReadRecord(i);

			int nSizeValue = (int)vecFieldValue.size();
			if (nShapeFieldCnt != nSizeValue)
				continue;

			pRecord = m_wndReportPreview.AddRecord(new CXTPReportRecord());

			for (int k= 0; k< nSizeValue; k++)
			{
				const DbfFieldValue &oDbfFieldValue = vecFieldValue[k];

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oDbfFieldValue.m_strValue));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
			}
		}

		m_wndReportPreview.Populate();
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

void KDlgShapeLink::OnReportRecordsDroppedFor( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);

	try
	{
		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		int nCount = pRecords->GetCount();
		if (nCount < 1)
		{
			return;
		}

		CXTPReportRecord*  pDropRecord    = pRecords->GetAt(0);
		CXTPReportRecords* pTargetRecords = m_wndReportForward.GetRecords();

		if (nullptr != pDropRecord)
		{
			int                   nRowIndex = pDropRecord->GetIndex(); // will be Drop Row's Index
			CXTPReportRecordItem* pItem2    = pDropRecord->GetItem(0);
			int					  nItemData = pItem2->GetItemData();

			pTargetRecords->RemoveRecord(pDropRecord);
			m_wndReportForward.Populate();

			pTargetRecords = m_wndReportForward.GetRecords();
			CXTPReportRecord* pTargetRecord = pTargetRecords->GetAt(nRowIndex); // pItemNotify->pTargetRecord 사용 불가

			if (nullptr != pTargetRecord)
			{
				CXTPReportRecordItemComboEx* pItemCombo = (CXTPReportRecordItemComboEx*)pTargetRecord->GetItem(3);
				CXTPReportRecordItemEditOptions* pOption = pItemCombo->GetEditOptions(NULL);
				CXTPReportRecordItemConstraint* pCons = pOption->FindConstraint((DWORD_PTR)nItemData);
				int nIndex = pCons->GetIndex();
				pItemCombo->SetCurSel(nIndex);

				CXTPReportRow* pRow = m_wndReportForward.GetRows()->Find(pTargetRecord);
				m_wndReportForward.SetFocusedRow(pRow);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDlgShapeLink::OnReportRecordsDroppedRev( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);

	try
	{
		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		int nCount = pRecords->GetCount();
		if (nCount < 1)
		{
			return;
		}

		CXTPReportRecord*  pDropRecord    = pRecords->GetAt(0);
		CXTPReportRecords* pTargetRecords = m_wndReportReverse.GetRecords();

		if (nullptr != pDropRecord)
		{
			int                   nRowIndex = pDropRecord->GetIndex(); // will be Drop Row's Index
			CXTPReportRecordItem* pItem2    = pDropRecord->GetItem(0);
			int					  nItemData = pItem2->GetItemData();

			pTargetRecords->RemoveRecord(pDropRecord);
			m_wndReportReverse.Populate();

			pTargetRecords = m_wndReportReverse.GetRecords();
			CXTPReportRecord* pTargetRecord = pTargetRecords->GetAt(nRowIndex); // pItemNotify->pTargetRecord 사용 불가

			if (nullptr != pTargetRecord)
			{
				CXTPReportRecordItemComboEx* pItemCombo = (CXTPReportRecordItemComboEx*)pTargetRecord->GetItem(3);
				CXTPReportRecordItemEditOptions* pOption = pItemCombo->GetEditOptions(NULL);
				CXTPReportRecordItemConstraint* pCons = pOption->FindConstraint((DWORD_PTR)nItemData);
				int nIndex = pCons->GetIndex();
				pItemCombo->SetCurSel(nIndex);

				CXTPReportRow* pRow = m_wndReportReverse.GetRows()->Find(pTargetRecord);
				m_wndReportReverse.SetFocusedRow(pRow);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDlgShapeLink::OnReportRecordsDroping( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	try
	{
		XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);

		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		int nCount = pRecords->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			m_wndReportDrag.RemoveRecordEx(pRecord);
		}
		m_wndReportDrag.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDlgShapeLink::OnBnClickedOk()
{
	if (!InvalidateInputInfo())
		return;

	bool bHadData(false); // 기존에 저장된 데이터가 있었는지 판단

	KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();

	if (KImportCommon::IsEmptyTable(m_pTarget, TABLE_NODE) == true) {// true: 데이터무, false : 데이터 존재
		//AfxMessageBox(_T("노드 데이터 불러오기를 먼저 수행해 주세요."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("노드 데이터 불러오기를 먼저 수행해 주세요."));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Import Node data first !"));
		}
	}

	if (KImportCommon::IsEmptyTable(m_pTarget, TABLE_LINK) == false) { // true: 데이터무, false : 데이터 존재
		bHadData = true;
	}
	
	CString strMsg(_T(""));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		if (bHadData) {
			strMsg = _T("데이터 불러오기 실행 후 기존 데이터가 삭제됩니다.\r\n");
			strMsg.Append(_T("계속 진행하시겠습니까?"));
		}
		else {
			strMsg.Append(_T("진행하시겠습니까?"));
		}
	}
	else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
		if (bHadData) {
			strMsg = _T("WARNING : Existing data would be delete.\r\n");
			strMsg.Append(_T("Do you want to continue?"));
		}
		else {
			strMsg.Append(_T("Do you want to continue?"));
		}
	}
	
	
	if (AfxMessageBox(strMsg, MB_OKCANCEL) != IDOK) {
		return;
	}

    KImportCommon::DeleteLinkRelation(spDBase); // 트랜잭션을 활용할 수가 없다. (spDBase와 spMapServer는 같은 곳을 바라보고 있기 때문)

    //-------------------------------------------------------------

    CString strShapeFile(_T("")); {
        m_editShapeFile.GetWindowText(strShapeFile);
    }

    Integer nxLinkID(0); {
        if (IsDlgButtonChecked(IDC_CHECK_ID) == BST_CHECKED)
        {
            CString strID(_T(""));
            GetDlgItem(IDC_EDIT_ID)->GetWindowText(strID);
            nxLinkID = _ttoi64(strID);
        }
    }

    int nLineType(1); { // 1:양선 타입, 2:단선 타입
        int nCheck = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
        if (IDC_RADIO1 == nCheck) {
            nLineType = 1;
        }
        else {
            nLineType = 2;
        }
    }

    BOOL bCustomID(FALSE);
    if (IsDlgButtonChecked(IDC_CHECK_ID) == BST_CHECKED)
        bCustomID = TRUE;

    int nOneWayIndex = -1;
    if (GetDlgItem(IDC_COMBO1)->IsWindowEnabled()) {
        nOneWayIndex = (int)m_cboOneField.GetItemData(m_cboOneField.GetCurSel());
    }

    ITxMapServerPtr spMapServer = nullptr;
    
    try
    {
        if (nullptr != m_spFeatureLayer) {
            m_spFeatureLayer.reset();
            m_spFeatureLayer = nullptr;
        }

        LoadLayerInfo(m_pTarget, spMapServer, m_spFeatureLayer);
        if (nullptr == spMapServer)
            throw 1;

        if (nullptr == m_spFeatureLayer)
            throw 2;
    }
    catch (int& ex) {
        CString strError(_T(""));
        if (1 == ex) {
            strError.Format(_T("Error : %d (server 접속 실패)"), ex);
        }
        else if (2 == ex) {
            strError.Format(_T("Error : %d (레이어 조회 실패)"), ex);
        }
        else {
            strError.Format(_T("Error : %d"), ex);
        }
        TxLogDebug((LPCTSTR)strError);

        //AfxMessageBox(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			AfxMessageBox(_T("Failed to import shape data."));
		}
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
        //AfxMessageBox(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			AfxMessageBox(_T("Failed to import shape data."));
		}
    }
    catch (...) {
        TxLogDebugException();
        //AfxMessageBox(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			AfxMessageBox(_T("Failed to import shape data."));
		}
    }

    spMapServer->BeginTransaction();

	try
	{
		ThreadPara oPara(this);
		if (true) {
            m_mapReadLinkType.clear();
            
            int nInsertCnt(0);
            int nFailedCnt(0);

			oPara.TKeyStr[0] = strShapeFile; // shape 파일

            oPara.TKeyInt[0] = nxLinkID;

            oPara.TKeyInt32[0] = nLineType;
            oPara.TKeyInt32[1] = bCustomID;;
            oPara.TKeyInt32[2] = nOneWayIndex;
            oPara.TKeyInt32[3] = nInsertCnt; // 결과를 리턴 받는다.
            oPara.TKeyInt32[4] = nFailedCnt; // 결과를 리턴 받는다.

			RThreadInfo.Init();
			
			// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
			int nLang = 1;
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				nLang = 0;
			}

			QBicSimpleProgressThread::ExecuteThread(ThreadRun, &oPara, false, nLang); 

			if (RThreadInfo.IsOK() == false) {
                nFailedCnt = oPara.TKeyInt32[4];

                if (nFailedCnt > 0) { //실패 건수가 있을 경우
                    ErrorFileOpen();
                }

				ThrowException(RThreadInfo.ErrorMsg());
			} else {
                CString strMsg(_T("")); {
                    int nIdx(1);

                    nInsertCnt = oPara.TKeyInt32[3];

					if (KmzSystem::GetLanguage() == KEMKorea) {
						strMsg.Append(_T("입력된 데이터의 내역은 아래와 같습니다.\r\n"));
						strMsg.AppendFormat(_T("#1. 데이터 건수: %d\r\n"), nInsertCnt);
						strMsg.Append(_T("#2. 링크 타입별 건수 :\r\n"));

						auto iter = m_mapReadLinkType.begin();
						auto itEnd = m_mapReadLinkType.end();
						for (; iter != itEnd; ++iter) {
							const int &nLinkType = iter->first;
							int       &nCount = iter->second;

							strMsg.AppendFormat(_T("- (%d): %d건\r\n"), nLinkType, nCount);
						}

						strMsg.Append(_T("\n계속 진행하시겠습니까?\r\n"));
					}
					else {
						strMsg.Append(_T("Imported data are as follows.\r\n"));
						strMsg.AppendFormat(_T("#1. Total number of records : %d\r\n"), nInsertCnt);
						strMsg.Append(_T("#2. The number of records by link type :\r\n"));

						auto iter = m_mapReadLinkType.begin();
						auto itEnd = m_mapReadLinkType.end();
						for (; iter != itEnd; ++iter) {
							const int &nLinkType = iter->first;
							int       &nCount = iter->second;

							strMsg.AppendFormat(_T("- (%d): %d\r\n"), nLinkType, nCount);
						}

						strMsg.Append(_T("\nDo you want to continue?\r\n"));
					}
                    
                }

                if (AfxMessageBox(strMsg, MB_YESNO) == IDNO) {
                    ThrowException(_T("User Canceled."));
                }
			}
		}
	} catch (int& ex) {
        spMapServer->Rollback();
        
        CString strError(_T(""));
		//strError.Format(_T("오류 : %d"), ex);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strError.Format(_T("오류 : %d"), ex);
		}
		else {
			strError.Format(_T("Error : %d"), ex);
		}
		TxLogDebug((LPCTSTR)strError);
		AfxMessageBox(strError);
		return;
	} catch (KExceptionPtr ex) {
        spMapServer->Rollback();

		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
		return;
	} catch (...) {
        spMapServer->Rollback();

		TxLogDebugException();
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
		return;
	}

    CWaitCursor cw;

    if (true) { // 완전 성공
        spMapServer->Commit();

        if (nullptr != m_spFeatureLayer)
            m_spFeatureLayer->RebuildIndex(); // RebuildIndex 수행시 트랙젠션 관계로, 외부의 트랙젝션이 끝난후 수행되야 한다.
    }

    try
    {
        KNetworkEditLog::SetImportNetworkLog(m_pTarget);

        KDefineSystemCodeDlg oSystemDefineDlg(m_pTarget, true);
        oSystemDefineDlg.DoModal();

        try
        {
            KImportCommon::NotifyLinkRelation(m_pTarget);
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

        KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(m_pTarget);
        if (nullptr != pCreatedMapView) {
            pCreatedMapView->ReloadRender(false, 2);
            pCreatedMapView->FullExtentMap();
        }
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

	KResizeDialogEx::OnOK();
}

bool KDlgShapeLink::InvalidateInputInfo()
{
	try
	{
		CString strShapeFile(_T(""));
		m_editShapeFile.GetWindowText(strShapeFile);
		if (strShapeFile.IsEmpty())
			throw 1;

		if (KEMGeoNull == m_nGeoType)
			throw 2;
	}
	catch (int& ex)
	{
#pragma region 예외처리 메시지
		if (KmzSystem::GetLanguage() == KEMKorea) {
			if (1 == ex)
				AfxMessageBox(_T("Shape 파일을 선택해 주세요."));
			else if (2 == ex)
				AfxMessageBox(_T("선택한 Shape 파일이 유효하지 않습니다."));
		}
		else {
			if (1 == ex)
				AfxMessageBox(_T("Select the shape file."));
			else if (2 == ex)
				AfxMessageBox(_T("The shape file you selected is not valid."));
		}
#pragma endregion 예외처리 메시지

		return false;
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	const int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2);
	if (nCheck == IDC_RADIO2) { // Oneway Type 일시 관련 필드 매칭을 하였는지 체크
		int nCurSel = m_cboOneField.GetCurSel();

		if (nCurSel < 0 ) {
			//AfxMessageBox(_T("양방향/단방향 구분 컬럼을 선택해 주세요."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("양방향/단방향 구분 컬럼을 선택해 주세요."));
			}
			else {
				AfxMessageBox(_T("Please select 'DIR' column."));
			}
			GetDlgItem(IDC_COMBO1)->SetFocus();
			return false;
		}

		int nOneWayIndex =  m_cboOneField.GetItemData(nCurSel);
		if (-1 == nOneWayIndex) { // NULL 선택이면
			//AfxMessageBox(_T("양방향/단방향 구분 컬럼을 선택해 주세요."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("양방향/단방향 구분 컬럼을 선택해 주세요."));
			}
			else {
				AfxMessageBox(_T("Please select 'DIR' column."));
			}
			GetDlgItem(IDC_COMBO1)->SetFocus();
			return false;
		}
	}

	if (IsDlgButtonChecked(IDC_CHECK_ID) == BST_CHECKED)
	{
		CString strID(_T(""));
		GetDlgItem(IDC_EDIT_ID)->GetWindowText(strID);
		if (QBicStringChecker::IsUnSignedInteger(strID) == false)
		{
			//AfxMessageBox(_T("ID에 잘못된 값을 입력하였습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("ID 값 추출 실패."));
			}
			else {
				AfxMessageBox(_T("Failed to read ID value."));
			}
			GetDlgItem(IDC_EDIT_ID)->SetFocus();
			return false;
		}
	}

	if (InValidForColumnData() == false)
	{
		return false;
	}

	if (InValidRevColumnData() == false)
	{
		return false;
	}

	if (nCheck == IDC_RADIO2)
	{
		if (CheckForwardReverseField() == false)
		{
			return false;
		}
	}

	return true;
}


unsigned __stdcall KDlgShapeLink::ThreadRun( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	if (pParameter != nullptr) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KDlgShapeLink* pDlg = (KDlgShapeLink*)pPara->TWindow;

			pDlg->Execute(pPara);
		}
	}

	return 1;
}

void KDlgShapeLink::Execute(ThreadPara* pPara)
{
	try 
	{
		CString strShapeFile(_T(""));
		strShapeFile = pPara->TKeyStr[0];

		ShapeFileReader oShapeFile; {
			int nEncoding = GetSelectedEncoding();
			oShapeFile.SetEncoding(nEncoding);
			OpenShapeFile(strShapeFile, oShapeFile);
		}

		bool bErrFile(true); // ErrFile 셋팅시 문제가 발생하더라도, 이와 관계 없이 진행 시키기 위해
		QBicFile::DeleteFile(m_strErrFile);
		QBicTxtWriter ofErr; {
			if (ofErr.Open(m_strErrFile) == false)
				bErrFile = false;
		}
		ImportShapeFileData(pPara, oShapeFile, bErrFile, ofErr);
		if (bErrFile) {
			ofErr.Close();
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		RThreadInfo.SetErrorFlag(-1, strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	} catch (...) {
		TxLogDebugException();
		//RThreadInfo.SetErrorFlag(-1, _T("오류가 발생하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			RThreadInfo.SetErrorFlag(-1, _T("오류가 발생하였습니다."));
		}
		else {
			RThreadInfo.SetErrorFlag(-1, _T("Error occured !"));
		}
	}
}

void KDlgShapeLink::ImportShapeFileData(ThreadPara* pPara, ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr)
{
    int nLineType   = pPara->TKeyInt32[0];
    int &nInsertCnt = pPara->TKeyInt32[3]; //결과 리턴
    int &nFailedCnt = pPara->TKeyInt32[4]; //결과 리턴
    nInsertCnt = 0;
    nFailedCnt = 0;

	try
	{
        std::set<int> setFwrFailedEntity;
        std::set<int> setRvrFailedEntity;
        std::map<int, int> mapReadLinkType;

		SaveForwardFeatureData(pPara, a_oShapeFile, a_bErrFile, ofErr, OUT nInsertCnt, setFwrFailedEntity);
		if (2 == nLineType) {
			SaveReverseFeatureData(pPara, a_oShapeFile, a_bErrFile, ofErr, OUT nInsertCnt, setRvrFailedEntity);
		}

        if (setFwrFailedEntity.size() > 0 || setRvrFailedEntity.size() > 0) { //실패 건수가 있을 경우
            nFailedCnt += (int)setFwrFailedEntity.size();
            nFailedCnt += (int)setRvrFailedEntity.size();

            CString strErrMsg(_T(""));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strErrMsg.Format(_T("불량 데이터 %d건이 발견되어 데이터 불러오기를 중단합니다.\r\n"), nFailedCnt);
				strErrMsg.AppendFormat(_T("%s 파일을 통해 상세 내역을 확인해 주세요."), IMPORT_ERROR_FILE_NAME);
			}
			else {
				strErrMsg.Format(_T("%d bad data found and stops importing data.\r\n"), nFailedCnt);
				strErrMsg.AppendFormat(_T("Check the details from the %s."), IMPORT_ERROR_FILE_NAME);
			}

            ThrowException(strErrMsg);
        }
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);

		//ThrowException(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);

		throw ex;
	} catch (...) {
		TxLogDebugException();

		//ThrowException(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
	}
}

void KDlgShapeLink::LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spFeatureLayer, bool bInitExtent)
{
	try 
	{
		KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(a_pTarget);
		if (pCreatedMapView != nullptr) { // MapView가 생성되어 있는지 확인
			ITxFeatureLayerPtr spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Link());
			if (spLayer != nullptr) {
				spFeatureLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spMapServer = spFeatureLayer->GetServer();
		} else { // MapView가 생성 되어 있지 않으면 서버에 직접 접속
			CString strServerPath = a_pTarget->GetIODataFilePath();
			spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, strServerPath));
			if ( spMapServer->Connect() != 1)  
				throw 1;

			// 서버의 레이어 정보 조회
			std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
			for (size_t j=0; j<vecLayerInfo.size(); j++) {
				TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

				if (_tcsicmp(m_pTable->Name(), spLayerInfo->Name()) == 0) { // 링크 레이어 조회
					spFeatureLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
					break;
				}
			}
		}

		if (bInitExtent) {
			if (nullptr != spFeatureLayer) {
				TxLayerInfoPtr    spLayerInfo    = spFeatureLayer->LayerInfo();
				TxLayerInfoSQLPtr spLayerInfoSQL = std::dynamic_pointer_cast<TxLayerInfoSQL>(spLayerInfo);
				spLayerInfoSQL->SetExtent(0.0,0.0,0.0,0.0); // 초기화
			}
		}
	} catch (int& ex) {
		TxLogDebugException();
		throw ex;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		throw ex;
	} catch (...) {
		TxLogDebugException();
		//ThrowException(_T("Import Shape에 실패 하였습니다.")); 
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
	}
}

void KDlgShapeLink::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

bool KDlgShapeLink::InValidForColumnData()
{
	m_vecForShapeMatting.clear();

	try
	{
		CXTPReportRecords* pRecords = m_wndReportForward.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord*		pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem*	pItem	= pRecord->GetItem(0);
			KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();

			CXTPReportRecordItemComboEx* pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->GetItem(3);
			int nCursel = pItemCombo->GetCurSel();
			int nItemData = (int)pItemCombo->GetItemData(nCursel);

			if (_tcsicmp(pColumn->Name(), COLUMN_FNODE_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_TNODE_ID) == 0 ||
				_tcsicmp(pColumn->Name(), COLUMN_LINK_TYPE) == 0 || _tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0)
			{
				if (nItemData < 0)
				{
					CString strMsg(_T(""));
					//strMsg.Format(_T("%s 컬럼에 대한 매칭 컬럼 정보가 존재하지 않습니다."), pColumn->Caption());
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strMsg.Format(_T("%s 컬럼에 대한 매칭 컬럼 정보가 존재하지 않습니다."), pColumn->Caption());
					}
					else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
						strMsg.Format(_T("There is no any matched information for %s Column."), pColumn->Caption());
					}
					AfxMessageBox(strMsg);
					return false;
				}
			}

			if (nItemData >= 0)
			{
				KImportShapeMatching oMatchingInfo;
				oMatchingInfo.pMatColumn	= pColumn;
				oMatchingInfo.nShapeIndex	=(long)nItemData;
				m_vecForShapeMatting.push_back(oMatchingInfo);
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KDlgShapeLink::InValidRevColumnData()
{
	m_vecRevShapeMatting.clear();

	try
	{
		CXTPReportRecords* pRecords = m_wndReportReverse.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord*		pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem*	pItem	= pRecord->GetItem(0);
			KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();

			CXTPReportRecordItemComboEx* pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->GetItem(3);
			int nCursel = pItemCombo->GetCurSel();
			int nItemData = (int)pItemCombo->GetItemData(nCursel);

			if (_tcsicmp(pColumn->Name(), COLUMN_FNODE_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_TNODE_ID) == 0 ||
				_tcsicmp(pColumn->Name(), COLUMN_LINK_TYPE) == 0 || _tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0)
			{
				if (nItemData < 0)
				{
					CString strMsg(_T(""));
					//strMsg.Format(_T("(역방향)%s 컬럼에 대한 매칭 컬럼 정보가 존재하지 않습니다."), pColumn->Caption());
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strMsg.Format(_T("%s 컬럼에 대한 매칭 컬럼 정보가 존재하지 않습니다."), pColumn->Caption());
					}
					else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
						strMsg.Format(_T("There is no any matched information for %s Column(Reverse)."), pColumn->Caption());
					}
					AfxMessageBox(strMsg);
					return false;
				}
			}
			if (nItemData >= 0)
			{
				KImportShapeMatching oMatchingInfo;
				oMatchingInfo.pMatColumn	= pColumn;
				oMatchingInfo.nShapeIndex	=(long)nItemData;
				m_vecRevShapeMatting.push_back(oMatchingInfo);
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KDlgShapeLink::CheckForwardReverseField()
{
	try
	{
		long nForID(0), nRevID(1);
		long nForFnode(0), nRevFnode(0);
		long nForTnode(0), nRevTnode(0);

		for (size_t i = 0; i < m_vecForShapeMatting.size(); i++)
		{
			KImportShapeMatching oInfo = m_vecForShapeMatting[i];
			if (_tcsicmp(oInfo.pMatColumn->Name(), COLUMN_LINK_ID) == 0)
			{
				nForID = oInfo.nShapeIndex;
			}
			else if (_tcsicmp(oInfo.pMatColumn->Name(), COLUMN_FNODE_ID) == 0)
			{
				nForFnode = oInfo.nShapeIndex;
			}
			else if (_tcsicmp(oInfo.pMatColumn->Name(), COLUMN_TNODE_ID) == 0)
			{
				nForTnode = oInfo.nShapeIndex;
			}
		}

		for (size_t i = 0; i < m_vecRevShapeMatting.size(); i++)
		{
			KImportShapeMatching oInfo = m_vecRevShapeMatting[i];
			if (_tcsicmp(oInfo.pMatColumn->Name(), COLUMN_LINK_ID) == 0)
			{
				nRevID = oInfo.nShapeIndex;
			}
			else if (_tcsicmp(oInfo.pMatColumn->Name(), COLUMN_FNODE_ID) == 0)
			{
				nRevFnode = oInfo.nShapeIndex;
			}
			else if (_tcsicmp(oInfo.pMatColumn->Name(), COLUMN_TNODE_ID) == 0)
			{
				nRevTnode = oInfo.nShapeIndex;
			}
		}

		if (nRevID == nForID)
		{
			//AfxMessageBox(_T("링크 ID 컬럼이 역방향 링크 ID 컬럼과 동일합니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("링크 ID 컬럼이 역방향 링크 ID 컬럼과 동일합니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("Link ID column should NOT be the same as Reverse Link ID column."));
			}
			return false;
		}

		if (nForFnode == nRevFnode)
		{
			//AfxMessageBox(_T("링크 F_Node 컬럼이 역방향 링크 F_Node 컬럼과 동일합니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("링크 F_Node 컬럼이 역방향 링크 F_Node 컬럼과 동일합니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("FromNode column should NOT be the same as Reverse FromNode column."));
			}
			return false;
		}

		if (nForTnode == nRevTnode)
		{
			//AfxMessageBox(_T("링크 T_Node 컬럼이 역방향 링크 T_Node 컬럼과 동일합니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("링크 F_Node 컬럼이 역방향 링크 F_Node 컬럼과 동일합니다."));
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("ToNode column should NOT be the same as Reverse ToNode column."));
			}
			return false;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

void KDlgShapeLink::SaveForwardFeatureData(ThreadPara* pPara, ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr, int &a_nInsertCnt, std::set<int> &a_setFwrFailedEntity)
{
	bool    bUseForwardLength(false);
	CString strErrorMsg(_T(""));

    Integer &nxCustomLinkID = pPara->TKeyInt[0]; // SaveForwardFeatureData까지 연결되서 진행해야 하기 때문에 참조

    BOOL    bCustomID       = (BOOL)pPara->TKeyInt32[1];
    int     nOneWayIndex    = pPara->TKeyInt32[2];

	try
	{
		const int nShapeType    = a_oShapeFile.ShapeType();
		const int nEntityCount  = a_oShapeFile.EntityCount();

		for (int i= 0; i< nEntityCount; i++)
		{
            bool bErrLine(false);

			if (i % 1000 == 0 || i == nEntityCount-1) {
				CString strMsg(_T(""));
				strMsg.Format(_T("#1 Read Forward Shape data ( %d / %d )"), (i+1), nEntityCount);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMsg.Format(_T("#1 (정방향) 읽은 파일 라인수 / 총 라인수 :  %d / %d"), (i+1), nEntityCount);
				}
				AddStatusMessage( strMsg );
			}
			QBicBinary oBinary(20);
			a_oShapeFile.ReadShape(i, oBinary);

			int nGeometryType(0);
			oBinary.Position(0);
			oBinary.Read(nGeometryType);

            if (KEMGeoPolyLine != nGeometryType) {

                strErrorMsg.Format(_T("Forward-Line : %d =>\t None PolyLine"), (i + 1));
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;

                a_setFwrFailedEntity.insert(i); // 여기는 특수한 continue 니깐 추가 해줘야지
                continue;
            }

			std::vector<DbfFieldValue> vecField = a_oShapeFile.ReadRecord(i);

			//OneWay Field 확인
			bool bOffset(false);
			if (nOneWayIndex != -1)
			{
				CString strValue    = vecField[nOneWayIndex].m_strValue;
				int     nOneWayType = _ttoi(strValue);

				if ( -1 == nOneWayType)
				{
					continue;
				}
				else if ( 0 != nOneWayType && 1 != nOneWayType)
				{
                    //strErrorMsg.Format(_T("Forward-Line : %d =>\t 선택한 OneWay 필드에 0 또는 1 이외의 값이 들어가 있습니다."), (i + 1));
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strErrorMsg.Format(_T("Forward-Line : %d =>\t 선택한 양방향/단방향 구분 컬럼에 0 또는 1 이외의 값이 들어가 있습니다."), (i + 1));
					}
					else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
						strErrorMsg.Format(_T("Forward-Line : %d =>\t The selected 'DIR' column contains a value other than 0 or 1."), (i + 1));
					}
                    if (a_bErrFile) {
                        ofErr.Write(strErrorMsg);
                    }
                    bErrLine = true;
				}

				bOffset = true;
			}

			//필수키
			Integer nxLinkID(0);
			Integer nxFnodeID(0);
			Integer nxTnodeID(0);
			int     nType(0);
			CString strLinkID(_T(""));
			CString strFNodeID(_T(""));
			CString strTNodeID(_T(""));
			CString strType(_T(""));

			TxPolyline oPolyLine(oBinary, 0);
			TxRecord oRecord;
			//테이블 정보 저장
			for (size_t k= 0; k< m_vecForShapeMatting.size(); k++)
			{
				KImportShapeMatching oMatchingInfo = m_vecForShapeMatting[k];
				KIOColumn*           pColumn       = oMatchingInfo.pMatColumn;
				CString              strColumnName = pColumn->Name();

				const DbfFieldValue &oDbfFieldValue = vecField[oMatchingInfo.nShapeIndex];
				CString              strValue       = oDbfFieldValue.m_strValue;
				strValue.Trim();

				if (pColumn->DataType() == KEMIODataTypeInteger) {
					Integer nxValue = _ttoi64(strValue);

					if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0) {
						nxLinkID = nxValue;
                        strLinkID = strValue;
					} else if (strColumnName.CompareNoCase(COLUMN_FNODE_ID) == 0) {
						nxFnodeID = nxValue;
						strFNodeID = strValue;
					} else if (strColumnName.CompareNoCase(COLUMN_TNODE_ID) == 0) {
						nxTnodeID = nxValue;
						strTNodeID = strValue;
					} else if (strColumnName.CompareNoCase(COLUMN_LINK_TYPE) == 0) {
						nType = (int)nxValue;
						strType = strValue;
					}

					oRecord.AddItem(strColumnName, nxValue);	
				} else if (pColumn->DataType() == KEMIODataTypeDouble) {
					double dValue   = _ttof(strValue);

					oRecord.AddItem(strColumnName, dValue);	
				} else {
					oRecord.AddItem(strColumnName, strValue);
				}

				if (strColumnName.CompareNoCase(COLUMN_LINK_LENGTH) == 0) {
					bUseForwardLength = true;
				}
// 				if (m_bUseForwardLength == false)
// 				{
// 					double dLength(0);
// 					dLength = oPolyLine.Length();
// 					oRecord.AddItem(COLUMN_LINK_LENGTH, dLength);	
// 				}
			}

			if (bCustomID)
			{
				nxLinkID = nxCustomLinkID;
				strLinkID.Format(_T("%I64d"), nxLinkID);

				oRecord.AddItem(COLUMN_LINK_ID, nxLinkID);	

                nxCustomLinkID++;
			}

			if (strLinkID.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Forward-Line : %d =>\t 링크 ID가 존재 하지 않습니다."), (i + 1));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t 링크 ID가 존재 하지 않습니다."), (i + 1));
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID does NOT exist."), (i + 1));
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
			}
			if (strFNodeID.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 Fnode 데이터가 없습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 F_Node 데이터가 없습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t The Link %s does NOT have FromNode data."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
			}
			if (strTNodeID.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 Tnode 데이터가 없습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 T_Node 데이터가 없습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t The Link %s does NOT have ToNode data."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
            }
			if (strType.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 Type 데이터가 없습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 Type 데이터가 없습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t The Link %s does NOT have Type data."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
			}

			auto itFind = m_mapNodeRecord.find(nxFnodeID);
			if (itFind == m_mapNodeRecord.end()) {
                //strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 Fnode가 노드 테이블에 등록되어 있지 않습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 F_Node가 노드 테이블에 입력되어 있지 않습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t The Link %s's FromNode does NOT exist in Node Table."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
			}

			itFind = m_mapNodeRecord.find(nxTnodeID);
			if (itFind == m_mapNodeRecord.end()) {
                //strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 Tnode가 노드 테이블에 등록되어 있지 않습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 T_Node가 노드 테이블에 입력되어 있지 않습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t The Link %s's ToNode does NOT exist in Node Table."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
			}

			if (nxFnodeID == nxTnodeID)	{
                if (strFNodeID.CompareNoCase(_T("")) != 0) { // 노드ID가 비었는데 비교할 필요도 없다.
                    //strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 Tnode와 Fnode의 데이터가 같습니다."), (i + 1), strLinkID);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strErrorMsg.Format(_T("Forward-Line : %d =>\t Link ID: %s 의 F_Node와 T_Node의 값이 동일합니다."), (i + 1), strLinkID);
					}
					else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
						strErrorMsg.Format(_T("Forward-Line : %d =>\t The Link %s's FromNode ID is same as ToNode ID."), (i + 1), strLinkID);
					}
                    if (a_bErrFile) {
                        ofErr.Write(strErrorMsg);
                    }

                    bErrLine = true;
                }
			}
 			
			if (IsFlipVertex(nxLinkID, nxFnodeID, oPolyLine)) //Flip 대상 여부 체크
			{
				oPolyLine.Flip();
				oBinary.Reset();
				oPolyLine.GetBinary(oBinary);
			}

			ITxGeometryPtr spGeometry = nullptr;
            /* 2017.11.28 - 명지대 요구사항으로 Shape Import 시 offset을 주지 않는다. (그리기 설정에서 '양선 표시' 옵션을 사용하도록 유도)
			if (bOffset) {
				std::vector<TxPoint> vecOffsetPoint = oPolyLine.GetOffsetPoint(m_dOneWayOffset);
				spGeometry = TxPolylinePtr(new TxPolyline(vecOffsetPoint));
			} else {
				spGeometry = TxPolylinePtr(new TxPolyline(oPolyLine));
			}
            */
            spGeometry = TxPolylinePtr(new TxPolyline(oPolyLine));

			m_spFeatureLayer->Insert(nxLinkID, spGeometry, oRecord);

            a_nInsertCnt++;
            auto itFindLinkType = m_mapReadLinkType.find(nType);
            if (itFindLinkType != m_mapReadLinkType.end()) {
                int &nTempCount = itFindLinkType->second;
                nTempCount++;
            }
            else {
                m_mapReadLinkType.insert(std::make_pair(nType, 1));
            }

            if (bErrLine) {
                a_setFwrFailedEntity.insert(i);
            }
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		//ThrowException(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		throw ex;
	} catch (...) {
		TxLogDebugException();
		//ThrowException(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
	}
}

void KDlgShapeLink::SaveReverseFeatureData(ThreadPara* pPara, ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr, int &a_nInsertCnt, std::set<int> &a_setRvrFailedEntity)
{
	bool    bUseReverseLength(false);
	CString strErrorMsg(_T(""));

    Integer &nxCustomLinkID = pPara->TKeyInt[0];

    BOOL    bCustomID       = (BOOL)pPara->TKeyInt32[1];
    int     nOneWayIndex    = pPara->TKeyInt32[2];

	try
	{
		const int nShapeType    = a_oShapeFile.ShapeType();
		const int nEntityCount  = a_oShapeFile.EntityCount();
		int       nSuccessCnt(0);
		for (int i= 0; i< nEntityCount; i++)
		{
            bool bErrLine(false);

			if (i % 1000 == 0 || i == nEntityCount-1) {
				CString strMsg(_T(""));
				strMsg.Format(_T("#2 Read Reverse Shape data ( %d / %d )"), (i+1), nEntityCount);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMsg.Format(_T("#2 (역방향) 읽은 파일 라인수 / 총 라인수 :  %d / %d"), (i+1), nEntityCount);
				}
				AddStatusMessage( strMsg );
			}
			QBicBinary oBinary(20);
			a_oShapeFile.ReadShape(i, oBinary);

			int nGeometryType(0);
			oBinary.Position(0);
			oBinary.Read(nGeometryType);

            if (KEMGeoPolyLine != nGeometryType) {

                /* Forward에서 기록되었기 때문에 여기서 기록은 하지 않는다.
                strErrorMsg.Format(_T("Reverse-Line : %d =>\t None PolyLine"), (i + 1));
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
                a_setRvrFailedEntity.insert(i); // 여기는 특수한 continue 니깐 추가 해줘야지
                */

                continue;
            }

			std::vector<DbfFieldValue> vecField = a_oShapeFile.ReadRecord(i);

			//OneWay Field 확인
			bool bOffset(false);
			if (nOneWayIndex != -1)
			{
				CString strValue = vecField[nOneWayIndex].m_strValue;
				int nOneWayType = _ttoi(strValue);

				if ( 1 == nOneWayType)
				{
					continue;
				}
				else if ( 0 != nOneWayType && -1 != nOneWayType)
				{
                    /* Forward에서 기록되었기 때문에 여기서 기록은 하지 않는다.
                    strErrorMsg.Format(_T("Reverse-Line : %d =>\t 선택한 OneWay 필드에 0 또는 1 이외의 값이 들어가 있습니다."), (i + 1));
                    if (a_bErrFile) {
                        ofErr.Write(strErrorMsg);
                    }
                    bErrLine = true;
                    */
				}

				bOffset = true;
			}

			//필수키
			Integer nxLinkID(0);
			Integer nxFnodeID(0);
			Integer nxTnodeID(0);
			int     nType(0);
			CString strLinkID(_T(""));
			CString strFNodeID(_T(""));
			CString strTNodeID(_T(""));
			CString strType(_T(""));

			TxPolyline oPolyLine(oBinary, 0);
			TxRecord oRecord;
			//테이블 정보 저장
			for (size_t k= 0; k < m_vecRevShapeMatting.size(); k++)
			{
				KImportShapeMatching oMatchingInfo = m_vecRevShapeMatting[k];
				KIOColumn*           pColumn       = oMatchingInfo.pMatColumn;
				CString              strColumnName = pColumn->Name();

				const DbfFieldValue &oDbfFieldValue = vecField[oMatchingInfo.nShapeIndex];
				CString              strValue       = oDbfFieldValue.m_strValue;
				strValue.Trim();

				if (pColumn->DataType() == KEMIODataTypeInteger) {
					Integer nxValue = _ttoi64(strValue);

					if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0) {
						nxLinkID = nxValue;
                        strLinkID = strValue;
					} else if (strColumnName.CompareNoCase(COLUMN_FNODE_ID) == 0) {
						nxFnodeID = nxValue;
						strFNodeID = strValue;
					} else if (strColumnName.CompareNoCase(COLUMN_TNODE_ID) == 0) {
						nxTnodeID = nxValue;
						strTNodeID = strValue;
					} else if (strColumnName.CompareNoCase(COLUMN_LINK_TYPE) == 0) {
						nType = (int)nxValue;
						strType = strValue;
					}

					oRecord.AddItem(strColumnName, nxValue);	
				} else if (pColumn->DataType() == KEMIODataTypeDouble) {
					double dValue   = _ttof(strValue);

					oRecord.AddItem(strColumnName, dValue);	
				} else {
					oRecord.AddItem(strColumnName, strValue);
				}

				if (strColumnName.CompareNoCase(COLUMN_LINK_LENGTH) == 0) {
					bUseReverseLength = true;
				}
// 				if (bUseReverseLength == false)
// 				{
// 					double dLength(0);
// 					dLength = oPolyLine.Length();
// 					oRecord.AddItem(COLUMN_LINK_LENGTH, dLength);	
// 				}
			}

			if (bCustomID)
			{
				nxLinkID = nxCustomLinkID;
				strLinkID.Format(_T("%I64d"), nxLinkID);

				oRecord.AddItem(COLUMN_LINK_ID, nxLinkID);	

                nxCustomLinkID++;
			}

			if (strFNodeID.CompareNoCase(_T("")) == 0 && strTNodeID.CompareNoCase(_T("")) == 0) {
				continue;
			}
            
            if (strLinkID.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Reverse-Line : %d =>\t 링크ID가 존재 하지 않습니다."), (i + 1));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t 링크 ID가 존재 하지 않습니다."), (i + 1));
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID does NOT exist."), (i + 1));
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }

                bErrLine = true;
            }
            if (strFNodeID.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 Fnode 데이터가 없습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 F_Node 데이터가 없습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t The Link %s does NOT have FromNode data."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }

                bErrLine = true;
			} else if (strTNodeID.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 Tnode 데이터가 없습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 T_Node 데이터가 없습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t The Link %s does NOT have ToNode data."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }

                bErrLine = true;
			}

			if (strType.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 Type 데이터가 없습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 Type 데이터가 없습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t The Link %s does NOT have Type data."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }

                bErrLine = true;
			}

			auto itFind = m_mapNodeRecord.find(nxFnodeID);
			if (itFind == m_mapNodeRecord.end()) {
                //strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 Fnode가 노드 테이블에 등록되어 있지 않습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 F_Node가 노드 테이블에 입력되어 있지 않습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t The Link %s's FromNode does NOT exist in Node Table."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }

                bErrLine = true;
			}

			itFind = m_mapNodeRecord.find(nxTnodeID);
			if (itFind == m_mapNodeRecord.end()) {
                //strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 Tnode가 노드 테이블에 등록되어 있지 않습니다."), (i + 1), strLinkID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 T_Node가 노드 테이블에 입력되어 있지 않습니다."), (i + 1), strLinkID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Reverse-Line : %d =>\t The Link %s's ToNode does NOT exist in Node Table."), (i + 1), strLinkID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }

                bErrLine = true;
			}

			if (nxFnodeID == nxTnodeID)	{
                if (strFNodeID.CompareNoCase(_T("")) != 0) { // 노드ID가 비었는데 비교할 필요도 없다.
                    //strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 Tnode와 Fnode의 데이터가 같습니다."), (i + 1), strLinkID);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strErrorMsg.Format(_T("Reverse-Line : %d =>\t Link ID: %s 의 F_Node와 T_Node의 값이 동일합니다."), (i + 1), strLinkID);
					}
					else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
						strErrorMsg.Format(_T("Reverse-Line : %d =>\t The Link %s's FromNode ID is same as ToNode ID."), (i + 1), strLinkID);
					}
                    if (a_bErrFile) {
                        ofErr.Write(strErrorMsg);
                    }

                    bErrLine = true;
                }
			}

			if (IsFlipVertex(nxLinkID, nxFnodeID, oPolyLine)) //Flip 대상 여부 체크
			{
				oPolyLine.Flip();
				oBinary.Reset();
				oPolyLine.GetBinary(oBinary);
			}

			ITxGeometryPtr spGeometry = nullptr;
			
            /* 2017.11.28 - 명지대 요구사항으로 Shape Import 시 offset을 주지 않는다. (그리기 설정에서 '양선 표시' 옵션을 사용하도록 유도)
            if (bOffset) {
				std::vector<TxPoint> vecOffsetPoint = oPolyLine.GetOffsetPoint(m_dOneWayOffset);
				spGeometry = TxPolylinePtr(new TxPolyline(vecOffsetPoint));
			} else {
				spGeometry = TxPolylinePtr(new TxPolyline(oPolyLine));
			}
            */
            spGeometry = TxPolylinePtr(new TxPolyline(oPolyLine));

			m_spFeatureLayer->Insert(nxLinkID, spGeometry, oRecord);
            
            a_nInsertCnt++;
            auto itFindLinkType = m_mapReadLinkType.find(nType);
            if (itFindLinkType != m_mapReadLinkType.end()) {
                int &nTempCount = itFindLinkType->second;
                nTempCount++;
            }
            else {
                m_mapReadLinkType.insert(std::make_pair(nType, 1));
            }

            if (bErrLine) {
                a_setRvrFailedEntity.insert(i);
            }
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);

		//ThrowException(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);

		throw ex;
	} catch (...) {
		TxLogDebugException();

		//ThrowException(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
	}
}

bool KDlgShapeLink::IsFlipVertex(Integer a_nxLinkID, Integer a_nxFNodeID, TxPolyline &a_oPolyLine)
{
	try
	{
		std::vector<TxPoint> vecVertex = a_oPolyLine.GetPoints();
		int nVertexCnt = vecVertex.size();
		if (nVertexCnt < 1)
			return false;

		TxPoint& oPointFirst = vecVertex[0];             // first point
		TxPoint& oPointLast  = vecVertex[nVertexCnt -1]; // last point

		AutoType itFind = m_mapNodeRecord.find(a_nxFNodeID);
		if (itFind == m_mapNodeRecord.end())
			return false;

		TxPoint& oFNodeInfo = itFind->second;

		const double &dX_FNode = oFNodeInfo.X;
		const double &dY_FNode = oFNodeInfo.Y;

		if (dX_FNode == oPointFirst.X && dY_FNode == oPointFirst.Y)
			return false; // 완벽히 일치 : flip 대상이 아니다

		double dFirstPointDist = QBicGeometry::Length(dX_FNode, dY_FNode, oPointFirst.X, oPointFirst.Y);
		double dLastPointDist  = QBicGeometry::Length(dX_FNode, dY_FNode, oPointLast.X, oPointLast.Y);

		if (dFirstPointDist <= dLastPointDist) // First와 가깝다면 Flip 대상이 아냐
		{
// 			CString strDebug(_T(""));
// 			strDebug.Format(_T(" none flip ----- %I64d "), a_nxLinkID);
// 			TxLogDebug((LPCTSTR)strDebug);
			return false;
		}
		else // Last와 가깝다면 Flip 대상
		{
// 			CString strDebug(_T(""));
// 			strDebug.Format(_T(" flip ----- %I64d "), a_nxLinkID);
// 			TxLogDebug((LPCTSTR)strDebug);
			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return false;
}

void KDlgShapeLink::ResizeComponent()
{
	SetResize(IDC_STATIC_FILE,          SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BROWSE,          SZ_TOP_LEFT,      SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_HOR1,          SZ_TOP_LEFT,      SZ_TOP_RIGHT);

	SetResize(IDC_RADIO1,				SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_RADIO2,				SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_STATIC_ONE,           SZ_TOP_RIGHT,      SZ_TOP_RIGHT);
	SetResize(IDC_COMBO1,				SZ_TOP_RIGHT,      SZ_TOP_RIGHT);

	SetResize(IDC_CHECK_ID,             SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_EDIT_ID,				SZ_TOP_LEFT,      SZ_TOP_LEFT);
	//SetResize(IDC_STATIC_CODE,          SZ_TOP_RIGHT,      SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_FOR,           SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_REPORT_FOR,           SZ_TOP_LEFT,      CXTPResizePoint(0.4, 0.5));

	SetResize(IDC_REPORT_DRAG,          CXTPResizePoint(0.4, 0),      CXTPResizePoint(0.6, 0.5));

	SetResize(IDC_STATIC_REV,           CXTPResizePoint(0.6, 0),      CXTPResizePoint(0.6, 0));
	SetResize(IDC_REPORT_REV,           CXTPResizePoint(0.6, 0),      CXTPResizePoint(1, 0.5));

	SetResize(IDC_STATIC_HOR2,             CXTPResizePoint(0, 0.5),      CXTPResizePoint(1, 0.5));

	SetResize(IDC_STATIC_PREVIEW,          CXTPResizePoint(0, 0.5),      CXTPResizePoint(0, 0.5));
	SetResize(IDC_REPORT_PRE,              CXTPResizePoint(0, 0.5),      SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_HOR3,             SZ_BOTTOM_LEFT,				SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_ENC,          SZ_BOTTOM_LEFT,       SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO4,              SZ_BOTTOM_LEFT,       SZ_BOTTOM_LEFT);

	SetResize(IDOK,                   SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,               SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}

void KDlgShapeLink::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow) {
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KDlgShapeLink::ErrorFileOpen()
{
	try
	{
		if (QBicFile::FileExist(m_strErrFile) == false){
			return;
		}
		ShellExecute(NULL, _T("open"), m_strErrFile, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
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

int KDlgShapeLink::GetSelectedEncoding()
{
	int nEncoding; {
		if (m_cboEncoding.GetCurSel() == 1)
			nEncoding = KEMCSVEncodingUTF8;
		else 
			nEncoding = KEMCSVEncodingANSI;
	}

	return nEncoding;
}

void KDlgShapeLink::OnCbnSelchangeCombo4()
{
	UpdateReportPreview();
}
