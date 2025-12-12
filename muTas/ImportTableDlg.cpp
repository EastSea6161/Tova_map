// ImportTableDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImportTableDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "KUIDefImportTable.h"

#include "DlgImportNode.h"
#include "DlgImportLink.h"
#include "DlgImportZone.h"
#include "DlgImportAccessStopNode.h"
#include "DlgImportTerminalSchedule.h"
#include "DlgImportTurn.h"
#include "DlgImportTransit.h"
#include "DlgImportTransitLine.h"
#include "DlgImportTransitLineNode.h"
#include "DlgImportPod.h"
#include "DlgImportMode.h"
#include "DlgImportParaOD.h"
#include "DlgImportSedVector.h"
#include "DlgImportSedPop.h"

#include "DlgShapeDemarcation.h"
#include "DlgExportTable.h"
#include "DlgExportTurnTable.h"
#include "ShapeExportDlg.h"

using namespace UIImportTable;

#if WINVER >= 0x0500
#include <uxtheme.h>
#pragma comment (lib, "UxTheme.lib")
#endif

//const CString IMPORT_ERROR_FILE_NAME = _T("importError.txt");
// KImportTableDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KImportTableDlg, KWhiteDlgEx)

KImportTableDlg::KImportTableDlg(CWnd* pParent /*=NULL*/)
	: KWhiteDlgEx(KImportTableDlg::IDD, pParent)
	, m_bIsImport(true)
{

}

KImportTableDlg::~KImportTableDlg()
{
}

void KImportTableDlg::DoDataExchange(CDataExchange* pDX)
{
	KWhiteDlgEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NODE, m_btnNode);
	DDX_Control(pDX, IDC_ZONE, m_btnZone);
	DDX_Control(pDX, IDC_SED, m_btnSED);
	DDX_Control(pDX, IDC_OD, m_btnOD);
	DDX_Control(pDX, IDC_LINK, m_btnLink);
	DDX_Control(pDX, IDC_TURN, m_btnTurn);
	DDX_Control(pDX, IDC_TRANSITINFO, m_btnTransitInfo);
	DDX_Control(pDX, IDC_TRANSITLINE, m_btnTransitLine);
	DDX_Control(pDX, IDC_SCHEDULE, m_btnSchedule);

	DDX_Control(pDX, IDC_STATIC_ARROW1,m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2,m_uiArrow2);

	DDX_Control(pDX, IDC_COMBO_SED, m_cboSED);
	DDX_Control(pDX, IDC_COMBO_OD, m_cboOD);
}


BEGIN_MESSAGE_MAP(KImportTableDlg, KWhiteDlgEx)
	//ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_NODE, &KImportTableDlg::OnBnClickedNode)
	ON_BN_CLICKED(IDC_ZONE, &KImportTableDlg::OnBnClickedZone)
	ON_BN_CLICKED(IDC_LINK, &KImportTableDlg::OnBnClickedLink)
	ON_BN_CLICKED(IDC_TURN, &KImportTableDlg::OnBnClickedTurn)
	ON_BN_CLICKED(IDC_TRANSITINFO, &KImportTableDlg::OnBnClickedTransitinfo)
	ON_BN_CLICKED(IDC_TRANSITLINE, &KImportTableDlg::OnBnClickedTransitline)
	ON_BN_CLICKED(IDC_SED, &KImportTableDlg::OnBnClickedSed)
	ON_BN_CLICKED(IDC_OD, &KImportTableDlg::OnBnClickedOd)
	ON_BN_CLICKED(IDC_INTERSECTION, &KImportTableDlg::OnBnClickedIntersection)
	ON_BN_CLICKED(IDCANCEL, &KImportTableDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDVIEWLOG, &KImportTableDlg::OnBnClickedViewlog)
    ON_BN_CLICKED(IDC_BUTTON_Demarc, &KImportTableDlg::OnBnClickedButtonDemarc)
	ON_BN_CLICKED(IDC_SCHEDULE, &KImportTableDlg::OnBnClickedSchedule)
	ON_BN_CLICKED(IDC_ACCESSSTOPNODE, &KImportTableDlg::OnBnClickedAccessStopNode)
	
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KImportTableDlg::OnBnClickedRadio)
END_MESSAGE_MAP()



