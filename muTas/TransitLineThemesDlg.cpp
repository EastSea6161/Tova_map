#include "stdafx.h"
#include "TransitLineThemesDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
#include "BulkDBaseTransit.h"
#include "TTransitPassenger.h"
#include "TransitLineThemesInfo.h"

enum
{
	ASSIGN_MODEL_TYPE_MULTI = 5,
	ASSIGN_MODEL_TYPE_INTER = 6
};

enum
{
	LINEFIELD_PASSENGER       = 0,
	LINEFIELD_BOARD_NET       = 1,
	LINEFIELD_BOARD_TRANSFER  = 2,
	LINEFIELD_BOARD_TOTAL     = 3,
	LINEFIELD_ALIGHT_NET      = 4,
	LINEFIELD_ALIGHT_TRANSFER = 5,
	LINEFIELD_ALIGHT_TOTAL    = 6
};

// KTransitLineThemesDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTransitLineThemesDlg, KDialogEx)

KTransitLineThemesDlg::KTransitLineThemesDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTransitLineThemesDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_pTableLine(nullptr)
{
	try
	{
		m_clrPDefault    = RGB (  35, 126, 219 );
		m_clrPOutRange   = RGB ( 221, 217, 195 );

		m_clrSCDefault  = RGB  ( 53, 53, 53);

		m_clrBaDefault   = RGB ( 251, 181,  32 );
		m_clrBaOutRange  = RGB ( 221, 217, 195 );

		m_pTableLine = m_pTarget->Tables()->FindTable(TABLE_TRANSITNODES);	

        KBulkDBaseLink::FTLinkMap(m_pTarget, m_mapFTNodeLinkID);
	}
	catch (KExceptionPtr ex)
	{
		m_pTableLine = nullptr;
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		m_pTableLine = nullptr;
		TxLogDebugException();
	}
}

KTransitLineThemesDlg::~KTransitLineThemesDlg()
{
	ClearMapDraw(true);
	CloseWindowProc();
}

void KTransitLineThemesDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1,       m_cboMultiModal);
	DDX_Control(pDX, IDC_CLR_DEFAULT,  m_cpDefaultPColor);
	DDX_Control(pDX, IDC_CLR_OUT,      m_cpOutRangePColor);
	DDX_Control(pDX, IDC_CLR_DEFAULT2, m_cpDefaultBaColor);
	DDX_Control(pDX, IDC_CLR_OUT2,     m_cpOutRangeBaColor);
	DDX_Control(pDX, IDC_COMBO2,       m_cboBoardAlight);
	DDX_Control(pDX, IDC_CLR_SEAT,	   m_cpDefaultSCColor);
}


BEGIN_MESSAGE_MAP(KTransitLineThemesDlg, KDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KTransitLineThemesDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &KTransitLineThemesDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KTransitLineThemesDlg::OnBnClickedButtonClear)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KTransitLineThemesDlg::OnCbnSelchangeComboMultiModal)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KTransitLineThemesDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KTransitLineThemesDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_CHECK_LABEL2, &KTransitLineThemesDlg::OnBnClickedCheckLabel2)
	ON_BN_CLICKED(IDC_BUTTON7, &KTransitLineThemesDlg::OnBnClickedButtonLineSearch)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KTransitLineThemesDlg::OnBnClickedRadioType)
	ON_BN_CLICKED(IDC_BUTTON1, &KTransitLineThemesDlg::OnBnClickedButtonSCInfo)
END_MESSAGE_MAP()


// KTransitLineThemesDlg 메시지 처리기입니다.

