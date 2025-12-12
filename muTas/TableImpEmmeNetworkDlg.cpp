// TempEmmeNetworkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableImpEmmeNetworkDlg.h"
#include "afxdialogex.h"

#include "Target.h"

#include "ImChampFrameWindow.h"
#include "NetworkShapeInsert.h"

#include "DefVDFFactorDlg.h"
#include "DefineSystemCodeDlg.h"
#include "TableRelationInfo.h"
#include "InitRelationProcess.h"
#include "ImportCommon.h"
#include "DBaseInterModal.h"

// KTableImpEmmeNetworkDlg 대화 상자입니다.

const CString IMPORT_NODE_FILE_NAME  = _T("importNodeData.dat");
const CString IMPORT_LINK_FILE_NAME  = _T("importLinkData.dat");

IMPLEMENT_DYNAMIC(KTableImpEmmeNetworkDlg, KResizeDialogEx)

KTableImpEmmeNetworkDlg::KTableImpEmmeNetworkDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableImpEmmeNetworkDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_strImpFile(_T(""))
	, m_nPage(0)
	, m_nStatus(0)
	, m_oNodeSubDlg(this)
	, m_oLinkSubDlg(this)
	, m_pNodeColumns(NULL)
	, m_pLinkColumns(NULL)
	, m_pVDFColumns(nullptr)
	, m_bResult(false)
	, m_nNodeDataCount(0)
	, m_nLinkDataCount(0)
	, m_cSeparator(_T(' '))
	, m_nStartLinkID(1)
{

}

KTableImpEmmeNetworkDlg::~KTableImpEmmeNetworkDlg()
{
	if (m_pNodeColumns != NULL)
	{
		delete m_pNodeColumns;
		m_pNodeColumns = NULL;
	}

	if (m_pLinkColumns != NULL)
	{
		delete m_pLinkColumns;
		m_pLinkColumns = NULL;
	}

	if (m_pVDFColumns != nullptr)
	{
		delete m_pVDFColumns;
		m_pVDFColumns = nullptr;
	}
}

void KTableImpEmmeNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KTableImpEmmeNetworkDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KTableImpEmmeNetworkDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_NEXT, &KTableImpEmmeNetworkDlg::OnBnClickedNext)
	ON_BN_CLICKED(ID_BACK, &KTableImpEmmeNetworkDlg::OnBnClickedBack)
	ON_BN_CLICKED(IDOK, &KTableImpEmmeNetworkDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// KTableImpEmmeNetworkDlg 메시지 처리기입니다.

int KTableImpEmmeNetworkDlg::GetStatus( void )
{
	return m_nStatus;
}

void KTableImpEmmeNetworkDlg::SetImportFile( CString a_strImpFile )
{
	m_strImpFile = a_strImpFile;
}

BOOL KTableImpEmmeNetworkDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWnd* pWnd = GetDlgItem(IDC_FRAME);
	CRect oRect;
	pWnd->GetWindowRect(&oRect);
	ScreenToClient(&oRect);

	KIOTables* pIOTables  = m_pTarget->Tables();
	KIOTable*  pNodeTable = pIOTables->FindTable(TABLE_NODE);
	KIOTable*  pLinkTable = pIOTables->FindTable(TABLE_LINK);

	m_oNodeSubDlg.Create(IDD_4167_TableImpEmmeNodeSubDlg, this);
	m_oNodeSubDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_oNodeSubDlg.SetWindowPos(
		NULL, oRect.left, oRect.top, oRect.Width(), oRect.Height(), SWP_HIDEWINDOW);
	m_oNodeSubDlg.SetImportFile(m_strImpFile);
	m_oNodeSubDlg.SetTable(pNodeTable);

	m_oLinkSubDlg.Create(IDD_4167_TableImpEmmeLinkSubDlg, this);
	m_oLinkSubDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_oLinkSubDlg.SetWindowPos(
		NULL, oRect.left, oRect.top, oRect.Width(), oRect.Height(), SWP_HIDEWINDOW);
	m_oLinkSubDlg.SetImportFile(m_strImpFile);
	m_oLinkSubDlg.SetTable(pLinkTable);

	m_oNodeSubDlg.ShowWindow(SW_HIDE);
	m_oLinkSubDlg.ShowWindow(SW_HIDE);

	UpdateBtnState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableImpEmmeNetworkDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}


void KTableImpEmmeNetworkDlg::UpdateBtnState( void )
{
	if (m_nPage == -1)
	{
		m_nStatus = 2;
		KResizeDialogEx::OnOK();
	}
	else if (m_nPage == 0)
	{
		m_oNodeSubDlg.ShowWindow(SW_SHOW);
		m_oLinkSubDlg.ShowWindow(SW_HIDE);

		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_NEXT)->ShowWindow(SW_SHOW);
	}
	else if(m_nPage == 1)
	{
		m_oNodeSubDlg.ShowWindow(SW_HIDE);
		m_oLinkSubDlg.ShowWindow(SW_SHOW);

		GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
		GetDlgItem(ID_NEXT)->ShowWindow(SW_HIDE);
	}
}


