/**
 * @file SelectTargetYearDlg.cpp
 * @brief KSelectTargetYearDlg ±¸Çö
 * @author 
 * @date 2010.07.30
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "SelectTargetYearDlg.h"



// KSelectTargetYearDlg dialog
/******************************************************************************
* KSelectTargetYearDlg                                                        *
******************************************************************************/
IMPLEMENT_DYNAMIC(KSelectTargetYearDlg, KDialogEx)


/**************************************
* Constructors and Destructor         *
**************************************/
KSelectTargetYearDlg::KSelectTargetYearDlg(CWnd* pParent /*=NULL*/)
	:	KDialogEx(KSelectTargetYearDlg::IDD, pParent),
		m_strSelectYear( _T("") )
{

}


KSelectTargetYearDlg::~KSelectTargetYearDlg()
{
}




/**************************************
* Operations                          *
**************************************/
void KSelectTargetYearDlg::updateTargetYearEdit(void)
{
	m_strSelectYear = _T("");

	//std::map<CString, CString>::iterator it = m_mapCheckedYear.begin();
	//while( m_mapCheckedYear.end() != it )
	//{
	//	if( m_strSelectYear.IsEmpty() == false )
	//	{
	//		m_strSelectYear += _T(", ");
	//	}
	//	m_strSelectYear += it->second;
	//	++it;
	//}

	std::set< int >::iterator itTargetYearSet = m_TargetYearSet.begin();
	while( m_TargetYearSet.end() != itTargetYearSet )
	{
		CString strYear;
		strYear.Format( _T("%d"), *itTargetYearSet );
		if( m_strSelectYear.IsEmpty() == false )
		{
			m_strSelectYear += _T(", ");
		}
		m_strSelectYear += strYear;
		++itTargetYearSet;
	}

	UpdateData(FALSE);
}


void KSelectTargetYearDlg::addCheckedYear( const CString& strYear )
{
	m_mapCheckedYear.insert( std::make_pair( strYear, strYear ) );
}


void KSelectTargetYearDlg::addTargetYear( int nYear )
{
	m_TargetYearSet.insert( nYear );
}




/**************************************
* Overrides                           *
**************************************/
void KSelectTargetYearDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TYCHK_YEAR01, m_btnYear01);
	DDX_Control(pDX, IDC_TYCHK_YEAR02, m_btnYear02);
	DDX_Control(pDX, IDC_TYCHK_YEAR03, m_btnYear03);
	DDX_Control(pDX, IDC_TYCHK_YEAR04, m_btnYear04);
	DDX_Control(pDX, IDC_TYCHK_YEAR05, m_btnYear05);
	DDX_Control(pDX, IDC_TYCHK_YEAR06, m_btnYear06);
	DDX_Control(pDX, IDC_TYCHK_YEAR07, m_btnYear07);
	DDX_Control(pDX, IDC_TYCHK_YEAR08, m_btnYear08);
	DDX_Control(pDX, IDC_TYCHK_YEAR09, m_btnYear09);
	DDX_Control(pDX, IDC_TYCHK_YEAR10, m_btnYear10);
	DDX_Control(pDX, IDC_TYCHK_YEAR11, m_btnYear11);
	DDX_Control(pDX, IDC_TYCHK_YEAR12, m_btnYear12);
	DDX_Control(pDX, IDC_TYCHK_YEAR13, m_btnYear13);
	DDX_Control(pDX, IDC_TYCHK_YEAR14, m_btnYear14);
	DDX_Control(pDX, IDC_TYCHK_YEAR15, m_btnYear15);
	DDX_Control(pDX, IDC_TYCHK_YEAR16, m_btnYear16);
	DDX_Control(pDX, IDC_TYCHK_YEAR17, m_btnYear17);
	DDX_Control(pDX, IDC_TYCHK_YEAR18, m_btnYear18);
	DDX_Control(pDX, IDC_TYCHK_YEAR19, m_btnYear19);
	DDX_Control(pDX, IDC_TYCHK_YEAR20, m_btnYear20);
	DDX_Control(pDX, IDC_TYCHK_YEAR21, m_btnYear21);
	DDX_Control(pDX, IDC_TYCHK_YEAR22, m_btnYear22);
	DDX_Control(pDX, IDC_TYCHK_YEAR23, m_btnYear23);
	DDX_Control(pDX, IDC_TYCHK_YEAR24, m_btnYear24);
	DDX_Control(pDX, IDC_TYCHK_YEAR25, m_btnYear25);
	DDX_Control(pDX, IDC_TYCHK_YEAR26, m_btnYear26);
	DDX_Control(pDX, IDC_TYCHK_YEAR27, m_btnYear27);
	DDX_Control(pDX, IDC_TYCHK_YEAR28, m_btnYear28);
	DDX_Control(pDX, IDC_TYCHK_YEAR29, m_btnYear29);
	DDX_Control(pDX, IDC_TYCHK_YEAR30, m_btnYear30);
	DDX_Control(pDX, IDC_TYCHK_YEAR31, m_btnYear31);
	DDX_Control(pDX, IDC_TYCHK_YEAR32, m_btnYear32);
	DDX_Control(pDX, IDC_TYCHK_YEAR33, m_btnYear33);
	DDX_Control(pDX, IDC_TYCHK_YEAR34, m_btnYear34);
	DDX_Control(pDX, IDC_TYCHK_YEAR35, m_btnYear35);
	DDX_Text(pDX, IDC_TYEDT_SELECTED, m_strSelectYear);
	DDX_Control(pDX, IDC_TYBTN_PERVYEAR, m_btnPrev);
	DDX_Control(pDX, IDC_TYBTN_NEXTYEAR, m_btnNext);
}


