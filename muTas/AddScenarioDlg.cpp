/**
 * @file AddScenarioDlg.cpp
 * @brief KAddScenarioDlg 구현
 * @author 
 * @date 2010.07.30
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "AddScenarioDlg.h"



/******************************************************************************
* KAddScenarioDlg                                                             *
******************************************************************************/
IMPLEMENT_DYNAMIC(KAddScenarioDlg, KDialogEx)


/**************************************
* Constructors and Destructor         *
**************************************/
KAddScenarioDlg::KAddScenarioDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KAddScenarioDlg::IDD, pParent)
{
	m_strProjectName = _T("");
	m_strNoOfScenario = _T("");
	m_strTargetYear = _T("");
	m_nNameIndex = 1;
}

KAddScenarioDlg::~KAddScenarioDlg()
{
}



/**************************************
* Operations                          *
**************************************/
void KAddScenarioDlg::updateButtonState(void)
{
	POSITION pos = m_lstScenario.GetFirstSelectedItemPosition();
	if( pos != NULL )
	{
		GetDlgItem(IDC_PRJBTN_MODIFYSCENARIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_PRJBTN_DELETESCENARIO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_PRJBTN_MODIFYSCENARIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_PRJBTN_DELETESCENARIO)->EnableWindow(FALSE);
	}
}



/**************************************
* Overrides                           *
**************************************/
void KAddScenarioDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRJEDT_NAME, m_strProjectName);
	DDX_Text(pDX, IDC_PRJEDT_NOOFSCENARIO, m_strNoOfScenario);
	DDX_Text(pDX, IDC_PRJEDT_TARGETYEAR, m_strTargetYear);
	DDX_Control(pDX, IDC_PRJLST_SCENARIO, m_lstScenario);
}


BOOL KAddScenarioDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  Add extra initialization here
	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE ;
	m_lstScenario.SetExtendedStyle(dwExStyle);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_lstScenario.InsertColumn(0, _T("순서"));
		m_lstScenario.InsertColumn(1, _T("시나리오 이름"));
	}
	else
	{
		m_lstScenario.InsertColumn(0, _T("No."));
		m_lstScenario.InsertColumn(1, _T("Scenario Name"));
    }

	if( m_lstScenario.GetSafeHwnd() )
	{
		CRect rectList;
		int nFirstWidth = 40;
		m_lstScenario.GetClientRect(rectList);
		m_lstScenario.SetColumnWidth(0, nFirstWidth);
		m_lstScenario.SetColumnWidth(1, rectList.Width() - nFirstWidth);
	}
	
	m_nNewScenarioCount = 0;
	m_strNoOfScenario.Format( _T("%d"), m_nNewScenarioCount );

	GetDlgItem(IDC_PRJBTN_MODIFYSCENARIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PRJBTN_DELETESCENARIO)->EnableWindow(FALSE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



// KAddScenarioDlg message handlers
BEGIN_MESSAGE_MAP(KAddScenarioDlg, KDialogEx)
	ON_BN_CLICKED(IDC_PRJBTN_ADDSCENARIO, &KAddScenarioDlg::OnClickedPrjbtnAddscenario)
	ON_BN_CLICKED(IDC_PRJBTN_DELETESCENARIO, &KAddScenarioDlg::OnClickedPrjbtnDeletescenario)
	ON_BN_CLICKED(IDC_PRJBTN_MODIFYSCENARIO, &KAddScenarioDlg::OnClickedPrjbtnModifyscenario)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PRJLST_SCENARIO, &KAddScenarioDlg::OnLvnItemchangedPrjlstScenario)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_PRJLST_SCENARIO, &KAddScenarioDlg::OnLvnBeginlabeleditPrjlstScenario)
	ON_BN_CLICKED(IDOK, &KAddScenarioDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_PRJLST_SCENARIO, &KAddScenarioDlg::OnLvnEndlabeleditPrjlstScenario)
	ON_NOTIFY(LVN_INSERTITEM, IDC_PRJLST_SCENARIO, &KAddScenarioDlg::OnLvnInsertitemPrjlstScenario)
