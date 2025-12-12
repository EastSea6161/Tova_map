#include "stdafx.h"
#include "KDlgLinkZoneTypeEdit.h"
#include "afxdialogex.h"
#include "Target.h"

IMPLEMENT_DYNAMIC(KDlgLinkZoneTypeEdit, CDialogEx)
    BEGIN_MESSAGE_MAP(KDlgLinkZoneTypeEdit, CDialogEx)
        ON_BN_CLICKED(IDOK, &KDlgLinkZoneTypeEdit::OnBnClickedOk)
        ON_BN_CLICKED(IDCANCEL, &KDlgLinkZoneTypeEdit::OnBnClickedCancel)
    END_MESSAGE_MAP()
KDlgLinkZoneTypeEdit::KDlgLinkZoneTypeEdit(KTarget* pTarget, CWnd* pParent /*=NULL*/)
	: CDialogEx(KDlgLinkZoneTypeEdit::IDD, pParent)
{
    m_pTarget = pTarget;
}

KDlgLinkZoneTypeEdit::~KDlgLinkZoneTypeEdit()
{
}

void KDlgLinkZoneTypeEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT1, m_wndReport);
}


BOOL KDlgLinkZoneTypeEdit::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);

    if (true) {
        CXTPReportColumn* pColumn = NULL;
        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Link Type"), 60, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("링크 타입"));
		}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Count"),     35));
        pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("개수"));
		}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Select Zone Connector"),   65));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetEditable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("존 연결링크 선택"));
		}

        m_wndReport.Populate();
    }

    LoadReportData();

    return TRUE;
}

void KDlgLinkZoneTypeEdit::LoadReportData()
{
    m_wndReport.ResetContent();

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T(" link_type, count(*) "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T(" Link "));
        strSQL.Append(_T(" Group By link_type "));
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
            if (nType == 0) {
                pItem->SetChecked(TRUE);
            }
            else {
                pItem->SetChecked(FALSE);
            }
        }
    }

    m_wndReport.Populate();
}

void KDlgLinkZoneTypeEdit::OnBnClickedOk()
{
    try {
        if (UpdateLinkType() == false)
            return;
    }
    catch (...) {

    }

    CDialogEx::OnOK();
}

bool KDlgLinkZoneTypeEdit::UpdateLinkType()
{
    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();

    CXTPReportRecords* pRecords = m_wndReport.GetRecords();
    int nCnt = pRecords->GetCount();

    if (nCnt == 0)
        return true;

    CXTPReportRecord*     pRecord = nullptr;
    CXTPReportRecordItem* pItem   = nullptr;

    std::vector<int> vecZoneType;
    bool bZero2Max(false);
    bool bExistZero(false);

    int nMaxType(0);
    int nZoneCnt(0);
    for (int i=0; i<nCnt; i++) {
        pRecord = pRecords->GetAt(i);
        int nType(0); {
            pItem = pRecord->GetItem(0);
            nType = _ttoi(pItem->GetCaption());

            if (nMaxType <= nType) {
                nMaxType = nType;
            }

            if (nType == 0) {
                bExistZero = true;
            }
        }

        pItem = pRecord->GetItem(2);
        if (pItem->IsChecked() == TRUE) {
            if (nType != 0) {
                vecZoneType.push_back(nType);
            } 

            nZoneCnt++;
        }
        else {
            if (nType == 0) {
                bZero2Max = true;
            }            
        }
    }

    //★ 존커넥터(0) 선택되었고, 나머지는 비선택 -> 변경이 없다.
    if (bExistZero == true && bZero2Max == false && vecZoneType.size() == 0) {
        return true;
    }

    if ( nZoneCnt == 0 ) { //★ 존 커넥터가 없다.
        CString strMsg(_T("TOVA에서 존 노드/존 연결링크의 타입 코드는 0으로 설정됩니다."));
        strMsg.Append(_T("\n선택된 존 연결링크 타입 정보가 없습니다."));

        if (bZero2Max == true) {
            strMsg.AppendFormat(_T("\n타입코드 0 -> %d"), nMaxType+1);
        }

        strMsg.Append(_T("\n계속 진행하시겠습니까?"));

        if ( AfxMessageBox(strMsg, MB_YESNO) == IDNO)
            return false;        
    }
    else {
        CString strMsg(_T("TOVA에서 존 노드/존 연결링크의 타입 코드는 0으로 설정됩니다."));
        if (bZero2Max == true) {
            strMsg.AppendFormat(_T("\n타입코드 0 -> %d"), nMaxType+1);
        }

        if (vecZoneType.size() > 0) {
            CString strTemp(_T(""));
            for (size_t i=0; i<vecZoneType.size(); i++) {
                if (i == 0) {
                    strTemp.AppendFormat(_T("%d"), vecZoneType[i]);
                }
                else {
                    strTemp.AppendFormat(_T(", %d"), vecZoneType[i]);
                }
            }
            strMsg.AppendFormat(_T("\n타입코드 %s -> 0"), strTemp);
        }

        strMsg.Append(_T("\n계속 진행하시겠습니까?"));

        if ( AfxMessageBox(strMsg, MB_YESNO) == IDNO) {
            return false;
        }
    }    
    
    //★ 0번 -> 신규 ID로 변경 
    if (bZero2Max == true) {
        CString strSQL(_T("")); {
            strSQL.Format(_T("Update Link Set link_type = '%d' Where link_type = '0' "), nMaxType+1);
        }

        spDBase->ExecuteUpdate(strSQL);
    }

    if (vecZoneType.size() == 0) {
        return true;
    }

    CString strTemp(_T("")); {
        for (size_t i=0; i<vecZoneType.size(); i++) {
            if (i == 0) {
                strTemp.AppendFormat(_T(" '%d' "), vecZoneType[i]);
            }
            else {
                strTemp.AppendFormat(_T(" ,'%d' "), vecZoneType[i]);
            }
        }
    }

    CString strSQL(_T("")); {
        strSQL.Format(_T("Update Link Set link_type = '0' Where link_type in (%s) "), strTemp);
    }
    spDBase->ExecuteUpdate(strSQL);

    return true;
}


void KDlgLinkZoneTypeEdit::OnBnClickedCancel()
{   
    // ESC 키
    //CDialogEx::OnCancel();
}