void KImportTableDlg::SetInitValue( KTarget* a_pTarget, int nInitFlg )
{
	m_pTarget   = a_pTarget;
    m_nInitFlag = nInitFlg;
}


BOOL KImportTableDlg::OnInitDialog()
{
	KWhiteDlgEx::OnInitDialog();
	KWhiteDlgEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
    SetBackgroundColor(RGB(255,255,255));

#ifndef _UNICODE
	SetWindowTheme( GetDlgItem(IDC_RADIO1)->m_hWnd, L"", L"");
	SetWindowTheme( GetDlgItem(IDC_RADIO2)->m_hWnd, L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_STATIC1)->m_hWnd, L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_STATIC2)->m_hWnd, L"", L"" );
#else
	SetWindowTheme( GetDlgItem(IDC_RADIO1)->m_hWnd, _T(""), _T(""));
	SetWindowTheme( GetDlgItem(IDC_RADIO2)->m_hWnd, _T(""), _T("") );
	SetWindowTheme( GetDlgItem(IDC_STATIC1)->m_hWnd, _T(""), _T(""));
	SetWindowTheme( GetDlgItem(IDC_STATIC2)->m_hWnd, _T(""), _T("") );
#endif
    	
	m_uiArrow1.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);
	m_uiArrow1.ArrowLineColor(RGB(255,0,0));
	m_uiArrow2.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);
	m_uiArrow2.ArrowLineColor(RGB(255,0,0));

	SetODDataInCombo();
	SetSEDDataInCombo();
	
    if (m_nInitFlag == 0) {
	    CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
    }
    else {
        CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
    }

	KRedrawWindow();
	UpdateBtnImportState();

    CString strName(_T("")); {
        strName = m_pTarget->Caption();
        SetDlgItemText(IDC_STATIC_NAME, strName);
    }

	return TRUE;  
}

