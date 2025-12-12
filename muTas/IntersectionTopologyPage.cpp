/**
 * @file IntersectionTopologyPage.cpp
 * @brief KIntersectionTopologyPage 구현 파일
 * @author jyk@qbicware.com
 * @date 2012.06.07
 * @remark
 */

#include "stdafx.h"
#include "IntersectionTopologyPage.h"
#include "afxdialogex.h"
//^#include "DBaseConnector.h"

// KIntersectionTopologyPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KIntersectionTopologyPage, KDialogEx)

KIntersectionTopologyPage::KIntersectionTopologyPage(KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo, CWnd* pParent)
	: KDialogEx(KIntersectionTopologyPage::IDD, pParent)
	, m_nRadioCtrlNum(0)
	, m_spDBaseConnection(spDBaseConnection)
	, m_oTSelectIntersection(a_oTSelectIntersection)
	, m_mapBoundInfo(a_mapBoundInfo)
{

}

KIntersectionTopologyPage::~KIntersectionTopologyPage()
{
}

void KIntersectionTopologyPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboLeft);
	DDX_Control(pDX, IDC_COMBO2, m_cboTop);
	DDX_Control(pDX, IDC_COMBO3, m_cboRight);
	DDX_Control(pDX, IDC_COMBO4, m_cboBottom);
	DDX_Radio(pDX, IDC_RADIO1,   m_nRadioCtrlNum);
}


BEGIN_MESSAGE_MAP(KIntersectionTopologyPage, KDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KIntersectionTopologyPage::OnCbnSelchangeComboLeft)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KIntersectionTopologyPage::OnCbnSelchangeComboTop)
	ON_CBN_SELCHANGE(IDC_COMBO3, &KIntersectionTopologyPage::OnCbnSelchangeComboRight)
	ON_CBN_SELCHANGE(IDC_COMBO4, &KIntersectionTopologyPage::OnCbnSelchangeComboBottom)
	ON_BN_CLICKED(IDC_RADIO1, &KIntersectionTopologyPage::OnBnClickedRadioLeft)
	ON_BN_CLICKED(IDC_RADIO2, &KIntersectionTopologyPage::OnBnClickedRadioTop)
	ON_BN_CLICKED(IDC_RADIO3, &KIntersectionTopologyPage::OnBnClickedRadioRight)
	ON_BN_CLICKED(IDC_RADIO4, &KIntersectionTopologyPage::OnBnClickedRadioBottom)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// KIntersectionTopologyPage 메시지 처리기입니다.
