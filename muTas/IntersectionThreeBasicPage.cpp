/**
 * @file IntersectionThreeBasicPage.cpp
 * @brief KIntersectionThreeBasicPage 구현 파일
 * @author jyk@qbicware.com
 * @date 2012.06.11
 * @remark
 */

#include "stdafx.h"
#include "IntersectionThreeBasicPage.h"
#include "afxdialogex.h"
//^#include "DBaseConnector.h"

enum
{
	_0_COLUMN_TITLE = 0,
	_1_COLUMN_1B,
	_2_COLUMN_2B,
	_3_COLUMN_3B
};

#define SEGNAL_SEQ_SIZE 5
#define DEFAULT_SIGNAL_TIME_GREEN  27
#define DEFAULT_SIGNAL_TIME_YELLOW 3

// KIntersectionThreeBasicPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KIntersectionThreeBasicPage, KDialogEx)

KIntersectionThreeBasicPage::KIntersectionThreeBasicPage(KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo, CWnd* pParent)
	: KDialogEx(KIntersectionThreeBasicPage::IDD, pParent)
	, m_nRadioSignal(0)
	, m_spDBaseConnection(spDBaseConnection)
	, m_oTSelectIntersection(a_oTSelectIntersection)
	, m_mapBoundInfo(a_mapBoundInfo)
{

}

KIntersectionThreeBasicPage::~KIntersectionThreeBasicPage()
{
	m_backBrush.DeleteObject();
	m_backBrush2.DeleteObject();
}

void KIntersectionThreeBasicPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                 m_wndReportCtrl);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadioSignal);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_1, m_btnSignalSeq1);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_2, m_btnSignalSeq2);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_3, m_btnSignalSeq3);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_4, m_btnSignalSeq4);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_5, m_btnSignalSeq5);
}


BEGIN_MESSAGE_MAP(KIntersectionThreeBasicPage, KDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_MFCBUTTON1,  &KIntersectionThreeBasicPage::OnBnClickedBtnArrow01)
	
	ON_BN_CLICKED(IDC_MFCBUTTON3,  &KIntersectionThreeBasicPage::OnBnClickedBtnArrow03)
	
	ON_BN_CLICKED(IDC_MFCBUTTON5,  &KIntersectionThreeBasicPage::OnBnClickedBtnArrow05)
	ON_BN_CLICKED(IDC_MFCBUTTON6,  &KIntersectionThreeBasicPage::OnBnClickedBtnArrow06)
	ON_BN_CLICKED(IDC_MFCBUTTON7,  &KIntersectionThreeBasicPage::OnBnClickedBtnArrow07)
	ON_BN_CLICKED(IDC_MFCBUTTON8,  &KIntersectionThreeBasicPage::OnBnClickedBtnArrow08)
	ON_BN_CLICKED(IDC_MFCBUTTON9,  &KIntersectionThreeBasicPage::OnBnClickedBtnArrow09)
	ON_BN_CLICKED(IDC_MFCBUTTON10, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow10)
	ON_BN_CLICKED(IDC_MFCBUTTON11, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow11)
	ON_BN_CLICKED(IDC_MFCBUTTON12, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow12)
	ON_BN_CLICKED(IDC_MFCBUTTON13, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow13)
	ON_BN_CLICKED(IDC_MFCBUTTON14, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow14)
	ON_BN_CLICKED(IDC_MFCBUTTON15, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow15)
	ON_BN_CLICKED(IDC_MFCBUTTON16, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow16)
	ON_BN_CLICKED(IDC_MFCBUTTON23, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow17)
	ON_BN_CLICKED(IDC_MFCBUTTON24, &KIntersectionThreeBasicPage::OnBnClickedBtnArrow18)
	ON_BN_CLICKED(IDC_BUTTON21, &KIntersectionThreeBasicPage::OnBnClickedBtnSetOne)
	ON_BN_CLICKED(IDC_BUTTON22, &KIntersectionThreeBasicPage::OnBnClickedBtnSetTwo)
	ON_BN_CLICKED(IDC_BUTTON23, &KIntersectionThreeBasicPage::OnBnClickedBtnAllRed)
	ON_BN_CLICKED(IDC_RADIO1, &KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqOne)
	ON_BN_CLICKED(IDC_RADIO2, &KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqTwo)
	ON_BN_CLICKED(IDC_RADIO3, &KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqThree)
	ON_BN_CLICKED(IDC_RADIO4, &KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqFour)
	ON_BN_CLICKED(IDC_RADIO5, &KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqFive)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_1, &KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqOne)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_2, &KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqTwo)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_3, &KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqThree)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_4, &KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqFour)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_5, &KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqFive)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_1, &KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqOne)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_2, &KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqTwo)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_3, &KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqThree)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_4, &KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqFour)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_5, &KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqFive)
	ON_EN_CHANGE(IDC_EDIT1,   &KIntersectionThreeBasicPage::OnEnChangeEditLampGreen1)
	ON_EN_CHANGE(IDC_EDIT2,   &KIntersectionThreeBasicPage::OnEnChangeEditLampGreen2)
	ON_EN_CHANGE(IDC_EDIT3,   &KIntersectionThreeBasicPage::OnEnChangeEditLampGreen3)
	ON_EN_CHANGE(IDC_EDIT4,   &KIntersectionThreeBasicPage::OnEnChangeEditLampGreen4)
	ON_EN_CHANGE(IDC_EDIT8,   &KIntersectionThreeBasicPage::OnEnChangeEditLampGreen5)
	ON_EN_CHANGE(IDC_EDIT13,  &KIntersectionThreeBasicPage::OnEnChangeEditLampYellow1)
	ON_EN_CHANGE(IDC_EDIT18,  &KIntersectionThreeBasicPage::OnEnChangeEditLampYellow2)
	ON_EN_CHANGE(IDC_EDIT19,  &KIntersectionThreeBasicPage::OnEnChangeEditLampYellow3)
	ON_EN_CHANGE(IDC_EDIT20,  &KIntersectionThreeBasicPage::OnEnChangeEditLampYellow4)
	ON_EN_CHANGE(IDC_EDIT21,  &KIntersectionThreeBasicPage::OnEnChangeEditLampYellow5)

