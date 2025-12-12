// FODDiv.cpp: 구현 파일
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "FODDiv.h"


// KFODDiv 대화 상자

IMPLEMENT_DYNAMIC(KFODDiv, CDialogEx)

KFODDiv::KFODDiv(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_7002_UTIL_FODDiv, pParent)
{

}

KFODDiv::~KFODDiv()
{
}

void KFODDiv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboAreaNumber);
	DDX_Control(pDX, IDC_COMBO2, m_cboDistributionYear);
	DDX_Control(pDX, IDC_LIST1, m_listODFilePath);
}


BEGIN_MESSAGE_MAP(KFODDiv, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADDOD, &KFODDiv::OnBnClickedBtnAddod)
	ON_BN_CLICKED(ID_RUN, &KFODDiv::OnBnClickedRun)
    ON_BN_CLICKED(IDC_BTN_DELETE, &KFODDiv::OnBnClickedBtnDelete)
END_MESSAGE_MAP()

BOOL KFODDiv::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

    for (int i = 0; i < UTIL_TRIPOD_TO_VEHOD_AREA_CNT; i++)
    {
        m_cboAreaNumber.InsertString(i, UTIL_TRIPOD_TO_VEHOD_AREA[i]);
    }

    LoadDistributionList();

    m_cboAreaNumber.SetCurSel(0);
    m_cboDistributionYear.SetCurSel(0);

	DWORD dwStyle = m_listODFilePath.GetExtendedStyle();
	m_listODFilePath.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listODFilePath.InsertColumn(0, _T("사회경제지표 파일"), LVCFMT_LEFT, 200);
	m_listODFilePath.InsertColumn(1, _T("입력 OD 파일"), LVCFMT_LEFT, 200);
	m_listODFilePath.InsertColumn(2, _T("폴더 경로"), LVCFMT_LEFT, 400);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// KFODDiv 메시지 처리기


void KFODDiv::OnBnClickedBtnAddod()
{
    CFileDialog dlgSE(TRUE, _T("txt"), nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("텍스트 파일 (*.txt)|*.txt||"), this);
    dlgSE.m_ofn.lpstrTitle = _T("사회경제지표 파일 선택");

    if (dlgSE.DoModal() != IDOK)
        return;

    CString seFullPath = dlgSE.GetPathName();
    CString seFileName = dlgSE.GetFileName();

    CString folderPath = seFullPath;
    int pos = folderPath.ReverseFind(_T('\\'));
    if (pos != -1)
        folderPath = folderPath.Left(pos);

    CFileDialog dlgOD(TRUE, _T("txt"), nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("텍스트 파일 (*.txt)|*.txt||"), this);
    dlgOD.m_ofn.lpstrTitle = _T("입력 OD 파일 선택");
    dlgOD.m_ofn.lpstrInitialDir = folderPath;

    if (dlgOD.DoModal() != IDOK)
        return;

    CString odFileName = dlgOD.GetFileName();

    int nItemCount = m_listODFilePath.GetItemCount();
    for (int i = 0; i < nItemCount; ++i)
    {
        CString existFolder = m_listODFilePath.GetItemText(i, 2);
        CString existSE = m_listODFilePath.GetItemText(i, 0);
        CString existOD = m_listODFilePath.GetItemText(i, 1);

        if (existFolder.CompareNoCase(folderPath) == 0 &&
            existSE.CompareNoCase(seFileName) == 0 &&
            existOD.CompareNoCase(odFileName) == 0)
        {
            return;
        }
    }

    int idx = m_listODFilePath.InsertItem(nItemCount, seFileName);
    m_listODFilePath.SetItemText(idx, 1, odFileName);
    m_listODFilePath.SetItemText(idx, 2, folderPath);

    UpdateListBoxHorizontalScroll();
}


void KFODDiv::OnBnClickedBtnDelete()
{
    // 여러 개 선택되어 있으면 모두 삭제
    int idx = -1;
    while ((idx = m_listODFilePath.GetNextItem(-1, LVNI_SELECTED)) != -1)
    {
        m_listODFilePath.DeleteItem(idx);
    }

    UpdateListBoxHorizontalScroll();
}


void KFODDiv::OnBnClickedRun()
{
    if (!CreateArgument())
        return;
}


