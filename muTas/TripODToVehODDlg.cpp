// TripODToVehODDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "TripODToVehODDlg.h"
#include "ExcelExportConfigDlg.h"
#include "ImChampDir.h"
#include "UtilExcelHandler.h"

#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// KTripODToVehODDlg 대화 상자

IMPLEMENT_DYNAMIC(KTripODToVehODDlg, KDialogEx)

KTripODToVehODDlg::KTripODToVehODDlg(CWnd* pParent /*=nullptr*/)
	: KDialogEx(IDD_7000_UTIL_TripODtoVehOD, pParent)
{
}

KTripODToVehODDlg::~KTripODToVehODDlg()
{
}

void KTripODToVehODDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboRegionNumber);
	DDX_Control(pDX, IDC_COMBO2, m_cboBaseYear);
	DDX_Control(pDX, IDC_COMBO3, m_cboMainSelSheet);
	DDX_Control(pDX, IDC_COMBO4, m_cboMainSelHeader);
	DDX_Control(pDX, IDC_COMBO5, m_cboSubSelSheet);
	DDX_Control(pDX, IDC_COMBO6, m_cboSubSelHeader);
	DDX_Control(pDX, IDC_EDITOR1, m_editMainODFilePath);
	DDX_Control(pDX, IDC_EDITOR2, m_editSubODFilePath);
	DDX_Control(pDX, IDC_REPORT1, m_rptODColumn);
	DDX_Control(pDX, IDC_REPORT2, m_rptMainFilePreview);
	DDX_Control(pDX, IDC_REPORT3, m_rptSubFilePreview);
	DDX_Control(pDX, IDC_CHECK4, m_chkAutoColumn);
	DDX_Control(pDX, IDC_STCANIMATE, m_loadingGif);
}


BEGIN_MESSAGE_MAP(KTripODToVehODDlg, KDialogEx)
	ON_BN_CLICKED(ID_RUN, &KTripODToVehODDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BTN_MAINODFILE, &KTripODToVehODDlg::OnBnClickedBtnMainODFile)
	ON_BN_CLICKED(IDC_BTN_SUBODFILE, &KTripODToVehODDlg::OnBnClickedBtnSubODFile)
	ON_BN_CLICKED(IDC_CHECK4, &KTripODToVehODDlg::OnBnClickedCheck4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KTripODToVehODDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KTripODToVehODDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BTN_CONVERT_EXCEL, &KTripODToVehODDlg::OnBnClickedBtnConvertExcel)
	ON_CBN_SELCHANGE(IDC_COMBO3, &KTripODToVehODDlg::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO5, &KTripODToVehODDlg::OnCbnSelchangeCombo5)
	ON_CBN_SELCHANGE(IDC_COMBO4, &KTripODToVehODDlg::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO6, &KTripODToVehODDlg::OnCbnSelchangeCombo6)
	ON_MESSAGE(WM_UPDATE_MAIN_REPORT, &KTripODToVehODDlg::OnUpdateMainReport)
	ON_MESSAGE(WM_UPDATE_SUB_REPORT, &KTripODToVehODDlg::OnUpdateSubReport)
	ON_MESSAGE(WM_THREAD_FINISHED, &KTripODToVehODDlg::OnThreadFinished)
END_MESSAGE_MAP()


// KTripODToVehODDlg 메시지 처리기

BOOL KTripODToVehODDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	m_loadingGif.Load(MAKEINTRESOURCE(IDR_PROGRESSIMAGE), _T("GIF"));
	DrawingLoadingGif(false);

	for (int i = 0; i < UTIL_TRIPOD_TO_VEHOD_AREA_CNT; i++)
	{
		m_cboRegionNumber.InsertString(i, UTIL_TRIPOD_TO_VEHOD_AREA[i]);
	}

	LoadBaseYearList();

	GetDlgItem(ID_RUN)->EnableWindow(FALSE);

	QBicReportCtrlSetting::Default(m_rptODColumn, TRUE, FALSE, TRUE);
	QBicReportCtrlSetting::Default(m_rptMainFilePreview, FALSE, FALSE, FALSE);
	QBicReportCtrlSetting::Default(m_rptSubFilePreview, FALSE, FALSE, FALSE);

	InitODColumn();
	LoadConfigFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTripODToVehODDlg::OnCancel()
{
	if (m_nActiveThreadCount > 0)
	{
		m_bAbortThread = true;

		while (m_nActiveThreadCount > 0)
		{
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Sleep(10);
		}
	}

	KDialogEx::OnCancel();
}


void KTripODToVehODDlg::OnBnClickedRun()
{
	if (!m_bMainODLoadFile && !m_bSubODLoadFile)
	{
		AfxMessageBox(_T("OD 파일이 선택되지 않았습니다."));
		return;
	}

	m_bAbortThread = false;
	m_nActiveThreadCount++;
	DrawingLoadingGif(true);

	GetDlgItem(ID_RUN)->EnableWindow(FALSE);

	AfxBeginThread(ThreadWorker, this);
}

void KTripODToVehODDlg::LoadBaseYearList()
{
	try
	{
		TCHAR szModulePath[MAX_PATH] = { 0 };
		GetModuleFileName(nullptr, szModulePath, MAX_PATH);

		CString basePath(szModulePath);
		int pos = basePath.ReverseFind(_T('\\'));
		if (pos != -1)
		{
			basePath = basePath.Left(pos + 1);
		}

		CString distPath = basePath + _T("Utility\\TripODToVehOD\\죤체계_재차인원\\");

		m_cboBaseYear.ResetContent();

		CFileFind finder;
		CString strSearch = distPath + _T("*.*");

		BOOL bWorking = finder.FindFile(strSearch);
		int addedCount = 0;

		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			if (finder.IsDots())
				continue;

			if (finder.IsDirectory())
			{
				CString fileName = finder.GetFileTitle();
				m_cboBaseYear.AddString(fileName);
				addedCount++;
			}
		}
		finder.Close();

		if (addedCount == 0)
		{
			throw 1;
		}
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("재차인원 폴더가 존재하지 않습니다."));
			AfxMessageBox(strError);
		}
		else
		{
			strError.Format(_T("Error : %d"), ex);
		}		
		TxLogDebug((LPCTSTR)strError);
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

void KTripODToVehODDlg::InitODColumn()
{
	TxLogDebugStartMsg();

	try
	{
		m_rptODColumn.ResetContent();
		m_rptODColumn.GetColumns()->Clear();
		m_rptODColumn.Populate();

		// 재차인원 적용 컬럼	
		// System Column
		CXTPReportColumn* pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(0, _T("System Column"), 36));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);

		// Type
		pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(1, _T("Type"), 20));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);

		// Main File Column
		pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(2, _T("Main File Column"), 39));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);

		// Sub File Column
		pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(3, _T("Sub File Column"), 39));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);
	}
	catch (int& ex) 
	{
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	}
	catch (KExceptionPtr ex) 
	{
		TxExceptionPrint(ex);
	}
	catch (...) 
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