END_MESSAGE_MAP()


// KIntersectionThreeBasicPage 메시지 처리기입니다.
BOOL KIntersectionThreeBasicPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE, FALSE, TRUE);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq1);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq2);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq3);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq4);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq5);

	m_vecLampGreenCtrlID.push_back(IDC_EDIT1);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT2);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT3);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT4);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT8);

	m_vecLampYellowCtrlID.push_back(IDC_EDIT13);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT18);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT19);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT20);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT21);

	RegisterDirectionImgID();

	if( GetSignalIndication() < 1 )
	{
		DefaultModuleDataSet(m_vecSignalSeq, m_oTSelectIntersection);
	}

	if( GetIntersectionInfo() < 1 )
	{
		DefaultModuleLaneBoundDataSet(m_oTSelectIntersection, m_mapBoundInfo, m_mapIntersectionInfo);
	}

	for( size_t i= 0; i< m_vecSignalSeq.size(); i++ )
	{
		TSignalIndication &oTSignalIndication = m_vecSignalSeq[i];
		SetDirectionSeq(oTSignalIndication.nSignalLampType, i, false);

		SetDlgItemInt(m_vecLampGreenCtrlID[i],  oTSignalIndication.nGreenLampDuration);
		SetDlgItemInt(m_vecLampYellowCtrlID[i], oTSignalIndication.nYellowLampDuration);
	}

	InitButtonCtrlStatus(m_oTSelectIntersection.nCrossType);
	
	InitReportHeader();
	InitReportControl();

	m_wndReportCtrl.Populate();

	m_backBrush.CreateSolidBrush(RGB(0,255,0));
	m_backBrush2.CreateSolidBrush(RGB(255,255,0));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


int KIntersectionThreeBasicPage::GetSignalIndication()
{
	for(int i= 0; i< SEGNAL_SEQ_SIZE; i++)
	{
		TSignalIndication oTSignal;
		oTSignal.nSignalLampType = KEMDirSeq_NONE;
		oTSignal.nGreenLampDuration  = 0;
		oTSignal.nYellowLampDuration = 0;

		m_vecSignalSeq.push_back(oTSignal);
	}

	CString strSQL;
	strSQL.Format (_T(" SELECT signal_seq, signal_lamp_type, green_lamp_duration, yellow_lamp_duration ")
		_T(" FROM Intersection_signal_indication ")
		_T(" WHERE node_id = %I64d order by signal_seq "), m_oTSelectIntersection.nxNodeID );

	KResultSetPtr pResult = m_spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return 0;
	}

	int nRecordCount = 0;
	while( pResult->Next() )
	{
		int nSignalSeq		                  = pResult->GetValueInt (0); // 1 ~ 5
		int nIdx                              = nSignalSeq -1;

		if( nSignalSeq > SEGNAL_SEQ_SIZE )
		{
			continue;
		}

		TSignalIndication &oSignalIndication  = m_vecSignalSeq[nIdx]; 

		oSignalIndication.nSignalLampType     = pResult->GetValueInt (1);
		oSignalIndication.nGreenLampDuration  = pResult->GetValueInt (2);
		oSignalIndication.nYellowLampDuration = pResult->GetValueInt (3);

		++nRecordCount;
	}

	return nRecordCount;
}


void KIntersectionThreeBasicPage::DefaultModuleDataSet(std::vector<TSignalIndication> &a_vecSignalSeq, TIntersectionBase &a_oTSelectIntersection)
{
	std::vector<int> vecDefaultSignalLampType;

	if( CROSS_TYPE_U == a_oTSelectIntersection.nCrossType )
	{
		vecDefaultSignalLampType.push_back(KEMDirSeq_EW_TH);
		vecDefaultSignalLampType.push_back(KEMDirSeq_W_THLT);
		vecDefaultSignalLampType.push_back(KEMDirSeq_N_LT);
	}
	else if( CROSS_TYPE_A  == a_oTSelectIntersection.nCrossType )
	{
		vecDefaultSignalLampType.push_back(KEMDirSeq_NS_TH);
		vecDefaultSignalLampType.push_back(KEMDirSeq_S_THLT);
		vecDefaultSignalLampType.push_back(KEMDirSeq_W_LT);
	}
	else if( CROSS_TYPE_E  == a_oTSelectIntersection.nCrossType )
	{
		vecDefaultSignalLampType.push_back(KEMDirSeq_NS_TH);
		vecDefaultSignalLampType.push_back(KEMDirSeq_N_THLT);
		vecDefaultSignalLampType.push_back(KEMDirSeq_E_LT);
	}
	else // CROSS_TYPE_O
	{
		vecDefaultSignalLampType.push_back(KEMDirSeq_EW_TH);
		vecDefaultSignalLampType.push_back(KEMDirSeq_E_THLT);
		vecDefaultSignalLampType.push_back(KEMDirSeq_S_LT);
	}

	for(size_t i= 0; i< vecDefaultSignalLampType.size(); i++)
	{
		TSignalIndication &oSignalIndication  = a_vecSignalSeq[i]; 

		oSignalIndication.nSignalLampType     = vecDefaultSignalLampType[i];

		oSignalIndication.nGreenLampDuration  = DEFAULT_SIGNAL_TIME_GREEN;
		oSignalIndication.nYellowLampDuration = DEFAULT_SIGNAL_TIME_YELLOW;
	}
}