void KTableImpEmmeNetworkDlg::OnBnClickedNext()
{
	++m_nPage;
	UpdateBtnState();
}


void KTableImpEmmeNetworkDlg::OnBnClickedBack()
{
	--m_nPage;
	UpdateBtnState();
}


void KTableImpEmmeNetworkDlg::OnBnClickedOk()
{
	if (theApp.IsFreeVersion())
	{
		if (m_oNodeSubDlg.GetNodeCount() > CKmzApp::MAX_NODE_OF_FREEVERSION)
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

	try
	{
		m_nStartLinkID = m_oLinkSubDlg.GetStartLinkID();

		if (m_nStartLinkID == 0)
			ThrowException(_T("시작 ID값은 0이 될 수 없습니다."));

		if (!NotifyTableChange())
			return;

		InitNodeFixColumn();
		InitLinkFixColumn();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(BinaryWriteThreadCaller, this, false, nLang);

		if (!m_bResult)
		{
			TxLogDebugException();
			ThrowException(m_strErrMsg);
		}

		KDefVDFFactorDlg oDefDlg(m_pTarget);
		oDefDlg.SetUserColumns(m_pVDFColumns);
		oDefDlg.FnID(m_setFnID);

		oDefDlg.DoModal();

		QBicSimpleProgressThread::ExecuteThread(ImpEmmeThreadCaller, this, false, nLang);

		if (!m_bResult)
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("Insert/Update/Add 실패 : 실패내역은 %s파일에서 확인하세요"), IMPORT_ERROR_FILE_NAME );
			ThrowException(strMsg);
		}

		oDefDlg.Apply();

		KDefineSystemCodeDlg oSystemDefineDlg(m_pTarget, true);
		oSystemDefineDlg.DoModal();

		try
		{
			KImportCommon::NotifyNodeRelation(m_pTarget);

			// Taget_region 코드가 레코드 내에 존재한다면, 코드메니저에 등록하자
			// 주의 사항은 NotifyNodeRelation() 이후에 해야 한다.
			KDBaseInterModal::AddTargetRegionCode(m_pTarget);
		} catch (int& ex) {
			CString strError(_T(""));
			strError.Format(_T("Error : %d"), ex);
			TxLogDebug((LPCTSTR)strError);
		} catch (KExceptionPtr ex) {
			TxExceptionPrint(ex);
		} catch (...) {
			TxLogDebugException();
		}

		KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(m_pTarget);
		if (nullptr != pCreatedMapView) {
			pCreatedMapView->ReloadRender(false, 0);
			pCreatedMapView->FullExtentMap();
		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}

	KResizeDialogEx::OnCancel();
}

bool KTableImpEmmeNetworkDlg::NotifyTableChange( void )
{
	bool bResult = false;

	CString strMsg = _T("데이터 불러오기 실행 후 기존 데이터가 모두 삭제됩니다.\n계속 진행하시겠습니까?");

	if ( IDYES == AfxMessageBox(strMsg, MB_YESNO) )
		bResult = true; 

	return bResult;
}

void KTableImpEmmeNetworkDlg::InitNodeFixColumn( void )
{
	if (m_pNodeColumns != NULL)
	{
		delete m_pNodeColumns;
		m_pNodeColumns = NULL;
	}

	m_pNodeColumns = new KIOColumns;


	KIOColumn* pAddColumn = nullptr;

	pAddColumn = m_pNodeColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_NODE_ID);
	pAddColumn->DataType(KEMIODataTypeInteger);

	pAddColumn = m_pNodeColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_NODE_TYPE);
	pAddColumn->DataType(KEMIODataTypeInteger);

	pAddColumn = m_pNodeColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_NODE_X);
	pAddColumn->DataType(KEMIODataTypeDouble);

	pAddColumn = m_pNodeColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_NODE_Y);
	pAddColumn->DataType(KEMIODataTypeDouble);

	KIOColumn* pColumn = nullptr;

	if (m_oNodeSubDlg.m_cboUser1.GetCurSel() != 0)
	{
		pColumn = (KIOColumn*)m_oNodeSubDlg.m_cboUser1.GetItemData(m_oNodeSubDlg.m_cboUser1.GetCurSel());

		pAddColumn = m_pNodeColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());
	}

	if (m_oNodeSubDlg.m_cboUser2.GetCurSel() != 0)
	{
		pColumn = (KIOColumn*)m_oNodeSubDlg.m_cboUser2.GetItemData(m_oNodeSubDlg.m_cboUser2.GetCurSel());

		pAddColumn = m_pNodeColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());
	}

	if (m_oNodeSubDlg.m_cboUser3.GetCurSel() != 0)
	{
		pColumn = (KIOColumn*)m_oNodeSubDlg.m_cboUser3.GetItemData(m_oNodeSubDlg.m_cboUser3.GetCurSel());

		pAddColumn = m_pNodeColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());
	}
}

