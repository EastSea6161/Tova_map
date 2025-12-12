// KAOBDefineVDFDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AOBDefineVDFDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <iostream>
#include "DBaseDefaultParaHighwayAssign.h"
#include "DBaseConnector.h"

// KAOBDefineVDFDlg 대화 상자입니다.

using namespace std;

IMPLEMENT_DYNAMIC(KAOBDefineVDFDlg, KDialogEx)

KAOBDefineVDFDlg::KAOBDefineVDFDlg(KTarget* a_pTarget,CWnd* pParent /*=NULL*/)
	: KDialogEx(KAOBDefineVDFDlg::IDD, pParent)
	, m_nCntSelInputMode(0)
	, m_nModeInVDF(1)
	, m_singleVDFDlg(a_pTarget)
	, m_pTarget(a_pTarget)
	, m_multiVDFDlg(a_pTarget)
	, m_bUseDefault(true)
{
	//m_pTarget = NULL;
}

KAOBDefineVDFDlg::~KAOBDefineVDFDlg()
{

}

void KAOBDefineVDFDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboModeCntInVDF);
}


BEGIN_MESSAGE_MAP(KAOBDefineVDFDlg, KDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KAOBDefineVDFDlg::OnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &KAOBDefineVDFDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &KAOBDefineVDFDlg::OnBnClickedButtonDefault)
END_MESSAGE_MAP()


// KAOBDefineVDFDlg 메시지 처리기입니다.


void KAOBDefineVDFDlg::InputList( std::list<KAInputInfo*>& inputList )
{
	m_nCntSelInputMode = 0;

	m_InputList = inputList;

	std::list<KAInputInfo*>::iterator itInput, itEnd = m_InputList.end();
	for(itInput = m_InputList.begin(); itInput != itEnd; ++itInput)
	{
		KAInputInfo* pInput  = *itInput;

		if (pInput->Selected())
		{
			++m_nCntSelInputMode;
		}
	}
}


BOOL KAOBDefineVDFDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWnd* pWnd = GetDlgItem(IDC_FRAME);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_singleVDFDlg.Create(IDD_6721_AOBSingleVDFDlg, this);
	m_singleVDFDlg.UsedDefalut(m_bUseDefault);
	m_singleVDFDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_singleVDFDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	m_multiVDFDlg.Create(IDD_6722_AOBMultiVDFDlg, this);
	m_multiVDFDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_multiVDFDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);


	InitComboModeCntInVDF();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KAOBDefineVDFDlg::InitComboModeCntInVDF( void )
{
	m_cboModeCntInVDF.ResetContent();

	CString strComboText(_T(""));
	int     nCboIdx(0);

	ASSERT( 0 != m_nCntSelInputMode );

	for (int i = 1; i <= m_nCntSelInputMode; ++i)
	{
		strComboText.Format( _T("%d"), i );
		nCboIdx = m_cboModeCntInVDF.InsertString( -1, strComboText );
		m_cboModeCntInVDF.SetItemData( nCboIdx, (DWORD_PTR)i );
	}

	m_cboModeCntInVDF.SetCurSel( m_nModeInVDF - 1 );


	int nModeNumInVDF = m_cboModeCntInVDF.GetItemData(m_cboModeCntInVDF.GetCurSel());

	if ( 1 > nModeNumInVDF)
	{
		m_cboModeCntInVDF.SetCurSel(0);
		m_multiVDFDlg.ShowWindow(SW_HIDE);
		m_singleVDFDlg.ShowWindow(SW_SHOW);
		if (m_oABSingeVDFInfo.TbUseVDf == true)
		{
			m_singleVDFDlg.SetOBSingleInfo(m_oABSingeVDFInfo);
		}
	}
	else if ( 1 == nModeNumInVDF )
	{
		m_multiVDFDlg.ShowWindow(SW_HIDE);
		m_singleVDFDlg.ShowWindow(SW_SHOW);
		if (m_oABSingeVDFInfo.TbUseVDf == true)
		{
			m_singleVDFDlg.SetOBSingleInfo(m_oABSingeVDFInfo);
		}
	}
	else
	{
		m_multiVDFDlg.ShowWindow(SW_SHOW);
		m_singleVDFDlg.ShowWindow(SW_HIDE);

		//m_multiVDFDlg.SetTarget(m_pTarget);
		m_multiVDFDlg.SetModeNumInVDF(nModeNumInVDF);
		m_multiVDFDlg.SetInputList(m_InputList);
		if (m_oABMultiVDFInfo.TbUseVDf == false)
		{
			m_multiVDFDlg.SetInitData();
		}
		else
		{
			m_multiVDFDlg.SetInitEachLinkData();
			m_multiVDFDlg.SetComboBoxInfo(m_oABMultiVDFInfo);
		}
	}
	//OnSelchangeCombo1();
}


void KAOBDefineVDFDlg::OnSelchangeCombo1()
{
	int nModeNumInVDF = m_cboModeCntInVDF.GetItemData(m_cboModeCntInVDF.GetCurSel());

	if ( 1 > nModeNumInVDF)
	{
		m_cboModeCntInVDF.SetCurSel(0);
		m_multiVDFDlg.ShowWindow(SW_HIDE);
		m_singleVDFDlg.ShowWindow(SW_SHOW);
		if (m_oABSingeVDFInfo.TbUseVDf == true)
		{
			m_singleVDFDlg.SetOBSingleInfo(m_oABSingeVDFInfo);
		}
	}
	else if ( 1 == nModeNumInVDF )
	{
		m_multiVDFDlg.ShowWindow(SW_HIDE);
		m_singleVDFDlg.ShowWindow(SW_SHOW);
		if (m_oABSingeVDFInfo.TbUseVDf == true)
		{
			m_singleVDFDlg.SetOBSingleInfo(m_oABSingeVDFInfo);
		}
	}
	else
	{
		m_multiVDFDlg.ShowWindow(SW_SHOW);
		m_singleVDFDlg.ShowWindow(SW_HIDE);

		//m_multiVDFDlg.SetTarget(m_pTarget);
		m_multiVDFDlg.SetModeNumInVDF(nModeNumInVDF);
		m_multiVDFDlg.SetInputList(m_InputList);
		m_multiVDFDlg.SetInitData();
	}
}