int KIntersectionThreeBasicPage::GetIntersectionInfo()
{
	CString strSQL;
	strSQL.Format (_T(" SELECT node_id, bound_direction, lt, th, rt, signal_type, lt_type, rt_type ")
		_T(" FROM Intersection_info ")
		_T(" WHERE node_id = %I64d order by bound_direction "), m_oTSelectIntersection.nxNodeID );

	KResultSetPtr pResult = m_spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return 0;
	}

	int  nRecordCount = 0;
	while( pResult->Next() )
	{
		TIntersectionInfo oTIntersectionInfo;

		oTIntersectionInfo.nxNodeID         = pResult->GetValueInt64(0);
		oTIntersectionInfo.nBoundDirection  = pResult->GetValueInt  (1);
		oTIntersectionInfo.nLeftTurnCnt     = pResult->GetValueInt  (2);
		oTIntersectionInfo.nThrowCnt        = pResult->GetValueInt  (3);
		oTIntersectionInfo.nRightTurnCnt    = pResult->GetValueInt  (4);
		oTIntersectionInfo.nSignalType      = pResult->GetValueInt  (5);
		oTIntersectionInfo.nLeftTurnType    = pResult->GetValueInt  (6);
		oTIntersectionInfo.nRightTurnType   = pResult->GetValueInt  (7);

		m_mapIntersectionInfo.insert(std::make_pair(oTIntersectionInfo.nBoundDirection, oTIntersectionInfo));

		++nRecordCount;
	}

	return nRecordCount;
}


void KIntersectionThreeBasicPage::DefaultModuleLaneBoundDataSet(TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo, std::map<int, TIntersectionInfo> &a_mapIntersectionInfo)
{
	try
	{
		std::vector<int>              vecMainRoadNum;
		std::vector<int>              vecSubRoadNum;
		std::vector<Integer>          vecBoundNodeID;

		Integer nxSubBoundNodeID = a_oTSelectIntersection.nxFourthBNode;
		vecBoundNodeID.push_back(a_oTSelectIntersection.nxFirstBNode);
		vecBoundNodeID.push_back(a_oTSelectIntersection.nxSecondBNode);
		vecBoundNodeID.push_back(a_oTSelectIntersection.nxThirdBNode);  // 이부분을 유동 적으로 변경 하면 될거야 첫번째 페이지 에서 변경한 노드 정보

		vecMainRoadNum.clear();
		vecSubRoadNum.clear();

		if( nxSubBoundNodeID == a_oTSelectIntersection.nxFirstBNode )
			vecSubRoadNum.push_back(0);
		if( nxSubBoundNodeID == a_oTSelectIntersection.nxSecondBNode )
			vecSubRoadNum.push_back(1);
		if( nxSubBoundNodeID == a_oTSelectIntersection.nxThirdBNode )
			vecSubRoadNum.push_back(2);

		if(vecSubRoadNum.size() < 1)
			return;

		int nSubNodeIdx = vecSubRoadNum[0];
		for(int i= 0; i< 2; i++)
		{
			nSubNodeIdx++;

			if(nSubNodeIdx > 2)
				nSubNodeIdx = 0;

			vecMainRoadNum.push_back(nSubNodeIdx);
		}

		TIntersectionInfo oTIntersectionInfo;
		// sub
		Integer nxBoundNodeID = vecBoundNodeID[vecSubRoadNum[0]];
		int     nLaneCnt      = GetBoundLaneCnt(a_mapBoundInfo, nxBoundNodeID);

		oTIntersectionInfo.nxNodeID         = a_oTSelectIntersection.nxNodeID;
		oTIntersectionInfo.nBoundDirection  = vecSubRoadNum[0] + 1;
		oTIntersectionInfo.nLeftTurnCnt     =  (nLaneCnt -1 < 1) ? 1 : (nLaneCnt -1);
		oTIntersectionInfo.nThrowCnt        = 0;
		oTIntersectionInfo.nRightTurnCnt    = 1;
		oTIntersectionInfo.nSignalType      = 0;
		oTIntersectionInfo.nLeftTurnType    = 0;
		oTIntersectionInfo.nRightTurnType   = 0;

		a_mapIntersectionInfo.insert(std::make_pair(oTIntersectionInfo.nBoundDirection, oTIntersectionInfo));

		// sub 에서 시계 방향으로 다음 (sub 기준 좌측)
		nxBoundNodeID = vecBoundNodeID[vecMainRoadNum[0]];
		nLaneCnt      = GetBoundLaneCnt(a_mapBoundInfo, nxBoundNodeID);

		oTIntersectionInfo.nxNodeID         = a_oTSelectIntersection.nxNodeID;
		oTIntersectionInfo.nBoundDirection  = vecMainRoadNum[0] + 1;
		oTIntersectionInfo.nLeftTurnCnt     = 0;
		oTIntersectionInfo.nThrowCnt        = nLaneCnt;
		oTIntersectionInfo.nRightTurnCnt    = 1;
		oTIntersectionInfo.nSignalType      = 1;
		oTIntersectionInfo.nLeftTurnType    = 2;
		oTIntersectionInfo.nRightTurnType   = 0;

		a_mapIntersectionInfo.insert(std::make_pair(oTIntersectionInfo.nBoundDirection, oTIntersectionInfo));

		// 시계 방향으로 다음 (sub 기준 우측)
		nxBoundNodeID = vecBoundNodeID[vecMainRoadNum[1]];
		nLaneCnt      = GetBoundLaneCnt(a_mapBoundInfo, nxBoundNodeID);

		oTIntersectionInfo.nxNodeID         = a_oTSelectIntersection.nxNodeID;
		oTIntersectionInfo.nBoundDirection  = vecMainRoadNum[1] + 1;
		oTIntersectionInfo.nLeftTurnCnt     = 1;
		oTIntersectionInfo.nThrowCnt        = nLaneCnt;
		oTIntersectionInfo.nRightTurnCnt    = 0;
		oTIntersectionInfo.nSignalType      = 1;
		oTIntersectionInfo.nLeftTurnType    = 1;
		oTIntersectionInfo.nRightTurnType   = 0;

		a_mapIntersectionInfo.insert(std::make_pair(oTIntersectionInfo.nBoundDirection, oTIntersectionInfo));
	}
	catch (...)
	{
		a_mapIntersectionInfo.clear();
		TxLogDebugException();	
	}
}