HBRUSH KImportTableDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	HBRUSH hbr = KWhiteDlgEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.       
	UINT ID = pWnd->GetDlgCtrlID();
	if( ID == IDC_RADIO1 || ID == IDC_RADIO2 || ID == IDC_STATIC1 || ID == IDC_STATIC2 )
	{        
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void KImportTableDlg::KRedrawWindow()
{
	//RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}

void KImportTableDlg::InitButtonControl()
{
	m_btnNode.EnableMarkup(TRUE);
	m_btnNode.SetWindowTextW(UIXAML_I_NODE_DEFAULT);

	m_btnLink.EnableMarkup(TRUE);
	m_btnLink.SetWindowTextW(UIXAML_I_LINK_DEFAULT);

	m_btnZone.EnableMarkup(TRUE);
	m_btnZone.SetWindowTextW(UIXAML_I_ZONE_DEFAULT);

	m_btnSED.EnableMarkup(TRUE);
	m_btnSED.SetWindowTextW(UIXAML_I_SED_DEFAULT);

	m_btnOD.EnableMarkup(TRUE);
	m_btnOD.SetWindowTextW(UIXAML_I_OD_DEFAULT);

	m_btnTurn.EnableMarkup(TRUE);
	m_btnTurn.SetWindowTextW(UIXAML_I_TURN_DEFAULT);

	m_btnTransitInfo.EnableMarkup(TRUE);
	m_btnTransitInfo.SetWindowTextW(UIXAML_I_TRINFO_DEFAULT);

	m_btnTransitLine.EnableMarkup(TRUE);
	m_btnTransitLine.SetWindowTextW(UIXAML_I_TRLINE_DEFAULT);
}


bool KImportTableDlg::ImportNomalTable( CString a_strTableName )
{
	KIOTable* pIOTable = nullptr; {
		KIOTables* pIOTables = m_pTarget->Tables();
		if (pIOTables == nullptr) {
			//AfxMessageBox(_T("관련 테이블 정보를 찾을 수 없습니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("관련 테이블 정보를 찾을 수 없습니다."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				AfxMessageBox(_T("CANNOT find the table."));
			}
			return false;
		}

		pIOTable = pIOTables->FindTable(a_strTableName);
	}

	if (nullptr == pIOTable) {
		//AfxMessageBox(_T("관련 테이블 정보를 찾을 수 없습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("관련 테이블 정보를 찾을 수 없습니다."));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("CANNOT find the table."));
		}
		return false;
	}

	if (a_strTableName.CompareNoCase(TABLE_NODE) == 0) {
		KDlgImportNode oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_LINK) == 0) {
		KDlgImportLink oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_ZONE) == 0) {
		KDlgImportZone oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
 	} else if (a_strTableName.CompareNoCase(TABLE_ACCESS_STOP_NODE) == 0) {
		KDlgImportAccessStopNode oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_TERMINAL_SCHEDULE) == 0) {
		KDlgImportTerminalSchedule oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_TRANSIT) == 0) {
		KDlgImportTransit oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_TRANSITLINKS) == 0) {
		KDlgImportTransitLine oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_TRANSITNODES) == 0) {
		KDlgImportTransitLineNode oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_TURN) == 0) {
		KDlgImportTurn oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0) {
		KDlgImportPod oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_MODE_OD) == 0) {
		KDlgImportMode oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_PARAMETER_OD) == 0) {
		KDlgImportParaOD oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_SED_VECTOR) == 0) {
		KDlgImportSedVector oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (a_strTableName.CompareNoCase(TABLE_SED_POP) == 0) {
		KDlgImportSedPop oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	}

	// 결과 실패일 경우 DoModal은 OK 가 되지 않기때문에, Cancel을 클릭 할 수 밖에 없다. 즉, return false; 가 실행 될 수 밖에 없다.
	return true;
}