void KAOBDefineVDFDlg::OnBnClickedOk()
{
	bool bUseVDF = false;

	int nCurIdx = m_cboModeCntInVDF.GetCurSel();

	m_nModeInVDF = nCurIdx + 1;

	if (m_nModeInVDF > 1)
	{
		bUseVDF =  m_multiVDFDlg.GetAOBMultiVDFInfo(m_oABMultiVDFInfo);
		m_oABMultiVDFInfo.TbUseVDf = bUseVDF;
	}
	else
	{
		bUseVDF = m_singleVDFDlg.GetOBSingleInfo(m_oABSingeVDFInfo);
		m_oABSingeVDFInfo.TbUseVDf = bUseVDF;
	}

	if (bUseVDF == false)
	{
		return;
	}
	else
	{
		KDialogEx::OnOK();
	}
	
}

void KAOBDefineVDFDlg::SetTarget( KTarget* pTarget )
{
	m_pTarget = pTarget;
}


int KAOBDefineVDFDlg::GetModeInVdf()
{
	return m_nModeInVDF;
}


void KAOBDefineVDFDlg::GetMultiVDFInfo( TAOBMultiVDF& a_TOBMultiVDF )
{
	a_TOBMultiVDF = m_oABMultiVDFInfo;
}


bool KAOBDefineVDFDlg::WriteInputArgument( CStdioFileEx& of )
{
	bool bResult = false;
	if (m_nModeInVDF < 2)
	{
		bResult  = WriteInputSingleArgumet(of);

	}
	else
	{
		bResult = WriteInputMultiArgument(of);
	}

	return bResult;
}


