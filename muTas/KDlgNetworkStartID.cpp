#include "stdafx.h"
#include "KDlgNetworkStartID.h"
#include "afxdialogex.h"

#include "MapView.h"

IMPLEMENT_DYNAMIC(KDlgNetworkStartID, CDialogEx)

KDlgNetworkStartID::KDlgNetworkStartID(KMapView* pMapView, CWnd* pParent /*=NULL*/)
	: CDialogEx(KDlgNetworkStartID::IDD, pParent), m_pMapView(pMapView)
{
}

KDlgNetworkStartID::~KDlgNetworkStartID()
{
}

void KDlgNetworkStartID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL KDlgNetworkStartID::OnInitDialog()
{
    if (m_pMapView != nullptr) {
        CString strTemp(_T(""));

        strTemp.Format(_T("%I64d"), m_pMapView->GetStartNodeID());
        SetDlgItemText(IDC_EDIT1, strTemp);

        strTemp.Format(_T("%I64d"), m_pMapView->GetStartLinkID());
        SetDlgItemText(IDC_EDIT2, strTemp);
    }

    return TRUE;
}

BEGIN_MESSAGE_MAP(KDlgNetworkStartID, CDialogEx)
    ON_BN_CLICKED(IDOK, &KDlgNetworkStartID::OnBnClickedOk)
END_MESSAGE_MAP()


void KDlgNetworkStartID::OnBnClickedOk()
{    
    __int64 nxNewNodeID(0);

    if (true) {
        CString strTemp(_T("")); {
            GetDlgItemText(IDC_EDIT1, strTemp);
        }

        __int64 nxNodeID = _ttoi64(strTemp);
        m_pMapView->SetStartNodeID(nxNodeID);
        nxNewNodeID = m_pMapView->GetStartNodeID();

        strTemp.Format(_T("%I64d"), nxNewNodeID);
        SetDlgItemText(IDC_EDIT1, strTemp);

        if (nxNodeID != nxNewNodeID) {
            m_pMapView->SetStartNodeID(nxNewNodeID);
            strTemp.Format(_T("노드 ID : %I64d는 현재 사용 중 입니다."), nxNodeID);
            AfxMessageBox(strTemp);
        }
    }

    __int64 nxNewLinkID(0);
    
    if (true) {
        CString strTemp(_T("")); {
            GetDlgItemText(IDC_EDIT2, strTemp);
        }

        __int64 nxLinkID = _ttoi64(strTemp);
        m_pMapView->SetStartLinkID(nxLinkID);
        nxNewLinkID = m_pMapView->GetStartLinkID();

        strTemp.Format(_T("%I64d"), nxNewLinkID);
        SetDlgItemText(IDC_EDIT2, strTemp);

        if (nxLinkID != nxNewLinkID) {
            m_pMapView->SetStartLinkID(nxNewLinkID);
            strTemp.Format(_T("링크 ID : %I64d는 현재 사용 중 입니다."), nxLinkID);
            AfxMessageBox(strTemp);
        }
    }

    CString strMsg(_T("")); {
        strMsg.Format(_T("아래와 같이 시작 ID 정보가 설정되었습니다.\n노드 : %I64d, 링크 : %I64d"), nxNewNodeID, nxNewLinkID);
    }

    AfxMessageBox(strMsg); 
}
