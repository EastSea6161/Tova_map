// ZoningIntegrationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ZoningIntegrationDlg.h"
#include "afxdialogex.h"

#include "ZoneSelectIntergrationDlg.h"
#include "Target.h"
#include "CustomReportRecordItem.h"
#include "MapView.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "NetworkShapeInsert.h"
#include "DefineNames.h"
#include "KBulkDBase2File.h"
#include "KBulkDBase.h"
#include "TDrawNodeInfo.h"
#include "ImChampMapDefine.h"

#include "Launcher.h"
#include "ImChampDir.h"

#include "ImChampFrameWindow.h"
#include "ZoningMultiScenarioSelectDlg.h"

#include "WriteCodeAgrument.h"


// User message define
#define UM_ORIGINZONEEDIT		WM_USER + 1001


#define FILE_TYPE_COUNT_ZONE 2
#define FILE_TYPE_COUNT_OD   3
#define NODE_TYPE_ZONE       0


#define CLASSIFICATION_CODE_INTEGRATION 2

enum
{
	_0_INTER_COLUMN_CHECK = 0,
	_1_INTER_COLUMN_ZONELIST,
	_2_INTER_COLUMN_MAP,
	_3_INTER_COLUMN_NAME
	//_4_INTER_COLUMN_NOTE		// 2020.02.11 - Han He : "Note"컬럼 삭제
};

enum
{
	_0_ORIGIN_COLUMN_NO = 0,
	_1_ORIGIN_COLUMN_ZONEID,
	_2_ORIGIN_COLUMN_NAME
};

// KZoningIntegrationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KZoningIntegrationDlg, KDialogEx)

bool KZoningIntegrationDlg::m_bRunIntegrationSucceess = false;

KZoningIntegrationDlg::KZoningIntegrationDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KZoningIntegrationDlg::IDD, pParent)
	, m_bAutoGenZoneConn(FALSE)
{
	m_pTarget = NULL;
	m_pRunningTarget = NULL;
	m_pMapView = NULL;
	m_nxTempMaxID = 0;
	m_nxSelectedTempID = 0;
}

KZoningIntegrationDlg::~KZoningIntegrationDlg()
{
	try
	{
		ClearDrawInfo();

		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*        pTable = pIOTables->FindTable(TABLE_ZONE);
		pTable->Unregister(this);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KZoningIntegrationDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_D, m_wndAAAZoneReportCtrl);
	DDX_Control(pDX, IDC_REPORT_O, m_wndBBBZoneReportCtrl);
	DDX_Check(pDX, IDC_CHECK1, m_bAutoGenZoneConn);
}


BEGIN_MESSAGE_MAP(KZoningIntegrationDlg, KDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, IDC_REPORT_D, &KZoningIntegrationDlg::OnIntegrationSelectChanged)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT_D, &KZoningIntegrationDlg::OnIntegrationItemButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT_D, &KZoningIntegrationDlg::OnIntegrationValueChanged)

	ON_NOTIFY(NM_CLICK, IDC_REPORT_O, &KZoningIntegrationDlg::OnReportItemClick)

	ON_BN_CLICKED(IDOK, &KZoningIntegrationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KZoningIntegrationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &KZoningIntegrationDlg::OnBnClickedBtnAddNewZone)
	ON_BN_CLICKED(IDC_BUTTON2, &KZoningIntegrationDlg::OnBnClickedBtnDelZone)
	ON_MESSAGE(UM_ORIGINZONEEDIT, &KZoningIntegrationDlg::OnOriginZoneEdit)

	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BTN_MULTI_SCENARIO, &KZoningIntegrationDlg::OnBnClickedBtnMultiScenario)
END_MESSAGE_MAP()


// KZoningIntegrationDlg 메시지 처리기입니다.

void KZoningIntegrationDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);

		if (strSubjectName.CompareNoCase(TABLE_ZONE) == 0)
		{
			InitializeNode();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KZoningIntegrationDlg::InitializeNode()
{
	CWaitCursor wc;

	m_mapZoneNodeData.clear();
	m_vecZoneNodeIDCaption.clear();

	GetZoneNodeInfo();
}


bool KZoningIntegrationDlg::GetZoneNodeInfo(void)
{
	TxLogDebugStartMsg();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format(_T(" SELECT zone_id, name FROM %s ORDER BY zone_id "), TABLE_ZONE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (nullptr == spResult)
		{
			return false;
		}

		m_mapZoneNodeData.clear();
		m_vecZoneNodeIDCaption.clear();

		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));

		while (spResult->Next())
		{
			nxNodeId = spResult->GetValueInt64(0);
			strNodeName = spResult->GetValueString(1);

			m_mapZoneNodeData.insert(std::make_pair(nxNodeId, strNodeName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxNodeId);
			oIDCaption.TID = strNodeID;
			oIDCaption.TCaption = strNodeName;

			m_vecZoneNodeIDCaption.push_back(oIDCaption);
		}
	}
	catch (...)
	{
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


void KZoningIntegrationDlg::ActivatedFrameWndNotifyProcess()
{
	if (nullptr != m_spImTasMultiIDFind)
	{
		if (!m_spImTasMultiIDFind->IsUserClosed())
		{
			if (!m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
		}
	}
}


void KZoningIntegrationDlg::DeActivatedFrameWndNotifyProcess()
{
	if (nullptr != m_spImTasMultiIDFind)
	{
		if (!m_spImTasMultiIDFind->IsUserClosed())
		{
			if (m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
		}
	}
}


BOOL KZoningIntegrationDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	KReportCtrlSetting::Default(m_wndAAAZoneReportCtrl, TRUE, FALSE, TRUE);
	KReportCtrlSetting::Default(m_wndBBBZoneReportCtrl, FALSE, FALSE, FALSE);

	InitReportHeader();

	if (!m_imageListSelectIcon.Create(16, 16, ILC_MASK | ILC_COLOR24, 0, 1))
	{
		AfxMessageBox(_T("Failed to create image list"));
		return -1;
	}

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_RADIO);
	m_imageListSelectIcon.Add(&bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0)));

	m_wndAAAZoneReportCtrl.SetImageList(&m_imageListSelectIcon);

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable = pIOTables->FindTable(TABLE_ZONE);
	pTable->Register(this);

	GetDlgItem(IDC_BTN_MULTI_SCENARIO)->EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KZoningIntegrationDlg::ModelessOKFeedback(UINT nDialogID)
{
	if (KImChampMultiIDFind::IDD == nDialogID)
	{
		if (m_spImTasMultiIDFind)
		{
			try
			{
				CWaitCursor cw;

				std::set<CString> setZoneList = m_spImTasMultiIDFind->GetSelectedIDSet();
				CString           strZoneList = m_spImTasMultiIDFind->GetSelectedID();

				AutoType iter = m_mapZoneIntegrationInfo.find(m_nxSelectedTempID);
				AutoType end = m_mapZoneIntegrationInfo.end();

				Integer tmpNewZID = 0;
				if (iter != end)
				{
					m_pRunningTarget = m_pTarget;
					GenerateNewZoneID();
					KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
					spZoningIntegrationZoneInfo->TIntegrationZoneList = strZoneList;
					tmpNewZID = spZoningIntegrationZoneInfo->TZoneID;
				}

				CString strTemp;

				if (tmpNewZID == 0) {
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strTemp = _T("집합화 할 존 노드 번호 : -");
					}
					else {
						strTemp = _T("Integrated New Zone [ID] including : -");
					}
				}
				else {
					if (KmzSystem::GetLanguage() == KEMKorea) {
						//strTemp.Format(_T("집합화 할 존 노드 번호 : %d"), setZoneList.size());
						strTemp.Format(_T("집합화 할 존 노드 번호 : %d"), tmpNewZID);
					}
					else {
						//strTemp.Format(_T("Integrated New Zone [ID] including : %d"), setZoneList.size());
						strTemp.Format(_T("Integrated New Zone [ID] including : %d"), tmpNewZID);
					}
				}
				SetDlgItemText(IDC_STATIC_Division, strTemp);

				DisplayBZone(m_nxSelectedTempID);

				// 2020.02.11 - Han He : "Note" 컬럼 삭제
				UpdateNote();

// 				CXTPReportRows* pRows = m_wndAAAZoneReportCtrl.GetRows();
// 				pRow = pRows->GetAt(nSelectedLineIndex);
// 				pRow->SetSelected(TRUE);

			}
			catch (...)
			{
				TxLogDebugException();
			}
		}
	}
}


