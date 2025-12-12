// DefaultCo2EmissionsInterpolationParaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultCo2EmissionsInterpolationParaDlg.h"
#include "afxdialogex.h"

#include "DefineNamesDefaultPara.h"
#include "AddModeNameDlg.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"

// KDefaultCo2EmissionsInterpolationParaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultCo2EmissionsInterpolationParaDlg, KResizeDialogEx)

KDefaultCo2EmissionsInterpolationParaDlg::KDefaultCo2EmissionsInterpolationParaDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultCo2EmissionsInterpolationParaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KDefaultCo2EmissionsInterpolationParaDlg::~KDefaultCo2EmissionsInterpolationParaDlg()
{
}

void KDefaultCo2EmissionsInterpolationParaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboMaxSpeed);
	DDX_Control(pDX, IDC_COMBO2, m_cboInterval);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KDefaultCo2EmissionsInterpolationParaDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, &KDefaultCo2EmissionsInterpolationParaDlg::OnReportItemClick)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KDefaultCo2EmissionsInterpolationParaDlg::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KDefaultCo2EmissionsInterpolationParaDlg::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDOK, &KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedInitialize)
	ON_BN_CLICKED(IDC_BUTTON2, &KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON3, &KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedDel)
	ON_BN_CLICKED(IDC_BUTTON5, &KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedExport)
	ON_BN_CLICKED(IDC_BUTTON4, &KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedImport)
END_MESSAGE_MAP()


// KDefaultCo2EmissionsInterpolationParaDlg 메시지 처리기입니다.


BOOL KDefaultCo2EmissionsInterpolationParaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		KDBaseDefaultCo2EmissionsInterpolation::GetDefaultCo2InterpolationInfo(m_mapDefCO2InterpolationInfo);
		KDBaseDefaultCo2EmissionsInterpolation::GetDefaultCo2InterpolationData(m_mapCO2InterpolationData);

		KReportCtrlSetting::Default(m_wndReport, TRUE);
		SetResizeCtrl();

		InitComboCtrl();
		InitReportHeader();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		OnBnClickedCancel();
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		OnBnClickedCancel();
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefaultCo2EmissionsInterpolationParaDlg::SetResizeCtrl( void )
{
	SetResize(IDC_REPORT,   SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON1,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON2,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON3,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON4,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON5,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDOK,         SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}

void KDefaultCo2EmissionsInterpolationParaDlg::InitComboCtrl( void )
{
	CString strValue;
	int     nValue, nIndex;

	//0. Init MaxSpeed Combo control
	m_cboMaxSpeed.ResetContent();

	for (int i = 1; i <= 20; ++i)
	{
		nValue = i * 10;

		strValue.Format(_T("%d"), nValue);
		nIndex = m_cboMaxSpeed.InsertString(-1, strValue);
		m_cboMaxSpeed.SetItemData(nIndex, nValue);
	}
	
	//1. Init Class Interval Combo Control
	m_cboInterval.ResetContent();

	for (int i = 1; i <= 10; ++i)
	{
		strValue.Format(_T("%d"), i);
		nIndex = m_cboInterval.InsertString(-1, strValue);
		m_cboInterval.SetItemData(nIndex, i);
	}

	UpdateComboCtrl();
}

void KDefaultCo2EmissionsInterpolationParaDlg::UpdateComboCtrl( void )
{
	CString strValue;
	int     nMaxSpeed, nInterval, nIndex;

	std::map<int, KCO2InterpolationSetting>::iterator fSysIter  = m_mapDefCO2InterpolationInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, KCO2InterpolationSetting>::iterator fUserIter = m_mapDefCO2InterpolationInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fUserIter != m_mapDefCO2InterpolationInfo.end())
	{
		KCO2InterpolationSetting oInfo = fUserIter->second;

		nMaxSpeed = oInfo.nMaxSpeed;
		nInterval = oInfo.nInterval;
	}
	else if (fSysIter != m_mapDefCO2InterpolationInfo.end())
	{
		KCO2InterpolationSetting oInfo = fSysIter->second;

		nMaxSpeed = oInfo.nMaxSpeed;
		nInterval = oInfo.nInterval;
	}
	else
	{
		TxLogDebugException();
		throw 1;
	}

	//Set Cursor MaxSpeed Combo
	strValue.Format(_T("%d"), nMaxSpeed);
	nIndex = m_cboMaxSpeed.FindString(-1, strValue);
	m_cboMaxSpeed.SetCurSel(nIndex);

	//Set Cursor Class Intervval Combo
	strValue.Format(_T("%d"), nInterval);
	nIndex = m_cboInterval.FindString(-1, strValue);
	m_cboInterval.SetCurSel(nIndex);
}