BOOL KSelectTargetYearDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	// TODO:  Add extra initialization here

	/// Load image list
	CBitmap bmp;
	bmp.LoadBitmap( IDB_SELECTTARGET_BUTTON );

	m_ilButtonImage.Create( 16, 16, ILC_COLOR24 | ILC_MASK , 2, 1 );
	m_ilButtonImage.Add( &bmp, RGB(255, 255, 255) );

	m_btnPrev.SetIcon( m_ilButtonImage.ExtractIcon(0) );
	m_btnNext.SetIcon( m_ilButtonImage.ExtractIcon(1) );

	m_arryButtons.push_back(&m_btnYear01);
	m_arryButtons.push_back(&m_btnYear02);
	m_arryButtons.push_back(&m_btnYear03);
	m_arryButtons.push_back(&m_btnYear04);
	m_arryButtons.push_back(&m_btnYear05);
	m_arryButtons.push_back(&m_btnYear06);
	m_arryButtons.push_back(&m_btnYear07);
	m_arryButtons.push_back(&m_btnYear08);
	m_arryButtons.push_back(&m_btnYear09);
	m_arryButtons.push_back(&m_btnYear10);
	m_arryButtons.push_back(&m_btnYear11);
	m_arryButtons.push_back(&m_btnYear12);
	m_arryButtons.push_back(&m_btnYear13);
	m_arryButtons.push_back(&m_btnYear14);
	m_arryButtons.push_back(&m_btnYear15);
	m_arryButtons.push_back(&m_btnYear16);
	m_arryButtons.push_back(&m_btnYear17);
	m_arryButtons.push_back(&m_btnYear18);
	m_arryButtons.push_back(&m_btnYear19);
	m_arryButtons.push_back(&m_btnYear20);
	m_arryButtons.push_back(&m_btnYear21);
	m_arryButtons.push_back(&m_btnYear22);
	m_arryButtons.push_back(&m_btnYear23);
	m_arryButtons.push_back(&m_btnYear24);
	m_arryButtons.push_back(&m_btnYear25);
	m_arryButtons.push_back(&m_btnYear26);
	m_arryButtons.push_back(&m_btnYear27);
	m_arryButtons.push_back(&m_btnYear28);
	m_arryButtons.push_back(&m_btnYear29);
	m_arryButtons.push_back(&m_btnYear30);
	m_arryButtons.push_back(&m_btnYear31);
	m_arryButtons.push_back(&m_btnYear32);
	m_arryButtons.push_back(&m_btnYear33);
	m_arryButtons.push_back(&m_btnYear34);
	m_arryButtons.push_back(&m_btnYear35);

	int nYear = m_nBaseYear+1;
	std::vector<CButton*>::iterator it = m_arryButtons.begin();

	while( m_arryButtons.end() != it )
	{
		CString strText;
		strText.Format(_T("%d"), nYear);
		(*it)->SetWindowText(strText);
		
		//std::map< CString, CString >::iterator itCheckedYear = m_mapCheckedYear.find( strText );
		//if( m_mapCheckedYear.end() != itCheckedYear )
		//{
		//	(*it)->SetCheck( BST_CHECKED );
		//}
		std::set< int >::iterator itTargetYearSet = m_TargetYearSet.find( nYear );
		if( m_TargetYearSet.end() != itTargetYearSet )
		{
			(*it)->SetCheck( BST_CHECKED );
		}

		++it, ++nYear;
	}
	updateTargetYearEdit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