int KIntersectionThreeBasicPage::GetBoundLaneCnt(std::map<Integer, TBoundInfo> &a_mapBoundInfo, Integer a_nxBoundNodeID)
{
	std::map<Integer, TBoundInfo>::iterator iter;
	iter = a_mapBoundInfo.find(a_nxBoundNodeID);
	if(iter != a_mapBoundInfo.end())
	{
		TBoundInfo &oTBoundINfo = iter->second;
		return oTBoundINfo.nLaneCnt;
	}

	return 0;
}


void KIntersectionThreeBasicPage::RegisterDirectionImgID()
{
	TDirectionImgID oTDirectionImgID;
	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A01;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A01;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B01;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_EW_TH, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A03;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A03;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B03;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_NS_TH, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A05;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A05;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B05;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_E_THLT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A06;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A06;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B06;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_W_THLT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A07;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A07;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B07;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_N_THLT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A08;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A08;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B08;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_S_THLT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A09;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A09;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B09;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_E_TH, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A10;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A10;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B10;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_W_TH, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A11;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A11;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B11;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_N_TH, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A12;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A12;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B12;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_S_TH, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A13;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A13;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B13;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_E_LT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A14;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A14;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B14;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_W_LT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A15;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A15;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B15;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_N_LT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A16;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A16;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B16;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_S_LT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A17;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A17;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B17;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_EW_NoProtectLT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A18;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A18;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B18;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_NS_NoProtectLT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A20;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A20;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_A20;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_AllRed, oTDirectionImgID));
}


void KIntersectionThreeBasicPage::InitButtonCtrlStatus( int a_nCrossType )
{
	if( CROSS_TYPE_A == a_nCrossType )
	{
		GetDlgItem(IDC_MFCBUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON3)->EnableWindow(TRUE);

		GetDlgItem(IDC_MFCBUTTON5)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON6)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON7)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON8)->EnableWindow(TRUE);

		GetDlgItem(IDC_MFCBUTTON9)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON10)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON11)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON12)->EnableWindow(TRUE);

		GetDlgItem(IDC_MFCBUTTON13)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON14)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON15)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON16)->EnableWindow(TRUE);

		GetDlgItem(IDC_MFCBUTTON23)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON24)->EnableWindow(TRUE);
	}
	else if( CROSS_TYPE_E == a_nCrossType )
	{
		GetDlgItem(IDC_MFCBUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON3)->EnableWindow(TRUE);

		GetDlgItem(IDC_MFCBUTTON5)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON6)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON7)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON8)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON9)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON10)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON11)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON12)->EnableWindow(TRUE);

		GetDlgItem(IDC_MFCBUTTON13)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON14)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON15)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON16)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON23)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON24)->EnableWindow(TRUE);
	}
	else if( CROSS_TYPE_O == a_nCrossType )
	{
		GetDlgItem(IDC_MFCBUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON3)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON5)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON6)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON7)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON8)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON9)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON10)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON11)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON12)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON13)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON14)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON15)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON16)->EnableWindow(TRUE);

		GetDlgItem(IDC_MFCBUTTON23)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON24)->EnableWindow(FALSE);
	}
	else if( CROSS_TYPE_U == a_nCrossType )
	{
		GetDlgItem(IDC_MFCBUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON3)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON5)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON6)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON7)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON8)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON9)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON10)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON11)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON12)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON13)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON14)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON15)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON16)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBUTTON23)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON24)->EnableWindow(FALSE);
	}
}


void KIntersectionThreeBasicPage::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_TITLE, _T("Variable"), 90, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("입력 항목"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_1B,    _T("1stB"), 34));
	pColumn->SetHeaderAlignment(DT_CENTER); 

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_2B,    _T("2ndB"), 33));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_3B,    _T("3rdB"), 33));
	pColumn->SetHeaderAlignment(DT_CENTER);
}


