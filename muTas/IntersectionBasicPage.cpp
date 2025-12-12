/**
 * @file IntersectionBasicPage.cpp
 * @brief KIntersectionBasicPage 구현 파일
 * @author jyk@qbicware.com
 * @date 2012.06.07
 * @remark
 */

#include "stdafx.h"
#include "IntersectionBasicPage.h"
#include "afxdialogex.h"
//^#include "DBaseConnector.h"

enum
{
	_0_COLUMN_TITLE = 0,
	_1_COLUMN_1B,
	_2_COLUMN_2B,
	_3_COLUMN_3B,
	_4_COLUMN_4B
};

#define SEGNAL_SEQ_SIZE 6
#define DEFAULT_SIGNAL_TIME_GREEN  27
#define DEFAULT_SIGNAL_TIME_YELLOW 3

// KIntersectionBasicPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KIntersectionBasicPage, KDialogEx)

KIntersectionBasicPage::KIntersectionBasicPage(KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo, CWnd* pParent)
	: KDialogEx(KIntersectionBasicPage::IDD, pParent)
	, m_nRadioSignal(0)
	, m_spDBaseConnection(spDBaseConnection)
	, m_oTSelectIntersection(a_oTSelectIntersection)
	, m_mapBoundInfo(a_mapBoundInfo)
{

}

KIntersectionBasicPage::~KIntersectionBasicPage()
{
	m_backBrush.DeleteObject();
	m_backBrush2.DeleteObject();
}

void KIntersectionBasicPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                 m_wndReportCtrl);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadioSignal);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_1, m_btnSignalSeq1);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_2, m_btnSignalSeq2);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_3, m_btnSignalSeq3);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_4, m_btnSignalSeq4);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_5, m_btnSignalSeq5);
	DDX_Control(pDX, IDC_MFCBUTTON_SIGSEQ_6, m_btnSignalSeq6);
}