bool KFODDiv::CreateArgument()
{
    int selArea = m_cboAreaNumber.GetCurSel();
    if (selArea == CB_ERR)
    {
        AfxMessageBox(_T("분석지역 번호를 선택하십시오."));
        return false;
    }

    int selYear = m_cboDistributionYear.GetCurSel();
    if (selYear == CB_ERR)
    {
        AfxMessageBox(_T("자료 배포년도를 선택하십시오."));
        return false;
    }

    int areaNumber = selArea + 1;

    CString strYear;
    m_cboDistributionYear.GetLBText(selYear, strYear);

    TCHAR szModulePath[MAX_PATH] = { 0 };
    GetModuleFileName(nullptr, szModulePath, MAX_PATH);

    CString strFilePath(szModulePath);
    int pos = strFilePath.ReverseFind(_T('\\'));
    if (pos != -1)
    {
        strFilePath = strFilePath.Left(pos + 1);
    }

    strFilePath += _T("Utility\\FODDiv\\arg_FODDiv.txt");

    CFileStatus status;
    if (CFile::GetStatus(strFilePath, status))
    {
        CFile::Remove(strFilePath);
    }

    CStdioFile file;
    if (!file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
    {
        AfxMessageBox(_T("arg_FODDiv.txt 파일을 생성할 수 없습니다."));
        return false;
    }

    file.WriteString(_T("#분석지역 번호(1:수도권, 2:전국권, 3:부산울산권, 4:대구광역권, 5:대전충청권, 6:광주광역권, 7:제주권)\n"));

    CString line;
    line.Format(_T("%d\n"), areaNumber);
    file.WriteString(line);

    file.WriteString(_T("#분석지역 자료 배포년도\n"));
    file.WriteString(strYear + _T("\n"));

    int nCount = m_listODFilePath.GetItemCount();

    if (nCount > 0)
    {
        file.WriteString(_T(">\n"));
    }

    for (int i = 0; i < nCount; ++i)
    {
        CString folderPath = m_listODFilePath.GetItemText(i, 2);
        CString socioFile = m_listODFilePath.GetItemText(i, 0);
        CString odFile = m_listODFilePath.GetItemText(i, 1);

        file.WriteString(_T("#OD 및 사회경제지표 폴더 경로 :\n"));
        file.WriteString(folderPath + _T("\n"));

        file.WriteString(_T("#사회경제지표 파일명\n"));
        file.WriteString(socioFile + _T("\n"));

        file.WriteString(_T("#입력 OD 파일명 :\n"));
        file.WriteString(odFile + _T("\n"));

        if (i != nCount - 1)
        {
            file.WriteString(_T(">\n"));
        }
    }

    file.Close();
    return true;
}

void KFODDiv::LoadDistributionList()
{
    TCHAR szModulePath[MAX_PATH] = { 0 };
    GetModuleFileName(nullptr, szModulePath, MAX_PATH);

    CString basePath(szModulePath);
    int pos = basePath.ReverseFind(_T('\\'));
    if (pos != -1)
    {
        basePath = basePath.Left(pos + 1);
    }

    CString distPath = basePath + _T("Utility\\FODDiv\\화물OD죤체계\\");

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
            CString folderName = finder.GetFileName();
            m_cboDistributionYear.AddString(folderName);
            addedCount++;
        }
    }
    finder.Close();

    if (addedCount == 0)
    {
        AfxMessageBox(_T("자료 배포년도 폴더가 존재하지 않습니다."));
        return;
    }
}

void KFODDiv::UpdateListBoxHorizontalScroll()
{
    int nCount = m_listODFilePath.GetItemCount();
    int baseWidth = 300;   // 최소 폭 (적당히)
    int maxWidth = baseWidth;

    for (int i = 0; i < nCount; ++i)
    {
        CString text = m_listODFilePath.GetItemText(i, 2); // 폴더 경로
        // 여유를 위해 공백 조금 붙여서 폭 계산
        int width = m_listODFilePath.GetStringWidth(text + _T("   "));
        if (width > maxWidth)
            maxWidth = width;
    }

    // 3번째 컬럼 폭 설정 → 전체 폭이 컨트롤 폭보다 크면 자동으로 가로스크롤 생김
    m_listODFilePath.SetColumnWidth(2, maxWidth);
}



