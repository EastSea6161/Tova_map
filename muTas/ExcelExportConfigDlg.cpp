// ExcelExportConfigDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "ExcelExportConfigDlg.h"

#define WM_HEADER_EDIT_FINISHED (WM_USER + 100)

class CInplaceHeaderEdit : public CEdit
{
public:
	int m_row = -1;
	int m_subItem = -1;

protected:
	afx_msg void OnKillFocus(CWnd* pNewWnd)
	{
		CEdit::OnKillFocus(pNewWnd);

		CString text;
		GetWindowText(text);

		// 부모(리스트컨트롤의 부모 = 다이얼로그)에 편집 완료 알림
		CWnd* pParent = GetParent();
		if (pParent && ::IsWindow(pParent->GetSafeHwnd()))
		{
			CString* pStr = new CString(text); // 나중에 부모에서 delete
			pParent->SendMessage(WM_HEADER_EDIT_FINISHED, (WPARAM)m_row, (LPARAM)pStr);
		}

		DestroyWindow(); // 자기 자신 파괴
	}

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CInplaceHeaderEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// KExcelExportConfigDlg 대화 상자

IMPLEMENT_DYNAMIC(KExcelExportConfigDlg, CDialogEx)

KExcelExportConfigDlg::KExcelExportConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_7010_UTIL_ExecelExportConfigDlg, pParent)
{

}

KExcelExportConfigDlg::~KExcelExportConfigDlg()
{
}

void KExcelExportConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SHEET, m_comboSheet);
	DDX_Control(pDX, IDC_LIST_COLUMN, m_listColumn);
	DDX_Control(pDX, IDC_LIST_SELECT_COLUMN, m_gridSelectColumn);
	DDX_Control(pDX, IDC_LIST_PREVIEW, m_listPreview);
	DDX_Control(pDX, IDC_EDIT_EXCEL_PATH, m_editExcelPath);
	DDX_Control(pDX, IDC_CHK_USE_HEADER, m_chkUseHeader);
	DDX_Control(pDX, IDC_STCANIMATE, m_loadingGif);
}


BEGIN_MESSAGE_MAP(KExcelExportConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_EXCEL_SELCET, &KExcelExportConfigDlg::OnBnClickedBtnExcelSelcet)
	ON_CBN_SELCHANGE(IDC_COMBO_SHEET, &KExcelExportConfigDlg::OnCbnSelchangeComboSheet)
	ON_BN_CLICKED(IDC_BTN_COL_ADD, &KExcelExportConfigDlg::OnBnClickedBtnColAdd)
	ON_BN_CLICKED(IDC_BTN_COL_UP, &KExcelExportConfigDlg::OnBnClickedBtnColUp)
	ON_BN_CLICKED(IDC_BTN_COL_DOWN, &KExcelExportConfigDlg::OnBnClickedBtnColDown)
	ON_BN_CLICKED(IDC_BTN_ADD_COL, &KExcelExportConfigDlg::OnBnClickedBtnAddCol)
	ON_BN_CLICKED(IDC_BTN_COL_DEL, &KExcelExportConfigDlg::OnBnClickedBtnColDel)
	ON_BN_CLICKED(IDC_CHK_USE_HEADER, &KExcelExportConfigDlg::OnBnClickedChkUseHeader)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SELECT_COLUMN, &KExcelExportConfigDlg::OnNMDblclkListSelectColumn)
	ON_MESSAGE(WM_USER + 100, &KExcelExportConfigDlg::OnHeaderEditFinished)
	ON_BN_CLICKED(IDC_BTN_PREVIEW, &KExcelExportConfigDlg::OnBnClickedBtnPreview)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &KExcelExportConfigDlg::OnBnClickedBtnExport)
	ON_LBN_DBLCLK(IDC_LIST_COLUMN, &KExcelExportConfigDlg::OnLbnDblclkListColumn)
END_MESSAGE_MAP()


// KExcelExportConfigDlg 메시지 처리기


