// FImportTableDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FImportTableDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "KUIDefImportTable.h"
#include "TableImport1Dlg.h"
#include "TableImport2Dlg.h"

#include "TableExportDlg.h"
#include "DemarcationImport.h"

using namespace UIImportTable;

// KFImportTableDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KFImportTableDlg, KDialogEx)

KFImportTableDlg::KFImportTableDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KFImportTableDlg::IDD, pParent),
	m_nIEMode(0)
{

}

KFImportTableDlg::~KFImportTableDlg()
{
}

void KFImportTableDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nIEMode);
	DDX_Control(pDX, IDC_STATIC_BACK, m_uiBackImage);
	DDX_Control(pDX, IDC_STATIC_ARROW1,m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2,m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3,m_uiArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW4,m_uiArrow4);
	DDX_Control(pDX, IDC_COMBO_SED, m_cboSED);
	DDX_Control(pDX, IDC_COMBO_OD, m_cboOD);
}


BEGIN_MESSAGE_MAP(KFImportTableDlg, KDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_NODE, &KFImportTableDlg::OnBnClickedNode)
	ON_BN_CLICKED(IDC_ZONE, &KFImportTableDlg::OnBnClickedZone)
	ON_BN_CLICKED(IDC_LINK, &KFImportTableDlg::OnBnClickedLink)
	ON_BN_CLICKED(IDC_OD, &KFImportTableDlg::OnBnClickedOd)
	ON_BN_CLICKED(IDC_SED, &KFImportTableDlg::OnBnClickedSed)
	ON_BN_CLICKED(IDCANCEL, &KFImportTableDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDVIEWLOG, &KFImportTableDlg::OnBnClickedViewlog)
	ON_BN_CLICKED(IDC_DEMAR, &KFImportTableDlg::OnBnClickedDemar)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO3, IDC_RADIO4, &KFImportTableDlg::OnBnClickedRadioModeChoiceStatus)
END_MESSAGE_MAP()

void KFImportTableDlg::SetTarget( KTarget* a_pTarget )
{
	m_pTarget = a_pTarget;
} 


BOOL KFImportTableDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);
	
	m_uiBackImage.MoveWindow(6, 5, 526, 183);    
	m_uiBackImage.SetMarkupText(UIXAML_I_BACKIMAGE);


	SetWindowTheme( GetDlgItem(IDC_RADIO1)->m_hWnd, _T(""), _T(""));
	SetWindowTheme( GetDlgItem(IDC_RADIO2)->m_hWnd, _T(""), _T("") );
	SetWindowTheme( GetDlgItem(IDC_STATIC1)->m_hWnd, _T(""), _T(""));
	SetWindowTheme( GetDlgItem(IDC_STATIC2)->m_hWnd, _T(""), _T("") );
	SetWindowTheme( GetDlgItem(IDC_RADIO3)->m_hWnd, _T(""), _T(""));
	SetWindowTheme( GetDlgItem(IDC_RADIO4)->m_hWnd, _T(""), _T("") );


	m_uiArrow1.SetPosition(ZEMArrowPositionLB, ZEMArrowPositionRC);
	m_uiArrow1.ArrowLineColor(RGB(0,0,0));
	m_uiArrow2.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB);
	m_uiArrow2.ArrowLineColor(RGB(0,0,0));
	m_uiArrow3.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);
	m_uiArrow3.ArrowLineColor(RGB(0,0,0));
	m_uiArrow4.SetPosition(ZEMArrowPositionLT, ZEMArrowPositionRB);
	m_uiArrow4.ArrowLineColor(RGB(0,0,0));
	
	SetODDataInCombo();
	// start - KOTI 요청에 의한 SED 제거 (2013.04.24)
	// SetSEDDataInCombo();
	GetDlgItem(IDC_STATIC_ARROW4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_SED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SED)->ShowWindow(SW_HIDE);
	// end - KOTI 요청에 의한 SED 제거 

	CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO3);
	UpdateDisplayView();

	return TRUE;  
}