void KTableImpEmmeNetworkDlg::InitLinkFixColumn( void )
{
	if (m_pLinkColumns != NULL)
	{
		delete m_pLinkColumns;
		m_pLinkColumns = NULL;
	}

	m_pLinkColumns = new KIOColumns;

	//Define Fixed Colum
	KIOColumn* pAddColumn = nullptr;

	pAddColumn = m_pLinkColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_LINK_ID);
	pAddColumn->DataType(KEMIODataTypeInteger);

	pAddColumn = m_pLinkColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_FNODE_ID);
	pAddColumn->DataType(KEMIODataTypeInteger);

	pAddColumn = m_pLinkColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_TNODE_ID);
	pAddColumn->DataType(KEMIODataTypeInteger);

	pAddColumn = m_pLinkColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_LINK_LENGTH);
	pAddColumn->DataType(KEMIODataTypeDouble);

	pAddColumn = m_pLinkColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_LINK_TYPE);
	pAddColumn->DataType(KEMIODataTypeInteger);

	pAddColumn = m_pLinkColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_LINK_LANE);
	pAddColumn->DataType(KEMIODataTypeInteger);

	pAddColumn = m_pLinkColumns->AddNewColumn();
	pAddColumn->Name(COLUMN_LINK_FN_ID);
	pAddColumn->DataType(KEMIODataTypeInteger);

	if (m_pVDFColumns != NULL)
	{
		delete m_pVDFColumns;
		m_pVDFColumns = NULL;
	}

	m_pVDFColumns = new KIOColumns;

	KIOColumn* pColumn = nullptr;

	if (m_oLinkSubDlg.m_cboUser1.GetCurSel() != 0)
	{
		pColumn = (KIOColumn*)m_oLinkSubDlg.m_cboUser1.GetItemData(m_oLinkSubDlg.m_cboUser1.GetCurSel());

		pAddColumn = m_pLinkColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());

		pAddColumn = m_pVDFColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());
	}

	if (m_oLinkSubDlg.m_cboUser2.GetCurSel() != 0)
	{
		pColumn = (KIOColumn*)m_oLinkSubDlg.m_cboUser2.GetItemData(m_oLinkSubDlg.m_cboUser2.GetCurSel());

		pAddColumn = m_pLinkColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());

		pAddColumn = m_pVDFColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());
	}

	if (m_oLinkSubDlg.m_cboUser3.GetCurSel() != 0)
	{
		pColumn = (KIOColumn*)m_oLinkSubDlg.m_cboUser3.GetItemData(m_oLinkSubDlg.m_cboUser3.GetCurSel());

		pAddColumn = m_pLinkColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());

		pAddColumn = m_pVDFColumns->AddNewColumn();
		pAddColumn->Name(pColumn->Name());
		pAddColumn->DataType(pColumn->DataType());
	}
}

void KTableImpEmmeNetworkDlg::LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spNodeLayer, TxFeatureLayerPtr &spLinkLayer, TxFeatureLayerPtr &spZoneLayer, bool bInitExtent)
{
	try 
	{
		KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(a_pTarget);
		if (pCreatedMapView != nullptr) { // MapView가 생성되어 있는지 확인
			ITxFeatureLayerPtr spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Node());
			if (spLayer != nullptr) {
				spNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Link());
			if (spLayer != nullptr) {
				spLinkLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Zone());
			if (spLayer != nullptr) {
				spZoneLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spMapServer = spNodeLayer->GetServer();
		} else { // MapView가 생성 되어 있지 않으면 서버에 직접 접속
			CString strServerPath = a_pTarget->GetIODataFilePath();
			spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, strServerPath));
			if ( spMapServer->Connect() != 1)  
				throw 1;

			// 서버의 레이어 정보 조회
			std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
			for (size_t j=0; j<vecLayerInfo.size(); j++) {
				TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

				if (_tcsicmp(TABLE_NODE, spLayerInfo->Name()) == 0) { // 노드 레이어 조회
					spNodeLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
				} else if (_tcsicmp(TABLE_LINK, spLayerInfo->Name()) == 0) { // 링크 레이어 조회
					spLinkLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
				} else if (_tcsicmp(TABLE_ZONE, spLayerInfo->Name()) == 0) { // 존 레이어 조회
					spZoneLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
				}
			}
		}

		if (bInitExtent) {
			if (nullptr != spNodeLayer) {
				TxLayerInfoPtr    spLayerInfo    = spNodeLayer->LayerInfo();
				TxLayerInfoSQLPtr spLayerInfoSQL = std::dynamic_pointer_cast<TxLayerInfoSQL>(spLayerInfo);
				spLayerInfoSQL->SetExtent(0.0,0.0,0.0,0.0); // 초기화
			}

			if (nullptr != spLinkLayer) {
				TxLayerInfoPtr    spLayerInfo    = spLinkLayer->LayerInfo();
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
		ThrowException(_T("Layer 조회 실패")); 
	}
}