BEGIN_MESSAGE_MAP(KIntersectionBasicPage, KDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_MFCBUTTON1,  &KIntersectionBasicPage::OnBnClickedBtnArrow01)
	ON_BN_CLICKED(IDC_MFCBUTTON2,  &KIntersectionBasicPage::OnBnClickedBtnArrow02)
	ON_BN_CLICKED(IDC_MFCBUTTON3,  &KIntersectionBasicPage::OnBnClickedBtnArrow03)
	ON_BN_CLICKED(IDC_MFCBUTTON4,  &KIntersectionBasicPage::OnBnClickedBtnArrow04)
	ON_BN_CLICKED(IDC_MFCBUTTON5,  &KIntersectionBasicPage::OnBnClickedBtnArrow05)
	ON_BN_CLICKED(IDC_MFCBUTTON6,  &KIntersectionBasicPage::OnBnClickedBtnArrow06)
	ON_BN_CLICKED(IDC_MFCBUTTON7,  &KIntersectionBasicPage::OnBnClickedBtnArrow07)
	ON_BN_CLICKED(IDC_MFCBUTTON8,  &KIntersectionBasicPage::OnBnClickedBtnArrow08)
	ON_BN_CLICKED(IDC_MFCBUTTON9,  &KIntersectionBasicPage::OnBnClickedBtnArrow09)
	ON_BN_CLICKED(IDC_MFCBUTTON10, &KIntersectionBasicPage::OnBnClickedBtnArrow10)
	ON_BN_CLICKED(IDC_MFCBUTTON11, &KIntersectionBasicPage::OnBnClickedBtnArrow11)
	ON_BN_CLICKED(IDC_MFCBUTTON12, &KIntersectionBasicPage::OnBnClickedBtnArrow12)
	ON_BN_CLICKED(IDC_MFCBUTTON13, &KIntersectionBasicPage::OnBnClickedBtnArrow13)
	ON_BN_CLICKED(IDC_MFCBUTTON14, &KIntersectionBasicPage::OnBnClickedBtnArrow14)
	ON_BN_CLICKED(IDC_MFCBUTTON15, &KIntersectionBasicPage::OnBnClickedBtnArrow15)
	ON_BN_CLICKED(IDC_MFCBUTTON16, &KIntersectionBasicPage::OnBnClickedBtnArrow16)
	ON_BN_CLICKED(IDC_MFCBUTTON23, &KIntersectionBasicPage::OnBnClickedBtnArrow17)
	ON_BN_CLICKED(IDC_MFCBUTTON24, &KIntersectionBasicPage::OnBnClickedBtnArrow18)
	ON_BN_CLICKED(IDC_BUTTON21, &KIntersectionBasicPage::OnBnClickedBtnSetOne)
	ON_BN_CLICKED(IDC_BUTTON22, &KIntersectionBasicPage::OnBnClickedBtnSetTwo)
	ON_BN_CLICKED(IDC_BUTTON23, &KIntersectionBasicPage::OnBnClickedBtnAllRed)
	ON_BN_CLICKED(IDC_RADIO1, &KIntersectionBasicPage::OnBnClickedRadioSignalSeqOne)
	ON_BN_CLICKED(IDC_RADIO2, &KIntersectionBasicPage::OnBnClickedRadioSignalSeqTwo)
	ON_BN_CLICKED(IDC_RADIO3, &KIntersectionBasicPage::OnBnClickedRadioSignalSeqThree)
	ON_BN_CLICKED(IDC_RADIO4, &KIntersectionBasicPage::OnBnClickedRadioSignalSeqFour)
	ON_BN_CLICKED(IDC_RADIO5, &KIntersectionBasicPage::OnBnClickedRadioSignalSeqFive)
	ON_BN_CLICKED(IDC_RADIO6, &KIntersectionBasicPage::OnBnClickedRadioSignalSeqSix)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_1, &KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqOne)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_2, &KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqTwo)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_3, &KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqThree)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_4, &KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqFour)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_5, &KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqFive)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIGSEQ_6, &KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqSix)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_1, &KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqOne)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_2, &KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqTwo)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_3, &KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqThree)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_4, &KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqFour)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_5, &KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqFive)
	ON_BN_DOUBLECLICKED(IDC_MFCBUTTON_SIGSEQ_6, &KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqSix)
	ON_EN_CHANGE(IDC_EDIT1,   &KIntersectionBasicPage::OnEnChangeEditLampGreen1)
	ON_EN_CHANGE(IDC_EDIT2,   &KIntersectionBasicPage::OnEnChangeEditLampGreen2)
	ON_EN_CHANGE(IDC_EDIT3,   &KIntersectionBasicPage::OnEnChangeEditLampGreen3)
	ON_EN_CHANGE(IDC_EDIT4,   &KIntersectionBasicPage::OnEnChangeEditLampGreen4)
	ON_EN_CHANGE(IDC_EDIT8,   &KIntersectionBasicPage::OnEnChangeEditLampGreen5)
	ON_EN_CHANGE(IDC_EDIT12,  &KIntersectionBasicPage::OnEnChangeEditLampGreen6)
	ON_EN_CHANGE(IDC_EDIT13,  &KIntersectionBasicPage::OnEnChangeEditLampYellow1)
	ON_EN_CHANGE(IDC_EDIT18,  &KIntersectionBasicPage::OnEnChangeEditLampYellow2)
	ON_EN_CHANGE(IDC_EDIT19,  &KIntersectionBasicPage::OnEnChangeEditLampYellow3)
	ON_EN_CHANGE(IDC_EDIT20,  &KIntersectionBasicPage::OnEnChangeEditLampYellow4)
	ON_EN_CHANGE(IDC_EDIT21,  &KIntersectionBasicPage::OnEnChangeEditLampYellow5)
	ON_EN_CHANGE(IDC_EDIT22,  &KIntersectionBasicPage::OnEnChangeEditLampYellow6)

END_MESSAGE_MAP()