HBRUSH KFImportTableDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.       
	UINT ID = pWnd->GetDlgCtrlID();
	if( ID == IDC_RADIO1 || ID == IDC_RADIO2 || ID == IDC_STATIC1 || ID == IDC_STATIC2 || ID == IDC_RADIO3 || ID == IDC_RADIO4 )
	{        
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}



void KFImportTableDlg::SetSEDDataInCombo()
{
	m_cboSED.ResetContent();

	std::vector<CString> vecSEDName;
	KIOTables*   pIOTables = m_pTarget->Tables();

	KIOTable* pSEDTable = pIOTables->FindTable(TABLE_SED_VECTOR);
	CString strSED = pSEDTable->Caption();
	vecSEDName.push_back(strSED);

	int nCount = vecSEDName.size();

	m_cboSED.AddString(_T("-- Select --"));
	for (int i= 0; i<nCount; i++)
	{
		CString strTableName = vecSEDName[i];
		m_cboSED.AddString(strTableName);
	}

	m_cboSED.SetCurSel(0);
}

void KFImportTableDlg::SetODDataInCombo()
{
	m_cboOD.ResetContent();

	std::vector<CString> vecODName;
	KIOTables*   pIOTables = m_pTarget->Tables();

	KIOTable* pPurposeTable = pIOTables->FindTable(TABLE_PURPOSE_OD);
	CString strPurpose = pPurposeTable->Caption();
	vecODName.push_back(strPurpose);
	
	/* jyk : koti 요청으로 품목 P/C만 보여 주도록
	KIOTable* pParameterTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
	CString strParameter = pParameterTable->Caption();
	vecODName.push_back(strParameter);

	KIOTable* pModeTable = pIOTables->FindTable(TABLE_MODE_OD);
	CString strMode = pModeTable->Caption();
	vecODName.push_back(strMode);

	KIOTable* pVehicleModeTable = pIOTables->FindTable(TABLE_VEHICLE_MODE_OD);
	CString strVehicle = pVehicleModeTable->Caption();
	vecODName.push_back(strVehicle);

	KIOTable* pALoadTable = pIOTables->FindTable(TABLE_AVERAGE_LOADAGE_OD);
	CString strALoad = pALoadTable->Caption();
	vecODName.push_back(strALoad);

	KIOTable* pChainTable = pIOTables->FindTable(TABLE_CHAIN_OD);
	CString strChain = pChainTable->Caption();
	vecODName.push_back(strChain);

	KIOTable* pEVehicleTable = pIOTables->FindTable(TABLE_EMPTY_VEHICLE_OD);
	CString strEVehicle = pEVehicleTable->Caption();
	vecODName.push_back(strEVehicle);

	KIOTable* pGTransportedTable = pIOTables->FindTable(TABLE_GOODS_TRANSPORTED_OD);
	CString strGTransported = pGTransportedTable->Caption();
	vecODName.push_back(strGTransported);

	KIOTable* pIRatioTable = pIOTables->FindTable(TABLE_INPUT_RATIO_OD);
	CString strIRatio = pIRatioTable->Caption();
	vecODName.push_back(strIRatio);

	KIOTable* pRRegion = pIOTables->FindTable(TABLE_RATIO_REGION);
	CString strRRegion = pRRegion->Caption();
	vecODName.push_back(strRRegion);
	*/
	int nCount = vecODName.size();

	m_cboOD.AddString(_T("-- Select --"));

	for (int i=0; i<nCount; i++)
	{
		CString strTableName = vecODName[i];
		m_cboOD.AddString(strTableName);
	}

	m_cboOD.SetCurSel(0);

}


