#include "stdafx.h"
#include "KDlgTurnTypeEdit.h"
#include "afxdialogex.h"

#include "Target.h"

IMPLEMENT_DYNAMIC(KDlgTurnTypeEdit, CDialogEx)

    BEGIN_MESSAGE_MAP(KDlgTurnTypeEdit, CDialogEx)
        ON_BN_CLICKED(IDOK, &KDlgTurnTypeEdit::OnBnClickedOk)
        ON_BN_CLICKED(IDCANCEL, &KDlgTurnTypeEdit::OnBnClickedCancel)
    END_MESSAGE_MAP()

KDlgTurnTypeEdit::KDlgTurnTypeEdit(KTarget* pTarget, CWnd* pParent /*=NULL*/)
	: CDialogEx(KDlgTurnTypeEdit::IDD, pParent)
{
    m_pTarget = pTarget;
}

KDlgTurnTypeEdit::~KDlgTurnTypeEdit()
{
}

void KDlgTurnTypeEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT1, m_wndReport);
}

BOOL KDlgTurnTypeEdit::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);

    if (true) {
        CXTPReportColumn* pColumn = NULL;
        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Turn Type"), 50, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("회전유형 코드"));
		}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Count"),     40));
        pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("개수"));
		}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Select Prohibited Turn Type"),   65));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetEditable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("회전금지유형 선택"));
		}
        m_wndReport.Populate();
    }

    LoadTurnInfo();

    return TRUE;
}

void KDlgTurnTypeEdit::LoadTurnInfo()
{
    m_wndReport.ResetContent();

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T(" turn_type, count(*) "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T(" Turn "));
        strSQL.Append(_T(" Group By turn_type "));
    }

    KDBaseConPtr  spDBase  = m_pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

    CXTPReportRecord     *pRecord = NULL;
    CXTPReportRecordItem *pItem   = NULL;
    CString strTemp(_T(""));

    while(spResult->Next()) {
        int nType = spResult->GetValueInt(0);
        int nCnt  = spResult->GetValueInt(1);

        pRecord = m_wndReport.AddRecord(new CXTPReportRecord()); 

        pItem = pRecord->AddItem(new CXTPReportRecordItem); {
            strTemp.Format(_T("%d"), nType);
            pItem->SetCaption(strTemp);
            pItem->SetAlignment(DT_CENTER);
        }
        
        pItem = pRecord->AddItem(new CXTPReportRecordItem); {
            strTemp.Format(_T("%d"), nCnt);
            pItem->SetCaption(strTemp);
            pItem->SetAlignment(DT_CENTER);
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItem); {
            pItem->HasCheckbox(TRUE);
            pItem->SetEditable(TRUE);
            pItem->SetAlignment(xtpColumnIconCenter);
            if (nType == 1) {
                pItem->SetChecked(FALSE);
            }
            else {
                pItem->SetChecked(TRUE);
            }
        }
    }

    m_wndReport.Populate();
}

void KDlgTurnTypeEdit::OnBnClickedOk()
{
    try {
        UpdateTurnType();
    }
    catch (...) {

    }

    CDialogEx::OnOK();
}

void KDlgTurnTypeEdit::UpdateTurnType()
{
    CXTPReportRecords* pRecords = m_wndReport.GetRecords();
    int nCnt = pRecords->GetCount();

    if (nCnt == 0)
        return;

    CXTPReportRecord*     pRecord = nullptr;
    CXTPReportRecordItem* pItem   = nullptr;

    std::vector<int> vecNotAllow;
    std::vector<int> vecAllow;    

    for (int i=0; i<nCnt; i++) {
        pRecord = pRecords->GetAt(i);
        int nType(0); {
            pItem = pRecord->GetItem(0);
            nType = _ttoi(pItem->GetCaption());
        }

        pItem = pRecord->GetItem(2);
        if (pItem->IsChecked() == TRUE) {
            vecNotAllow.push_back(nType);
        }
        else {
            vecAllow.push_back(nType);
        }
    }

    size_t nxNotAllow = vecNotAllow.size();
    size_t nxAllow    = vecAllow.size();

    CString strSQL(_T(""));
    if (nxNotAllow != 0 && nxAllow != 0) {
        //★ 허용과 금지가 석여 있다.
        CString strTemp(_T("")); {
            for (size_t i=0; i<vecAllow.size(); i++) {
                if (i == 0) {
                    strTemp.AppendFormat(_T(" '%d' "), vecAllow[i]);
                }
                else {
                    strTemp.AppendFormat(_T(" ,'%d' "), vecAllow[i]);
                }
            }
        }

        strSQL.Format(_T("Update turn Set turn_type = (Case when turn_type In (%s) then '1' else '0' end)"), strTemp);
    }
    else if (nxNotAllow == 0) {
        //★ 모두 허용
        strSQL = _T("Update turn Set turn_type = '1'");
    }
    else {
        //★ 모두 금지
        strSQL = _T("Update turn Set turn_type = '0'");
    }

    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    spDBase->ExecuteUpdate(strSQL);
}


void KDlgTurnTypeEdit::OnBnClickedCancel()
{   
    // ESC 키
    //CDialogEx::OnCancel();
}