void KIntersectionThreeBasicPage::InitReportControl()
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	std::vector<CString> vecTitle;
    if (KmzSystem::GetLanguage() == KEMKorea) {
        vecTitle.push_back(_T("차로수(좌회전"));
        vecTitle.push_back(_T("차로수(직진"));
        vecTitle.push_back(_T("차로수(우회전"));
        vecTitle.push_back(_T("신호운영"));
        vecTitle.push_back(_T("좌회전형태"));
        vecTitle.push_back(_T("우회전형태"));
    }
    else {
        vecTitle.push_back(_T("Lane LT"));
        vecTitle.push_back(_T("Lane TH"));
        vecTitle.push_back(_T("Lane RT"));
        vecTitle.push_back(_T("Signal Type"));
        vecTitle.push_back(_T("Left Turn Type"));
        vecTitle.push_back(_T("Right Turn Type"));
    }	

	for(size_t i= 0; i< vecTitle.size(); i++)
	{
		if( KEMInterSeqThrowCnt == i)
		{
			continue;
		}

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		// Title
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(vecTitle[i]));
		pItem->SetAlignment(xtpColumnTextCenter);

		// - 1stB
		double dValue = GetValue(KEMBoundDirectionCode1B, i);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextRight);

		// - 2ndB
		dValue = GetValue(KEMBoundDirectionCode2B, i);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextRight);

		// - 3rdB
		dValue = GetValue(KEMBoundDirectionCode3B, i);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextRight);
	}
}


double KIntersectionThreeBasicPage::GetValue( KEMBoundDirectionCode emDirectionCode,  size_t a_nVariable )
{
	std::map<int, TIntersectionInfo>::iterator iter = m_mapIntersectionInfo.find(emDirectionCode);

	int nValue;
	if(iter != m_mapIntersectionInfo.end())
	{
		TIntersectionInfo &oTIntersectionInfo = iter->second;

		if( KEMInterSeqLeftTurnCnt         == a_nVariable )
			nValue = oTIntersectionInfo.nLeftTurnCnt;
		else if( KEMInterSeqThrowCnt       == a_nVariable )
			nValue = oTIntersectionInfo.nThrowCnt;
		else if( KEMInterSeqRightTurnCnt   == a_nVariable )
			nValue = oTIntersectionInfo.nRightTurnCnt;
		else if( KEMInterSeqSignalType     == a_nVariable )
			nValue = oTIntersectionInfo.nSignalType;
		else if( KEMInterSeqLeftTurnType   == a_nVariable )
			nValue = oTIntersectionInfo.nLeftTurnType;
		else if( KEMInterSeqRightTurnType  == a_nVariable )
			nValue = oTIntersectionInfo.nRightTurnType;
		else
			return 0.0;
	}
	else 
	{
		return 0.0;
	}

	return (double)nValue;
}


void KIntersectionThreeBasicPage::SetValue( std::map<int, TIntersectionInfo> &a_mapIntersectionInfo, KEMBoundDirectionCode emDirectionCode,  int a_nRowIndex, int a_nValue )
{
	std::map<int, TIntersectionInfo>::iterator iter = a_mapIntersectionInfo.find(emDirectionCode);

	if(iter != a_mapIntersectionInfo.end())
	{
		TIntersectionInfo &oTIntersection = iter->second;

		oTIntersection.nxNodeID        = m_oTSelectIntersection.nxNodeID;
		oTIntersection.nBoundDirection = emDirectionCode;

		if( KEMInterSeqLeftTurnCnt        == a_nRowIndex )
			oTIntersection.nLeftTurnCnt   =  a_nValue;
		else if( KEMInterSeqThrowCnt      == a_nRowIndex )
			oTIntersection.nThrowCnt      =  a_nValue;
		else if( KEMInterSeqRightTurnCnt  == a_nRowIndex )
			oTIntersection.nRightTurnCnt  =  a_nValue;
		else if( KEMInterSeqSignalType    == a_nRowIndex )
			oTIntersection.nSignalType    =  a_nValue;
		else if( KEMInterSeqLeftTurnType  == a_nRowIndex )
			oTIntersection.nLeftTurnType  =  a_nValue;
		else if( KEMInterSeqRightTurnType == a_nRowIndex )
			oTIntersection.nRightTurnType =  a_nValue;
	}
	else 
	{
		TIntersectionInfo oTIntersection;

		oTIntersection.nxNodeID        = m_oTSelectIntersection.nxNodeID;
		oTIntersection.nBoundDirection = emDirectionCode;

		if( KEMInterSeqLeftTurnCnt        == a_nRowIndex )
			oTIntersection.nLeftTurnCnt   =  a_nValue;
		else if( KEMInterSeqThrowCnt      == a_nRowIndex )
			oTIntersection.nThrowCnt      =  a_nValue;
		else if( KEMInterSeqRightTurnCnt  == a_nRowIndex )
			oTIntersection.nRightTurnCnt  =  a_nValue;
		else if( KEMInterSeqSignalType    == a_nRowIndex )
			oTIntersection.nSignalType    =  a_nValue;
		else if( KEMInterSeqLeftTurnType  == a_nRowIndex )
			oTIntersection.nLeftTurnType  =  a_nValue;
		else if( KEMInterSeqRightTurnType == a_nRowIndex )
			oTIntersection.nRightTurnType =  a_nValue;

		a_mapIntersectionInfo.insert(std::make_pair(oTIntersection.nBoundDirection, oTIntersection));
	}
}