void  KZoningIntegrationDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_0_INTER_COLUMN_CHECK, _T("선택"), 45, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	else {
		pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_0_INTER_COLUMN_CHECK, _T("Select"), 45, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}

	// 2020.02.11 - Han He : "OriginalZoneList"와 "New Name" 컬럼 위치 바꾸기
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_1_INTER_COLUMN_ZONELIST, _T("집합화 존 선택"), 77, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	else {
		pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_1_INTER_COLUMN_ZONELIST, _T("OriginalZoneList"), 87, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	/*pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_1_INTER_COLUMN_ZONELIST, _T("OriginalZoneList"), 77, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("집합화 존 선택"));
	}*/

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_2_INTER_COLUMN_MAP, _T("지정상태"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	else {
		pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_2_INTER_COLUMN_MAP, _T("Map"), 28));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	/*pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_2_INTER_COLUMN_MAP, _T("Map"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("지정상태"));
	}*/

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_3_INTER_COLUMN_NAME, _T("새 명칭"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	else {
		pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_3_INTER_COLUMN_NAME, _T("New Name"), 32));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	/*pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_3_INTER_COLUMN_NAME, _T("New Name"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("새 명칭"));
	}*/

	// 2020.02.11 - Han He : "Note" 컬럼 삭제
	/*pColumn = m_wndAAAZoneReportCtrl.AddColumn(new CXTPReportColumn(_4_INTER_COLUMN_NOTE,     _T("Note"),     30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("진행상태"));
	}*/

	// 	CXTPReportHeader* pHeader = m_wndAAAZoneReportCtrl.GetReportHeader();
	// 	pHeader->AllowColumnRemove(FALSE);
	// 	pHeader->AllowColumnReorder(FALSE);
	// 	pHeader->AllowColumnSort(FALSE);

	CXTPReportPaintManager* pPaintMgr = m_wndAAAZoneReportCtrl.GetPaintManager();
	pPaintMgr->m_strNoItems.Format(_T(""));
	// 2020.02.11 - Han He : 기존 메세지 삭제
	/*pPaintMgr->m_strNoItems.Format(_T("Please Click Add Button First."));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pPaintMgr->m_strNoItems.Format(_T("새 존 추가 버튼을 클릭해 주세요."));
	}*/

	// 	m_wndAAAZoneReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid); 
	// 	m_wndAAAZoneReportCtrl.AllowEdit(TRUE);
	// 	m_wndAAAZoneReportCtrl.EditOnClick(FALSE);
	// 	m_wndAAAZoneReportCtrl.SetMultipleSelection(FALSE);
	m_wndAAAZoneReportCtrl.Populate();
	//-----------------------------------------------------------------------------

	pColumn = m_wndBBBZoneReportCtrl.AddColumn(new CXTPReportColumn(_0_ORIGIN_COLUMN_NO, _T("No"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("번호"));
	}

	pColumn = m_wndBBBZoneReportCtrl.AddColumn(new CXTPReportColumn(_1_ORIGIN_COLUMN_ZONEID, _T("Zone ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("존 노드 ID"));
	}

	pColumn = m_wndBBBZoneReportCtrl.AddColumn(new CXTPReportColumn(_2_ORIGIN_COLUMN_NAME, _T("Name"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("존 명칭"));
	}

	pPaintMgr = m_wndBBBZoneReportCtrl.GetPaintManager();
	pPaintMgr->m_strNoItems.Format(_T(""));
	// 2020.02.11 - Han He : 기존 메세지 삭제
	//pPaintMgr->m_strNoItems.Format(_T("통합하고자 하는 존을 선택해 주세요"));

	m_wndBBBZoneReportCtrl.Populate();
}


void KZoningIntegrationDlg::ReceivedZoneInfoFromMap(TxPointPtr a_spNodeGeometry, TxPolygonPtr a_spDemarcationGeometry)
{
	if (m_wndAAAZoneReportCtrl.GetSelectedRows()->GetCount() == 0)
		return;

	CXTPReportRow* pIntegrationRow = m_wndAAAZoneReportCtrl.GetSelectedRows()->GetAt(0);

	int nSelectedLineIndex = pIntegrationRow->GetIndex();

	if (NULL == pIntegrationRow)
	{
		return;
	}

	CXTPReportRecord*     pIntegrationRecord = pIntegrationRow->GetRecord();
	CXTPReportRecordItem* pIntegrationItem = pIntegrationRecord->GetItem(_0_INTER_COLUMN_CHECK);
	CXTPReportRecordItem* pMapItem = pIntegrationRecord->GetItem(_2_INTER_COLUMN_MAP);

	Integer nxTempMaxID = (Integer)pIntegrationItem->GetItemData();

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.find(nxTempMaxID);
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	if (iter == end) return;

	std::vector<Integer> vecOriginID;
	if (iter != end)
	{
		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;

		spZoningIntegrationZoneInfo->TNodeGeometry = a_spNodeGeometry;
		spZoningIntegrationZoneInfo->TDemarcationGeometry = a_spDemarcationGeometry;

		pMapItem->SetCaption(_T("Add Complete"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pMapItem->SetCaption(_T("존지정(O)"));
		}
		UpdateNote();
		m_wndAAAZoneReportCtrl.RedrawControl();

		CXTPReportRows* pRows = m_wndAAAZoneReportCtrl.GetRows();
		pIntegrationRow = pRows->GetAt(nSelectedLineIndex);
		pIntegrationRow->SetSelected(TRUE);

		DrawAddedZone(spZoningIntegrationZoneInfo);
	}
}


void KZoningIntegrationDlg::DrawAddedZone(KZoneDivisionInfoPtr spZoneDivisionInfo)
{
	try
	{
		double  dX(0.0), dY(0.0);
		std::vector<TUserPointInfo> vecUserPoint;
		TPreferenceSymbol          &ZoneSymbol = ImChampMapDefineSymbol::ZoneSymbol;

		TxPointPtr spPoint = spZoneDivisionInfo->TNodeGeometry;

		if (nullptr == spPoint)
			return;

		dX = spPoint->X;
		dY = spPoint->Y;

		TUserPointInfo oTUserPointInfo;
		oTUserPointInfo.Tx = dX;
		oTUserPointInfo.Ty = dY;

		oTUserPointInfo.TSize = ZoneSymbol.dSize;
		oTUserPointInfo.TLabel = _T("");
		//oTUserPointInfo.TColorRGB       = ZoneSymbol.clrSymbol;
		oTUserPointInfo.TColorRGB = RGB(255, 128, 0);
		oTUserPointInfo.TMarker = ZoneSymbol.nCharIndex;

		vecUserPoint.push_back(oTUserPointInfo);

		m_pMapView->TxDrawCompositeUserPoint(vecUserPoint, false, true);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::OnIntegrationValueChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTRECORDITEM*    pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		CXTPReportRecord*           pRecord = pItemNotify->pItem->GetRecord();

		CXTPReportRecordItem*       pIntegrationItem = pRecord->GetItem(_0_INTER_COLUMN_CHECK);
		CXTPReportRecordItemText*   pItemName = (CXTPReportRecordItemText*)pRecord->GetItem(_3_INTER_COLUMN_NAME);

		Integer nxTempMaxID = (Integer)pIntegrationItem->GetItemData();

		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.find(nxTempMaxID);
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		if (iter == end) return;

		if (iter != end)
		{
			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
			spZoningIntegrationZoneInfo->TZoneName = pItemName->GetValue();
		}

		UpdateNote();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::OnBnClickedCancel()
{
	ClearDrawInfo();

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();

	KDialogEx::OnCancel();
}


void KZoningIntegrationDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_pRunningTarget = m_pTarget;
	m_nResultCode = DLL_UNEXPECTED_ERROR;

	CString strError;
	if (!InputCheck(strError))
	{
		AfxMessageBox(strError);
		return;
	}

	GenerateNewZoneID();

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(IntegrationZoneThreadCaller, this, false, nLang);

	if (DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{

	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("존 세분화/집합화가 중단되었습니다."));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("User stopped."));
		}
		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("Not Found Dll"));
		return;
	}
	else if (DLL_CREATE_ARGFILE_ERROR == m_nResultCode)
	{
		//AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occurred while creating the Argument file !"));
		}
		return;
	}
	else
	{
		//AfxMessageBox(_T("존 집합화 수행 중 오류가 발생하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("존 집합화 수행 중 오류가 발생하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occurred while executing !"));
		}
		return;
	}

	CWaitCursor wc;

	bool bTemp = ReadOutPutFile();
	if (!bTemp)
	{
		//AfxMessageBox(_T("집합화한 존 정보 입력에 실패하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("집합화한 존 정보 입력에 실패하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occurred while writing output data !"));
		}
		TxLogDebug(_T("Output Integration 정보 입력 실패..."));
		return;
	}

	if (!UpdateIntegrationSystemInfo())
	{
		//AfxMessageBox(_T("존 관련 정보 입력에 실패하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("존 관련 정보 입력에 실패하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occurred while writing output data !"));
		}
		return;
	}

	if (m_bRunIntegrationSucceess)
	{
		// 항목 리셋
		m_wndAAAZoneReportCtrl.ResetContent();
		m_wndBBBZoneReportCtrl.ResetContent();
		m_mapZoneIntegrationInfo.clear();

		if (KmzSystem::GetLanguage() == KEMKorea) {
			SetDlgItemText(IDC_STATIC_Division, _T("집합화 할 존 노드 번호 : -"));
		}
		else {
			SetDlgItemText(IDC_STATIC_Division, _T("Integrated New Zone [ID] including : -"));
		}

		ClearDrawInfo();
	}

	m_pMapView->RefreshAll();

	NotifyRelationTable();
}


bool KZoningIntegrationDlg::InputCheck(CString& ar_strError)
{
	CXTPReportRecords* pRecords = m_wndAAAZoneReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for (int i = 0; i < nRecordCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(_0_INTER_COLUMN_CHECK);
		//CXTPReportRecordItem* pItemNote = pRecord->GetItem(_4_INTER_COLUMN_NOTE);

		Integer nxTempMaxID = (Integer)pItem->GetItemData();
		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.find(nxTempMaxID);
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();
		if (iter == end) continue;

		switch (IsInputDone(iter->second))
		{
		case KEMNewInputErrorMap:
			//ar_strError.Format(_T("%I64d - 존지정(X)"), nxTempMaxID);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ar_strError.Format(_T("오류 %I64d : 새 존 위치를 지도에서 지정해주세요 !"), nxTempMaxID);
			}
			else //if (KmzSystem::GetLanguage() == KEMEnglish)
			{
				ar_strError.Format(_T("Error %I64d : Specify new zone location on the map !"), nxTempMaxID);
			}
			return false;
			break;
		case KEMNewInputErrorZoneList:
			//ar_strError.Format(_T("%I64d - 미입력(존)"), nxTempMaxID);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ar_strError.Format(_T("오류 %I64d : 집합화 할 존을 선택해주세요 !"), nxTempMaxID);
			}
			else //if (KmzSystem::GetLanguage() == KEMEnglish)
			{
				ar_strError.Format(_T("Error %I64d : Select original zone list !"), nxTempMaxID);
			}
			return false;
			break;
		case KEMNewInputErrorZoneShort:
			ar_strError.Format(_T("%I64d - 추가입력(존)"), nxTempMaxID);
			return false;
			break;
		default:
			break;
		}
	}

	if (nRecordCount == 0)
	{
		ar_strError = _T("Please Click Add Button First.");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ar_strError = _T("새 존 추가 버튼을 클릭하여 새로운 존을 추가해 주세요.");
		}
		return false;
	}

	return true;
}


void KZoningIntegrationDlg::GenerateNewZoneID()
{
	//Integer nxMaxPlusOneZoneID = KBulkDBaseNode::MaxNodeID(m_pTarget) + 1;
	Integer nxMaxPlusOneZoneID = KBulkDBaseNode::MaxNodeID(m_pRunningTarget) + 1;

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	while (iter != end)
	{
		KZoneDivisionInfoPtr spZoneDivisionInfo = iter->second;
		spZoneDivisionInfo->TZoneID = nxMaxPlusOneZoneID++;

		++iter;
	}
}

/**********************************************************/
/* 2019.01.18 수정 : 여러 시나리오 처리기능 추가에 따른 변경 */
//void KZoningIntegrationDlg::SaveGeometry()
//{
//	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
//	std::map<Integer, KZoneDivisionInfoPtr>::iterator end  = m_mapZoneIntegrationInfo.end();
//
//	std::set<Integer>::iterator itDelLink    = m_setDeleteLinkID.begin();
//	std::set<Integer>::iterator itDelLinkEnd = m_setDeleteLinkID.end();
//
//    TxFeatureLayerPtr  spNodeLayer; {
//        ITxFeatureLayerPtr spLayer = m_pMapView->MapGetFeatureLayer(KLayerID::Node());
//        spNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
//    }
//
//    TxFeatureLayerPtr  spLinkLayer; {
//        ITxFeatureLayerPtr spLayer = m_pMapView->MapGetFeatureLayer(KLayerID::Link());
//        spLinkLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
//    }
//
//    TxFeatureLayerPtr  spZoneLayer; {
//        ITxFeatureLayerPtr spLayer = m_pMapView->MapGetFeatureLayer(KLayerID::Zone());
//        spZoneLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
//    }  
//
//	try
//	{
//		while (itDelLink != itDelLinkEnd)
//		{
//			Integer nxLinkID = *itDelLink;
//			spLinkLayer->Delete(nxLinkID);// oNetworkImportTxt.DeleteLink(nxLinkID);
//
//			++itDelLink;
//		}
//
//		while (iter != end)
//		{
//			KZoneDivisionInfoPtr spZoneDivisionInfo = iter->second;
//
//			std::vector<Integer> vecOriginID;
//			ConvertZoneListToVector(spZoneDivisionInfo->TIntegrationZoneList, vecOriginID);
//
//			for(size_t i= 0; i< vecOriginID.size(); i++)
//			{
//				spZoneLayer->Delete(vecOriginID[i]); //oNetworkImportTxt.DeleteZone(vecOriginID[i]);
//				spNodeLayer->Delete(vecOriginID[i]); //oNetworkImportTxt.DeleteNode(vecOriginID[i]);
//			}
//			
//            spNodeLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TNodeGeometry);       // oNetworkImportTxt.InsertNode(spZoneDivisionInfo->TZoneID, 0, spZoneDivisionInfo->TNodeGeometry);
//            spZoneLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);// oNetworkImportTxt.InsertZone(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);
//
//			++iter;
//		}
//
//	}
//	catch (...)
//	{		
//		ThrowException(_T("Shape 입력 오류"));
//	}
//}

