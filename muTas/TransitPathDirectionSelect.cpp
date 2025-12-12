// TransitPathDirectionSelect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TransitPathDirectionSelect.h"
#include "afxdialogex.h"


// KTransitPathDirectionSelect 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTransitPathDirectionSelect, KDialogEx)

KTransitPathDirectionSelect::KTransitPathDirectionSelect(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTransitPathDirectionSelect::IDD, pParent), m_windowPosition(0, 0), m_nSelectedIndex(-1), m_bEnablePass(true), m_nInitIndex(0)
{

}

KTransitPathDirectionSelect::~KTransitPathDirectionSelect()
{
}

void KTransitPathDirectionSelect::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_cboSelect);
}


BEGIN_MESSAGE_MAP(KTransitPathDirectionSelect, KDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO1, &KTransitPathDirectionSelect::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDOK, &KTransitPathDirectionSelect::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL KTransitPathDirectionSelect::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    SetWindowPos(NULL, m_windowPosition.x, m_windowPosition.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    SetDlgItemText(IDC_STATIC_INFO, m_strCaption);
    size_t nxCount = m_vecSelect.size();

    for (size_t i=0; i<nxCount; i++)
    {
        m_cboSelect.AddString(m_vecSelect[i]);
    }

    if (nxCount  > 0)
    {
        if (m_nInitIndex < (int)nxCount)
        {
            m_cboSelect.SetCurSel(m_nInitIndex);
            m_nSelectedIndex = m_nInitIndex;
        }
        else
        {
            m_cboSelect.SetCurSel(0);
            m_nSelectedIndex = 0;
        }        
    }

    if (!m_bEnablePass)
    {
        GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
    }

    return TRUE;
}

void KTransitPathDirectionSelect::OnCbnSelchangeCombo1()
{
    try
    {
        int nSeq = m_cboSelect.GetCurSel();
        m_nSelectedIndex = nSeq;
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

void KTransitPathDirectionSelect::SetSelect( CString a_strCaption, std::vector<CString> a_vecSelect, int a_nInitIndex )
{
    try
    {        
        m_strCaption = a_strCaption;
        m_vecSelect  = a_vecSelect;    
        m_nInitIndex = a_nInitIndex;
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

int KTransitPathDirectionSelect::GetSelectedIndex()
{
    return m_nSelectedIndex;
}

void KTransitPathDirectionSelect::OnBnClickedOk()
{
    int nSeq(-1);

    try
    {
        nSeq = m_cboSelect.GetCurSel();

        if (nSeq < 0)
        {
            m_cboSelect.SetFocus();
            return;
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

    KDialogEx::OnOK();
}

void KTransitPathDirectionSelect::SetEnablePass( bool a_bEnablePass /*= true*/ )
{
    m_bEnablePass = a_bEnablePass;
}