END_MESSAGE_MAP()

/**************************************
* Message handlers                    *
**************************************/
void KAddScenarioDlg::OnClickedPrjbtnAddscenario()
{
	int nItemCount = m_lstScenario.GetItemCount();
	CString strNumber, strName;
	strNumber.Format( _T("%d"), nItemCount + 1 );
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strName.Format( _T("%s%d"), _T("시나리오"), m_nNameIndex++);
	} else {
		strName.Format( _T("%s%d"), _T("Scenario"), m_nNameIndex++);
	}
	nItemCount = m_lstScenario.InsertItem(nItemCount, strNumber);
	m_lstScenario.SetItemText(nItemCount, 1, strName );
	m_lstScenario.editSubItem(nItemCount, 1);
	++m_nNewScenarioCount;
	m_strNoOfScenario.Format( _T("%d"), m_nNewScenarioCount );
	UpdateData( FALSE );

	updateButtonState();
}


void KAddScenarioDlg::OnClickedPrjbtnDeletescenario()
{
	POSITION pos = m_lstScenario.GetFirstSelectedItemPosition();
	if( NULL != pos )
	{
		int nItem = m_lstScenario.GetNextSelectedItem(pos);
		m_lstScenario.DeleteItem(nItem);
		CString strText = m_lstScenario.GetItemText(nItem, 0);
		while( strText.IsEmpty() == false )
		{
			CString strNumber;
			strNumber.Format( _T("%d"), nItem+1);
			m_lstScenario.SetItemText(nItem++, 0, strNumber);
			strText = m_lstScenario.GetItemText(nItem, 0);
		}
		--m_nNewScenarioCount;
		m_strNoOfScenario.Format( _T("%d"), m_nNewScenarioCount );
		UpdateData( FALSE );
	}
	updateButtonState();
}


void KAddScenarioDlg::OnClickedPrjbtnModifyscenario()
{
	POSITION pos = m_lstScenario.GetFirstSelectedItemPosition();
	if( NULL != pos )
	{
		int nItem = m_lstScenario.GetNextSelectedItem(pos);
		m_lstScenario.editSubItem(nItem, 1);
	}
	updateButtonState();
}


void KAddScenarioDlg::OnLvnItemchangedPrjlstScenario(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	updateButtonState();
}


void KAddScenarioDlg::OnLvnBeginlabeleditPrjlstScenario(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	
	CRect rectSubItem;
	m_lstScenario.GetSubItemRect(pDispInfo->item.iItem, 1, LVIR_BOUNDS, rectSubItem);


	*pResult = 0;
}


void KAddScenarioDlg::OnBnClickedOk()
{
    if ( ! CheckDuplicateScenarioName() )
    {
        return;
    }

	int nItemCount = m_lstScenario.GetItemCount();
	for( int i = 0; i < nItemCount; ++i )
	{
		CString strName = m_lstScenario.GetItemText(i, 1);

		CString strTemp = strName;
		strTemp.Trim();

		if(strTemp.IsEmpty() == true)
		{
			continue;
		}

		m_arrScenario.push_back(strName);
	}


	KDialogEx::OnOK();
}