void KZoningIntegrationDlg::SaveGeometry(KMapView* pMapView)
{
	if (NULL == pMapView)
	{
		return;
	}

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	std::set<Integer>::iterator itDelLink = m_setDeleteLinkID.begin();
	std::set<Integer>::iterator itDelLinkEnd = m_setDeleteLinkID.end();

	TxFeatureLayerPtr  spNodeLayer; {
		ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Node());
		spNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
	}

	TxFeatureLayerPtr  spLinkLayer; {
		ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Link());
		spLinkLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
	}

	TxFeatureLayerPtr  spZoneLayer; {
		ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Zone());
		spZoneLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
	}

	try
	{
		while (itDelLink != itDelLinkEnd)
		{
			Integer nxLinkID = *itDelLink;
			spLinkLayer->Delete(nxLinkID);// oNetworkImportTxt.DeleteLink(nxLinkID);

			++itDelLink;
		}

		while (iter != end)
		{
			KZoneDivisionInfoPtr spZoneDivisionInfo = iter->second;

			std::vector<Integer> vecOriginID;
			ConvertZoneListToVector(spZoneDivisionInfo->TIntegrationZoneList, vecOriginID);

			for (size_t i = 0; i < vecOriginID.size(); i++)
			{
				spZoneLayer->Delete(vecOriginID[i]); //oNetworkImportTxt.DeleteZone(vecOriginID[i]);
				spNodeLayer->Delete(vecOriginID[i]); //oNetworkImportTxt.DeleteNode(vecOriginID[i]);
			}

			spNodeLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TNodeGeometry);       // oNetworkImportTxt.InsertNode(spZoneDivisionInfo->TZoneID, 0, spZoneDivisionInfo->TNodeGeometry);
			spZoneLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);// oNetworkImportTxt.InsertZone(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);

			++iter;
		}

		//★ 노드, 존 정보 갱신
		pMapView->ReloadRender(false, 1);
		pMapView->ReloadRender(false, 3);
	}
	catch (...)
	{
		ThrowException(_T("Shape 입력 오류"));
	}
}
void KZoningIntegrationDlg::SaveGeometry(KDBaseConPtr spDBConnection)
{
	if (nullptr == spDBConnection)
	{
		return;
	}

	try
	{
		ITxMapServerPtr spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, spDBConnection->GetConnectionString().m_strUrl));
		if (spMapServer->Connect(spDBConnection) != 1)
			throw 1;

		TxFeatureLayerPtr  spNodeLayer = nullptr;
		TxFeatureLayerPtr  spLinkLayer = nullptr;
		TxFeatureLayerPtr  spZoneLayer = nullptr;


		// 서버의 레이어 정보 조회
		std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
		for (size_t j = 0; j < vecLayerInfo.size(); j++) {
			TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

			if (_tcsicmp(TABLE_NODE, spLayerInfo->Name()) == 0) { // 노드 레이어 조회
				spNodeLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}

			if (_tcsicmp(TABLE_LINK, spLayerInfo->Name()) == 0) { // 링크 레이어 조회
				spLinkLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}

			if (_tcsicmp(TABLE_ZONE, spLayerInfo->Name()) == 0) { // 존 레이어 조회
				spZoneLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}
		}

		if (spNodeLayer == nullptr || spLinkLayer == nullptr || spZoneLayer == nullptr)
		{
			throw _T("some layer(s) is(are) not exists");
		}

		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		std::set<Integer>::iterator itDelLink = m_setDeleteLinkID.begin();
		std::set<Integer>::iterator itDelLinkEnd = m_setDeleteLinkID.end();

		try
		{
			while (itDelLink != itDelLinkEnd)
			{
				Integer nxLinkID = *itDelLink;
				spLinkLayer->Delete(nxLinkID);// oNetworkImportTxt.DeleteLink(nxLinkID);

				++itDelLink;
			}

			while (iter != end)
			{
				KZoneDivisionInfoPtr spZoneDivisionInfo = iter->second;

				std::vector<Integer> vecOriginID;
				ConvertZoneListToVector(spZoneDivisionInfo->TIntegrationZoneList, vecOriginID);

				for (size_t i = 0; i < vecOriginID.size(); i++)
				{
					spZoneLayer->Delete(vecOriginID[i]); //oNetworkImportTxt.DeleteZone(vecOriginID[i]);
					spNodeLayer->Delete(vecOriginID[i]); //oNetworkImportTxt.DeleteNode(vecOriginID[i]);
				}

				spNodeLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TNodeGeometry);       // oNetworkImportTxt.InsertNode(spZoneDivisionInfo->TZoneID, 0, spZoneDivisionInfo->TNodeGeometry);
				spZoneLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);// oNetworkImportTxt.InsertZone(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);

				++iter;
			}
		}
		catch (...)
		{
			CString strError = spDBConnection->GetConnectionString().m_strUrl;
			strError += _T(" Zone 집합화 처리 중 Layer 처리 오류");
			ThrowException(strError);
		}
	}
	catch (...)
	{
		ThrowException(_T("Shape 입력 오류"));
	}
}
/* 2019.01.18 수정 끝 : 여러 시나리오 처리기능 추가에 따른 변경 */
/**********************************************************/




void KZoningIntegrationDlg::SaveGeometryCache()
{
}


unsigned __stdcall KZoningIntegrationDlg::IntegrationZoneThreadCaller(void* pParam)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("존 집합화 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Zone Integration Module..."));
	}

	KZoningIntegrationDlg* pDlg = (KZoningIntegrationDlg*)pParameter->GetParameter();;

	m_bRunIntegrationSucceess = pDlg->RunIntegrateGeneration();

	return 0;

}


bool KZoningIntegrationDlg::RunIntegrateGeneration()
{
	TxLogDebugStartMsg();

	CString strModelFolder;
	//strModelFolder.Format(_T("%s%s"),m_pTarget->GetLocation(), DLL_READ_WRITE_PATH);
	strModelFolder.Format(_T("%s%s"), m_pRunningTarget->GetLocation(), DLL_READ_WRITE_PATH);
	CreateDirectory(strModelFolder, NULL);
	CreateDirectory(ConvertSavePath(_T("")), NULL);

	if (!CreateArgument())
	{
		m_nResultCode = DLL_CREATE_ARGFILE_ERROR;
		return false;
	}

	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		m_nResultCode = DLL_USER_STOPPING;
		return false;
	}

	CString strWindowName(_T(""));
	GetWindowText(strWindowName);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("존 집합화 모듈 실행 중..."));
	}
	else {
		AddStatusMessage(_T("Run Zoning(Integration) Module..."));
	}

	KLauncher::Run(strWindowName, 30);

	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		m_nResultCode = DLL_USER_STOPPING;
		return false;
	}

	if (DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("존 집합화 모듈 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Zoning(Integration) Module Complete."));
		}
		TxLogDebug(_T("Integration Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		TxLogDebug(_T("Integration Stop.."));

		return false;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Zonning Dll Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));

		return false;
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Running Failed."));
		}
		TxLogDebug(_T("Integration Run Error.."));

		return false;
	}
	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		m_nResultCode = DLL_USER_STOPPING;
		return false;
	}

	/**************************************************************************/
	/* 2019.03.05 수정 : 처리 완료후 사용된 Zoning.arg 파일을 해당 시나리오 경로에 복사
	/**************************************************************************/
	CString strZoneArg = ConvertSavePath(T_ARG_ZONING);
	QBicFile::CopyFileW(T_ARG_ZONING, strZoneArg);
	/**************************************************************************/


	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Saving Output Data..."));
	}

	TxLogDebugEndMsg();
	return true;
}


bool KZoningIntegrationDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();

	//KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KDBaseConPtr spDBaseConnection = m_pRunningTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		UpsertZoneData(spDBaseConnection);
		UpsertSedData(spDBaseConnection);
		UpsertNodeSystemData(spDBaseConnection);
		DeleteLinkData(spDBaseConnection);
		UpsertPurposeODData(spDBaseConnection);
		UpsertModeODData(spDBaseConnection);
		UpsertParameterODData(spDBaseConnection);

		/**************************************************************************/
		/* 2019.01.18 수정 : 여러 시나리오 처리기능 추가에 따른 변경                     */
		//SaveGeometry();
		KMapView* pMapView = ImChampFrameWindow::GetCreatedMapView(m_pRunningTarget);
		if (nullptr != pMapView)
		{
			SaveGeometry(pMapView);
		}
		else
		{
			// MapView가 없는 시나리오의 Geometry 저장
			SaveGeometry(spDBaseConnection);
		}

		if (m_bAutoGenZoneConn)
		{
			std::vector<TLinkFTNodeID> arrZoneConnectLink;
			InsertZoneConnectLink(spDBaseConnection, arrZoneConnectLink);
			if (nullptr != pMapView)
			{
				SaveZoneConnectLinkGeometry(arrZoneConnectLink, pMapView);
			}
			else
			{
				SaveZoneConnectLinkGeometry(arrZoneConnectLink, spDBaseConnection);
			}
		}
		/**************************************************************************/

		spDBaseConnection->Commit();

		TxLogDebugEndMsg();
		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch (...)
	{
		TxLogDebugWarning();
	}

	return false;
}


bool KZoningIntegrationDlg::UpdateIntegrationSystemInfo()
{
	TxLogDebugStartMsg();

	//KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KDBaseConPtr spDBaseConnection = m_pRunningTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		UpdateZoneSystemData(spDBaseConnection);

		UpdateSedSystemData(spDBaseConnection);

		spDBaseConnection->Commit();

		TxLogDebugEndMsg();
		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch (...)
	{
		TxLogDebugWarning();
	}

	return false;
}