void KTripODToVehODDlg::LoadConfigFile()
{
	try
	{
		TCHAR szModulePath[MAX_PATH] = { 0 };
		GetModuleFileName(nullptr, szModulePath, MAX_PATH);

		CString basePath(szModulePath);
		int pos = basePath.ReverseFind(_T('\\'));
		if (pos != -1)
		{
			basePath = basePath.Left(pos + 1);
		}

		CString configPath = basePath + _T("Utility\\TripODToVehOD\\TripODToVehODConfig.json");

		std::ifstream file;
		file.open((LPCTSTR)configPath);
		if (!file.is_open())
		{
			throw 1;
		}

		json j;
		file >> j;

		for (auto& region : j.items())
		{
			std::string sRegionName = region.key();
			CString strRegion = CA2T(sRegionName.c_str(), CP_UTF8);

			for (auto& year : region.value().items())
			{
				std::string sYear = year.key();
				CString strYear = CA2T(sYear.c_str(), CP_UTF8);

				std::vector<ColumnInfo> colInfo;

				for (auto& col : year.value())
				{
					ColumnInfo info;
					std::string sColName = col["name"];
					info.name = CA2T(sColName.c_str(), CP_UTF8);
					if (col.contains("maincol") && col["maincol"].is_number())
						info.mainCol = col["maincol"];
					else
						info.mainCol = 0;
					if (col.contains("subcol") && col["subcol"].is_number())
						info.subCol = col["subcol"];
					else
						info.subCol = 0;
					colInfo.push_back(info);
				}

				m_AreaInfoMap[strRegion][strYear] = colInfo;
			}
		}
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("TripODToVehODConfig.json 설정 파일을 열 수 없습니다."));
			AfxMessageBox(strError);
		}
		else
		{
			strError.Format(_T("Error : %d"), ex);
		}
		TxLogDebug((LPCTSTR)strError);
	}
	catch (json::parse_error& e)
	{
		CString strError(_T("JSON 파일 읽는 중 오류 발생."));
		AfxMessageBox(strError);
		TxLogDebug((LPCTSTR)strError);
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

bool KTripODToVehODDlg::GetColumnInfo(CString region, CString year, std::vector<ColumnInfo>& columnInfo)
{
	auto itRegion = m_AreaInfoMap.find(region);
	if (itRegion != m_AreaInfoMap.end())
	{
		auto itYear = itRegion->second.find(year);
		if (itYear != itRegion->second.end())
		{
			columnInfo = itYear->second;
			return true;
		}
	}

	return false;
}

void KTripODToVehODDlg::OnBnClickedBtnMainODFile()
{
	if (m_cboRegionNumber.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("권역번호를 선택하여 주십시오."));
		return;
	}
	if (m_cboBaseYear.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("기준년도를 선택하여 주십시오."));
		return;
	}

	int areaIdx = m_cboRegionNumber.GetCurSel();
	if (areaIdx == 0)
		m_strRegion = _T("수도권");
	else if (areaIdx == 1)
		m_strRegion = _T("전국권");
	else
		m_strRegion = _T("5대권역");

	m_cboBaseYear.GetWindowText(m_strYear);


	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, 
		_T("지원되는 파일 (*.xlsx; *.xlsb; *.txt)|*.xlsx;*.xlsb;*.txt||"), this);
	dlg.m_ofn.lpstrTitle = _T("OD 파일 선택");

	if (dlg.DoModal() != IDOK)
		return;

	m_editMainODFilePath.SetWindowText(dlg.GetPathName());
	m_strMainODFilePath = dlg.GetPathName();

	CString strExt = dlg.GetFileExt();
	strExt.MakeLower();

	if (strExt == _T("txt"))
		m_eMainODFileType = Text;
	else
		m_eMainODFileType = Excel;

	m_bMainODFileLoading = true;
	m_nActiveThreadCount++;
	
	DrawingLoadingGif(true);

	AfxBeginThread(ThreadFileLoading, this);
}

void KTripODToVehODDlg::OnBnClickedBtnSubODFile()
{
	if (m_cboRegionNumber.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("권역번호를 선택하여 주십시오."));
		return;
	}
	if (m_cboBaseYear.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("기준년도를 선택하여 주십시오."));
		return;
	}

	int areaIdx = m_cboRegionNumber.GetCurSel();
	if (areaIdx == 0)
		m_strRegion = _T("수도권");
	else if (areaIdx == 1)
		m_strRegion = _T("전국권");
	else
		m_strRegion = _T("5대권역");

	m_cboBaseYear.GetWindowText(m_strYear);


	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("지원되는 파일 (*.xlsx; *.xlsb; *.txt)|*.xlsx;*.xlsb;*.txt||"), this);
	dlg.m_ofn.lpstrTitle = _T("OD 파일 선택");

	if (dlg.DoModal() != IDOK)
		return;

	m_editSubODFilePath.SetWindowText(dlg.GetPathName());
	m_strSubODFilePath = dlg.GetPathName();

	CString strExt = dlg.GetFileExt();
	strExt.MakeLower();

	if (strExt == _T("txt"))
		m_eSubODFileType = Text;
	else
		m_eSubODFileType = Excel;

	m_bSubODFileLoading = true;
	m_nActiveThreadCount++;

	DrawingLoadingGif(true);

	AfxBeginThread(ThreadFileLoading, this);
}