BOOL KIntersectionTopologyPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if( 3 == m_oTSelectIntersection.nType )
	{
		m_emIntersectionType = KEMIntersectionTypeThree;
		InitIntersectionThreeDir();
	}
	else
	{
		m_emIntersectionType = KEMIntersectionTypeFour;
		InitIntersectionFourDir();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KIntersectionTopologyPage::InitIntersectionFourDir()
{
    CString strNodeID = _T("");  
	strNodeID.Format(_T("%I64d"), m_oTSelectIntersection.nxNodeID);
	SetDlgItemText(IDC_STATIC10, strNodeID);

	InitComboBox(m_cboLeft,   0);
	InitComboBox(m_cboTop,    1);
	InitComboBox(m_cboRight,  2);
	InitComboBox(m_cboBottom, 3);

	GetDlgItem(IDC_RADIO1)->ShowWindow(FALSE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
	GetDlgItem(IDC_RADIO3)->ShowWindow(FALSE);
	GetDlgItem(IDC_RADIO4)->ShowWindow(FALSE);

	SetDisplayBoundInfo(m_cboLeft,   IDC_EDIT1);
	SetDisplayBoundInfo(m_cboTop,    IDC_EDIT2);
	SetDisplayBoundInfo(m_cboRight , IDC_EDIT3);
	SetDisplayBoundInfo(m_cboBottom, IDC_EDIT4);
}


void KIntersectionTopologyPage::InitIntersectionThreeDir()
{
	CString strNodeID = _T("");  
	strNodeID.Format(_T("%I64d"), m_oTSelectIntersection.nxNodeID);
	SetDlgItemText(IDC_STATIC10, strNodeID);

	Integer nxSubBoundNodeID =  m_oTSelectIntersection.nxFourthBNode;
	int     nCrossType       =  m_oTSelectIntersection.nCrossType;

	m_vecMainRoadNum.clear();
	m_vecSubRoadNum.clear();

	if( nxSubBoundNodeID == m_oTSelectIntersection.nxFirstBNode )
		m_vecSubRoadNum.push_back(0);
	if( nxSubBoundNodeID == m_oTSelectIntersection.nxSecondBNode )
		m_vecSubRoadNum.push_back(1);
	if( nxSubBoundNodeID == m_oTSelectIntersection.nxThirdBNode )
		m_vecSubRoadNum.push_back(2);

	if(m_vecSubRoadNum.size() < 1)
		return;

	int nSubNodeIdx = m_vecSubRoadNum[0];
	for(int i= 0; i< 2; i++)
	{
		nSubNodeIdx++;

		if(nSubNodeIdx > 2)
			nSubNodeIdx = 0;

		m_vecMainRoadNum.push_back(nSubNodeIdx);
	}

	InitComboBoxThree(m_cboLeft,   0);
	InitComboBoxThree(m_cboTop,    0);
	InitComboBoxThree(m_cboRight,  0);
	InitComboBoxThree(m_cboBottom, 0);

	CString strLabel = _T("");
	std::vector<int> vecBound;

	switch( nCrossType )
	{
	case CROSS_TYPE_A:
				
		m_cboRight.SetCurSel ( m_vecSubRoadNum[0] );	//sub
		m_cboBottom.SetCurSel( m_vecMainRoadNum[0]);
		m_cboTop.SetCurSel   ( m_vecMainRoadNum[1]);

		strLabel.Format(_T("%d Bound"), m_vecSubRoadNum[0]+1);
		SetDlgItemText(IDC_STATIC3, strLabel);
		strLabel.Format(_T("%d Bound"), m_vecMainRoadNum[0]+1);
		SetDlgItemText(IDC_STATIC4, strLabel);
		strLabel.Format(_T("%d Bound"), m_vecMainRoadNum[1]+1);
		SetDlgItemText(IDC_STATIC2, strLabel);

		EnableControls(1);
		
		m_nRadioCtrlNum = 2;
		break;

	case CROSS_TYPE_E:

		m_cboLeft.SetCurSel  ( m_vecSubRoadNum[0] );	//sub
		m_cboTop.SetCurSel   ( m_vecMainRoadNum[0]);
		m_cboBottom.SetCurSel( m_vecMainRoadNum[1]);

		strLabel.Format(_T("%d Bound"), m_vecSubRoadNum[0]+1);
		SetDlgItemText(IDC_STATIC1, strLabel);
		strLabel.Format(_T("%d Bound"), m_vecMainRoadNum[0]+1);
		SetDlgItemText(IDC_STATIC2, strLabel);
		strLabel.Format(_T("%d Bound"), m_vecMainRoadNum[1]+1);
		SetDlgItemText(IDC_STATIC4, strLabel);

		EnableControls(3);

		m_nRadioCtrlNum = 0;
		break;

	case CROSS_TYPE_O:

		m_cboTop.SetCurSel  ( m_vecSubRoadNum[0] );	//sub
		m_cboRight.SetCurSel( m_vecMainRoadNum[0]);
		m_cboLeft.SetCurSel ( m_vecMainRoadNum[1]);

		strLabel.Format(_T("%d Bound"), m_vecSubRoadNum[0]+1);
		SetDlgItemText(IDC_STATIC2, strLabel);
		strLabel.Format(_T("%d Bound"), m_vecMainRoadNum[0]+1);
		SetDlgItemText(IDC_STATIC3, strLabel);
		strLabel.Format(_T("%d Bound"), m_vecMainRoadNum[1]+1);
		SetDlgItemText(IDC_STATIC1, strLabel);

		EnableControls(4);

		m_nRadioCtrlNum = 1;
		break;

	case CROSS_TYPE_U:

		m_cboBottom.SetCurSel( m_vecSubRoadNum[0] );	//sub
		m_cboLeft.SetCurSel  ( m_vecMainRoadNum[0]);
		m_cboRight.SetCurSel ( m_vecMainRoadNum[1]);

		strLabel.Format(_T("%d Bound"), m_vecSubRoadNum[0]+1);
		SetDlgItemText(IDC_STATIC4, strLabel);
		strLabel.Format(_T("%d Bound"), m_vecMainRoadNum[0]+1);
		SetDlgItemText(IDC_STATIC1, strLabel);
		strLabel.Format(_T("%d Bound"), m_vecMainRoadNum[1]+1);
		SetDlgItemText(IDC_STATIC3, strLabel);

		EnableControls(2);

		m_nRadioCtrlNum = 3;
		break;

	default:
		break;
	}

	SetDisplayBoundInfo(m_cboLeft,   IDC_EDIT1);
	SetDisplayBoundInfo(m_cboTop,    IDC_EDIT2);
	SetDisplayBoundInfo(m_cboRight,  IDC_EDIT3);
	SetDisplayBoundInfo(m_cboBottom, IDC_EDIT4);

	UpdateData(FALSE);
}


void KIntersectionTopologyPage::SetDisplayBoundInfo( CComboBox &a_cboBound, int nIDC )
{
	int     nCurSel   = a_cboBound.GetCurSel();
	Integer nxNodeID  = a_cboBound.GetItemData(nCurSel);

	std::map<Integer, TBoundInfo>::iterator iter = m_mapBoundInfo.find(nxNodeID);
	if(iter != m_mapBoundInfo.end())
	{
		TBoundInfo &oTBoundINfo = iter->second;

		CString strLane = _T("");
		strLane.Format(_T("%d 차로"), oTBoundINfo.nLaneCnt);
		SetDlgItemText(nIDC, strLane);
		
		UpdateData(FALSE);
	}
}


void KIntersectionTopologyPage::InitComboBox( CComboBox &a_cboBound, int a_initIndex )
{
	Integer &nxFirstBNodeID  = m_oTSelectIntersection.nxFirstBNode;
	Integer &nxSecondBNodeID = m_oTSelectIntersection.nxSecondBNode;
	Integer &nxThirdBNodeID  = m_oTSelectIntersection.nxThirdBNode;
	Integer &nxFourthNodeID  = m_oTSelectIntersection.nxFourthBNode;

	CString strNodeID = _T("");

	strNodeID.Format(_T("%I64d"), nxFirstBNodeID);
	int nCur = a_cboBound.AddString(strNodeID);
	a_cboBound.SetItemData( nCur, (DWORD_PTR)( nxFirstBNodeID ) );

	strNodeID.Format(_T("%I64d"), nxSecondBNodeID);
	nCur = a_cboBound.AddString(strNodeID);
	a_cboBound.SetItemData( nCur, (DWORD_PTR)( nxSecondBNodeID ) );

	strNodeID.Format(_T("%I64d"), nxThirdBNodeID);
	nCur = a_cboBound.AddString(strNodeID);
	a_cboBound.SetItemData( nCur, (DWORD_PTR)( nxThirdBNodeID ) );

	strNodeID.Format(_T("%I64d"), nxFourthNodeID);
	nCur = a_cboBound.AddString(strNodeID);
	a_cboBound.SetItemData( nCur, (DWORD_PTR)( nxFourthNodeID ) );

	a_cboBound.SetCurSel(a_initIndex);
}


void KIntersectionTopologyPage::InitComboBoxThree( CComboBox &a_cboBound, int a_initIndex )
{
	a_cboBound.Clear();
	a_cboBound.ResetContent();
	//a_cboBound.SetRedraw(FALSE);

	Integer &nxFirstBNodeID  = m_oTSelectIntersection.nxFirstBNode;
	Integer &nxSecondBNodeID = m_oTSelectIntersection.nxSecondBNode;
	Integer &nxThirdBNodeID  = m_oTSelectIntersection.nxThirdBNode;

	CString strNodeID = _T("");

	strNodeID.Format(_T("%I64d"), nxFirstBNodeID);
	int nCur = a_cboBound.AddString(strNodeID);
	a_cboBound.SetItemData( nCur, (DWORD_PTR)( nxFirstBNodeID ) );

	strNodeID.Format(_T("%I64d"), nxSecondBNodeID);
	nCur = a_cboBound.AddString(strNodeID);
	a_cboBound.SetItemData( nCur, (DWORD_PTR)( nxSecondBNodeID ) );

	strNodeID.Format(_T("%I64d"), nxThirdBNodeID);
	nCur = a_cboBound.AddString(strNodeID);
	a_cboBound.SetItemData( nCur, (DWORD_PTR)( nxThirdBNodeID ) );

	a_cboBound.SetCurSel(a_initIndex);
}


// BOOL KIntersectionTopologyPage::OnApply()
// {
// 	bool bResult = false;
// 	
// 	if( KEMIntersectionTypeThree == m_emIntersectionType )
// 	{
// 		bResult = InputCheckThreeDir();
// 	}
// 	else if( KEMIntersectionTypeFour == m_emIntersectionType )
// 	{
// 		bResult = InputCheckFourDir();
// 	}
// 
// 	if(! bResult)
// 	{
// 		return FALSE;
// 	}
// 	if( KEMIntersectionTypeThree == m_emIntersectionType )
// 	{
// 		bResult = UpdateIntersectionThreeDir();
// 	}
// 	else if(  KEMIntersectionTypeFour == m_emIntersectionType )
// 	{
// 		bResult = UpdateIntersectionFourDir();
// 	}
// 
// 	if(! bResult)
// 	{
// 		return FALSE;
// 	}
// 
// 	KDialogEx::OnApply();
// 	return TRUE;
// }

BOOL KIntersectionTopologyPage::SaveTopology()
{
	bool bResult = false;
	
	if( KEMIntersectionTypeThree == m_emIntersectionType )
	{
		bResult = InputCheckThreeDir();
	}
	else if( KEMIntersectionTypeFour == m_emIntersectionType )
	{
		bResult = InputCheckFourDir();
	}

	if(! bResult)
	{
		return FALSE;
	}
	if( KEMIntersectionTypeThree == m_emIntersectionType )
	{
		bResult = UpdateIntersectionThreeDir();
	}
	else if(  KEMIntersectionTypeFour == m_emIntersectionType )
	{
		bResult = UpdateIntersectionFourDir();
	}

	if(! bResult)
	{
		return FALSE;
	}

	return TRUE;
}


bool KIntersectionTopologyPage::InputCheckThreeDir()
{
	UpdateData(TRUE);

	int nSelectedCrossType  = 0;

	switch(m_nRadioCtrlNum)
	{
	case 0:
		nSelectedCrossType = CROSS_TYPE_E;
		break;
	case 1:
		nSelectedCrossType = CROSS_TYPE_O;
		break;
	case 2:
		nSelectedCrossType = CROSS_TYPE_A;
		break;
	case 3:
		nSelectedCrossType = CROSS_TYPE_U;
		break;
	default:
		break;
	}

	std::set<Integer> setNode;

	switch( nSelectedCrossType )
	{
	case CROSS_TYPE_A:
		setNode.insert(GetCurBoundNode(m_cboRight) );
		setNode.insert(GetCurBoundNode(m_cboBottom));
		setNode.insert(GetCurBoundNode(m_cboTop));

		break;

	case CROSS_TYPE_E:
		setNode.insert(GetCurBoundNode(m_cboLeft) );
		setNode.insert(GetCurBoundNode(m_cboTop));
		setNode.insert(GetCurBoundNode(m_cboBottom));

		break;

	case CROSS_TYPE_O:
		setNode.insert(GetCurBoundNode(m_cboTop));
		setNode.insert(GetCurBoundNode(m_cboRight) );
		setNode.insert(GetCurBoundNode(m_cboLeft) );

		break;

	case CROSS_TYPE_U:
		setNode.insert(GetCurBoundNode(m_cboBottom));
		setNode.insert(GetCurBoundNode(m_cboLeft) );
		setNode.insert(GetCurBoundNode(m_cboRight) );

		break;

	default:
		break;
	}

	if(setNode.size() < 3)	// 중복 된 케이스가 아니라면 size는 3 
	{
		AfxMessageBox(_T("중복하여 선택한 노드 ID가 존재합니다."));
		return false;
	}

	return true;
}


bool KIntersectionTopologyPage::InputCheckFourDir()
{
	UpdateData(TRUE);

	std::set<Integer> setNode;

	setNode.insert(GetCurBoundNode(m_cboLeft));
	setNode.insert(GetCurBoundNode(m_cboTop));
	setNode.insert(GetCurBoundNode(m_cboRight) );
	setNode.insert(GetCurBoundNode(m_cboBottom));

	if(setNode.size() < 4)
	{
		AfxMessageBox(_T("중복하여 선택한 노드 ID가 존재합니다."));
		return false;
	}

	return true;
}



bool KIntersectionTopologyPage::UpdateIntersectionFourDir()
{
	UpdateData(TRUE);
	int     nCurSel         = m_cboLeft.GetCurSel();
	Integer nxFirstBNodeID  = (Integer)m_cboLeft.GetItemData(nCurSel);
	nCurSel                 = m_cboTop.GetCurSel();
	Integer nxSecondBNodeID = (Integer)m_cboTop.GetItemData(nCurSel);
	nCurSel                 = m_cboRight.GetCurSel();
	Integer nxThirdBNodeID  = (Integer)m_cboRight.GetItemData(nCurSel);
	nCurSel                 = m_cboBottom.GetCurSel();
	Integer nxFourthNodeID  = (Integer)m_cboBottom.GetItemData(nCurSel);

	CString  strSQLUpdate(_T(""));

	strSQLUpdate.Format( _T(" UPDATE %s SET ")
		_T(" first_bound_node_id = %I64d, second_bound_node_id = %I64d, third_bound_node_id = %I64d, fourth_bound_node_id = %I64d ")
		_T(" WHERE node_id = %I64d "), TABLE_INTERSECTION, nxFirstBNodeID, nxSecondBNodeID, nxThirdBNodeID, nxFourthNodeID, m_oTSelectIntersection.nxNodeID );
	try
	{  
		if ( m_spDBaseConnection->ExecuteUpdate(strSQLUpdate) != SQLITE_DONE )
		{
			TxLogDebug(strSQLUpdate.AllocSysString());

			return false;
		}

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

	return false;
}


bool KIntersectionTopologyPage::UpdateIntersectionThreeDir()
{
	UpdateData(TRUE);
	int &nOriginCrossType = m_oTSelectIntersection.nCrossType;
	int nEditedCrossType  = 0;

	switch(m_nRadioCtrlNum)
	{
	case 0:
		nEditedCrossType = CROSS_TYPE_E;
		break;
	case 1:
		nEditedCrossType = CROSS_TYPE_O;
		break;
	case 2:
		nEditedCrossType = CROSS_TYPE_A;
		break;
	case 3:
		nEditedCrossType = CROSS_TYPE_U;
		break;
	default:
		break;
	}

	if( nOriginCrossType == nEditedCrossType )
	{
		EditOnlyNode(nOriginCrossType);
	}
	else
	{
		EditNodeAndCross(nEditedCrossType);
	}

	return true;
}


bool KIntersectionTopologyPage::EditOnlyNode( int &a_nOriginCrossType )
{
	UpdateData(TRUE);

	std::vector<CString> vecFieldName;
	std::vector<Integer> vecNode;

	vecFieldName.push_back( ConvertFiledName(m_vecSubRoadNum[0] ) );//sub road
	vecFieldName.push_back( ConvertFiledName(m_vecMainRoadNum[0]) );
	vecFieldName.push_back( ConvertFiledName(m_vecMainRoadNum[1]) );
	vecFieldName.push_back( ConvertFiledName(3)                   );

	switch( a_nOriginCrossType )
	{
	case CROSS_TYPE_A:
		vecNode.push_back(GetCurBoundNode(m_cboRight));	//sub road
		vecNode.push_back(GetCurBoundNode(m_cboBottom));
		vecNode.push_back(GetCurBoundNode(m_cboTop));
		vecNode.push_back(vecNode[0]);

		break;

	case CROSS_TYPE_E:
		vecNode.push_back(GetCurBoundNode(m_cboLeft));	//sub road
		vecNode.push_back(GetCurBoundNode(m_cboTop));
		vecNode.push_back(GetCurBoundNode(m_cboBottom));
		vecNode.push_back(vecNode[0]);

		break;

	case CROSS_TYPE_O:
		vecNode.push_back(GetCurBoundNode(m_cboTop));	//sub road
		vecNode.push_back(GetCurBoundNode(m_cboRight));
		vecNode.push_back(GetCurBoundNode(m_cboLeft));
		vecNode.push_back(vecNode[0]);

		break;

	case CROSS_TYPE_U:
		vecNode.push_back(GetCurBoundNode(m_cboBottom));	//sub road
		vecNode.push_back(GetCurBoundNode(m_cboLeft));
		vecNode.push_back(GetCurBoundNode(m_cboRight));
		vecNode.push_back(vecNode[0]);

		break;

	default:
		break;
	}
    
    CString  strSQLUpdate(_T(""));

	strSQLUpdate.Format( _T(" UPDATE %s SET ")
		_T(" %s = %I64d, %s = %I64d, %s = %I64d, %s = %I64d ")
		_T(" WHERE node_id = %I64d "), TABLE_INTERSECTION, 
		vecFieldName[0], vecNode[0], vecFieldName[1], vecNode[1], vecFieldName[2], vecNode[2], vecFieldName[3], vecNode[3], 
		m_oTSelectIntersection.nxNodeID );
	try
	{  
		if ( m_spDBaseConnection->ExecuteUpdate(strSQLUpdate) != SQLITE_DONE )
		{
			TxLogDebug(strSQLUpdate.AllocSysString());

			return false;
		}

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

	return false;
}


bool KIntersectionTopologyPage::EditNodeAndCross( int &a_nEditedCrossType )
{
	UpdateData(TRUE);

	std::vector<CString> vecFieldName;
	std::vector<Integer> vecNode;

	vecFieldName.push_back( ConvertFiledName(0) );
	vecFieldName.push_back( ConvertFiledName(1) );
	vecFieldName.push_back( ConvertFiledName(2) );
	vecFieldName.push_back( ConvertFiledName(3) );

	switch( a_nEditedCrossType )
	{
	case CROSS_TYPE_A:
		vecNode.push_back(GetCurBoundNode(m_cboTop));
		vecNode.push_back(GetCurBoundNode(m_cboRight));	//sub road
		vecNode.push_back(GetCurBoundNode(m_cboBottom));
		vecNode.push_back(vecNode[1]);

		break;

	case CROSS_TYPE_E:
		vecNode.push_back(GetCurBoundNode(m_cboLeft));	//sub road
		vecNode.push_back(GetCurBoundNode(m_cboTop));
		vecNode.push_back(GetCurBoundNode(m_cboBottom));
		vecNode.push_back(vecNode[0]);

		break;

	case CROSS_TYPE_O:
		vecNode.push_back(GetCurBoundNode(m_cboLeft));
		vecNode.push_back(GetCurBoundNode(m_cboTop));	//sub road
		vecNode.push_back(GetCurBoundNode(m_cboRight));
		vecNode.push_back(vecNode[1]);

		break;

	case CROSS_TYPE_U:
		vecNode.push_back(GetCurBoundNode(m_cboLeft));
		vecNode.push_back(GetCurBoundNode(m_cboRight));
		vecNode.push_back(GetCurBoundNode(m_cboBottom));	//sub road
		vecNode.push_back(vecNode[2]);

		break;

	default:
		break;
	}
    
	CString  strSQLUpdate(_T(""));

	strSQLUpdate.Format( _T(" UPDATE %s SET ")
		_T(" %s = %I64d, %s = %I64d, %s = %I64d, %s = %I64d, %s = %d ")
		_T(" WHERE node_id = %I64d "), TABLE_INTERSECTION, 
		vecFieldName[0], vecNode[0], vecFieldName[1], vecNode[1], vecFieldName[2], vecNode[2], vecFieldName[3], vecNode[3], COLUMN_INTERSECTION_CROSS_TYPE, a_nEditedCrossType,
		m_oTSelectIntersection.nxNodeID );
	try
	{  
		if ( m_spDBaseConnection->ExecuteUpdate(strSQLUpdate) != SQLITE_DONE )
		{
			TxLogDebug(strSQLUpdate.AllocSysString());

			return false;
		}

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

	return false;
}


CString KIntersectionTopologyPage::ConvertFiledName(int a_nIndex)
{
	CString strFiledName(_T(""));

	if(a_nIndex == 0)
		strFiledName = COLUMN_INTERSECTION_FIRST_BOUND_NODE_ID;
	else if(a_nIndex == 1)
		strFiledName = COLUMN_INTERSECTION_SECOND_BOUND_NODE_ID;
	else if(a_nIndex == 2)
		strFiledName = COLUMN_INTERSECTION_THIRD_BOUND_NODE_ID;
	else if(a_nIndex == 3)
		strFiledName = COLUMN_INTERSECTION_FOURTH_BOUND_NODE_ID;

	return strFiledName;
}


Integer KIntersectionTopologyPage::GetCurBoundNode(CComboBox& a_cboBound)
{
	int nCurSel = a_cboBound.GetCurSel();	//sub road
	Integer nxNodeID= a_cboBound.GetItemData(nCurSel);	

	return nxNodeID;
}


void KIntersectionTopologyPage::OnCbnSelchangeComboLeft()
{
	UpdateData(TRUE);        

	SetDisplayBoundInfo(m_cboLeft,   IDC_EDIT1);

	UpdateData(FALSE);
}


void KIntersectionTopologyPage::OnCbnSelchangeComboTop()
{
	UpdateData(TRUE);        

	SetDisplayBoundInfo(m_cboTop,    IDC_EDIT2);

	UpdateData(FALSE);
}


void KIntersectionTopologyPage::OnCbnSelchangeComboRight()
{
	UpdateData(TRUE);        

	SetDisplayBoundInfo(m_cboRight,  IDC_EDIT3);

	UpdateData(FALSE);
}


void KIntersectionTopologyPage::OnCbnSelchangeComboBottom()
{
	UpdateData(TRUE);        

	SetDisplayBoundInfo(m_cboBottom, IDC_EDIT4);

	UpdateData(FALSE);
}

// 'ㅓ'
void KIntersectionTopologyPage::OnBnClickedRadioLeft()
{
	UpdateData(TRUE);

	if( CROSS_TYPE_E == m_oTSelectIntersection.nCrossType )
	{
		InitIntersectionThreeDir();
	}
	else
	{
		m_cboLeft.SetCurSel  (0);	//sub
		m_cboTop.SetCurSel   (1);
		m_cboBottom.SetCurSel(2);

		SetDlgItemText(IDC_STATIC1, _T("1 Bound"));
		SetDlgItemText(IDC_STATIC2, _T("2 Bound"));
		SetDlgItemText(IDC_STATIC4, _T("3 Bound"));

		SetDisplayBoundInfo(m_cboLeft,   IDC_EDIT1);
		SetDisplayBoundInfo(m_cboTop,    IDC_EDIT2);
		SetDisplayBoundInfo(m_cboRight,  IDC_EDIT3);
		SetDisplayBoundInfo(m_cboBottom, IDC_EDIT4);
	}

	EnableControls(3);

	UpdateData(FALSE);
}

// 'ㅗ'
void KIntersectionTopologyPage::OnBnClickedRadioTop()
{
	UpdateData(TRUE);

	if( CROSS_TYPE_O == m_oTSelectIntersection.nCrossType )
	{
		InitIntersectionThreeDir();
	}
	else
	{
		m_cboLeft.SetCurSel (0);
		m_cboTop.SetCurSel  (1);	//sub
		m_cboRight.SetCurSel(2);

		SetDlgItemText(IDC_STATIC1, _T("1 Bound"));
		SetDlgItemText(IDC_STATIC2, _T("2 Bound"));
		SetDlgItemText(IDC_STATIC3, _T("3 Bound"));

		SetDisplayBoundInfo(m_cboLeft,   IDC_EDIT1);
		SetDisplayBoundInfo(m_cboTop,    IDC_EDIT2);
		SetDisplayBoundInfo(m_cboRight,  IDC_EDIT3);
		SetDisplayBoundInfo(m_cboBottom, IDC_EDIT4);
	}

	EnableControls(4);

	UpdateData(FALSE);
}

// 'ㅏ' 
void KIntersectionTopologyPage::OnBnClickedRadioRight()
{
	UpdateData(TRUE);

	if( CROSS_TYPE_A == m_oTSelectIntersection.nCrossType )
	{
		InitIntersectionThreeDir();
	}
	else
	{
		m_cboTop.SetCurSel    (0);
		m_cboRight.SetCurSel  (1);	//sub
		m_cboBottom.SetCurSel (2);

		SetDlgItemText(IDC_STATIC2, _T("1 Bound"));
		SetDlgItemText(IDC_STATIC3, _T("2 Bound"));
		SetDlgItemText(IDC_STATIC4, _T("3 Bound"));

		SetDisplayBoundInfo(m_cboLeft,   IDC_EDIT1);
		SetDisplayBoundInfo(m_cboTop,    IDC_EDIT2);
		SetDisplayBoundInfo(m_cboRight,  IDC_EDIT3);
		SetDisplayBoundInfo(m_cboBottom, IDC_EDIT4);
	}

	EnableControls(1);

	UpdateData(FALSE);
}

// 'ㅜ'
void KIntersectionTopologyPage::OnBnClickedRadioBottom()
{
	UpdateData(TRUE);

	if( CROSS_TYPE_U == m_oTSelectIntersection.nCrossType )
	{
		InitIntersectionThreeDir();
	}
	else
	{
		m_cboLeft.SetCurSel   (0);
		m_cboRight.SetCurSel  (1);	//sub
		m_cboBottom.SetCurSel (2);

		SetDlgItemText(IDC_STATIC1, _T("1 Bound"));
		SetDlgItemText(IDC_STATIC3, _T("2 Bound"));
		SetDlgItemText(IDC_STATIC4, _T("3 Bound"));

		SetDisplayBoundInfo(m_cboLeft,   IDC_EDIT1);
		SetDisplayBoundInfo(m_cboTop,    IDC_EDIT2);
		SetDisplayBoundInfo(m_cboRight,  IDC_EDIT3);
		SetDisplayBoundInfo(m_cboBottom, IDC_EDIT4);
	}

	EnableControls(2);

	UpdateData(FALSE);
}


void KIntersectionTopologyPage::EnableControls(int a_nIndex)
{
	BOOL bEnable  = TRUE;
	if( 1 == a_nIndex )
	{
		bEnable = FALSE;
	}
	m_cboLeft.EnableWindow  (bEnable);	
	GetDlgItem(IDC_EDIT1)->ShowWindow(bEnable);
	GetDlgItem(IDC_STATIC1)->ShowWindow(bEnable);
	GetDlgItem(IDC_COMBO1)->ShowWindow(bEnable);
	GetDlgItem(IDC_LINE1)->ShowWindow(bEnable);

	bEnable  = TRUE;
	if( 2 == a_nIndex )
	{
		bEnable = FALSE;
	}
	m_cboTop.EnableWindow  (bEnable);	
	GetDlgItem(IDC_EDIT2)->ShowWindow(bEnable);
	GetDlgItem(IDC_STATIC2)->ShowWindow(bEnable);
	GetDlgItem(IDC_COMBO2)->ShowWindow(bEnable);
	GetDlgItem(IDC_LINE2)->ShowWindow(bEnable);

	bEnable  = TRUE;
	if( 3 == a_nIndex )
	{
		bEnable = FALSE;
	}
	m_cboRight.EnableWindow  (bEnable);	
	GetDlgItem(IDC_EDIT3)->ShowWindow(bEnable);
	GetDlgItem(IDC_STATIC3)->ShowWindow(bEnable);
	GetDlgItem(IDC_COMBO3)->ShowWindow(bEnable);
	GetDlgItem(IDC_LINE3)->ShowWindow(bEnable);

	bEnable  = TRUE;
	if( 4 == a_nIndex )
	{
		bEnable = FALSE;
	}
	m_cboBottom.EnableWindow  (bEnable);	
	GetDlgItem(IDC_EDIT4)->ShowWindow(bEnable);
	GetDlgItem(IDC_STATIC4)->ShowWindow(bEnable);
	GetDlgItem(IDC_COMBO4)->ShowWindow(bEnable);
	GetDlgItem(IDC_LINE4)->ShowWindow(bEnable);
}


HBRUSH KIntersectionTopologyPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	return hbr;
}