void KTransitLineThemesDlg::ActivatedFrameWndNotifyProcess()
{
	try
	{
		if(nullptr != m_spImTasMultiIDFind)
		{
			if(!m_spImTasMultiIDFind->IsUserClosed())
			{
				if(!m_spImTasMultiIDFind->IsWindowVisible())
					m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
			}
		}

		if(nullptr != m_spSeatCapaInfo)
		{
			if(!m_spSeatCapaInfo->IsUserClosed())
			{
				if(!m_spSeatCapaInfo->IsWindowVisible())
					m_spSeatCapaInfo->ShowWindow(SW_SHOW);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTransitLineThemesDlg::DeActivatedFrameWndNotifyProcess()
{
	try
	{
		if(nullptr != m_spImTasMultiIDFind)
		{
			if(!m_spImTasMultiIDFind->IsUserClosed())
			{
				if(m_spImTasMultiIDFind->IsWindowVisible())
					m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
			}
		}

		if (nullptr != m_spSeatCapaInfo)
		{
			if (!m_spSeatCapaInfo->IsUserClosed())
			{
				if (m_spSeatCapaInfo->IsWindowVisible())
					m_spSeatCapaInfo->ShowWindow(SW_HIDE);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTransitLineThemesDlg::SetToolTip(std::vector<Integer> &a_vecTransitID, std::map<Integer, CString> &a_mapLineData)
{
	CString strIdName (_T(""));
	CString strLineName(_T(""));
	CString strToolTip(_T(""));

	size_t nxCount = a_vecTransitID.size();
	for (size_t i= 0; i< nxCount; i++)
	{
		Integer &nxTransitID = a_vecTransitID[i];
		strLineName          = _T("");

		AutoType iter = m_mapLineData.find(nxTransitID);
		if (iter != m_mapLineData.end())
		{
			strLineName = iter->second;
		}
		if (strLineName.Compare(_T("")) == 0)
		{
			strLineName = _T("-");
		}

		strIdName.Format(_T("%I64d[%s]\r\n"), nxTransitID, strLineName);

		strToolTip.Append(strIdName);
	}

	AddImChampTooltip(IDC_EDIT_LINEID,       strToolTip);
}


void KTransitLineThemesDlg::SetViewTransit(Integer a_nxTransitID )
{
	try
	{
		if (a_nxTransitID > 0) {
			if (m_setTransitID.find(a_nxTransitID) == m_setTransitID.end())
			{
				m_vecTransitID.push_back(a_nxTransitID);
				m_setTransitID.insert(a_nxTransitID);
			}
			else
			{
				return;
			}
		}
	
		CString strCaption;
		strCaption.Format(_T("%s"), m_pTarget->Caption());
		SetDlgItemText(IDC_STATIC_INFO, strCaption);

		KBulkDBaseTransit::TransitAllIdCaption(m_pTarget, m_vecLineIDCaption, m_mapLineData);

		SetToolTip(m_vecTransitID, m_mapLineData);

		m_mapPassengerMinMax.clear();
		SetDlgItemText(IDC_EDIT_MIN_VALUE,   _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE,   _T(""));
		SetDlgItemText(IDC_EDIT_MIN_VALUE2,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE2,  _T(""));

		CString strIDs(_T(""));
		for (size_t i= 0; i< m_vecTransitID.size(); i++)
		{
			if (i > 0)
			{
				strIDs.Append(_T(","));
			}
			strIDs.AppendFormat(_T("%I64d"), m_vecTransitID[i]);
		}

		SetDlgItemText(IDC_EDIT_LINEID, strIDs);

		LoadAssignModelInfo(m_vecMultiModalInfo);
		InitComboAssignModelSelect(m_vecMultiModalInfo);
		InitComboBoardAlight();
		InitPassengerField();

		ClearMapDraw(true);
	}
	catch (...)
	{
		TxLogDebugException();
	}	
}


void KTransitLineThemesDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampMultiIDFind::IDD == nDialogID)
	{
		if (m_spImTasMultiIDFind)
		{
			try
			{
				CString           strIDs = m_spImTasMultiIDFind->GetSelectedID();
				SetDlgItemText(IDC_EDIT_LINEID, strIDs);
				std::set<CString> setID  = m_spImTasMultiIDFind->GetSelectedIDSet();

				m_vecTransitID.clear();
				m_setTransitID.clear();

				AutoType iter  = setID.begin();
				AutoType itEnd = setID.end();
				while (iter != itEnd)
				{
					Integer nxLineID = _ttoi64(*iter);

					m_vecTransitID.push_back(nxLineID);
					m_setTransitID.insert(nxLineID);
					++iter;
				}

				SetToolTip(m_vecTransitID, m_mapLineData);

				m_mapPassengerMinMax.clear();
				SetDlgItemText(IDC_EDIT_MIN_VALUE,   _T(""));
				SetDlgItemText(IDC_EDIT_MAX_VALUE,   _T(""));
				SetDlgItemText(IDC_EDIT_MIN_VALUE2,  _T(""));
				SetDlgItemText(IDC_EDIT_MAX_VALUE2,  _T(""));

				UpdateData(TRUE);
				InitComboBoardAlight();
				UpdateTotalMinMaxData();
				InitPassengerField();

				ClearMapDraw(true);
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


BOOL KTransitLineThemesDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	SetDlgItemText(IDC_EDIT_MINSIZE,   _T("5.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE,   _T("20.0"));
	SetDlgItemText(IDC_EDIT_MINSIZE2,  _T("5.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE2,  _T("20.0"));

	CheckDlgButton(IDC_CHECK_BOARDALIGHT, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_LABEL2, BST_UNCHECKED);

	CheckDlgButton(IDC_CHECK_SEAT, BST_UNCHECKED);
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	GetDlgItem(IDC_EDIT_CAPA)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CAPA)->SetWindowTextW(_T("100"));

	KColorPickerSetting::DefaultCtrl(m_cpDefaultPColor,     m_clrPDefault);
	KColorPickerSetting::DefaultCtrl(m_cpOutRangePColor,    m_clrPOutRange);
	KColorPickerSetting::DefaultCtrl(m_cpDefaultBaColor,    m_clrBaDefault);
	KColorPickerSetting::DefaultCtrl(m_cpOutRangeBaColor,   m_clrBaOutRange);
	KColorPickerSetting::DefaultCtrl(m_cpDefaultSCColor,   m_clrSCDefault);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTransitLineThemesDlg::LoadAssignModelInfo(std::vector<TMultiModalInfo> &a_vecMultiModalInfo)
{
	a_vecMultiModalInfo.clear();

    KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strQuery(_T(""));
	strQuery.Format(_T(" SELECT Description, ")
		_T(" line_board_net, line_board_Transfer, line_board_Total, ")
		_T(" line_alight_net, line_alight_Transfer, line_alight_Total, ")
		_T(" line_passenger ")
		_T(" FROM %s WHERE type in (%d, %d) ORDER BY run_seq "),TABLE_ASSIGN_MODEL, ASSIGN_MODEL_TYPE_MULTI, ASSIGN_MODEL_TYPE_INTER);
	try
	{        
		KResultSetPtr spResultSet  = spDBaseConnection->ExecuteQuery(strQuery);
		while ( spResultSet->Next() )
		{
			TMultiModalInfo oTMultiModalInfo;
			oTMultiModalInfo.strDescript             = spResultSet->GetValueString(0);

			oTMultiModalInfo.strFieldBoardNet        = spResultSet->GetValueString(1);
			oTMultiModalInfo.strFieldBoardTransfer   = spResultSet->GetValueString(2);
			oTMultiModalInfo.strFieldBoardTot        = spResultSet->GetValueString(3);
			oTMultiModalInfo.strFieldAlightNet       = spResultSet->GetValueString(4);
			oTMultiModalInfo.strFieldAlightTransfer  = spResultSet->GetValueString(5);
			oTMultiModalInfo.strFieldAlightTot       = spResultSet->GetValueString(6);
			
			oTMultiModalInfo.strFieldPassenger       = spResultSet->GetValueString(7);

			a_vecMultiModalInfo.push_back(oTMultiModalInfo);
		}
	}
	catch (...)
	{
		AfxMessageBox(_T("ModelData 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 
}


void KTransitLineThemesDlg::InitComboAssignModelSelect(std::vector<TMultiModalInfo> &a_vecMultiModalInfo)
{
	try
	{
		m_cboMultiModal.ResetContent();

		for (size_t i= 0; i< a_vecMultiModalInfo.size(); i++)
		{
			TMultiModalInfo &oTMultiModalInfo = a_vecMultiModalInfo[i];

			int nCur = m_cboMultiModal.AddString(oTMultiModalInfo.strDescript);
			m_cboMultiModal.SetItemData(nCur, (DWORD_PTR)&oTMultiModalInfo);
		}

		if(a_vecMultiModalInfo.size() > 0)
		{
			m_cboMultiModal.SetCurSel(0);
			UpdateTotalMinMaxData();
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KTransitLineThemesDlg::InitComboBoardAlight()
{
	try
	{
		m_cboBoardAlight.ResetContent();
		m_mapPassengerField.clear();
		int nCurSel = m_cboMultiModal.GetCurSel();
		if(nCurSel < 0)
			return;

		TMultiModalInfo *pTMultiModalInfo = (TMultiModalInfo*)m_cboMultiModal.GetItemData(nCurSel);

		CString &strFieldPassenger        = pTMultiModalInfo->strFieldPassenger;
		CString &strFieldBoardNet         = pTMultiModalInfo->strFieldBoardNet;
		CString &strFieldBoardTransfer    = pTMultiModalInfo->strFieldBoardTransfer;
		CString &strFieldBoardTot         = pTMultiModalInfo->strFieldBoardTot;
		CString &strFieldAlightNet        = pTMultiModalInfo->strFieldAlightNet;
		CString &strFieldAlightTransfer   = pTMultiModalInfo->strFieldAlightTransfer;
		CString &strFieldAlightTot        = pTMultiModalInfo->strFieldAlightTot;

		if (m_pTableLine == nullptr)
			throw 1;

		const KIOColumns* pColumnCollection = m_pTableLine->Columns();
		int nColumnCount = pColumnCollection->ColumnCount();
		int nCur(-1);
		for (int i=0; i<nColumnCount; i++)
		{
			KIOColumn* pIOColumn    = pColumnCollection->GetColumn(i);
			CString strColumnName   = pIOColumn->Name();
			
			TTransitPassengerMinMax oTTransitPassengerMinMax;
			oTTransitPassengerMinMax.strFieldName = strColumnName;
			
			if (strColumnName.CompareNoCase(strFieldBoardNet)       == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_BOARD_NET,       pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldBoardTransfer)  == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_BOARD_TRANSFER,  pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldBoardTot)       == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_BOARD_TOTAL,     pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldAlightNet)      == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_ALIGHT_NET,      pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldAlightTransfer) == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_ALIGHT_TRANSFER, pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldAlightTot)      == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_ALIGHT_TOTAL,    pIOColumn));
			}
		}

		AutoType iter  = m_mapPassengerField.begin();
		AutoType itEnd = m_mapPassengerField.end();

		while(iter != itEnd)
		{
			int        nKeyIndex = iter->first;
			KIOColumn* pIOColumn = iter->second;

			InsertComboBoardAlightData(pIOColumn->Caption(), nKeyIndex);

			++iter;
		}

		m_cboBoardAlight.SetCurSel(0);
		UpdateBoardAlightMinMaxData();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KTransitLineThemesDlg::InsertComboBoardAlightData(CString a_strAddString, int a_nKeyIndex)
{
	int nCur(-1); 
	nCur = m_cboBoardAlight.AddString(a_strAddString);
	m_cboBoardAlight.SetItemData(nCur, (DWORD_PTR)a_nKeyIndex);
}


void KTransitLineThemesDlg::InitPassengerField()
{
	try
	{
		int nCurSel = m_cboMultiModal.GetCurSel();
		if(nCurSel < 0)
			return;

		TMultiModalInfo* pTMultiModalInfo  = (TMultiModalInfo*)m_cboMultiModal.GetItemData(nCurSel);
		CString          strFieldPassenger = pTMultiModalInfo->strFieldPassenger;
		CString          strFieldCaption(_T(""));

		if (nullptr == m_pTableLine)
			throw 1;

		const KIOColumns* pColumnCollection = m_pTableLine->Columns();
		int nColumnCount         = pColumnCollection->ColumnCount();
		for (int i=0; i<nColumnCount; i++)
		{
			KIOColumn* pIOColumn  = pColumnCollection->GetColumn(i);
			CString strColumnName = pIOColumn->Name();

			if (strColumnName.CompareNoCase(strFieldPassenger) == 0)
			{
				strFieldCaption = pIOColumn->Caption();
			}
		}

		SetDlgItemText(IDC_EDIT_PASSENGERFIELD, strFieldCaption);
		if (strFieldCaption.Compare(_T("")) == 0 || strFieldPassenger.Compare(_T("")) == 0)
		{
			SetDlgItemText(IDC_EDIT_PASSENGERFIELD, _T(""));
			SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T(""));
			SetDlgItemText(IDC_EDIT_MAX_VALUE,  _T(""));
			CheckDlgButton(IDC_CHECK_PASSENGER, BST_UNCHECKED);
			GetDlgItem(IDC_CHECK_PASSENGER)->EnableWindow(FALSE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_PASSENGER, BST_CHECKED);
			GetDlgItem(IDC_CHECK_PASSENGER)->EnableWindow(TRUE);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


bool KTransitLineThemesDlg::GetSelectedMinMaxValue( std::map<int, TTransitPassengerMinMax> &a_mapPassengerMinMax )
{   
	a_mapPassengerMinMax.clear();

	int nCur = m_cboMultiModal.GetCurSel();
	if(nCur < 0)
		return false;

	TMultiModalInfo *pTMultiModalInfo = (TMultiModalInfo*)m_cboMultiModal.GetItemData(nCur);

	CString &strFieldPassenger        = pTMultiModalInfo->strFieldPassenger;
	CString &strFieldBoardNet         = pTMultiModalInfo->strFieldBoardNet;
	CString &strFieldBoardTransfer    = pTMultiModalInfo->strFieldBoardTransfer;
	CString &strFieldBoardTot         = pTMultiModalInfo->strFieldBoardTot;
	CString &strFieldAlightNet        = pTMultiModalInfo->strFieldAlightNet;
	CString &strFieldAlightTransfer   = pTMultiModalInfo->strFieldAlightTransfer;
	CString &strFieldAlightTot        = pTMultiModalInfo->strFieldAlightTot;

	if (m_pTableLine == nullptr)
		throw 1;

	const KIOColumns* pColumnCollection = m_pTableLine->Columns();
	int nColumnCount  = pColumnCollection->ColumnCount();
	
    int nFlag(2);
	std::map<int, int> mapFlag;
    for (int i=0; i<nColumnCount; i++) {
		KIOColumn* pIOColumn    = pColumnCollection->GetColumn(i);
		CString strColumnName   = pIOColumn->Name();

		TTransitPassengerMinMax oTTransitPassengerMinMax;
		oTTransitPassengerMinMax.strFieldName = strColumnName;

		if (strColumnName.CompareNoCase(strFieldPassenger ) == 0)
		{
			oTTransitPassengerMinMax.bPassenger = true;
            a_mapPassengerMinMax.insert(std::make_pair(LINEFIELD_PASSENGER,      oTTransitPassengerMinMax));      
            //nFlag = 2;
			mapFlag.insert({ LINEFIELD_PASSENGER, 2 });
		}
		else if (strColumnName.CompareNoCase(strFieldBoardNet)       == 0)
		{
			a_mapPassengerMinMax.insert(std::make_pair(LINEFIELD_BOARD_NET,      oTTransitPassengerMinMax));
            //nFlag = 0;
			mapFlag.insert({ LINEFIELD_BOARD_NET, 0 });
		}
		else if (strColumnName.CompareNoCase(strFieldBoardTransfer)  == 0)
		{
			a_mapPassengerMinMax.insert(std::make_pair(LINEFIELD_BOARD_TRANSFER, oTTransitPassengerMinMax));
            //nFlag = 0;
			mapFlag.insert({ LINEFIELD_BOARD_TRANSFER, 0 });
		}
		else if (strColumnName.CompareNoCase(strFieldBoardTot)       == 0)
		{
			a_mapPassengerMinMax.insert(std::make_pair(LINEFIELD_BOARD_TOTAL,    oTTransitPassengerMinMax));
            //nFlag = 0;
			mapFlag.insert({ LINEFIELD_BOARD_TOTAL, 0 });
		}
		else if (strColumnName.CompareNoCase(strFieldAlightNet)      == 0)
		{
			a_mapPassengerMinMax.insert(std::make_pair(LINEFIELD_ALIGHT_NET,     oTTransitPassengerMinMax));
            //nFlag = 1;
			mapFlag.insert({ LINEFIELD_ALIGHT_NET, 1 });
		}
		else if (strColumnName.CompareNoCase(strFieldAlightTransfer) == 0)
		{
			a_mapPassengerMinMax.insert(std::make_pair(LINEFIELD_ALIGHT_TRANSFER,oTTransitPassengerMinMax));
            //nFlag = 1;
			mapFlag.insert({ LINEFIELD_ALIGHT_TRANSFER, 1 });
		}
		else if (strColumnName.CompareNoCase(strFieldAlightTot)      == 0)
		{
			a_mapPassengerMinMax.insert(std::make_pair(LINEFIELD_ALIGHT_TOTAL,   oTTransitPassengerMinMax));
            //nFlag = 1;
			mapFlag.insert({ LINEFIELD_ALIGHT_TOTAL, 1 });
		}
	}

	try
	{
		AutoType iter  = a_mapPassengerMinMax.begin();
		AutoType itEnd = a_mapPassengerMinMax.end();

		while(iter != itEnd)
		{
			TTransitPassengerMinMax& oTTransitPassengerMinMax = iter->second;
			if (oTTransitPassengerMinMax.bPassenger) {
                KBulkDBaseTransit::TransitLinksPerson( m_pTarget, m_mapFTNodeLinkID, oTTransitPassengerMinMax.strFieldName, m_vecTransitID, oTTransitPassengerMinMax.dMinVol, oTTransitPassengerMinMax.dMaxVol);
            } 
            else {
				
                KBulkDBaseTransit::TransitBoardAlightMinMax(m_pTarget, m_mapFTNodeLinkID, oTTransitPassengerMinMax.strFieldName, m_vecTransitID, mapFlag[iter->first], oTTransitPassengerMinMax.dMinVol, oTTransitPassengerMinMax.dMaxVol);
            }
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}    

	return true;
}


void KTransitLineThemesDlg::OnBnClickedCancel()
{
	try
	{
		ClearMapDraw(true);
		CloseWindowProc();
		KDialogEx::OnCancel();
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


void KTransitLineThemesDlg::CloseWindowProc()
{
	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();

	if (nullptr != m_spSeatCapaInfo)
		m_spSeatCapaInfo.reset();
}


void KTransitLineThemesDlg::OnBnClickedButtonDraw()
{
	try
	{
		int nCur = m_cboMultiModal.GetCurSel();
		if(nCur < 0)
			return;

		if (UserInputCheck() == false) {
			return;
		}

        ClearMapDraw(false);

		TMultiModalInfo *pTMultiModalInfo = (TMultiModalInfo*)m_cboMultiModal.GetItemData(nCur);

		TTransitLineThemesInfo oTTransitLineThemesInfo; {
            oTTransitLineThemesInfo.vecTransitID  = m_vecTransitID;
            oTTransitLineThemesInfo.strPassenger = pTMultiModalInfo->strFieldPassenger;

            CString strValue(_T(""));
            GetDlgItemText(IDC_EDIT_MIN_VALUE, strValue);
            oTTransitLineThemesInfo.dMinVolumeP    = _ttof(strValue);
            GetDlgItemText(IDC_EDIT_MAX_VALUE, strValue);
            oTTransitLineThemesInfo.dMaxVolumeP    = _ttof(strValue);
            GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
            oTTransitLineThemesInfo.dMinSizeP      = _ttof(strValue);
            GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
            oTTransitLineThemesInfo.dMaxSizeP      = _ttof(strValue);
            oTTransitLineThemesInfo.clrLineP       = m_cpDefaultPColor.GetColor();
            oTTransitLineThemesInfo.clrOutRangeP   = m_cpOutRangePColor.GetColor();
            if( IsDlgButtonChecked(IDC_CHECK_OUTRANGE) == BST_CHECKED)
                oTTransitLineThemesInfo.bOutRangeP     =  true;
            else
                oTTransitLineThemesInfo.bOutRangeP     =  false;

            GetDlgItemText(IDC_EDIT_MIN_VALUE2, strValue);
            oTTransitLineThemesInfo.dMinVolumeBA  = _ttof(strValue);
            GetDlgItemText(IDC_EDIT_MAX_VALUE2, strValue);
            oTTransitLineThemesInfo.dMaxVolumeBA  = _ttof(strValue);
            GetDlgItemText(IDC_EDIT_MINSIZE2, strValue);
            oTTransitLineThemesInfo.dMinSizeBA    = _ttof(strValue);
            GetDlgItemText(IDC_EDIT_MAXSIZE2, strValue);
            oTTransitLineThemesInfo.dMaxSizeBA    = _ttof(strValue);
            oTTransitLineThemesInfo.clrLineBA     = m_cpDefaultBaColor.GetColor();
            oTTransitLineThemesInfo.clrOutRangeBA = m_cpOutRangeBaColor.GetColor();
            if( IsDlgButtonChecked(IDC_CHECK_OUTRANGE2) == BST_CHECKED)
                oTTransitLineThemesInfo.bOutRangeBA     =  true;
            else
                oTTransitLineThemesInfo.bOutRangeBA     =  false;
            
            oTTransitLineThemesInfo.clrSeatCapa  = m_cpDefaultSCColor.GetColor();
            oTTransitLineThemesInfo.TPassengerAlpha = 255;
            oTTransitLineThemesInfo.TSeatCapaAlpha  = 180;
        }
        
		std::vector<TDrawLinkInfo>         vecDrawLinkInfo;
		std::vector<TTransitPassengerInfo> vecBoardInfo;
		std::vector<TTransitPassengerInfo> vecAlightInfo;

		bool bShowLinkLabel(false);
		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED) {
			bShowLinkLabel = true;
		}
		else {
			bShowLinkLabel = false;
		}

		bool bShowPassengerLabel(false);
		if(IsDlgButtonChecked(IDC_CHECK_LABEL2) == BST_CHECKED) {
			bShowPassengerLabel = true;
		}
		else
		{
			bShowPassengerLabel = false;
		}

		if (IsDlgButtonChecked(IDC_CHECK_PASSENGER) == BST_CHECKED || IsDlgButtonChecked(IDC_CHECK_SEAT) == BST_CHECKED)
		{
			if (IsDlgButtonChecked(IDC_CHECK_PASSENGER) == BST_CHECKED)
			{
				KTransitLineThemesInfo::GetPassengerInfo(m_pTarget, m_mapFTNodeLinkID, oTTransitLineThemesInfo, vecDrawLinkInfo);
			}

			if (IsDlgButtonChecked(IDC_CHECK_SEAT) == BST_CHECKED)
			{
				int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2);
				if (nCheck == IDC_RADIO1) {
					KTransitLineThemesInfo::GetSeatInfo(m_pTarget, oTTransitLineThemesInfo, vecDrawLinkInfo);
				}
				else {
					CString strCaPa(_T("")); {
                        GetDlgItemText(IDC_EDIT_CAPA, strCaPa);
                    }
					
					oTTransitLineThemesInfo.dCapaRatio = _ttof(strCaPa);
					KTransitLineThemesInfo::GetCapacityInfo(m_pTarget, oTTransitLineThemesInfo, vecDrawLinkInfo);
				}
			}

            //★ 정렬 시키지 않도록 수정(2016-08-02)
			//std::sort(vecDrawLinkInfo.begin(), vecDrawLinkInfo.end(), PathWidthDescending);
			m_pMapView->TxDrawCompositeLink(vecDrawLinkInfo, bShowLinkLabel, false, false, true);
		}

		if (IsDlgButtonChecked(IDC_CHECK_BOARDALIGHT) == BST_CHECKED)
		{
			int nCurSel = m_cboBoardAlight.GetCurSel();
			if (nCurSel < 0) {
				return;
			}

			int     nKeyIndex = m_cboBoardAlight.GetItemData(nCurSel);
			CString strFieldName(_T(""));
			AutoType iter  = m_mapPassengerField.find(nKeyIndex);
			AutoType itEnd = m_mapPassengerField.end();

			if (iter != itEnd)
			{
				KIOColumn* pIOColumn = iter->second;
				strFieldName = pIOColumn->Name();
			}

			// 승차 필드's
			if(LINEFIELD_BOARD_NET == nKeyIndex || LINEFIELD_BOARD_TRANSFER == nKeyIndex || LINEFIELD_BOARD_TOTAL == nKeyIndex)
			{
				oTTransitLineThemesInfo.strBoard     = strFieldName;
	
				KTransitLineThemesInfo::GetBoardingInfo(m_pTarget, m_mapFTNodeLinkID, oTTransitLineThemesInfo, vecBoardInfo);
				m_pMapView->TxDrawTransitBoardAlight(vecBoardInfo, bShowPassengerLabel, false);
			}
			else // 하차 필드's
			{
				oTTransitLineThemesInfo.strAlight = strFieldName;

				KTransitLineThemesInfo::GetAlightingInfo(m_pTarget, m_mapFTNodeLinkID, oTTransitLineThemesInfo, vecAlightInfo);
				m_pMapView->TxDrawTransitBoardAlight(vecAlightInfo, bShowPassengerLabel, false);
			}
		}

		if (m_vecTransitID.size() > 0) {
			// Union 하여, 심볼들 전체의 Extent를 만들고
			// 화면의 Extent와 교차 여부 파악하여, 교차되지 않을 때만 Extent

			TxEnvelope oCurrentEnv;
			m_pMapView->GetCurrentExtent(oCurrentEnv);

			TxEnvelope oUnionEnv;
			int nIndex(0);
			for (size_t i= 0; i< m_vecTransitID.size(); i++) {
				Integer nxTransitID = m_vecTransitID[i];
				
				try
				{
					TxEnvelope oEnv;
					if (KBulkDBaseTransit::TransitExtent( m_pTarget, nxTransitID, oEnv) <= 0) {
						continue;
					} 

					if (0 == nIndex) {
						oUnionEnv = oEnv;
					} else {
						oUnionEnv.Union(oEnv);
					}
					nIndex++;
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

			if (false == oCurrentEnv.Intersect(oUnionEnv)) {
				double dDistance = QBicGeometry::Length(oUnionEnv.GetMinX(), oUnionEnv.GetMinY(), oUnionEnv.GetMaxX(), oUnionEnv.GetMaxY()); 
				if (dDistance < 1000) {
					dDistance = 1000;
				} else {
					dDistance *= 1.3;
				}
				oUnionEnv.Offset(dDistance);

				m_pMapView->ZoomMap(oUnionEnv, true);
			} else {
				m_pMapView->MapRefresh();
			}
		} else {
			m_pMapView->MapRefresh();
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KTransitLineThemesDlg::OnBnClickedButtonClear()
{
	ClearMapDraw(true);
}


void KTransitLineThemesDlg::ClearMapDraw(bool bRefresh)
{
	try
	{
		m_pMapView->TxClearCompositeThemes(bRefresh);
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


void KTransitLineThemesDlg::OnCbnSelchangeComboMultiModal()
{
	UpdateData(TRUE);

	InitComboBoardAlight();
	
	UpdateTotalMinMaxData();

	InitPassengerField();

	ClearMapDraw(true);
}


void KTransitLineThemesDlg::UpdateTotalMinMaxData()
{
	int nCur = m_cboMultiModal.GetCurSel();
	if(nCur < 0)
		return;

	if (m_vecTransitID.size() < 1)
		return;

	GetSelectedMinMaxValue(m_mapPassengerMinMax);

	CString strMinVal(_T(""));
	CString strMaxVal(_T(""));
	double  dMinVal(0.0);
	double  dMaxVal(0.0);

	AutoType iter  = m_mapPassengerMinMax.find(LINEFIELD_PASSENGER);
	AutoType itEnd = m_mapPassengerMinMax.end();

	if (iter != itEnd)
	{
		TTransitPassengerMinMax &oTTransitPassengerMinMax = iter->second;
		dMinVal = oTTransitPassengerMinMax.dMinVol;
		dMaxVal = oTTransitPassengerMinMax.dMaxVol;

		dMaxVal += 0.6;
		strMaxVal.Format(_T("%.0f"), dMaxVal);

		dMinVal -= 0.5;
		if(dMinVal < 0.0)
			dMinVal = 0.0;
		strMinVal.Format(_T("%.0f"), dMinVal);

		SetDlgItemText(IDC_EDIT_MIN_VALUE,  strMinVal);
		SetDlgItemText(IDC_EDIT_MAX_VALUE,  strMaxVal);
	}

	nCur = m_cboBoardAlight.GetCurSel();
	if(nCur < 0)
	{
		SetDlgItemText(IDC_EDIT_MIN_VALUE2,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE2,  _T(""));
		return;
	}
	int nKeyIndex = m_cboBoardAlight.GetItemData(nCur);

	AutoType iterB  = m_mapPassengerMinMax.find(nKeyIndex);
	AutoType itEndB = m_mapPassengerMinMax.end();

	if(iterB != itEndB) // 승차 Min Max
	{
		TTransitPassengerMinMax &oTTransitPassengerMinMax = iterB->second;
		dMinVal = oTTransitPassengerMinMax.dMinVol;
		dMaxVal = oTTransitPassengerMinMax.dMaxVol;

		dMaxVal += 0.6;
		strMaxVal.Format(_T("%.0f"), dMaxVal);

		dMinVal -= 0.5;
		if(dMinVal < 0.0)
			dMinVal = 0.0;
		strMinVal.Format(_T("%.0f"), dMinVal);

		SetDlgItemText(IDC_EDIT_MIN_VALUE2,  strMinVal);
		SetDlgItemText(IDC_EDIT_MAX_VALUE2,  strMaxVal);
	}
}


void KTransitLineThemesDlg::OnCbnSelchangeCombo2()
{
	UpdateBoardAlightMinMaxData();
}


void KTransitLineThemesDlg::UpdateBoardAlightMinMaxData()
{
	int nCur = m_cboBoardAlight.GetCurSel();
	if(nCur < 0)
	{
		SetDlgItemText(IDC_EDIT_MIN_VALUE2,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE2,  _T(""));
		return;
	}

	CString strMinVal(_T(""));
	CString strMaxVal(_T(""));
	double  dMinVal(0.0);
	double  dMaxVal(0.0);
	
	int nKeyIndex = m_cboBoardAlight.GetItemData(nCur);

	AutoType iterB  = m_mapPassengerMinMax.find(nKeyIndex);
	AutoType itEndB = m_mapPassengerMinMax.end();

	if(iterB != itEndB) // 승차 Min Max
	{
		TTransitPassengerMinMax &oTTransitPassengerMinMax = iterB->second;
		dMinVal = oTTransitPassengerMinMax.dMinVol;
		dMaxVal = oTTransitPassengerMinMax.dMaxVol;

		dMaxVal += 0.6;
		strMaxVal.Format(_T("%.0f"), dMaxVal);

		dMinVal -= 0.5;
		if(dMinVal < 0.0)
			dMinVal = 0.0;
		strMinVal.Format(_T("%.0f"), dMinVal);

		SetDlgItemText(IDC_EDIT_MIN_VALUE2,  strMinVal);
		SetDlgItemText(IDC_EDIT_MAX_VALUE2,  strMaxVal);
	}
}


bool KTransitLineThemesDlg::UserInputCheck()
{
	try
	{
		if (m_vecTransitID.size() < 1)
		{
			AfxMessageBox(_T("노선 ID를 선택해 주세요."));
			GetDlgItem(IDC_EDIT_LINEID)->SetFocus();
			return false;
		}
	
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize (0.0), dMaxSize (0.0);

		CString strValue(_T(""));

		if (IsDlgButtonChecked(IDC_CHECK_PASSENGER) == BST_CHECKED)
		{
			if(m_cboMultiModal.GetCurSel() < 0)
			{
				AfxMessageBox(_T("대중교통 통행배정 결과를 선택해 주세요."));
				GetDlgItem(IDC_COMBO1)->SetFocus();		
				return false;
			}

			//min value 체크
			GetDlgItemText(IDC_EDIT_MIN_VALUE, strValue);
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MIN_VALUE)->SetFocus();		
				return false;
			}
			dMinValue = _ttof(strValue);

			//IDC_EDIT_MAX_VALUE
			GetDlgItemText(IDC_EDIT_MAX_VALUE, strValue);
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MAX_VALUE)->SetFocus();		
				return false;
			}
			dMaxValue = _ttof(strValue);

			//IDC_EDIT_MINSIZE
			GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
				return false;
			}
			double dMinSize = _ttof(strValue);

			//IDC_EDIT_MAXSIZE
			GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MAXSIZE)->SetFocus();		
				return false;
			}
			double dMaxSize = _ttof(strValue);

			if (dMinValue > dMaxValue)
			{
				AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MIN_VALUE)->SetFocus();		
				return false;
			}

			if (dMinSize > dMaxSize)
			{
				AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
				return false;
			}        
		}
		CString strFieldCaption(_T(""));
		GetDlgItemText(IDC_EDIT_PASSENGERFIELD, strFieldCaption);
		if (strFieldCaption.Compare(_T("")) == 0)
		{
			AfxMessageBox(_T("재차인원 컬럼이 존재하지 않습니다."));
			return false;
		}

		if (IsDlgButtonChecked(IDC_CHECK_BOARDALIGHT) == BST_CHECKED)
		{
			if(m_cboBoardAlight.GetCurSel() < 0)
			{
				AfxMessageBox(_T("승/하차 컬럼을 선택해 주세요."));
				GetDlgItem(IDC_COMBO2)->SetFocus();		
				return false;
			}

			//min value 체크
			GetDlgItemText(IDC_EDIT_MIN_VALUE2, strValue);
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MIN_VALUE2)->SetFocus();		
				return false;
			}
			dMinValue = _ttof(strValue);

			//IDC_EDIT_MAX_VALUE
			GetDlgItemText(IDC_EDIT_MAX_VALUE2, strValue);
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MAX_VALUE2)->SetFocus();		
				return false;
			}
			dMaxValue = _ttof(strValue);

			//IDC_EDIT_MINSIZE
			GetDlgItemText(IDC_EDIT_MINSIZE2, strValue);
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MINSIZE2)->SetFocus();		
				return false;
			}
			dMinSize = _ttof(strValue);

			//IDC_EDIT_MAXSIZE
			GetDlgItemText(IDC_EDIT_MAXSIZE2, strValue);
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MAXSIZE2)->SetFocus();		
				return false;
			}
			dMaxSize = _ttof(strValue);

			if (dMinValue > dMaxValue)
			{
				AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MIN_VALUE2)->SetFocus();		
				return false;
			}

			if (dMinSize > dMaxSize)
			{
				AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MINSIZE2)->SetFocus();		
				return false;
			}        
		}

		if (IsDlgButtonChecked(IDC_CHECK_SEAT) == BST_CHECKED)
		{
			int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2);
			if (nCheck == IDC_RADIO2)
			{
				GetDlgItemText(IDC_EDIT_CAPA, strValue);
				if (QBicStringChecker::IsNumeric(strValue) == false)
				{
					AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
					GetDlgItem(IDC_EDIT_CAPA)->SetFocus();		
					return false;
				}
// 				double dRatio = _ttof(strValue);
// 				if (dRatio > 100)
// 				{
// 					AfxMessageBox(_T("100보다 작은 숫자를 입력해 주세요."));
// 					GetDlgItem(IDC_EDIT_CAPA)->SetFocus();		
// 					return false;
// 				}
			}
		}

		if (IsDlgButtonChecked(IDC_CHECK_PASSENGER) == BST_UNCHECKED && IsDlgButtonChecked(IDC_CHECK_BOARDALIGHT) == BST_UNCHECKED)
		{
			return false;
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
	return true;
}


void KTransitLineThemesDlg::OnBnClickedCheckLabel()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			m_pMapView->TxDrawShowCompositeLinkLablel(true);
		}
		else
		{
			m_pMapView->TxDrawShowCompositeLinkLablel(false);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KTransitLineThemesDlg::OnBnClickedCheckLabel2()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_LABEL2) == BST_CHECKED)
		{
			m_pMapView->TxDrawShowTransitPassengerLabel(true);
		}
		else
		{
			m_pMapView->TxDrawShowTransitPassengerLabel(false);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KTransitLineThemesDlg::OnBnClickedButtonLineSearch()
{
	SelectLineList();
}


BOOL KTransitLineThemesDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT_LINEID)->m_hWnd)
		{
			ClickedEditLine();
			return TRUE;
		}
	}

	return KDialogEx::PreTranslateMessage(pMsg);
}


void KTransitLineThemesDlg::ClickedEditLine()
{
	SelectLineList();
}


void KTransitLineThemesDlg::SelectLineList()
{
	try
	{
		CWaitCursor cw;

		CString strSelect(_T("")); 
		GetDlgItemText(IDC_EDIT_LINEID, strSelect);

		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		m_spImTasMultiIDFind->SetIDCaption(m_vecLineIDCaption);

		m_spImTasMultiIDFind->SetSelectedID(strSelect);
		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}

void KTransitLineThemesDlg::ModelessCancleFeedback(UINT nDialogID)
{

}

void KTransitLineThemesDlg::OnBnClickedRadioType( UINT nID )
{
	int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2);
	if (nCheck == IDC_RADIO1)
	{
		GetDlgItem(IDC_EDIT_CAPA)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CAPA)->EnableWindow(TRUE);
	}
}



void KTransitLineThemesDlg::OnBnClickedButtonSCInfo()
{
	if (m_vecTransitID.size() < 1)
	{
		AfxMessageBox(_T("선택된 노선이 없습니다."));
		return;
	}

	int nType(0);
	double dRatio(0);

	int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2);
	if (nCheck == IDC_RADIO1)
	{
		nType = 0;
	}
	else
	{
		nType = 1;
		CString strValue;
		GetDlgItemText(IDC_EDIT_CAPA, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_CAPA)->SetFocus();		
			return;
		}

		dRatio = _ttof(strValue);
	}

	if (nullptr != m_spSeatCapaInfo)
	{
		m_spSeatCapaInfo.reset();
		m_spSeatCapaInfo = KSeatCapaInfoDlgPtr(new KSeatCapaInfoDlg(m_pTarget, nType, m_vecTransitID, dRatio));
		m_spSeatCapaInfo->Create(KSeatCapaInfoDlg::IDD, this);
		m_spSeatCapaInfo->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spSeatCapaInfo = KSeatCapaInfoDlgPtr(new KSeatCapaInfoDlg(m_pTarget, nType, m_vecTransitID, dRatio));
		m_spSeatCapaInfo->Create(KSeatCapaInfoDlg::IDD, this);
		m_spSeatCapaInfo->ShowWindow(SW_SHOW);
	}
}

bool KTransitLineThemesDlg::PathWidthDescending( TDrawLinkInfo left, TDrawLinkInfo right )
{
	return left.TWidth > right.TWidth;
}
