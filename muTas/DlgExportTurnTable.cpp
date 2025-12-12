#include "stdafx.h"
#include "KmzApp.h"
#include "DlgExportTurnTable.h"
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

IMPLEMENT_DYNAMIC(KDlgExportTurnTable, KWhiteDlgEx)

KDlgExportTurnTable::KDlgExportTurnTable(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KWhiteDlgEx(KDlgExportTurnTable::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pTable(a_pTable)
	, VIRTUAL_FIELD_FROMNODE(_T("F_Node_ID"))
	, VIRTUAL_FIELD_TONODE(_T("T_Node_ID"))
	, VIRTUAL_FIELD_FROMNODE_COLINFO(_T("From Node ID"))
	, VIRTUAL_FIELD_TONODE_COLINFO(_T("To Node ID"))
{
}

KDlgExportTurnTable::~KDlgExportTurnTable()
{
	KProject* pProject = ImChampFrameWindow::GetProject();

	m_editFile.SetDlgInitialDir(pProject->GetLocation());
}

BOOL KDlgExportTurnTable::OnInitDialog()
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
	InitComboBaseDataType();

	UpdateExportColumnInfo();

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDlgExportTurnTable::DoDataExchange(CDataExchange* pDX)
{
	KWhiteDlgEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,         m_ctrlReport);
	DDX_Control(pDX, IDC_FILENAME,       m_editFile );
	DDX_Control(pDX, IDC_COMBO_DATATYPE, m_cboDataType);
    DDX_Control(pDX, IDC_COMBO4,         m_cboEncoding);
}

BEGIN_MESSAGE_MAP(KDlgExportTurnTable, KWhiteDlgEx)
	ON_BN_CLICKED(IDOK,                   &KDlgExportTurnTable::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK3,             &KDlgExportTurnTable::OnBnClickedCheckSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KDlgExportTurnTable::OnReportCheckItem)
	ON_CBN_SELCHANGE(IDC_COMBO_DATATYPE,  &KDlgExportTurnTable::OnSelchangeComboBaseDataType)
END_MESSAGE_MAP()


void KDlgExportTurnTable::ResizeComponent()
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

void KDlgExportTurnTable::InitReportHeader()
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

void KDlgExportTurnTable::InitComboEncoding()
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

void KDlgExportTurnTable::InitComboBaseDataType( void )
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

void KDlgExportTurnTable::OnBnClickedOk()
{
	UpdateData( TRUE );

	CString strFile; {
		m_editFile.GetWindowText( strFile );
		if( strFile.IsEmpty() == true ) {
			AfxMessageBox( _T("파일명이 비어있습니다.") );
			return;
		}
	}

	BOOL bExportColumn; {
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bExportColumn = TRUE;
		else
			bExportColumn = FALSE;
	}

    int nEncoding = KEMCSVEncodingUTF8; {
        int nCur = m_cboEncoding.GetCurSel();
        if (nCur >= 0) {
            nEncoding = (int)m_cboEncoding.GetItemData(nCur);
        }
    }

	CString strSeparator; {
		int nSelRadioID = GetCheckedRadioButton(IDC_RADIO8, IDC_RADIO11);
		if (IDC_RADIO8 == nSelRadioID)
			strSeparator = _T(",");
		else if (IDC_RADIO9 == nSelRadioID)
			strSeparator = _T(";");
		else if (IDC_RADIO10 == nSelRadioID)
			strSeparator = _T("|");
		else if (IDC_RADIO11 == nSelRadioID)
			strSeparator = _T("\t");
		else
			return;
	}

	m_oExportColumns.Clear(); {
		CXTPReportRecords*	  pRecords = nullptr;
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		pRecords         = m_ctrlReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(0); 

			if (pItem->IsChecked()) {
				KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
				m_oExportColumns.AddNewColumn(pColumn);
			}
		}
	}

	try 
	{
		ThreadPara oPara(this);
		if (true) {
			oPara.TKeyInt32[0] = bExportColumn;   // First Header 여부
			oPara.TKeyInt32[1] = nEncoding;       // 인코딩 정보
			oPara.TKeyStr[0] = strFile;         // 파일path
			oPara.TKeyStr[1] = strSeparator;    // 구분자

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
			}
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("오류 : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		AfxMessageBox(strError);
		return;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
		return;
	} catch (...) {
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}

	OnOK();
}

unsigned __stdcall KDlgExportTurnTable::ThreadRun( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	if (pParameter != nullptr) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KDlgExportTurnTable* pDlg = (KDlgExportTurnTable*)pPara->TWindow;
			pDlg->Execute(pPara);
		}
	}

	return 1;
}

void KDlgExportTurnTable::Execute(ThreadPara* pPara)
{
	try 
	{
		ExportTable(pPara);
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
		RThreadInfo.SetErrorFlag(-1, _T("오류가 발생하였습니다."));
	}
}