BEGIN_MESSAGE_MAP(KSelectTargetYearDlg, KDialogEx)
	ON_BN_CLICKED(IDC_TYBTN_PERVYEAR, &KSelectTargetYearDlg::OnBnClickedTybtnPervyear)
	ON_BN_CLICKED(IDC_TYBTN_NEXTYEAR, &KSelectTargetYearDlg::OnBnClickedTybtnNextyear)
	ON_COMMAND_RANGE(IDC_TYCHK_YEAR01, IDC_TYCHK_YEAR35, &KSelectTargetYearDlg::OnCheckYearRange)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void KSelectTargetYearDlg::OnBnClickedTybtnPervyear()
{
	CString strYear;
	m_btnYear01.GetWindowText(strYear);
	int nYear = ::_ttoi(strYear);
	nYear -= 5;
	std::vector<CButton*>::iterator it = m_arryButtons.begin();
	while( m_arryButtons.end() != it )
	{
		strYear.Format(_T("%d"), nYear);
		(*it)->SetWindowText(strYear);
		//std::map<CString, CString>::iterator itYear = m_mapCheckedYear.find(strYear);
		//if( m_mapCheckedYear.end() != itYear )
		//{
		//	(*it)->SetCheck(BST_CHECKED);
		//}
		//else
		//{
		//	(*it)->SetCheck(BST_UNCHECKED);
		//}

		std::set< int >::iterator itTargetYearSet = m_TargetYearSet.find( nYear );
		if( m_TargetYearSet.end() != itTargetYearSet )
		{
			(*it)->SetCheck( BST_CHECKED );
		}
		else
		{
			(*it)->SetCheck( BST_UNCHECKED );
		}

		++it, ++nYear;
	}
}


void KSelectTargetYearDlg::OnBnClickedTybtnNextyear()
{
	CString strYear;
	m_btnYear01.GetWindowText(strYear);
	int nYear = ::_ttoi(strYear);
	nYear += 5;
	std::vector<CButton*>::iterator it = m_arryButtons.begin();
	while( m_arryButtons.end() != it )
	{
		strYear.Format(_T("%d"), nYear);
		(*it)->SetWindowText(strYear);

		//std::map<CString, CString>::iterator itYear = m_mapCheckedYear.find(strYear);
		//if( m_mapCheckedYear.end() != itYear )
		//{
		//	(*it)->SetCheck(BST_CHECKED);
		//}
		//else
		//{
		//	(*it)->SetCheck(BST_UNCHECKED);
		//}

		std::set< int >::iterator itTargetYearSet = m_TargetYearSet.find( nYear );
		if( m_TargetYearSet.end() != itTargetYearSet )
		{
			(*it)->SetCheck( BST_CHECKED );
		}
		else
		{
			(*it)->SetCheck( BST_UNCHECKED );
		}

		++it, ++nYear;
	}
}


void KSelectTargetYearDlg::OnCheckYearRange(UINT nID)
{
	CButton* pbtnYear = (CButton*)GetDlgItem(nID);
	CString strYear;
	pbtnYear->GetWindowText(strYear);
	int nYear = _ttoi( strYear );

	BOOL bChecked = IsDlgButtonChecked(nID);
	if( TRUE == bChecked )
	{
		//std::map<CString, CString>::iterator it = m_mapCheckedYear.find(strYear);
		//if( m_mapCheckedYear.end() == it )
		//{
		//	m_mapCheckedYear.insert( std::make_pair(strYear, strYear) );
		//}

		std::set< int >::iterator itTargetYearSet = m_TargetYearSet.find( nYear );
		if( m_TargetYearSet.end() == itTargetYearSet )
		{
			m_TargetYearSet.insert( nYear );
		}
	}
	else
	{
		//std::map<CString, CString>::iterator it = m_mapCheckedYear.find(strYear);
		//if( m_mapCheckedYear.end() != it )
		//{
		//	m_mapCheckedYear.erase(it);
		//}

		std::set< int >::iterator itTargetYearSet = m_TargetYearSet.find( nYear );
		if( m_TargetYearSet.end() != itTargetYearSet )
		{
			m_TargetYearSet.erase( itTargetYearSet );
		}
	}
	updateTargetYearEdit();
}