// BOOL KIntersectionThreeBasicPage::OnApply()
// {
// 	std::map<int, TIntersectionInfo> mapIntersectionInfo;
// 
// 	int nIdx = 0;
// 	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
// 	for( int i= 0; i< pRecords->GetCount(); i++ )
// 	{
// 		CXTPReportRecord*           pRecord     = pRecords->GetAt(i);
// 		CXTPReportRecordItemNumber* pItemNumber = NULL;  
// 
// 		if( nIdx == KEMInterSeqThrowCnt )
// 			nIdx++;
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_1B);
// 		SetValue( mapIntersectionInfo, KEMBoundDirectionCode1B, nIdx, (int)(pItemNumber->GetValue()) );
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_2B);
// 		SetValue( mapIntersectionInfo, KEMBoundDirectionCode2B, nIdx, (int)(pItemNumber->GetValue()) );
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_3B);
// 		SetValue( mapIntersectionInfo, KEMBoundDirectionCode3B, nIdx, (int)(pItemNumber->GetValue()) );
// 		nIdx++;
// 	}
// 
// 	if( false == UpsertIntersectionInfo(mapIntersectionInfo) )
// 	{
// 		AfxMessageBox(_T("Update Intersection_Info Data 오류 발생"));
// 		return FALSE;
// 	}
// 
// 	if(UpsertIntersectionSignalIndication() == false)
// 	{
// 		AfxMessageBox(_T("Update IntersectionSignalIndication Data 오류 발생"));
// 		return FALSE;
// 	}
// 
// 	KDialogEx::OnApply();
// 	return TRUE;
// }


BOOL KIntersectionThreeBasicPage::SaveBasicThree()
{
	std::map<int, TIntersectionInfo> mapIntersectionInfo;

	int nIdx = 0;
	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	for( int i= 0; i< pRecords->GetCount(); i++ )
	{
		CXTPReportRecord*           pRecord     = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pItemNumber = NULL;  

		if( nIdx == KEMInterSeqThrowCnt )
			nIdx++;

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_1B);
		SetValue( mapIntersectionInfo, KEMBoundDirectionCode1B, nIdx, (int)(pItemNumber->GetValue()) );

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_2B);
		SetValue( mapIntersectionInfo, KEMBoundDirectionCode2B, nIdx, (int)(pItemNumber->GetValue()) );

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_3B);
		SetValue( mapIntersectionInfo, KEMBoundDirectionCode3B, nIdx, (int)(pItemNumber->GetValue()) );
		nIdx++;
	}

	if( false == UpsertIntersectionInfo(mapIntersectionInfo) )
	{
		AfxMessageBox(_T("Update Intersection_Info Data 오류 발생"));
		return FALSE;
	}

	if(UpsertIntersectionSignalIndication() == false)
	{
		AfxMessageBox(_T("Update IntersectionSignalIndication Data 오류 발생"));
		return FALSE;
	}

	return TRUE;
}