// KIntersectionBasicPage 메시지 처리기입니다.
BOOL KIntersectionBasicPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE, FALSE, TRUE);

	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq1);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq2);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq3);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq4);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq5);
	m_vecSignalBtnCtrl.push_back(&m_btnSignalSeq6);

	m_vecLampGreenCtrlID.push_back(IDC_EDIT1);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT2);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT3);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT4);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT8);
	m_vecLampGreenCtrlID.push_back(IDC_EDIT12);

	m_vecLampYellowCtrlID.push_back(IDC_EDIT13);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT18);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT19);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT20);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT21);
	m_vecLampYellowCtrlID.push_back(IDC_EDIT22);
	
	RegisterDirectionImgID();

	if( GetSignalIndication() < 1 )
	{
		DefaultModuleDataSet(m_vecSignalSeq, m_mapIntersectionInfo);
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
	
	InitReportHeader();
	InitReportControl();

	m_wndReportCtrl.Populate();

	m_backBrush.CreateSolidBrush(RGB(0,255,0));
	m_backBrush2.CreateSolidBrush(RGB(255,255,0));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


int KIntersectionBasicPage::GetSignalIndication()
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
		int nSignalSeq		                  = pResult->GetValueInt (0); // 1 ~ 6
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


void KIntersectionBasicPage::DefaultModuleDataSet(std::vector<TSignalIndication> &a_vecSignalSeq, std::map<int, TIntersectionInfo> &a_mapIntersectionInfo)
{
	std::vector<int> vecDefaultSignalLampType;
	std::map<int, TIntersectionInfo>::iterator iter;
	std::vector<int> vecBoundDirSignalType;
	
	for(int i= 0; i< 4; i++)
	{
		iter = a_mapIntersectionInfo.find(KEMBoundDirectionCode1B + i);
		if( iter != a_mapIntersectionInfo.end() )
		{
			TIntersectionInfo &oTIntersectionInfo = iter->second;
			vecBoundDirSignalType.push_back(oTIntersectionInfo.nSignalType);
		}
		else
		{
			vecBoundDirSignalType.push_back(0);
		}
	}
	
	// 마주 보는 두 방향의 signalType이 다른 경우 무조건 1로 셋팅
	if( vecBoundDirSignalType[0] != vecBoundDirSignalType[2] )
	{
		vecBoundDirSignalType[0] = 1;
		vecBoundDirSignalType[2] = 1;
	}
	if( vecBoundDirSignalType[1] != vecBoundDirSignalType[3] )
	{
		vecBoundDirSignalType[1]  = 1;
		vecBoundDirSignalType[3]  = 1;
	}

	/*if( nBoundDir1_SignalType == 1 && nBoundDir3_SignalType == 1 )*/
	if( vecBoundDirSignalType[0] == 1 )
	{
		vecDefaultSignalLampType.push_back(KEMDirSeq_E_THLT);
		vecDefaultSignalLampType.push_back(KEMDirSeq_W_THLT);
	}
	/*if( nBoundDir1_SignalType == 0 && nBoundDir3_SignalType == 0 )*/
	else if( vecBoundDirSignalType[0] == 0 )
	{
		vecDefaultSignalLampType.push_back(KEMDirSeq_EW_TH);
		vecDefaultSignalLampType.push_back(KEMDirSeq_EW_LT);
	}

	/*if( nBoundDir2_SignalType == 1 && nBoundDir4_SignalType == 1 )*/
	if( vecBoundDirSignalType[1] == 1 )
	{
		vecDefaultSignalLampType.push_back(KEMDirSeq_N_THLT);
		vecDefaultSignalLampType.push_back(KEMDirSeq_S_THLT);
	}
	/*if( nBoundDir2_SignalType == 0 && nBoundDir4_SignalType == 0 )*/
	else if( vecBoundDirSignalType[1] == 0 )
	{
		vecDefaultSignalLampType.push_back(KEMDirSeq_NS_TH);
		vecDefaultSignalLampType.push_back(KEMDirSeq_NS_LT);
	}

// 	for(int i= 0; i< (SEGNAL_SEQ_SIZE -2); i++)
// 	{
// 		TSignalIndication &oSignalIndication  = m_vecSignalSeq[i]; 
// 
// 		oSignalIndication.nGreenLampDuration  = DEFAULT_SIGNAL_TIME_GREEN;
// 		oSignalIndication.nYellowLampDuration = DEFAULT_SIGNAL_TIME_YELLOW;
// 	}

	for(size_t i= 0; i< vecDefaultSignalLampType.size(); i++)
	{
		TSignalIndication &oSignalIndication  = a_vecSignalSeq[i]; 

		oSignalIndication.nSignalLampType     = vecDefaultSignalLampType[i];

		oSignalIndication.nGreenLampDuration  = DEFAULT_SIGNAL_TIME_GREEN;
		oSignalIndication.nYellowLampDuration = DEFAULT_SIGNAL_TIME_YELLOW;
	}
}


int  KIntersectionBasicPage::GetIntersectionInfo()
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


void KIntersectionBasicPage::DefaultModuleLaneBoundDataSet(TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo, std::map<int, TIntersectionInfo> &a_mapIntersectionInfo)
{
	try
	{
		std::vector<Integer>          vecBoundNodeID;

		vecBoundNodeID.push_back(a_oTSelectIntersection.nxFirstBNode);
		vecBoundNodeID.push_back(a_oTSelectIntersection.nxSecondBNode);
		vecBoundNodeID.push_back(a_oTSelectIntersection.nxThirdBNode); 
		vecBoundNodeID.push_back(a_oTSelectIntersection.nxFourthBNode); 

		for(int i= 0; i< 4; i++)
		{
			TIntersectionInfo oTIntersectionInfo;

			Integer nxBoundNodeID = vecBoundNodeID[i];
			int     nLaneCnt      = GetBoundLaneCnt(a_mapBoundInfo, nxBoundNodeID);

			oTIntersectionInfo.nxNodeID             = a_oTSelectIntersection.nxNodeID;
			oTIntersectionInfo.nBoundDirection      = KEMBoundDirectionCode1B + i;
			oTIntersectionInfo.nLeftTurnCnt         = 1;
			oTIntersectionInfo.nThrowCnt            = (nLaneCnt < 3) ? nLaneCnt : (nLaneCnt-1);
			oTIntersectionInfo.nRightTurnCnt        = 1;

			if( nLaneCnt <= 1 )
			{
				oTIntersectionInfo.nSignalType      = 1;
				oTIntersectionInfo.nLeftTurnType    = 1;
				oTIntersectionInfo.nRightTurnType   = 0;
			}
			else if( nLaneCnt == 2 )
			{
				oTIntersectionInfo.nSignalType      = 1;
				oTIntersectionInfo.nLeftTurnType    = 1;
				oTIntersectionInfo.nRightTurnType   = 0;
			}
			else if( nLaneCnt == 3 )
			{
				oTIntersectionInfo.nSignalType      = 0;
				oTIntersectionInfo.nLeftTurnType    = 0;
				oTIntersectionInfo.nRightTurnType   = 0;
			}
			else
			{
				oTIntersectionInfo.nSignalType      = 0;
				oTIntersectionInfo.nLeftTurnType    = 0;
				oTIntersectionInfo.nRightTurnType   = 1;
			}

			a_mapIntersectionInfo.insert(std::make_pair(oTIntersectionInfo.nBoundDirection, oTIntersectionInfo));
		}
	}
	catch (...)
	{
		a_mapIntersectionInfo.clear();
		TxLogDebugException();	
	}

}


int KIntersectionBasicPage::GetBoundLaneCnt(std::map<Integer, TBoundInfo> &a_mapBoundInfo, Integer a_nxBoundNodeID)
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


void KIntersectionBasicPage::RegisterDirectionImgID()
{
	TDirectionImgID oTDirectionImgID;
	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A01;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A01;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B01;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_EW_TH, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A02;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A02;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B02;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_EW_LT, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A03;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A03;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B03;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_NS_TH, oTDirectionImgID));

	oTDirectionImgID.uiBmpResId     = IDB_SIGNAL_AR_A04;
	oTDirectionImgID.uiBmpHotResId  = IDB_SIGNAL_AR_A04;
	oTDirectionImgID.uiBmpDsblResId = IDB_SIGNAL_AR_B04;
	m_mapDirectionImgID.insert(std::make_pair(KEMDirSeq_NS_LT, oTDirectionImgID));

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