void KDlgExportTurnTable::ExportTable( ThreadPara* pPara )
{
	TxLogDebugStartMsg();

	BOOL    bFirstHeader  = (BOOL)pPara->TKeyInt32[0]; // First Header 여부
	int     nEncoding     = (int)pPara->TKeyInt32[1]; // 인코딩 정보
	CString strFileName   = pPara->TKeyStr[0]; // 파일path
	CString strSeparator  = pPara->TKeyStr[1]; // 구분자

	QBicTxtWriter oWriter;
	if (oWriter.Open(strFileName, nEncoding) == false) {
		ThrowException(_T("파일을 열 수 없습니다."));
	}

	int nBaseType; {
		int nIndex = m_cboDataType.GetCurSel();
		nBaseType = (int)m_cboDataType.GetItemData(nIndex);
	}

	int nColumnCount = m_oExportColumns.ColumnCount();

	//transit export시, Node 선택시의 로직
	if (KEMBaseNode == nBaseType) {
		//write column header
		if(TRUE == bFirstHeader)	{
			CString strColumnLine = _T(""); {
				for(int i= 0; i< nColumnCount; i++) {
					KIOColumn* pColumn = m_oExportColumns.GetColumn(i);
					if(0 < i) {
						strColumnLine.Append(strSeparator);
					}
					strColumnLine.Append(_T("\""));

					CString strColumnName = pColumn->Name();
					if (strColumnName.CompareNoCase(COLUMN_TURN_F_LINK) == 0) {
						strColumnLine.Append(VIRTUAL_FIELD_FROMNODE);
					} else if (strColumnName.CompareNoCase(COLUMN_TURN_T_LINK) == 0) {
						strColumnLine.Append(VIRTUAL_FIELD_TONODE);
					} else {
						strColumnLine.Append(pColumn->Caption());
					}

					strColumnLine.Append(_T("\""));
				}
			}

			oWriter.Write(strColumnLine);
		}
		//write data
		ExportTransit2TxtTypeNode(strSeparator, oWriter);
	} else {
		//write Column header
		if(TRUE == bFirstHeader) {
			CString strColumnLine = _T("");
			for(int i= 0; i< nColumnCount; i++) {
				KIOColumn* pColumn = m_oExportColumns.GetColumn(i);
				if(0 < i) {
					strColumnLine.Append(strSeparator);
				}
				strColumnLine.Append(_T("\""));
				strColumnLine.Append(pColumn->Caption());
				strColumnLine.Append(_T("\""));
			}

			oWriter.Write(strColumnLine);
		}

		ExportDB2Txt(strSeparator, oWriter);
	}

	oWriter.Close();

	TxLogDebugEndMsg();
}