BOOL KExcelExportConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	m_loadingGif.Load(MAKEINTRESOURCE(IDR_PROGRESSIMAGE), _T("GIF"));
	DrawingLoadingGif(false);

	m_gridSelectColumn.SetExtendedStyle(m_gridSelectColumn.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	m_gridSelectColumn.InsertColumn(0, _T("Use"), LVCFMT_LEFT, 70);
	m_gridSelectColumn.InsertColumn(1, _T("Column"), LVCFMT_LEFT, 230);
	m_gridSelectColumn.InsertColumn(2, _T("Header"), LVCFMT_LEFT, 230);

	m_chkUseHeader.SetCheck(BST_UNCHECKED);

	m_bUseHeader = false;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KExcelExportConfigDlg::OnBnClickedBtnExcelSelcet()
{
	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("엑셀 파일 (*.xlsx;*.xls)|*.xlsx;*.xls||"), this);
	dlg.m_ofn.lpstrTitle = _T("OD 파일 선택");

	if (dlg.DoModal() != IDOK)
		return;

	m_editExcelPath.SetWindowText(dlg.GetPathName());
	m_excelFilePath = dlg.GetPathName();

	LoadSheetList();
}

void KExcelExportConfigDlg::OnCbnSelchangeComboSheet()
{
	LoadColumnListSheet();
}