void KTripODToVehODDlg::OnBnClickedCheck4()
{
	std::vector <ColumnInfo> stColumnInfo;
	if (!GetColumnInfo(m_strRegion, m_strYear, stColumnInfo))
	{
		AfxMessageBox(_T("선택한 지역과 연도에 대한 컬럼 정보가 없습니다.\n설정 파일을 확인해 주세요."));
		return;
	}

	if (!m_bMainODLoadFile && !m_bSubODLoadFile)
		return;

	try
	{
		CXTPReportRecord* pRecord = nullptr;
		KColumnReportRecordItem* pItem = nullptr;

		for (int i = 0; i < stColumnInfo.size(); i++)
		{
			pRecord = m_rptODColumn.GetRecords()->GetAt(i);

			if (m_bMainODLoadFile)
			{
				pItem = (KColumnReportRecordItem*)pRecord->GetItem(2);
				if (m_chkAutoColumn.GetCheck() == BST_CHECKED)
					pItem->m_nIndex = stColumnInfo[i].mainCol;
				else
					pItem->m_nIndex = i + 1;
			}
			
			if (m_bSubODLoadFile)
			{
				pItem = (KColumnReportRecordItem*)pRecord->GetItem(3);
				if (m_chkAutoColumn.GetCheck() == BST_CHECKED)
					pItem->m_nIndex = stColumnInfo[i].subCol;
				else
					pItem->m_nIndex = i + 1;
			}
		}
		m_rptODColumn.Populate();
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
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

void KTripODToVehODDlg::OnCbnSelchangeCombo1()
{
	int areaIdx = m_cboRegionNumber.GetCurSel();
	if (areaIdx == 0)
		m_strRegion = _T("수도권");
	else if (areaIdx == 1)
		m_strRegion = _T("전국권");
	else
		m_strRegion = _T("5대권역");

	if (m_cboBaseYear.GetCurSel() == -1)
		return;

	SetCurColumnInfo();
	UpdateODColumnReport();
}


void KTripODToVehODDlg::OnCbnSelchangeCombo2()
{
	m_cboBaseYear.GetWindowText(m_strYear);
	if (m_cboRegionNumber.GetCurSel() == -1)
		return;

	SetCurColumnInfo();
	UpdateODColumnReport();
}

void KTripODToVehODDlg::OnBnClickedBtnConvertExcel()
{
	KExcelExportConfigDlg dlg(this);
	dlg.DoModal();
}



void KTripODToVehODDlg::LoadSheetList(CString strPath, bool bIsMain)
{
	try
	{
		if (bIsMain)
		{
			m_cboMainSelSheet.Clear();
			m_cboMainSelHeader.Clear();
			m_cboMainSelSheet.EnableWindow(TRUE);
			m_cboMainSelHeader.EnableWindow(TRUE);
		}
		else
		{
			m_cboSubSelSheet.Clear();
			m_cboSubSelHeader.Clear();
			m_cboSubSelSheet.EnableWindow(TRUE);
			m_cboSubSelHeader.EnableWindow(TRUE);
		}

		strPath.Trim();

		if (strPath.IsEmpty())
			throw 1;

		std::vector<CString> sheetList;
		sheetList.clear();

		if (!m_excelHandler.GetExcelSheetNames(strPath, sheetList))
			throw 2;

		for (size_t i = 0; i < sheetList.size(); i++)
		{
			if (bIsMain)
				m_cboMainSelSheet.AddString(sheetList[i]);
			else
				m_cboSubSelSheet.AddString(sheetList[i]);
		}

		if (!sheetList.empty())
		{
			if (bIsMain)
				m_cboMainSelSheet.SetCurSel(0);
			else
				m_cboSubSelSheet.SetCurSel(0);
		}

		for (int i = 0; i < 5; i++)
		{
			CString strHeaderIdx;
			strHeaderIdx.Format(_T("%d"), i + 1);
			if (bIsMain)
				m_cboMainSelHeader.AddString(strHeaderIdx);
			else
				m_cboSubSelHeader.AddString(strHeaderIdx);
		}

		if (bIsMain)
			m_cboMainSelHeader.SetCurSel(0);
		else
			m_cboSubSelHeader.SetCurSel(0);
	}
	catch (int& ex)
	{
		CString strMsg(_T(""));
		if (1 == ex)
			strMsg.Format(_T("Fail - Read ExcelFile : %s"), strPath);
		else if (ex == 2)
			strMsg = _T("Failed to Load Excel Sheet List");
		else
			strMsg.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strMsg);
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

void KTripODToVehODDlg::CheckImportHeader(CString a_strFile, bool bIsMain)
{
	std::vector<CString>& vecHeader = bIsMain ? m_vecMainHeader : m_vecSubHeader;
	vecHeader.clear();
	vecHeader.push_back(_T("null"));

	try
	{
		CStdioFile file;
		if (!file.Open(a_strFile, CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
			throw 1;

		CString strFirstLine;
		if (!file.ReadString(strFirstLine))
		{
			file.Close();
			return;
		}
		file.Close();

		CString strOriginal = strFirstLine;
		strFirstLine.TrimLeft(_T(" \t,;"));

		if (strFirstLine.IsEmpty()) return;

		TCHAR ch = strFirstLine.GetAt(0);
		bool bHasHeader = !((ch >= _T('0') && ch <= _T('9')) || ch == _T('.'));

		// 멤버 변수 업데이트
		if (bIsMain) m_bMainImportHeader = bHasHeader;
		else         m_bSubImportHeader = bHasHeader;

		// --- 헤더 벡터 생성 로직 ---
		int nPos = 0;
		CString strToken = strOriginal.Tokenize(_T(" \t,;"), nPos);

		if (bHasHeader)
		{
			// 1. 헤더가 있는 경우: 실제 텍스트를 넣음
			while (!strToken.IsEmpty())
			{
				vecHeader.push_back(strToken);
				strToken = strOriginal.Tokenize(_T(" \t,;"), nPos);
			}
		}
		else
		{
			// 2. 헤더가 없는 경우: "Column1", "Column2..." 자동 생성
			int nColIndex = 1;
			while (!strToken.IsEmpty())
			{
				CString strDefaultHeader;
				strDefaultHeader.Format(_T("Column%d"), nColIndex++);
				vecHeader.push_back(strDefaultHeader);

				strToken = strOriginal.Tokenize(_T(" \t,;"), nPos);
			}
		}
	}
	catch (int& ex) 
	{
		CString strMsg(_T(""));
		if (1 == ex)
			strMsg.Format(_T("Fail - Read TextFile : %s"), a_strFile);
		else
			strMsg.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strMsg);
	}
	catch (KExceptionPtr ex) 
	{
		TxExceptionPrint(ex);
	}
	catch (...) 
	{
		TxLogDebugException();
	}

	//UpdateData(FALSE);
}

void KTripODToVehODDlg::UpdateMainPreviewReport()
{
	TxLogDebugStartMsg();

	try
	{
		if (!m_bMainODLoadFile)
			return;

		m_rptMainFilePreview.ResetContent();
		m_rptMainFilePreview.GetColumns()->Clear();
		m_rptMainFilePreview.Populate();

		bool bAutoSize = false;
		if (m_vecMainODData.size() < 10)
			bAutoSize = true;

		if (!bAutoSize)
			QBicReportCtrlSetting::AutoColumnSizeFalse(m_rptMainFilePreview);
		else
			m_rptMainFilePreview.GetReportHeader()->SetAutoColumnSizing(TRUE);

		CXTPReportColumn* pColumn = NULL;

		int nColumnIndex(0);
		std::vector< CString >::iterator itColumnArray = m_vecMainHeader.begin() + 1;
		while (m_vecMainHeader.end() != itColumnArray)
		{
			pColumn = m_rptMainFilePreview.AddColumn(new CXTPReportColumn(nColumnIndex, (*itColumnArray), 100));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (!bAutoSize)
				pColumn->SetBestFitMode(xtpColumnBestFitModeAllData);

			++nColumnIndex;
			++itColumnArray;
		}

		CXTPReportRecord* pRecord = nullptr;
		CXTPReportRecordItem* pItem = nullptr;

		int nRow(0);
		std::vector<ODData>::iterator itRows = m_vecMainODData.begin(), itRowsEnd = m_vecMainODData.end();
		if (m_bMainImportHeader)
			itRows++; // 첫번째 행은 컬럼명 이므로 패스
		for (; itRows != itRowsEnd && nRow < 200; ++itRows)
		{
			pRecord = m_rptMainFilePreview.AddRecord(new CXTPReportRecord());

			ODData& row = (*itRows);
			ODData::iterator itrow, itrowEnd = row.end();
			int nAddedCol = 0;
			for (itrow = row.begin(); itrow != itrowEnd && nAddedCol < nColumnIndex; ++itrow, ++nAddedCol)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(*itrow));
				pItem->SetAlignment(DT_CENTER);
			}

			if (nColumnIndex > nAddedCol)
			{
				for (int i = 0; i < (nColumnIndex - nAddedCol); i++)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItem);
				}
			}

			++nRow;
		}

		m_rptMainFilePreview.Populate();

		CXTPReportHeader* pHeader = m_rptMainFilePreview.GetReportHeader();
		CXTPReportColumns* pReportColumns = m_rptMainFilePreview.GetColumns();
		int                nReportColumns = pReportColumns->GetCount();
		for (int i = 0; i < nReportColumns; i++)
		{
			CXTPReportColumn* pReportColumn = pReportColumns->GetAt(i);
			if (!bAutoSize)
				pHeader->BestFit(pReportColumn);
		}
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

void KTripODToVehODDlg::UpdateSubPreviewReport()
{
	TxLogDebugStartMsg();

	try
	{
		if (!m_bSubODLoadFile)
			return;

		m_rptSubFilePreview.ResetContent();
		m_rptSubFilePreview.GetColumns()->Clear();
		m_rptSubFilePreview.Populate();

		bool bAutoSize = false;
		if (m_vecSubODData.size() < 10)
			bAutoSize = true;

		if (!bAutoSize)
			QBicReportCtrlSetting::AutoColumnSizeFalse(m_rptSubFilePreview);
		else
			m_rptSubFilePreview.GetReportHeader()->SetAutoColumnSizing(TRUE);

		CXTPReportColumn* pColumn = NULL;

		int nColumnIndex(0);
		std::vector< CString >::iterator itColumnArray = m_vecSubHeader.begin() + 1;
		while (m_vecSubHeader.end() != itColumnArray)
		{
			pColumn = m_rptSubFilePreview.AddColumn(new CXTPReportColumn(nColumnIndex, (*itColumnArray), 100));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (!bAutoSize)
				pColumn->SetBestFitMode(xtpColumnBestFitModeAllData);

			++nColumnIndex;
			++itColumnArray;
		}

		CXTPReportRecord* pRecord = nullptr;
		CXTPReportRecordItem* pItem = nullptr;

		int nRow(0);
		std::vector<ODData>::iterator itRows = m_vecSubODData.begin(), itRowsEnd = m_vecSubODData.end();
		if (m_bSubImportHeader)
			itRows++; // 첫번째 행은 컬럼명 이므로 패스
		for (; itRows != itRowsEnd && nRow < 200; ++itRows)
		{
			pRecord = m_rptSubFilePreview.AddRecord(new CXTPReportRecord());

			ODData& row = (*itRows);
			ODData::iterator itrow, itrowEnd = row.end();
			int nAddedCol = 0;
			for (itrow = row.begin(); itrow != itrowEnd && nAddedCol < nColumnIndex; ++itrow, ++nAddedCol)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(*itrow));
				pItem->SetAlignment(DT_CENTER);
			}

			if (nColumnIndex > nAddedCol)
			{
				for (int i = 0; i < (nColumnIndex - nAddedCol); i++)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItem);
				}
			}

			++nRow;
		}

		m_rptSubFilePreview.Populate();

		CXTPReportHeader* pHeader = m_rptSubFilePreview.GetReportHeader();
		CXTPReportColumns* pReportColumns = m_rptSubFilePreview.GetColumns();
		int                nReportColumns = pReportColumns->GetCount();
		for (int i = 0; i < nReportColumns; i++)
		{
			CXTPReportColumn* pReportColumn = pReportColumns->GetAt(i);
			if (!bAutoSize)
				pHeader->BestFit(pReportColumn);
		}
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