bool KIntersectionThreeBasicPage::UpsertIntersectionInfo(std::map<int, TIntersectionInfo> &a_mapIntersectionInfo)
{
	CString  strSQLDeleteTable(_T(""));
	CString  strSQLInsert     (_T(""));

	strSQLDeleteTable.Format( _T(" Delete From Intersection_info where node_id = %I64d "), m_oTSelectIntersection.nxNodeID );
	strSQLInsert.Format(_T(" Insert Into Intersection_info ")
		_T(" (  node_id, bound_direction, lt, th, rt, signal_type, lt_type, rt_type ) ")
		_T(" Values ( ?, ?, ?, ?, ?, ?, ?, ? ) ") );
	try
	{  
		m_spDBaseConnection->BeginTransaction();

		if ( m_spDBaseConnection->ExecuteUpdate(strSQLDeleteTable) != SQLITE_DONE )
		{
			TxLogDebug(strSQLDeleteTable.AllocSysString());

			m_spDBaseConnection->RollBack();

			return false;
		}

		KPreparedStatementPtr prepareStatementPtr = m_spDBaseConnection->PrepareStatement(strSQLInsert);

		std::map<int, TIntersectionInfo>::iterator iter = a_mapIntersectionInfo.begin();
		std::map<int, TIntersectionInfo>::iterator end  = a_mapIntersectionInfo.end();

		while(iter != end)
		{
			int                nBooundDirection     = iter->first;
			TIntersectionInfo  &oTIntersectionInfo  = iter->second;

			prepareStatementPtr->BindInt64(1, oTIntersectionInfo.nxNodeID);
			prepareStatementPtr->BindInt  (2, nBooundDirection);
			prepareStatementPtr->BindInt  (3, oTIntersectionInfo.nLeftTurnCnt);
			//prepareStatementPtr->BindInt  (4, oTIntersectionInfo.nThrowCnt);
			prepareStatementPtr->BindInt  (4, 0); // jyk: 현재 3지에서는 직선 Lane 편집을 하지 않고 있다.
			prepareStatementPtr->BindInt  (5, oTIntersectionInfo.nRightTurnCnt);
			prepareStatementPtr->BindInt  (6, oTIntersectionInfo.nSignalType);
			prepareStatementPtr->BindInt  (7, oTIntersectionInfo.nLeftTurnType);
			prepareStatementPtr->BindInt  (8, oTIntersectionInfo.nRightTurnType);

			prepareStatementPtr->ExecuteUpdate();
			prepareStatementPtr->Reset();

			++iter;
		}    

		m_spDBaseConnection->Commit(); 

		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		m_spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return false;
}


bool KIntersectionThreeBasicPage::UpsertIntersectionSignalIndication()
{
	CString  strSQLDeleteTable(_T(""));
	CString  strSQLInsert     (_T(""));

	strSQLDeleteTable.Format( _T(" Delete From Intersection_signal_indication where node_id = %I64d "), m_oTSelectIntersection.nxNodeID );
	strSQLInsert.Format(_T(" Insert Into Intersection_signal_indication ")
		_T(" ( node_id, signal_seq, signal_lamp_type, green_lamp_duration, yellow_lamp_duration ) ")
		_T(" Values ( ?, ?, ?, ?, ? ) ") );
	try
	{  
		m_spDBaseConnection->BeginTransaction();

		if ( m_spDBaseConnection->ExecuteUpdate(strSQLDeleteTable) != SQLITE_DONE )
		{
			TxLogDebug(strSQLDeleteTable.AllocSysString());

			m_spDBaseConnection->RollBack();

			return false;
		}

		KPreparedStatementPtr prepareStatementPtr = m_spDBaseConnection->PrepareStatement(strSQLInsert);

		for(size_t i= 0; i< m_vecSignalSeq.size(); i++)
		{
			TSignalIndication &oTSignalIndication = m_vecSignalSeq[i];

			if( oTSignalIndication.nSignalLampType < 1 )
			{
				continue;
			}

			prepareStatementPtr->BindInt64(1, m_oTSelectIntersection.nxNodeID);
			prepareStatementPtr->BindInt  (2, (i+1)); 
			prepareStatementPtr->BindInt  (3, oTSignalIndication.nSignalLampType);
			prepareStatementPtr->BindInt  (4, oTSignalIndication.nGreenLampDuration);
			prepareStatementPtr->BindInt  (5, oTSignalIndication.nYellowLampDuration);

			prepareStatementPtr->ExecuteUpdate();
			prepareStatementPtr->Reset();
		}    

		m_spDBaseConnection->Commit(); 

		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		m_spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return false;
}


HBRUSH KIntersectionThreeBasicPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if( nCtlColor == CTLCOLOR_EDIT )
	{
		if( pWnd->GetDlgCtrlID() == IDC_EDIT1 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT2 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT3 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT4 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT8  )
		{
			pDC->SetBkColor( RGB( 255, 255, 255) );
			pDC->SetBkMode( TRANSPARENT );
			hbr = (HBRUSH)m_backBrush;
		}
		else if( pWnd->GetDlgCtrlID() == IDC_EDIT13 || 
			pWnd->GetDlgCtrlID() == IDC_EDIT18 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT19 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT20 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT21  )
		{
			pDC->SetBkColor( RGB( 255, 255, 255) );
			pDC->SetBkMode( TRANSPARENT );
			hbr = (HBRUSH)m_backBrush2;
		}
		else
		{
			hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	}
	else
	{
		hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL KIntersectionThreeBasicPage::OnEraseBkgnd(CDC* pDC) 
{
	//return TRUE;
	return KDialogEx::OnEraseBkgnd(pDC);
}


void KIntersectionThreeBasicPage::SetDirectionSeq(int a_nSignalLampType, bool a_bRedraw/*=true*/)
{
	UpdateData(TRUE);

	TSignalIndication &oTSignalIndication = m_vecSignalSeq[m_nRadioSignal];
	oTSignalIndication.nSignalLampType    = a_nSignalLampType;

	CMFCButton* pButton            = m_vecSignalBtnCtrl[m_nRadioSignal];	// 현시 방향 이미지 

	std::map<int, TDirectionImgID>::iterator iter = m_mapDirectionImgID.find(a_nSignalLampType);
	if(iter != m_mapDirectionImgID.end())
	{
		TDirectionImgID &oTDirectionImgID = iter->second;
		pButton->SetImage(oTDirectionImgID.uiBmpResId, oTDirectionImgID.uiBmpHotResId, oTDirectionImgID.uiBmpDsblResId);
	}
	else
	{
		pButton->SetImage((HBITMAP) NULL);

		/*oTSignalIndication.nGreenLampDuration  = 0;
		oTSignalIndication.nYellowLampDuration = 0;
		SetDlgItemInt(m_vecLampGreenCtrlID[m_nRadioSignal], 0);
		SetDlgItemInt(m_vecLampYellowCtrlID[m_nRadioSignal], 0);*/
	}
	pButton->RedrawWindow();
	UpdateData(FALSE);
}


void KIntersectionThreeBasicPage::SetDirectionSeq(int a_nSignalLampType, int a_nIndex, bool a_bRedraw/*=true*/)
{
	UpdateData(TRUE);

	TSignalIndication &oTSignalIndication = m_vecSignalSeq[a_nIndex];
	oTSignalIndication.nSignalLampType    = a_nSignalLampType;

	CMFCButton* pButton            = m_vecSignalBtnCtrl[a_nIndex];	// 현시 방향 이미지 

	std::map<int, TDirectionImgID>::iterator iter = m_mapDirectionImgID.find(a_nSignalLampType);
	if(iter != m_mapDirectionImgID.end())
	{
		TDirectionImgID &oTDirectionImgID = iter->second;
		pButton->SetImage(oTDirectionImgID.uiBmpResId, oTDirectionImgID.uiBmpHotResId, oTDirectionImgID.uiBmpDsblResId);
	}
	else
	{
		pButton->SetImage((HBITMAP) NULL);

		/*oTSignalIndication.nGreenLampDuration  = 0;
		oTSignalIndication.nYellowLampDuration = 0;
		SetDlgItemInt(m_vecLampGreenCtrlID[a_nIndex], 0);
		SetDlgItemInt(m_vecLampYellowCtrlID[a_nIndex], 0);*/
	}
	pButton->RedrawWindow();
	UpdateData(FALSE);
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow01()
{
	SetDirectionSeq(KEMDirSeq_EW_TH);	// 동서 양방향 직진
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow03()
{
	SetDirectionSeq(KEMDirSeq_NS_TH);	// 남북 양방향 직진
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow05()
{
	SetDirectionSeq(KEMDirSeq_E_THLT);	// 동쪽 방향 직좌
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow06()
{
	SetDirectionSeq(KEMDirSeq_W_THLT);	// 서쪽 방향 직좌
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow07()
{
	SetDirectionSeq(KEMDirSeq_N_THLT);	// 북쪽 방향 직좌
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow08()
{
	SetDirectionSeq(KEMDirSeq_S_THLT);	// 남쪽 방향 직좌
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow09()
{
	SetDirectionSeq(KEMDirSeq_E_TH);	// 동쪽 방향 직진
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow10()
{
	SetDirectionSeq(KEMDirSeq_W_TH);	// 서쪽 방향 직진
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow11()
{
	SetDirectionSeq(KEMDirSeq_N_TH);	// 북쪽 방향 직진
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow12()
{
	SetDirectionSeq(KEMDirSeq_S_TH);	// 남쪽 방향 직진
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow13()
{
	SetDirectionSeq(KEMDirSeq_E_LT);	// 동쪽 방향 좌회전
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow14()
{
	SetDirectionSeq(KEMDirSeq_W_LT);	// 서쪽 방향 좌회전
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow15()
{
	SetDirectionSeq(KEMDirSeq_N_LT);	// 북쪽 방향 좌회전
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow16()
{
	SetDirectionSeq(KEMDirSeq_S_LT);	// 남쪽 방향 좌회전
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow17()
{
	SetDirectionSeq(KEMDirSeq_EW_NoProtectLT);	// 동서 양방향 비보호 좌회전
}


void KIntersectionThreeBasicPage::OnBnClickedBtnArrow18()
{
	SetDirectionSeq(KEMDirSeq_NS_NoProtectLT);	// 남북 양방향 비보호 좌회전
}


void KIntersectionThreeBasicPage::OnBnClickedBtnAllRed()
{
	SetDirectionSeq(KEMDirSeq_AllRed);	        // All Red
}


void KIntersectionThreeBasicPage::OnBnClickedBtnSetOne()
{
	//m_oTSelectIntersection.nCrossType;

	/*SetDirectionSeq(KEMDirSeq_EW_LT, 1, false);
	SetDirectionSeq(KEMDirSeq_NS_TH, 2, false);
	SetDirectionSeq(KEMDirSeq_NS_LT, 3, false);

	for(int i = 4; i< SEGNAL_SEQ_SIZE; i++ )
	{
		SetDirectionSeq(KEMDirSeq_NONE, i, false);
	}

	SetDirectionSeq(KEMDirSeq_EW_TH, 0, true);*/
}


void KIntersectionThreeBasicPage::OnBnClickedBtnSetTwo()
{
	/*SetDirectionSeq(KEMDirSeq_W_THLT, 1, false);
	SetDirectionSeq(KEMDirSeq_N_THLT, 2, false);
	SetDirectionSeq(KEMDirSeq_S_THLT, 3, false);

	for(int i = 4; i< SEGNAL_SEQ_SIZE; i++ )
	{
		SetDirectionSeq(KEMDirSeq_NONE, i, false);
	}

	SetDirectionSeq(KEMDirSeq_E_THLT, 0, true);*/
}


void KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqOne()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqTwo()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqThree()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqFour()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnClickedRadioSignalSeqFive()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqOne()
{
	m_nRadioSignal = 0;
	UpdateData(FALSE);
}


void KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqTwo()
{
	m_nRadioSignal = 1;
	UpdateData(FALSE);
}


void KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqThree()
{
	m_nRadioSignal = 2;
	UpdateData(FALSE);
}


void KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqFour()
{
	m_nRadioSignal = 3;
	UpdateData(FALSE);
}


void KIntersectionThreeBasicPage::OnBnClickedMfcbuttonSigseqFive()
{
	m_nRadioSignal = 4;
	UpdateData(FALSE);
}


void KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqOne()
{
	m_nRadioSignal = 0;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqTwo()
{
	m_nRadioSignal = 1;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqThree()
{
	m_nRadioSignal = 2;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqFour()
{
	m_nRadioSignal = 3;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionThreeBasicPage::OnBnDoubleclickedMfcbuttonSigseqFive()
{
	m_nRadioSignal = 4;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampGreen1()
{
	SetGreenLampTime(0);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampGreen2()
{
	SetGreenLampTime(1);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampGreen3()
{
	SetGreenLampTime(2);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampGreen4()
{
	SetGreenLampTime(3);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampGreen5()
{
	SetGreenLampTime(4);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampYellow1()
{
	SetYellowLampTime(0);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampYellow2()
{
	SetYellowLampTime(1);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampYellow3()
{
	SetYellowLampTime(2);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampYellow4()
{
	SetYellowLampTime(3);
}

void KIntersectionThreeBasicPage::OnEnChangeEditLampYellow5()
{
	SetYellowLampTime(4);
}

void KIntersectionThreeBasicPage::SetGreenLampTime(int a_nSeq)
{
	UpdateData(TRUE);
	TSignalIndication &oTSignal = m_vecSignalSeq[a_nSeq];
	oTSignal.nGreenLampDuration = GetDlgItemInt(m_vecLampGreenCtrlID[a_nSeq]);
}

void KIntersectionThreeBasicPage::SetYellowLampTime(int a_nSeq)
{
	UpdateData(TRUE);
	TSignalIndication &oTSignal  = m_vecSignalSeq[a_nSeq];
	oTSignal.nYellowLampDuration = GetDlgItemInt(m_vecLampYellowCtrlID[a_nSeq]);
}