void KDefaultCo2EmissionsInterpolationParaDlg::InitReportHeader( void )
{
	m_wndReport.GetColumns()->Clear();
	
	int     nItemIndex = 0;
	BOOL    bAutoSize  = FALSE;
	CString strValue;

	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Select"), 50, bAutoSize));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Mode"), 60, bAutoSize));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("PCE"), 60, bAutoSize));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_RIGHT);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("승용차 환산계수"));
    }

	int nMaxSpeed = m_cboMaxSpeed.GetItemData(m_cboMaxSpeed.GetCurSel());
	int nInterval = m_cboInterval.GetItemData(m_cboInterval.GetCurSel());

	int nColumns  = (int)ceil((double)nMaxSpeed/nInterval);

	for (int i = 0; i < nColumns; ++i)
	{
		strValue.Format(_T("%d"), i*nInterval);
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, strValue, 60, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_RIGHT);
	}

	strValue.Format(_T("%d"), nMaxSpeed);
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, strValue, 60, bAutoSize));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_RIGHT);
	
	UpdateReportData();
}

void KDefaultCo2EmissionsInterpolationParaDlg::UpdateReportData( void )
{
	m_wndReport.ResetContent();

	std::map<int, std::map<int, KCO2InterpolationData>>::iterator fSysIter  = m_mapCO2InterpolationData.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::map<int, KCO2InterpolationData>>::iterator fUserIter = m_mapCO2InterpolationData.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	std::map<int, KCO2InterpolationData> mapData;

	if (fUserIter != m_mapCO2InterpolationData.end())
	{
		mapData = fUserIter->second;
	}
	else if (fSysIter != m_mapCO2InterpolationData.end())
	{
		mapData = fSysIter->second;
	}

	int nColumn = m_wndReport.GetColumns()->GetCount();

	CXTPReportRecord*         pRecord   = nullptr;
	CXTPReportRecordItem*     pItem     = nullptr;
	CXTPReportRecordItemText* pItemText = nullptr;

	std::map<int, KCO2InterpolationData>::iterator bIter = mapData.begin();
	std::set<int>::iterator fSetIter;

	int nMaxSpeed = m_cboMaxSpeed.GetItemData(m_cboMaxSpeed.GetCurSel());
	int nInterval = m_cboInterval.GetItemData(m_cboInterval.GetCurSel());

	bool bDefault = false;	//Default 편람 값을 호출하는지 아닌지

	if (nMaxSpeed == 100 && nInterval == 10)
		bDefault = true;

	while (bIter != mapData.end())
	{
		KCO2InterpolationData oData = bIter->second;

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(oData.bIsSelected);
		pItem->SetItemData(bIter->first);

		fSetIter = m_setSelected.find(bIter->first);

		if (fSetIter != m_setSelected.end())
			pItem->SetChecked(TRUE);

		//mode name
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(oData.strModeName));

		//pce
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oData.dPCE, _T("%.2f")));

		std::vector<double> vecData = oData.vecData;

		if (bDefault)
		{
			for (int i = 0; i < (nColumn - 3); ++i)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(vecData[i], _T("%.5f")));
			}
		}
		else
		{
			for (int i = 0; i < (nColumn - 3); ++i)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.5f")));
			}
		}

		++bIter;
	}

	m_wndReport.Populate();
}

