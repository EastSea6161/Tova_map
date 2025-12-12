// DlgShapeDemarcation.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "DlgShapeDemarcation.h"
#include "afxdialogex.h"

#include "Target.h"
#include "KExRecordItem.h"
#include "ImChampFrameWindow.h"
// #include "ImBicManDefineCodeManage.h"
// #include "../QBicDataStructure/QBicGeometry.h"
// 
// #include "MapView.h"

#include "../QBicCommon/QBicTxtWriter.h"

#include "../QBicGIS/TxPoint.h"
#include "../QBicGIS/TxPolyline.h"
#include "../QBicGIS/TxPolygon.h"
#include "../QBicGIS/TxGridTileIndex.h"

// KDlgShapeDemarcation 대화 상자입니다.

namespace ShapeDemarcationENUM
{
	enum
	{
		_0_COLUMN_FIELDNAME = 0,
		_1_COLUMN_FIELDTYPE,
		_2_COLUMN_FIELDPK,
		_3_COLUMN_IMPORT_FIELDNAME
	};

	enum KEMGeoType
	{
		KEMGeoNull     = 0,
		KEMGeoPoint    = 1,
		KEMGeoPolyLine = 3,
		KEMGeoPolyGon  = 5
	};
}

using namespace ShapeDemarcationENUM;

IMPLEMENT_DYNAMIC(KDlgShapeDemarcation, KWhiteDlgEx)