void KTripODToVehODDlg::UpdateODColumnReport()
{
	TxLogDebugStartMsg();

	try
	{
		if (!m_bMainODLoadFile && !m_bSubODLoadFile)
			return;

		std::vector <ColumnInfo> stColumnInfo;
		if (!GetColumnInfo(m_strRegion, m_strYear, stColumnInfo))
		{
			AfxMessageBox(_T("선택한 지역과 연도에 대한 컬럼 정보가 없습니다.\n설정 파일을 확인해 주세요."));
			return;
		}

		InitODColumn();

		CXTPReportColumn* pMainColumn = m_rptODColumn.GetColumns()->GetAt(2);
		CXTPReportColumn* pSubColumn = m_rptODColumn.GetColumns()->GetAt(3);

		if (m_bMainODLoadFile)
		{
			for (int i = 0; i < m_vecMainHeader.size(); i++)
			{
				pMainColumn->GetEditOptions()->AddConstraint(m_vecMainHeader[i], i);
			}
			pMainColumn->SetAlignment(DT_CENTER);
			pMainColumn->GetEditOptions()->m_bAllowEdit = TRUE;
			pMainColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
			pMainColumn->GetEditOptions()->AddComboButton(TRUE);
		}

		if (m_bSubODLoadFile)
		{
			for (int i = 0; i < m_vecSubHeader.size(); i++)
			{
				pSubColumn->GetEditOptions()->AddConstraint(m_vecSubHeader[i], i);
			}
			pSubColumn->SetAlignment(DT_CENTER);
			pSubColumn->GetEditOptions()->m_bAllowEdit = TRUE;
			pSubColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
			pSubColumn->GetEditOptions()->AddComboButton(TRUE);
		}	

		auto colStart = stColumnInfo.begin();
		auto colEnd = stColumnInfo.end();
		int nIdx = 0;
		CXTPReportRecord* pRecord = nullptr;

		for (; colStart != colEnd; ++colStart, nIdx++)
		{
			pRecord = m_rptODColumn.AddRecord(new CXTPReportRecord());

			pRecord->AddItem(new CXTPReportRecordItemText(colStart->name));
			if (nIdx < 2)
				pRecord->AddItem(new CXTPReportRecordItemText(_T("Integer")));
			else
				pRecord->AddItem(new CXTPReportRecordItemText(_T("Double")));

			if (m_bMainODLoadFile)
			{
				if (m_chkAutoColumn.GetCheck() == BST_CHECKED)
					pRecord->AddItem(new KColumnReportRecordItem(colStart->mainCol));
				else
					pRecord->AddItem(new KColumnReportRecordItem(nIdx + 1));
			}
			else
			{
				pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
			}
			if (m_bSubODLoadFile)
			{
				if (m_chkAutoColumn.GetCheck() == BST_CHECKED)
					pRecord->AddItem(new KColumnReportRecordItem(colStart->subCol));
				else
					pRecord->AddItem(new KColumnReportRecordItem(nIdx + 1));
			}
			else
			{
				pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
			}
		}

		m_rptODColumn.GetReportHeader()->AllowColumnRemove(FALSE);
		m_rptODColumn.FocusSubItems(TRUE);
		m_rptODColumn.Populate();
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

bool KTripODToVehODDlg::LoadMainODData()
{
	TxLogDebugStartMsg();
	m_vecMainODData.clear();

	try
	{
		// txt 파일
		if (m_eMainODFileType == Text)
		{
			CStdioFile file;
			if (!file.Open(m_strMainODFilePath, CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
				throw 1;

			CString strLine;

			bool bFirst = true;
			while (file.ReadString(strLine))
			{
				if (bFirst && m_bMainImportHeader)
				{
					bFirst = false;
					continue;
				}

				if (strLine.Trim().IsEmpty())
					continue;

				ODData rowData;
				int nPos = 0;

				CString strToken = strLine.Tokenize(_T(" \t,;"), nPos);

				while (!strToken.IsEmpty())
				{
					TCHAR ch = strToken.GetAt(0);
					if ((ch >= _T('0') && ch <= _T('9')) || ch == _T('.'))
					{
						rowData.push_back(strToken);
					}

					strToken = strLine.Tokenize(_T(" \t,;"), nPos);
				}

				if (!rowData.empty())
				{
					m_vecMainODData.push_back(rowData);
				}
			}

			file.Close();
		}
		// 엑셀 파일
		else
		{
			int sel = m_cboMainSelSheet.GetCurSel();
			if (sel == CB_ERR)
				throw 2;
			
			CString sheetName;
			m_cboMainSelSheet.GetLBText(sel, sheetName);

			int headerRow = m_cboMainSelHeader.GetCurSel() + 1;

			if (!m_excelHandler.ReadRowRangeFromExcel(m_strMainODFilePath, sheetName, headerRow, 30, m_vecMainHeader))
				throw 3;

			for (size_t i = 0; i < m_vecMainHeader.size(); i++)
			{
				if (m_vecMainHeader[i].IsEmpty())
					m_vecMainHeader[i].Format(_T("Column%d"), (int)(i + 1));
			}

			m_vecMainHeader.insert(m_vecMainHeader.begin(), _T("null"));

			if (!m_excelHandler.ReadExcelToODData(m_strMainODFilePath, sheetName, headerRow + 1, m_vecMainODData))
				throw 4;
		}
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
			strError.Format(_T("Error - Open File: %s"), m_strMainODFilePath);
		else if (ex == 2)
			strError.Format(_T("선택된 시트가 없습니다. %s"), m_strMainODFilePath);
		else if (ex == 3)
			strError.Format(_T("선택된 시트에 헤더를 읽어 오지 못했습니다. %s"), m_strMainODFilePath);
		else if (ex == 4)
			strError.Format(_T("OD 데이터를 읽어오지 못했습니다. %s"), m_strMainODFilePath);
		else
			strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		return false;
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
	TxLogDebugEndMsg();
}

bool KTripODToVehODDlg::LoadSubODData()
{
	TxLogDebugStartMsg();
	m_vecSubODData.clear();

	try
	{
		// txt 파일
		if (m_eSubODFileType == Text)
		{
			CStdioFile file;
			if (!file.Open(m_strSubODFilePath, CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
				throw 1;

			CString strLine;

			bool bFirst = true;
			while (file.ReadString(strLine))
			{
				if (bFirst && m_bSubImportHeader)
				{
					bFirst = false;
					continue;
				}

				if (strLine.Trim().IsEmpty())
					continue;

				ODData rowData;
				int nPos = 0;

				CString strToken = strLine.Tokenize(_T(" \t,;"), nPos);

				while (!strToken.IsEmpty())
				{
					TCHAR ch = strToken.GetAt(0);
					if ((ch >= _T('0') && ch <= _T('9')) || ch == _T('.'))
					{
						rowData.push_back(strToken);
					}

					strToken = strLine.Tokenize(_T(" \t,;"), nPos);
				}

				if (!rowData.empty())
				{
					m_vecSubODData.push_back(rowData);
				}
			}

			file.Close();
		}
		// 엑셀 파일
		else
		{
			int sel = m_cboSubSelSheet.GetCurSel();
			if (sel == CB_ERR)
				throw 2;

			CString sheetName;
			m_cboSubSelSheet.GetLBText(sel, sheetName);

			int headerRow = m_cboSubSelHeader.GetCurSel() + 1;

			if (!m_excelHandler.ReadRowRangeFromExcel(m_strSubODFilePath, sheetName, headerRow, 30, m_vecSubHeader))
				throw 3;

			for (size_t i = 0; i < m_vecSubHeader.size(); i++)
			{
				if (m_vecSubHeader[i].IsEmpty())
					m_vecSubHeader[i].Format(_T("Column%d"), (int)(i + 1));
			}

			m_vecSubHeader.insert(m_vecSubHeader.begin(), _T("null"));

			if (!m_excelHandler.ReadExcelToODData(m_strSubODFilePath, sheetName, headerRow + 1, m_vecSubODData))
				throw 4;
		}
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
			strError.Format(_T("Error - Open File: %s"), m_strSubODFilePath);
		else if (ex == 2)
			strError.Format(_T("선택된 시트가 없습니다. %s"), m_strSubODFilePath);
		else if (ex == 3)
			strError.Format(_T("선택된 시트에 헤더를 읽어 오지 못했습니다. %s"), m_strSubODFilePath);
		else if (ex == 4)
			strError.Format(_T("OD 데이터를 읽어오지 못했습니다. %s"), m_strSubODFilePath);
		else
			strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		return false;
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
	TxLogDebugEndMsg();
}

bool KTripODToVehODDlg::LoadExcelAutoFactorData()
{
	try
	{
		if (m_strRegion.IsEmpty() || m_strYear.IsEmpty())
		{
			AfxMessageBox(_T("분석지역 번호나 배포년도가 선택되지 않았습니다."));
			return false;
		}

		TCHAR szModulePath[MAX_PATH] = { 0 };
		GetModuleFileName(nullptr, szModulePath, MAX_PATH);

		CString strBasePath(szModulePath);
		int pos = strBasePath.ReverseFind(_T('\\'));
		if (pos != -1)
			strBasePath = strBasePath.Left(pos + 1);

		CString strDistFolder;
		strDistFolder.Format(_T("%sUtility\\TripODToVehOD\\죤체계_재차인원\\"), strBasePath);
		CString strExcelPath;
		strExcelPath.Format(_T("%s%s\\승용차재차인원_%s.xlsx"), strDistFolder, m_strYear, m_strYear);

		// 파일이 없으면 파일 선택 다이얼로그로 사용자에게 직접 선택하게 함
		if (GetFileAttributes(strDistFolder) == INVALID_FILE_ATTRIBUTES)
		{
			CFileDialog dlg(TRUE, _T("xlsx"), nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("Excel Files (*.xlsx)|*.xlsx||"), this);

			dlg.m_ofn.lpstrInitialDir = strDistFolder;
			dlg.m_ofn.lpstrTitle = _T("승용차 재차인원 배분 엑셀 파일 선택");

			if (dlg.DoModal() != IDOK)
				throw 1;

			strExcelPath = dlg.GetPathName();
		}

		UtilExcelHandler excel;
		ExcelColumnRequest reqA;
		ExcelColumnRequest reqB;
		ExcelColumnRequest reqC;

		reqA.colLetter = _T("A");   // O (출발)
		reqB.colLetter = _T("B");   // D (도착)
		reqC.colLetter = _T("C");   // Auto 값 (재차인원 비율)

		std::vector<ExcelColumnRequest*> requests;
		requests.push_back(&reqA);
		requests.push_back(&reqB);
		requests.push_back(&reqC);

		// 1행은 헤더(O, D, Auto) 이므로 2행부터 읽기
		if (!excel.ReadColumnsBatch(strExcelPath, m_strRegion, 2, requests))
		{
			throw 2;
		}

		m_mapAutoScaleFactor.clear();

		size_t rowCount = min(reqA.data.size(), min(reqB.data.size(), reqC.data.size()));

		for (size_t i = 0; i < rowCount; ++i)
		{
			CString sA = reqA.data[i];
			CString sB = reqB.data[i];
			CString sC = reqC.data[i];

			sA.Trim(); sB.Trim(); sC.Trim();

			if (sA.IsEmpty() || sB.IsEmpty() || sC.IsEmpty())
				continue;

			int origin = _ttoi(sA);
			int dest = _ttoi(sB);
			double val = _tstof(sC);

			if ((origin == 0 && sA.CompareNoCase(_T("0")) != 0) || (dest == 0 && sB.CompareNoCase(_T("0")) != 0))
			{
				continue;
			}

			ODKey key(origin, dest);
			m_mapAutoScaleFactor[key] = val;   // 중복 키가 있으면 마지막 값으로 덮어씀
		}

		if (m_mapAutoScaleFactor.empty())
		{
			throw 3;
		}

		return true;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("승용차 재차인원 배분 엑셀 파일을 여는데 실패했습니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 2)
		{
			strError.Format(_T("승용차 재차인원 엑셀 파일 시트를 읽어오는데 실패했습니다.\n시트 데이터를 확인해 주세요."));
			AfxMessageBox(strError);
		}
		else if (ex == 3)
		{
			strError.Format(_T("승용차 재차인원 엑셀에서 재차인원 데이터를 읽지 못했습니다.\n엑셀 파일 형식을 확인해 주세요."));
			AfxMessageBox(strError);
		}
		else
		{
			strError.Format(_T("Error : %d"), ex);
		}

		TxLogDebug((LPCTSTR)strError);
		return false;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		AfxMessageBox(_T("승용차 재차인원 기준 파일을 읽는데 실패하였습니다."));
		TxLogDebugException();
		return false;
	}
}

bool KTripODToVehODDlg::LoadExcelAccessFactorData()
{
	try
	{
		if (m_strRegion.IsEmpty() || m_strYear.IsEmpty())
		{
			AfxMessageBox(_T("분석지역 번호나 배포년도가 선택되지 않았습니다."));
			return false;
		}

		TCHAR szModulePath[MAX_PATH] = { 0 };
		GetModuleFileName(nullptr, szModulePath, MAX_PATH);

		CString strBasePath(szModulePath);
		int pos = strBasePath.ReverseFind(_T('\\'));
		if (pos != -1)
			strBasePath = strBasePath.Left(pos + 1);

		CString strDistFolder;
		strDistFolder.Format(_T("%sUtility\\TripODToVehOD\\죤체계_재차인원\\"), strBasePath);
		CString strExcelPath;
		strExcelPath.Format(_T("%s%s\\기타수단재차인원_%s.xlsx"), strDistFolder, m_strYear, m_strYear);

		// 파일이 없으면 파일 선택 다이얼로그로 사용자에게 직접 선택하게 함
		if (GetFileAttributes(strDistFolder) == INVALID_FILE_ATTRIBUTES)
		{
			CFileDialog dlg(TRUE, _T("xlsx"), nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("Excel Files (*.xlsx)|*.xlsx||"), this);

			dlg.m_ofn.lpstrInitialDir = strDistFolder;
			dlg.m_ofn.lpstrTitle = _T("기타수단 재차인원 배분 엑셀 파일 선택");

			if (dlg.DoModal() != IDOK)
				throw 1;

			strExcelPath = dlg.GetPathName();
		}

		UtilExcelHandler excel;
		ExcelColumnRequest reqA;
		ExcelColumnRequest reqB;
		ExcelColumnRequest reqC;
		ExcelColumnRequest reqD;
		ExcelColumnRequest reqE;

		reqA.colLetter = _T("A");   // O (출발)
		reqB.colLetter = _T("B");   // Bus_전국권
		reqC.colLetter = _T("C");   // OtherBus
		reqD.colLetter = _T("D");	// Taxi
		reqE.colLetter = _T("E");	// Bus_수도권

		std::vector<ExcelColumnRequest*> requests;
		requests.push_back(&reqA);
		requests.push_back(&reqB);
		requests.push_back(&reqC);
		requests.push_back(&reqD);
		requests.push_back(&reqE);

		// 1행은 헤더(O, D, Auto) 이므로 2행부터 읽기
		if (!excel.ReadColumnsBatch(strExcelPath, m_strYear, 2, requests))
		{
			throw 2;
		}

		m_mapAccessScaleFactor.clear();

		size_t rowCount = min(reqA.data.size(), min(reqB.data.size(), reqC.data.size()));

		for (size_t i = 0; i < rowCount; ++i)
		{
			CString sA = reqA.data[i];
			CString sB = reqB.data[i];
			CString sC = reqC.data[i];
			CString sD = reqD.data[i];
			CString sE = reqE.data[i];

			sA.Trim(); sB.Trim(); sC.Trim(); sD.Trim(); sE.Trim();

			if (sA.IsEmpty() || sB.IsEmpty() || sC.IsEmpty() || sD.IsEmpty() || sE.IsEmpty())
				continue;

			int origin = _ttoi(sA);
			AccessFactorType factorData;
			factorData.nationalBus = _tstof(sB);
			factorData.otherBus = _tstof(sC);
			factorData.taxi = _tstof(sD);
			factorData.capitalBus = _tstof(sE);

			if ((origin == 0 && sA.CompareNoCase(_T("0")) != 0))
			{
				continue;
			}

			m_mapAccessScaleFactor[origin] = factorData;
		}

		if (m_mapAccessScaleFactor.empty())
		{
			throw 3;
		}

		return true;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("기타수단 재차인원 배분 엑셀 파일을 여는데 실패했습니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 2)
		{
			strError.Format(_T("기타수단 재차인원 엑셀 파일 시트를 읽어오는데 실패했습니다.\n시트 데이터를 확인해 주세요."));
			AfxMessageBox(strError);
		}
		else if (ex == 3)
		{
			strError.Format(_T("기타수단 재차인원 엑셀에서 재차인원 데이터를 읽지 못했습니다.\n엑셀 파일 형식을 확인해 주세요."));
			AfxMessageBox(strError);
		}
		else
		{
			strError.Format(_T("Error : %d"), ex);
		}

		TxLogDebug((LPCTSTR)strError);
		return false;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		AfxMessageBox(_T("기타수단 재차인원 기준 파일을 읽는데 실패하였습니다."));
		TxLogDebugException();
		return false;
	}
}

bool KTripODToVehODDlg::GetAutoScaleFactorValue(int nO, int nD, double& dValue)
{
	ODKey key(nO, nD);
	auto it = m_mapAutoScaleFactor.find(key);

	if (it == m_mapAutoScaleFactor.end())
		return false;

	dValue = it->second;
	return true;
}

bool KTripODToVehODDlg::GetAccessScaleFactorValue(int nO, AccessFactorType& factorData)
{
	auto it = m_mapAccessScaleFactor.find(nO);

	if (it == m_mapAccessScaleFactor.end())
		return false;

	factorData = it->second;
	return true;
}

bool KTripODToVehODDlg::GetScaleFactorValue(int nO, int nD, int nCol, double& dValue, bool bIsMain)
{
	if (m_curColumnInfo.size() <= 0)
		return false;

	if (nCol == 2 || nCol == 4)
	{
		return GetAutoScaleFactorValue(nO, nD, dValue);
	}
	else if (nCol == 3 || nCol == 5 || nCol == 6)
	{
		AccessFactorType factorData;
		if (!GetAccessScaleFactorValue(nO, factorData))
			return false;

		if (nCol == 3)
			dValue = factorData.taxi;
		else if (nCol == 5 && m_strRegion == "전국권")
			dValue = factorData.nationalBus;
		else if (nCol == 5 && m_strRegion == "수도권")
			dValue = factorData.capitalBus;
		else if (nCol == 5 && m_strRegion == "5대권역")
			dValue = factorData.capitalBus;
		else if (nCol == 6)
			dValue = factorData.otherBus;
	}

	return true;
}

void KTripODToVehODDlg::SetCurColumnInfo()
{
	try
	{
		if (m_strRegion.IsEmpty() || m_strYear.IsEmpty())
		{
			throw 1;
		}

		auto itRegion = m_AreaInfoMap.find(m_strRegion);
		if (itRegion == m_AreaInfoMap.end())
			throw 2;

		auto itYear = itRegion->second.find(m_strYear);
		if (itYear == itRegion->second.end())
			throw 3;

		m_curColumnInfo = itYear->second;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("선택된 지역이나 연도가 없습니다.."));
			AfxMessageBox(strError);
		}
		else if (ex == 2)
		{
			strError.Format(_T("현재 선택된 지역의 Column 데이터가 존재하지 않습니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 3)
		{
			strError.Format(_T("현재 선택된 연도의 Column 데이터가 존재하지 않습니다."));
			AfxMessageBox(strError);
		}
		else
		{
			strError.Format(_T("Error : %d"), ex);
		}

		TxLogDebug((LPCTSTR)strError);
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

bool KTripODToVehODDlg::BuildMainODDataSet()
{
	if (!m_bMainODLoadFile)
		return true;

	m_vecMainOD_O.clear();
	m_vecMainOD_D.clear();
	m_vecMainODDataSet.clear();

	try
	{
		CXTPReportRecords* pRecords = m_rptODColumn.GetRecords();
		int nTotalRows = (int)pRecords->GetCount();
		int nDataColCount = nTotalRows - 2;

		std::vector<int> vecMainIdx(nTotalRows, -1);
		for (int i = 0; i < nTotalRows; i++)
		{
			KColumnReportRecordItem* pItem = (KColumnReportRecordItem*)pRecords->GetAt(i)->GetItem(2);
			if (pItem)
				vecMainIdx[i] = pItem->m_nIndex - 1;
		}

		m_vecMainODDataSet.resize(nDataColCount);

		auto itRow = m_vecMainODData.begin();
		if (m_bMainImportHeader && itRow != m_vecMainODData.end())
			itRow++;

		for (; itRow != m_vecMainODData.end(); ++itRow)
		{
			const ODData& row = *itRow;
			int nO = _ttoi(row[vecMainIdx[0]]);
			int nD = _ttoi(row[vecMainIdx[1]]);

			m_vecMainOD_O.push_back(nO);
			m_vecMainOD_D.push_back(nD);

			for (int ds = 0; ds < nDataColCount; ds++)
			{
				double dVal = 0.0;
				double dFactor = 1.0;
				int colIdx = vecMainIdx[ds + 2];

				if (colIdx == -1)
				{
					m_vecMainODDataSet[ds].push_back(dVal);
					continue;
				}
				if (colIdx >= 0 && colIdx < (int)row.size())
				{
					dVal = _tstof(row[colIdx]);
				}
				if (!GetScaleFactorValue(nO, nD, ds + 2, dFactor, true))
				{
					dFactor = 1.0;
				}

				m_vecMainODDataSet[ds].push_back(dVal / dFactor);
			}
		}

		return true;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);

		TxLogDebug((LPCTSTR)strError);
		return false;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		AfxMessageBox(_T("재차인원 계산 적용에 실패하였습니다."));
		TxLogDebugException();
		return false;
	}
}

bool KTripODToVehODDlg::BuildSubODDataSet()
{
	if (!m_bSubODLoadFile)
		return true;

	m_vecSubOD_O.clear();
	m_vecSubOD_D.clear();
	m_vecSubODDataSet.clear();

	try
	{
		CXTPReportRecords* pRecords = m_rptODColumn.GetRecords();
		int nTotalRows = (int)pRecords->GetCount();
		int nDataColCount = nTotalRows - 2;

		std::vector<int> vecSubIdx(nTotalRows, -1);
		for (int i = 0; i < nTotalRows; i++)
		{
			KColumnReportRecordItem* pItem = (KColumnReportRecordItem*)pRecords->GetAt(i)->GetItem(3);
			if (pItem)
				vecSubIdx[i] = pItem->m_nIndex - 1;
		}

		m_vecSubODDataSet.resize(nDataColCount);

		auto itRow = m_vecSubODData.begin();
		if (m_bSubImportHeader && itRow != m_vecSubODData.end())
			itRow++;

		for (; itRow != m_vecSubODData.end(); ++itRow)
		{
			const ODData& row = *itRow;
			int nO = _ttoi(row[vecSubIdx[0]]);
			int nD = _ttoi(row[vecSubIdx[1]]);

			m_vecSubOD_O.push_back(nO);
			m_vecSubOD_D.push_back(nD);

			for (int ds = 0; ds < nDataColCount; ds++)
			{
				double dVal = 0.0;
				double dFactor = 1.0;
				int colIdx = vecSubIdx[ds + 2];

				if (colIdx == -1)
				{
					m_vecSubODDataSet[ds].push_back(dVal);
					continue;
				}
				if (colIdx >= 0 && colIdx < (int)row.size())
				{
					dVal = _tstof(row[colIdx]);
				}
				if (!GetScaleFactorValue(nO, nD, ds + 2, dFactor, false))
				{
					dFactor = 1.0;
				}

				m_vecSubODDataSet[ds].push_back(dVal / dFactor);
			}
		}

		return true;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);

		TxLogDebug((LPCTSTR)strError);
		return false;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		AfxMessageBox(_T("재차인원 계산 적용에 실패하였습니다."));
		TxLogDebugException();
		return false;
	}
}

bool KTripODToVehODDlg::ExportMainVehODFile()
{
	if (!m_bMainODLoadFile)
		return true;

	try
	{
		const int nRowCnt = (int)m_vecMainOD_O.size();
		if (nRowCnt == 0 || (int)m_vecMainOD_D.size() != nRowCnt)
			throw 1;

		const int nDataSetCnt = (int)m_vecMainODDataSet.size();
		for (int ds = 0; ds < nDataSetCnt; ds++)
		{
			if ((int)m_vecMainODDataSet[ds].size() != nRowCnt)
				throw 2;
		}

		CString strOutPath = m_strMainODFilePath;

		int posSlash = strOutPath.ReverseFind(_T('\\'));
		int posDot = strOutPath.ReverseFind(_T('.'));

		if (posDot == -1 || (posSlash != -1 && posDot < posSlash))
		{
			strOutPath += _T("_Veh.txt");
		}
		else
		{
			CString prefix = strOutPath.Left(posDot);
			strOutPath.Format(_T("%s_Veh.txt"), prefix.GetString());
		}

		CFile file;
		if (!file.Open(strOutPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
		{
			throw 3;
		}

		// 버퍼 설정 (예: 64KB ~ 1MB 정도)
		const int BUF_SIZE = 1024 * 1024; // 1MB 버퍼
		std::vector<char> buffer(BUF_SIZE);
		char* pBuffer = buffer.data();
		int nBufOffset = 0;

		// ANSI
		//UINT nCodePage = CP_ACP;
		
		// UTF-8
		UINT nCodePage = CP_UTF8;
		unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
		memcpy(pBuffer + nBufOffset, bom, 3);
		nBufOffset += 3;

		CStringA strTmp;

		// Header 삽입
		for (const auto& info : m_curColumnInfo)
		{
			CW2A pszName(info.name, nCodePage);
			CStringA strItem;
			strItem.Format("%s\t", (LPCSTR)pszName);	// 탭 구분자 사용
			strTmp += strItem;
		}

		strTmp += "\r\n";
		int nLen = strTmp.GetLength();
		memcpy(pBuffer + nBufOffset, (LPCSTR)strTmp, nLen);
		nBufOffset += nLen;

		for (int row = 0; row < nRowCnt; ++row)
		{
			// O, D 먼저
			strTmp.Format("%d\t%d", m_vecMainOD_O[row], m_vecMainOD_D[row]);

			// 그 뒤에 각 데이터셋 값들 이어 붙이기
			for (int ds = 0; ds < nDataSetCnt; ++ds)
			{
				CString strVal;
				// g 서식: 유효자릿수 기반. 필요하면 %.6f 등으로 변경 가능
				strVal.Format(_T("\t%.5g"), m_vecMainODDataSet[ds][row]);
				strTmp += strVal;
			}

			strTmp += "\r\n";

			int nLen = strTmp.GetLength();
			// 버퍼가 꽉 차면 파일에 쓰기
			if (nBufOffset + nLen >= BUF_SIZE)
			{
				file.Write(pBuffer, nBufOffset);
				nBufOffset = 0;
			}

			// 버퍼에 데이터 추가
			memcpy(pBuffer + nBufOffset, (LPCSTR)strTmp, nLen);
			nBufOffset += nLen;
		}

		// 남은 버퍼 쓰기
		if (nBufOffset > 0)
		{
			file.Write(pBuffer, nBufOffset);
		}

		file.Close();
		return true;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("주수단 OD 데이터가 없거나 O/D 행 수가 불일치 합니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 2)
		{
			strError.Format(_T("주수단데이터셋의 행 수가 주수단 O/D 행 수와 일치하지 않습니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 3)
		{
			strError.Format(_T("주수단 재차인원 파일을 생성할 수 없습니다."));
			AfxMessageBox(strError);
		}
		else
		{
			strError.Format(_T("Error : %d"), ex);
		}

		TxLogDebug((LPCTSTR)strError);
		return false;
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
}

bool KTripODToVehODDlg::ExportSubVehODFile()
{
	if (!m_bSubODLoadFile)
		return true;

	try
	{
		const int nRowCnt = (int)m_vecSubOD_O.size();
		if (nRowCnt == 0 || (int)m_vecSubOD_D.size() != nRowCnt)
			throw 1;

		const int nDataSetCnt = (int)m_vecSubODDataSet.size();
		for (int ds = 0; ds < nDataSetCnt; ds++)
		{
			if ((int)m_vecSubODDataSet[ds].size() != nRowCnt)
				throw 2;
		}

		CString strOutPath = m_strSubODFilePath;

		int posSlash = strOutPath.ReverseFind(_T('\\'));
		int posDot = strOutPath.ReverseFind(_T('.'));

		if (posDot == -1 || (posSlash != -1 && posDot < posSlash))
		{
			strOutPath += _T("_Veh.txt");
		}
		else
		{
			CString prefix = strOutPath.Left(posDot);
			strOutPath.Format(_T("%s_Veh.txt"), prefix.GetString());
		}

		CFile file;
		if (!file.Open(strOutPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
		{
			throw 3;
		}

		// 버퍼 설정 (예: 64KB ~ 1MB 정도)
		const int BUF_SIZE = 1024 * 1024; // 1MB 버퍼
		std::vector<char> buffer(BUF_SIZE);
		char* pBuffer = buffer.data();
		int nBufOffset = 0;

		// ANSI
		//UINT nCodePage = CP_ACP;

		// UTF-8
		UINT nCodePage = CP_UTF8;
		unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
		memcpy(pBuffer + nBufOffset, bom, 3);
		nBufOffset += 3;

		CStringA strTmp;

		// Header 삽입
		for (const auto& info : m_curColumnInfo)
		{
			CW2A pszName(info.name, nCodePage);
			CStringA strItem;
			strItem.Format("%s\t", (LPCSTR)pszName);	// 탭 구분자 사용
			strTmp += strItem;
		}

		strTmp += "\r\n";
		int nLen = strTmp.GetLength();
		memcpy(pBuffer + nBufOffset, (LPCSTR)strTmp, nLen);
		nBufOffset += nLen;

		for (int row = 0; row < nRowCnt; ++row)
		{
			// O, D 먼저
			strTmp.Format("%d\t%d", m_vecSubOD_O[row], m_vecSubOD_D[row]);

			// 그 뒤에 각 데이터셋 값들 이어 붙이기
			for (int ds = 0; ds < nDataSetCnt; ++ds)
			{
				CString strVal;
				// g 서식: 유효자릿수 기반. 필요하면 %.6f 등으로 변경 가능
				strVal.Format(_T("\t%.5g"), m_vecSubODDataSet[ds][row]);
				strTmp += strVal;
			}

			strTmp += "\r\n";

			int nLen = strTmp.GetLength();
			// 버퍼가 꽉 차면 파일에 쓰기
			if (nBufOffset + nLen >= BUF_SIZE)
			{
				file.Write(pBuffer, nBufOffset);
				nBufOffset = 0;
			}

			// 버퍼에 데이터 추가
			memcpy(pBuffer + nBufOffset, (LPCSTR)strTmp, nLen);
			nBufOffset += nLen;
		}

		// 남은 버퍼 쓰기
		if (nBufOffset > 0)
		{
			file.Write(pBuffer, nBufOffset);
		}

		file.Close();

		return true;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("보조수단 OD 데이터가 없거나 O/D 행 수가 불일치 합니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 2)
		{
			strError.Format(_T("보조수단데이터셋의 행 수가 주수단 O/D 행 수와 일치하지 않습니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 3)
		{
			strError.Format(_T("보조수단 재차인원 파일을 생성할 수 없습니다."));
			AfxMessageBox(strError);
		}
		else
		{
			strError.Format(_T("Error : %d"), ex);
		}

		TxLogDebug((LPCTSTR)strError);
		return false;
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
}

void KTripODToVehODDlg::DrawingLoadingGif(bool bStart)
{
	if (bStart && m_bIsLoading == false)
	{
		m_bIsLoading = true;
		m_loadingGif.ShowWindow(SW_SHOW);
		m_loadingGif.Draw();
	}
	else if (!bStart && m_bIsLoading)
	{
		m_bIsLoading = false;
		m_loadingGif.ShowWindow(SW_HIDE);
		m_loadingGif.Stop();
	}
}

UINT KTripODToVehODDlg::ThreadWorker(LPVOID pParam)
{
	KTripODToVehODDlg* pDlg = (KTripODToVehODDlg*)pParam;

	if (pDlg == nullptr)
		return 1;

	bool bSuccess = true;

	if (!pDlg->LoadExcelAutoFactorData())
		bSuccess = false;
	if (pDlg->m_bAbortThread)
		bSuccess = false;
	if (bSuccess && !pDlg->LoadExcelAccessFactorData())
		bSuccess = false;
	if (pDlg->m_bAbortThread)
		bSuccess = false;
	if (bSuccess && !pDlg->BuildMainODDataSet())
		bSuccess = false;
	if (pDlg->m_bAbortThread)
		bSuccess = false;
	if (bSuccess && !pDlg->BuildSubODDataSet())
		bSuccess = false;
	if (pDlg->m_bAbortThread)
		bSuccess = false;
	if (bSuccess && !pDlg->ExportMainVehODFile())
		bSuccess = false;
	if (pDlg->m_bAbortThread)
		bSuccess = false;
	if (bSuccess && !pDlg->ExportSubVehODFile())
		bSuccess = false;
	if (pDlg->m_bAbortThread)
		bSuccess = false;

	CString msg;
	msg.Format(_T("Veh OD 파일을 생성했습니다."));
	AfxMessageBox(msg);

	::PostMessage(pDlg->m_hWnd, WM_THREAD_FINISHED, (WPARAM)bSuccess, 0);

	return 0;
}

UINT KTripODToVehODDlg::ThreadFileLoading(LPVOID pParam)
{
	KTripODToVehODDlg* pDlg = (KTripODToVehODDlg*)pParam;

	if (pDlg == nullptr)
		return 1;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	pDlg->m_excelHandler.m_pbAbort = &pDlg->m_bAbortThread;

	try
	{
		if (pDlg->m_bMainODFileLoading)
		{
			pDlg->m_bMainODFileLoading = false;
			if (pDlg->m_eMainODFileType == Text)
			{
				pDlg->CheckImportHeader(pDlg->m_strMainODFilePath, true);
			}
			else
			{
				pDlg->m_bMainImportHeader = false;
				pDlg->LoadSheetList(pDlg->m_strMainODFilePath, true);
			}

			if (pDlg->m_bAbortThread)
				throw 0;

			pDlg->m_bMainODLoadFile = pDlg->LoadMainODData();
			::PostMessage(pDlg->m_hWnd, WM_UPDATE_MAIN_REPORT, (WPARAM)pDlg->m_bMainODLoadFile, 0);
		}
		if (pDlg->m_bSubODFileLoading)
		{
			pDlg->m_bSubODFileLoading = false;
			if (pDlg->m_eSubODFileType == Text)
			{
				pDlg->CheckImportHeader(pDlg->m_strSubODFilePath, false);
			}
			else
			{
				pDlg->m_bSubImportHeader = false;
				pDlg->LoadSheetList(pDlg->m_strSubODFilePath, false);
			}

			if (pDlg->m_bAbortThread)
				throw 0;

			pDlg->m_bSubODLoadFile = pDlg->LoadSubODData();
			::PostMessage(pDlg->m_hWnd, WM_UPDATE_SUB_REPORT, (WPARAM)pDlg->m_bSubODLoadFile, 0);
		}
	}
	catch (...)
	{

	}

	CoUninitialize();

	return 0;
}

UINT KTripODToVehODDlg::ThreadExcelLoading(LPVOID pParam)
{
	KTripODToVehODDlg* pDlg = (KTripODToVehODDlg*)pParam;

	if (pDlg == nullptr)
		return 1;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	pDlg->m_excelHandler.m_pbAbort = &pDlg->m_bAbortThread;

	try
	{
		if (pDlg->m_bMainODFileLoading)
		{
			pDlg->m_bMainODFileLoading = false;
			pDlg->m_bMainODLoadFile = pDlg->LoadMainODData();

			if (pDlg->m_bAbortThread)
				throw 0;

			::PostMessage(pDlg->m_hWnd, WM_UPDATE_MAIN_REPORT, (WPARAM)pDlg->m_bMainODLoadFile, 0);
		}
		else
		{
			pDlg->m_bSubODFileLoading = false;
			pDlg->m_bSubODLoadFile = pDlg->LoadSubODData();

			if (pDlg->m_bAbortThread)
				throw 0;

			::PostMessage(pDlg->m_hWnd, WM_UPDATE_SUB_REPORT, (WPARAM)pDlg->m_bSubODLoadFile, 0);
		}
	}
	catch (...)
	{

	}

	CoUninitialize();

	return 0;
}

void KTripODToVehODDlg::OnCbnSelchangeCombo3()
{
	GetDlgItem(ID_RUN)->EnableWindow(FALSE);
	m_nActiveThreadCount++;
	m_bMainODFileLoading = true;

	DrawingLoadingGif(true);
	m_cboMainSelSheet.EnableWindow(FALSE);
	m_cboMainSelHeader.EnableWindow(FALSE);
	AfxBeginThread(ThreadExcelLoading, this);
}

void KTripODToVehODDlg::OnCbnSelchangeCombo5()
{
	GetDlgItem(ID_RUN)->EnableWindow(FALSE);
	m_nActiveThreadCount++;
	m_bSubODFileLoading = true;

	DrawingLoadingGif(true);
	m_cboSubSelSheet.EnableWindow(FALSE);
	m_cboSubSelHeader.EnableWindow(FALSE);
	AfxBeginThread(ThreadExcelLoading, this);
}

void KTripODToVehODDlg::OnCbnSelchangeCombo4()
{
	GetDlgItem(ID_RUN)->EnableWindow(FALSE);
	m_nActiveThreadCount++;
	m_bMainODFileLoading = true;

	DrawingLoadingGif(true);
	m_cboMainSelSheet.EnableWindow(FALSE);
	m_cboMainSelHeader.EnableWindow(FALSE);
	AfxBeginThread(ThreadExcelLoading, this);
}

void KTripODToVehODDlg::OnCbnSelchangeCombo6()
{
	GetDlgItem(ID_RUN)->EnableWindow(FALSE);
	m_nActiveThreadCount++;
	m_bSubODFileLoading = true;

	DrawingLoadingGif(true);
	m_cboSubSelSheet.EnableWindow(FALSE);
	m_cboSubSelHeader.EnableWindow(FALSE);
	AfxBeginThread(ThreadExcelLoading, this);
}

LRESULT KTripODToVehODDlg::OnUpdateMainReport(WPARAM wParam, LPARAM lParam)
{
	UpdateMainPreviewReport();
	UpdateODColumnReport();

	if (m_eMainODFileType == Text)
	{
		m_cboMainSelSheet.EnableWindow(FALSE);
		m_cboMainSelHeader.EnableWindow(FALSE);
	}
	else
	{
		m_cboMainSelSheet.EnableWindow(TRUE);
		m_cboMainSelHeader.EnableWindow(TRUE);
	}

	::PostMessage(m_hWnd, WM_THREAD_FINISHED, wParam, 0);
	return 0;
}

LRESULT KTripODToVehODDlg::OnUpdateSubReport(WPARAM wParam, LPARAM lParam)
{
	UpdateSubPreviewReport();
	UpdateODColumnReport();

	if (m_eSubODFileType == Text)
	{
		m_cboSubSelSheet.EnableWindow(FALSE);
		m_cboSubSelHeader.EnableWindow(FALSE);
	}
	else
	{
		m_cboSubSelSheet.EnableWindow(TRUE);
		m_cboSubSelHeader.EnableWindow(TRUE);
	}

	::PostMessage(m_hWnd, WM_THREAD_FINISHED, wParam, 0);
	return 0;
}

LRESULT KTripODToVehODDlg::OnThreadFinished(WPARAM wParam, LPARAM lParam)
{
	m_nActiveThreadCount--;

	if (m_nActiveThreadCount <= 0)
	{
		m_nActiveThreadCount = 0;
		DrawingLoadingGif(false);
		GetDlgItem(ID_RUN)->EnableWindow(TRUE);
	}

	bool bSuccess = (bool)wParam;

	return 0;
}