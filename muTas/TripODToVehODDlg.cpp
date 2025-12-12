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

IMPLEMENT_DYNAMIC(KTripODToVehODDlg, CDialogEx)

KTripODToVehODDlg::KTripODToVehODDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_7000_UTIL_TripODtoVehOD, pParent)
{
	m_mapSeparator.clear(); {
		TSeparator oTSeparator;
		oTSeparator.nCode = 0;
		oTSeparator.tcSeparator = _T(',');
		oTSeparator.strDispName = _T("Comma(,)");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));

		oTSeparator.nCode = 1;
		oTSeparator.tcSeparator = _T(';');
		oTSeparator.strDispName = _T("Semicolon(;)");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));

		oTSeparator.nCode = 2;
		oTSeparator.tcSeparator = _T('|');
		oTSeparator.strDispName = _T("Pipe(|)");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));

		oTSeparator.nCode = 3;
		oTSeparator.tcSeparator = _T('	');
		oTSeparator.strDispName = _T("Tab");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));

		oTSeparator.nCode = 4;
		oTSeparator.tcSeparator = _T(' ');
		oTSeparator.strDispName = _T("Space");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));
	}
}

KTripODToVehODDlg::~KTripODToVehODDlg()
{
}

void KTripODToVehODDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboAreaNumber);
	DDX_Control(pDX, IDC_COMBO2, m_cboDistributionYear);
	DDX_Control(pDX, IDC_EDITOR1, m_editODFilePath);
	DDX_Control(pDX, IDC_REPORT1, m_rptODColumn);
	DDX_Control(pDX, IDC_REPORT2, m_rptFileColumn);
	DDX_Control(pDX, IDC_REPORT3, m_rptFilePreview);
	DDX_Control(pDX, IDC_COMBO3, m_cboSeparator);
	DDX_Control(pDX, IDC_COMBO4, m_cboEncoding);
	DDX_Control(pDX, IDC_CHECK4, m_chkAutoColumn);
	DDX_Control(pDX, IDC_STCANIMATE, m_loadingGif);
}


BEGIN_MESSAGE_MAP(KTripODToVehODDlg, CDialogEx)
	ON_BN_CLICKED(ID_RUN, &KTripODToVehODDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BTN_ODFILE, &KTripODToVehODDlg::OnBnClickedBtnOdfile)
	ON_BN_CLICKED(IDC_CHECK4, &KTripODToVehODDlg::OnBnClickedCheck4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KTripODToVehODDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KTripODToVehODDlg::OnCbnSelchangeCombo2)
	ON_NOTIFY(XTP_NM_REPORT_RECORDS_DROPPED, IDC_REPORT1, &KTripODToVehODDlg::OnReportRecordsDropped)
	ON_BN_CLICKED(IDC_BTN_CONVERT_EXCEL, &KTripODToVehODDlg::OnBnClickedBtnConvertExcel)
	ON_MESSAGE(WM_THREAD_FINISHED, &KTripODToVehODDlg::OnThreadFinished)
END_MESSAGE_MAP()


// KTripODToVehODDlg 메시지 처리기

BOOL KTripODToVehODDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	m_loadingGif.Load(MAKEINTRESOURCE(IDR_PROGRESSIMAGE), _T("GIF"));
	DrawingLoadingGif(false);

	for (int i = 0; i < UTIL_TRIPOD_TO_VEHOD_AREA_CNT; i++)
	{
		m_cboAreaNumber.InsertString(i, UTIL_TRIPOD_TO_VEHOD_AREA[i]);
	}

	LoadDistributionList();
	InitComboSeparator();
	InitComboEncoding();

	QBicReportCtrlSetting::Default(m_rptODColumn, TRUE, FALSE, TRUE);
	QBicReportCtrlSetting::Default(m_rptFileColumn, TRUE);
	QBicReportCtrlSetting::Default(m_rptFilePreview, FALSE, FALSE, FALSE);

	m_rptODColumn.EnableDragDrop(_T("TripOD_DragDrop"), xtpReportAllowDrop, xtpReportDropSelect);
	m_rptFileColumn.EnableDragDrop(_T("TripOD_DragDrop"), xtpReportAllowDragCopy, xtpReportDropSelect);

	InitReportColumn();
	LoadConfigFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTripODToVehODDlg::OnBnClickedRun()
{
	if (!m_bLoadFile)
	{
		AfxMessageBox(_T("OD 파일이 선택되지 않았습니다."));
		return;
	}

	m_tcCachedSeparator = GetSelectedSeparator();
	m_nCachedEncoding = GetSelectedEncoding();

	DrawingLoadingGif(true);

	GetDlgItem(ID_RUN)->EnableWindow(FALSE);

	AfxBeginThread(ThreadWorker, this);
}