KDlgShapeDemarcation::KDlgShapeDemarcation(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KWhiteDlgEx(KDlgShapeDemarcation::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pTable(a_pTable)
	, m_nGeoType(KEMGeoNull)
	, m_nShapeIndexOfID(-1)
	, m_strKeyFieldName(_T(""))
{
	CString strTargetLocation = m_pTarget->GetLocation();

	m_strErrFile.Format(_T("%s\\%s"), strTargetLocation, IMPORT_ERROR_FILE_NAME);
}

KDlgShapeDemarcation::~KDlgShapeDemarcation()
{
}

void KDlgShapeDemarcation::DoDataExchange(CDataExchange* pDX)
{
	KWhiteDlgEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BROWSE,	 m_editShapeFile);
	DDX_Control(pDX, IDC_REPORT_DEFINE,  m_ctrlReportDefine);
	DDX_Control(pDX, IDC_REPORT_COLUMN,  m_ctrlReportDrag);
	DDX_Control(pDX, IDC_REPORT_PREVIEW, m_ctrlReportPreview);
	DDX_Control(pDX, IDC_COMBO4, m_cboEncoding);
}


BEGIN_MESSAGE_MAP(KDlgShapeDemarcation, KWhiteDlgEx)
	ON_EN_CHANGE(IDC_EDIT_BROWSE, &KDlgShapeDemarcation::OnEnChangeEditBrowse)
	ON_NOTIFY(XTP_NM_REPORT_RECORDS_DROPPED,      IDC_REPORT_DEFINE,  &KDlgShapeDemarcation::OnReportRecordsDropped)
	ON_NOTIFY(XTP_NM_REPORT_DROP,                 IDC_REPORT_COLUMN,  &KDlgShapeDemarcation::OnReportRecordsDroping)

	ON_BN_CLICKED(IDC_BUTTON1, &KDlgShapeDemarcation::OnBnClickedButtonRun)
	ON_CBN_SELCHANGE(IDC_COMBO4, &KDlgShapeDemarcation::OnCbnSelchangeCombo4)
END_MESSAGE_MAP()


// KDlgShapeDemarcation 메시지 처리기입니다.
void KDlgShapeDemarcation::SetResizeComponent()
{
	try
	{
		SetResize(IDC_EDIT_BROWSE,      SZ_TOP_LEFT,          SZ_TOP_RIGHT);
		SetResize(IDC_STATIC6,          SZ_TOP_LEFT,          SZ_TOP_RIGHT);

		SetResize(IDC_REPORT_DEFINE,             SZ_TOP_LEFT,         CXTPResizePoint(1.0, 0.4f));
		SetResize(IDC_REPORT_COLUMN,             SZ_TOP_RIGHT,        CXTPResizePoint(1, 0.4f));
		SetResize(IDC_STATIC4,          CXTPResizePoint(0, 0.4f),      CXTPResizePoint(1, 0.4f));

		SetResize(IDC_STATIC15,          CXTPResizePoint(0, 0.4f),      CXTPResizePoint(0, 0.4f));
		SetResize(IDC_REPORT_PREVIEW,   CXTPResizePoint(0, 0.4f),      SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC_ENC,          SZ_BOTTOM_LEFT,       SZ_BOTTOM_LEFT);
		SetResize(IDC_COMBO4,          SZ_BOTTOM_LEFT,       SZ_BOTTOM_LEFT);

		SetResize(IDC_STATIC11,          SZ_BOTTOM_LEFT,       SZ_BOTTOM_RIGHT);
		SetResize(IDC_BUTTON1,          SZ_BOTTOM_RIGHT,      SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,             SZ_BOTTOM_RIGHT,      SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KDlgShapeDemarcation::OnInitDialog()
{
	KWhiteDlgEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		InitControl();

		QBicReportCtrlSetting::Default(m_ctrlReportDefine, TRUE);
		QBicReportCtrlSetting::Default(m_ctrlReportDrag, TRUE);
		QBicReportCtrlSetting::Default(m_ctrlReportPreview, FALSE, FALSE, FALSE);

		m_ctrlReportDefine.EnableDragDrop(_T("Drop"), xtpReportAllowDrop, xtpReportDropSelect);
		m_ctrlReportDrag.EnableDragDrop  (_T("Drop"), xtpReportAllowDragCopy | xtpReportAllowDrop, xtpReportDropSelect);

		InitReportHeaderDefine();
		InitReportHeaderDrag();

		m_ctrlReportDefine.Populate();
		m_ctrlReportDrag.Populate();

		SetResizeComponent();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDlgShapeDemarcation::InitControl()
{
	TCHAR szFilter[] = _T("Shp Files (*.shp)|*.shp|");
	m_editShapeFile.Initialize( this, BES_XT_CHOOSEFILE );
	m_editShapeFile.SetDlgOpenFile( TRUE );
	m_editShapeFile.SetReadOnly( TRUE );
	m_editShapeFile.SetDlgTitle( _T("Select Shape File") );
	m_editShapeFile.SetDlgFilter( szFilter );

	InitComboEncoding();
}

void KDlgShapeDemarcation::InitComboEncoding()
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

void KDlgShapeDemarcation::InitReportHeaderDefine()
{
	try
	{
		CXTPReportColumn* pColumn = NULL;

		pColumn = m_ctrlReportDefine.AddColumn( new CXTPReportColumn( _0_COLUMN_FIELDNAME, _T("System Column"), 45 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("시스템 컬럼"));
		}

		pColumn = m_ctrlReportDefine.AddColumn( new CXTPReportColumn( _1_COLUMN_FIELDTYPE, _T("Type"), 60, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("데이터 유형"));
		}

		pColumn = m_ctrlReportDefine.AddColumn( new CXTPReportColumn( _2_COLUMN_FIELDPK, _T("PK"), 40, FALSE ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		pColumn->SetVisible(FALSE);

		pColumn = m_ctrlReportDefine.AddColumn( new CXTPReportColumn( _3_COLUMN_IMPORT_FIELDNAME, _T("File Column"), 55 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
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


void KDlgShapeDemarcation::InitReportHeaderDrag()
{
	try
	{
		CXTPReportColumn* pColumn = m_ctrlReportDrag.AddColumn( new CXTPReportColumn( 0, _T("Drag & Drop"), 100 ) );
		pColumn->SetHeaderAlignment(DT_CENTER);
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


void KDlgShapeDemarcation::OnEnChangeEditBrowse()
{
	RefreshInfo();

	if (!LoadPreViewShapeField()) {
		RefreshInfo();
		return;
	} else {
		if (KEMGeoPolyGon != m_nGeoType && KEMGeoPolyGon-1 != m_nGeoType) {
			RefreshInfo();
			//AfxMessageBox(_T("선택한 Shape 파일(Polygon)이 유효하지 않습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("선택한 Shape 파일(Polygon)이 유효하지 않습니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("The shape file you selected(Polygon) is not valid."));
			}
			return;
		}
	}

	UpdateReportDataDefine();
	UpdateReportDataDrag();
	UpdateReportDataPreview();
}


void KDlgShapeDemarcation::RefreshInfo()
{
	m_mapShapeField.clear();
	m_nGeoType = KEMGeoNull;

	m_ctrlReportDefine.ResetContent();
	m_ctrlReportDrag.ResetContent();

	m_ctrlReportPreview.ResetContent();
	m_ctrlReportPreview.GetColumns()->Clear();
	m_ctrlReportPreview.Populate();
}


bool KDlgShapeDemarcation::LoadPreViewShapeField()
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


void KDlgShapeDemarcation::OpenShapeFile(CString a_strShapeFile, ShapeFileReader &a_oShapeFile)
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
			ThrowException(_T("Failed to open file"));  

		int nEntityCount = a_oShapeFile.EntityCount();
		int nRecordCount = a_oShapeFile.RecordCount();
		int nFieldCount  = a_oShapeFile.FieldCount();

		if (nEntityCount != nRecordCount) {
			//ThrowException(_T("Shape 정보와 속성(dbf) 정보의 개수가 일치하지 않습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("Shape 정보와 속성(dbf) 정보의 개수가 일치하지 않습니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("The number of shape and attribute (dbf) information does not match."));
			}
		}

		if (0 == nEntityCount) {
			//ThrowException(_T("Shape 정보와 개수가 0개 입니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("Shape 정보와 개수가 0개 입니다."));
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("There is no any shape information in data file."));
			}
		}
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


void KDlgShapeDemarcation::UpdateReportDataDefine()
{
	m_ctrlReportDefine.ResetContent();

	try
	{
		CXTPReportRecord*            pRecord	= nullptr;
		CXTPReportRecordItem*        pItem		= nullptr;
		CXTPReportRecordItemComboEx* pItemCombo = nullptr;

		KIOColumn*                   pIOColumn  = nullptr;
		const KIOColumns*            pIOColumns = m_pTable->Columns();
		int nIOColumnCnt           = pIOColumns->ColumnCount();

		for (int i= 0; i< nIOColumnCnt; i++)
		{
			pIOColumn = pIOColumns->GetColumn(i);

			pRecord   = m_ctrlReportDefine.AddRecord(new CXTPReportRecord);

			bool bPK = pIOColumn->PrimaryKey();

			// 0.field
			//CString strFieldName = pIOColumn->Caption();
			CString strFieldName     = pIOColumn->Name();
			CString	strColumnCaption = pIOColumn->Caption();

			if (bPK) {
				CString strCaptionPK(_T(""));
				strCaptionPK.Format(_T("* %s"), strColumnCaption);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strCaptionPK));
			}
			else {
				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strColumnCaption));
			}
			pItem->SetItemData((DWORD_PTR)pIOColumn);
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			if (bPK) {
				pItem->SetTextColor(RGB(255,0,0));
			}

			// 1.type
			CString strDataType(_T(""));
			switch(pIOColumn->DataType())
			{
			case KEMIODataTypeInteger : strDataType = DATATYPESTRING_INTEGER; break;
			case KEMIODataTypeDouble  : strDataType = DATATYPESTRING_DOUBLE;  break;
			case KEMIODataTypeString  : strDataType = DATATYPESTRING_STRING;  break;
			default: break;
			}

			pItem     = pRecord->AddItem(new CXTPReportRecordItemText(strDataType));
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			// 2.PK
			CString strPK = _T("N");
			if (true == bPK) {
				strPK = _T("Y");
			}

			pItem     = pRecord->AddItem(new CXTPReportRecordItemText(strPK));
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			// 3.Import Field
			pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx);
			pItemCombo->AddString(_T("Null"), -1);

			int nInitIdx(0);
			int nIndex(0);

			auto iter  = m_mapShapeField.begin();
			auto itEnd = m_mapShapeField.end();
			for (; iter != itEnd; ++iter)
			{
				int     nShapeFieldIdx    = (int)iter->first;
				CString strShapeFieldName = iter->second;

				nIndex = pItemCombo->AddString(strShapeFieldName, (DWORD_PTR)nShapeFieldIdx);

				if (strFieldName.CompareNoCase(strShapeFieldName) == 0 || strColumnCaption.CompareNoCase(strShapeFieldName) == 0)
					nInitIdx = nIndex;
			}

			pItemCombo->SetCurSel(nInitIdx);
			pItemCombo->SetAlignment(DT_CENTER);
			pItemCombo->SetEditable(TRUE);
		}

		m_ctrlReportDefine.FocusSubItems(TRUE);
		m_ctrlReportDefine.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDlgShapeDemarcation::UpdateReportDataDrag()
{
	m_ctrlReportDrag.ResetContent();

	try
	{
		CXTPReportRecord*     pRecord	= nullptr;
		CXTPReportRecordItem* pItem		= nullptr;

#pragma region 맨처음 Null 정보 삽입
		pRecord = m_ctrlReportDrag.AddRecord(new CXTPReportRecord);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Null")));
		pItem->SetItemData((DWORD_PTR) -1);
		pItem->SetAlignment(DT_CENTER);
		//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
#pragma endregion 맨처음 Null 정보 삽입

		auto iter  = m_mapShapeField.begin();
		auto itEnd = m_mapShapeField.end();

		for (; iter != itEnd; ++iter)
		{
			int		nShapeFieldIndex	= (int)iter->first;
			CString	strColumnCaption	= iter->second;

			pRecord	= m_ctrlReportDrag.AddRecord(new CXTPReportRecord);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strColumnCaption));
			pItem->SetItemData((DWORD_PTR)nShapeFieldIndex);
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}

		m_ctrlReportDrag.Populate();
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


void KDlgShapeDemarcation::UpdateReportDataPreview()
{
	m_ctrlReportPreview.ResetContent();
	m_ctrlReportPreview.GetColumns()->Clear();
	m_ctrlReportPreview.Populate();

	const int nShapeFieldCnt = (int)m_mapShapeField.size();
	if (nShapeFieldCnt < 1)
		return;

	bool bAutoSize(false); {
		if (nShapeFieldCnt < 10)
			bAutoSize = true;
	}

	if (!bAutoSize) {
		QBicReportCtrlSetting::AutoColumnSizeFalse(m_ctrlReportPreview);
	} else {
		m_ctrlReportPreview.GetReportHeader()->SetAutoColumnSizing(TRUE);
	}

#pragma region InitReportHeaderPreview
	CXTPReportColumn* pColumn = nullptr;
	int nIndex(0);

	auto iter  = m_mapShapeField.begin();
	auto itEnd = m_mapShapeField.end();

	pColumn = m_ctrlReportPreview.AddColumn(new CXTPReportColumn (nIndex++, _T("No."), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	pColumn->SetEditable(FALSE);
	if (!bAutoSize)
		pColumn->SetBestFitMode(xtpColumnBestFitModeAllData);

	for (; iter != itEnd; ++iter, nIndex++)
	{
		pColumn = m_ctrlReportPreview.AddColumn(new CXTPReportColumn (nIndex, iter->second, 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		if (!bAutoSize)
			pColumn->SetBestFitMode(xtpColumnBestFitModeAllData);
	}

	m_ctrlReportPreview.Populate();

	CXTPReportHeader*  pHeader        = m_ctrlReportPreview.GetReportHeader();
	CXTPReportColumns* pReportColumns = m_ctrlReportPreview.GetColumns();
	int                nReportColumns = pReportColumns->GetCount();
	for (int i= 0; i< nReportColumns; i++) {
		CXTPReportColumn* pReportColumn = pReportColumns->GetAt(i);
		if (!bAutoSize)
			pHeader->BestFit(pReportColumn);
	}
#pragma endregion InitReportHeaderPreview

	try
	{
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

			pRecord = m_ctrlReportPreview.AddRecord(new CXTPReportRecord());

			// No. 필드(참고를 위한) 
			strGridSeq.Format(_T("%d"), nGridSeq++);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strGridSeq));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);

			for (int k= 0; k< nSizeValue; k++)
			{
				const DbfFieldValue &oDbfFieldValue = vecFieldValue[k];

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oDbfFieldValue.m_strValue));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
			}
		}

		m_ctrlReportPreview.Populate();
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


void KDlgShapeDemarcation::OnReportRecordsDropped( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);

	try
	{
		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		int nCount       = pRecords->GetCount();
		if (nCount < 1)
			return;

		CXTPReportRecord*  pDropRecord    = pRecords->GetAt(0);
		CXTPReportRecords* pTargetRecords = m_ctrlReportDefine.GetRecords();

		if (nullptr != pDropRecord)
		{
			int                   nRowIndex = pDropRecord->GetIndex(); // will be Drop Row's Index
			CXTPReportRecordItem* pItem2    = pDropRecord->GetItem(0);
			int					  nItemData = (int)pItem2->GetItemData();

			pTargetRecords->RemoveRecord(pDropRecord);
			m_ctrlReportDefine.Populate();

			pTargetRecords = m_ctrlReportDefine.GetRecords();
			CXTPReportRecord* pTargetRecord = pTargetRecords->GetAt(nRowIndex); // pItemNotify->pTargetRecord 사용 불가

			if (nullptr != pTargetRecord)
			{
				//KColumnReportRecordItem* pItem = (KColumnReportRecordItem*)pTargetRecord->GetItem(3);
				CXTPReportRecordItemComboEx*     pItemCombo = (CXTPReportRecordItemComboEx*)pTargetRecord->GetItem(3);
				CXTPReportRecordItemEditOptions* pOption    = pItemCombo->GetEditOptions(NULL);
				CXTPReportRecordItemConstraint*  pCons      = pOption->FindConstraint((DWORD_PTR)nItemData);
				int nIndex = pCons->GetIndex();
				pItemCombo->SetCurSel(nIndex);

				CXTPReportRow* pRow = m_ctrlReportDefine.GetRows()->Find(pTargetRecord);
				m_ctrlReportDefine.SetFocusedRow(pRow);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDlgShapeDemarcation::OnReportRecordsDroping( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	try
	{
		XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);

		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		int nCount       = pRecords->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			m_ctrlReportDrag.RemoveRecordEx(pRecord);
		}

		m_ctrlReportDrag.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDlgShapeDemarcation::OnBnClickedButtonRun()
{
	UpdateData(TRUE);

	if (!InvalidateInputInfo())
		return;

	CString strShapeFile(_T(""));
	m_editShapeFile.GetWindowText(strShapeFile);

	try
	{
		ThreadPara oPara(this);
		if (true) {
			//oPara.TKeyInt[0] = 0;      // 레코드 개수 저장
			oPara.TKeyStr[0] = strShapeFile;
			RThreadInfo.Init();

			// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
			int nLang = 1;
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				nLang = 0;
			}

			QBicSimpleProgressThread::ExecuteThread(ThreadRun, &oPara, false, nLang); 

			if (RThreadInfo.IsOK() == false) {
				ThrowException(RThreadInfo.ErrorMsg());
			} else {
				int nUID = -1;
				if (KmzSystem::GetLanguage() == KEMKorea) {
					nUID = AfxMessageBox(_T("경계정보를 생성하였습니다.\n새 경계 정보를 이용하여 'Zone_ID' 컬럼을 업데이트 하시겠습니까?"), MB_OKCANCEL);
				}
				else
				{
					nUID = AfxMessageBox(_T("The Zone Demarcation has been created.\nDo you want to update the 'Zone_ID' column with the new Demarcation?"), MB_OKCANCEL);
				}
				if (IDOK == nUID) {
					SpatialCalculateZoneID(this, m_pTarget);
				} 
				// Cancel 이라면 갱신 하지 않고 다음으로 진행

				KIOTable* pTableNode = m_pTarget->Tables()->FindTable(TABLE_NODE);
				pTableNode->Notify(); // zone
				m_pTable->Notify(); 

				KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(m_pTarget);
				if (nullptr != pCreatedMapView) {
					pCreatedMapView->FullExtentMap();
				}
			}
		}
	} catch (int& ex) {
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
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
		return;
	} catch (...) {
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

	KWhiteDlgEx::OnOK();
}

bool KDlgShapeDemarcation::InvalidateInputInfo()
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

	if (!InValidColumnData())
		return false;

	return true;
}

bool KDlgShapeDemarcation::InValidColumnData()
{
	m_vecShapeMatching.clear();
	m_vecShapeMatching_Normal.clear();
	m_vecShapeMatching_PK.clear();

	try
	{
		KIOColumn*                   pIOColumn  = nullptr;
		CXTPReportRecordItemComboEx* pItemCombo = nullptr;
		CXTPReportRecordItem*        pItem      = nullptr;
		CXTPReportRecord*            pRecord    = nullptr;
		CXTPReportRecords*           pRecords   = m_ctrlReportDefine.GetRecords();
		int nRecordCount           = pRecords->GetCount();

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord	           = pRecords->GetAt(i);
			pItem              = pRecord->GetItem(_0_COLUMN_FIELDNAME);
			pIOColumn          = (KIOColumn*)pItem->GetItemData();

			pItemCombo         = (CXTPReportRecordItemComboEx*)pRecord->GetItem(_3_COLUMN_IMPORT_FIELDNAME);
			int nCursel        = pItemCombo->GetCurSel();
			int nShapeFieldIdx = (int)pItemCombo->GetItemData(nCursel);

			// PK 필드는 매칭 되는 shape field 정보가 반드시 있어야 함. (null: -1)
			if (nShapeFieldIdx < 0)
			{
				if (pIOColumn->PrimaryKey())
				{
					CString strMsg(_T(""));
					//strMsg.Format(_T("%s 컬럼에 대한 매칭 컬럼 정보가 존재하지 않습니다."), pIOColumn->Caption());
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strMsg.Format(_T("%s 컬럼에 대한 매칭 컬럼 정보가 존재하지 않습니다."), pIOColumn->Caption());
					}
					else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
						strMsg.Format(_T("There is no any matched information for %s Column."), pIOColumn->Caption());
					}
					AfxMessageBox(strMsg);
					return false;
				}
			}
			else
			{
				KImportShapeMatching oMatchingInfo;
				oMatchingInfo.pMatColumn  = pIOColumn;
				oMatchingInfo.nShapeIndex = (long)nShapeFieldIdx;
				m_vecShapeMatching.push_back(oMatchingInfo);

				if (pIOColumn->PrimaryKey())
					m_vecShapeMatching_PK.push_back(oMatchingInfo);
				else
					m_vecShapeMatching_Normal.push_back(oMatchingInfo);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

unsigned __stdcall KDlgShapeDemarcation::ThreadRun( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	if (pParameter != nullptr) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KDlgShapeDemarcation* pDlg = (KDlgShapeDemarcation*)pPara->TWindow;

			pDlg->Execute(pPara);
		}
	}

	return 1;
}

void KDlgShapeDemarcation::Execute(ThreadPara* pPara)
{
	try 
	{
		CString strShapeFile(_T(""));
		strShapeFile = pPara->TKeyStr[0];

		ShapeFileReader oShapeFile;
		OpenShapeFile(strShapeFile, oShapeFile);

		bool bErrFile(true); // ErrFile 셋팅시 문제가 발생하더라도, 이와 관계 없이 진행 시키기 위해
		QBicFile::DeleteFile(m_strErrFile);
		QBicTxtWriter ofErr; {
			if (ofErr.Open(m_strErrFile) == false)
				bErrFile = false;
		}

		m_nShapeIndexOfID = -1; // ID 필드와 매칭되어있는 Shape 필드 Index
		m_strKeyFieldName = _T("");
		if (!GetShapeIndexOfID(m_nShapeIndexOfID, m_strKeyFieldName)) { 
			//ThrowException(_T("ID 필드와 매칭되는 Shape 정보가 없습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("ID 컬럼과 매칭되는 Shape 정보가 없습니다."));
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("There is no any matched Shape for ID Column."));
			}
		}

		UpdateShapeFileData(oShapeFile, bErrFile, ofErr);

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

bool KDlgShapeDemarcation::GetShapeIndexOfID(OUT int &a_nShapeIndex_ID, CString &a_strKeyFieldName)
{
	try
	{
		CString strIDColumn(_T("zone_id"));
		a_strKeyFieldName = strIDColumn;

		int nSizeVec = (int)m_vecShapeMatching.size();
		for (int i= 0; i< nSizeVec; i++)
		{
			const KImportShapeMatching &oMatchInfo = m_vecShapeMatching[i];
			const KIOColumn*            pIOColumn  = oMatchInfo.pMatColumn;

			if (_tcsicmp(pIOColumn->Name(), strIDColumn) == 0)
			{
				a_nShapeIndex_ID = oMatchInfo.nShapeIndex;
				return true;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return false;
}

void KDlgShapeDemarcation::UpdateShapeFileData(ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr)
{
	// 레이어 테이블 Full 조회 
	std::map<Integer, TDBLayerInfo> mapShapeData; {
		LoadShapeData(m_pTarget->GetDBaseConnection(), TABLE_TXSHP_ZONE, mapShapeData);
	}

	ITxMapServerPtr   spMapServer = nullptr;
	TxFeatureLayerPtr spZoneLayer = nullptr;
	try 
	{
		LoadLayerInfo(m_pTarget, spMapServer, spZoneLayer);
		if (nullptr == spMapServer)
			throw 1;
		if (nullptr == spZoneLayer)
			throw 2;
	} catch (int& ex) {
		CString strError(_T(""));
		if (1 == ex) {
			strError.Format(_T("Error : %d (server 접속 실패)"), ex);
		} else if (2 == ex) {
			strError.Format(_T("Error : %d (레이어 조회 실패)"), ex);
		} else {
			strError.Format(_T("Error : %d"), ex);
		}
		TxLogDebug((LPCTSTR)strError);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 갱신에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to update shape."));
		}
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		throw ex;
	} catch (...) {
		TxLogDebugException();
		//ThrowException(_T("Update Shape에 실패 하였습니다.")); 
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 갱신에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to update shape."));
		}
	}

	spMapServer->BeginTransaction();

	try
	{
#pragma region Layer 테이블 Update 시작
		const int nSizeVecMatchInfo_Normal = (int)m_vecShapeMatching_Normal.size();
		const int nShapeType               = a_oShapeFile.ShapeType();
		const int nEntityCount             = a_oShapeFile.EntityCount();

		Integer nxTxID(0);
		int     nSuccessCnt(0);

		for (int i= 0; i< nEntityCount; i++)
		{
			if (i % 1000 == 0 || i == nEntityCount-1) {
				CString strMsg(_T(""));
				//strMsg.Format(_T("Update Shape %d / %d"), (i+1), nEntityCount);
				strMsg.Format(_T("Read Shape data ( %d / %d )"), (i+1), nEntityCount);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMsg.Format(_T("읽은 파일 라인수 / 총 라인수 :  %d / %d"), (i+1), nEntityCount);
				}
				AddStatusMessage( strMsg );
			}

			QBicBinary oBinary(20);
			a_oShapeFile.ReadShape(i, oBinary);

			int nGeometryType(0);
			oBinary.Position(0);
			oBinary.Read(nGeometryType);

			std::vector<DbfFieldValue> vecField       = a_oShapeFile.ReadRecord(i);
			const DbfFieldValue       &oDbfFieldValue = vecField[m_nShapeIndexOfID];

			nxTxID = _ttoi64(oDbfFieldValue.m_strValue); // ID

			auto itFindPreData = mapShapeData.find(nxTxID); // 저장된 ID인지 체크
			if (mapShapeData.end() == itFindPreData) {
				CString strFilterMsg(_T(""));
				//strFilterMsg.Format(_T("Update 실패: TxID(%I64d) 존재하지 않음"), nxTxID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strFilterMsg.Format(_T("갱신 실패 : ID (%I64d)가 존재하지 않음"), nxTxID);
				}
				else {
					strFilterMsg.Format(_T("Failed to update : ID (%I64d) does NOT exist"), nxTxID);
				}
				TxLogDebug((LPCTSTR)strFilterMsg);

				if (a_bErrFile) {
					ofErr.Write(strFilterMsg);
				}

				continue;
			}

			TxRecord oRecord; //zone
			for (int k= 0; k< nSizeVecMatchInfo_Normal; k++) {
				const KImportShapeMatching &oMatchInfo     = m_vecShapeMatching_Normal[k];
				const KIOColumn*            pIOColumn      = oMatchInfo.pMatColumn;
				const int                   nShapeIndex    = oMatchInfo.nShapeIndex;

				const DbfFieldValue        &oDbfFieldValue = vecField[nShapeIndex];

				if (pIOColumn->DataType() == KEMIODataTypeInteger) {
					Integer nxValue = _ttoi64(oDbfFieldValue.m_strValue);
					oRecord.AddItem(pIOColumn->Name(), nxValue);	
				} else if (pIOColumn->DataType() == KEMIODataTypeDouble) {
					double dValue   = _ttof(oDbfFieldValue.m_strValue);
					oRecord.AddItem(pIOColumn->Name(), dValue);	
				} else {
					oRecord.AddItem(pIOColumn->Name(), oDbfFieldValue.m_strValue);	
				}
			}

			if (KEMGeoPolyGon-1 == nGeometryType) { // 4 : TxPolygon
				ITxGeometryPtr spGeometry = TxPolygonPtr(new TxPolygon(oBinary, 0));
				spZoneLayer->Update(nxTxID, spGeometry, oRecord);
			} else if(KEMGeoPolyGon == nGeometryType) { // 5: TxMPolygon
				ITxGeometryPtr spGeometry = TxMPolygonPtr(new TxMPolygon(oBinary, 0));
				spZoneLayer->Update(nxTxID, spGeometry, oRecord);
			} else {
				CString strFilterMsg(_T(""));
				//strFilterMsg.Format(_T("Update 실패: TxID(%I64d) - None Polygon"), nxTxID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strFilterMsg.Format(_T("갱신 실패 : ID (%I64d)의 Polygon 정보가 존재하지 않음"), nxTxID);
				}
				else {
					strFilterMsg.Format(_T("Failed to update : Polygon does NOT exist for ID (%I64d)"), nxTxID);
				}
				if (a_bErrFile) {
					ofErr.Write(strFilterMsg);
				}

				continue;
			}

			nSuccessCnt++;
		}
#pragma endregion Layer 테이블 Update 시작

		int nFailCnt = nEntityCount - nSuccessCnt; // 전체개수-입력성공개수
		CString strMsg(_T("")); {
			int nIdx(1);

			if (KmzSystem::GetLanguage() == KEMKorea) {
				strMsg = _T("입력된 데이터의 내역은 아래와 같습니다.\r\n");
				strMsg.AppendFormat(_T("#%d. 데이터 개수 : %d\r\n"), nIdx++, nEntityCount);
				if (nFailCnt > 0) { //실패 건수가 있을 경우
					strMsg.AppendFormat(_T("#%d. 양호: %d 건, 불량: %d 건 \r\n"), nIdx++, nSuccessCnt, nFailCnt);
				}

				//strMsg.Append(_T("\n새 경계 정보를 이용하여 'Zone_ID' 컬럼을 갱신 하시겠습니까?"));
			}
			else {
				strMsg = _T("Imported data are as follows.\r\n");
				strMsg.AppendFormat(_T("#%d. Total number of records : %d\r\n"), nIdx++, nEntityCount);
				if (nFailCnt > 0) { //실패 건수가 있을 경우
					strMsg.AppendFormat(_T("#%d. Imported : %d, Skipped : %d\r\n"), nIdx++, nSuccessCnt, nFailCnt);
				}

				//strMsg.Append(_T("\nDo you want to update the 'Zone_ID' column with the new Zone Demarcation?"));
			}
			
		}

		if (nFailCnt > 0 && a_bErrFile) { //실패 건수가 있을 경우
			ErrorFileOpen();
		}

		if (AfxMessageBox(strMsg, MB_YESNO) == IDNO) {
			ThrowException(_T("User Canceled."));
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);

		spMapServer->Rollback();
		//ThrowException(_T("Update Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 갱신에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to update shape."));
		}
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);

		spMapServer->Rollback();
		throw ex;
	} catch (...) {
		TxLogDebugException();

		spMapServer->Rollback();
		//ThrowException(_T("Update Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 갱신에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to update shape."));
		}
	}

	spMapServer->Commit();
}


void KDlgShapeDemarcation::LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spZoneLayer, bool bInitExtent)
{
	try 
	{
		KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(a_pTarget);
		if (pCreatedMapView != nullptr) { // MapView가 생성되어 있는지 확인
			ITxFeatureLayerPtr spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Zone());
			if (spLayer != nullptr) {
				spZoneLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spMapServer = spZoneLayer->GetServer();
		} else { // MapView가 생성 되어 있지 않으면 서버에 직접 접속
			CString strServerPath = a_pTarget->GetIODataFilePath();
			spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, strServerPath));
			if ( spMapServer->Connect() != 1)  
				throw 1;

			// 서버의 레이어 정보 조회
			std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
			for (size_t j=0; j<vecLayerInfo.size(); j++) {
				TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

				if (_tcsicmp(_T("zone"), spLayerInfo->Name()) == 0) { // 존 레이어 조회
					spZoneLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
					break;
				}
			}
		}

		if (bInitExtent) {
			if (nullptr != spZoneLayer) {
				TxLayerInfoPtr    spLayerInfo    = spZoneLayer->LayerInfo();
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


void KDlgShapeDemarcation::LoadShapeData(KDBaseConPtr a_spDBConnection, LPCTSTR lpszLayerTable, std::map<Integer, TDBLayerInfo> &a_mapShapeData)
{
	a_mapShapeData.clear();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" SELECT TxID, Avg_Extent, MinX, MinY, MaxX, MaxY, Version FROM %s "), lpszLayerTable);
		KResultSetPtr spResult = a_spDBConnection->ExecuteQuery(strSQL);

		TDBLayerInfo oDBLayerInfo;

		while (spResult->Next())
		{
			oDBLayerInfo.nxTxID     = spResult->GetValueInt64 (0);
			oDBLayerInfo.dAvgExtent = spResult->GetValueDouble(1);
			oDBLayerInfo.dMinX      = spResult->GetValueDouble(2);
			oDBLayerInfo.dMinY      = spResult->GetValueDouble(3);
			oDBLayerInfo.dMaxX      = spResult->GetValueDouble(4);
			oDBLayerInfo.dMaxY      = spResult->GetValueDouble(5);
			oDBLayerInfo.nVersion   = spResult->GetValueInt   (6);

			a_mapShapeData.insert(std::make_pair(oDBLayerInfo.nxTxID, oDBLayerInfo));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDlgShapeDemarcation::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow) {
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KDlgShapeDemarcation::ErrorFileOpen()
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

bool KDlgShapeDemarcation::SpatialCalculateZoneID(CWnd* a_pParent, KTarget* a_pTarget)
{
	CString         strServerPath = a_pTarget->GetIODataFilePath();
	ITxMapServerPtr spMapServer(new TxSQLMapServer(TxMapDataServerSqlite, strServerPath));
	if ( spMapServer->Connect() != 1)  
		return false; 

	TxFeatureLayerPtr spPolygonFeatureLayer = nullptr; // 노드 Import는 존 정보도 Import 해야 한다.
	TxFeatureLayerPtr spPointFeatureLayer   = nullptr; {
		// 서버의 레이어 정보 조회
		std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
		for (size_t j=0; j<vecLayerInfo.size(); j++) {
			TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

			if (_tcsicmp(TABLE_NODE, spLayerInfo->Name()) == 0) { // 노드 레이어 조회
				spPointFeatureLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}

			if (_tcsicmp(TABLE_ZONE, spLayerInfo->Name()) == 0) { // 존 레이어 조회
				spPolygonFeatureLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}
		}
		if (nullptr == spPointFeatureLayer || nullptr == spPolygonFeatureLayer)
			return false;
	}

	std::map<Integer, std::vector<Integer>> mapMappingInfo; // 폴리곤에 속한 포인트ID들
	PolygonToPoint(spPolygonFeatureLayer, spPointFeatureLayer, mapMappingInfo);

	CString strSQL(_T(""));
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{        
		spDBaseConnection->BeginTransaction();
		strSQL.Format(_T(" Update %s Set %s = '0' "), TABLE_NODE, COLUMN_ZONE_ID);
		spDBaseConnection->ExecuteUpdate(strSQL);

		strSQL.Format(_T(" Update %s Set %s = ? Where %s = ? "), TABLE_NODE, COLUMN_ZONE_ID, COLUMN_NODE_ID);
		KPreparedStatementPtr spPrepared = spDBaseConnection->PrepareStatement(strSQL);

		int nStep(1);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicDonkeyProgress oDonkeyProgress(a_pParent, QBicDonkeyProgress::DEFAULT_TITLE, TRUE, FALSE, FALSE, nLang);
		CString strMsg(_T(""));
		
		auto iter  = mapMappingInfo.begin();
		auto itEnd = mapMappingInfo.end();

		for (; iter != itEnd; ++iter) {
			const Integer        &nxPolygonID = iter->first;
			std::vector<Integer> &vecPointID  = iter->second;

			size_t nxSizePoint = vecPointID.size();
			for (size_t i= 0; i< nxSizePoint; i++)
			{
				strMsg.Format(_T("Spatial Calculate[%d] zone_id[%I64d] -> Node : %I64d"), nStep, nxPolygonID, vecPointID[i]);
				oDonkeyProgress.SendTextEvent(strMsg);

				spPrepared->BindInt64(1, nxPolygonID);   //zone_id
				spPrepared->BindInt64(2, vecPointID[i]); //node_id

				spPrepared->ExecuteUpdate();
				spPrepared->Reset();
			}

			nStep++;
			QBicDoEvents::DoEvents();
		}

		strSQL.Format(_T(" Update %s Set %s = %s Where %s = '0' "), TABLE_NODE, COLUMN_ZONE_ID, COLUMN_NODE_ID, COLUMN_NODE_TYPE);
		spDBaseConnection->ExecuteUpdate(strSQL);

		spDBaseConnection->Commit();
		return true;
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
	}

	//AfxMessageBox(_T("경계 정보 업데이트 중 오류가 발생하였습니다."));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		AfxMessageBox(_T("경계 정보 갱신 중 오류가 발생하였습니다."));
	}
	else {
		AfxMessageBox(_T("An error occurred while updating with Zone Demarcation."));
	}
	return false;
}

void KDlgShapeDemarcation::PolygonToPoint( ITxFeatureLayerPtr a_spPolygonFeatureLayer, ITxFeatureLayerPtr a_spPointFeatureLayer, 
											OUT std::map<Integer, std::vector<Integer>> &ar_mapMappingInfo )
{
	std::map<Integer, ITxGeometryPtr> mapPolygonGeometry;
	a_spPolygonFeatureLayer->GetGeometry(mapPolygonGeometry);

	std::map<Integer, ITxGeometryPtr> mapPointGeometry;
	a_spPointFeatureLayer->GetGeometry(mapPointGeometry);

	// 매칭 데이터 mapping 데이터 <point, vec<polygon>>
	std::map<Integer, std::vector<Integer>> mapPointIncludePolygon; {
		auto iter  = mapPolygonGeometry.begin();
		auto itEnd = mapPolygonGeometry.end();

		for (; iter != itEnd; ++iter) {
			const Integer  &nxPolygonID = iter->first;
			ITxGeometryPtr spGeoPolygon = iter->second;

			std::vector<TxFeaturePtr> vecPointFeature; { // Polygon 안에 있는 포인트들
				a_spPointFeatureLayer->FindFeature(spGeoPolygon->Envelope(), vecPointFeature);
			}

			for (size_t i= 0; i< vecPointFeature.size(); i++) {
				TxFeaturePtr spFeature = vecPointFeature[i];
				Integer      nxPointID = spFeature->TxObjectID;

				std::vector<Integer> &vecPolygon = mapPointIncludePolygon[nxPointID];
				vecPolygon.push_back(nxPolygonID);
			}
		}
	}

	auto iter  = mapPointIncludePolygon.begin();
	auto itEnd = mapPointIncludePolygon.end();
	for (; iter != itEnd; ++iter) {
		const Integer        &nxPointID   = iter->first;
		std::vector<Integer> &vecPolygons = iter->second;
		const int            nCntPolygon  = (int)vecPolygons.size();
		
		auto findPoint = mapPointGeometry.find(nxPointID);
		auto fendPoint = mapPointGeometry.end();
		if (findPoint == fendPoint)
			continue;

		TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(findPoint->second->CopyGeometry());
		TxPoint	   oPoint(spPoint->X, spPoint->Y);

		Integer nxGoalPolygonID(0); { // Point 가 속해있는 PolygonID 찾기
			if (nCntPolygon == 1) {// 1개이면 바로 입력
				nxGoalPolygonID = vecPolygons[0];
			} else if (nCntPolygon > 1) { // 1개 이상이면 그 중 가장 가까운 데이터 
#pragma region 가까운 PolygonID 부여
				Integer nxMinPolygonID(0);
				double  dMinDist(0);
				for (int i= 0; i< nCntPolygon; i++) {
					const Integer &nxPolygonID = vecPolygons[i];

					auto findPolygon = mapPolygonGeometry.find(nxPolygonID);
					if (findPolygon == mapPolygonGeometry.end())
						continue; 

					ITxGeometryPtr spGeoPolygon = findPolygon->second;

					double dDist(0); {
						if (spGeoPolygon->GeometryType() == GeoMPolygonType) {
							TxMPolygonPtr spMPolygon = std::dynamic_pointer_cast<TxMPolygon>(spGeoPolygon->CopyGeometry());
							dDist = spMPolygon->MinDist(oPoint);
						} else {
							TxPolygonPtr  spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeoPolygon->CopyGeometry());
							dDist = spPolygon->MinDist(oPoint);
						}
					}

					if (0 == i) {
						dMinDist = dDist;
						nxMinPolygonID = nxPolygonID;
					} else {
						if (dMinDist > dDist) {
							dMinDist = dDist;
							nxMinPolygonID = nxPolygonID;
						}
					}
				}
#pragma endregion 가까운 PolygonID 부여

				nxGoalPolygonID = nxMinPolygonID;
			} else if (nCntPolygon < 1) {//없으면 전체 데이터중 가장 가까운 데이터
#pragma region 가까운 PolygonID 부여
				Integer nxMinPolygonID(0);
				double  dMinDist(0);
				int		nIndex(0);

				auto iter  = mapPolygonGeometry.begin();
				auto itEnd = mapPolygonGeometry.end();
				for (; iter != itEnd; ++iter) {
					const Integer  &nxPolygonID = iter->first; 
					ITxGeometryPtr spGeoPolygon = iter->second;

					double dDist(0);{
						if (spGeoPolygon->GeometryType() == GeoMPolygonType) {
							TxMPolygonPtr spMPolygon = std::dynamic_pointer_cast<TxMPolygon>(spGeoPolygon->CopyGeometry());
							dDist = spMPolygon->MinDist(oPoint);
						} else {
							TxPolygonPtr  spPolygon	 = std::dynamic_pointer_cast<TxPolygon>(spGeoPolygon->CopyGeometry());
							dDist = spPolygon->MinDist(oPoint);
						}

						if (0 == nIndex) {
							dMinDist       = dDist;
							nxMinPolygonID = nxPolygonID;
						} else {
							if (dMinDist > dDist) {
								dMinDist = dDist;
								nxMinPolygonID = nxPolygonID;
							}
						}
					}

					nIndex++;
				}
#pragma endregion 가까운 PolygonID 부여

				nxGoalPolygonID = nxMinPolygonID;
			}
		}

		// insert
		std::vector<Integer> &vecPointID = ar_mapMappingInfo[nxGoalPolygonID]; // 최종 분별된 폴리곤에 속한 포인트ID를 저장한다.
		vecPointID.push_back(nxPointID);
	}
}

int KDlgShapeDemarcation::GetSelectedEncoding()
{
	int nEncoding; {
		if (m_cboEncoding.GetCurSel() == 1)
			nEncoding = KEMCSVEncodingUTF8;
		else 
			nEncoding = KEMCSVEncodingANSI;
	}

	return nEncoding;
}

void KDlgShapeDemarcation::OnCbnSelchangeCombo4()
{
	UpdateReportDataPreview();
}