bool KAOBDefineVDFDlg::WriteInputMultiArgument( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		if(m_oABMultiVDFInfo.TemEachFunctionType == KEMAssignmentBPRFunctionAllLink)
		{
		}
		else if(m_oABMultiVDFInfo.TemEachFunctionType == KEMAssignmentBPRFunctionEachFn)
		{
			int nRecordCount = m_oABMultiVDFInfo.TvecFucntionType.size();

			int nVDFRows = nRecordCount + 2;

			strOut.Format(_T("vdf\t%d\r\n"), nVDFRows);
			of.WriteString(strOut);


			strOut.Format(_T("%d\t%d\t"), m_oABMultiVDFInfo.TemEachFunctionType
										, m_nModeInVDF);

			CString strMakeOut;
			std::map<int, double> mapVDFValue = m_oABMultiVDFInfo.TmapVDFValue;

			std::map<int, double>:: iterator iter = mapVDFValue.begin();
			std::map<int, double>:: iterator end  = mapVDFValue.end();
			while(iter != end)
			{
				double dValue = iter->second;
				strMakeOut.Format(_T("%f\t"), dValue);
				strOut = strOut + strMakeOut;
				++iter;
			}

			strMakeOut.Format(_T("%d\r\n"),m_oABMultiVDFInfo.TnStandardVDF);
			strOut = strOut + strMakeOut;
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\t\r\n"), m_oABMultiVDFInfo.TbPamAlBeta?1:0,
				m_oABMultiVDFInfo.TbPamabToll?1:0,
				m_oABMultiVDFInfo.TbpamIntialTime?1:0);
			of.WriteString(strOut);

			for (int i = 0; i < nRecordCount; ++i)
			{
				std::vector<CString> vecRow =  m_oABMultiVDFInfo.TvecFucntionType[i];

				int nRowCount = vecRow.size();
				
				strOut = _T("");

				for (int nIndex = 0; nIndex < nRowCount; nIndex++ )
				{
					strMakeOut.Format(_T("%s\t"), vecRow[nIndex]);
					strOut = strOut + strMakeOut;
				}
				strOut = strOut +_T("\t\r\n");
				of.WriteString(strOut);
			}

		}
		else if (m_oABMultiVDFInfo.TemEachFunctionType == KEMAssignmentBPRFunctionEachLink)
		{
			strOut.Format(_T("vdf\t%d\r\n"), 2);
			of.WriteString(strOut);


			strOut.Format(_T("%d\t%d\t"), m_oABMultiVDFInfo.TemEachFunctionType
				, m_nModeInVDF);

			CString strMakeOut = _T("");
			std::map<int, double> mapVDFValue = m_oABMultiVDFInfo.TmapVDFValue;

			std::map<int, double>:: iterator iter = mapVDFValue.begin();
			std::map<int, double>:: iterator end  = mapVDFValue.end();
			while(iter != end)
			{
				double dValue = iter->second;
				strMakeOut.Format(_T("%f\t"), dValue);
				strOut = strOut + strMakeOut;
				++iter;
			}

			strMakeOut.Format(_T("%d\r\n"),m_oABMultiVDFInfo.TnStandardVDF);
			strOut = strOut + strMakeOut;
			of.WriteString(strOut);


			strOut.Format(_T("%d\t%d\t%d\t\r\n"), m_oABMultiVDFInfo.TbPamAlBeta?1:0,
												  m_oABMultiVDFInfo.TbPamabToll?1:0,
												  m_oABMultiVDFInfo.TbpamIntialTime?1:0);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


bool KAOBDefineVDFDlg::ExportDB2LinkFile( KDBaseConPtr a_spDBaseConnection )
{
	if (m_nModeInVDF < 2)
	{
		ExportDB2SingeLinkFile(a_spDBaseConnection);
		return true;
	}
	else
	{
		ExportDB2MulityLinkFile(a_spDBaseConnection);
		return true;
	}
}


bool KAOBDefineVDFDlg::ExportDB2SingeLinkFile( KDBaseConPtr a_spDBaseConnection )
{
	if (m_oABSingeVDFInfo.TemFunctionType != KEMAssignmentBPRFunctionEachLink)
	{
		TxLogDebugStartMsg();

		ofstream ofs(_T("link.dat"), ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		CString strlinkOffResult(_T(""));
		CString strlinkOffSQL = _T("select link_off from network_off");

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strlinkOffSQL);

			if (spResultSet->Next())
			{
				strlinkOffResult = spResultSet->GetValueString(0);
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		CString strTok(_T(""));
		int nLoop = 0;
		std::vector<CString> vecOffType;

		while (AfxExtractSubString(strTok, strlinkOffResult, nLoop++, ','))
		{
			if (strTok.CompareNoCase(_T("")) == 0)
				break;

			vecOffType.push_back(strTok);
		}

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id From link") ;

		if (vecOffType.size() != 0)
		{
			strSQL.Append(_T(" where link_type not in ("));
		}

		int nSize = vecOffType.size();

		for (int i = 0; i < nSize; ++i)
		{
			if (0 != i)
				strSQL.Append(_T(","));

			strSQL.Append(_T(" ") + vecOffType[i]);

			if (nSize - 1 == i)
				strSQL.Append(_T(" )"));
		}
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= 0.0;
				fixedLink.beta		= 0.0;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}
	else
	{
		TxLogDebugStartMsg();

		ofstream ofs(_T("link.dat"), ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		//network_off read
		CString strlinkOffResult(_T(""));
		CString strlinkOffSQL = _T("select link_off from network_off");

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strlinkOffSQL);

			if (spResultSet->Next())
			{
				strlinkOffResult = spResultSet->GetValueString(0);
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		CString strTok(_T(""));
		int nLoop = 0;
		std::vector<CString> vecOffType;

		while (AfxExtractSubString(strTok, strlinkOffResult, nLoop++, ','))
		{
			if (strTok.CompareNoCase(_T("")) == 0)
				break;

			vecOffType.push_back(strTok);
		}

		CString strAlphaColumnName		= m_oABSingeVDFInfo.TstrAlpha;
		CString strBetaColumnName		= m_oABSingeVDFInfo.TstrBeta;
		CString strATollColumnName		= m_oABSingeVDFInfo.TA;
		CString strBTollColumnName		= m_oABSingeVDFInfo.TB;
 
		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;

		strSQL  = strSQL + _T(", ") + strAlphaColumnName + _T(", ") + strBetaColumnName;

		if (strATollColumnName != _T(""))
		{
			strSQL = strSQL + _T(", ") + strATollColumnName;
		}

		if (strBTollColumnName !=_T(""))
		{
			strSQL = strSQL + _T(", ") + strBTollColumnName;
		}

		strSQL = strSQL + _T(" From link");

		if (vecOffType.size() != 0)
		{
			strSQL.Append(_T("where link_type not in ("));
		}

		int nSize = vecOffType.size();

		for (int i = 0; i < nSize; ++i)
		{
			if (0 != i)
				strSQL.Append(_T(","));

			strSQL.Append(_T(" ") + vecOffType[i]);

			if (nSize - 1 == i)
				strSQL.Append(_T(" )"));
		}
		strSQL.Append(_T(" Order By link_id "));
		
		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= spResultSet->GetValueDouble(9);
				fixedLink.beta		= spResultSet->GetValueDouble(10);

				if (strATollColumnName == _T(""))
				{
					fixedLink.a		= 0.0;
				}
				else
				{
					fixedLink.a			= spResultSet->GetValueDouble(11);
				}

				if (strBTollColumnName == _T(""))
				{
					fixedLink.b		= 0.0;
				}
				else
				{
					fixedLink.b			= spResultSet->GetValueDouble(12);
				}

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
			}
		}
		catch (...)
		{
			TxLogDebugException();
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}

	return false;
}


bool KAOBDefineVDFDlg::ExportDB2MulityLinkFile( KDBaseConPtr a_spDBaseConnection )
{
	if (m_oABMultiVDFInfo.TemEachFunctionType != KEMAssignmentBPRFunctionEachLink)
	{
		TxLogDebugStartMsg();

		ofstream ofs(_T("link.dat"), ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		CString strlinkOffResult(_T(""));
		CString strlinkOffSQL = _T("select link_off from network_off");

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strlinkOffSQL);
			if (spResultSet->Next())
			{
				strlinkOffResult = spResultSet->GetValueString(0);
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		CString strTok(_T(""));
		int nLoop = 0;
		std::vector<CString> vecOffType;

		while (AfxExtractSubString(strTok, strlinkOffResult, nLoop++, ','))
		{
			if (strTok.CompareNoCase(_T("")) == 0)
				break;

			vecOffType.push_back(strTok);
		}

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id From link") ;

		if (vecOffType.size() != 0)
		{
			strSQL.Append(_T(" where link_type not in ("));
		}

		int nSize = vecOffType.size();

		for (int i = 0; i < nSize; ++i)
		{
			if (0 != i)
				strSQL.Append(_T(","));

			strSQL.Append(_T(" ") + vecOffType[i]);

			if (nSize - 1 == i)
				strSQL.Append(_T(" )"));
		}
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= 0.0;
				fixedLink.beta		= 0.0;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}
	else
	{
		TxLogDebugStartMsg();


		ofstream ofs(_T("link.dat"), ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		//network_off read
		CString strlinkOffResult(_T(""));
		CString strlinkOffSQL = _T("select node_off from network_off");

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strlinkOffSQL);	

			if (spResultSet->Next())
			{
				strlinkOffResult = spResultSet->GetValueString(0);
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		CString strTok(_T(""));
		int nLoop = 0;
		std::vector<CString> vecOffType;

		while (AfxExtractSubString(strTok, strlinkOffResult, nLoop++, ','))
		{
			if (strTok.CompareNoCase(_T("")) == 0)
				break;

			vecOffType.push_back(strTok);
		}

		CString strColumnName = _T("");
		bool    bCheckName	  = false;
		std::vector<CString> vecEachLinkColumnName = m_oABMultiVDFInfo.TvecEachLinkColumn;

		//int nCount = vecEachLinkColumnName.size();
		/*int nIndexCount = nCount -1;
		strColumnName = vecEachLinkColumnName[0];
		
		for (int i = 0; i < nIndexCount; i++)
		{
			int nIndex = i + 1;
			CString strEachLinkColumn = vecEachLinkColumnName[nIndex]; 
			strColumnName = strColumnName + _T(",") + strEachLinkColumn;
		}*/

		int nCount = (int)vecEachLinkColumnName.size();

		strColumnName = vecEachLinkColumnName[0];
		if (strColumnName == _T(""))
		{
			strColumnName = _T("0.0");
		}

		for (int i = 1; i < nCount; i ++)
		{
			CString strEachColumnName = vecEachLinkColumnName[i];
			if (strEachColumnName == _T(""))
			{
				strEachColumnName = _T("0.0");
			}

			strColumnName = strColumnName + _T(", ") + strEachColumnName;
		}

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;

		strSQL  = strSQL + _T(", ") + strColumnName;

		strSQL = strSQL + _T(" From link");

		if (vecOffType.size() != 0)
		{
			strSQL.Append(_T(" where node_type not in ("));
		}

		int nSize = vecOffType.size();

		for (int i = 0; i < nSize; ++i)
		{
			if (0 != i)
				strSQL.Append(_T(","));

			strSQL.Append(_T(" ") + vecOffType[i]);

			if (nSize - 1 == i)
				strSQL.Append(_T(" )"));
		}
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				Integer nxlinkid   = spResultSet->GetValueInt64(0);
				ofs.write(reinterpret_cast<char*>( &nxlinkid ), sizeof(Integer) );  
				Integer nxfnodeid  = spResultSet->GetValueInt64(1);
				ofs.write(reinterpret_cast<char*>( &nxfnodeid ), sizeof(Integer) );  
				Integer nxtnodeid  = spResultSet->GetValueInt64(2);
				ofs.write(reinterpret_cast<char*>( &nxtnodeid ), sizeof(Integer) );
				int		nlinktype	= spResultSet->GetValueInt (3);
				ofs.write(reinterpret_cast<char*>( &nlinktype ), sizeof(int) );
				double	dlength		= spResultSet->GetValueDouble(4);
				ofs.write(reinterpret_cast<char*>( &dlength ), sizeof(double));
				int		nlane		= spResultSet->GetValueInt   (5);
				ofs.write(reinterpret_cast<char*>( &nlane ), sizeof(int) );
				double	dcapa		= spResultSet->GetValueDouble(6);
				ofs.write(reinterpret_cast<char*>( &dcapa ), sizeof(double));
				double	dspeed		= spResultSet->GetValueDouble(7); 
				ofs.write(reinterpret_cast<char*>( &dspeed ), sizeof(double));
				int	nfnNumber		= spResultSet->GetValueInt   (8);
				ofs.write(reinterpret_cast<char*>( &nfnNumber ), sizeof(int) );

				int nIndex = 9;
				for (int i = 0; i < nCount; i ++ )
				{
					double dAB	    = spResultSet->GetValueDouble(nIndex);
					ofs.write(reinterpret_cast<char*>( &dAB ), sizeof(double));
					nIndex++;
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}

	return false;
}


bool KAOBDefineVDFDlg::WriteInputSingleArgumet( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		if(m_oABSingeVDFInfo.TemFunctionType == KEMAssignmentBPRFunctionAllLink)
		{
			of.WriteString(_T("vdf\t2\r\n")); 

			strOut.Format(_T("%d\t%d\t%f\t%d\r\n"), m_oABSingeVDFInfo.TemFunctionType, 1, m_oABSingeVDFInfo.TdValueofTime , 1);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\t\r\n"), 0 ,0 ,0);
			of.WriteString(strOut);

// 			strOut.Format(_T("%d\t%f\t%f\t0\t0\t\r\n"), 0, m_oABSingeVDFInfo.TdAllAlpha, m_oABSingeVDFInfo.TdAllBeta);
// 			of.WriteString(strOut);
		}
		else if(m_oABSingeVDFInfo.TemFunctionType == KEMAssignmentBPRFunctionEachFn)
		{
// 			std::vector<std::vector<CString>> vecReportData;
// 
// 			vecReportData = m_oABSingeVDFInfo.TvecFucntionType;
// 			
// 			int nRecordCount = vecReportData.size();
// 
// 			int nVDFRows = nRecordCount + 2;

			strOut.Format(_T("vdf\t%d\r\n"), 2);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%f\t%d\r\n"), m_oABSingeVDFInfo.TemFunctionType, 1, m_oABSingeVDFInfo.TdValueofTime ,1);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\t\r\n"), 0 ,0 ,0);
			of.WriteString(strOut);

// 			int nIndex;
// 			double dAlpha, dBeta, dA, dB;
// 
// 			for (int i = 0; i < nRecordCount; ++i)
// 			{
// 				std::vector<CString> vecRow = vecReportData[i];
// 
// 				nIndex	= _ttoi(vecRow[0]);
// 				dAlpha	= _ttof(vecRow[1]);
// 				dBeta	= _ttof(vecRow[2]);
// 				dA		= _ttof(vecRow[3]);
// 				dB		= _ttof(vecRow[4]);
// 
// 				strOut.Format(_T("%d\t%g\t%g\t%g\t%g\r\n"), nIndex, dAlpha, dBeta, dA, dB);
// 				of.WriteString(strOut);
// 			}

		}
		else if (m_oABSingeVDFInfo.TemFunctionType == KEMAssignmentBPRFunctionEachLink)
		{
			strOut.Format(_T("vdf\t%d\r\n"), 2);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%f\t%d\r\n"), m_oABSingeVDFInfo.TemFunctionType, 1, m_oABSingeVDFInfo.TdValueofTime ,1 );
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%d\t%d\r\n"), 0 ,0 ,0);
			of.WriteString(strOut);

		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


bool KAOBDefineVDFDlg::ExportDB2LinkFileExcludeType( KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation)
{
	if (m_nModeInVDF < 2)
	{
		ExportDB2SingeLinkFileExcludeType(a_spDBaseConnection, a_setIncludeLink, strLocation);
		return true;
	}
	else
	{
		ExportDB2MulityLinkFileExcludType(a_spDBaseConnection, a_setIncludeLink, strLocation);
		return true;
	}
}

bool KAOBDefineVDFDlg::ExportDB2SingeLinkFileExcludeType( KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation )
{
	if (m_oABSingeVDFInfo.TemFunctionType == KEMAssignmentBPRFunctionAllLink)
	{
		TxLogDebugStartMsg();

		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}
		
		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id From link") ;
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			int nIndex(0);
			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= m_oABSingeVDFInfo.TdAllAlpha;
				fixedLink.beta		= m_oABSingeVDFInfo.TdAllBeta;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if (iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
				}

				if (fixedLink.link_id == 286)
				{
					nIndex++;
				}


				nIndex++;
			}

			if (nIndex < 80000)
			{
				int a = 10;
			}

		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;

	}
	else if (m_oABSingeVDFInfo.TemFunctionType == KEMAssignmentBPRFunctionEachFn)
	{
		TxLogDebugStartMsg();

		bool bUseAlpha = true;
		bool bUseBeta  = true;
		bool bUseA     = true;
		bool bUseB     = true;
		
		//GenerateQuery;
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		CString strQuery, strAddQuery, strTemp;
		strQuery.Format(_T("SELECT alpha_column_name, beta_column_name, a_column_name, b_column_name FROM function_group WHERE function_group_id = %d"), 
			m_oABSingeVDFInfo.TnEachFnCombo);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("데이터가 없습니다."));

		//Alpha
		strTemp = spResultSet->GetValueString(0);

		if (strTemp.IsEmpty())
		{
			bUseAlpha = false;
			strTemp   = _T("''");
		}

		strAddQuery.Format(_T(", %s as Alpha"), strTemp);

		//Beta
		strTemp = spResultSet->GetValueString(1);

		if (strTemp.IsEmpty())
		{
			bUseBeta = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as Beta"), strTemp);

		//a
		strTemp = spResultSet->GetValueString(2);

		if (strTemp.IsEmpty())
		{
			bUseA = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as a"), strTemp);

		//b
		strTemp = spResultSet->GetValueString(3);

		if (strTemp.IsEmpty())
		{
			bUseB = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as b"), strTemp);

		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		std::map<int, std::vector<double>>::iterator fIter, eIter = m_oABSingeVDFInfo.TmapEachFN.end();

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ");
		strSQL.Append(strAddQuery);
		strSQL.Append(_T(" From link"));
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= 0.0;
				fixedLink.beta		= 0.0;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				fIter = m_oABSingeVDFInfo.TmapEachFN.find(fixedLink.fuNumber);

                // TODO : 
				if (fIter == eIter)
					ThrowException(_T("정의되지 않은 functionID입니다."));

				std::vector<double> vecEachFn = fIter->second;

				if (bUseAlpha)
				{
					fixedLink.alpha	= spResultSet->GetValueDouble(9);
				}
				else
				{
					fixedLink.alpha = vecEachFn[0];
				}

				if (bUseBeta)
				{
					fixedLink.beta = spResultSet->GetValueDouble(10);
				}
				else
				{
					fixedLink.beta = vecEachFn[1];
				}

				if (bUseA)
				{
					fixedLink.a	= spResultSet->GetValueDouble(11);
				}
				else
				{
					fixedLink.a = vecEachFn[2];
				}

				if (bUseB)
				{
					fixedLink.b	= spResultSet->GetValueDouble(12);
				}
				else
				{
					fixedLink.b = vecEachFn[3];
				}

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if (iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}
	else
	{
		TxLogDebugStartMsg();

		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}


		CString strAlphaColumnName		= m_oABSingeVDFInfo.TstrAlpha;
		CString strBetaColumnName		= m_oABSingeVDFInfo.TstrBeta;
		CString strATollColumnName		= m_oABSingeVDFInfo.TA;
		CString strBTollColumnName		= m_oABSingeVDFInfo.TB;

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;

		strSQL  = strSQL + _T(", ") + strAlphaColumnName + _T(", ") + strBetaColumnName;

		if (strATollColumnName != _T(""))
		{
			strSQL = strSQL + _T(", ") + strATollColumnName;
		}

		if (strBTollColumnName !=_T(""))
		{
			strSQL = strSQL + _T(", ") + strBTollColumnName;
		}

		strSQL = strSQL + _T(" From link");

		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResult->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResult->GetValueInt64 (0);
				fixedLink.fnode_id  = spResult->GetValueInt64 (1);
				fixedLink.tnode_id  = spResult->GetValueInt64 (2);
				fixedLink.link_type = spResult->GetValueInt   (3);
				fixedLink.length    = spResult->GetValueDouble(4);
				fixedLink.lane      = spResult->GetValueInt   (5);
				fixedLink.capa      = spResult->GetValueDouble(6);
				fixedLink.speed     = spResult->GetValueDouble(7);
				fixedLink.fuNumber	= spResult->GetValueInt   (8);
				fixedLink.alpha		= spResult->GetValueDouble(9);
				fixedLink.beta		= spResult->GetValueDouble(10);

				if (strATollColumnName == _T(""))
				{
					fixedLink.a		= 0.0;
				}
				else
				{
					fixedLink.a			= spResult->GetValueDouble(11);
				}

				if (strBTollColumnName == _T(""))
				{
					fixedLink.b		= 0.0;
				}
				else
				{
					fixedLink.b			= spResult->GetValueDouble(12);
				}

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if (iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}

	return false;
}

bool KAOBDefineVDFDlg::ExportDB2MulityLinkFileExcludType( KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation )
{
	if (m_oABMultiVDFInfo.TemEachFunctionType != KEMAssignmentBPRFunctionEachLink)
	{
		TxLogDebugStartMsg();

		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}
		

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id From link") ;

		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= 0.0;
				fixedLink.beta		= 0.0;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if (iterfind  != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}
	else
	{
		TxLogDebugStartMsg();


		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}


		CString strColumnName = _T("");
		bool    bCheckName	  = false;
		std::vector<CString> vecEachLinkColumnName = m_oABMultiVDFInfo.TvecEachLinkColumn;

		//int nCount = vecEachLinkColumnName.size();
		/*int nIndexCount = nCount -1;
		strColumnName = vecEachLinkColumnName[0];
		
		for (int i = 0; i < nIndexCount; i++)
		{
			int nIndex = i + 1;
			CString strEachLinkColumn = vecEachLinkColumnName[nIndex]; 
			strColumnName = strColumnName + _T(",") + strEachLinkColumn;
		}*/

		int nCount = (int)vecEachLinkColumnName.size();

		strColumnName = vecEachLinkColumnName[0];
		if (strColumnName == _T(""))
		{
			strColumnName = _T("0.0");
		}

		for (int i = 1; i < nCount; i ++)
		{
			CString strEachColumnName = vecEachLinkColumnName[i];
			if (strEachColumnName == _T(""))
			{
				strEachColumnName = _T("0.0");
			}

			strColumnName = strColumnName + _T(", ") + strEachColumnName;
		}

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;

		strSQL  = strSQL + _T(", ") + strColumnName;

		strSQL = strSQL + _T(" From link");

		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				Integer nxlinkid   = spResultSet->GetValueInt64(0);

				AutoType iterfind = a_setIncludeLink.find(nxlinkid);
				AutoType iterend  = a_setIncludeLink.end();
				if (iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &nxlinkid ), sizeof(Integer) );  
					Integer nxfnodeid  = spResultSet->GetValueInt64(1);
					ofs.write(reinterpret_cast<char*>( &nxfnodeid ), sizeof(Integer) );  
					Integer nxtnodeid  = spResultSet->GetValueInt64(2);
					ofs.write(reinterpret_cast<char*>( &nxtnodeid ), sizeof(Integer) );
					int		nlinktype	= spResultSet->GetValueInt (3);
					ofs.write(reinterpret_cast<char*>( &nlinktype ), sizeof(int) );
					double	dlength		= spResultSet->GetValueDouble(4);
					ofs.write(reinterpret_cast<char*>( &dlength ), sizeof(double));
					int		nlane		= spResultSet->GetValueInt  (5);
					ofs.write(reinterpret_cast<char*>( &nlane ), sizeof(int) );
					double	dcapa		= spResultSet->GetValueDouble(6);
					ofs.write(reinterpret_cast<char*>( &dcapa ), sizeof(double));
					double	dspeed		= spResultSet->GetValueDouble(7);
					ofs.write(reinterpret_cast<char*>( &dspeed ), sizeof(double));
					int	nfnNumber		= spResultSet->GetValueInt (8);
					ofs.write(reinterpret_cast<char*>( &nfnNumber ), sizeof(int) );

					int nIndex = 9;
					for (int i = 0; i < nCount; i ++ )
					{
						double dAB	    = spResultSet->GetValueDouble(nIndex);
						ofs.write(reinterpret_cast<char*>( &dAB ), sizeof(double));
						nIndex++;
					}
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false; 
		}
		
		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}

	return false;
}

void KAOBDefineVDFDlg::InitButtonControl( void )
{
	m_singleVDFDlg.InitButtonControl();
}


void KAOBDefineVDFDlg::OnBnClickedButtonDefault()
{
	int nModeNumInVDF = m_cboModeCntInVDF.GetItemData(m_cboModeCntInVDF.GetCurSel());
	if ( 1 == nModeNumInVDF )
	{
		m_singleVDFDlg.SetDefaultSetting();
	}
	else
	{

	}
}

void KAOBDefineVDFDlg::UsedDefault( bool a_bUseDefault )
{
	m_bUseDefault = a_bUseDefault;
}

bool KAOBDefineVDFDlg::MultiScenarioExportDB2LinkFileExcludeType( KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation, KTarget* a_pRunTarget )
{
	if (m_nModeInVDF < 2)
	{
		MultiScenarioExportDB2SingeLinkFileExcludeType(a_spDBaseConnection, a_setIncludeLink, strLocation, a_pRunTarget);
		return true;
	}
	else
	{
		MultiScenarioExportDB2MulityLinkFileExcludType(a_spDBaseConnection, a_setIncludeLink, strLocation, a_pRunTarget);
		return true;
	}
}

bool KAOBDefineVDFDlg::MultiScenarioExportDB2SingeLinkFileExcludeType( KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink, CString strLocation, KTarget* a_pRunTarget )
{
	if (m_oABSingeVDFInfo.TemFunctionType == KEMAssignmentBPRFunctionAllLink)
	{
		TxLogDebugStartMsg();

		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id From link") ;
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			int nIndex(0);
			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= m_oABSingeVDFInfo.TdAllAlpha;
				fixedLink.beta		= m_oABSingeVDFInfo.TdAllBeta;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if (iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
				}

				if (fixedLink.link_id == 286)
				{
					nIndex++;
				}


				nIndex++;
			}

			if (nIndex < 80000)
			{
				int a = 10;
			}

		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;

	}
	else if (m_oABSingeVDFInfo.TemFunctionType == KEMAssignmentBPRFunctionEachFn)
	{
		TxLogDebugStartMsg();

		bool bUseAlpha = true;
		bool bUseBeta  = true;
		bool bUseA     = true;
		bool bUseB     = true;

		//GenerateQuery;
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		CString strQuery, strAddQuery, strTemp;
		strQuery.Format(_T("SELECT alpha_column_name, beta_column_name, a_column_name, b_column_name FROM function_group WHERE function_group_id = %d"), 
			m_oABSingeVDFInfo.TnEachFnCombo);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("데이터가 없습니다."));

		//Alpha
		strTemp = spResultSet->GetValueString(0);

		if (strTemp.IsEmpty())
		{
			bUseAlpha = false;
			strTemp   = _T("''");
		}

		strAddQuery.Format(_T(", %s as Alpha"), strTemp);

		//Beta
		strTemp = spResultSet->GetValueString(1);

		if (strTemp.IsEmpty())
		{
			bUseBeta = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as Beta"), strTemp);

		//a
		strTemp = spResultSet->GetValueString(2);

		if (strTemp.IsEmpty())
		{
			bUseA = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as a"), strTemp);

		//b
		strTemp = spResultSet->GetValueString(3);

		if (strTemp.IsEmpty())
		{
			bUseB = false;
			strTemp   = _T("''");
		}

		strAddQuery.AppendFormat(_T(", %s as b"), strTemp);

		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		std::map<int, std::vector<double>>::iterator fIter, eIter = m_oABSingeVDFInfo.TmapEachFN.end();

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ");
		strSQL.Append(strAddQuery);
		strSQL.Append(_T(" From link"));
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= 0.0;
				fixedLink.beta		= 0.0;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				fIter = m_oABSingeVDFInfo.TmapEachFN.find(fixedLink.fuNumber);

				if (fIter == eIter)
					ThrowException(_T("정의되지 않은 functionID입니다."));

				std::vector<double> vecEachFn = fIter->second;

				if (bUseAlpha)
				{
					fixedLink.alpha	= spResultSet->GetValueDouble(9);
				}
				else
				{
					fixedLink.alpha = vecEachFn[0];
				}

				if (bUseBeta)
				{
					fixedLink.beta = spResultSet->GetValueDouble(10);
				}
				else
				{
					fixedLink.beta = vecEachFn[1];
				}

				if (bUseA)
				{
					fixedLink.a	= spResultSet->GetValueDouble(11);
				}
				else
				{
					fixedLink.a = vecEachFn[2];
				}

				if (bUseB)
				{
					fixedLink.b	= spResultSet->GetValueDouble(12);
				}
				else
				{
					fixedLink.b = vecEachFn[3];
				}

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if (iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}
	else
	{
		TxLogDebugStartMsg();

		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		KIOColumns* pIOColumns = a_pRunTarget->Tables()->FindTable(TABLE_LINK)->Columns();
		int         nColumns   = pIOColumns->ColumnCount();

		CString strAlphaColumnName = _T("");
		CString strBetaColumnName  = _T("");
		CString strATollColumnName = _T("");
		CString strBTollColumnName = _T("");

		//alpha
		for (int i = 0; i < nColumns; ++i)
		{
			if (m_oABSingeVDFInfo.TstrCaptionAlpha.CompareNoCase(pIOColumns->GetColumn(i)->Caption()) ==  0)
			{
				strAlphaColumnName = pIOColumns->GetColumn(i)->Name();
				break;
			}
		}

		//beta
		for (int i = 0; i < nColumns; ++i)
		{
			if (m_oABSingeVDFInfo.TstrCaptionBeta.CompareNoCase(pIOColumns->GetColumn(i)->Caption()) ==  0)
			{
				strBetaColumnName = pIOColumns->GetColumn(i)->Name();
				break;
			}
		}

		//a
		for (int i = 0; i < nColumns; ++i)
		{
			if (m_oABSingeVDFInfo.TstrCaptionA.CompareNoCase(pIOColumns->GetColumn(i)->Caption()) ==  0)
			{
				strATollColumnName = pIOColumns->GetColumn(i)->Name();
				break;
			}
		}

		//b
		for (int i = 0; i < nColumns; ++i)
		{
			if (m_oABSingeVDFInfo.TstrCaptionB.CompareNoCase(pIOColumns->GetColumn(i)->Caption()) ==  0)
			{
				strBTollColumnName = pIOColumns->GetColumn(i)->Name();
				break;
			}
		}

		if (strAlphaColumnName.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("'%s - %s'에 %s 관련 컬럼 정보가 존재하지 않습니다."), a_pRunTarget->CaptionScenario(), a_pRunTarget->CaptionTargetYear(), m_oABSingeVDFInfo.TstrCaptionAlpha);
			AfxMessageBox(strMsg);

			throw 1;
		}
		
        if (strBetaColumnName.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("'%s - %s'에 %s 관련 컬럼 정보가 존재하지 않습니다."), a_pRunTarget->CaptionScenario(), a_pRunTarget->CaptionTargetYear(), m_oABSingeVDFInfo.TstrCaptionBeta);
			AfxMessageBox(strMsg);

			throw 1;
		}
		
        /*
        if (strATollColumnName.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("%s_%s에 %s컬럼 정보가 없습니다."), a_pRunTarget->CaptionScenario(), a_pRunTarget->CaptionTargetYear(), m_oABSingeVDFInfo.TstrCaptionA);
			AfxMessageBox(strMsg);

			throw 1;
		}
		
        if (strBTollColumnName.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("%s_%s에 %s컬럼 정보가 없습니다."), a_pRunTarget->CaptionScenario(), a_pRunTarget->CaptionTargetYear(), m_oABSingeVDFInfo.TstrCaptionB);
			AfxMessageBox(strMsg);

			throw 1;
		}*/
		
		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;

		strSQL  = strSQL + _T(", ") + strAlphaColumnName + _T(", ") + strBetaColumnName;

		if (strATollColumnName != _T(""))
		{
			strSQL = strSQL + _T(", ") + strATollColumnName;
		}

		if (strBTollColumnName !=_T(""))
		{
			strSQL = strSQL + _T(", ") + strBTollColumnName;
		}

		strSQL = strSQL + _T(" From link");
		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResult->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResult->GetValueInt64 (0);
				fixedLink.fnode_id  = spResult->GetValueInt64 (1);
				fixedLink.tnode_id  = spResult->GetValueInt64 (2);
				fixedLink.link_type = spResult->GetValueInt   (3);
				fixedLink.length    = spResult->GetValueDouble(4);
				fixedLink.lane      = spResult->GetValueInt   (5);
				fixedLink.capa      = spResult->GetValueDouble(6);
				fixedLink.speed     = spResult->GetValueDouble(7);
				fixedLink.fuNumber	= spResult->GetValueInt   (8);
				fixedLink.alpha		= spResult->GetValueDouble(9);
				fixedLink.beta		= spResult->GetValueDouble(10);

				if (strATollColumnName == _T(""))
				{
					fixedLink.a		= 0.0;
				}
				else
				{
					fixedLink.a			= spResult->GetValueDouble(11);
				}

				if (strBTollColumnName == _T(""))
				{
					fixedLink.b		= 0.0;
				}
				else
				{
					fixedLink.b			= spResult->GetValueDouble(12);
				}

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if (iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}

	return false;
}

bool KAOBDefineVDFDlg::MultiScenarioExportDB2MulityLinkFileExcludType( KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeLink ,CString strLocation, KTarget* a_pRunTarget )
{
	if (m_oABMultiVDFInfo.TemEachFunctionType != KEMAssignmentBPRFunctionEachLink)
	{
		TxLogDebugStartMsg();

		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}
		

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id From link") ;

		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			TSingeFixedLink fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= 0.0;
				fixedLink.beta		= 0.0;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				AutoType iterfind = a_setIncludeLink.find(fixedLink.link_id);
				AutoType iterend  = a_setIncludeLink.end();

				if (iterfind  != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TSingeFixedLink) );  
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}
	else
	{
		TxLogDebugStartMsg();


		ofstream ofs(strLocation, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}


		CString strColumnName = _T("");
		bool    bCheckName	  = false;
		std::vector<CString> vecEachLinkColumnName = m_oABMultiVDFInfo.TvecEachLinkColumn;

		//int nCount = vecEachLinkColumnName.size();
		/*int nIndexCount = nCount -1;
		strColumnName = vecEachLinkColumnName[0];
		
		for (int i = 0; i < nIndexCount; i++)
		{
			int nIndex = i + 1;
			CString strEachLinkColumn = vecEachLinkColumnName[nIndex]; 
			strColumnName = strColumnName + _T(",") + strEachLinkColumn;
		}*/

		int nCount = (int)vecEachLinkColumnName.size();

		strColumnName = vecEachLinkColumnName[0];
		if (strColumnName == _T(""))
		{
			strColumnName = _T("0.0");
		}

		if (m_pTarget == a_pRunTarget)
		{
			for (int i = 1; i < nCount; i ++)
			{
				CString strEachColumnName = vecEachLinkColumnName[i];
				if (strEachColumnName == _T(""))
				{
					strEachColumnName = _T("0.0");
				}

				strColumnName = strColumnName + _T(", ") + strEachColumnName;
			}
		}
		else
		{
			CString     strCaption;

			KIOColumns* pOriginColumns = m_pTarget->Tables()->FindTable(TABLE_LINK)->Columns();
			int         nOriginColumns = pOriginColumns->ColumnCount();

			for (int i = 1; i < nCount; ++i)
			{
				CString strEachColumnName = vecEachLinkColumnName[i];

				if (strEachColumnName == _T(""))
				{
					strEachColumnName = _T("0.0");
				}
				else
				{
					//Name -> Caption
					strCaption = _T("");

					for (int j = 0; j < nOriginColumns; ++j)
					{
						if (strEachColumnName.CompareNoCase(pOriginColumns->GetColumn(j)->Name()) == 0)
						{
							strCaption = pOriginColumns->GetColumn(j)->Caption();
							break;
						}
					}

					if (strCaption.IsEmpty())
					{
						TxLogDebugException();
						//throw 1;
                        return false;
					}

					//Caption -> runTarget ColumnName
					strEachColumnName = _T("");

					KIOColumns* pRunColumns = a_pRunTarget->Tables()->FindTable(TABLE_LINK)->Columns();
					int         nRunColumns = pRunColumns->ColumnCount();

					for (int j = 0; j < nRunColumns; ++j)
					{
						if (strCaption.CompareNoCase(pRunColumns->GetColumn(j)->Caption()) == 0)
						{
							strEachColumnName = pRunColumns->GetColumn(j)->Name();
							break;
						}					
					}

					if (strEachColumnName.IsEmpty())
					{
						TxLogDebugException();
						//throw 1;
                        return false;
					}					
				}	

                strColumnName = strColumnName + _T(", ") + strEachColumnName;
			}
		}        	

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;

		strSQL  = strSQL + _T(", ") + strColumnName;

		strSQL = strSQL + _T(" From link");

		strSQL.Append(_T(" Order By link_id "));

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				Integer nxlinkid   = spResultSet->GetValueInt64(0);

				AutoType iterfind = a_setIncludeLink.find(nxlinkid);
				AutoType iterend  = a_setIncludeLink.end();
				if (iterfind != iterend)
				{
					ofs.write(reinterpret_cast<char*>( &nxlinkid ), sizeof(Integer) );  
					Integer nxfnodeid  = spResultSet->GetValueInt64(1);
					ofs.write(reinterpret_cast<char*>( &nxfnodeid ), sizeof(Integer) );  
					Integer nxtnodeid  = spResultSet->GetValueInt64(2);
					ofs.write(reinterpret_cast<char*>( &nxtnodeid ), sizeof(Integer) );
					int		nlinktype	= spResultSet->GetValueInt (3);
					ofs.write(reinterpret_cast<char*>( &nlinktype ), sizeof(int) );
					double	dlength		= spResultSet->GetValueDouble(4);
					ofs.write(reinterpret_cast<char*>( &dlength ), sizeof(double));
					int		nlane		= spResultSet->GetValueInt  (5);
					ofs.write(reinterpret_cast<char*>( &nlane ), sizeof(int) );
					double	dcapa		= spResultSet->GetValueDouble(6);
					ofs.write(reinterpret_cast<char*>( &dcapa ), sizeof(double));
					double	dspeed		= spResultSet->GetValueDouble(7);
					ofs.write(reinterpret_cast<char*>( &dspeed ), sizeof(double));
					int	nfnNumber		= spResultSet->GetValueInt (8);
					ofs.write(reinterpret_cast<char*>( &nfnNumber ), sizeof(int) );

					int nIndex = 9;
					for (int i = 0; i < nCount; i ++ )
					{
						double dAB	    = spResultSet->GetValueDouble(nIndex);
						ofs.write(reinterpret_cast<char*>( &dAB ), sizeof(double));
						nIndex++;
					}
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false; 
		}
		
		ofs.close();

		TxLogDebugEndMsg();
		return true;
	}

	return false;
}
