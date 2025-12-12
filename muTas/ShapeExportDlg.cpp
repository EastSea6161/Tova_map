// ShapeExportDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ShapeExportDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "ImChampFrameWindow.h"
#include "MapView.h"

#include "../QBicGIS/ShapeFileWriter.h"

// KShapeExportDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KShapeExportDlg, KResizeDialogEx)

KShapeExportDlg::KShapeExportDlg(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KShapeExportDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pTable(a_pTable)
{

}

KShapeExportDlg::~KShapeExportDlg()
{
}

void KShapeExportDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME,       m_editFile );
	DDX_Control(pDX, IDC_REPORT,   m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_cboTable);
}


BEGIN_MESSAGE_MAP(KShapeExportDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &KShapeExportDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDOK, &KShapeExportDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE, &KShapeExportDlg::OnCbnSelchangeComboTable)
END_MESSAGE_MAP()


BOOL KShapeExportDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);

	IniteTableComboBox();
	IniteEditFile();
	IniteReportHeader();
	UpdateReportData();
	
	ResizeComponet();

	return TRUE;
}


void KShapeExportDlg::IniteTableComboBox()
{
	m_cboTable.ResetContent();
	try
	{
		std::vector<CString> vecShapeExportTable;
		vecShapeExportTable.push_back(TABLE_NODE);
		vecShapeExportTable.push_back(TABLE_LINK);
		vecShapeExportTable.push_back(TABLE_ZONE);

		int nIndex(0);
		int keepIndex=-1;
		for (size_t i= 0; i< vecShapeExportTable.size(); i++) {
			CString   strTableName = vecShapeExportTable[i];
			KIOTable* pShapeTable  = m_pTarget->Tables()->FindTable(strTableName);

			if (pShapeTable->RecordCount() < 1) {
				continue;
			}

			nIndex = m_cboTable.AddString(pShapeTable->Caption());
			m_cboTable.SetItemData(nIndex, (DWORD_PTR)pShapeTable);

			if (_tcsicmp(m_pTable->Caption(), pShapeTable->Caption()) == 0) {
				keepIndex = nIndex;
			}
		}

		if (keepIndex > 0) {
			m_cboTable.SetCurSel(keepIndex);
		}
		else
		{
			m_cboTable.SetCurSel(0);
		}
		//m_cboTable.SetCurSel(0);
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

//void KShapeExportDlg::SetCursor_cbo(CString& TName)
//{
//	CString str_node = _T("");
//	CString str_link = _T("");
//	CString str_zone = _T("");
//	str_node.Format(_T("%s"), TABLE_NODE);
//	str_link.Format(_T("%s"), TABLE_LINK);
//	str_zone.Format(_T("%s"), TABLE_ZONE);
//	if (!TName.CompareNoCase(str_node)) {
//		m_cboTable.SetCurSel(0);
//	}
//	else if (!TName.CompareNoCase(str_link)) {
//		m_cboTable.SetCurSel(1);
//	}
//	else if (!TName.CompareNoCase(str_zone)) {
//		m_cboTable.SetCurSel(2);
//	}
//	else
//	{
//		m_cboTable.SetCurSel(0);
//	}
//}

void KShapeExportDlg::IniteEditFile()
{
	try
	{
		TCHAR szFilter[] = _T("Shape Files (*.Shp)|*.Shp||");
		m_editFile.Initialize( this, BES_XT_CHOOSEFILE );
		m_editFile.SetDlgOpenFile( FALSE );
		m_editFile.SetReadOnly( TRUE );
		m_editFile.SetDlgTitle( _T("Select file") );
		m_editFile.SetDlgFilter( szFilter );
		m_editFile.SetDlgStyle(OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);

		KProject* pProject = ImChampFrameWindow::GetProject();
		m_editFile.SetDlgInitialDir(pProject->GetLocation());
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

void KShapeExportDlg::IniteReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0 , _T("Select"), 60, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("선택"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1,  _T("Column Name"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("컬럼명"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn( 2, _T("Data Type"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("데이터 유형"));
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

void KShapeExportDlg::UpdateReportData()
{
	m_wndReportCtrl.ResetContent();

	KIOTable* pTable = GetSelectedIOTable();

	try
	{
		if (pTable == nullptr)
		{
			m_wndReportCtrl.Populate();
			return;
		}

		KIOColumns* pColumns = pTable->Columns();
		int nCount = pColumns->ColumnCount();

		for (int i = 0; i <nCount ; i++)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);

			CXTPReportRecord* pRecord		= m_wndReportCtrl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem		= nullptr;

			BOOL bCheckAll = IsDlgButtonChecked(IDC_CHECK1);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->HasCheckbox(TRUE);
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)pColumn);

			if (_tcsicmp(pTable->Name(), TABLE_NODE) == 0)
			{
				if (_tcsicmp(pColumn->Name(), COLUMN_NODE_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0)
				{
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem->SetChecked(TRUE);
				}
				else
				{
					pItem->SetChecked(bCheckAll);
				}
			}
			else if (_tcsicmp(pTable->Name(), TABLE_LINK) == 0)
			{
				if (_tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_LINK_TYPE) == 0 
					|| _tcsicmp(pColumn->Name(), COLUMN_FNODE_ID) == 0 || _tcsicmp(pColumn->Name(), COLUMN_TNODE_ID) == 0)
				{
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem->SetChecked(TRUE);
				}
				else
				{
					pItem->SetChecked(bCheckAll);
				}
			}
			else if (_tcsicmp(pTable->Name(), TABLE_ZONE) == 0)
			{
				if (_tcsicmp(pColumn->Name(), COLUMN_ZONE_ID) == 0)
				{
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem->SetChecked(TRUE);
				}
				else
				{
					pItem->SetChecked(bCheckAll);
				}
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pColumn->Caption()));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			CString strDataType(_T(""));
			switch(pColumn->DataType())
			{
			case KEMIODataTypeInteger : strDataType = DATATYPESTRING_INTEGER; break;
			case KEMIODataTypeDouble  : strDataType = DATATYPESTRING_DOUBLE; break;
			case KEMIODataTypeString  : strDataType = DATATYPESTRING_STRING; break;
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDataType));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
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

	m_wndReportCtrl.Populate();
}


void KShapeExportDlg::OnCbnSelchangeComboTable()
{
	try
	{
		UpdateReportData();
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


void KShapeExportDlg::OnBnClickedCheckAll()
{
	try
	{
		BOOL bCheckAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == TRUE)
		{
			bCheckAll = TRUE;
		}

		CXTPReportRecords*   pRecords = m_wndReportCtrl.GetRecords();
		int nRecordCount   = pRecords->GetCount();

		for (int i= 0; i< nRecordCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(0);
			KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
			if (true == pColumn->PrimaryKey() || pItem->IsEditable() == FALSE)
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(bCheckAll);
			}
		}
		m_wndReportCtrl.Populate();
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

void KShapeExportDlg::OnBnClickedOk()
{
	CString strMsg(_T("Error"));
	try
	{
		GetColumns();
		if (SaveShapeFile(strMsg) == false)
		{
			throw -1;
		}

		ExportMatchingColumn();
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(strMsg);
		return;
	}

	KResizeDialogEx::OnOK();
}

void KShapeExportDlg::GetColumns()
{
	m_oSelectColumnColletion.Clear();

	try
	{
		CXTPReportRecords*   pRecords = m_wndReportCtrl.GetRecords();
		int nRecordCount   = pRecords->GetCount();

		for (int i= 0; i< nRecordCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(0);
			if (TRUE == pItem->IsChecked())
			{
				KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
				m_oSelectColumnColletion.AddNewColumn(pColumn);
			}
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

bool KShapeExportDlg::SaveShapeFile(CString& strMsg)
{
	try
	{
		KIOTable* pTable = GetSelectedIOTable();
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicDonkeyProgress oDonKeyProgress(this, QBicDonkeyProgress::DEFAULT_TITLE, TRUE, FALSE, FALSE, nLang);

		CString strFullPath;
		CString strFile;
		CString strPath;
		CString strFileName; {
#pragma region Get File 정보
			m_editFile.GetWindowText( strFullPath );

			strFile = strFullPath.Right(strFullPath.GetLength() - strFullPath.ReverseFind('\\') -1);
			strPath = strFullPath.Left(strFullPath.ReverseFind('\\'));
			strFileName = strFile.Left(strFile.ReverseFind('.'));

			if (strFile == _T("") || strPath == _T("") || strFileName == _T(""))
			{
				strMsg.Format(_T("선택한 파일이 없습니다."));
				throw -1;
			}
#pragma endregion Get File 정보
		}

		std::vector<DbfFieldInfo> vecFieldInfo; {
#pragma region Get vecFieldInfo 정보
			int nCount = m_oSelectColumnColletion.ColumnCount();
			for (int i= 0; i< nCount; i++) 
			{
				KIOColumn* pColumn       = m_oSelectColumnColletion.GetColumn(i);
				CString    strColumnName = pColumn->Name();
				if (strColumnName.GetLength() > 10)	{
					strColumnName = strColumnName.Left(10);
				}

				int nFieldType(0); // (단 ID값에 이용되는 Integer 같은 형식은 큰값이라, string인 FTString으로 사용해야 함)
				KEMIODataType emIODataType = pColumn->DataType();
				if (KEMIODataTypeInteger == emIODataType) {
					nFieldType = FTString; // FTInteger -> FTString
				} else if (KEMIODataTypeDouble == emIODataType) {
					nFieldType = FTDouble;
				} else { // KEMIODataTypeString 
					nFieldType = FTString;
				}

				DbfFieldInfo oSelFieldInfo;
				oSelFieldInfo.m_strFieldName = strColumnName;
				oSelFieldInfo.m_nFiledType   = nFieldType;
				//oSelFieldInfo.m_nWidth 는 디폴트로..
				if (FTDouble == nFieldType) {
					oSelFieldInfo.m_nDecimals = 3;
				}

				vecFieldInfo.push_back(oSelFieldInfo);
			}
#pragma endregion Get vecFieldInfo 정보
		}

		TxFeatureLayerPtr spFeatureLayer = nullptr; {
#pragma region Get FeatureLayer 정보
			try 
			{
				LoadLayerInfo(m_pTarget, pTable, spFeatureLayer);

				if (nullptr == spFeatureLayer)
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
				strMsg = strError;
				throw -1;
			} catch (KExceptionPtr ex) {
				TxExceptionPrint(ex);
				strMsg = _T("선택한 필드의 레이어가 존재하지 않습니다.");
				throw ex;
			} catch (...) {
				TxLogDebugException();
				strMsg = _T("선택한 필드의 레이어가 존재하지 않습니다.");
				throw -1;
			}
#pragma endregion Get FeatureLayer 정보
		}

		ShapeFileWriter oShpWrite;

		const int nGeoType = spFeatureLayer->LayerInfo()->GeoType();
		if (GeoPointType == nGeoType) {
			if ( oShpWrite.CreateShapeFile(strPath, strFileName, 1, vecFieldInfo) != 1 ) {
				strMsg = _T("Shape 파일 생성중 오류가 발생하였습니다.");
				throw -1;
			}
		} else if (GeoPolylineType == nGeoType) {
			if ( oShpWrite.CreateShapeFile(strPath, strFileName, 3, vecFieldInfo) != 1 ) {
				strMsg = _T("Shape 파일 생성중 오류가 발생하였습니다.");
				throw -1;
			}
		} else if (GeoPolygonType == nGeoType || GeoMPolygonType == nGeoType) {
			if ( oShpWrite.CreateShapeFile(strPath, strFileName, 5, vecFieldInfo) != 1 ) {
				strMsg = _T("Shape 파일 생성중 오류가 발생하였습니다.");
				throw -1;
			}
		} else {
			strMsg = _T("포인트, 폴리라인, 폴리곤 타입이외는 현재 지원하지 않습니다.");
			throw -1;
		}

		std::map<Integer, std::vector<CString>> mapInsertValue;
		GetInsertValues(pTable, mapInsertValue);

		std::map<Integer, ITxGeometryPtr> mapFeature;
		spFeatureLayer->GetGeometry(mapFeature);

		if (oShpWrite.Open(strPath, strFileName) == false) {
			strMsg = _T("Shape Open Error");
			throw -1;
		}

		int nFeatureCnt = (int)mapFeature.size();
		int nWritingCnt(1);

		auto iter  = mapFeature.begin();
		auto itEnd = mapFeature.end();
		for (; iter != itEnd; ++iter, nWritingCnt++)
		{
			const Integer  &nxID      = iter->first;
			ITxGeometryPtr spGeometry = iter->second;

			if (nullptr == spGeometry)
				continue;

			auto itFind = mapInsertValue.find(nxID);
			if (itFind == mapInsertValue.end())
				continue;

			std::vector<DbfFieldValue> vecFieldValue; {
				const std::vector<CString> &vecRecord = itFind->second;
				int nSizeRecord = (int)vecRecord.size();
				for (int i= 0; i< nSizeRecord; i++)
				{
					DbfFieldValue oDbfFieldValue;
					oDbfFieldValue.m_nFieldIndex = i;
					oDbfFieldValue.m_strValue    = vecRecord[i];

					vecFieldValue.push_back(oDbfFieldValue);
				}
			}

			oShpWrite.AppendShape(spGeometry, vecFieldValue);

			if (nWritingCnt % 1000 == 0)
			{
				CString strMsg(_T(""));
				strMsg.Format(_T("%d / %d 데이터 저장중 입니다. "), nWritingCnt, nFeatureCnt);

				oDonKeyProgress.SendTextEvent(strMsg);
			}
		}

		oDonKeyProgress.SendTextEvent(_T("데이터 저장 중.."));
		oShpWrite.Close();
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

KIOTable* KShapeExportDlg::GetSelectedIOTable()
{
	KIOTable* pIOTable = nullptr;

	int nCurSel = m_cboTable.GetCurSel();
	pIOTable = (KIOTable*)m_cboTable.GetItemData(nCurSel);

	return pIOTable;
}

void KShapeExportDlg::LoadLayerInfo(KTarget* a_pTarget, KIOTable* a_pTable, TxFeatureLayerPtr &spFeatureLayer)
{
	try 
	{
		KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(a_pTarget);
		if (pCreatedMapView != nullptr) { // MapView가 생성되어 있는지 확인

			ITxFeatureLayerPtr spLayer = nullptr; {
				CString strTableName = a_pTable->Name();
				if (strTableName.CompareNoCase(TABLE_NODE) == 0) {
					spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Node());
				} else if (strTableName.CompareNoCase(TABLE_LINK) == 0) {
					spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Link());
				} else if (strTableName.CompareNoCase(TABLE_ZONE) == 0) {
					spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Zone());
				}
			}
			
			if (spLayer != nullptr) {
				spFeatureLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}
		} else { // MapView가 생성 되어 있지 않으면 서버에 직접 접속
			CString strServerPath = a_pTarget->GetIODataFilePath();
			ITxMapServerPtr spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, strServerPath));
			if ( spMapServer->Connect() != 1)  
				throw 1;

			// 서버의 레이어 정보 조회
			std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
			for (size_t j=0; j<vecLayerInfo.size(); j++) {
				TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

				if (_tcsicmp(a_pTable->Name(), spLayerInfo->Name()) == 0) { // 레이어 조회
					spFeatureLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
					break;
				}
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
		throw 1;
	}
}

void KShapeExportDlg::GetInsertValues(KIOTable* a_pTable, std::map<Integer, std::vector<CString>> &a_mapInsertValue)
{
	a_mapInsertValue.clear();
	int nPKIndex(0);
	CString strSQL(_T(""));
	CString strColumnSQL(_T(""));

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		int nCount = m_oSelectColumnColletion.ColumnCount();
		for (int i = 0; i < nCount; i++)
		{
			KIOColumn* pColumn = m_oSelectColumnColletion.GetColumn(i);

			if (i == 0)
			{
				strColumnSQL.Format(_T(" %s "), pColumn->Name());
			}
			else
			{
				strColumnSQL.AppendFormat(_T(", %s "), pColumn->Name());
			}

			if (_tcsicmp(a_pTable->Name(), TABLE_NODE)== 0 && _tcsicmp(pColumn->Name(), COLUMN_NODE_ID) == 0)
			{
				nPKIndex = i;
			}
			else if (_tcsicmp(a_pTable->Name(), TABLE_LINK) == 0 && _tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0)
			{
				nPKIndex = i;
			}
			else if (_tcsicmp(a_pTable->Name(), TABLE_ZONE) == 0 && _tcsicmp(pColumn->Name(), COLUMN_ZONE_ID) == 0)
			{
				nPKIndex = i;
			}
		}

		strSQL.Format(_T("Select %s From %s "),strColumnSQL, a_pTable->Name());
		
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			Integer nxID(0);
			std::vector<CString> vecValues;

			for (int i = 0; i < nCount; i++)
			{
				CString strValue = spResultset->GetValueString(i);
				if (i == nPKIndex)
				{
					nxID = _ttoi64(strValue);
				}
				vecValues.push_back(strValue);
			}

			a_mapInsertValue.insert(std::make_pair(nxID, vecValues));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KShapeExportDlg::ResizeComponet()
{
	try
	{
		SetResize(IDC_STATIC_EXPORT,           SZ_TOP_LEFT,      SZ_TOP_LEFT);
		SetResize(IDC_FILENAME,             SZ_TOP_LEFT,      SZ_TOP_RIGHT);
		
		SetResize(IDC_STATIC_TABLE,             SZ_TOP_LEFT,      SZ_TOP_LEFT);
		SetResize(IDC_COMBO_TABLE,             SZ_TOP_LEFT,      SZ_TOP_LEFT);

		SetResize(IDC_CHECK1,             SZ_TOP_LEFT,      SZ_TOP_LEFT);

		SetResize(IDC_REPORT,             SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC_HOR1,             SZ_BOTTOM_LEFT,      SZ_BOTTOM_RIGHT);

		SetResize(IDOK,             SZ_BOTTOM_RIGHT,      SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,             SZ_BOTTOM_RIGHT,      SZ_BOTTOM_RIGHT);
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

void KShapeExportDlg::ExportMatchingColumn()
{
	try
	{
		CString strFullPath;
		CString strFile;
		CString strPath;
		CString strFileName;

		m_editFile.GetWindowText( strFullPath );

		strFileName = strFullPath.Left(strFullPath.ReverseFind('.'));
		strFileName = strFileName + _T(".txt");

		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(strFileName, nOpenMode);

		CString strOut(_T(""));
		strOut.Format(_T("Shape_Filed_Name \t Matting_Field_Name \r\n"));
		of.WriteString(strOut);

		int nCount = m_oSelectColumnColletion.ColumnCount();
		for (int i = 0; i < nCount; i++)
		{
			KIOColumn* pColumn = m_oSelectColumnColletion.GetColumn(i);
			strOut.Format(_T("%s \t %s \r\n"), pColumn->Name(), pColumn->Caption());
			of.WriteString(strOut);
		}

		of.Close();
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