void KExcelExportConfigDlg::OnBnClickedBtnColAdd()
{
	int sel = m_listColumn.GetCurSel();
	if (sel == LB_ERR)
	{
		AfxMessageBox(_T("추가할 열을 선택하세요."));
		return;
	}

	CString text;
	m_listColumn.GetText(sel, text);  // 예: "C: 출발시군구"

	CString letter = text;
	CString header;
	int colon = text.Find(_T(':'));
	if (colon != -1)
	{
		letter = text.Left(colon);
		header = text.Mid(colon + 1);
		header.Trim();
	}
	letter.Trim();

	OutputColumnConfig cfg{};
	cfg.enabled = true;
	cfg.addColumn = false;
	cfg.excelColLetter = letter;
	cfg.excelColIndex = 0;      // 나중에 필요하면 Letter→Index 변환
	cfg.excelHeader = header;
	cfg.outputHeader = header; // 기본은 엑셀 헤더와 동일하게

	m_selectedColumns.push_back(cfg);

	RefreshSelectedGrid();

	int newRow = (int)m_selectedColumns.size() - 1;
	if (newRow >= 0)
		m_gridSelectColumn.SetItemState(newRow, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}


void KExcelExportConfigDlg::OnBnClickedBtnColUp()
{
	int sel = m_gridSelectColumn.GetNextItem(-1, LVNI_SELECTED);
	if (sel <= 0) return; // 선택 없음 또는 이미 맨 위

	std::swap(m_selectedColumns[sel], m_selectedColumns[sel - 1]);

	RefreshSelectedGrid();

	m_gridSelectColumn.SetItemState(sel - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}


void KExcelExportConfigDlg::OnBnClickedBtnColDown()
{
	int sel = m_gridSelectColumn.GetNextItem(-1, LVNI_SELECTED);
	if (sel == -1) return;
	if (sel >= (int)m_selectedColumns.size() - 1) return; // 맨 아래

	std::swap(m_selectedColumns[sel], m_selectedColumns[sel + 1]);

	RefreshSelectedGrid();

	m_gridSelectColumn.SetItemState(sel + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}


void KExcelExportConfigDlg::OnBnClickedBtnAddCol()
{
	OutputColumnConfig cfg{};
	cfg.enabled = true;
	cfg.addColumn = true;
	cfg.excelColLetter = _T("");
	cfg.excelColIndex = 0;
	cfg.excelHeader = _T("");
	cfg.outputHeader = _T("");   // 헤더는 사용자가 나중에 직접 입력

	m_selectedColumns.push_back(cfg);

	RefreshSelectedGrid();

	int newRow = (int)m_selectedColumns.size() - 1;
	if (newRow >= 0)
		m_gridSelectColumn.SetItemState(newRow, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}


void KExcelExportConfigDlg::OnBnClickedBtnColDel()
{
	int sel = m_gridSelectColumn.GetNextItem(-1, LVNI_SELECTED);
	if (sel == -1) return;

	m_selectedColumns.erase(m_selectedColumns.begin() + sel);

	RefreshSelectedGrid();

	int newSel = sel;
	if (newSel >= (int)m_selectedColumns.size())
		newSel = (int)m_selectedColumns.size() - 1;

	if (newSel >= 0)
	{
		m_gridSelectColumn.SetItemState(newSel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void KExcelExportConfigDlg::OnBnClickedChkUseHeader()
{
	BOOL checked = m_chkUseHeader.GetCheck();
	m_bUseHeader = (checked == BST_CHECKED);

	if (!m_bUseHeader)
		return;

	ApplyHeaderFromFirstRow();
	RefreshSelectedGrid();
}


void KExcelExportConfigDlg::OnNMDblclkListSelectColumn(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (!m_bUseHeader)
		return;

	LPNMITEMACTIVATE pAct = (LPNMITEMACTIVATE)pNMHDR;
	int row = pAct->iItem;
	int subItem = pAct->iSubItem;

	if (row < 0 || subItem != 2)  // "출력 헤더" 열만 편집
		return;

	// 해당 셀의 위치 얻기
	CRect rc;
	if (!m_gridSelectColumn.GetSubItemRect(row, subItem, LVIR_LABEL, rc))
		return;

	// 에디트 컨트롤 생성
	CInplaceHeaderEdit* pEdit = new CInplaceHeaderEdit;
	pEdit->m_row = row;
	pEdit->m_subItem = subItem;

	// WS_BORDER 빼고싶으면 스타일 조정
	pEdit->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT,
		rc, &m_gridSelectColumn, 0);

	CString curText = m_gridSelectColumn.GetItemText(row, subItem);
	pEdit->SetWindowText(curText);
	pEdit->SetFocus();
	pEdit->SetSel(0, -1);
}

LRESULT KExcelExportConfigDlg::OnHeaderEditFinished(WPARAM wParam, LPARAM lParam)
{
	int row = (int)wParam;
	CString* pText = (CString*)lParam;

	if (pText)
	{
		if (row >= 0 && row < (int)m_selectedColumns.size())
		{
			m_selectedColumns[row].outputHeader = *pText;
			RefreshSelectedGrid();
		}
		delete pText;
	}
	return 0;
}

void KExcelExportConfigDlg::OnBnClickedBtnPreview()
{
	std::vector<CString> lines;
	if (!BuildODLines(lines, 20))   // 프리뷰는 최대 20행 정도만
		return;

	m_listPreview.ResetContent();

	for (const auto& line : lines)
		m_listPreview.AddString(line);
}


void KExcelExportConfigDlg::OnBnClickedBtnExport()
{
	std::vector<CString> lines;
	if (!BuildODLines(lines, -1))   // -1 → 전체 행
		return;

	if (lines.empty())
	{
		AfxMessageBox(_T("내보낼 데이터가 없습니다."));
		return;
	}

	// 기본 저장 경로/파일명: 엑셀과 같은 폴더, {엑셀파일명}_OD.txt
	CString initialDir, defName;

	if (!m_excelFilePath.IsEmpty())
	{
		CString path = m_excelFilePath;
		int pos = path.ReverseFind(_T('\\'));
		if (pos != -1)
		{
			initialDir = path.Left(pos);

			CString base = path.Mid(pos + 1); // 파일명
			int dot = base.ReverseFind(_T('.'));
			if (dot != -1)
				base = base.Left(dot);

			defName = base + _T("_OD.txt");
		}
		else
		{
			defName = _T("OD.txt");
		}
	}
	else
	{
		defName = _T("OD.txt");
	}

	CFileDialog dlg(FALSE, _T("txt"), defName, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("텍스트 파일 (*.txt)|*.txt||"), this);

	if (!initialDir.IsEmpty())
		dlg.m_ofn.lpstrInitialDir = initialDir;

	if (dlg.DoModal() != IDOK)
		return;

	CString savePath = dlg.GetPathName();

	CStdioFile file;
	if (!file.Open(savePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		AfxMessageBox(_T("OD 파일을 저장할 수 없습니다."));
		return;
	}

	for (const auto& line : lines)
	{
		file.WriteString(line);
		file.WriteString(_T("\n"));
	}

	file.Close();

	// KTripODToVehODDlg 로 넘겨줄 최종 TXT 경로
	m_exportExcelPath = savePath;

	// 다이얼로그 종료 (OK)
	EndDialog(IDOK);
}

void KExcelExportConfigDlg::OnLbnDblclkListColumn()
{
	OnBnClickedBtnColAdd();
}

void KExcelExportConfigDlg::LoadSheetList()
{
	DrawingLoadingGif(true);

	m_comboSheet.ResetContent();
	m_listColumn.ResetContent();
	m_sheetNames.clear();

	CString excelPath;
	m_editExcelPath.GetWindowText(excelPath);
	excelPath.Trim();

	if (excelPath.IsEmpty())
		return;

	if (!m_excelHandler.GetExcelSheetNames(excelPath, m_sheetNames))
	{
		AfxMessageBox(_T("엑셀 파일의 시트를 불러오는데 실패했습니다."));
		return;
	}

	for (size_t i = 0; i < m_sheetNames.size(); i++)
	{
		m_comboSheet.AddString(m_sheetNames[i]);
	}

	if (!m_sheetNames.empty())
	{
		m_comboSheet.SetCurSel(0);
		LoadColumnListSheet();
	}

	DrawingLoadingGif(false);
}

void KExcelExportConfigDlg::LoadColumnListSheet()
{
	DrawingLoadingGif(true);
	m_listColumn.ResetContent();

	int sel = m_comboSheet.GetCurSel();
	if (sel == CB_ERR)
		return;

	CString sheetName;
	m_comboSheet.GetLBText(sel, sheetName);

	if (m_excelFilePath.IsEmpty())
		return;

	// 헤더는 1행이라고 가정 (나중에 옵션으로 바꿔도 됨)
	const long headerRow = 1;

	// A ~ ? 열까지 한 번에 읽기 (필요하면 30, 50 등으로 늘리면 됨)
	const int maxCol = 30;

	std::vector<CString> headers;
	if (!m_excelHandler.ReadRowRangeFromExcel(m_excelFilePath, sheetName, headerRow, maxCol, headers))
		return;

	// 결과 벡터: A=0, B=1, C=2 ...
	for (size_t i = 0; i < headers.size(); ++i)
	{
		CString header = headers[i];
		header.Trim();

		if (header.IsEmpty())
			continue;   // 완전히 빈 열은 스킵

		int colIndex = (int)i + 1;                  // 1-based
		CString colLetter = m_excelHandler.ExcelColumnLetterFromIndex(colIndex);

		CString item;
		item.Format(_T("%s: %s"), colLetter.GetString(), header.GetString());
		m_listColumn.AddString(item);
	}
	DrawingLoadingGif(false);
}

void KExcelExportConfigDlg::RefreshSelectedGrid()
{
	m_gridSelectColumn.DeleteAllItems();

	for (size_t i = 0; i < m_selectedColumns.size(); ++i)
	{
		const auto& cfg = m_selectedColumns[i];

		int row = m_gridSelectColumn.InsertItem((int)i, _T(""));
		m_gridSelectColumn.SetCheck(row, cfg.enabled ? TRUE : FALSE);

		CString colText;
		if (cfg.addColumn)
		{
			colText = _T("(Add Column)");
		}
		else
		{
			if (!cfg.excelHeader.IsEmpty())
				colText.Format(_T("%s: %s"), cfg.excelColLetter.GetString(), cfg.excelHeader.GetString());
			else
				colText = cfg.excelColLetter;
		}

		m_gridSelectColumn.SetItemText(row, 1, colText);
		m_gridSelectColumn.SetItemText(row, 2, cfg.outputHeader);
	}
}

void KExcelExportConfigDlg::ApplyHeaderFromFirstRow()
{
	for (auto& cfg : m_selectedColumns)
	{
		if (!cfg.addColumn)
		{
			// 엑셀 헤더가 있으면 그걸 사용, 없으면 엑셀 열 문자라도 넣자
			if (!cfg.excelHeader.IsEmpty())
				cfg.outputHeader = cfg.excelHeader;
			else
				cfg.outputHeader = cfg.excelColLetter;
		}
		else
		{
			// 빈 열은 일단 공백 또는 원하는 기본값
			// cfg.outputHeader.Empty();
		}
	}
}

bool KExcelExportConfigDlg::BuildODLines(std::vector<CString>& outLines, int maxPreviewRows)
{
	DrawingLoadingGif(true);
	outLines.clear();

	// 1) 엑셀 파일 / 시트 / 선택 컬럼 체크
	if (m_excelFilePath.IsEmpty())
	{
		AfxMessageBox(_T("엑셀 파일을 먼저 선택하십시오."));
		return false;
	}

	int sheetSel = m_comboSheet.GetCurSel();
	if (sheetSel == CB_ERR)
	{
		AfxMessageBox(_T("시트를 선택하십시오."));
		return false;
	}

	CString sheetName;
	m_comboSheet.GetLBText(sheetSel, sheetName);

	// 사용 체크된 컬럼들만 출력 대상으로 삼음
	std::vector<int> enabledIdx;
	for (int i = 0; i < (int)m_selectedColumns.size(); ++i)
	{
		if (m_selectedColumns[i].enabled)
			enabledIdx.push_back(i);
	}

	if (enabledIdx.empty())
	{
		AfxMessageBox(_T("Selected Column 에서 사용할 열을 선택하십시오."));
		return false;
	}

	// 2) 시작 행 읽기
	int dataStartRow = 2;

	// 3) 배치 읽기를 위한 요청 목록 생성
	std::vector<ExcelColumnRequest> requestStorage; // 실제 객체 저장소
	std::vector<ExcelColumnRequest*> batchRequests; // 포인터 전달용

	// 중복된 열을 여러번 읽지 않도록 맵으로 관리 (Letter -> Index)
	std::map<CString, int> letterToReqIndex;

	for (int idx : enabledIdx)
	{
		const auto& cfg = m_selectedColumns[idx];
		if (cfg.addColumn || cfg.excelColLetter.IsEmpty()) continue;

		// 이미 요청 목록에 있는지 확인
		if (letterToReqIndex.find(cfg.excelColLetter) == letterToReqIndex.end())
		{
			ExcelColumnRequest req;
			req.colLetter = cfg.excelColLetter;
			requestStorage.push_back(req);
			letterToReqIndex[cfg.excelColLetter] = (int)requestStorage.size() - 1;
		}
	}

	// 포인터 벡터 구성
	for (size_t i = 0; i < requestStorage.size(); ++i)
		batchRequests.push_back(&requestStorage[i]);

	// 4) 엑셀에서 한방에 읽어오기 (핵심 변경 사항)
	if (!batchRequests.empty())
	{
		if (!m_excelHandler.ReadColumnsBatch(m_excelFilePath, sheetName, dataStartRow, batchRequests))
		{
			AfxMessageBox(_T("엑셀 데이터를 읽는데 실패했습니다."));
			return false;
		}
	}

	// 5) 읽어온 데이터를 라인별로 조합
	// 먼저 최대 행 수 계산
	size_t maxRows = 0;
	for (const auto& req : requestStorage)
		if (req.data.size() > maxRows) maxRows = req.data.size();

	// 프리뷰인 경우 행 제한
	if (maxPreviewRows > 0 && maxRows > (size_t)maxPreviewRows)
		maxRows = (size_t)maxPreviewRows;

	// 헤더 처리 (기존 로직 유지)
	if (m_bUseHeader)
	{
		CString headerLine;
		bool first = true;
		for (int idx : enabledIdx)
		{
			if (!first) headerLine += _T("\t");
			headerLine += m_selectedColumns[idx].outputHeader;
			first = false;
		}
		outLines.push_back(headerLine);
	}

	// 데이터 라인 생성
	for (size_t r = 0; r < maxRows; ++r)
	{
		CString line;
		bool first = true;

		for (int idx : enabledIdx)
		{
			if (!first) line += _T("\t");

			const auto& cfg = m_selectedColumns[idx];
			if (!cfg.addColumn)
			{
				// 캐시된 데이터에서 찾기
				auto it = letterToReqIndex.find(cfg.excelColLetter);
				if (it != letterToReqIndex.end())
				{
					const auto& vec = requestStorage[it->second].data;
					if (r < vec.size())
						line += vec[r];
				}
			}
			else
			{
				// 사용자가 추가한 열(Add Column)은 "0"으로 채움
				line += _T("0");
			}
			first = false;
		}
		outLines.push_back(line);
	}

	DrawingLoadingGif(false);
	return !outLines.empty();
}

void KExcelExportConfigDlg::DrawingLoadingGif(bool bStart)
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