void KIntersectionBasicPage::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_TITLE, _T("Variable"), 90, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("입력 항목"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_1B,    _T("1stB"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER); 
	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_2B,    _T("2ndB"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_3B,    _T("3rdB"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_4B,    _T("4thB"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER);
}


void KIntersectionBasicPage::InitReportControl()
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

		// - 4thB 
		dValue = GetValue(KEMBoundDirectionCode4B, i);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextRight);
	}
}


double KIntersectionBasicPage::GetValue( KEMBoundDirectionCode emDirectionCode,  size_t a_nVariable )
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


void KIntersectionBasicPage::SetValue( std::map<int, TIntersectionInfo> &a_mapIntersectionInfo, KEMBoundDirectionCode emDirectionCode,  int a_nRowIndex, int a_nValue )
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


// BOOL KIntersectionBasicPage::OnApply()
// {
// 	std::map<int, TIntersectionInfo> mapIntersectionInfo;
// 
// 	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
// 	for( int i= 0; i< pRecords->GetCount(); i++ )
// 	{
// 		CXTPReportRecord*           pRecord     = pRecords->GetAt(i);
// 		CXTPReportRecordItemNumber* pItemNumber = NULL;  
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_1B);
// 		SetValue( mapIntersectionInfo, KEMBoundDirectionCode1B, i, (int)(pItemNumber->GetValue()) );
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_2B);
// 		SetValue( mapIntersectionInfo, KEMBoundDirectionCode2B, i, (int)(pItemNumber->GetValue()) );
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_3B);
// 		SetValue( mapIntersectionInfo, KEMBoundDirectionCode3B, i, (int)(pItemNumber->GetValue()) );
// 
// 		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_4B);
// 		SetValue( mapIntersectionInfo, KEMBoundDirectionCode4B, i, (int)(pItemNumber->GetValue()) );
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