void KDefaultCo2EmissionsInterpolationParaDlg::OnCbnSelchangeCombo()
{
	InitReportHeader();
}

void KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedOk()
{
	try
	{
		SaveInterpolationInfo();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;	
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("저장하는 과정에서 오류가 발생하였습니다."));
		return;
	}
	
	AfxMessageBox(_T("적용되었습니다."));
	KResizeDialogEx::OnOK();
}


void KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KDefaultCo2EmissionsInterpolationParaDlg::SaveInterpolationInfo( void )
{
	//Get Interpolation Info
	std::map<int, KCO2InterpolationSetting> mapCO2InterpolationInfo;

	int nMaxSpeed = m_cboMaxSpeed.GetItemData(m_cboMaxSpeed.GetCurSel());
	int nInterval = m_cboInterval.GetItemData(m_cboInterval.GetCurSel());
	
	KCO2InterpolationSetting oSetting;
	oSetting.nMaxSpeed = nMaxSpeed;
	oSetting.nInterval = nInterval;

	mapCO2InterpolationInfo.insert(std::make_pair(KDefaultParameterMasterCode::USER_MASETER_CODE, oSetting));

	//Get Interpolation Data
	std::map<int, std::map<int, KCO2InterpolationData>> mapCO2InterpolationData;
	std::map<int, KCO2InterpolationData> mapData;

	int nColumn = m_wndReport.GetColumns()->GetCount();
	int nRecord = m_wndReport.GetRecords()->GetCount();

	CXTPReportRecord*           pRecord     = nullptr;
	CXTPReportRecordItem*       pItem       = nullptr;
	CXTPReportRecordItemText*   pItemText   = nullptr;
	CXTPReportRecordItemNumber* pItemNumber = nullptr;

	for (int i = 0; i < nRecord; ++i)
	{
		KCO2InterpolationData oData;

		pRecord = m_wndReport.GetRecords()->GetAt(i);

		pItem = pRecord->GetItem(0);
		
		oData.nMasterCode = KDefaultParameterMasterCode::USER_MASETER_CODE;
		oData.nModeSeq    = pItem->GetItemData();
		oData.bIsSelected = pItem->IsChecked() ? TRUE : FALSE;

		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
		oData.strModeName = pItemText->GetValue();

		//PCE
		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
		oData.dPCE  = pItemNumber->GetValue();

		if (oData.dPCE == 0.0)
		{
			ThrowException(_T("PCE값은 0이 될 수 없습니다."));
		}

		//Value부분;
		std::vector<double> vecData;

		for (int i = 3; i < nColumn; ++i)
		{
			pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(i);
			vecData.push_back(pItemNumber->GetValue());
		}

		oData.vecData = vecData;

		mapData.insert(std::make_pair(oData.nModeSeq, oData));
	}

	mapCO2InterpolationData.insert(std::make_pair(KDefaultParameterMasterCode::USER_MASETER_CODE, mapData));

	KDBaseDefaultCo2EmissionsInterpolation::SaveDefaultCo2Interpolation(mapCO2InterpolationInfo, mapCO2InterpolationData);
}


void KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedInitialize()
{
	KDBaseDefaultCo2EmissionsInterpolation::GetDefaultCo2InterpolationInfo(m_mapDefCO2InterpolationInfo);
	KDBaseDefaultCo2EmissionsInterpolation::GetDefaultCo2InterpolationData(m_mapCO2InterpolationData);

	//ComboInit;
	UpdateComboCtrl();
	OnCbnSelchangeCombo();
}