void KTripODToVehODDlg::LoadDistributionList()
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

		m_cboDistributionYear.ResetContent();

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
				m_cboDistributionYear.AddString(fileName);
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

void KTripODToVehODDlg::InitComboSeparator()
{
	try
	{
		m_cboSeparator.ResetContent();

		auto iter = m_mapSeparator.begin();
		auto itEnd = m_mapSeparator.end();

		for (; iter != itEnd; ++iter)
		{
			const int& nCode = iter->first;
			TSeparator& oTSeparator = iter->second;

			int nCur = m_cboSeparator.AddString(oTSeparator.strDispName);
			m_cboSeparator.SetItemData(nCur, (DWORD_PTR)oTSeparator.nCode);
		}

		m_cboSeparator.SetCurSel(3);
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

void KTripODToVehODDlg::InitComboEncoding()
{
	try
	{
		m_cboEncoding.ResetContent();
		int nCur = m_cboEncoding.AddString(_T("ANSI"));
		m_cboEncoding.SetItemData(nCur, (DWORD_PTR)1);

		nCur = m_cboEncoding.AddString(_T("UTF-8"));
		m_cboEncoding.SetItemData(nCur, (DWORD_PTR)2);

		m_cboEncoding.SetCurSel(0);
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

void KTripODToVehODDlg::InitReportColumn()
{
	TxLogDebugStartMsg();

	try
	{
		m_rptODColumn.ResetContent();
		m_rptODColumn.GetColumns()->Clear();
		m_rptODColumn.Populate();

		// 재차인원 적용 컬럼
		// No
		CXTPReportColumn* pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(0, _T("No"), 10));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);
		
		// OD Column
		pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(1, _T("OD Column"), 36));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);

		// File Column
		pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(2, _T("File Column"), 39));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);

		// 파일 컬럼 리스트
		pCol = m_rptFileColumn.AddColumn(new CXTPReportColumn(0, _T("File Column List"), 50));
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

void KTripODToVehODDlg::InitODColumn()
{
	TxLogDebugStartMsg();

	try
	{
		m_rptODColumn.ResetContent();
		m_rptODColumn.GetColumns()->Clear();
		m_rptODColumn.Populate();

		// 재차인원 적용 컬럼
		// No
		CXTPReportColumn* pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(0, _T("No"), 10));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);

		// OD Column
		pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(1, _T("OD Column"), 36));
		pCol->GetEditOptions()->m_bAllowEdit = FALSE;
		pCol->SetHeaderAlignment(DT_CENTER);
		pCol->SetAlignment(DT_CENTER);

		// File Column
		pCol = m_rptODColumn.AddColumn(new CXTPReportColumn(2, _T("File Column"), 39));
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
					info.index = col["col"];
					std::string sApplyName = col["apply"];
					info.apply = CA2T(sApplyName.c_str(), CP_UTF8);
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

void KTripODToVehODDlg::OnBnClickedBtnOdfile()
{
	if (m_cboAreaNumber.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("분석지역 번호를 선택하여 주십시오."));
		return;
	}
	if (m_cboDistributionYear.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("자료 배포년도를 선택하여 주십시오."));
		return;
	}

	int areaIdx = m_cboAreaNumber.GetCurSel();
	if (areaIdx == 0)
		m_strRegion = _T("수도권");
	else if (areaIdx == 1)
		m_strRegion = _T("전국권");
	else
		m_strRegion = _T("5대권역");

	m_cboDistributionYear.GetWindowText(m_strYear);

	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("텍스트 파일 (*.txt)|*.txt||"), this);
	dlg.m_ofn.lpstrTitle = _T("OD 파일 선택");

	if (dlg.DoModal() != IDOK)
		return;

	m_editODFilePath.SetWindowText(dlg.GetPathName());
	m_strODFilePath = dlg.GetPathName();

	CheckSeperator(m_strODFilePath);
	CheckEncoding(m_strODFilePath);
	CheckImportHeader(m_strODFilePath);

	SetCurColumnInfo();

	UpdatePreviewReport();
	UpdateODColumnReport();
	UpdateFileColumnReport();

	m_bLoadFile = true;
}