void KTableImpEmmeNetworkDlg::Run( void )
{
	m_bResult = false;

	KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
	KImportCommon::DeleteNodeRelation(spDBase); // 트랜잭션을 활용할 수가 없다. (spDBase와 spMapServer는 같은 곳을 바라보고 있기 때문)

	ITxMapServerPtr   spMapServer = nullptr;
	TxFeatureLayerPtr spNodeFeatureLayer = nullptr; 
	TxFeatureLayerPtr spLinkFeatureLayer = nullptr; 
	TxFeatureLayerPtr spZoneFeatureLayer = nullptr; // 노드 Import는 존 정보도 Import 해야 한다.

	try 
	{
		LoadLayerInfo(m_pTarget, spMapServer, spNodeFeatureLayer, spLinkFeatureLayer, spZoneFeatureLayer);

		if (nullptr == spMapServer)
			throw 1;
		if (nullptr == spNodeFeatureLayer || nullptr == spLinkFeatureLayer || nullptr == spZoneFeatureLayer)
			throw 2;
	} catch (int& ex) {
		CString strError(_T(""));
		if (1 == ex) {
			strError.Format(_T("Error : %d (server 접속 실패)"), ex);
		} else {
			strError.Format(_T("Error : %d"), ex);
		}
		TxLogDebug((LPCTSTR)strError);
		return;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		return;
	} catch (...) {
		TxLogDebugException();
		return;
	}

	spMapServer->BeginTransaction();

	try
	{
		ImportNodeData(spNodeFeatureLayer, spZoneFeatureLayer);
		ImportLinkData(spLinkFeatureLayer);

		spMapServer->Commit();

		try
		{
 			if (nullptr != spLinkFeatureLayer){
				// RebuildIndex 수행시 트랙젠션 관계로, 외부의 트랙젝션이 끝난후 수행되야 한다.
				spLinkFeatureLayer->RebuildIndex();
			}

		} catch (...) {
			TxLogDebugException();
		}

		m_bResult = true;
	}
	catch (KExceptionPtr ex)
	{
		spMapServer->Rollback();
	}
	catch (...)
	{
		spMapServer->Rollback();
	}
}

unsigned __stdcall KTableImpEmmeNetworkDlg::ImpEmmeThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	KTableImpEmmeNetworkDlg* pDlg = (KTableImpEmmeNetworkDlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}

void KTableImpEmmeNetworkDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KTableImpEmmeNetworkDlg::Text2BinaryNode( void )
{
	TxLogDebugStartMsg();

	UINT nOpenMode        = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nOpenModeErrFile = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	UINT nCodePage        = 949;

	CString strTargetLocation = m_pTarget->GetLocation();
	CString strErrFileName    = strTargetLocation + _T("\\") + IMPORT_ERROR_FILE_NAME;

	remove((CStringA)IMPORT_NODE_FILE_NAME);

	CStdioFileEx  csvFile;
	std::ofstream ofs(IMPORT_NODE_FILE_NAME, std::ios::binary);
	CStdioFileEx  ofErr(strErrFileName, nOpenModeErrFile);

	csvFile.SetCodePage( nCodePage );

	if( csvFile.Open( m_strImpFile, nOpenMode ) == FALSE )
		ThrowException(_T("File Open Error"));

	int nCurrentLine(0);

	try
	{
		CString strReadLine(_T(""));
		bool    bIsData      = false;
		CString strTemp;

		//t init node 까지 고의로 Read
		while (!bIsData)
		{
			csvFile.ReadString(strReadLine);
			nCurrentLine++;

			strTemp = strReadLine;

			if ((strTemp.Trim()).CompareNoCase(_T("t nodes init")) == 0)
				bIsData = true;
		}

		while (csvFile.ReadString(strReadLine))
		{
			++nCurrentLine;
			strTemp = strReadLine;

			if ((strTemp.Trim()).CompareNoCase(_T("")) == 0)
			{
				m_strErrMsg.Format(_T("Line : %d =>\t 데이터 없음\r\n"), nCurrentLine);
				ofErr.WriteString(m_strErrMsg);

				continue;
			}

			//link 정보가 시작되면 break;
			if ((strReadLine.Trim()).CompareNoCase(_T("t links init")) == 0)
				break;

			RemoveDoubleSpace(strReadLine);

			CSVRow row;
			ParseCSVLineString( strReadLine, row );
			CSV2BinaryNode(nCurrentLine, row, ofs, ofErr);

			++m_nNodeDataCount;
		}

		ofs.close();
		csvFile.Close();
		ofErr.Close();

	}
	catch (KExceptionPtr ex)
	{
		ofs.close();
		csvFile.Close();
		ofErr.Close();

		throw ex;
	}
	catch (...)
	{
		ofs.close();
		csvFile.Close();
		ofErr.Close();

		throw 1;
	}
}