void KImportTableDlg::OnBnClickedNode()
{
	//AfxMessageBox(_T("노드 데이터를 삽입하면 모든 데이터가 삭제됩니다."));
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		if (ImportNomalTable(TABLE_NODE) == true)
		{
			m_btnTurn.EnableWindow(FALSE);

			m_btnTransitInfo.EnableWindow(FALSE);

			m_btnTransitLine.EnableWindow(FALSE);
		}
		else
		{
			//InitButtonControl();
		}
	}
	else
	{
		ExportTable(TABLE_NODE);
	}

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedZone()
{
	UpdateData(TRUE);

	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		if (ImportNomalTable(TABLE_ZONE) == true)
		{
			m_btnSED.EnableWindow(TRUE);
			m_btnOD.EnableWindow(TRUE);

		}
	}
	else
	{
		ExportTable(TABLE_ZONE);
	}

	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedLink()
{
	UpdateData(TRUE);
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		if (ImportNomalTable(TABLE_LINK) == true)
		{
			m_btnTurn.EnableWindow(TRUE);
			m_btnTransitInfo.EnableWindow(TRUE);
			m_btnTransitLine.EnableWindow(FALSE);
		}
	}
	else
	{
		ExportTable(TABLE_LINK);
	}

	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedTurn()
{
	UpdateData(TRUE);
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		ImportNomalTable(TABLE_TURN);
	}
	else
	{
		ExportTable(TABLE_TURN);
	}
	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedTransitinfo()
{
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		if (ImportNomalTable(TABLE_TRANSIT) == true)
		{
			m_btnTransitLine.EnableWindow(TRUE);
		}
	}
	else
	{
		ExportTable(TABLE_TRANSIT);
	}
	
	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedTransitline()
{
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		ImportNomalTable(TABLE_TRANSITNODES);
	}
	else
	{
		ExportTable(TABLE_TRANSITNODES);
	}
	
	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedSed()
{
	UpdateData(TRUE);
	CString strSEDCombo = _T("");
	CString strSEDTable = _T("");

	int nSelectCur = m_cboSED.GetCurSel();

	if (nSelectCur == 0 )
	{
		//AfxMessageBox(_T("선택된 사회경제지표 테이블이 없습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("선택된 사회경제지표 테이블이 없습니다."));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("No SED table selected."));
		}
		return;
	}

	m_cboSED.GetLBText(nSelectCur,strSEDCombo);

	KIOTables*   pIOTables = m_pTarget->Tables();
	
	int nTableCount = pIOTables->GetCount();
	for(int i=0; i<nTableCount; ++i)
	{
		KIOTable* pTable       = pIOTables->GetTableAt(i);
		if (pTable->Caption() == strSEDCombo)
		{
			strSEDTable = pTable->Name();

			if (!m_bIsImport && pTable->RecordCount() == 0)
			{
				//AfxMessageBox(_T("선택된 사회경제지표 테이블에 데이터가 존재하지 않습니다."));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					AfxMessageBox(_T("선택된 사회경제지표 테이블에 데이터가 존재하지 않습니다."));
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					AfxMessageBox(_T("No data exists in the selected SED table."));
				}
				return;
			}
		}
	}

	if (strSEDTable == _T(""))
	{
		//AfxMessageBox(_T("선택된 사회경제지표 테이블이 없습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("선택된 사회경제지표 테이블이 없습니다."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("No SED table selected."));
		}
		return;
	}

	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		ImportNomalTable(strSEDTable);
	}
	else
	{
		ExportTable(strSEDTable);
	}
	

	m_cboSED.SetCurSel(0);
	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedOd()
{
	UpdateData(TRUE);
	CString strODCombo = _T("");
	CString strODTable = _T("");

	int nCurSel = m_cboOD.GetCurSel();

	if (nCurSel == 0)
	{
		//AfxMessageBox(_T("선택된 기종점 통행 데이터 테이블이 없습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("선택된 기종점 통행 데이터 테이블이 없습니다."));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("No OD table selected."));
		}
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

			//export할 OD 데이터가 있는지 확인
			if (!m_bIsImport && pTable->RecordCount() == 0)
			{
				//AfxMessageBox(_T("선택된 기종점 통행 데이터 테이블에 데이터가 존재하지 않습니다."));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					AfxMessageBox(_T("선택된 기종점 통행 데이터 테이블가 존재하지 않습니다."));
				}
				else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
					AfxMessageBox(_T("No data exists in the selected OD table."));
				}
				return;
			}
			
		}
	}

	if (strODTable == _T(""))
	{
		//AfxMessageBox(_T("선택된 기종점 통행 데이터 테이블이 없습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("선택된 기종점 통행 데이터 테이블이 없습니다."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("No OD table selected."));
		}
		return;
	}

	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		ImportNomalTable(strODTable);
	}
	else
	{
		ExportTable(strODTable);
	}

	m_cboOD.SetCurSel(0);
	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedIntersection()
{
	if (ImportNomalTable(TABLE_INTERSECTION) == true)
	{
		//m_btnTransitLine.SetWindowTextW(UIXAML_I_TRLINE_SELECTED);
	}
	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedSchedule()
{
	UpdateData(TRUE);
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		ImportNomalTable(TABLE_TERMINAL_SCHEDULE);
	}
	else
	{
		ExportTable(TABLE_TERMINAL_SCHEDULE);
	}
	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::OnBnClickedAccessStopNode()
{
	UpdateData(TRUE);
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		ImportNomalTable(TABLE_ACCESS_STOP_NODE);
	}
	else
	{
		ExportTable(TABLE_ACCESS_STOP_NODE);
	}
	KRedrawWindow();

	UpdateBtnImportState();
}