void KTripODToVehODDlg::OnBnClickedCheck4()
{
	if (m_bLoadFile)
	{
		std::vector <ColumnInfo> stColumnInfo;
		if (!GetColumnInfo(m_strRegion, m_strYear, stColumnInfo))
		{
			AfxMessageBox(_T("선택한 지역과 연도에 대한 컬럼 정보가 없습니다.\n설정 파일을 확인해 주세요."));
			return;
		}

		try
		{
			CXTPReportRecord* pRecord = nullptr;
			KColumnReportRecordItem* pItem = nullptr;

			for (int i = 0; i < stColumnInfo.size(); i++)
			{
				pRecord = m_rptODColumn.GetRecords()->GetAt(i);
				pItem = (KColumnReportRecordItem*)pRecord->GetItem(2);
				if (m_chkAutoColumn.GetCheck() == BST_CHECKED)
				{
					pItem->m_nIndex = stColumnInfo[i].index - 1;
				}
				else
				{
					pItem->m_nIndex = i;
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
}

void KTripODToVehODDlg::OnCbnSelchangeCombo1()
{
	if (m_bLoadFile)
	{
		int areaIdx = m_cboAreaNumber.GetCurSel();
		if (areaIdx == 0)
			m_strRegion = _T("수도권");
		else if (areaIdx == 1)
			m_strRegion = _T("전국권");
		else
			m_strRegion = _T("5대권역");

		UpdateODColumnReport();
		SetCurColumnInfo();
	}
}


void KTripODToVehODDlg::OnCbnSelchangeCombo2()
{
	if (m_bLoadFile)
	{
		m_cboDistributionYear.GetWindowText(m_strYear);
		UpdateODColumnReport();
		SetCurColumnInfo();
	}
}

void KTripODToVehODDlg::OnReportRecordsDropped(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTDRAGDROP* pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP*>(pNotifyStruct);

	try
	{
		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		if (pRecords->GetCount() < 1) return;

		CXTPReportRecord* pDropRecord = pRecords->GetAt(0);
		CXTPReportRecords* pTargetRecords = m_rptODColumn.GetRecords();

		if (pDropRecord != nullptr)
		{
			int nRowIndex = pDropRecord->GetIndex();

			CXTPReportRecordItem* pSrcItem = pDropRecord->GetItem(0);

			int nNewColIndex = (int)pSrcItem->GetItemData() - 1;

			CString strNewText = pSrcItem->GetCaption();

			pTargetRecords->RemoveRecord(pDropRecord);
			m_rptODColumn.Populate();

			pTargetRecords = m_rptODColumn.GetRecords();
			if (nRowIndex < pTargetRecords->GetCount())
			{
				CXTPReportRecord* pTargetRecord = pTargetRecords->GetAt(nRowIndex);
				if (pTargetRecord != nullptr)
				{
					KColumnReportRecordItem* pItem = (KColumnReportRecordItem*)pTargetRecord->GetItem(2);
					if (pItem)
					{
						pItem->m_nIndex = nNewColIndex;
						pItem->SetCaption(strNewText);

						CXTPReportRow* pRow = m_rptODColumn.GetRows()->Find(pTargetRecord);
						m_rptODColumn.SetFocusedRow(pRow);
						m_rptODColumn.RedrawControl();
					}
				}
			}
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
}

void KTripODToVehODDlg::OnBnClickedBtnConvertExcel()
{
	KExcelExportConfigDlg dlg(this);
	dlg.DoModal();
}

LRESULT KTripODToVehODDlg::OnThreadFinished(WPARAM wParam, LPARAM lParam)
{
	DrawingLoadingGif(false);

	GetDlgItem(ID_RUN)->EnableWindow(TRUE);

	bool bSuccess = (bool)wParam;

	return 0;
}

void KTripODToVehODDlg::CheckSeperator(CString a_strFile)
{
	try
	{
		QBicTxtReader oReader; 
		if (oReader.Open(a_strFile) == false)
			throw 1;	

		CString strLine(_T(""));
		oReader.ReadLine(strLine);

		int nSeparator(0);
		int nMax = -9; // 하나씩 파싱을 해보고 파싱된 결과가 가장 많은 개수인 구분자를 디폴트 구분자로.. 

		auto iter = m_mapSeparator.begin();
		auto itEnd = m_mapSeparator.end();

		for (; iter != itEnd; ++iter) 
		{
			const int& nCode = iter->first;
			TSeparator& oTSeparator = iter->second;

			std::vector<CString> vecSelect = QBicSplit::Split(strLine, oTSeparator.tcSeparator);

			int nSizeVec = (int)vecSelect.size();
			if (nSizeVec > nMax) {
				nMax = nSizeVec;
				nSeparator = nCode;
			}
		}

		m_cboSeparator.SetCurSel(nSeparator);

		oReader.Close();
	}
	catch (int& ex) 
	{
		CString strMsg(_T(""));
		if (1 == ex)
			strMsg = _T("Fail - Read TextFile");
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

	UpdateData(FALSE);
}

void KTripODToVehODDlg::CheckEncoding(CString a_strFile)
{
	try
	{
		int nEncoding = KEMCSVEncodingUnknown; 
		{
			QBicTxtReader oReader;
			nEncoding = oReader.GetTxtFileEncoding(a_strFile);

			if (nEncoding != KEMCSVEncodingUTF8 && nEncoding != KEMCSVEncodingANSI)
				throw 9;
		}

		if (nEncoding == KEMCSVEncodingUTF8) 
		{
			m_cboEncoding.SetCurSel(1);
		}
		else
		{
			m_cboEncoding.SetCurSel(0);
		}
	}
	catch (int& ex) 
	{
		CString strMsg(_T(""));
		if (KmzSystem::GetLanguage() == KEMKorea) 
		{
			if (1 == ex)
				strMsg = _T("인코딩 인식에 실패하였습니다.");
			else if (9 == ex)
				strMsg = _T("ANSI와 UTF-8 인코딩 파일만 인식 가능합니다.");
			else
				strMsg.Format(_T("오류 : %d"), ex);
		}
		else 
		{
			if (1 == ex)
				strMsg = _T("Encoding recognition failed.");
			else if (9 == ex)
				strMsg = _T("Only ANSI and UTF-8 encoded files are available.");
			else
				strMsg.Format(_T("Error : %d"), ex);
		}
		TxLogDebug((LPCTSTR)strMsg);
		AfxMessageBox(strMsg);
	}
	catch (KExceptionPtr ex) 
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...) 
	{
		TxLogDebugException();
		if (KmzSystem::GetLanguage() == KEMKorea) 
		{
			AfxMessageBox(_T("인코딩 인식에 실패하였습니다."));
		}
		else 
		{
			AfxMessageBox(_T("Encoding recognition failed."));
		}
	}
}

void KTripODToVehODDlg::CheckImportHeader(CString a_strFile)
{
	try
	{
		QBicTxtReader oReader; 
		if (oReader.Open(a_strFile) == false)
			throw 1;

		TCHAR tcSeparator; 
		int nCurSel = m_cboSeparator.GetCurSel();
		int nCode = (int)m_cboSeparator.GetItemData(nCurSel);

		auto itFind = m_mapSeparator.find(nCode);
		if (itFind != m_mapSeparator.end()) 
		{
			TSeparator& oTSeparator = itFind->second;
			tcSeparator = oTSeparator.tcSeparator;
		}

		std::vector<CString> vecSelect;
		oReader.ReadLine(tcSeparator, vecSelect);
		oReader.Close();

		CString strFirstLine = vecSelect[0];
		if (QBicStringChecker::IsNumeric(strFirstLine))
			m_bImportHeader = false;
		else
			m_bImportHeader = true;
	}
	catch (int& ex) 
	{
		CString strMsg(_T(""));
		if (1 == ex)
			strMsg = _T("Fail - Read TextFile");
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

	UpdateData(FALSE);
}

TCHAR KTripODToVehODDlg::GetSelectedSeparator()
{
	TCHAR tcSeparator = _T(',');

	int nCurSel = m_cboSeparator.GetCurSel();
	if (nCurSel != CB_ERR) 
	{
		int nCode = (int)m_cboSeparator.GetItemData(nCurSel);

		auto itFind = m_mapSeparator.find(nCode);
		if (itFind != m_mapSeparator.end()) {
			TSeparator& oTSeparator = itFind->second;
			tcSeparator = oTSeparator.tcSeparator;
		}
	}

	return tcSeparator;
}

int KTripODToVehODDlg::GetSelectedEncoding()
{
	int nEncoding = KEMCSVEncodingANSI;

	int nCurSel = m_cboEncoding.GetCurSel();
	if (nCurSel != CB_ERR) 
	{
		int nCode = (int)m_cboEncoding.GetItemData(nCurSel);

		if (nCode == 1)
			nEncoding = KEMCSVEncodingANSI;
		else if (nCode == 2)
			nEncoding = KEMCSVEncodingUTF8;
	}

	return nEncoding;
}

void KTripODToVehODDlg::UpdatePreviewReport()
{
	TxLogDebugStartMsg();

	try
	{
		if (LoadCSVPreviewData() == false)
			return;

		m_rptFilePreview.ResetContent();
		m_rptFilePreview.GetColumns()->Clear();
		m_rptFilePreview.Populate();

		bool bAutoSize(false);
		{
			if (m_ColumnRow.size() < 10)
				bAutoSize = true;
		}

		if (!bAutoSize)
		{
			QBicReportCtrlSetting::AutoColumnSizeFalse(m_rptFilePreview);
		}
		else
		{
			m_rptFilePreview.GetReportHeader()->SetAutoColumnSizing(TRUE);
		}

		CXTPReportColumn* pColumn = NULL;

		int nColumnIndex(0);
		std::vector< CString >::iterator itColumnArray = m_ColumnRow.begin();
		while (m_ColumnRow.end() != itColumnArray)
		{
			pColumn = m_rptFilePreview.AddColumn(new CXTPReportColumn(nColumnIndex, (*itColumnArray), 100));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (!bAutoSize)
				pColumn->SetBestFitMode(xtpColumnBestFitModeAllData);

			++nColumnIndex;
			++itColumnArray;
		}

		CXTPReportRecord* pRecord = nullptr;
		CXTPReportRecordItem* pItem = nullptr;

		int nRow(0);
		std::vector<CSVRow>::iterator itRows = m_Rows.begin(), itRowsEnd = m_Rows.end();
		if (m_bImportHeader)
			itRows++; // 첫번째 행은 컬럼명 이므로 패스
		for (; itRows != itRowsEnd; ++itRows)
		{
			pRecord = m_rptFilePreview.AddRecord(new CXTPReportRecord());

			CSVRow& row = (*itRows);
			CSVRow::iterator itrow, itrowEnd = row.end();
			for (itrow = row.begin(); itrow != itrowEnd; ++itrow)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(*itrow));
				pItem->SetAlignment(DT_CENTER);
			}

			int nSizeRowData = (int)row.size();
			if (nColumnIndex > nSizeRowData)
			{
				for (int i = 0; i < (nColumnIndex - nSizeRowData); i++)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItem);
				}
			}

			++nRow;
		}

		m_rptFilePreview.Populate();

		CXTPReportHeader* pHeader = m_rptFilePreview.GetReportHeader();
		CXTPReportColumns* pReportColumns = m_rptFilePreview.GetColumns();
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
		std::vector <ColumnInfo> stColumnInfo;
		if (!GetColumnInfo(m_strRegion, m_strYear, stColumnInfo))
		{
			AfxMessageBox(_T("선택한 지역과 연도에 대한 컬럼 정보가 없습니다.\n설정 파일을 확인해 주세요."));
			return;
		}

		InitODColumn();

		CXTPReportColumn* pColumn = m_rptODColumn.GetColumns()->GetAt(2);

		for (int i = 0; i < m_ColumnRow.size(); i++)
		{
			pColumn->GetEditOptions()->AddConstraint(m_ColumnRow[i], i);
		}
		pColumn->SetAlignment(DT_CENTER);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
		pColumn->GetEditOptions()->AddComboButton(TRUE);

		auto colStart = stColumnInfo.begin();
		auto colEnd = stColumnInfo.end();
		int nIdx = 0;
		CXTPReportRecord* pRecord = nullptr;

		for (; colStart != colEnd; ++colStart, nIdx++)
		{
			pRecord = m_rptODColumn.AddRecord(new CXTPReportRecord());

			CString strIdx;
			strIdx.Format(_T("%d"), nIdx + 1);
			pRecord->AddItem(new CXTPReportRecordItemText(strIdx));
			pRecord->AddItem(new CXTPReportRecordItemText(colStart->name));
			if (m_chkAutoColumn.GetCheck() == BST_CHECKED && colStart->index <= m_Rows[0].size())
				pRecord->AddItem(new KColumnReportRecordItem(colStart->index - 1));
			else
				pRecord->AddItem(new KColumnReportRecordItem(nIdx));
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

void KTripODToVehODDlg::UpdateFileColumnReport()
{
	TxLogDebugStartMsg();

	try
	{
		m_rptFileColumn.ResetContent();

		if (m_ColumnRow.size() > 0)
		{
			CXTPReportColumn* pImport = m_rptFileColumn.GetColumns()->GetAt(0);
			int colIdx = 1;

			auto colStart = m_ColumnRow.begin();
			auto colEnd = m_ColumnRow.end();

			for (; colStart != colEnd; ++colStart, colIdx++)
			{
				pImport->GetEditOptions()->AddConstraint(*colStart, colIdx);
				CXTPReportRecord* pRecord = m_rptFileColumn.AddRecord(new CXTPReportRecord());
				CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText(*colStart));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetItemData(colIdx);
			}

			m_rptFileColumn.Populate();
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

bool KTripODToVehODDlg::LoadCSVPreviewData()
{
	TxLogDebugStartMsg();
	m_Rows.clear();
	m_ColumnRow.clear();

	TCHAR tcSeparator = GetSelectedSeparator();
	int   nEncoding = GetSelectedEncoding();

	try
	{
		QBicTxtReader oReader;
		oReader.SetEncoding(nEncoding);
		if (oReader.Open(m_strODFilePath) == false)
			throw 2;

		CString strLine(_T(""));
		oReader.ReadLine(strLine);

		int nIndex(1);
		CSVRow row;

		ParseCSVLineString(strLine, tcSeparator, row);

		CString strColumnName(_T(""));
		CSVRow::iterator itRow = row.begin();
		while (row.end() != itRow)
		{
			if (m_bImportHeader)
			{
				strColumnName = *itRow;
			}
			else
			{
				strColumnName.Format(_T("Column %d"), nIndex++);
			}
			m_ColumnRow.push_back(strColumnName);
			++itRow;
		}

		m_Rows.push_back(row);

		int nLoopCount = 0;

		while (oReader.ReadLine(strLine) && nLoopCount < 5000)
		{
			++nLoopCount;

			if ((strLine.Trim()).CompareNoCase(_T("")) == 0)
				continue;

			if (nLoopCount < 100 || nLoopCount % 5 == 0)
			{
				CSVRow row;
				ParseCSVLineString(strLine, tcSeparator, row);
				m_Rows.push_back(row);
			}
		}

		oReader.Close();
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 2)
			strError.Format(_T("Error - Open File: %s"), m_strODFilePath);
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

void KTripODToVehODDlg::ParseCSVLineString(CString& strLine, TCHAR tcSeparator, CSVRow& row)
{
	bool bQuote(false);
	int  nSub(0);
	CString strRow, strQuote;

	while (AfxExtractSubString(strRow, strLine, nSub++, tcSeparator) == TRUE)
	{
		if (true == bQuote) 
		{
			strQuote += strRow;
			if (strRow.GetAt(strRow.GetLength() - 1) == _T('"')) 
			{
				strQuote.Remove(_T('"'));
				strRow = strQuote;
				bQuote = false;
				strQuote = _T("");
				row.push_back(strRow);
			}
		}
		else 
		{
			TCHAR chFirst, chLast;
			if (strRow.IsEmpty() == TRUE) 
			{
				row.push_back(strRow);
			}
			else if (strRow.GetLength() == 1) 
			{
				chFirst = strRow.GetAt(0);
				if (chFirst == _T('"')) 
				{
					bQuote = true;
				}
				else {
					row.push_back(strRow);
				}
			}
			else 
			{
				chFirst = strRow.GetAt(0);
				chLast = strRow.GetAt(strRow.GetLength() - 1);

				if ((chFirst == _T('"')) && (chLast == _T('"'))) 
				{
					strRow.Remove(_T('"'));
					row.push_back(strRow);
				}
				else if (chFirst == _T('"')) 
				{
					bQuote = true;
					strQuote = strRow;
				}
				else 
				{
					row.push_back(strRow);
				}
			}
		}
	}
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

bool KTripODToVehODDlg::GetScaleFactorValue(int nO, int nD, int nCol, double& dValue)
{
	if (m_curColumnInfo.size() <= 0)
		return false;

	CString apply;
	for (const auto& info : m_curColumnInfo)
	{
		if (info.index == nCol + 1)
		{
			apply = info.apply;
			break;
		}
	}

	if (apply == "Auto")
	{
		return GetAutoScaleFactorValue(nO, nD, dValue);
	}
	else
	{
		AccessFactorType factorData;
		if (!GetAccessScaleFactorValue(nO, factorData))
			return false;

		if (apply == "Bus_전국권")
			dValue = factorData.nationalBus;
		else if (apply == "OtherBus")
			dValue = factorData.otherBus;
		else if (apply == "Taxi")
			dValue = factorData.taxi;
		else if (apply == "Bus_수도권")
			dValue = factorData.capitalBus;
		else
			dValue = 1.0;
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

bool KTripODToVehODDlg::BulildODDataSet()
{
	try
	{
		CXTPReportRecords* pRecords = m_rptODColumn.GetRecords();
		int nDataSetCount = (int)pRecords->GetCount();
		if (!pRecords || nDataSetCount <= 0)
		{
			throw 1;
		}

		std::vector<int> vecODColumnIndex(nDataSetCount, -1);

		for (int i = 0; i < nDataSetCount; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			if (!pRecord)
				continue;

			KColumnReportRecordItem* pItem = (KColumnReportRecordItem*)pRecord->GetItem(2);
			if (!pItem)
				continue;

			vecODColumnIndex[i] = pItem->m_nIndex;
		}

		m_vecOD_O.clear();
		m_vecOD_D.clear();
		m_vecODDataSet.clear();
		m_vecODDataSet.resize(nDataSetCount - 2);

		QBicTxtReader reader;
		reader.SetEncoding(m_nCachedEncoding);

		if (!reader.Open(m_strODFilePath))
		{
			throw 2;
		}

		CString strLine;
		bool bFirstLine = true;

		while (reader.ReadLine(strLine))
		{
			CString tmp = strLine;
			if (tmp.Trim().IsEmpty())
				continue;

			// 첫 줄이 헤더이면 스킵
			if (bFirstLine && m_bImportHeader)
			{
				bFirstLine = false;
				continue;
			}
			bFirstLine = false;

			CSVRow row;
			ParseCSVLineString(strLine, m_tcCachedSeparator, row);

			// 최소한 O,D(0,1열)는 있어야 함
			if (row.size() < 2)
				continue;

			int o = _ttoi(row[vecODColumnIndex[0]]);
			int d = _ttoi(row[vecODColumnIndex[1]]);

			m_vecOD_O.push_back(o);
			m_vecOD_D.push_back(d);

			double dScaleFactor = 1.0;

			// 각 ColumnInfo에 대응되는 OD 열 값 추출
			for (int ds = 0; ds < nDataSetCount - 2; ++ds)
			{
				int colIdx = vecODColumnIndex[ds + 2];
				double val = 0.0;
				double dScaleFactor = 0.0;

				if (colIdx >= 0 && colIdx < (int)row.size())
				{
					if (!GetScaleFactorValue(o, d, colIdx, dScaleFactor))
						dScaleFactor = 1.0;

					CString sVal = row[colIdx];
					sVal.Trim();
					if (!sVal.IsEmpty())
					{
						val = _tstof(sVal) / dScaleFactor;
					}
				}

				m_vecODDataSet[ds].push_back(val);
			}
		}

		reader.Close();

		// 간단 검증: 각 데이터셋의 행 수가 O/D 행 수와 같은지 확인
		int nRowCount = (int)m_vecOD_O.size();
		for (int ds = 0; ds < nDataSetCount - 2; ++ds)
		{
			if ((int)m_vecODDataSet[ds].size() != nRowCount)
			{
				throw 3;
			}
		}

		return true;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("OD 매핑정보가 존재하지 않습니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 2)
		{
			strError.Format(_T("OD 파일을 여는 데 실패했습니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 3)
		{
			strError.Format(_T("OD 데이터셋의 크기가 O/D 행 수와 맞지 않습니다.\n매핑 설정을 확인해 주세요."));
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
		AfxMessageBox(_T("재차인원 계산 적용에 실패하였습니다."));
		TxLogDebugException();
		return false;
	}
}

bool KTripODToVehODDlg::ExportVehODFile()
{
	try
	{
		const int nRowCnt = (int)m_vecOD_O.size();
		if (nRowCnt == 0 || (int)m_vecOD_D.size() != nRowCnt)
		{
			throw 1;
		}

		const int nDataSetCnt = (int)m_vecODDataSet.size();
		for (int ds = 0; ds < nDataSetCnt; ds++)
		{
			if ((int)m_vecODDataSet[ds].size() != nRowCnt)
			{
				throw 2;
			}
		}

		CString strOutPath = m_strODFilePath;

		int posSlash = strOutPath.ReverseFind(_T('\\'));
		int posDot = strOutPath.ReverseFind(_T('.'));

		if (posDot == -1 || (posSlash != -1 && posDot < posSlash))
		{
			strOutPath += _T("_Veh.txt");
		}
		else
		{
			// 확장자가 있는 경우 -> 이름부분에 _Veh 붙이고 .txt
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
		char* pBuffer = new char[BUF_SIZE];
		int nBufOffset = 0;

		UINT nCodePage = CP_ACP;
		if (m_nCachedEncoding == KEMCSVEncodingUTF8)
		{
			nCodePage = CP_UTF8;
			unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
			memcpy(pBuffer + nBufOffset, bom, 3);
			nBufOffset += 3;
		}

		CStringA strTmp;

		// Header 삽입
		for (const auto& info : m_curColumnInfo)
		{
			CW2A pszName(info.name, nCodePage);
			CStringA strItem;
			strItem.Format("%s%c", (LPCSTR)pszName, m_tcCachedSeparator);
			strTmp += strItem;
		}

		strTmp += "\r\n";
		int nLen = strTmp.GetLength();
		memcpy(pBuffer + nBufOffset, (LPCSTR)strTmp, nLen);
		nBufOffset += nLen;

		for (int row = 0; row < nRowCnt; ++row)
		{
			// O, D 먼저
			strTmp.Format("%d%c%d", m_vecOD_O[row], (char)m_tcCachedSeparator, m_vecOD_D[row]);

			// 그 뒤에 각 데이터셋 값들 이어 붙이기
			for (int ds = 0; ds < nDataSetCnt; ++ds)
			{
				CString strVal;
				// g 서식: 유효자릿수 기반. 필요하면 %.6f 등으로 변경 가능
				strVal.Format(_T("%c%.5g"), m_tcCachedSeparator, m_vecODDataSet[ds][row]);
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

		delete[] pBuffer;
		file.Close();

		CString msg;
		msg.Format(_T("Veh OD 파일을 생성했습니다.\n\n파일 : %s"), strOutPath.GetString());
		AfxMessageBox(msg);

		return true;
	}
	catch (int& ex)
	{
		CString strError(_T(""));
		if (ex == 1)
		{
			strError.Format(_T("OD 데이터가 없거나 O/D 행 수가 불일치 합니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 2)
		{
			strError.Format(_T("데이터셋의 행 수가 O/D 행 수와 일치하지 않습니다."));
			AfxMessageBox(strError);
		}
		else if (ex == 3)
		{
			strError.Format(_T("파일을 생성할 수 없습니다."));
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
	if (bSuccess && !pDlg->LoadExcelAccessFactorData())
		bSuccess = false;
	if (bSuccess && !pDlg->BulildODDataSet())
		bSuccess = false;
	if (bSuccess && !pDlg->ExportVehODFile())
		bSuccess = false;

	::PostMessage(pDlg->m_hWnd, WM_THREAD_FINISHED, (WPARAM)bSuccess, 0);

	return 0;
}