bool KFImportTableDlg::ImportNomalTable( CString a_strTableName )
{
	bool bImport = false;
	bool bBackStep = false;
	TCHAR szCurrentDirectory[MAX_PATH] = {0};
	::GetCurrentDirectory( MAX_PATH, szCurrentDirectory );

	int     nSeparator   = 0;
	int     nCSVEncoding = 0;
	CString strFile      = szCurrentDirectory;
	BOOL    bContainColumn(FALSE);

	KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTable*          pTable        = pIOTables->FindTable(a_strTableName);

	bool bContinue = true;
	while( true == bContinue )
	{
		KTableImport1Dlg dlg1;
		dlg1.SetTarget(m_pTarget);

		if (bBackStep)
		{
			dlg1.m_strFile		  = strFile;
			dlg1.m_nSeparator	  = nSeparator;
			dlg1.m_nEncoding	  = nCSVEncoding;
			dlg1.m_bContainColumn = bContainColumn;

			bBackStep = false;
		}

		if( dlg1.DoModal() == IDCANCEL )
		{
			return false;
		}
		strFile = dlg1.m_strFile;
		nSeparator = dlg1.m_nSeparator;

		KTableImport2Dlg dlg2;
		dlg2.SetFile( strFile );

		TCHAR tcSeparator;
		switch( nSeparator )
		{
		case 0 : tcSeparator = _T(','); break;
		case 1 : tcSeparator = _T(';'); break;
		case 2 : tcSeparator = _T('|'); break;
		case 3 : tcSeparator = _T('	'); break;
		case 4 : tcSeparator = _T(' '); break;
		default : ASSERT( FALSE ); break;
		}
		nCSVEncoding = dlg1.m_nEncoding;
		KEMCSVEncoding emCSVEncoding;
		switch( nCSVEncoding )
		{
		case 0 : emCSVEncoding = KEMCSVEncodingANSI; break;
		case 1 : emCSVEncoding = KEMCSVEncodingUTF8; break;
		case 2 : emCSVEncoding = KEMCSVEncodingUTF16LE; break;
		default : ASSERT( FALSE ); break;
		}

		dlg2.SetSeparator( tcSeparator );
		bContainColumn = dlg1.m_bContainColumn;
		dlg2.SetContainsColumn( bContainColumn == TRUE ? true : false );

		dlg2.SetTable(pTable);
		dlg2.SetCSVEncoding(emCSVEncoding);
		dlg2.SetTarget(m_pTarget);

		if( dlg2.DoModal() == IDCANCEL )
		{
			return false;
		}

		if(dlg2.GetStatus() == 2)
		{
			bBackStep = true;
		}
		else
		{
			bContinue = false;
			bImport = dlg2.m_bResult;
		}
	}

	if( true == bImport)
	{
		pTable->Notify();
	}

	return bImport;

}


void KFImportTableDlg::OnBnClickedNode()
{
	UpdateData(TRUE);
	if (m_nIEMode == 0)
	{
		ImportNomalTable(TABLE_NODE);
	}
	else
	{
		ExportTable(TABLE_NODE);
	}
	KRedrawWindow();
}


void KFImportTableDlg::OnBnClickedZone()
{
	UpdateData(TRUE);
	if (m_nIEMode == 0)
	{
		ImportNomalTable(TABLE_ZONE);
	}
	else
	{
		ExportTable(TABLE_ZONE);
	}
	KRedrawWindow();
}


void KFImportTableDlg::OnBnClickedLink()
{
	UpdateData(TRUE);
	if (m_nIEMode == 0)
	{
		ImportNomalTable(TABLE_LINK);
	}
	else
	{
		ExportTable(TABLE_LINK);
	}
}


void KFImportTableDlg::OnBnClickedOd()
{
	UpdateData(TRUE);
	CString strODCombo = _T("");
	CString strODTable = _T("");

	int nCurSel = m_cboOD.GetCurSel();

	if (nCurSel == 0)
	{
		AfxMessageBox(_T("선택된 OD 테이블이 없습니다."));
		return;
	}

	m_cboOD.GetLBText(nCurSel,strODCombo);

	KIOTables*   pIOTables = m_pTarget->Tables();

	int nTableCount = pIOTables->GetCount();
	for(int i=0; i<nTableCount; ++i)
	{
		KIOTable* pTable       = pIOTables->GetTableAt(i);
		if (pTable->Caption() == strODCombo)
		{
			strODTable = pTable->Name();
		}
	}

	if (strODTable == _T(""))
	{
		AfxMessageBox(_T("선택된 OD 테이블이 없습니다."));
		return;
	}

	if (m_nIEMode == 0)
	{
		ImportNomalTable(strODTable);
	}
	else
	{
		ExportTable(strODTable);
	}

	m_cboOD.SetCurSel(0);
	KRedrawWindow();
}