void KImportTableDlg::SetSEDDataInCombo()
{
	m_cboSED.ResetContent();
	try
	{
		std::vector<CString> vecSEDName;
		KIOTables*   pIOTables = m_pTarget->Tables();

		if (KmzSystem::IsPassengerSystem())
		{
			KIOTable* pSEDTable = pIOTables->FindTable(TABLE_SED_VECTOR);
			CString strSED = pSEDTable->Caption();
			vecSEDName.push_back(strSED);

			KIOTable* pPopTable = pIOTables->FindTable(TABLE_SED_POP);
			CString strPop = pPopTable->Caption();
			vecSEDName.push_back(strPop);
		}
		else
		{
			KIOTable* pSEDTable = pIOTables->FindTable(TABLE_SED_VECTOR);
			CString strSED = pSEDTable->Caption();
			vecSEDName.push_back(strSED);
		}

		int nCount = vecSEDName.size();

		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_cboSED.AddString(_T("----선택----"));
		}
		else {
			m_cboSED.AddString(_T("---Select---"));
		}

		for (int i=0; i<nCount; i++)
		{
			CString strTableName = vecSEDName[i];
			m_cboSED.AddString(strTableName);
		}

		if(nCount > 0)
		{
			m_cboSED.SetCurSel(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KImportTableDlg::SetODDataInCombo()
{
	
	m_cboOD.ResetContent();

	try
	{
		std::vector<CString> vecODName;
		KIOTables*   pIOTables = m_pTarget->Tables();

		if (KmzSystem::IsPassengerSystem())
		{
			KIOTable* pPurposeTable = pIOTables->FindTable(TABLE_PURPOSE_OD);
			CString strPurpose = pPurposeTable->Caption();
			vecODName.push_back(strPurpose);

			KIOTable* pParameterTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
			CString strParameter = pParameterTable->Caption();
			vecODName.push_back(strParameter);

			KIOTable* pModeTable = pIOTables->FindTable(TABLE_MODE_OD);
			CString strMode = pModeTable->Caption();
			vecODName.push_back(strMode);

		}
		else
		{
			KIOTable* pPurposeTable = pIOTables->FindTable(TABLE_PURPOSE_OD);
			CString strPurpose = pPurposeTable->Caption();
			vecODName.push_back(strPurpose);

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
		}

		int nCount = vecODName.size();

		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_cboOD.AddString(_T("----선택----"));
		}
		else {
			m_cboOD.AddString(_T("---Select---"));
		}

		for (int i=0; i<nCount; i++)
		{
			CString strTableName = vecODName[i];
			m_cboOD.AddString(strTableName);
		}

		if(nCount > 0)
		{
			m_cboOD.SetCurSel(0);
		}
		
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KImportTableDlg::OnBnClickedCancel()
{
	KWhiteDlgEx::OnCancel();
}


void KImportTableDlg::OnBnClickedViewlog()
{
	CString strTargetLocation = m_pTarget->GetLocation();
	CString strErrFileName    = strTargetLocation + _T("\\") + IMPORT_ERROR_FILE_NAME;

	CFileFind   cFileFinder;
	if(FALSE == cFileFinder.FindFile(strErrFileName) )
	{
		CString strMsg(_T(""));
		//strMsg.Format(_T("%s 파일이 존재하지 않습니다."), IMPORT_ERROR_FILE_NAME);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strMsg.Format(_T("%s 파일이 존재하지 않습니다."), IMPORT_ERROR_FILE_NAME);
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			strMsg.Format(_T("File %s does not exist."), IMPORT_ERROR_FILE_NAME);
		}
		AfxMessageBox(strMsg);
		return;
	}

	ShellExecute(NULL, _T("open"), strErrFileName, NULL, NULL, SW_SHOW); 
}

bool KImportTableDlg::ExportTable( CString a_strTableName )
{
	KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTable*    pTable    = pIOTables->FindTable(a_strTableName);

	if (a_strTableName.CompareNoCase(TABLE_TURN) == 0) {
		KDlgExportTurnTable oDlg(m_pTarget, pTable);
		if( oDlg.DoModal() == IDOK ) {
			return true;
		}
	} else {
		KDlgExportTable oDlg(m_pTarget, pTable);
		if( oDlg.DoModal() == IDOK ) {
			return true;
		}
	}

	return false;
}


void KImportTableDlg::OnBnClickedButtonDemarc()
{
    try
    {
        KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
        if (pTable->RecordCount() < 1)
        {
            CString strMsg;
            //strMsg.Format(_T("%s 관련 정보가 존재하지 않습니다.\n존 노드 데이터 입력 후 실행해 주세요."), pTable->Caption());
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strMsg.Format(_T("%s 관련 정보가 존재하지 않습니다.\n존 노드 데이터 입력 후 실행해 주세요."), pTable->Caption());
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				strMsg.Format(_T("No any information about %s.\nPlease import zone node data first."), pTable->Caption());
			}
            AfxMessageBox(strMsg);
            return;
        }

		//2020.03.08 - Han He : Export 선택 시 존 경계 버튼 클릭하면 Import 창이 팝업되는 오류 수정
		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			KDlgShapeDemarcation dlg(m_pTarget, pTable);
			dlg.DoModal();
		}
		else
		{
			KShapeExportDlg dlg(m_pTarget, pTable);
			dlg.DoModal();
		}
		//KDlgShapeDemarcation dlg(m_pTarget, pTable);
		//dlg.DoModal();
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

void KImportTableDlg::UpdateBtnImportState( void )
{
	if (m_bIsImport)
	{
		UpdateBtnNode();
		KRedrawWindow();
	}	
}

void KImportTableDlg::UpdateBtnNode( void )
{
	//Node는 무조건 활성화
	GetDlgItem(IDC_NODE)->EnableWindow(TRUE);

	KIOTables* pIOTables  = m_pTarget->Tables();
	KIOTable*  pNodeTable = pIOTables->FindTable(TABLE_NODE);
	BOOL       bEnable;

	if (pNodeTable->RecordCount() != 0)
	{
		GetDlgItem(IDC_SCHEDULE)->EnableWindow(TRUE);
		GetDlgItem(IDC_ACCESSSTOPNODE)->EnableWindow(TRUE);

		bEnable = TRUE;
	}
	else
	{
		GetDlgItem(IDC_SCHEDULE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ACCESSSTOPNODE)->EnableWindow(FALSE);

		bEnable = FALSE;
	}

	UpdateBtnLink(bEnable);
	UpdateBtnZone(bEnable);
}

void KImportTableDlg::UpdateBtnLink( BOOL a_bEnable )
{
	GetDlgItem(IDC_LINK)->EnableWindow(a_bEnable);

	if (a_bEnable)
	{
		KIOTables* pIOTables  = m_pTarget->Tables();
		KIOTable*  pLinkTable = pIOTables->FindTable(TABLE_LINK);

		if (pLinkTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_TURN)->EnableWindow(a_bEnable);
			UpdateBtnTrasitInfo(a_bEnable);
		}
	}
	else
	{
		GetDlgItem(IDC_TURN)->EnableWindow(a_bEnable);
		UpdateBtnTrasitInfo(a_bEnable);
	}
}

void KImportTableDlg::UpdateBtnTrasitInfo( BOOL a_bEnable )
{
	GetDlgItem(IDC_TRANSITINFO)->EnableWindow(a_bEnable);

	if (a_bEnable)
	{
		KIOTables* pIOTables  = m_pTarget->Tables();
		KIOTable*  pTransitTable = pIOTables->FindTable(TABLE_TRANSIT);

		if (pTransitTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_TRANSITLINE)->EnableWindow(a_bEnable);
		}
	}
	else
	{
		GetDlgItem(IDC_TRANSITLINE)->EnableWindow(a_bEnable);
	}
}

void KImportTableDlg::UpdateBtnZone( BOOL a_bEnable )
{
	GetDlgItem(IDC_ZONE)->EnableWindow(a_bEnable);

	if (a_bEnable)
	{
		KIOTables* pIOTables  = m_pTarget->Tables();
		KIOTable*  pZoneTable = pIOTables->FindTable(TABLE_ZONE);

		if (pZoneTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_BUTTON_Demarc)->EnableWindow(a_bEnable);
			GetDlgItem(IDC_COMBO_OD)->EnableWindow(a_bEnable);
			GetDlgItem(IDC_COMBO_SED)->EnableWindow(a_bEnable);
			GetDlgItem(IDC_OD)->EnableWindow(a_bEnable);
			GetDlgItem(IDC_SED)->EnableWindow(a_bEnable);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_Demarc)->EnableWindow(a_bEnable);
		GetDlgItem(IDC_COMBO_OD)->EnableWindow(a_bEnable);
		GetDlgItem(IDC_COMBO_SED)->EnableWindow(a_bEnable);
		GetDlgItem(IDC_OD)->EnableWindow(a_bEnable);
		GetDlgItem(IDC_SED)->EnableWindow(a_bEnable);
	}
}