void KZoningIntegrationDlg::UpdateZoneSystemData(KDBaseConPtr a_spDBaseConnection)
{
	CString  strSQLUpdate(_T(""));

	strSQLUpdate.Format(_T(" UPDATE Zone SET Name = ?  where zone_id = ? "));

	try
	{
		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLUpdate);

		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		while (iter != end)
		{
			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;

			spPrepareStatement->BindText(1, spZoningIntegrationZoneInfo->TZoneName);
			spPrepareStatement->BindInt64(2, spZoningIntegrationZoneInfo->TZoneID);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KZoningIntegrationDlg::UpdateSedSystemData(KDBaseConPtr a_spDBaseConnection)
{
	CString  strSQLUpdate(_T(""));

	strSQLUpdate.Format(_T(" UPDATE Sed_Vector SET Name = ? where zone_id = ? "));

	try
	{
		KPreparedStatementPtr spPrepareStatementPtr = a_spDBaseConnection->PrepareStatement(strSQLUpdate);

		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		while (iter != end)
		{
			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;

			spPrepareStatementPtr->BindText(1, spZoningIntegrationZoneInfo->TZoneName);
			spPrepareStatementPtr->BindInt64(2, spZoningIntegrationZoneInfo->TZoneID);

			spPrepareStatementPtr->ExecuteUpdate();
			spPrepareStatementPtr->Reset();

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KZoningIntegrationDlg::UpsertNodeSystemData(KDBaseConPtr a_spDBaseConnection)
{
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From Node Where node_id = ? "));

	try
	{
		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		while (iter != end)
		{
			std::vector<Integer> vecOriginID;

			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
			ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);

			for (size_t i = 0; i < vecOriginID.size(); i++)
			{
				spPrepareStatement->BindInt64(1, vecOriginID[i]);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}

			++iter;
		}

		CString strSQLUpdate(_T(""));

		strSQLUpdate.Format(_T(" UPDATE Node set zone_id = 0 where zone_id = ? "));

		spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLUpdate);

		iter = m_mapZoneIntegrationInfo.begin();
		end = m_mapZoneIntegrationInfo.end();

		while (iter != end)
		{
			std::vector<Integer> vecOriginID;

			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
			ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);

			for (size_t i = 0; i < vecOriginID.size(); i++)
			{
				spPrepareStatement->BindInt64(1, vecOriginID[i]);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
			}

			++iter;
		}

		// insert
		CString  strSQLInsert(_T(" Insert Into Node "));

		strSQLInsert += _T(" ( node_id, name, node_type, x, y, zone_id ");

		strSQLInsert += _T(" ) Values ( ?, ?, ?, ?, ?, ? ");

		strSQLInsert += _T(" )");

		spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

		iter = m_mapZoneIntegrationInfo.begin();
		end = m_mapZoneIntegrationInfo.end();

		while (iter != end)
		{
			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;

			spPrepareStatement->BindInt64(1, spZoningIntegrationZoneInfo->TZoneID);
			spPrepareStatement->BindText(2, spZoningIntegrationZoneInfo->TZoneName);
			spPrepareStatement->BindInt(3, NODE_TYPE_ZONE);

			TxPointPtr spPoint = spZoningIntegrationZoneInfo->TNodeGeometry;
			double dMapX, dMapY;
			dMapX = spPoint->X;
			dMapY = spPoint->Y;

			spPrepareStatement->BindDouble(4, dMapX);
			spPrepareStatement->BindDouble(5, dMapY);
			spPrepareStatement->BindInt64(6, spZoningIntegrationZoneInfo->TZoneID);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::DeleteLinkData(KDBaseConPtr a_spDBaseConnection)
{
	m_setDeleteLinkID.clear();

	try
	{
		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		while (iter != end)
		{
			KZoneDivisionInfoPtr spZoneDivisionInfo = iter->second;

			std::vector<Integer> vecOriginID;

			ConvertZoneListToVector(spZoneDivisionInfo->TIntegrationZoneList, vecOriginID);

			for (size_t i = 0; i < vecOriginID.size(); i++)
			{
				CString strSQLSelect;
				strSQLSelect.Format(_T(" SELECT Link_id FROM %s Where fnode_id = %I64d or tnode_id = %I64d "), TABLE_LINK, vecOriginID[i], vecOriginID[i]);

				KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQLSelect);

				while (spResult->Next())
				{
					Integer nxLinkID = spResult->GetValueInt64(0);
					m_setDeleteLinkID.insert(nxLinkID);
				}
			}

			++iter;
		}

		//delete
		CString strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" Delete From Link Where link_id = ? "));

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		std::set<Integer>::iterator itLink = m_setDeleteLinkID.begin();
		std::set<Integer>::iterator itLinkEnd = m_setDeleteLinkID.end();

		while (itLink != itLinkEnd)
		{
			spPrepareStatement->BindInt64(1, *itLink);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++itLink;
		}

		DeleteTransitData(a_spDBaseConnection, m_setDeleteLinkID);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::DeleteTransitData(KDBaseConPtr a_spDBaseConnection, std::set<Integer> &a_setDeleteLinkID)
{
	try
	{
		AutoType iter = a_setDeleteLinkID.begin();
		AutoType itEnd = a_setDeleteLinkID.end();

		// collect delete transit_ids
		std::set<Integer> setDeleteTransitID;

		while (iter != itEnd)
		{
			CString strSQLSelect;
			strSQLSelect.Format(_T(" SELECT Transit_id FROM %s WHERE link_id = %I64d "), TABLE_TRANSITLINKS, *iter); //★ 필요

			KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQLSelect);

			while (spResult->Next())
			{
				Integer nxTransitID = spResult->GetValueInt64(0);
				setDeleteTransitID.insert(nxTransitID);
			}

			++iter;
		}

		// delete 
		CString  strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" DELETE FROM %s WHERE Transit_id = ? "), TABLE_TRANSITLINKS);  //★ 필요

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		AutoType itTransit = setDeleteTransitID.begin();
		AutoType itTransitEnd = setDeleteTransitID.end();

		while (itTransit != itTransitEnd) {
			spPrepareStatement->BindInt64(1, *itTransit);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++itTransit;
		}

		strSQLDelete.Format(_T(" DELETE FROM %s WHERE Transit_id = ? "), TABLE_TRANSITNODES);
		spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		itTransit = setDeleteTransitID.begin();
		itTransitEnd = setDeleteTransitID.end();

		while (itTransit != itTransitEnd) {
			spPrepareStatement->BindInt64(1, *itTransit);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++itTransit;
		}

		strSQLDelete.Format(_T(" DELETE FROM %s WHERE Transit_id = ? "), TABLE_TRANSIT);
		spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		itTransit = setDeleteTransitID.begin();
		itTransitEnd = setDeleteTransitID.end();

		while (itTransit != itTransitEnd) {
			spPrepareStatement->BindInt64(1, *itTransit);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++itTransit;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::UpsertZoneData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From Zone Where zone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	while (iter != end)
	{
		std::vector<Integer> vecOriginID;

		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
		ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);

		for (size_t i = 0; i < vecOriginID.size(); i++)
		{
			spPrepareStatement->BindInt64(1, vecOriginID[i]);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}

	// insert
	CString  strSQLInsert(_T(" Insert Into Zone "));

	strSQLInsert += _T(" ( zone_id");

	for (int i = 0; i < m_oZoneColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oZoneColumns.GetColumn(i)->Name();
	}

	strSQLInsert += _T(" ) Values ( ?");

	for (int i = 0; i < m_oZoneColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert += _T(" )");


	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs(ConvertSavePath(T_DAT_OUT_ZONE), std::ios::binary);

	if (ifs)
	{
		TxLogDebug(_T("--- start insert Division Zone result"));
		int nCount = 0;

		while (!ifs.eof())
		{
			Integer nxOrigin;
			ifs.read(reinterpret_cast<char*>(&nxOrigin), sizeof(Integer));

			if (ifs.eof())
			{
				break;
			}

			spPrepareStatement->BindInt64(1, nxOrigin);

			for (int i = 0; i < m_oZoneColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read(reinterpret_cast<char*>(&dRecord), sizeof(double));

				spPrepareStatement->BindDouble(i + 2, dRecord);
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division Zone result size : %d"), nCount);
		TxLogDebug((LPCTSTR)strResult);
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningIntegrationDlg::UpsertSedData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From Sed_Vector Where zone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	while (iter != end)
	{
		std::vector<Integer> vecOriginID;

		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
		ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);

		for (size_t i = 0; i < vecOriginID.size(); i++)
		{
			spPrepareStatement->BindInt64(1, vecOriginID[i]);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}

	strSQLDelete.Format(_T(" Delete From Sed_Pop Where zone_id = ? "));

	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	iter = m_mapZoneIntegrationInfo.begin();
	end = m_mapZoneIntegrationInfo.end();

	while (iter != end)
	{
		std::vector<Integer> vecOriginID;

		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
		ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);

		for (size_t i = 0; i < vecOriginID.size(); i++)
		{
			spPrepareStatement->BindInt64(1, vecOriginID[i]);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}

	// insert
	CString  strSQLInsert(_T(" Insert Into Sed_Vector "));

	strSQLInsert += _T(" ( zone_id");

	for (int i = 0; i < m_oSedVectorColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oSedVectorColumns.GetColumn(i)->Name();
	}

	strSQLInsert += _T(" ) Values ( ?");

	for (int i = 0; i < m_oSedVectorColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert += _T(" )");


	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs(ConvertSavePath(T_DAT_OUT_SED), std::ios::binary);

	if (ifs)
	{
		TxLogDebug(_T("--- start insert Division Sed result"));
		int nCount = 0;

		while (!ifs.eof())
		{
			Integer nxOrigin;
			ifs.read(reinterpret_cast<char*>(&nxOrigin), sizeof(Integer));

			if (ifs.eof())
			{
				break;
			}

			spPrepareStatement->BindInt64(1, nxOrigin);

			for (int i = 0; i < m_oSedVectorColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read(reinterpret_cast<char*>(&dRecord), sizeof(double));

				spPrepareStatement->BindDouble(i + 2, dRecord);
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division Sed result size : %d"), nCount);
		TxLogDebug((LPCTSTR)strResult);
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningIntegrationDlg::UpsertPurposeODData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From purpose_od Where ozone_id = ? or dzone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	while (iter != end)
	{
		std::vector<Integer> vecOriginID;

		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
		ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);

		for (size_t i = 0; i < vecOriginID.size(); i++)
		{
			spPrepareStatement->BindInt64(1, vecOriginID[i]);
			spPrepareStatement->BindInt64(2, vecOriginID[i]);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}

	// insert
	CString  strSQLInsert(_T(" Insert Into purpose_od "));

	strSQLInsert += _T(" ( ozone_id, dzone_id");

	for (int i = 0; i < m_oPurposeODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oPurposeODColumns.GetColumn(i)->Name();
	}

	strSQLInsert += _T(" ) Values ( ?, ? ");

	for (int i = 0; i < m_oPurposeODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert += _T(" )");

	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs(ConvertSavePath(T_DAT_OUT_PURPOSEOD), std::ios::binary);

	if (ifs)
	{
		TxLogDebug(_T("--- start insert Division purpose_od result"));
		int nCount = 0;

		while (!ifs.eof())
		{
			Integer nxOrigin;
			ifs.read(reinterpret_cast<char*>(&nxOrigin), sizeof(Integer));

			if (ifs.eof())
			{
				break;
			}

			Integer nxDestination;
			ifs.read(reinterpret_cast<char*>(&nxDestination), sizeof(Integer));

			spPrepareStatement->BindInt64(1, nxOrigin);
			spPrepareStatement->BindInt64(2, nxDestination);

			for (int i = 0; i < m_oPurposeODColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read(reinterpret_cast<char*>(&dRecord), sizeof(double));

				spPrepareStatement->BindDouble(i + 3, dRecord);
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division purpose_od result size : %d"), nCount);
		TxLogDebug((LPCTSTR)strResult);
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningIntegrationDlg::UpsertModeODData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From mode_od Where ozone_id = ? or dzone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	while (iter != end)
	{
		std::vector<Integer> vecOriginID;

		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
		ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);

		for (size_t i = 0; i < vecOriginID.size(); i++)
		{
			spPrepareStatement->BindInt64(1, vecOriginID[i]);
			spPrepareStatement->BindInt64(2, vecOriginID[i]);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}

	// insert
	CString  strSQLInsert(_T(" Insert Into mode_od "));

	strSQLInsert += _T(" ( ozone_id, dzone_id");

	for (int i = 0; i < m_oModeODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oModeODColumns.GetColumn(i)->Name();
	}

	strSQLInsert += _T(" ) Values ( ?, ? ");

	for (int i = 0; i < m_oModeODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert += _T(" )");

	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs(ConvertSavePath(T_DAT_OUT_MODEOD), std::ios::binary);

	if (ifs)
	{
		TxLogDebug(_T("--- start insert Division mode_od result"));
		int nCount = 0;

		while (!ifs.eof())
		{
			Integer nxOrigin;
			ifs.read(reinterpret_cast<char*>(&nxOrigin), sizeof(Integer));

			if (ifs.eof())
			{
				break;
			}

			Integer nxDestination;
			ifs.read(reinterpret_cast<char*>(&nxDestination), sizeof(Integer));

			spPrepareStatement->BindInt64(1, nxOrigin);
			spPrepareStatement->BindInt64(2, nxDestination);

			for (int i = 0; i < m_oModeODColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read(reinterpret_cast<char*>(&dRecord), sizeof(double));

				spPrepareStatement->BindDouble(i + 3, dRecord);
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division mode_od result size : %d"), nCount);
		TxLogDebug((LPCTSTR)strResult);
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningIntegrationDlg::UpsertParameterODData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From Parameter_OD Where ozone_id = ? or dzone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	while (iter != end)
	{
		std::vector<Integer> vecOriginID;

		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
		ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);

		for (size_t i = 0; i < vecOriginID.size(); i++)
		{
			spPrepareStatement->BindInt64(1, vecOriginID[i]);
			spPrepareStatement->BindInt64(2, vecOriginID[i]);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}

	// insert
	CString  strSQLInsert(_T(" Insert Into Parameter_OD "));

	strSQLInsert += _T(" ( ozone_id, dzone_id");

	for (int i = 0; i < m_oParameterODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oParameterODColumns.GetColumn(i)->Name();
	}

	strSQLInsert += _T(" ) Values ( ?, ? ");

	for (int i = 0; i < m_oParameterODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert += _T(" )");

	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs(ConvertSavePath(T_DAT_OUT_ODPARAMETER), std::ios::binary);

	if (ifs)
	{
		TxLogDebug(_T("--- start insert Division parameter_od result"));
		int nCount = 0;

		while (!ifs.eof())
		{
			Integer nxOrigin;
			ifs.read(reinterpret_cast<char*>(&nxOrigin), sizeof(Integer));

			if (ifs.eof())
			{
				break;
			}

			Integer nxDestination;
			ifs.read(reinterpret_cast<char*>(&nxDestination), sizeof(Integer));

			spPrepareStatement->BindInt64(1, nxOrigin);
			spPrepareStatement->BindInt64(2, nxDestination);

			for (int i = 0; i < m_oParameterODColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read(reinterpret_cast<char*>(&dRecord), sizeof(double));

				spPrepareStatement->BindDouble(i + 3, dRecord);
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division parameter_od result size : %d"), nCount);
		TxLogDebug((LPCTSTR)strResult);
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningIntegrationDlg::NotifyRelationTable()
{
	try
	{
		//KIOTables* pIOTables = m_pTarget->Tables();
		KIOTables* pIOTables = m_pRunningTarget->Tables();
		KIOTable*        pTable = pIOTables->FindTable(TABLE_NODE);
		pTable->Notify();

		NotifyZoneRelation();

		NotifyLinkRelation();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::NotifyZoneRelation()
{
	try
	{
		//KIOTables* pIOTables = m_pTarget->Tables();
		KIOTables* pIOTables = m_pRunningTarget->Tables();
		KIOTable*        pTable = pIOTables->FindTable(TABLE_ZONE);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_SED_VECTOR);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_SED_POP);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_PURPOSE_OD);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_MODE_OD);
		pTable->Notify();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::NotifyLinkRelation()
{
	try
	{
		//KIOTables* pIOTables = m_pTarget->Tables();
		KIOTables* pIOTables = m_pRunningTarget->Tables();
		KIOTable*  pTable = pIOTables->FindTable(TABLE_LINK);
		pTable->Notify();

		if (KmzSystem::IsPassengerSystem())
		{
			pTable = pIOTables->FindTable(TABLE_TRANSIT);
			if (nullptr != pTable) {
				pTable->Notify();
			}

			pTable = pIOTables->FindTable(TABLE_TRANSITNODES);
			if (nullptr != pTable) {
				pTable->Notify();
			}

			pTable = pIOTables->FindTable(TABLE_TURN);
			if (nullptr != pTable) {
				pTable->Notify();
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


bool KZoningIntegrationDlg::CreateArgument()
{
	TxLogDebugStartMsg();
	ImChampDir::SetCurrentDirectoryToApp();
	if (!ClearFiles())
	{
		return false;
	}
	//TxLogDebug("END CLEAR FILE ------ ");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(T_ARG_ZONING, nOpenMode);

	bool bResult = false;

	// .arg 작성
	bResult = ArgumentWriteBase(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteBase) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = ArgumentWriteIntegration(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteIntegration) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	of.Close();


	// ZONE.dat 생성
	m_oZoneColumns.Clear();
	GetColumnInfo(TABLE_ZONE, m_oZoneColumns);

	//bResult = KBulkDBase2File::ExportDB2ZoneFile(m_pTarget->GetDBaseConnection(), m_oZoneColumns, ConvertSavePath(T_DAT_IN_ZONE));
	bResult = KBulkDBase2File::ExportDB2ZoneFile(m_pRunningTarget->GetDBaseConnection(), m_oZoneColumns, ConvertSavePath(T_DAT_IN_ZONE));
	if (!bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ZoneFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// SED.dat 생성
	m_oSedVectorColumns.Clear();
	GetColumnInfo(TABLE_SED_VECTOR, m_oSedVectorColumns);

	//bResult = KBulkDBase2File::ExportDB2SedVecFile(m_pTarget->GetDBaseConnection(), m_oSedVectorColumns, ConvertSavePath(T_DAT_IN_SED));
	bResult = KBulkDBase2File::ExportDB2SedVecFile(m_pRunningTarget->GetDBaseConnection(), m_oSedVectorColumns, ConvertSavePath(T_DAT_IN_SED));
	if (!bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2SedVecFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// MODEOD.dat 생성
	m_oModeODColumns.Clear();
	GetColumnInfo(TABLE_MODE_OD, m_oModeODColumns);

	//bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_MODE_OD, m_oModeODColumns, ConvertSavePath(T_DAT_IN_MODEOD));
	bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunningTarget->GetDBaseConnection(), TABLE_MODE_OD, m_oModeODColumns, ConvertSavePath(T_DAT_IN_MODEOD));
	if (!bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// PURPOSEOD.dat 생성
	m_oPurposeODColumns.Clear();
	GetColumnInfo(TABLE_PURPOSE_OD, m_oPurposeODColumns);

	//bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_PURPOSE_OD, m_oPurposeODColumns, ConvertSavePath(T_DAT_IN_PURPOSEOD));
	bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunningTarget->GetDBaseConnection(), TABLE_PURPOSE_OD, m_oPurposeODColumns, ConvertSavePath(T_DAT_IN_PURPOSEOD));
	if (!bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// ODParameter.dat 생성 
	m_oParameterODColumns.Clear();
	GetColumnInfo(TABLE_PARAMETER_OD, m_oParameterODColumns);

	//bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_PARAMETER_OD, m_oParameterODColumns, ConvertSavePath(T_DAT_IN_ODPARAMETER));
	bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunningTarget->GetDBaseConnection(), TABLE_PARAMETER_OD, m_oParameterODColumns, ConvertSavePath(T_DAT_IN_ODPARAMETER));
	if (!bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	/**************************************************************************/
	/* 2019.01.18 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
	if (m_bAutoGenZoneConn)
	{
		// node.dat 생성
		bResult = KBulkDBase2File::ExportDB2NodeFile(m_pRunningTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_NODE));
		if (!bResult)
		{
			AfxMessageBox(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
			return false;
		}

		// link.dat 생성
		KIOColumns oEmptyLinkColumns;
		bResult = KBulkDBase2File::ExportDB2LinkFileD(m_pRunningTarget->GetDBaseConnection(), oEmptyLinkColumns, ConvertSavePath(T_DAT_LINK));
		if (!bResult)
		{
			AfxMessageBox(_T("Dat 생성(ExportDB2LinkFileD) 중 오류가 발생하였습니다. !!!"));
			return false;
		}
	}
	/**************************************************************************/

	/**************************************************************************/
	/* 2019.03.05 수정 : 존 연결로 자동생성시 DefineNetworkType.arg 파일을 각 시나리오에 출력 */
	CString strCodePath(_T(""));
	strCodePath.Format(_T("%s\\%s"), DLL_READ_WRITE_PATH, ZONING_FOLDER_NAME);
	try
	{
		KWriteCodeAgrument::WirteMatchingCodeArument(m_pRunningTarget, strCodePath);
	}
	catch (...) {
		AfxMessageBox(_T("Dat 생성(DefineNetworkType.arg) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	/**************************************************************************/

	TxLogDebugEndMsg();
	return true;
}


bool KZoningIntegrationDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove((CStringA)ConvertSavePath(T_ARG_ZONING));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_ZONE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_SED));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_MODEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_PURPOSEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_ODPARAMETER));

		/**************************************************************************/
		/* 2019.01.18 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
		/**************************************************************************/

		// 결과 파일 삭제 
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ZONE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_SED));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_MODEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_PURPOSEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ODPARAMETER));

		/**************************************************************************/
		/* 2019.01.18 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ZONECONNECTLINK));
		/**************************************************************************/
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


bool KZoningIntegrationDlg::ArgumentWriteBase(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		//In Out FolderName
		CString strPath = ConvertSavePath(_T(""));
		strOut.Format(_T("%s\r\n"), strPath);
		of.WriteString(strOut);

		// 존 수
		strOut.Format(_T("Nzone\t%d\r\n"), InitZoneCount());
		of.WriteString(strOut);

		// 존 파일 형태수
		strOut.Format(_T("Nzonefile\t%d\r\n"), FILE_TYPE_COUNT_ZONE);
		of.WriteString(strOut);

		// 존 파일 1
		strOut.Format(_T("Zonefile1\t%s\t%d\r\n"), _T("ZONE"), GetDoubleTypeColumnCount(TABLE_ZONE));
		of.WriteString(strOut);

		// 존 파일 2
		strOut.Format(_T("Zonefile2\t%s\t%d\r\n"), _T("SED"), GetDoubleTypeColumnCount(TABLE_SED_VECTOR));
		of.WriteString(strOut);

		// OD 파일 형태수
		strOut.Format(_T("Nodfile\t%d\r\n"), FILE_TYPE_COUNT_OD);
		of.WriteString(strOut);

		// od 파일 1
		strOut.Format(_T("ODfile1\t%s\t%d\r\n"), _T("MODEOD"), GetDoubleTypeColumnCount(TABLE_MODE_OD));
		of.WriteString(strOut);

		// od 파일 2
		strOut.Format(_T("ODfile2\t%s\t%d\r\n"), _T("PURPOSEOD"), GetDoubleTypeColumnCount(TABLE_PURPOSE_OD));
		of.WriteString(strOut);

		// od 파일 3
		strOut.Format(_T("ODfile3\t%s\t%d\r\n"), _T("ODParameter"), GetDoubleTypeColumnCount(TABLE_PARAMETER_OD));
		of.WriteString(strOut);

		// classification 
		strOut.Format(_T("Classification\t%d\r\n"), CLASSIFICATION_CODE_INTEGRATION);
		of.WriteString(strOut);

		/**************************************************************************/
		/* 2019.01.18 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
		// 존 연결로 자동 생성 여부
		strOut.Format(_T("AutoGenZoneConn\t%d\r\n"), (m_bAutoGenZoneConn == TRUE ? 1 : 0));
		of.WriteString(strOut);
		/**************************************************************************/

	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KZoningIntegrationDlg::ArgumentWriteIntegration(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		// Integration 횟수
		strOut.Format(_T("Integration\t%d\r\n"), m_mapZoneIntegrationInfo.size());
		of.WriteString(strOut);

		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		CString strIndex;
		CString strOriginalZoneInfo;
		CString strIntegrationZoneInfo;

		CString strTempZone;

		int     nIndex = 1;
		while (iter != end)
		{
			KZoneDivisionInfoPtr spZoneDivisionInfo = iter->second;

			//index
			strOut.Format(_T("Index\t%d\r\n"), nIndex++);
			of.WriteString(strOut);

			// origin zones 
			std::vector<Integer> vecOriginID;
			ConvertZoneListToVector(spZoneDivisionInfo->TIntegrationZoneList, vecOriginID);

			size_t nxCount = vecOriginID.size();

			strOriginalZoneInfo = _T("");
			for (size_t i = 0; i < nxCount; i++)
			{
				strTempZone.Format(_T("%I64d"), vecOriginID[i]);

				if (i != nxCount - 1)
				{
					strOriginalZoneInfo = strOriginalZoneInfo + strTempZone + _T("\t");
				}
				else
				{
					strOriginalZoneInfo = strOriginalZoneInfo + strTempZone;
				}
			}

			// origin zone 번호
			strOut.Format(_T("OriginZone\t%s\r\n"), strOriginalZoneInfo);
			of.WriteString(strOut);

			// Integeration Zone 수
			strOut.Format(_T("nIntegrationZone\t%d\r\n"), nxCount);
			of.WriteString(strOut);

			/**************************************************************************/
			/* 2019.03.08 수정 : 존 세분화/집합화 argument 파일 변경에 따른 수정 */
			// Integration zone NewID
			//strOut.Format(_T("IntegrationZoneNewID\t%I64d\r\n"), spZoneDivisionInfo->TZoneID);
			//of.WriteString(strOut);


			of.WriteString(_T("IntegrationInfo\r\n"));

			strOut.Format(_T("%I64d\t%f\t%f\r\n"),
				spZoneDivisionInfo->TZoneID,
				spZoneDivisionInfo->TNodeGeometry->X, spZoneDivisionInfo->TNodeGeometry->Y);
			of.WriteString(strOut);
			/**************************************************************************/
			++iter;
		}

	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


void KZoningIntegrationDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KZoningIntegrationDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if (spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


int KZoningIntegrationDlg::InitZoneCount()
{
	//KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KDBaseConPtr spDBaseConnection = m_pRunningTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format(_T(" SELECT COUNT(*) AS Count FROM %s "), TABLE_ZONE);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if (nullptr == spResult)
	{
		return 0;
	}

	int nZoneCount = 0;
	while (spResult->Next())
	{
		nZoneCount = spResult->GetValueInt(0);
	}

	return nZoneCount;
}


int KZoningIntegrationDlg::GetDoubleTypeColumnCount(CString a_strTableName)
{
	//KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTables*   pIOTables = m_pRunningTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(a_strTableName);
	if (NULL == pTable)
	{
		return 0;
	}

	int nColumnCount = 0;
	const KIOColumns* pColumns = pTable->Columns();
	for (int i = 0; i < pColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if (pColumn->DataType() == KEMIODataTypeDouble)
		{
			nColumnCount++;
		}
	}

	return nColumnCount;
}


void KZoningIntegrationDlg::GetColumnInfo(CString a_strTableName, KIOColumns &a_oColumns)
{
	//KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTables*   pIOTables = m_pRunningTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(a_strTableName);
	if (NULL == pTable)
	{
		return;
	}

	int nColumnCount = 0;
	const KIOColumns* pColumns = pTable->Columns();
	for (int i = 0; i < pColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if (pColumn->DataType() == KEMIODataTypeDouble)
		{
			a_oColumns.AddNewColumn(pColumn);
		}
	}
}


void KZoningIntegrationDlg::OnBnClickedBtnAddNewZone()
{
	CXTPReportRow* pRow = m_wndAAAZoneReportCtrl.GetSelectedRows()->GetAt(0);
	int            nIndexPrevious(0);

	if (m_wndAAAZoneReportCtrl.GetSelectedRows()->GetCount() > 0)
		nIndexPrevious = pRow->GetIndex();
	else
		nIndexPrevious = 0;

	m_nxTempMaxID++;
	m_mapZoneIntegrationInfo.insert(std::make_pair(m_nxTempMaxID, KZoneDivisionInfoPtr(new KZoneDivisionInfo())));

	//refresh
	DisplayAZone();
	m_wndBBBZoneReportCtrl.ResetContent();

	// after refresh
	pRow = m_wndAAAZoneReportCtrl.GetRows()->GetAt(nIndexPrevious);
	pRow->SetSelected(TRUE);
	m_wndAAAZoneReportCtrl.SetFocusedRow(pRow);
	//m_wndAAAZoneReportCtrl.SetTopRow(nIndexPrevious);

	CXTPReportRecord*     pRecord = pRow->GetRecord();
	RefreshRelationRecord(pRecord);
}


void KZoningIntegrationDlg::OnBnClickedBtnDelZone()
{
	CXTPReportRow* pRow = m_wndAAAZoneReportCtrl.GetSelectedRows()->GetAt(0);
	if (NULL == pRow)
	{
		return;
	}

	int nIndexPrevious = pRow->GetIndex();

	CXTPReportRecord*     pRecord = pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(_0_INTER_COLUMN_CHECK);
	Integer nxTempID = (Integer)(pItem->GetItemData());

	// delete selected data
	m_wndAAAZoneReportCtrl.GetRecords()->RemoveRecord(pRecord);
	m_wndAAAZoneReportCtrl.Populate();

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.find(nxTempID);

	if (iter != m_mapZoneIntegrationInfo.end())
	{
		m_mapZoneIntegrationInfo.erase(iter->first);
	}

	// refresh
	DisplayAZone();
	m_wndBBBZoneReportCtrl.ResetContent();

	// after refresh 
	if (m_wndAAAZoneReportCtrl.GetRows()->GetCount() > 0)
	{
		int nIndexAfter(0);
		if (nIndexPrevious > 0)
			nIndexAfter = nIndexPrevious - 1; // 선택 행을 삭제 한 후 focus는 이전 행으로 셋팅
		else
			nIndexAfter = 0;

		pRow = m_wndAAAZoneReportCtrl.GetRows()->GetAt(nIndexAfter);

		pRow->SetSelected(TRUE);
		m_wndAAAZoneReportCtrl.SetFocusedRow(pRow);
		//m_wndAAAZoneReportCtrl.SetTopRow(nIndexAfter);

		pRecord = pRow->GetRecord();
		RefreshRelationRecord(pRecord);
	}
}


void KZoningIntegrationDlg::DisplayAZone()
{
	if (KmzSystem::GetLanguage() == KEMKorea) {
		SetDlgItemText(IDC_STATIC_Division, _T("집합화 할 존 노드 번호 : -"));
	}
	else {
		SetDlgItemText(IDC_STATIC_Division, _T("Integrated New Zone [ID] including : -"));
	}

	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
	std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

	if (iter == end) return;

	m_wndAAAZoneReportCtrl.ResetContent(FALSE);

	CXTPReportRecord*            pRecord = NULL;
	CXTPReportRecordItem*        pRecordItem = NULL;
	CXTPReportRecordItemControl* pButton = NULL;
	while (iter != end)
	{
		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;

		CString strTempKeyID(_T(""));
		strTempKeyID.Format(_T("%I64d"), iter->first);

		// 선택
		pRecord = m_wndAAAZoneReportCtrl.AddRecord(new CXTPReportRecord());
		// 		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strTempKeyID));
		// 		pRecordItem->SetAlignment(xtpColumnIconLeft);
		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText());
		pRecordItem->SetAlignment(xtpColumnIconCenter);
		pRecordItem->SetItemData((DWORD_PTR)iter->first);
		pRecordItem->SetIconIndex(0);
		pRecordItem->SetEditable(FALSE);

		// zone list 선택
		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(FALSE);

		pButton = pRecordItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
		if (NULL != pButton)
		{
			pButton->SetAlignment(xtpItemControlUnknown);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pButton->SetCaption(_T("선택"));
				pButton->SetSize(CSize(77, 0));
			}
			else {
				pButton->SetCaption(_T("Select"));
				pButton->SetSize(CSize(87, 0));
			}
		}

		// map
		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		if (spZoningIntegrationZoneInfo->TNodeGeometry == NULL)
		{
			pRecordItem->SetCaption(_T("Not Added"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pRecordItem->SetCaption(_T("존지정(X)"));
			}
		}
		else
		{
			pRecordItem->SetCaption(_T("Add Complete"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pRecordItem->SetCaption(_T("존지정(O)"));
			}
		}

		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(FALSE);

		pButton = pRecordItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
		if (NULL != pButton)
		{
			pButton->SetAlignment(xtpItemControlUnknown);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pButton->SetCaption(_T("지도"));
				pButton->SetSize(CSize(35, 0));
			}
			else {
				pButton->SetCaption(_T("map"));
				pButton->SetSize(CSize(35, 0));
			}
		}

		// new zone name
		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(spZoningIntegrationZoneInfo->TZoneName));
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(TRUE);

		// note
		/*pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(FALSE);*/

		++iter;
	}

	UpdateNote();
	m_wndAAAZoneReportCtrl.Populate();
}


void KZoningIntegrationDlg::OnIntegrationSelectChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	try
	{
		CXTPReportRecords* pRecords = m_wndAAAZoneReportCtrl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		if (nRecordCount < 1)
		{
			return;
		}

		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(_0_INTER_COLUMN_CHECK);
			pItem->SetIconIndex(0);
		}

		/*CXTPReportRow* pRow = m_wndAAAZoneReportCtrl.GetFocusedRow();*/
		CXTPReportRow* pRow = m_wndAAAZoneReportCtrl.GetSelectedRows()->GetAt(0);

		if (NULL != pRow)
		{
			CXTPReportRecord*     pRecord = pRow->GetRecord();
			RefreshRelationRecord(pRecord);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::RefreshRelationRecord(CXTPReportRecord* a_pRecordA)
{
	try
	{
		CXTPReportRecordItem* pItem = a_pRecordA->GetItem(_0_INTER_COLUMN_CHECK);
		pItem->SetIconIndex(1);

		Integer nxTempMaxID = (Integer)pItem->GetItemData();

		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.find(nxTempMaxID);
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		std::vector<Integer> vecOriginID;
		Integer tmpNewZID = 0;
		if (iter != end)
		{
			m_pRunningTarget = m_pTarget;
			GenerateNewZoneID();
			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
			ConvertZoneListToVector(spZoningIntegrationZoneInfo->TIntegrationZoneList, vecOriginID);
			tmpNewZID=spZoningIntegrationZoneInfo->TZoneID;
		}

		CString strTemp;

		if (tmpNewZID == 0) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				//strTemp.Format(_T("집합화 할 존 노드 번호 : %d"), vecOriginID.size());
				strTemp = _T("집합화 할 존 노드 번호 : -");
			}
			else {
				//strTemp.Format(_T("Integrated New Zone [ID] including : %d"), vecOriginID.size());
				strTemp = _T("Integrated New Zone [ID] including : -");
			}
		}
		else {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				//strTemp.Format(_T("집합화 할 존 노드 번호 : %d"), vecOriginID.size());
				strTemp.Format(_T("집합화 할 존 노드 번호 : %d"), tmpNewZID);
			}
			else {
				//strTemp.Format(_T("Integrated New Zone [ID] including : %d"), vecOriginID.size());
				strTemp.Format(_T("Integrated New Zone [ID] including : %d"), tmpNewZID);
			}
		}

		SetDlgItemText(IDC_STATIC_Division, strTemp);

		DisplayBZone(nxTempMaxID);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::OnIntegrationItemButtonClick(NMHDR * pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*)pNMHDR;
	if (!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRow* pRow = pItemNotify->pRow;

	pRow->SetSelected(TRUE);
	OnIntegrationSelectChanged(pNMHDR, pResult);

	CXTPReportRecordItem* pItem = pItemNotify->pItem;

	if (_2_INTER_COLUMN_MAP == pItem->GetIndex())
	{
		m_pMapView->ZoningProcessStart(this);
	}
	else if (_1_INTER_COLUMN_ZONELIST == pItem->GetIndex())
	{
		PostMessage(UM_ORIGINZONEEDIT, (WPARAM)pRow);
	}
}

#pragma region old

// LRESULT KZoningIntegrationDlg::OnOriginZoneEdit(WPARAM a_wParam, LPARAM a_lParam)
// {
// 	CXTPReportRow*        pRow    = (CXTPReportRow*)a_wParam;
// 	CXTPReportRecord*     pRecord = pRow->GetRecord();
// 	CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_INTER_COLUMN_CHECK);
// 	
// 	int nSelectedLineIndex        = pRow->GetIndex();
// 
// 	Integer nxTempID              = (Integer)(pItem->GetItemData());
// 
// 	std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.begin();
// 	std::map<Integer, KZoneDivisionInfoPtr>::iterator end  = m_mapZoneIntegrationInfo.end();
// 	std::vector<Integer> vecZoneID;
// 
// 	std::map<Integer, Integer> mapZoneIDTempID; /* <zoneId, MaxTempID >*/
// 	while(iter != end)
// 	{
// 		KZoneDivisionInfoPtr   spZoningIntegrationZoneInfo = iter->second;
// 
// 		CString &strZoneList = spZoningIntegrationZoneInfo->TIntegrationZoneList;
// 
// 		if( strZoneList.IsEmpty() == false )
// 		{
// 			int nPos = 0;
// 			CString strList = strZoneList;
// 			strList.Trim();
// 			CString strZone = strList.Tokenize( _T(","), nPos );
// 			while( strZone != _T("") )
// 			{
// 				Integer nxZoneID = _ttoi64( strZone );
// 
// 				mapZoneIDTempID.insert(std::make_pair(nxZoneID, iter->first));
// 				strZone = strZoneList.Tokenize( _T(","), nPos );
// 			}
// 		}
// 
// 		++iter;
// 	}
// 
// 	KZoneSelectIntergrationDlg oZoneSelectDlg(this);
// 	oZoneSelectDlg.SetTarget(m_pTarget);
// 	oZoneSelectDlg.SetInitialSelectedZone(mapZoneIDTempID);
// 	oZoneSelectDlg.SetParentID(nxTempID);
// 
// 	if (oZoneSelectDlg.DoModal() != IDOK)
// 	{
// 		return -1;
// 	}
// 
// 	vecZoneID = oZoneSelectDlg.GetSelectedParentZones();
// 
// 	iter = m_mapZoneIntegrationInfo.find(nxTempID);
// 	end  = m_mapZoneIntegrationInfo.end();
// 
// 	if( iter != end )
// 	{
// 		CString strZoneList(_T(""));
// 		CString strTemp(_T(""));
// 		for( size_t i= 0; i< vecZoneID.size(); i++)
// 		{
// 			if(i > 0)
// 			{
// 				strZoneList += _T(",");
// 			}
// 
// 			strTemp.Format(_T("%I64d"), vecZoneID[i]);
// 			strZoneList += strTemp;
// 		}
// 
// 		KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
// 		spZoningIntegrationZoneInfo->TIntegrationZoneList = strZoneList;
// 	}
// 
// 	CString strTemp;
// 	strTemp.Format(_T("Integration Zone Count: %d"), vecZoneID.size());
// 	SetDlgItemText(IDC_STATIC_Division, strTemp);
// 
// 	DisplayBZone(nxTempID);
// 
// 	UpdateNote();
// 
// 	CXTPReportRows* pRows = m_wndAAAZoneReportCtrl.GetRows();
// 	pRow = pRows->GetAt(nSelectedLineIndex);
// 	pRow->SetSelected(TRUE);
// 
// 	return 0;
// }
#pragma endregion old
LRESULT KZoningIntegrationDlg::OnOriginZoneEdit(WPARAM a_wParam, LPARAM a_lParam)
{
	try
	{
		CWaitCursor cw;

		CXTPReportRow*        pRow = (CXTPReportRow*)a_wParam;
		CXTPReportRecord*     pRecord = pRow->GetRecord();
		CXTPReportRecordItem* pItem = pRecord->GetItem(_0_INTER_COLUMN_CHECK);

		int     nSelectedLineIndex = pRow->GetIndex();
		m_nxSelectedTempID = (Integer)(pItem->GetItemData());

		AutoType iter = m_mapZoneIntegrationInfo.begin();
		AutoType end = m_mapZoneIntegrationInfo.end();

		CString strSelectedZoneList(_T(""));
		std::map<Integer, Integer> mapDeSelectedZoneIDTempID; /* <zoneId, MaxTempID >*/

		while (iter != end)
		{
			KZoneDivisionInfoPtr   spZoningIntegrationZoneInfo = iter->second;

			if (m_nxSelectedTempID == iter->first)// Report 포커스가 된 데이터의 ZoneList
			{
				strSelectedZoneList = spZoningIntegrationZoneInfo->TIntegrationZoneList;
			}
			else						// Report none 포커스가 된 데이터들의 ZoneList
			{
				CString &strZoneList = spZoningIntegrationZoneInfo->TIntegrationZoneList;

				if (strZoneList.IsEmpty() == false)
				{
					int nPos = 0;
					CString strList = strZoneList;
					strList.Trim();
					CString strZone = strList.Tokenize(_T(","), nPos);
					while (strZone != _T(""))
					{
						Integer nxZoneID = _ttoi64(strZone);

						mapDeSelectedZoneIDTempID.insert(std::make_pair(nxZoneID, iter->first));
						strZone = strZoneList.Tokenize(_T(","), nPos);
					}
				}
			}

			++iter;
		}

		KTDNodeDataMap          mapReZoneNodeData;
		std::vector<KIDCaption> vecReZoneNodeIDCaption;

		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));

		AutoType itReNode = m_mapZoneNodeData.begin();
		AutoType itEndReNode = m_mapZoneNodeData.end();
		while (itReNode != itEndReNode)
		{
			nxNodeId = itReNode->first;
			strNodeName = itReNode->second;

			if (mapDeSelectedZoneIDTempID.find(nxNodeId) != mapDeSelectedZoneIDTempID.end()) // deselected Data의 ZoneList 들은 제외
			{
				++itReNode;
				continue;
			}

			mapReZoneNodeData.insert(std::make_pair(nxNodeId, strNodeName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxNodeId);
			oIDCaption.TID = strNodeID;
			oIDCaption.TCaption = strNodeName;

			vecReZoneNodeIDCaption.push_back(oIDCaption);

			++itReNode;
		}

		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		m_spImTasMultiIDFind->SetNecessaryCheckCount(2);

		m_spImTasMultiIDFind->SetIDCaption(vecReZoneNodeIDCaption);
		m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, mapReZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);

		m_spImTasMultiIDFind->SetSelectedID(strSelectedZoneList);
		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return 0;
}


void KZoningIntegrationDlg::DisplayBZone(Integer &a_nxTempKeyID)
{
	try
	{
		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.find(a_nxTempKeyID);
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end = m_mapZoneIntegrationInfo.end();

		if (iter == end) return;

		std::vector<Integer> vecOriginID;
		if (iter != end)
		{
			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;
			CString &strZoneList = spZoningIntegrationZoneInfo->TIntegrationZoneList;

			if (strZoneList.IsEmpty() == false)
			{
				int nPos = 0;
				CString strList = strZoneList;
				strList.Trim();
				CString strZone = strList.Tokenize(_T(","), nPos);
				while (strZone != _T(""))
				{
					Integer nxZoneID = _ttoi64(strZone);
					vecOriginID.push_back(nxZoneID);
					strZone = strZoneList.Tokenize(_T(","), nPos);
				}
			}
		}

		CXTPReportRecord*            pRecord = NULL;
		CXTPReportRecordItem*        pRecordItem = NULL;
		CXTPReportRecordItemControl* pButton = NULL;

		m_wndBBBZoneReportCtrl.ResetContent(FALSE);
		for (size_t i = 0; i < vecOriginID.size(); i++)
		{
			KZoneDivisionInfoPtr spZoningIntegrationZoneInfo = iter->second;

			pRecord = m_wndBBBZoneReportCtrl.AddRecord(new CXTPReportRecord());
			pRecordItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)i + 1, _T("%.0f")));
			pRecordItem->SetAlignment(DT_CENTER);
			pRecordItem->SetItemData((DWORD_PTR)(i + 1));
			pRecordItem->SetEditable(FALSE);

			CString strID(_T(""));
			strID.Format(_T("%I64d"), vecOriginID[i]);
			pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strID));
			pRecordItem->SetAlignment(DT_CENTER);
			pRecordItem->SetItemData((DWORD_PTR)vecOriginID[i]);
			pRecordItem->SetEditable(FALSE);

			CString strName(_T(""));
			AutoType iter = m_mapZoneNodeData.find(vecOriginID[i]);
			if (iter != m_mapZoneNodeData.end())
			{
				strName = iter->second;
			}
			pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strName));
			pRecordItem->SetAlignment(DT_LEFT);
			pRecordItem->SetEditable(FALSE);
		}
		m_wndBBBZoneReportCtrl.Populate();

		DrawZoneAccess(vecOriginID);
		DrawAddedZone(iter->second);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::UpdateNote()
{
	CXTPReportRecords* pRecords = m_wndAAAZoneReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	int nCompleteCount = 0;
	for (int i=0; i<nRecordCount; i++)
	{
		CXTPReportRecord*     pRecord   = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem     = pRecord->GetItem(_0_INTER_COLUMN_CHECK);
		//CXTPReportRecordItem* pItemNote = pRecord->GetItem(_4_INTER_COLUMN_NOTE);

		Integer nxTempMaxID = (Integer)pItem->GetItemData();
		std::map<Integer, KZoneDivisionInfoPtr>::iterator iter = m_mapZoneIntegrationInfo.find(nxTempMaxID);
		std::map<Integer, KZoneDivisionInfoPtr>::iterator end  = m_mapZoneIntegrationInfo.end();
		if (iter == end) continue;

		switch(IsInputDone(iter->second))
		{
		case KEMNewInputOK:
			/*pItemNote->SetCaption(_T("Select Complete"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemNote->SetCaption(_T("선택완료"));
			}
			pItemNote->SetBackgroundColor(RGB(0, 255, 0));*/
			++nCompleteCount;
			break;
		case KEMNewInputErrorMap:
			/*pItemNote->SetCaption(_T("Not Added"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemNote->SetCaption(_T("존지정(X)"));
			}
			pItemNote->SetBackgroundColor(RGB(255, 201, 14));*/
			break;
		case KEMNewInputErrorZoneList:
			/*pItemNote->SetCaption(_T("Please Add"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemNote->SetCaption(_T("미입력(존)"));
			}
			pItemNote->SetBackgroundColor(RGB(255, 201, 14));*/
			break;
		case KEMNewInputErrorZoneShort:
			/*pItemNote->SetCaption(_T("Please Add"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemNote->SetCaption(_T("추가입력(존)"));
			}
			pItemNote->SetBackgroundColor(RGB(255, 201, 14));*/
			break;
		default:
			/*pItemNote->SetCaption(_T("-"));
			pItemNote->SetBackgroundColor(RGB(255, 0, 0));*/
			break;
		} 
	}    

	//m_wndAAAZoneReportCtrl.Populate();
	m_wndAAAZoneReportCtrl.RedrawControl();
	if ((nRecordCount == 0) || (nRecordCount != nCompleteCount))
	{
		GetDlgItem(IDC_BTN_MULTI_SCENARIO)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_MULTI_SCENARIO)->EnableWindow(TRUE);
	}
}


void KZoningIntegrationDlg::DrawZoneAccess(std::vector<Integer> &a_vecZoneID)
{
	try
	{
		ClearDrawInfo();

		std::vector<TDrawZoneAccess> vecZoneAccess;
		std::vector<TDrawNodeInfo>   vecNodeInfo;

		size_t  nxCount = a_vecZoneID.size();

		for (size_t i = 0; i < nxCount; i++)
		{
			Integer &nxZoneID = a_vecZoneID[i];

			TDrawZoneAccess oDrawZoneAccess;
			oDrawZoneAccess.TNodeID = nxZoneID;
			TPreferenceSymbol &ZoneSymbol = ImChampMapDefineSymbol::ZoneSymbol;
			oDrawZoneAccess.TSymbol = ZoneSymbol.nCharIndex;
			oDrawZoneAccess.TColorRGB = ZoneSymbol.clrSymbol;
			oDrawZoneAccess.TSize = ZoneSymbol.dSize;
			CString strZoneID(_T(""));
			strZoneID.Format(_T("%I64d"), nxZoneID);
			oDrawZoneAccess.TLabel = strZoneID;
			oDrawZoneAccess.TRadius = 1.0;

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = nxZoneID;
			oNodeInfo.TLabel = strZoneID;
			oNodeInfo.TColorRGB = ZoneSymbol.clrSymbol;
			oNodeInfo.TSize = ZoneSymbol.dSize;
			oNodeInfo.TSymbol = ZoneSymbol.nCharIndex;

			vecNodeInfo.push_back(oNodeInfo);

			vecZoneAccess.push_back(oDrawZoneAccess);
		}

		m_pMapView->TxDrawCompositeNode(vecNodeInfo, false, false);
		m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, true);

		m_pMapView->RefreshAll();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningIntegrationDlg::ClearDrawInfo()
{
	try
	{
		m_pMapView->TxClearCompositeThemes();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


size_t KZoningIntegrationDlg::ConvertZoneListToVector(CString a_strZoneList, std::vector<Integer> &a_vecOriginID)
{
	if (a_strZoneList.IsEmpty())
		return 0;

	a_vecOriginID.clear();

	int nPos = 0;
	CString strList = a_strZoneList;
	strList.Trim();
	CString strZone = strList.Tokenize(_T(","), nPos);
	while (strZone != _T(""))
	{
		Integer nxZoneID = _ttoi64(strZone);
		a_vecOriginID.push_back(nxZoneID);
		strZone = a_strZoneList.Tokenize(_T(","), nPos);
	}

	return a_vecOriginID.size();
}


KEMNewInputError KZoningIntegrationDlg::IsInputDone(KZoneDivisionInfoPtr a_spZoningIntegrationZoneInfoPtr)
{
	std::vector<Integer> vecOriginID;
	ConvertZoneListToVector(a_spZoningIntegrationZoneInfoPtr->TIntegrationZoneList, vecOriginID);

	if (vecOriginID.size() == 0)
	{
		return KEMNewInputErrorZoneList;
	}
	else if (vecOriginID.size() < 2)
	{
		return KEMNewInputErrorZoneShort;
	}

	if (a_spZoningIntegrationZoneInfoPtr->TNodeGeometry == NULL || a_spZoningIntegrationZoneInfoPtr->TDemarcationGeometry == NULL)
	{
		return KEMNewInputErrorMap;
	}

	return KEMNewInputOK;
}


void KZoningIntegrationDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(_1_ORIGIN_COLUMN_ZONEID);

	Integer nxZoneID = (Integer)pItem->GetItemData();

	m_pMapView->PanToFeature(KLayerID::Node(), nxZoneID);
}


BOOL KZoningIntegrationDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	try
	{
		int     nMsgCode = pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return KDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void KZoningIntegrationDlg::ModelessCancleFeedback(UINT nDialogID)
{

}

CString KZoningIntegrationDlg::ConvertSavePath(CString a_strFileName)
{
	CString strSavePath(_T(""));
	//strSavePath.Format(_T("%s%s%s\\%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, ZONING_FOLDER_NAME,a_strFileName);
	strSavePath.Format(_T("%s%s%s\\%s"), m_pRunningTarget->GetLocation(), DLL_READ_WRITE_PATH, ZONING_FOLDER_NAME, a_strFileName);
	return strSavePath;
}


void KZoningIntegrationDlg::OnBnClickedBtnMultiScenario()
{
	UpdateData(TRUE);

	std::set<__int64> setZoneId;
	ConvertZoneIntegrationInfoToZoneOriginSet(setZoneId);

	size_t zoneCount = setZoneId.size();


	KZoningMultiScenarioSelectDlg dlg(KEMZoningOperationIntegration);
	dlg.SetTarget(m_pTarget);
	dlg.SetSelectedZoneNodeSet(setZoneId);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	bool bSuccessAll = true;

	std::vector<KTarget*> arrTarget = dlg.GetSelectedTarget();
	for (KTarget* pTarget : arrTarget)
	{
		m_pRunningTarget = pTarget;

		m_nResultCode = DLL_UNEXPECTED_ERROR;

		CString strError;
		if (!InputCheck(strError))
		{
			AfxMessageBox(strError);
			return;
		}

		GenerateNewZoneID();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(IntegrationZoneThreadCaller, this, false, nLang);

		if (DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{

		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
			AfxMessageBox(_T("존 세분화/집합화가 중단되었습니다."));
			return;
		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			AfxMessageBox(_T("Not Found Dll"));
			return;
		}
		else if (DLL_CREATE_ARGFILE_ERROR == m_nResultCode)
		{
			AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
			return;
		}
		else
		{
			AfxMessageBox(_T("존 집합화 수행 중 오류가 발생하였습니다."));
			return;
		}

		CWaitCursor wc;

		bool bTemp = ReadOutPutFile();
		if (!bTemp)
		{
			AfxMessageBox(_T("집합화한 존 정보 입력에 실패하였습니다."));
			TxLogDebug(_T("Output Integration 정보 입력 실패..."));
			return;
		}

		if (!UpdateIntegrationSystemInfo())
		{
			AfxMessageBox(_T("존 관련 정보 입력에 실패하였습니다."));
			return;
		}

		if (m_bRunIntegrationSucceess)
		{
			//ClearDrawInfo();
			if (m_pRunningTarget == m_pTarget)
			{
				ClearDrawInfo();
			}
		}
		else
		{
			bSuccessAll = false;
			KMapView* pMapView = ImChampFrameWindow::GetCreatedMapView(m_pRunningTarget);
			if (nullptr != pMapView)
			{
				pMapView->MapRefresh();
			}

		}

		NotifyRelationTable();
	}

	if (bSuccessAll)
	{
		// 항목 리셋
		m_wndAAAZoneReportCtrl.ResetContent();
		m_wndBBBZoneReportCtrl.ResetContent();
		m_mapZoneIntegrationInfo.clear();

		if (KmzSystem::GetLanguage() == KEMKorea) {
			SetDlgItemText(IDC_STATIC_Division, _T("집합화 할 존 노드 번호 : -"));
		}
		else {
			SetDlgItemText(IDC_STATIC_Division, _T("Integrated New Zone [ID] including : -"));
		}
	}

	GetDlgItem(IDC_BTN_MULTI_SCENARIO)->EnableWindow(FALSE);
}


size_t  KZoningIntegrationDlg::ConvertZoneIntegrationInfoToZoneOriginSet(std::set<Integer> &a_setOriginID)
{
	for (std::map<Integer, KZoneDivisionInfoPtr>::iterator it = m_mapZoneIntegrationInfo.begin();
		it != m_mapZoneIntegrationInfo.end(); ++it)
	{
		KZoneDivisionInfoPtr& sp = it->second;
		int nPos = 0;
		CString strList = sp->TIntegrationZoneList;
		strList.Trim();
		CString strZone = strList.Tokenize(_T(","), nPos);
		while (strZone != _T(""))
		{
			Integer nxZoneID = _ttoi64(strZone);
			a_setOriginID.insert(nxZoneID);
			strZone = strList.Tokenize(_T(","), nPos);
		}
	}

	return a_setOriginID.size();
}



/**************************************************************************/
/* 2019.01.15 수정 : 여러 시나리오 처리기능 추가에 따른 변경                     */
void KZoningIntegrationDlg::InsertZoneConnectLink(KDBaseConPtr a_spDBaseConnection, std::vector<TLinkFTNodeID>& arrZoneConnectLink)
{
	// insert	
	CString  strSQLInsert = _T(
		" Insert Into Link (link_id, fnode_id, tnode_id, link_type, length "
		" , lane, capa, speed, function_id, alpha, beta, a, b) "
		" VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
	);

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs(ConvertSavePath(T_DAT_OUT_ZONECONNECTLINK), std::ios::binary);

	if (ifs)
	{
		TxLogDebug(_T("--- start insert zone connect link result"));
		int nCount = 0;

		while (!ifs.eof())
		{
			Integer nLinkId, nFNodeId, nTNodeId;
			int nLinkType, nLane, nFunctionId;
			double dLength, dCapa, dSpeed, dAlpha, dBeta, dA, dB;

			ifs.read(reinterpret_cast<char*>(&nLinkId), sizeof(Integer));
			ifs.read(reinterpret_cast<char*>(&nFNodeId), sizeof(Integer));
			ifs.read(reinterpret_cast<char*>(&nTNodeId), sizeof(Integer));

			ifs.read(reinterpret_cast<char*>(&nLinkType), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&dLength), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&nLane), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&dCapa), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&dSpeed), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&nFunctionId), sizeof(int));

			ifs.read(reinterpret_cast<char*>(&dAlpha), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&dBeta), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&dA), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&dB), sizeof(double));

			int idx = 1;
			spPrepareStatement->BindInt64(idx++, nLinkId);		// 1
			spPrepareStatement->BindInt64(idx++, nFNodeId);		// 2
			spPrepareStatement->BindInt64(idx++, nTNodeId);		// 3
			spPrepareStatement->BindInt(idx++, nLinkType);		// 4
			spPrepareStatement->BindDouble(idx++, dLength);		// 5
			spPrepareStatement->BindInt(idx++, nLane);			// 6
			spPrepareStatement->BindDouble(idx++, dCapa);		// 7
			spPrepareStatement->BindDouble(idx++, dSpeed);		// 8
			spPrepareStatement->BindInt(idx++, nFunctionId);	// 9
			spPrepareStatement->BindDouble(idx++, dAlpha);		// 10
			spPrepareStatement->BindDouble(idx++, dBeta);		// 11
			spPrepareStatement->BindDouble(idx++, dA);			// 12
			spPrepareStatement->BindDouble(idx++, dB);			// 13

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			TLinkFTNodeID zoneConnLink = { nLinkId, nFNodeId, nTNodeId };
			arrZoneConnectLink.push_back(zoneConnLink);

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert zone connect link size : %d"), nCount);
		TxLogDebug((LPCTSTR)strResult);
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningIntegrationDlg::SaveZoneConnectLinkGeometry(std::vector<TLinkFTNodeID>& arrZoneConnectLink, KMapView* pMapView)
{
	try
	{
		TxFeatureLayerPtr  spNodeLayer; {
			ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Node());
			spNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
		}

		TxFeatureLayerPtr  spLinkLayer; {
			ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Link());
			spLinkLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
		}

		std::map<Integer, ITxGeometryPtr> mapNodeGeom;
		spNodeLayer->GetGeometry(mapNodeGeom);

		for (TLinkFTNodeID& linkInfo : arrZoneConnectLink)
		{
			std::vector<TxPoint> arrPoints;
			std::map<Integer, ITxGeometryPtr>::iterator itFNode = mapNodeGeom.find(linkInfo.TFromNodeID);
			if (mapNodeGeom.end() == itFNode || itFNode->second == nullptr)
			{
				CString strError;
				strError.Format(_T("From Node not exists : LINK_ID=%I64d, FNODE=%I64d, TNODE=%i64d"),
					linkInfo.TLinkID, linkInfo.TFromNodeID, linkInfo.TToNodeID);
				TxLogDebug(strError);
				continue;
			}

			std::map<Integer, ITxGeometryPtr>::iterator itTNode = mapNodeGeom.find(linkInfo.TToNodeID);
			if (mapNodeGeom.end() == itTNode || itTNode->second == nullptr)
			{
				CString strError;
				strError.Format(_T("To Node not exists : LINK_ID=%I64d, FNODE=%I64d, TNODE=%i64d"),
					linkInfo.TLinkID, linkInfo.TFromNodeID, linkInfo.TToNodeID);
				TxLogDebug(strError);
				continue;
			}

			TxPointPtr spFNode = std::dynamic_pointer_cast<TxPoint>(itFNode->second);
			TxPointPtr spTNode = std::dynamic_pointer_cast<TxPoint>(itTNode->second);

			TxPolylinePtr spZoneConnectLink = std::make_shared<TxPolyline>(spFNode, spTNode);

			spLinkLayer->ProhibitGeoInsert(linkInfo.TLinkID, spZoneConnectLink);
		}
	}
	catch (...)
	{
		CString strError = _T("Zone 세분화 처리 중 Zone connect link 생성 처리 오류(using mapview)");
		ThrowException(strError);
	}

}


void KZoningIntegrationDlg::SaveZoneConnectLinkGeometry(std::vector<TLinkFTNodeID>& arrZoneConnectLink, KDBaseConPtr a_spDBaseConnection)
{
	try
	{
		ITxMapServerPtr spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, a_spDBaseConnection->GetConnectionString().m_strUrl));
		if (spMapServer->Connect(a_spDBaseConnection) != 1)
			throw 1;

		TxFeatureLayerPtr  spNodeLayer = nullptr;
		TxFeatureLayerPtr  spLinkLayer = nullptr;

		// 서버의 레이어 정보 조회
		std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
		for (size_t j = 0; j < vecLayerInfo.size(); j++) {
			TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

			if (_tcsicmp(TABLE_NODE, spLayerInfo->Name()) == 0) { // 노드 레이어 조회
				spNodeLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}

			if (_tcsicmp(TABLE_LINK, spLayerInfo->Name()) == 0) { // 링크 레이어 조회
				spLinkLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}
		}

		if (spNodeLayer == nullptr || spLinkLayer == nullptr)
		{
			throw _T("some layer(s) is(are) not exists");
		}

		std::map<Integer, ITxGeometryPtr> mapNodeGeom;
		spNodeLayer->GetGeometry(mapNodeGeom);

		for (TLinkFTNodeID& linkInfo : arrZoneConnectLink)
		{
			std::vector<TxPoint> arrPoints;
			std::map<Integer, ITxGeometryPtr>::iterator itFNode = mapNodeGeom.find(linkInfo.TFromNodeID);
			if (mapNodeGeom.end() == itFNode || itFNode->second == nullptr)
			{
				CString strError;
				strError.Format(_T("From Node not exists : LINK_ID=%I64d, FNODE=%I64d, TNODE=%i64d"),
					linkInfo.TLinkID, linkInfo.TFromNodeID, linkInfo.TToNodeID);
				TxLogDebug(strError);
				continue;
			}

			std::map<Integer, ITxGeometryPtr>::iterator itTNode = mapNodeGeom.find(linkInfo.TToNodeID);
			if (mapNodeGeom.end() == itTNode || itTNode->second == nullptr)
			{
				CString strError;
				strError.Format(_T("To Node not exists : LINK_ID=%I64d, FNODE=%I64d, TNODE=%i64d"),
					linkInfo.TLinkID, linkInfo.TFromNodeID, linkInfo.TToNodeID);
				TxLogDebug(strError);
				continue;
			}

			TxPointPtr spFNode = std::dynamic_pointer_cast<TxPoint>(itFNode->second);
			TxPointPtr spTNode = std::dynamic_pointer_cast<TxPoint>(itTNode->second);

			TxPolylinePtr spZoneConnectLink = std::make_shared<TxPolyline>(spFNode, spTNode);

			spLinkLayer->ProhibitGeoInsert(linkInfo.TLinkID, spZoneConnectLink);
		}
	}
	catch (...)
	{
		CString strError = _T("Zone 세분화 처리 중 Zone connect link 생성 처리 오류(using KDBaseConPtr)");
		ThrowException(strError);
	}
}
/**************************************************************************/