void KTableImpEmmeNetworkDlg::Text2BinaryLink( void )
{
	TxLogDebugStartMsg();

	UINT nOpenMode        = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nOpenModeErrFile = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	UINT nCodePage        = 949;

	CString strTargetLocation = m_pTarget->GetLocation();
	CString strErrFileName    = strTargetLocation + _T("\\") + IMPORT_ERROR_FILE_NAME;

	remove((CStringA)IMPORT_LINK_FILE_NAME);

	CStdioFileEx  csvFile;
	std::ofstream ofs(IMPORT_LINK_FILE_NAME, std::ios::binary);
	CStdioFileEx  ofErr(strErrFileName, nOpenModeErrFile);

	csvFile.SetCodePage( nCodePage );

	if( csvFile.Open( m_strImpFile, nOpenMode ) == FALSE )
		ThrowException(_T("File Open Error"));

	int nCurrentLine(0);
	m_nLinkDataCount = 0;
	try
	{
		CString strReadLine(_T(""));
		bool    bEverErrLine = false;
		bool    bIsData      = false;

		//t links init 까지 고의로 Read
		while (!bIsData)
		{
			csvFile.ReadString(strReadLine);
			nCurrentLine++;

			if ((strReadLine.Trim()).CompareNoCase(_T("t links init")) == 0)
				bIsData = true;
		}

		while (csvFile.ReadString(strReadLine))
		{
			++nCurrentLine;

			if ((strReadLine.Trim()).CompareNoCase(_T("")) == 0)
			{
				CString strErrorMsg(_T(""));
				strErrorMsg.Format(_T("Line : %d =>\t 데이터 없음\r\n"), nCurrentLine);

				ofErr.WriteString(strErrorMsg);

				continue;
			}

			RemoveDoubleSpace(strReadLine);

			CSVRow row;
			ParseCSVLineString(strReadLine, row);
			CSV2BinaryLink(nCurrentLine, row, ofs, ofErr);

			++m_nLinkDataCount;
		}

		if (bEverErrLine == true)
		{
			CString strErrorMsg(_T(""));
			strErrorMsg.Format(_T("Import 실패\r\n %s 파일에서 내역을 확인 하세요."), IMPORT_ERROR_FILE_NAME);

			ThrowException(strErrorMsg);
		}

		ofs.close();
		csvFile.Close();
		ofErr.Close();

	}
	catch (KExceptionPtr ex)
	{
		ofs.close();
		csvFile.Close();
		ofErr.Close();
		ThrowException(ex->GetErrorMessage());
	}
	catch (...)
	{
		ofs.close();
		csvFile.Close();
		ofErr.Close();
		throw 1;
	}
}

void KTableImpEmmeNetworkDlg::ParseCSVLineString( CString& strLine, CSVRow& row )
{
	bool bQuote = false;
	int nSub = 0;
	CString strRow, strQuote;

	while( AfxExtractSubString( strRow, strLine, nSub++, m_cSeparator ) == TRUE )
	{
		if( true == bQuote )
		{
			strQuote += strRow;
			if( strRow.GetAt( strRow.GetLength() - 1 ) == _T('"') )
			{
				strQuote.Remove( _T('"') );
				strRow = strQuote;
				bQuote = false;
				strQuote = _T("");
				row.push_back(strRow);
			}
		}
		else
		{
			TCHAR chFirst, chLast;
			if( strRow.IsEmpty() == TRUE )
			{
				row.push_back( strRow );
			}
			else if( strRow.GetLength() == 1 )
			{
				chFirst = strRow.GetAt( 0 );
				if( chFirst == _T('"') )
				{
					bQuote = true;
				}
				else
				{
					row.push_back(strRow);
				}
			}
			else
			{
				chFirst = strRow.GetAt( 0 );
				chLast = strRow.GetAt( strRow.GetLength() - 1 );

				if( (chFirst == _T('"')) && (chLast == _T('"')) )
				{
					strRow.Remove( _T('"') );
					row.push_back( strRow );
				}
				else if( chFirst == _T('"') )
				{
					bQuote = true;
					strQuote = strRow;
				}
				else
				{
					row.push_back( strRow );
				}
			}
		}
	}
}

