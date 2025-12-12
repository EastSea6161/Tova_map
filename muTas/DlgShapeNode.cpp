// DlgShapeNode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "DlgShapeNode.h"
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

#include "DefineSystemCodeDlg.h"
#include "NetworkEditLog.h"
#include "ImportCommon.h"
#include "DBaseInterModal.h"

// KDlgShapeNode 대화 상자입니다.

namespace ShapeNodeENUM
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

using namespace ShapeNodeENUM;

IMPLEMENT_DYNAMIC(KDlgShapeNode, KWhiteDlgEx)

KDlgShapeNode::KDlgShapeNode(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KWhiteDlgEx(KDlgShapeNode::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pTable(a_pTable)
	, m_nGeoType(KEMGeoNull)
	, m_nShapeIndexOfID(-1)
	, m_strKeyFieldName(_T(""))
    , m_spFeatureLayer(nullptr)
    , m_spZoneLayer(nullptr)
{
	CString strTargetLocation = m_pTarget->GetLocation();

	m_strErrFile.Format(_T("%s\\%s"), strTargetLocation, IMPORT_ERROR_FILE_NAME);
}

KDlgShapeNode::~KDlgShapeNode()
{
}

void KDlgShapeNode::DoDataExchange(CDataExchange* pDX)
{
	KWhiteDlgEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BROWSE,	 m_editShapeFile);
	DDX_Control(pDX, IDC_REPORT_DEFINE,  m_ctrlReportDefine);
	DDX_Control(pDX, IDC_REPORT_COLUMN,  m_ctrlReportDrag);
	DDX_Control(pDX, IDC_REPORT_PREVIEW, m_ctrlReportPreview);
	DDX_Control(pDX, IDC_COMBO4, m_cboEncoding);
}


BEGIN_MESSAGE_MAP(KDlgShapeNode, KWhiteDlgEx)
	ON_EN_CHANGE(IDC_EDIT_BROWSE, &KDlgShapeNode::OnEnChangeEditBrowse)
	ON_NOTIFY(XTP_NM_REPORT_RECORDS_DROPPED,      IDC_REPORT_DEFINE,  &KDlgShapeNode::OnReportRecordsDropped)
	ON_NOTIFY(XTP_NM_REPORT_DROP,                 IDC_REPORT_COLUMN,  &KDlgShapeNode::OnReportRecordsDroping)

	ON_BN_CLICKED(IDC_BUTTON1, &KDlgShapeNode::OnBnClickedButtonRun)
	ON_CBN_SELCHANGE(IDC_COMBO4, &KDlgShapeNode::OnCbnSelchangeCombo4)
END_MESSAGE_MAP()


// KDlgShapeNode 메시지 처리기입니다.
void KDlgShapeNode::SetResizeComponent()
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


BOOL KDlgShapeNode::OnInitDialog()
{
	KWhiteDlgEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		InitControl();

		QBicReportCtrlSetting::Default(m_ctrlReportDefine, TRUE, FALSE, FALSE);
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


void KDlgShapeNode::InitControl()
{
	TCHAR szFilter[] = _T("Shp Files (*.shp)|*.shp|");
	m_editShapeFile.Initialize( this, BES_XT_CHOOSEFILE );
	m_editShapeFile.SetDlgOpenFile( TRUE );
	m_editShapeFile.SetReadOnly( TRUE );
	m_editShapeFile.SetDlgTitle( _T("Select Shape File") );
	m_editShapeFile.SetDlgFilter( szFilter );

	InitComboEncoding();
}


void KDlgShapeNode::InitComboEncoding()
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

void KDlgShapeNode::InitReportHeaderDefine()
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


void KDlgShapeNode::InitReportHeaderDrag()
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


void KDlgShapeNode::OnEnChangeEditBrowse()
{
	RefreshInfo();

	if (!LoadPreViewShapeField()) {
		RefreshInfo();
		return;
	} else {
		if (KEMGeoPoint != m_nGeoType) {
			RefreshInfo();
			//AfxMessageBox(_T("선택한 형상 파일이 유효하지 않습니다.\r\n(Point 형상이 아닙니다.)"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("선택한 Shape 파일(Point)이 유효하지 않습니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("The shape file you selected(Point) is not valid."));
			}
			return;
		}
	}

	UpdateReportDataDefine();
	UpdateReportDataDrag();
	UpdateReportDataPreview();
}


void KDlgShapeNode::RefreshInfo()
{
	m_mapShapeField.clear();
	m_nGeoType = KEMGeoNull;

	m_ctrlReportDefine.ResetContent();
	m_ctrlReportDrag.ResetContent();

	m_ctrlReportPreview.ResetContent();
	m_ctrlReportPreview.GetColumns()->Clear();
	m_ctrlReportPreview.Populate();
}


bool KDlgShapeNode::LoadPreViewShapeField()
{
	m_mapShapeField.clear();
	m_nGeoType = KEMGeoNull;

	try
	{
		CString strShapeFile(_T(""));
		m_editShapeFile.GetWindowText(strShapeFile);

		ShapeFileReader oShapeFile;
		OpenShapeFile(strShapeFile, oShapeFile);

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


void KDlgShapeNode::OpenShapeFile(CString a_strShapeFile, ShapeFileReader &a_oShapeFile)
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
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
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


void KDlgShapeNode::UpdateReportDataDefine()
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

			// 0.field			
			CString strFieldName = pIOColumn->Name();

			// x,y 좌표는 shp 파일로 부터 얻는다.
			if (_tcsicmp(pIOColumn->Name(), COLUMN_NODE_X) == 0 || _tcsicmp(pIOColumn->Name(), COLUMN_NODE_Y) == 0) {
				continue;
			}

			pRecord   = m_ctrlReportDefine.AddRecord(new CXTPReportRecord);

            bool bRed(false);
            if (strFieldName.CompareNoCase(COLUMN_NODE_ID) == 0 || strFieldName.CompareNoCase(COLUMN_NODE_TYPE) == 0) {
                CString strCaption(_T("")); {
                    strCaption.Format(_T("* %s"), pIOColumn->Caption());                    
                }
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
                bRed = true;
            }
            else {
			    pItem = pRecord->AddItem(new CXTPReportRecordItemText(pIOColumn->Caption()));
            }

			pItem->SetItemData((DWORD_PTR)pIOColumn);
			pItem->SetAlignment(DT_CENTER);
            
            if (bRed) {
                pItem->SetTextColor(RGB(255,0,0));
            }

			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			// 1.type
			CString strDataType(_T(""));
			switch(pIOColumn->DataType())
			{
			case KEMIODataTypeInteger : strDataType = DATATYPESTRING_INTEGER; break;
			case KEMIODataTypeDouble  : strDataType = DATATYPESTRING_DOUBLE;  break;
			case KEMIODataTypeString  : strDataType = DATATYPESTRING_STRING;  break;
			default: break;
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDataType));
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			// 2.PK
			CString strPK = _T("N");
			if (_tcsicmp(pIOColumn->Name(), COLUMN_NODE_ID) == 0 || _tcsicmp(pIOColumn->Name(), COLUMN_NODE_TYPE) == 0) {
				strPK = _T("Y");
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strPK));
			pItem->SetAlignment(DT_CENTER);
			//pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			// 3.Import Field
			pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx);
			pItemCombo->AddString(_T("Null"), -1);

			int nInitIdx(0);
			int nIndex(0);

            CString strCaption = pIOColumn->Caption();
			auto iter  = m_mapShapeField.begin();
			auto itEnd = m_mapShapeField.end();
			for (; iter != itEnd; ++iter)
			{
				int     nShapeFieldIdx    = (int)iter->first;
				CString strShapeFieldName = iter->second;

				nIndex = pItemCombo->AddString(strShapeFieldName, (DWORD_PTR)nShapeFieldIdx);

				if (strCaption.CompareNoCase(strShapeFieldName) == 0 || strFieldName.CompareNoCase(strShapeFieldName) == 0)
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


void KDlgShapeNode::UpdateReportDataDrag()
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


void KDlgShapeNode::UpdateReportDataPreview()
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


void KDlgShapeNode::OnReportRecordsDropped( NMHDR *pNotifyStruct, LRESULT *pResult )
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


void KDlgShapeNode::OnReportRecordsDroping( NMHDR *pNotifyStruct, LRESULT *pResult )
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

void KDlgShapeNode::OnBnClickedButtonRun()
{
	UpdateData(TRUE);

	if (!InvalidateInputInfo())
		return;

	// 2019.05.27 : Free 버전 관련 수정
	// Shape 파일을 열어서 Record 수를 Free버전의 최대 노드수와 비교
	if (theApp.IsFreeVersion())
	{
		CString strShapeFile;
		m_editShapeFile.GetWindowText(strShapeFile);
		ShapeFileReader oShapeFile;
		OpenShapeFile(strShapeFile, oShapeFile);
		int nNodeRecCount = oShapeFile.RecordCount();
		if (nNodeRecCount > CKmzApp::MAX_NODE_OF_FREEVERSION)
		{
			CString strOverNodeMsg = _T("Up to 3000 nodes can be entered in free mode.");
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				strOverNodeMsg = _T("Free mode에서는 최대 3000개 노드까지 입력할 수 있습니다.");
			}
			AfxMessageBox(strOverNodeMsg);
			return;
		}
	}

    bool bHadData(false); { // 기존에 저장된 데이터가 있었는지 판단
        if (KImportCommon::IsEmptyTable(m_pTarget, TABLE_NODE) == false) { // true: 데이터무, false : 데이터 존재
            bHadData = true;
        }
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

    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    KImportCommon::DeleteNodeRelation(spDBase); // 트랜잭션을 활용할 수가 없다. (spDBase와 spMapServer는 같은 곳을 바라보고 있기 때문)

    //-----------------------------------------------------------

    ITxMapServerPtr   spMapServer = nullptr;

    try
    {
        if (nullptr != m_spFeatureLayer) {
            m_spFeatureLayer.reset();
            m_spFeatureLayer = nullptr;
        }
        if (nullptr != m_spZoneLayer) { // 노드 Import는 존 정보도 Import 해야 한다.
            m_spZoneLayer.reset();
            m_spZoneLayer = nullptr;
        }

        LoadLayerInfo(m_pTarget, spMapServer, m_spFeatureLayer, m_spZoneLayer);
        if (nullptr == spMapServer)
            throw 1;
        if (nullptr == m_spFeatureLayer || nullptr == m_spZoneLayer)
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

        //ThrowException(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...) {
        TxLogDebugException();
        //ThrowException(_T("Import Shape에 실패 하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("Shape 불러오기에 실패 하였습니다."));
		}
		else {
			ThrowException(_T("Failed to import shape data."));
		}
    }

    spMapServer->BeginTransaction();

	CString strShapeFile(_T(""));
	m_editShapeFile.GetWindowText(strShapeFile);

	try
	{
		ThreadPara oPara(this);
		if (true) {
            m_mapReadNodeType.clear();

            int nInsertCnt(0);
            int nFailedCnt(0);

			oPara.TKeyStr[0] = strShapeFile; // shape 파일

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
						strMsg.Append(_T("#2. 노드 타입별 건수 :\r\n"));

						auto iter = m_mapReadNodeType.begin();
						auto itEnd = m_mapReadNodeType.end();
						for (; iter != itEnd; ++iter) {
							const int &nNodeType = iter->first;
							int       &nCount = iter->second;

							if (0 == nNodeType) {
								strMsg.AppendFormat(_T("- (%d: 존): %d\r\n"), nNodeType, nCount);
							}
							else {
								strMsg.AppendFormat(_T("- (%d): %d\r\n"), nNodeType, nCount);
							}
						}

						strMsg.Append(_T("\n계속 진행하시겠습니까?\r\n"));
					}
					else {
						strMsg.Append(_T("Imported data are as follows.\r\n"));
						strMsg.AppendFormat(_T("#1. Total number of records : %d\r\n"), nInsertCnt);
						strMsg.Append(_T("#2. The number of records by node type :\r\n"));

						auto iter = m_mapReadNodeType.begin();
						auto itEnd = m_mapReadNodeType.end();
						for (; iter != itEnd; ++iter) {
							const int &nNodeType = iter->first;
							int       &nCount = iter->second;

							if (0 == nNodeType) {
								strMsg.AppendFormat(_T("- (%d: Zone): %d\r\n"), nNodeType, nCount);
							}
							else {
								strMsg.AppendFormat(_T("- (%d): %d\r\n"), nNodeType, nCount);
							}
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
    }

    try
    {
        KNetworkEditLog::SetImportNetworkLog(m_pTarget);

// 				KDefineSystemCodeDlg		oSystemDefineDlg(m_pTarget, true);
// 				oSystemDefineDlg.DoModal(); // 제거 - 명지대 요청 사항(16.07.15)

        KImportCommon::NotifyNodeRelation(m_pTarget);

        // Taget_region 코드가 레코드 내에 존재한다면, 코드메니저에 등록하자
        // 주의 사항은 NotifyNodeRelation() 이후에 해야 한다.
        KDBaseInterModal::AddTargetRegionCode(m_pTarget);

        KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(m_pTarget);
        if (nullptr != pCreatedMapView) {
            pCreatedMapView->ReloadRender(false, 1);
            pCreatedMapView->ReloadRender(false, 3);
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

	KWhiteDlgEx::OnOK();
}

bool KDlgShapeNode::InvalidateInputInfo()
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

bool KDlgShapeNode::InValidColumnData()
{
	m_vecShapeMatching.clear();

	try
	{
		KIOColumn*                   pIOColumn  = nullptr;
		CXTPReportRecordItemComboEx* pItemCombo = nullptr;
		CXTPReportRecordItem*        pItem      = nullptr;
		CXTPReportRecord*            pRecord    = nullptr;
		CXTPReportRecords*           pRecords   = m_ctrlReportDefine.GetRecords();
		int nRecordCount           = pRecords->GetCount();

		CString strColumnName(_T(""));

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord	           = pRecords->GetAt(i);
			pItem              = pRecord->GetItem(_0_COLUMN_FIELDNAME);
			pIOColumn          = (KIOColumn*)pItem->GetItemData();

			pItemCombo         = (CXTPReportRecordItemComboEx*)pRecord->GetItem(_3_COLUMN_IMPORT_FIELDNAME);
			int nCursel        = pItemCombo->GetCurSel();
			int nShapeFieldIdx = (int)pItemCombo->GetItemData(nCursel);

			strColumnName = pIOColumn->Name();

			// PK 필드는 매칭 되는 shape field 정보가 반드시 있어야 함. (null: -1)
			if (nShapeFieldIdx < 0)
			{
				if( strColumnName.CompareNoCase(COLUMN_NODE_ID) == 0 || 
					strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0 )
				{
					CString strMsg(_T(""));
					//strMsg.Format(_T("%s 컬럼에 대한 매칭 컬럼 정보가 존재하지 않습니다."), strColumnName);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strMsg.Format(_T("%s 컬럼에 대한 매칭 컬럼 정보가 존재하지 않습니다."), strColumnName);
					}
					else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
						strMsg.Format(_T("There is no any matched information for %s Column."), strColumnName);
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

unsigned __stdcall KDlgShapeNode::ThreadRun( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	if (pParameter != nullptr) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KDlgShapeNode* pDlg = (KDlgShapeNode*)pPara->TWindow;

			pDlg->Execute(pPara);
		}
	}

	return 1;
}

void KDlgShapeNode::Execute(ThreadPara* pPara)
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

		m_nShapeIndexOfID = -1; // ID 필드와 매칭되어있는 Shape 필드 Index
		m_strKeyFieldName = _T("");
		if (!GetShapeIndexOfID(m_nShapeIndexOfID, m_strKeyFieldName)) { 
			//ThrowException(_T("ID 필드와 매칭되는 Shape 정보가 없습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("ID 컬럼과 매칭되는 Shape 정보가 없습니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("There is no any matched Shape for ID Column."));
			}
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

bool KDlgShapeNode::GetShapeIndexOfID(OUT int &a_nShapeIndex_ID, CString &a_strKeyFieldName)
{
	try
	{
		CString strIDColumn(_T("node_id"));
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

void KDlgShapeNode::ImportShapeFileData(ThreadPara* pPara, ShapeFileReader &a_oShapeFile, const bool a_bErrFile, QBicTxtWriter &ofErr)
{
    int &nInsertCnt = pPara->TKeyInt32[3]; //결과 리턴
    int &nFailedCnt = pPara->TKeyInt32[4]; //결과 리턴
    nInsertCnt = 0;
    nFailedCnt = 0;

    std::set<int> setFailedEntity;
    CString strErrorMsg(_T(""));

	try
	{
#pragma region Layer 테이블 Update 시작
		const int nSizeVecMatchInfo        = (int)m_vecShapeMatching.size();
		const int nShapeType               = a_oShapeFile.ShapeType();
		const int nEntityCount             = a_oShapeFile.EntityCount();

		Integer nxTxID(0);
		int     nSuccessCnt(0);
		std::map<int, int> mapReadNodeType;

		for (int i= 0; i< nEntityCount; i++)
		{
            bool bErrLine(false);

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

            if (KEMGeoPoint != nGeometryType) { // 1 : TxPoint
                CString strFilterMsg(_T(""));
                strErrorMsg.Format(_T("Line : %d =>\t None Point"), (i + 1));

                if (a_bErrFile) {
                    ofErr.Write(strFilterMsg);
                }
                bErrLine = true;

                setFailedEntity.insert(i); // 여기는 특수한 continue 니깐 추가 해줘야지
                continue;
            }

			std::vector<DbfFieldValue> vecField = a_oShapeFile.ReadRecord(i);

			Integer node_id(0);
			int     node_type(-1);

            CString strNodeID(_T(""));
            CString strNodeType(_T(""));
			CString strColumnName(_T(""));

			TxPoint oPoint(oBinary, 0);
			TxRecord oRecord; //node
			for (int k= 0; k< nSizeVecMatchInfo; k++) {
				const KImportShapeMatching &oMatchInfo     = m_vecShapeMatching[k];
				const KIOColumn*            pIOColumn      = oMatchInfo.pMatColumn;
				strColumnName             = pIOColumn->Name();
				const int                   nShapeIndex    = oMatchInfo.nShapeIndex;

				const DbfFieldValue        &oDbfFieldValue = vecField[nShapeIndex];
                CString                    strValue        = oDbfFieldValue.m_strValue;
                strValue.Trim();

				if (pIOColumn->DataType() == KEMIODataTypeInteger) {
					Integer nxValue = _ttoi64(strValue);

                    if (strColumnName.CompareNoCase(COLUMN_NODE_ID) == 0) {
                        node_id = nxValue;
                        strNodeID = strValue;
                    }
                    else if (strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0) {
                        node_type = (int)nxValue;
                        strNodeType = strValue;
                    }

					oRecord.AddItem(strColumnName, nxValue);	
				} else if (pIOColumn->DataType() == KEMIODataTypeDouble) {
					double dValue   = _ttof(strValue);

					oRecord.AddItem(strColumnName, dValue);	
				} else {
					oRecord.AddItem(strColumnName, strValue);
				}
			}

			// x, y 필드 추가
			{
				oRecord.AddItem(COLUMN_NODE_X, oPoint.X);
				oRecord.AddItem(COLUMN_NODE_Y, oPoint.Y);
			}

            if (strNodeID.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Line : %d =>\t 노드ID가 존재 하지 않습니다."), (i + 1));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t 노드 ID가 존재 하지 않습니다."), (i + 1));
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t Node ID does NOT exist."), (i + 1));
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
            }

            if (strNodeType.CompareNoCase(_T("")) == 0) {
                //strErrorMsg.Format(_T("Line : %d =>\t Node ID: %s 의 Type 데이터가 없습니다."), (i + 1), strNodeID);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t Node ID: %s 의 Type 데이터가 없습니다."), (i + 1), strNodeID);
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Forward-Line : %d =>\t The Node %s does NOT have Type data."), (i + 1), strNodeID);
				}
                if (a_bErrFile) {
                    ofErr.Write(strErrorMsg);
                }
                bErrLine = true;
            }

			ITxGeometryPtr spGeometry = TxPointPtr(new TxPoint(oPoint));
			m_spFeatureLayer->Insert(node_id, spGeometry, oRecord);

			//import한 data중 type이 0인 경우는 존노드이므로 따로 정보를 관리하여 insert한다.
			if (strNodeType.CompareNoCase(_T("")) != 0 && node_type == 0) { // strNodeType에 값이 있을때 체크
				double dX = oPoint.X;
				double dY = oPoint.Y;
				std::vector<TxPoint> vecPoint; { //x, y 좌표를 기점으로 +-50
					vecPoint.push_back(TxPoint(dX+50, dY+50)); // 1사분면
					vecPoint.push_back(TxPoint(dX-50, dY+50)); // 2사분면
					vecPoint.push_back(TxPoint(dX-50, dY-50)); // 3사분면
					vecPoint.push_back(TxPoint(dX+50, dY-50)); // 4사분면
					vecPoint.push_back(TxPoint(dX+50, dY+50));
				}
				TxPolygon oTxPolygon;
				ITxGeometryPtr spGeometry = TxPolygonPtr(new TxPolygon(vecPoint));
				TxRecord oRecord; //zone
				oRecord.AddItem(_T("zone_id"), node_id);
				m_spZoneLayer->Insert(node_id, spGeometry, oRecord);
			}

			nInsertCnt++;
            auto itFindNodeType = m_mapReadNodeType.find(node_type);
            if (itFindNodeType != m_mapReadNodeType.end()) {
                int &nTempCount = itFindNodeType->second;
                nTempCount++;
            }
            else {
                m_mapReadNodeType.insert(std::make_pair(node_type, 1));
            }

            if (bErrLine) {
                setFailedEntity.insert(i);
            }
		}
#pragma endregion Layer 테이블 Update 시작

        nFailedCnt = (int)setFailedEntity.size();
		if (nFailedCnt > 0) { //실패 건수가 있을 경우
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
		//strError.Format(_T("오류 : %d"), ex);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strError.Format(_T("오류 : %d"), ex);
		}
		else {
			strError.Format(_T("Error : %d"), ex);
		}
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


void KDlgShapeNode::LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spFeatureLayer, TxFeatureLayerPtr &spZoneLayer, bool bInitExtent)
{
	try 
	{
		KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(a_pTarget);
		if (pCreatedMapView != nullptr) { // MapView가 생성되어 있는지 확인
			ITxFeatureLayerPtr spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Node());
			if (spLayer != nullptr) {
				spFeatureLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Zone());
			if (spLayer != nullptr) {
				spZoneLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
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

				if (_tcsicmp(m_pTable->Name(), spLayerInfo->Name()) == 0) { // 노드 레이어 조회
					spFeatureLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
				}

				if (_tcsicmp(_T("zone"), spLayerInfo->Name()) == 0) { // 존 레이어 조회
					spZoneLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
				}
			}
		}

		if (bInitExtent) {
			if (nullptr != spFeatureLayer) {
				TxLayerInfoPtr    spLayerInfo    = spFeatureLayer->LayerInfo();
				TxLayerInfoSQLPtr spLayerInfoSQL = std::dynamic_pointer_cast<TxLayerInfoSQL>(spLayerInfo);
				spLayerInfoSQL->SetExtent(0.0,0.0,0.0,0.0); // 초기화
			}

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


void KDlgShapeNode::LoadShapeData(KDBaseConPtr a_spDBConnection, LPCTSTR lpszLayerTable, std::map<Integer, TDBLayerInfo> &a_mapShapeData)
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

void KDlgShapeNode::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow) {
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KDlgShapeNode::ErrorFileOpen()
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

int KDlgShapeNode::GetSelectedEncoding()
{
	int nEncoding; {
		if (m_cboEncoding.GetCurSel() == 1)
			nEncoding = KEMCSVEncodingUTF8;
		else 
			nEncoding = KEMCSVEncodingANSI;
	}

	return nEncoding;
}

void KDlgShapeNode::OnCbnSelchangeCombo4()
{
	UpdateReportDataPreview();
}