bool KAddScenarioDlg::CheckDuplicateScenarioName()
{
    int nItemCount = m_lstScenario.GetItemCount();
    if (nItemCount <= 0)
    {
        AfxMessageBox( _T("입력 정보가 없습니다.") );
        return false; /*입력이 없는 경우*/
    }

    CString strScenarioName(_T(""));

    for (int i=0; i<nItemCount; i++)
    {
        strScenarioName = m_lstScenario.GetItemText(i, 1);
        strScenarioName = strScenarioName.Trim();
        m_lstScenario.SetItemText(i, 1, strScenarioName);

        if (strScenarioName.IsEmpty())
        {
            AfxMessageBox( _T("시나리오 이름은 비워두거나 공백으로 만들 수 없습니다.\n시나리오 이름을 입력해 주세요.") );
            m_lstScenario.editSubItem( i, 1 );
            return false;
        }

		//Base는 추가할 수 없도록
		if (strScenarioName.CompareNoCase(_T("BASE")) == 0)
		{
			CString strMsg;
			strMsg.Format(_T("사용할 수 없는 시나리오 명칭입니다."));
			AfxMessageBox(strMsg);
			m_lstScenario.editSubItem( i, 1 );
			return false;
		}

		std::set<CString>::iterator bSetIter = m_ExistsNameSet.begin();

		while (bSetIter != m_ExistsNameSet.end())
		{
			CString strExistName = *bSetIter;

			if (strExistName.CompareNoCase(strScenarioName) == 0)
			{
				CString strMsg;
				strMsg.Format( _T("%s : 동일한 이름의 시나리오가 이미 존재합니다."), strScenarioName);
				AfxMessageBox( strMsg );
				m_lstScenario.editSubItem( i, 1 );
				return false;
			}

			++bSetIter;
		}

		//폴더는 대소문자가 다르더라도 동일하게 인식하여 오류발생

//         if( m_ExistsNameSet.find( strScenarioName ) != m_ExistsNameSet.end() )
//         {
//             CString strMsg;
//             strMsg.Format( _T("%s : 동일한 이름의 시나리오가 존재 합니다."), strScenarioName);
//             AfxMessageBox( strMsg );
//             m_lstScenario.editSubItem( i, 1 );
//             return false;
//         }

        CString strErroMsg(_T(""));
		if ( !QBicStringChecker::IsValidFileName(strScenarioName, strErroMsg))
		{
			AfxMessageBox(_T("시나리오 이름에는 다음과 같은 문자를 허용하지 않습니다.\r\n\\/:*?\\<>|"));
			m_lstScenario.editSubItem( i, 1 );
			return false;
		}

        for (int k=i+1; k<nItemCount; k++)
        {
            CString strNextName = m_lstScenario.GetItemText( k, 1 );
            strNextName = strNextName.Trim();

            if ( strScenarioName == strNextName )
            {
                CString strMsg;
                strMsg.Format( _T("%s : 동일한 이름의 시나리오가 이미 존재합니다."), strScenarioName);
                AfxMessageBox( strMsg );
                m_lstScenario.editSubItem( i, 1 );
                return false;
            }
        }
    }    
    return true;
}

void KAddScenarioDlg::OnLvnEndlabeleditPrjlstScenario(NMHDR *pNMHDR, LRESULT *pResult)
{
    return;

	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	CString strNewName = m_lstScenario.GetItemText( pDispInfo->item.iItem, 1 );
// 	m_lstScenario.SetItemText( pDispInfo->item.iItem, 1, pDispInfo->item.pszText);
// 	CString strNewName = pDispInfo->item.pszText;

	CString strTemp = strNewName;
	strTemp.Trim();
	if( strTemp.IsEmpty() == true )
	{
		AfxMessageBox( _T("시나리오 이름은 비워두거나 공백으로 만들 수 없습니다.\n시나리오 이름을 입력해 주세요.") );
		m_lstScenario.editSubItem( pDispInfo->item.iItem, 1 );
		
		*pResult = 0;
		return;
	}

	bool bExistName = false;

	if( m_ExistsNameSet.find( strNewName ) != m_ExistsNameSet.end() )
	{
		bExistName = true;
	}

	if( bExistName == false )
	{
		int nItemCount = m_lstScenario.GetItemCount();

		for( int i = 0; i < nItemCount; ++i )
		{
			if( i == pDispInfo->item.iItem )
			{
				continue;
			}
			CString strExistName = m_lstScenario.GetItemText( i, 1 );
			
			if( strExistName == strNewName )
			{
				bExistName = true;
			}
		}
	}
	if( true == bExistName )
	{
		CString strMsg;
		strMsg.Format( _T("%s : 동일한 이름의 시나리오가 이미 존재합니다."), strNewName);
		AfxMessageBox( strMsg );
		m_lstScenario.editSubItem( pDispInfo->item.iItem, 1 );

		return;
	}
	
	*pResult = 0;
}


void KAddScenarioDlg::OnLvnInsertitemPrjlstScenario(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}