BOOL KIntersectionBasicPage::SaveBasicFour()
{
	std::map<int, TIntersectionInfo> mapIntersectionInfo;

	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	for( int i= 0; i< pRecords->GetCount(); i++ )
	{
		CXTPReportRecord*           pRecord     = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pItemNumber = NULL;  

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_1B);
		SetValue( mapIntersectionInfo, KEMBoundDirectionCode1B, i, (int)(pItemNumber->GetValue()) );

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_2B);
		SetValue( mapIntersectionInfo, KEMBoundDirectionCode2B, i, (int)(pItemNumber->GetValue()) );

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_3B);
		SetValue( mapIntersectionInfo, KEMBoundDirectionCode3B, i, (int)(pItemNumber->GetValue()) );

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_4B);
		SetValue( mapIntersectionInfo, KEMBoundDirectionCode4B, i, (int)(pItemNumber->GetValue()) );
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


bool KIntersectionBasicPage::UpsertIntersectionInfo(std::map<int, TIntersectionInfo> &a_mapIntersectionInfo)
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
			prepareStatementPtr->BindInt  (4, oTIntersectionInfo.nThrowCnt);
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


bool KIntersectionBasicPage::UpsertIntersectionSignalIndication()
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


HBRUSH KIntersectionBasicPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if( nCtlColor == CTLCOLOR_EDIT )
	{
		if( pWnd->GetDlgCtrlID() == IDC_EDIT1 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT2 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT3 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT4 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT8 ||
			pWnd->GetDlgCtrlID() == IDC_EDIT12 )
		{
			pDC->SetBkColor( RGB( 255, 255, 255) );
			pDC->SetBkMode( TRANSPARENT );
			hbr = (HBRUSH)m_backBrush;
		}
		else if( pWnd->GetDlgCtrlID() == IDC_EDIT13 || 
			     pWnd->GetDlgCtrlID() == IDC_EDIT18 ||
				 pWnd->GetDlgCtrlID() == IDC_EDIT19 ||
				 pWnd->GetDlgCtrlID() == IDC_EDIT20 ||
				 pWnd->GetDlgCtrlID() == IDC_EDIT21 ||
				 pWnd->GetDlgCtrlID() == IDC_EDIT22  )
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


BOOL KIntersectionBasicPage::OnEraseBkgnd(CDC* pDC) 
{
	//return TRUE;
	return KDialogEx::OnEraseBkgnd(pDC);
}


void KIntersectionBasicPage::SetDirectionSeq(int a_nSignalLampType, bool a_bRedraw/*=true*/)
{
	UpdateData(TRUE);

	TSignalIndication &oTSignalIndication = m_vecSignalSeq[m_nRadioSignal];
	oTSignalIndication.nSignalLampType    = a_nSignalLampType;
	
	CMFCButton* pButton                   = m_vecSignalBtnCtrl[m_nRadioSignal];	// 현시 방향 이미지 
	
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


void KIntersectionBasicPage::SetDirectionSeq(int a_nSignalLampType, int a_nIndex, bool a_bRedraw/*=true*/)
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


void KIntersectionBasicPage::OnBnClickedBtnArrow01()
{
	SetDirectionSeq(KEMDirSeq_EW_TH);	// 동서 양방향 직진
}


void KIntersectionBasicPage::OnBnClickedBtnArrow02()
{
	SetDirectionSeq(KEMDirSeq_EW_LT);	// 동서 양방향 좌회전
}


void KIntersectionBasicPage::OnBnClickedBtnArrow03()
{
	SetDirectionSeq(KEMDirSeq_NS_TH);	// 남북 양방향 직진
}


void KIntersectionBasicPage::OnBnClickedBtnArrow04()
{
	SetDirectionSeq(KEMDirSeq_NS_LT);	// 남북 양방향 좌회전
}


void KIntersectionBasicPage::OnBnClickedBtnArrow05()
{
	SetDirectionSeq(KEMDirSeq_E_THLT);	// 동쪽 방향 직좌
}


void KIntersectionBasicPage::OnBnClickedBtnArrow06()
{
	SetDirectionSeq(KEMDirSeq_W_THLT);	// 서쪽 방향 직좌
}


void KIntersectionBasicPage::OnBnClickedBtnArrow07()
{
	SetDirectionSeq(KEMDirSeq_N_THLT);	// 북쪽 방향 직좌
}


void KIntersectionBasicPage::OnBnClickedBtnArrow08()
{
	SetDirectionSeq(KEMDirSeq_S_THLT);	// 남쪽 방향 직좌
}


void KIntersectionBasicPage::OnBnClickedBtnArrow09()
{
	SetDirectionSeq(KEMDirSeq_E_TH);	// 동쪽 방향 직진
}


void KIntersectionBasicPage::OnBnClickedBtnArrow10()
{
	SetDirectionSeq(KEMDirSeq_W_TH);	// 서쪽 방향 직진
}


void KIntersectionBasicPage::OnBnClickedBtnArrow11()
{
	SetDirectionSeq(KEMDirSeq_N_TH);	// 북쪽 방향 직진
}


void KIntersectionBasicPage::OnBnClickedBtnArrow12()
{
	SetDirectionSeq(KEMDirSeq_S_TH);	// 남쪽 방향 직진
}


void KIntersectionBasicPage::OnBnClickedBtnArrow13()
{
	SetDirectionSeq(KEMDirSeq_E_LT);	// 동쪽 방향 좌회전
}


void KIntersectionBasicPage::OnBnClickedBtnArrow14()
{
	SetDirectionSeq(KEMDirSeq_W_LT);	// 서쪽 방향 좌회전
}


void KIntersectionBasicPage::OnBnClickedBtnArrow15()
{
	SetDirectionSeq(KEMDirSeq_N_LT);	// 북쪽 방향 좌회전
}


void KIntersectionBasicPage::OnBnClickedBtnArrow16()
{
	SetDirectionSeq(KEMDirSeq_S_LT);	// 남쪽 방향 좌회전
}


void KIntersectionBasicPage::OnBnClickedBtnArrow17()
{
	SetDirectionSeq(KEMDirSeq_EW_NoProtectLT);	// 동서 양방향 비보호 좌회전
}


void KIntersectionBasicPage::OnBnClickedBtnArrow18()
{
	SetDirectionSeq(KEMDirSeq_NS_NoProtectLT);	// 남북 양방향 비보호 좌회전
}


void KIntersectionBasicPage::OnBnClickedBtnAllRed()
{
	SetDirectionSeq(KEMDirSeq_AllRed);	        // All Red
}


void KIntersectionBasicPage::OnBnClickedBtnSetOne()
{
	SetDirectionSeq(KEMDirSeq_EW_LT, 1, false);
	SetDirectionSeq(KEMDirSeq_NS_TH, 2, false);
	SetDirectionSeq(KEMDirSeq_NS_LT, 3, false);

	for(int i = 4; i< SEGNAL_SEQ_SIZE; i++ )
	{
		SetDirectionSeq(KEMDirSeq_NONE, i, false);
	}

	SetDirectionSeq(KEMDirSeq_EW_TH, 0, true);
}


void KIntersectionBasicPage::OnBnClickedBtnSetTwo()
{
	SetDirectionSeq(KEMDirSeq_W_THLT, 1, false);
	SetDirectionSeq(KEMDirSeq_N_THLT, 2, false);
	SetDirectionSeq(KEMDirSeq_S_THLT, 3, false);

	for(int i = 4; i< SEGNAL_SEQ_SIZE; i++ )
	{
		SetDirectionSeq(KEMDirSeq_NONE, i, false);
	}

	SetDirectionSeq(KEMDirSeq_E_THLT, 0, true);
}


void KIntersectionBasicPage::OnBnClickedRadioSignalSeqOne()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnClickedRadioSignalSeqTwo()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnClickedRadioSignalSeqThree()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnClickedRadioSignalSeqFour()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnClickedRadioSignalSeqFive()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnClickedRadioSignalSeqSix()
{
	//SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqOne()
{
	m_nRadioSignal = 0;
	UpdateData(FALSE);
}


void KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqTwo()
{
	m_nRadioSignal = 1;
	UpdateData(FALSE);
}


void KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqThree()
{
	m_nRadioSignal = 2;
	UpdateData(FALSE);
}


void KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqFour()
{
	m_nRadioSignal = 3;
	UpdateData(FALSE);
}


void KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqFive()
{
	m_nRadioSignal = 4;
	UpdateData(FALSE);
}


void KIntersectionBasicPage::OnBnClickedMfcbuttonSigseqSix()
{
	m_nRadioSignal = 5;
	UpdateData(FALSE);
}


void KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqOne()
{
	m_nRadioSignal = 0;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqTwo()
{
	m_nRadioSignal = 1;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqThree()
{
	m_nRadioSignal = 2;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqFour()
{
	m_nRadioSignal = 3;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqFive()
{
	m_nRadioSignal = 4;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}


void KIntersectionBasicPage::OnBnDoubleclickedMfcbuttonSigseqSix()
{
	m_nRadioSignal = 5;
	UpdateData(FALSE);
	SetDirectionSeq(KEMDirSeq_NONE);
}

void KIntersectionBasicPage::OnEnChangeEditLampGreen1()
{
	SetGreenLampTime(0);
}

void KIntersectionBasicPage::OnEnChangeEditLampGreen2()
{
	SetGreenLampTime(1);
}

void KIntersectionBasicPage::OnEnChangeEditLampGreen3()
{
	SetGreenLampTime(2);
}

void KIntersectionBasicPage::OnEnChangeEditLampGreen4()
{
	SetGreenLampTime(3);
}

void KIntersectionBasicPage::OnEnChangeEditLampGreen5()
{
	SetGreenLampTime(4);
}

void KIntersectionBasicPage::OnEnChangeEditLampGreen6()
{
	SetGreenLampTime(5);
}

void KIntersectionBasicPage::OnEnChangeEditLampYellow1()
{
	SetYellowLampTime(0);
}

void KIntersectionBasicPage::OnEnChangeEditLampYellow2()
{
	SetYellowLampTime(1);
}

void KIntersectionBasicPage::OnEnChangeEditLampYellow3()
{
	SetYellowLampTime(2);
}

void KIntersectionBasicPage::OnEnChangeEditLampYellow4()
{
	SetYellowLampTime(3);
}

void KIntersectionBasicPage::OnEnChangeEditLampYellow5()
{
	SetYellowLampTime(4);
}

void KIntersectionBasicPage::OnEnChangeEditLampYellow6()
{
	SetYellowLampTime(5);
}

void KIntersectionBasicPage::SetGreenLampTime(int a_nSeq)
{
	UpdateData(TRUE);
	TSignalIndication &oTSignal = m_vecSignalSeq[a_nSeq];
	oTSignal.nGreenLampDuration = GetDlgItemInt(m_vecLampGreenCtrlID[a_nSeq]);
}


void KIntersectionBasicPage::SetYellowLampTime(int a_nSeq)
{
	UpdateData(TRUE);
	TSignalIndication &oTSignal  = m_vecSignalSeq[a_nSeq];
	oTSignal.nYellowLampDuration = GetDlgItemInt(m_vecLampYellowCtrlID[a_nSeq]);
}