void KFImportTableDlg::OnBnClickedSed()
{
	UpdateData(TRUE);

	CString strSEDCombo = _T("");
	CString strSEDTable = _T("");

	int nSelectCur = m_cboSED.GetCurSel();

	if (nSelectCur == 0 )
	{
		AfxMessageBox(_T("선택된 SED 테이블이 없습니다."));
		return;
	}

	m_cboOD.GetLBText(nSelectCur,strSEDCombo);

	KIOTables*   pIOTables = m_pTarget->Tables();

	int nTableCount = pIOTables->GetCount();
	for(int i=0; i<nTableCount; ++i)
	{
		KIOTable* pTable       = pIOTables->GetTableAt(i);
		if (pTable->Caption() == strSEDCombo)
		{
			strSEDTable = pTable->Name();
		}
	}

	if (strSEDTable == _T(""))
	{
		AfxMessageBox(_T("선택된 SED 테이블이 없습니다."));
		return;
	}

	if (m_nIEMode == 0)
	{
		ImportNomalTable(strSEDTable);
	}
	else
	{
		ExportTable(strSEDTable);
	}

	m_cboSED.SetCurSel(0);
	KRedrawWindow();
}


void KFImportTableDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}


void KFImportTableDlg::OnBnClickedViewlog()
{
	CString strTargetLocation = m_pTarget->GetLocation();
	CString strErrFileName    = strTargetLocation + _T("\\") + IMPORT_ERROR_FILE_NAME;

	CFileFind   cFileFinder;
	if(FALSE == cFileFinder.FindFile(strErrFileName) )
	{
		CString strMsg(_T(""));
		strMsg.Format(_T("%s 파일이 존재 하지 않습니다."), IMPORT_ERROR_FILE_NAME);
		AfxMessageBox(strMsg);
		return;
	}

	ShellExecute(NULL, _T("open"), strErrFileName, NULL, NULL, SW_SHOW); 
}

void KFImportTableDlg::KRedrawWindow()
{
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}

bool KFImportTableDlg::ExportTable( CString a_strTableName )
{
	KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTable*          pTable        = pIOTables->FindTable(a_strTableName);

	KTableExportDlg dlg;
	dlg.SetTable(pTable);
	dlg.SetTarget(m_pTarget);

	if( dlg.DoModal() == IDOK )
	{
		return true;
	}
	return false;
}


void KFImportTableDlg::OnBnClickedDemar()
{
	try
	{
		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
		if (pTable->RecordCount() < 1)
		{
			CString strMsg;
			strMsg.Format(_T("%s 데이터가 존재하지 않습니다. 존 생성 후 실행하세요"), pTable->Caption());
			AfxMessageBox(strMsg);
			return;
		}

		KDemarcationImport dlg(m_pTarget);
		dlg.DoModal();
	}
	catch (KExceptionPtr ex)
	{
		Log4cxxExceptionPrint(ex);
	}
	catch (...)
	{
		KLogDebugException();
	}    
}


void KFImportTableDlg::OnBnClickedRadioModeChoiceStatus(UINT nID)
{
	UpdateDisplayView();
}


void KFImportTableDlg::UpdateDisplayView()
{
	BOOL bEnable(TRUE);
	int  nShowID(SW_SHOW);

	GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4);

	if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
	{
		bEnable = FALSE;
		nShowID = SW_HIDE;
	}
	else
	{
		bEnable = TRUE;
		nShowID = SW_SHOW;
	}

	m_uiArrow3.ShowWindow(nShowID);
	//GetDlgItem(IDC_STATIC1)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_OD)->EnableWindow(bEnable);
	GetDlgItem(IDC_OD)->EnableWindow(bEnable);
}