void KDlgExportTurnTable::ExportDB2Txt(LPCTSTR lpszSeparator, QBicTxtWriter &oWriter )
{
	try 
	{
		//Generate Query
		CString strQuery(_T(""));
		strQuery.Append(_T(" SELECT "));

		int nColumnCount = m_oExportColumns.ColumnCount();
		for(int i= 0; i< nColumnCount; i++) {
			KIOColumn* pColumn = m_oExportColumns.GetColumn(i);

			if(i != 0) {
				strQuery.Append(_T("||'"));
				strQuery.Append(lpszSeparator);
				strQuery.Append(_T("'||"));
			}

			if(KEMIODataTypeString == pColumn->DataType()) {
				strQuery.Append(_T("'\"'||"));
				strQuery.Append(_T("IFNULL("));
				strQuery.Append(pColumn->Name());
				strQuery.Append(_T(", '')"));
				strQuery.Append(_T("||'\"'"));
			} else {
				strQuery.Append(_T("IFNULL("));
				strQuery.Append(pColumn->Name());
				strQuery.Append(_T(", '0')"));
			}
		}
		strQuery.Append(_T(" From "));
		strQuery.Append(m_pTable->Name());

		if (m_strFilter.IsEmpty() == false) {
			strQuery.AppendFormat(_T(" Where %s "), m_strFilter);
		}

		TxLogDebug(strQuery.AllocSysString());

		//Write Text	
		KDBaseConPtr spDBase     = m_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet = spDBase->ExecuteQuery(strQuery);

		while (spResultSet->Next()) {
			CString strLine = spResultSet->GetValueString(0);
			oWriter.Write(strLine);
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

void KDlgExportTurnTable::ExportTransit2TxtTypeNode(LPCTSTR lpszSeparator, QBicTxtWriter &oWriter )
{
	try 
	{
		CString strQuery(_T(""));
		std::map<Integer, TFTNode> mapLinkNodeInfo;

		strQuery = _T(" select link_id, fnode_id, tnode_id from link ");

		KDBaseConPtr spDBase      = m_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet  = spDBase->ExecuteQuery(strQuery);

		TFTNode oFTNode = {};
		Integer nxLinkdID(0);

		while (spResultSet->Next()) {
			nxLinkdID         = spResultSet->GetValueInt64(0);
			oFTNode.fnode_id  = spResultSet->GetValueInt64(1);
			oFTNode.tnode_id  = spResultSet->GetValueInt64(2);

			mapLinkNodeInfo.insert(std::make_pair(nxLinkdID, oFTNode));
		}

		//쿼리 생성
		strQuery = _T(" SELECT ");
#pragma region 쿼리 생성
		int nColumnCount = m_oExportColumns.ColumnCount();
		for(int i= 0; i< nColumnCount; i++) {
			if (i > 0) {
				strQuery.Append(_T(", "));
			}
			KIOColumn* pColumn = m_oExportColumns.GetColumn(i);
			CString strColName = pColumn->Name();

			strQuery.AppendFormat(_T("%s "), strColName);
		}
		strQuery.AppendFormat(_T(" FROM %s "), TABLE_TURN);

		if (m_strFilter.IsEmpty() == false)
			strQuery.AppendFormat(_T(" Where %s"), m_strFilter);

		strQuery.Append(_T(" ORDER BY turn_id "));
#pragma endregion 쿼리 생성

		spResultSet = spDBase->ExecuteQuery(strQuery);

		while (spResultSet->Next()) {
			int nSeq(0);
			CString strWriteLine(_T(""));

			for (int i= 0; i< nColumnCount; i++) {
				if (0 != i) {
					strWriteLine.Append(lpszSeparator);
				}

				KIOColumn* pColumn     = m_oExportColumns.GetColumn(i);
				CString    strColName  = pColumn->Name();

				CString strData(_T(""));
				if (pColumn->DataType() == KEMIODataTypeDouble) {
					strData.Format(_T("%f"), spResultSet->GetValueDouble(i));
				} else if (pColumn->DataType() == KEMIODataTypeString) {
					strData.Format(_T("%s"), spResultSet->GetValueString(i));
				} else if (pColumn->DataType() == KEMIODataTypeInteger) {
					strData.Format(_T("%I64d"), spResultSet->GetValueInt64(i));
				}

				if (strColName.CompareNoCase(COLUMN_TURN_F_LINK) == 0) {
				
					// From Link의 From Node 출력
					Integer nxFLinkID = spResultSet->GetValueInt64(i);
					
					TFTNode oFTNode = {}; {
						auto itFind = mapLinkNodeInfo.find(nxFLinkID);
						if (itFind != mapLinkNodeInfo.end())
							oFTNode = itFind->second;
					}
	
					CString strFNode(_T(""));
					strFNode.Format(_T("%I64d"), oFTNode.fnode_id);

					strWriteLine.Append(strFNode);
				} else if (strColName.CompareNoCase(COLUMN_TURN_T_LINK) == 0) {

					// To Link의 To Node 출력
					Integer nxTLinkID = spResultSet->GetValueInt64(i);

					TFTNode oFTNode = {}; {
						auto itFind = mapLinkNodeInfo.find(nxTLinkID);
						if (itFind != mapLinkNodeInfo.end())
							oFTNode = itFind->second;
					}

					CString strTNode(_T(""));
					strTNode.Format(_T("%I64d"), oFTNode.tnode_id);

					strWriteLine.Append(strTNode);
				} else {
					strWriteLine.Append(strData);
				}
			}
			
			oWriter.Write(strWriteLine);
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

void KDlgExportTurnTable::OnBnClickedCheckSelectAll()
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

void KDlgExportTurnTable::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
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

void KDlgExportTurnTable::UpdateExportColumnInfo( void )
{
	TxLogDebugStartMsg();

	m_ctrlReport.ResetContent();

	int nBaseType; {
		int nIndex = m_cboDataType.GetCurSel();
		nBaseType = (int)m_cboDataType.GetItemData(nIndex);
	}

	BOOL bSelectAll(TRUE);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	const KIOColumns* pColumns     = m_pTable->Columns();
	int               nColumnCount = pColumns->ColumnCount();

	for(int i= 0; i< nColumnCount; i++)	{
		KIOColumn* pColumn        = pColumns->GetColumn(i);
		CString    strFiledName   = pColumn->Name();
		CString    strDisplayName = pColumn->Caption();
		CString    strColumnInfo  = pColumn->ColumnInfo();
		CString    strDataType    = _T("");

		//Export Based를 Node, Link 따로 변경하는 경우에만 해당 로직을 탐
		if (strFiledName.CompareNoCase(COLUMN_TURN_F_LINK) == 0) {
			if (KEMBaseNode == nBaseType) {
				strDisplayName = VIRTUAL_FIELD_FROMNODE;
				strColumnInfo  = VIRTUAL_FIELD_FROMNODE_COLINFO;
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strColumnInfo = _T("진입노드 번호");
				}
			}
		} else if (strFiledName.CompareNoCase(COLUMN_TURN_T_LINK) == 0) {
			if (KEMBaseNode == nBaseType) {
				strDisplayName = VIRTUAL_FIELD_TONODE;
				strColumnInfo  = VIRTUAL_FIELD_TONODE_COLINFO;
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strColumnInfo = _T("진출노드 번호");
				}
			}
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

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strDisplayName));
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

void KDlgExportTurnTable::OnSelchangeComboBaseDataType()
{
	UpdateExportColumnInfo();
}