void KImportTableDlg::OnBnClickedRadio( UINT nID )
{
	try
	{
		switch (nID)
		{
		case IDC_RADIO1:
			m_bIsImport = true;
			UpdateBtnImportState();
			
			break;
		case IDC_RADIO2:
			m_bIsImport = false;
			UpdateBtnExportState();
			break;
		default:
			ThrowException(_T("unexpected error"));
			break;
		}
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("알 수 없는 오류가 발생하였습니다."));
	}
}

void KImportTableDlg::UpdateBtnExportState( void )
{
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = nullptr;

	//Node
	pIOTable = pIOTables->FindTable(TABLE_NODE);
	if (nullptr != pIOTable) {
		if (pIOTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_NODE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_NODE)->EnableWindow(FALSE);
		}
	}

	//Link
	pIOTable = pIOTables->FindTable(TABLE_LINK);
	if (nullptr != pIOTable) {
		if (pIOTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_LINK)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_LINK)->EnableWindow(FALSE);
		}
	}

	//Turn
	pIOTable = pIOTables->FindTable(TABLE_TURN);
	if (nullptr != pIOTable) {
		if (pIOTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_TURN)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_TURN)->EnableWindow(FALSE);
		}
	}

	//TABLE_TRANSIT
	pIOTable = pIOTables->FindTable(TABLE_TRANSIT);
	if (nullptr != pIOTable) {
		if (pIOTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_TRANSITINFO)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_TRANSITINFO)->EnableWindow(FALSE);
		}
	}
	
	//TABLE_TRANSITLINKS
	pIOTable = pIOTables->FindTable(TABLE_TRANSITNODES);
	if (nullptr != pIOTable) {
		if (pIOTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_TRANSITLINE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_TRANSITLINE)->EnableWindow(FALSE);
		}
	}

	//TABLE_TERMINAL_SCHEDULE
	pIOTable = pIOTables->FindTable(TABLE_TERMINAL_SCHEDULE);
	if (nullptr != pIOTable) {
		if (pIOTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_SCHEDULE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_SCHEDULE)->EnableWindow(FALSE);
		}
	}

	//TABLE_ACCESS_STOP_NODE
	pIOTable = pIOTables->FindTable(TABLE_ACCESS_STOP_NODE);
	if (nullptr != pIOTable) {
		if (pIOTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_ACCESSSTOPNODE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_ACCESSSTOPNODE)->EnableWindow(FALSE);
		}
	}

	//TABLE_ZONE
	pIOTable = pIOTables->FindTable(TABLE_ZONE);
	if (nullptr != pIOTable) {
		if (pIOTable->RecordCount() != 0)
		{
			GetDlgItem(IDC_ZONE)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_OD)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_SED)->EnableWindow(TRUE);
			GetDlgItem(IDC_OD)->EnableWindow(TRUE);
			GetDlgItem(IDC_SED)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_ZONE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_OD)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_SED)->EnableWindow(FALSE);
			GetDlgItem(IDC_OD)->EnableWindow(FALSE);
			GetDlgItem(IDC_SED)->EnableWindow(FALSE);
		}
	}

	GetDlgItem(IDC_BUTTON_Demarc)->EnableWindow(FALSE);
}