void KDefaultCo2EmissionsInterpolationParaDlg::OnReportItemClick( NMHDR* a_pNMHDR, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) a_pNMHDR;

		if (!pClickedItem->pRow || !pClickedItem->pColumn)
			return;

		int nRowIndex    = pClickedItem->pRow->GetIndex();
		int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

		if (nColumnIndex != 0)
			return;

		CXTPReportRecord*     pRecord = pClickedItem->pRow->GetRecord();
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked())
		{
			m_setSelected.insert(pItem->GetItemData());
		}
		else
		{
			m_setSelected.erase(pItem->GetItemData());
		}
		
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedAdd()
{
	try
	{
		CXTPReportRecord*         pRecord   = nullptr;
		CXTPReportRecordItemText* pItemText = nullptr;

		int nRecord  = m_wndReport.GetRecords()->GetCount();
		int nModeSeq = 0;

		std::set<CString> setModeName;

		for (int i = 0; i < nRecord; ++i)
		{
			pRecord = m_wndReport.GetRecords()->GetAt(i);

			if (nModeSeq < (int)pRecord->GetItem(0)->GetItemData())
				nModeSeq = pRecord->GetItem(0)->GetItemData();

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			setModeName.insert(pItemText->GetValue());
		}

		int nNextModeSeq = nModeSeq + 1;

		KAddModeNameDlg oDlg(setModeName);

		if (oDlg.DoModal() != IDOK)
			return;

		std::map<int, std::map<int, KCO2InterpolationData>>::iterator fUserData = m_mapCO2InterpolationData.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		std::map<int, std::map<int, KCO2InterpolationData>>::iterator fSysData  = m_mapCO2InterpolationData.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		int nMasterCode;

		if (fUserData != m_mapCO2InterpolationData.end())
		{
			nMasterCode = KDefaultParameterMasterCode::USER_MASETER_CODE;
		}
		else if (fSysData != m_mapCO2InterpolationData.end())
		{
			nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
		}

		fUserData = m_mapCO2InterpolationData.find(nMasterCode);

		std::map<int, KCO2InterpolationData>& mapData = fUserData->second;

		KCO2InterpolationData oData;

		oData.nMasterCode = nMasterCode;
		oData.nModeSeq    = nNextModeSeq;
		oData.bIsSelected = 0;
		oData.strModeName = oDlg.GetModeName();
		oData.dPCE        = 0.0;

		//VecData Genereate zero;
		int nColumn  = m_wndReport.GetColumns()->GetCount();
		int nVecSize = nColumn - 2;

		std::vector<double> vecData;

		for (int i = 0; i < nVecSize; ++i)
		{
			vecData.push_back(0.0);
		}

		oData.vecData = vecData;

		mapData.insert(std::make_pair(nNextModeSeq, oData));
	}
	catch (...)
	{
		AfxMessageBox(_T("통행수단을 추가하는 과정에서 오류가 발생하였습니다."));
		return;
	}

	UpdateReportData();
}


void KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedDel()
{
	try
	{
		std::set<int> setChecked;

		CXTPReportRecordItem* pItem   = nullptr;

		int nRecord = m_wndReport.GetRecords()->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pItem = m_wndReport.GetRecords()->GetAt(i)->GetItem(0);

			if (pItem->IsChecked())
				setChecked.insert(pItem->GetItemData());
		}

		if (setChecked.size() == 0)
		{
			AfxMessageBox(_T("선택한 항목이 없습니다."));
			return;
		}

		std::map<int, std::map<int, KCO2InterpolationData>>::iterator fUserData = m_mapCO2InterpolationData.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		std::map<int, std::map<int, KCO2InterpolationData>>::iterator fSysData  = m_mapCO2InterpolationData.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		int nMasterCode;

		if (fUserData != m_mapCO2InterpolationData.end())
		{
			nMasterCode = KDefaultParameterMasterCode::USER_MASETER_CODE;
		}
		else if (fSysData != m_mapCO2InterpolationData.end())
		{
			nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
		}
		else
		{
			ThrowException(_T("데이터가 존재하지 않습니다."));
		}

		fUserData = m_mapCO2InterpolationData.find(nMasterCode);

		std::map<int, KCO2InterpolationData>& mapData = fUserData->second;

		std::set<int>::iterator bSetIter = setChecked.begin();

		while (bSetIter != setChecked.end())
		{
			mapData.erase(*bSetIter);
			m_setSelected.erase(*bSetIter);
			++bSetIter;
		}
	}
	catch (...)
	{
		AfxMessageBox(_T("통행수단을 삭제하는 과정에서 오류가 발생하였습니다."));
		return;
	}
	
	UpdateReportData();
}


void KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedExport()
{
	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle = _T("Export Interpolation Parameter");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strExportFile = fileDlg.GetPathName();
		strExportFile.Replace(_T(".txt"), _T(""));
		m_strFile.Format(_T("%s.txt"), strExportFile);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ExportThreadCaller, this, false, nLang);

		if (!m_strErrMsg.IsEmpty())
		{
			AfxMessageBox(m_strErrMsg);
			return;
		}
		
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("데이터 불러오기에 실패하였습니다."));
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("데이터 불러오기에 실패하였습니다."));
		return;
	}

	AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
}

unsigned __stdcall KDefaultCo2EmissionsInterpolationParaDlg::ExportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefaultCo2EmissionsInterpolationParaDlg* pDlg = (KDefaultCo2EmissionsInterpolationParaDlg*)pParameter->GetParameter();
	pDlg->ExportData();

	return 0;
}

void KDefaultCo2EmissionsInterpolationParaDlg::ExportData( void )
{
	m_strErrMsg = _T("");

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(m_strFile, nOpenMode);
	CString      strLine;

	try
	{
		//Write, maxSpeed, Class Interval
		int nMaxSpeed = m_cboMaxSpeed.GetItemData(m_cboMaxSpeed.GetCurSel());
		int nInterval = m_cboInterval.GetItemData(m_cboInterval.GetCurSel());

		strLine.Format(_T("%d, %d\r\n"), nMaxSpeed, nInterval);
		outFile.WriteString(strLine);

		CXTPReportRecord*           pRecord     = nullptr;
		CXTPReportRecordItem*       pItem       = nullptr;
		CXTPReportRecordItemText*   pItemText   = nullptr;
		CXTPReportRecordItemNumber* pItemNumber = nullptr;

		int nColumn = m_wndReport.GetColumns()->GetCount();
		int nRecord = m_wndReport.GetRecords()->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pRecord = m_wndReport.GetRecords()->GetAt(i);
			pItem   = pRecord->GetItem(0);

			if (pItem->IsChecked())
			{
				strLine = _T("1");
			}
			else
			{
				strLine = _T("0");
			}

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			strLine.AppendFormat(_T(", %s"), pItemText->GetValue());

			for (int j = 2; j < nColumn; ++j)
			{
				pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(j);
				strLine.AppendFormat(_T(", %f"), pItemNumber->GetValue());
			}

			strLine.Append(_T("\r\n"));
			outFile.WriteString(strLine);
		}

		outFile.Close();
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		TxLogDebug(ex->GetErrorMessage());
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		outFile.Close();
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KDefaultCo2EmissionsInterpolationParaDlg::OnBnClickedImport()
{
	try
	{
		TCHAR       szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		CFileDialog filedlg( TRUE, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter );

		KScenario* pScenario   = (KScenario*)m_pTarget->GetParentObject();
		KProject*  pProject    = (KProject*)pScenario->GetParentObject();
		CString    strLocation = pProject->GetLocation();

		if (!strLocation.IsEmpty())
			filedlg.m_ofn.lpstrInitialDir = strLocation;

		if( filedlg.DoModal() != IDOK )
			return;

		m_strFile = filedlg.GetPathName();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ImportThreadCaller, this, false, nLang);

		if (!m_strErrMsg.IsEmpty())
		{
			AfxMessageBox(m_strErrMsg);
			return;
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("데이터 내보내기에 실패하였습니다."));
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("데이터 내보내기에 실패하였습니다."));
		return;
	}

	AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));
}

unsigned __stdcall KDefaultCo2EmissionsInterpolationParaDlg::ImportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefaultCo2EmissionsInterpolationParaDlg* pDlg = (KDefaultCo2EmissionsInterpolationParaDlg*)pParameter->GetParameter();
	pDlg->ImportData();

	return 0;
}