void KTableImpEmmeNetworkDlg::CSV2BinaryNode( int a_nCurrentLine, CSVRow a_row, std::ofstream& ofs, CStdioFileEx& ofErr )
{
	try
	{
		int nColumns = m_pNodeColumns->ColumnCount();

		KEMIODataType emDataType;
		KIOColumn*    pColumn = nullptr;
		CString       strValue(_T(""));

		//#Column 0
		CString strType, strNodeID, strX, strY;

		strValue = a_row[0];
		if (strValue.CompareNoCase(_T("a*")) == 0)
		{
			strType = _T("0");
		}
		else
		{
			strType = _T("1");
		}

		//#Column 1
		strValue  = a_row[1];
		strNodeID = strValue;

		//#Column 2
		strValue = a_row[2];
		strX     = strValue;

		//#column 3
		strValue = a_row[3];
		strY     = strValue;

		//
		WriteOutputStream(strNodeID, KEMIODataTypeInteger, ofs);
		WriteOutputStream(strType,   KEMIODataTypeInteger, ofs);
		WriteOutputStream(strX,      KEMIODataTypeDouble,  ofs);
		WriteOutputStream(strY,      KEMIODataTypeDouble,  ofs);

		if (nColumns > 4)
		{
			for (int i = 4; i < nColumns; ++i)
			{
				pColumn    = m_pNodeColumns->GetColumn(i);
				emDataType = pColumn->DataType();
				strValue   = a_row[i];

				WriteOutputStream(strValue, emDataType, ofs);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		m_strErrMsg.Format(_T("Line : %d =>\t 오류\r\n"), a_nCurrentLine);
		ofErr.WriteString(m_strErrMsg);

		throw ex;
	}
	catch (...)
	{
		m_strErrMsg.Format(_T("Line : %d =>\t 오류\r\n"), a_nCurrentLine);
		ofErr.WriteString(m_strErrMsg);

		TxLogDebugException();
		ThrowException(_T("Failed Create Binary Data..."));
	}
}

void KTableImpEmmeNetworkDlg::WriteOutputStream( CString a_strImportValue, KEMIODataType a_emDataType, std::ofstream& ofs )
{
	try
	{
		double	dValue  = 0.0;
		Integer nxValue = 0;

		if(a_emDataType == KEMIODataTypeDouble)
		{
			dValue = _ttof(a_strImportValue);
			ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));
		}
		else if(a_emDataType == KEMIODataTypeInteger)
		{
			nxValue = _ttoi64(a_strImportValue);
			ofs.write(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
		}
		else if(a_emDataType == KEMIODataTypeString)
		{
			const int STR_LENGTH = 200;

			if(a_strImportValue.GetLength() > STR_LENGTH - 1)
				a_strImportValue.Truncate(STR_LENGTH - 1);

			TCHAR     szStrOut[STR_LENGTH];

			_tcscpy_s(szStrOut, a_strImportValue);
			ofs.write( reinterpret_cast<char*>( szStrOut  ), sizeof(TCHAR) * STR_LENGTH );
		}
	}
	catch (KExceptionPtr ex)
	{
		throw ex;
	}
	catch (...)
	{
		throw 1;
	}
}

void KTableImpEmmeNetworkDlg::CSV2BinaryLink( int a_nCurrentLine, CSVRow a_row, std::ofstream& ofs, CStdioFileEx& ofErr )
{
	try
	{
		int nColumns = m_pLinkColumns->ColumnCount();

		KEMIODataType emDataType;
		KIOColumn*    pColumn = nullptr;
		CString       strValue(_T(""));

		CString strType, strFNodeID, strTNodeID, strLength, strLanes, strVDFNo;
		//#Column 0 - Skip

		//#Column 1 - fNodeID
		strValue   = a_row[1];
		strFNodeID = strValue;

		//#Column 2 - tNodeID
		strValue   = a_row[2];
		strTNodeID = strValue;

		//#column 3
		strValue   = a_row[3];
		strLength  = strValue;

		//#column 4 - modes skip
		strValue   = a_row[4];

		//#column 5
		strValue   = a_row[5];
		strType    = strValue;

		//#column 6
		strValue   = a_row[6];
		strLanes   = strValue;

		//#column 7
		strValue   = a_row[7];
		strVDFNo   = strValue;

		m_setFnID.insert(_ttoi(strVDFNo));

		WriteOutputStream(strFNodeID, KEMIODataTypeInteger, ofs);
		WriteOutputStream(strTNodeID, KEMIODataTypeInteger, ofs);
		WriteOutputStream(strLength,  KEMIODataTypeDouble,  ofs);
		WriteOutputStream(strType,    KEMIODataTypeInteger, ofs);
		WriteOutputStream(strLanes,   KEMIODataTypeInteger, ofs);
		WriteOutputStream(strVDFNo,   KEMIODataTypeInteger, ofs);


		if (nColumns > 7)
		{
			for (int i = 7; i < nColumns; ++i)		
			{
				pColumn    = m_pLinkColumns->GetColumn(i);
				emDataType = pColumn->DataType();
				strValue   = a_row[i + 1];	//column define에는 없으나 데이터에는 update Code, modes 정보가 있으므로 +2 함/link_id때문에 또 -1 해서 총 1;

				WriteOutputStream(strValue, emDataType, ofs);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		CString strErrorMsg(_T(""));
		strErrorMsg.Format(_T("Line : %d =>\t 오류\r\n"), a_nCurrentLine);
		ofErr.WriteString(strErrorMsg);

		throw ex;
	}
	catch (...)
	{
		CString strErrorMsg(_T(""));
		strErrorMsg.Format(_T("Line : %d =>\t 오류\r\n"), a_nCurrentLine);
		ofErr.WriteString(strErrorMsg);

		TxLogDebugException();
		ThrowException(_T("Failed Create Binary Data..."));
	}
}

void KTableImpEmmeNetworkDlg::ImportNodeData(TxFeatureLayerPtr spNodeLayer, TxFeatureLayerPtr spZoneLayer)
{
	TxLogDebugStartMsg();
	m_mapNodeRecord.clear(); // ImportLinkData 에서도 사용됨

	std::ifstream ifs(IMPORT_NODE_FILE_NAME, std::ios::binary);

	try
	{
		CString strValue;
		Integer nxValue;
		double  dValue;

		Integer node_id(0);
		Integer node_type(-1);
		double  dX(0.0);
		double  dY(0.0);

		int nColumns = m_pNodeColumns->ColumnCount();
		int nLoopcnt = 1;

		while (!ifs.eof())
		{
			if (nLoopcnt++ % 500 == 0)
			{
				CString strMsg;
				strMsg.Format(_T("Updating Node data rows ( %d / %d )"), nLoopcnt, m_nNodeDataCount);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMsg.Format(_T("읽은 파일 라인수 / 총 라인수 :  ( %d / %d )"), nLoopcnt, m_nNodeDataCount);
				}
				AddStatusMessage( strMsg );
			}

			TxRecord oRecord;

			//#0 - nodeID
			ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
			node_id = nxValue;
			oRecord.AddItem(COLUMN_NODE_ID, node_id);

			if(ifs.eof())
				break;

			//#1 - node_type
			ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
			node_type = nxValue;
			oRecord.AddItem(COLUMN_NODE_TYPE, node_type);

			//#2 - x
			ifs.read(reinterpret_cast<char*>(&dValue), sizeof(double));
			dX = dValue;
			oRecord.AddItem(COLUMN_NODE_X, dX);

			//#3 - y
			ifs.read(reinterpret_cast<char*>(&dValue), sizeof(double));
			dY = dValue;
			oRecord.AddItem(COLUMN_NODE_Y, dY);

			if (nColumns > 4)
			{
				KIOColumn*    pColumn = nullptr;
				KEMIODataType emDataType;

				for (int i = 4; i < nColumns; ++i)
				{
					pColumn    = m_pNodeColumns->GetColumn(i);
					emDataType = pColumn->DataType();

					if (emDataType == KEMIODataTypeDouble)
					{
						ifs.read(reinterpret_cast<char*>(&dValue), sizeof(double));
						oRecord.AddItem(pColumn->Name(), dValue);
					}
					else if (emDataType == KEMIODataTypeInteger)
					{
						ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
						oRecord.AddItem(pColumn->Name(), nxValue);
					}
					else if (emDataType == KEMIODataTypeString)
					{
						const int STR_LENGTH = 200;
						TCHAR*  szStringIn = new TCHAR[STR_LENGTH];

						ifs.read( reinterpret_cast<char*>( szStringIn ), sizeof(TCHAR) * STR_LENGTH );

						strValue = szStringIn;
						delete szStringIn;

						oRecord.AddItem(pColumn->Name(), strValue);
					}
				}
			}

			TxPoint oPoint(dX, dY);
			ITxGeometryPtr spGeometry = TxPointPtr(new TxPoint(oPoint));
			spNodeLayer->Insert(node_id, spGeometry, oRecord);
			
			m_mapNodeRecord.insert(std::make_pair(node_id, oPoint));

			//import한 data중 type이 0인 경우는 존노드이므로 따로 정보를 관리하여 insert한다.
			if (node_type == 0) {
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
				spZoneLayer->Insert(node_id, spGeometry, oRecord);
			}
		}

		ifs.close();
	}
	catch (KExceptionPtr ex)
	{
		ifs.close();
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		ifs.close();
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}

void KTableImpEmmeNetworkDlg::ImportLinkData(TxFeatureLayerPtr spLinkLayer)
{
	TxLogDebugStartMsg();

	std::ifstream ifs(IMPORT_LINK_FILE_NAME, std::ios::binary);

	try
	{
		CString strValue;
		Integer nxValue;
		double  dValue;

		Integer link_id(0);
		Integer nxFNodeID(0);
		Integer nxTNodeID(0);

		int nColumns = m_pLinkColumns->ColumnCount();
		int nLoopCnt = 1;

		while (!ifs.eof())
		{
			if (nLoopCnt++ % 500 == 0)
			{
				CString strMsg;
				strMsg.Format(_T("Updating Link data rows ( %d / %d )"), nLoopCnt, m_nLinkDataCount);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMsg.Format(_T("읽은 파일 라인수 / 총 라인수 :  ( %d / %d )"), nLoopCnt, m_nLinkDataCount);
				}
				AddStatusMessage( strMsg );
			}

			TxRecord oRecord;

			//#0 - linkID
			link_id = m_nStartLinkID++;
			oRecord.AddItem(COLUMN_LINK_ID, link_id);

			//#1 - fNodeID
			ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
			nxFNodeID = nxValue;
			oRecord.AddItem(COLUMN_FNODE_ID, nxFNodeID);

			if(ifs.eof())
				break;

			if (m_mapNodeRecord.find(nxFNodeID) == m_mapNodeRecord.end())
				ThrowException(_T("Node Table에 정의되지 않은 ID입니다."));

			//#2 - tNodeID
			ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
			nxTNodeID = nxValue;
			oRecord.AddItem(COLUMN_TNODE_ID, nxTNodeID);

			if (m_mapNodeRecord.find(nxTNodeID) == m_mapNodeRecord.end())
				ThrowException(_T("Node Table에 정의되지 않은 ID입니다."));

			//#3 - length
			ifs.read(reinterpret_cast<char*>(&dValue), sizeof(double));
			oRecord.AddItem(COLUMN_LINK_LENGTH, dValue);

			//#4 - linkType
			ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
			oRecord.AddItem(COLUMN_LINK_TYPE, nxValue);

			//#5 - lanes
			ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
			oRecord.AddItem(COLUMN_LINK_LANE, nxValue);

			//#6 - vdfNo
			ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
			oRecord.AddItem(COLUMN_LINK_FN_ID, nxValue);

			if (nColumns > 7)
			{
				KIOColumn*    pColumn = nullptr;
				KEMIODataType emDataType;

				for (int i = 7; i < nColumns; ++i)
				{
					pColumn    = m_pLinkColumns->GetColumn(i);
					emDataType = pColumn->DataType();

					if (emDataType == KEMIODataTypeDouble)
					{
						ifs.read(reinterpret_cast<char*>(&dValue), sizeof(double));
						oRecord.AddItem(pColumn->Name(), dValue);
					}
					else if (emDataType == KEMIODataTypeInteger)
					{
						ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
						oRecord.AddItem(pColumn->Name(), nxValue);
					}
					else if (emDataType == KEMIODataTypeString)
					{
						const int STR_LENGTH = 200;
						TCHAR*  szStringIn = new TCHAR[STR_LENGTH];

						ifs.read( reinterpret_cast<char*>( szStringIn ), sizeof(TCHAR) * STR_LENGTH );

						strValue = szStringIn;
						delete szStringIn;

						oRecord.AddItem(pColumn->Name(), strValue);
					}
				}
			}

			std::vector<TxPoint> vecPt;
			auto itFind = m_mapNodeRecord.find(nxFNodeID);
			if (itFind == m_mapNodeRecord.end())
				continue;
			else
				vecPt.push_back(itFind->second);

			itFind = m_mapNodeRecord.find(nxTNodeID);
			if (itFind == m_mapNodeRecord.end())
				continue;
			else
				vecPt.push_back(itFind->second);

			ITxGeometryPtr spGeometry = TxPolylinePtr(new TxPolyline(vecPt));
			spLinkLayer->Insert(link_id, spGeometry, oRecord);
		}

		ifs.close();
	}
	catch (KExceptionPtr ex)
	{
		ifs.close();
		throw ex;
	}
	catch (...)
	{
		ifs.close();
		throw 1;
	}

	TxLogDebugEndMsg();
}

void KTableImpEmmeNetworkDlg::RemoveDoubleSpace( CString& strLine )
{
	try
	{
		CString strTempLine = strLine;
		int     nResult(1);
		while (nResult > 0)
		{
			nResult = strTempLine.Replace(_T("  "), _T(" ")); // '  ' -> ' ' 변환
		}

		strLine = strTempLine;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

unsigned __stdcall KTableImpEmmeNetworkDlg::BinaryWriteThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	KTableImpEmmeNetworkDlg* pDlg = (KTableImpEmmeNetworkDlg*)pParameter->GetParameter();
	pDlg->BinaryWrite();

	return 0;
}

void KTableImpEmmeNetworkDlg::BinaryWrite( void )
{
	m_bResult   = false;
	m_strErrMsg = _T("");
	m_setFnID.clear();

	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("파일을 불러오는 중..."));
		}
		else {
			AddStatusMessage(_T("Reading File..."));
		}

		Text2BinaryNode();
		Text2BinaryLink();

		m_bResult = true;
	}
	catch (KExceptionPtr ex)
	{
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		TxLogDebugException();
		m_strErrMsg = _T("Failed Create Binary Data...");
	}
}