void KDefaultCo2EmissionsInterpolationParaDlg::ImportData( void )
{
	m_strErrMsg = _T("");
	m_vecRow.clear();

	UINT         nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT         nCodePage = 949;
	CStdioFileEx csvFile;

	CString strLine, strTemp, strErr;
	int     nMaxSpeedIndex, nIntervalIndex;
	try
	{
		csvFile.SetCodePage(nCodePage);
		
		if (csvFile.Open(m_strFile, nOpenMode) == FALSE)
			ThrowException(_T("파일 열기를 실패했습니다."));

		int nLine(0);

		CSVRow row;

		//MaxSpeed, Interval Read
		while (csvFile.ReadString(strLine))
		{
			++nLine;

			strLine.Trim();
			strLine.Replace(_T(" "), _T(""));

			if (strLine.IsEmpty())
				continue;

			ParseCSVLineString(strLine, row);
			break;
		}

		if (row.size() != 2)
		{
			strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
			ThrowException(strTemp);
		}

		nMaxSpeedIndex = m_cboMaxSpeed.FindString(-1, row[0]);
		nIntervalIndex = m_cboInterval.FindString(-1, row[1]);

		//입력한 maxSpeed, interval 값이 콤보에 있는지를 체크함;
		if (nMaxSpeedIndex == -1 || nIntervalIndex == -1)
		{
			strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
			ThrowException(strTemp);
		}

		//Import Text의 Item 개수는 ReportCtrl의 컬럼수와 같아야함
		int nColumn = m_wndReport.GetColumns()->GetCount();	

		//Import Data
		while (csvFile.ReadString(strLine))
		{
			++nLine;
			row.clear();

			strLine.Trim();
			strLine.Replace(_T(" "), _T(""));

			if (strLine.IsEmpty())
				continue;

			ParseCSVLineString(strLine, row);
			
			if (row.size() != nColumn)
			{
				strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
				ThrowException(strTemp);
			}

			m_vecRow.push_back(row);
		}

		csvFile.Close();

		//Update Data m_mapCO2InterpolationData;
		std::map<int, KCO2InterpolationData> mapData;

		int nSize = m_vecRow.size();

		for (int i = 0; i < nSize; ++i)
		{
			CSVRow csvRow = m_vecRow[i];

			KCO2InterpolationData oData;

			oData.nMasterCode = KDefaultParameterMasterCode::USER_MASETER_CODE;
			oData.nModeSeq    = i + 1;

			if (csvRow[0].CompareNoCase(_T("1")) == 0)
			{
				oData.bIsSelected = TRUE;
			}
			else
			{
				oData.bIsSelected = FALSE;
			}

			oData.strModeName = csvRow[1];
			oData.dPCE        = _ttof(csvRow[2]);
			
			std::vector<double> vecData;

			int nDataSize = csvRow.size();

			for (int j = 3; j < nDataSize; ++j)
			{
				vecData.push_back(_ttof(csvRow[j]));
			}

			oData.vecData = vecData;

			mapData.insert(std::make_pair(oData.nModeSeq, oData));
		}

		m_mapCO2InterpolationData.erase(KDefaultParameterMasterCode::USER_MASETER_CODE);
		m_mapCO2InterpolationData.insert(std::make_pair(KDefaultParameterMasterCode::USER_MASETER_CODE, mapData));

		m_cboMaxSpeed.SetCurSel(nMaxSpeedIndex);
		m_cboInterval.SetCurSel(nIntervalIndex);
		m_setSelected.clear();
		
		OnCbnSelchangeCombo();
	}
	catch (KExceptionPtr ex)
	{
		csvFile.Close();
		TxLogDebug(ex->GetErrorMessage());
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		csvFile.Close();
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KDefaultCo2EmissionsInterpolationParaDlg::ParseCSVLineString( CString strLine, CSVRow& row )
{
	bool bQuote = false;
	int nSub = 0;
	CString strRow, strQuote;

	while( AfxExtractSubString( strRow, strLine, nSub++, ',' ) == TRUE )
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
