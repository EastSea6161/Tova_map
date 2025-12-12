// GravityCalibrationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImChampDir.h"
#include "afxcmn.h"
#include "GravityCalibrationDlg.h"
#include "afxdialogex.h"
#include "GravityTreeCtrl.h"
#include "Target.h"
#include "KEMTree.h"
#include "Purpose.h"
#include "Mode.h"
#include "model_common.h"
#include "DBaseAssignment.h"
#include "DBaseCalibration.h"
#include "KBulkDBase2File.h"
#include "Launcher.h"

// KGravityCalibrationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGravityCalibrationDlg, CXTPResizeDialog)

KGravityCalibrationDlg::KGravityCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KGravityCalibrationDlg::IDD, pParent)
	, m_nIntervalGrade(0)
	, m_dIntervalError(0.0)
	, m_nSelPurposeCount(0)
{
	m_dEntropyLBound = 0.0;
	m_dEntropyUBound = 0.0;

	m_hICon = AfxGetApp()->LoadIcon(IDR_DLG_ICO);
}

KGravityCalibrationDlg::~KGravityCalibrationDlg()
{
}

void KGravityCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Text(pDX,		IDC_EDIT1,	m_nIntervalGrade);
	DDX_Text(pDX,		IDC_EDIT2,	m_dIntervalError);
	DDX_Text(pDX,		IDC_EDIT3,	m_dEntropyUBound);
	DDX_Text(pDX,		IDC_EDIT4,	m_dEntropyLBound);
	DDX_Radio(pDX,		IDC_RADIO1,	m_nBtnRadioIndex);
	DDX_Control(pDX,	IDC_TREE1,	m_Tree);
	DDX_Control(pDX,	IDC_COMBO1,	m_cboIntervalVariable);
	DDX_Control(pDX,	IDC_COMBO2,	m_cboEntropyVariable);

	//ZNumberEdit
	DDX_Control(pDX,	IDC_EDIT2,	m_edtIntervalError);
	DDX_Control(pDX,	IDC_EDIT3,	m_edtEntropyUBound);
	DDX_Control(pDX,	IDC_EDIT4,	m_edtEntropyLBound);
}


BEGIN_MESSAGE_MAP(KGravityCalibrationDlg, CXTPResizeDialog)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &KGravityCalibrationDlg::OnNMClickTree)
	ON_NOTIFY(TVN_SELCHANGED,  IDC_TREE1, &KGravityCalibrationDlg::OnTvnSelchangedTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, &KGravityCalibrationDlg::OnTvnSelchangingTree)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KGravityCalibrationDlg::OnBnClickedRadio)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO3, IDC_RADIO4, &KGravityCalibrationDlg::OnBnClickedMatrixRadio)
	ON_BN_CLICKED(IDOK, &KGravityCalibrationDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KGravityCalibrationDlg::OnCbnSelchangeInterVariableCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KGravityCalibrationDlg::OnCbnSelchangeEntropyVariableCombo)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


BOOL KGravityCalibrationDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->wParam == VK_RETURN ||
		pMsg->wParam == VK_ESCAPE)
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);

		return TRUE;
	}
	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}



// KGravityCalibrationDlg 메시지 처리기입니다.


void KGravityCalibrationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hICon);
	}
	else
	{
		CXTPResizeDialog::OnPaint();
	}
}


void KGravityCalibrationDlg::SetTarget( KTarget* a_pTaget )
{
	m_pTarget = a_pTaget;
}


//step 0: tree 데이터 구성
void KGravityCalibrationDlg::InitTreeCtrl()
{
	m_ImgTreeList.DeleteImageList();
	m_Tree.DeleteAllItems();

	TxLogDebugStartMsg();
	CBitmap BMP;
	BMP.LoadBitmap(IDB_GRAVITYTREE);
	m_ImgTreeList.Create(18, 17, ILC_COLOR24, 6, 0);
	m_ImgTreeList.Add(&BMP, RGB(255, 0, 0));
	m_Tree.SetImageList(&m_ImgTreeList,TVSIL_NORMAL);
	InsertTreeItemPurpose();
	TxLogDebugEndMsg();
	
}

void KGravityCalibrationDlg::InsertTreeItemPurpose()
{
	TxLogDebugStartMsg();
	if(NULL == m_pTarget)
		return;

	std::map<int, std::vector<CString>> mapMatrixColumnInfo;

	m_mapGravityInfo.clear();

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3);

	KIOTables* pIOTables = m_pTarget->Tables();
	
	if (pButton->GetCheck() == BST_CHECKED)	//Purpose
	{
		KDBaseAssignment::GetDistributionODAtGroupingPurpose(m_pTarget, mapMatrixColumnInfo);
		std::map<int, std::vector<CString>>::iterator bIter, eIter = mapMatrixColumnInfo.end();

		KIOTable*         pIOTable = pIOTables->FindTable(TABLE_PURPOSE_OD);
		const KIOColumns* pColumns = pIOTable->Columns();
		KIOColumn*        pColumn  = nullptr;

		for (bIter = mapMatrixColumnInfo.begin(); bIter != eIter; ++bIter)
		{
			std::vector<CString> vecColumName = bIter->second;
			int                  nSize        = vecColumName.size();

			for (int i = 0; i < nSize; ++i)
			{

				pColumn = pColumns->GetColumn(vecColumName[i]);

				if (pColumn == nullptr)
					continue;

				CString   strPurposeName = pColumn->Caption();
				HTREEITEM hItemPurpose = m_Tree.InsertItem(strPurposeName, IMAGE_CHILDEN, IMAGE_CHILDEN, TVI_ROOT);

				STGravityCaliInfo oSTGravityCailInfo;
				oSTGravityCailInfo.strPurposeName = strPurposeName;
				oSTGravityCailInfo.nGrade = 5;
				oSTGravityCailInfo.GravityFunction = KEMIntervalRatio;
				oSTGravityCailInfo.dToleranceError = 0.1;
				m_mapGravityInfo.insert(std::make_pair(hItemPurpose, oSTGravityCailInfo));
			}
		}
	}
	else	//Mode
	{
		KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pTarget, mapMatrixColumnInfo);
		std::map<int, std::vector<CString>>::iterator bIter, eIter = mapMatrixColumnInfo.end();

		KIOTable*         pIOTable = pIOTables->FindTable(TABLE_MODE_OD);
		const KIOColumns* pColumns = pIOTable->Columns();
		KIOColumn*        pColumn  = nullptr;

		for (bIter = mapMatrixColumnInfo.begin(); bIter != eIter; ++bIter)
		{
			std::vector<CString> vecColumName = bIter->second;
			int                  nSize        = vecColumName.size();

			for (int i = 0; i < nSize; ++i)
			{
				pColumn = pColumns->GetColumn(vecColumName[i]);

				if (pColumn == nullptr)
					continue;

				CString   strModeName = pColumn->Caption();
				HTREEITEM hItemMode   = m_Tree.InsertItem(strModeName, IMAGE_CHILDEN, IMAGE_CHILDEN, TVI_ROOT);

				STGravityCaliInfo oSTGravityCailInfo;
				oSTGravityCailInfo.strPurposeName = strModeName;
				oSTGravityCailInfo.nGrade = 5;
				oSTGravityCailInfo.GravityFunction = KEMIntervalRatio;
				oSTGravityCailInfo.dToleranceError = 0.1;
				m_mapGravityInfo.insert(std::make_pair(hItemMode, oSTGravityCailInfo));
			}
		}
	}
		
	TxLogDebugEndMsg();
}

void KGravityCalibrationDlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	TxLogDebugStartMsg();
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMTreeView);

	ClearComboControl();

	//클릭된 아이템 가져오기
	HTREEITEM hItemCheck = pNMTreeView->itemNew.hItem;
	if (hItemCheck != NULL)
	{
		//if (m_Tree.GetCheck(hItemCheck))
		{
			std::map<HTREEITEM, STGravityCaliInfo>::iterator findIter = m_mapGravityInfo.find(hItemCheck);
			std::map<HTREEITEM, STGravityCaliInfo>::iterator end      = m_mapGravityInfo.end();
			if (findIter != end)
			{
				STGravityCaliInfo oSTGravityInfo;
				oSTGravityInfo = findIter->second;
				GetTreeValue(oSTGravityInfo);
			}
		}
	}
	UpdateData(FALSE);
	TxLogDebugEndMsg();
}


void KGravityCalibrationDlg::OnTvnSelchangingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	TxLogDebugStartMsg();
	UpdateData(TRUE);
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	
	if( NULL == hItem )
	{
		return;
	}
	std::map<HTREEITEM, STGravityCaliInfo>::iterator findIter = m_mapGravityInfo.find(hItem);
	std::map<HTREEITEM, STGravityCaliInfo>::iterator end      = m_mapGravityInfo.end();
	if (findIter != end)
	{
		STGravityCaliInfo& oSTGravityInfo = findIter->second;
		if(SaveSelectedItem(hItem, oSTGravityInfo) == false && m_Tree.GetCheck(hItem) == TRUE)
		{
			CString strErrmsg = _T("");
			strErrmsg.Format(_T("%s의 값을 지정하지 않았습니다."), oSTGravityInfo.strPurposeName);
			AfxMessageBox(strErrmsg);
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
	TxLogDebugEndMsg();
}

void KGravityCalibrationDlg::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	DWORD pos = ::GetMessagePos();
	CPoint point ( LOWORD (pos), HIWORD (pos));

	m_Tree.ScreenToClient(&point);

	UINT nFlags = 0;

	HTREEITEM hItem = m_Tree.HitTest(point, &nFlags);

	if (NULL == hItem)
		return;

	m_Tree.Expand(hItem, TVE_EXPAND);

	if (m_Tree.GetCheck(hItem) && (nFlags & TVHT_ONITEMSTATEICON) != 0)
	{
		UnCheckedChildItems(hItem);
	}
	else
	{
		CheckChildItems(hItem);
		m_Tree.SelectItem(hItem);
	}

	*pResult = 0;
}

void KGravityCalibrationDlg::CheckChildItems( HTREEITEM a_hItem )
{
	m_Tree.Expand(a_hItem, TVE_EXPAND);
	CheckParentsItems(a_hItem);

	HTREEITEM hChildItem = m_Tree.GetNextItem(a_hItem,TVGN_CHILD);

	while (hChildItem != NULL)
	{
		m_Tree.SetCheck(hChildItem, TRUE);
		if (m_Tree.GetNextItem(hChildItem, TVGN_CHILD) != NULL)
		{
			CheckChildItems(hChildItem);
		}
		hChildItem = m_Tree.GetNextItem(hChildItem, TVGN_NEXT);
	}

}

void KGravityCalibrationDlg::CheckParentsItems( HTREEITEM a_hItem )
{
	HTREEITEM hParentItem = m_Tree.GetNextItem(a_hItem,TVGN_PARENT);
	if (hParentItem != NULL)
	{
		m_Tree.SetCheck(hParentItem, TRUE);
		CheckParentsItems(hParentItem);
	}
}

void KGravityCalibrationDlg::UnCheckedChildItems( HTREEITEM a_hItem )
{
	m_Tree.Expand(a_hItem, TVE_EXPAND);
	HTREEITEM hChildItem = m_Tree.GetNextItem(a_hItem, TVGN_CHILD);

	while (hChildItem != NULL)
	{
		m_Tree.SetCheck(hChildItem, FALSE);
		if (m_Tree.GetNextItem(hChildItem, TVGN_CHILD) != NULL)
		{
			UnCheckedChildItems(hChildItem);
		}
		hChildItem = m_Tree.GetNextItem(hChildItem, TVGN_NEXT);
	}
	hChildItem = m_Tree.GetNextItem(hChildItem, TVGN_NEXT);
}

// step 1 :다이얼로그 구성 

BOOL KGravityCalibrationDlg::OnInitDialog()
{
	TxLogDebugStartMsg();
	CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	UpdateData(TRUE);

	m_nBtnRadioIndex = 0;

// 	SetIcon(m_hICon, TRUE);
// 	SetIcon(m_hICon, FALSE);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3);
	pButton->SetCheck(BST_CHECKED);

	try
	{
		InitTreeCtrl();
		InitVariableCombo();
		UpdateControlState();
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		CXTPResizeDialog::OnCancel();
	}	
	catch (...)
	{
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		CXTPResizeDialog::OnCancel();
	}
	m_edtIntervalError.EnableRealNumber(true);
	m_edtIntervalError.EnableMinMax(0.0, 1.0);

	m_edtEntropyUBound.EnableRealNumber(true);
	m_edtEntropyUBound.EnableMinMax(0.0, 2.0);

	m_edtEntropyLBound.EnableRealNumber(true);
	m_edtEntropyLBound.EnableMinMax(0.0, 2.0);

	UpdateData(FALSE);
	TxLogDebugEndMsg();


	return TRUE;  
}


void KGravityCalibrationDlg::InitVariableCombo()
{
	TxLogDebugStartMsg();
	m_cboIntervalVariable.ResetContent();
	m_cboEntropyVariable.ResetContent();
	
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
	
	if(NULL == pTable)
	{
		CString strErrorMsg(_T(""));
		strErrorMsg.Format(_T("'%s'가 존재하지 않습니다."), TABLE_PARAMETER_OD);
		ThrowException(strErrorMsg);
	}
		

	const KIOColumns*	pColumns		= pTable->Columns();
	int							nColumnCount	= pColumns->ColumnCount();
	int							nComboIndex(0);

	nComboIndex = m_cboIntervalVariable.InsertString(-1, _T("-------- Select --------"));
	m_cboIntervalVariable.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	nComboIndex = m_cboEntropyVariable.InsertString(-1,  _T("-------- Select --------"));
	m_cboEntropyVariable.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		
		if(pColumn->DataType() == KEMIODataTypeDouble)
		{
			nComboIndex = m_cboIntervalVariable.InsertString(-1, pColumn->Caption());
			m_cboIntervalVariable.SetItemData(nComboIndex, (DWORD_PTR)pColumn);
			
			nComboIndex = m_cboEntropyVariable.InsertString(-1, pColumn->Caption());
			m_cboEntropyVariable.SetItemData(nComboIndex, (DWORD_PTR)pColumn);
		}
	}
	TxLogDebugEndMsg();
}


void KGravityCalibrationDlg::GetTreeValue( STGravityCaliInfo a_oGavityInfo )
{
	TxLogDebugStartMsg();

	UpdateData(TRUE);
	KEMGravityFunction nFunctionType = a_oGavityInfo.GravityFunction;
	
	int nRadioindex = 0;
	switch(nFunctionType)

	{
	case KEMIntervalRatio:	
		m_cboIntervalVariable.SetCurSel(a_oGavityInfo.nVariableIndex);
		m_nIntervalGrade = a_oGavityInfo.nGrade;
		m_dIntervalError = a_oGavityInfo.dToleranceError;
		nRadioindex = 0;
		break;

	case KEMEntropyMaximization:
		m_cboEntropyVariable.SetCurSel(a_oGavityInfo.nVariableIndex);
		m_dEntropyUBound = a_oGavityInfo.dUpperBound;
		m_dEntropyLBound = a_oGavityInfo.dLowerBound;
		nRadioindex = 1;
		break;

	default:
		return;
	}

	m_nBtnRadioIndex = nRadioindex;
	UpdateControlState();
	UpdateData(FALSE);
	TxLogDebugEndMsg();
}


bool KGravityCalibrationDlg::SaveSelectedItem(HTREEITEM a_hItem , STGravityCaliInfo& a_oGavityInfo )
{
	TxLogDebugStartMsg();
	a_oGavityInfo.nGrade          = m_nIntervalGrade;
	a_oGavityInfo.dUpperBound      = m_dEntropyUBound;
	a_oGavityInfo.dLowerBound     = m_dEntropyLBound;
	a_oGavityInfo.dToleranceError = m_dIntervalError;

	int     nFunctionNo    = 0;
	int     nUnitIndex     = 0;
	int     nVariableIndex = 0;

	switch(m_nBtnRadioIndex)
	{
	case 0:	
		a_oGavityInfo.GravityFunction = KEMIntervalRatio;
		nVariableIndex = m_cboIntervalVariable.GetCurSel();
		if (nVariableIndex == -1 || nUnitIndex == -1 )
		{
			return false;
		}
		break;

	case 1:	
		a_oGavityInfo.GravityFunction = KEMEntropyMaximization;
		nVariableIndex = m_cboEntropyVariable.GetCurSel();
		if (nVariableIndex == -1 )
		{
			return false;
		}
		break;

	default:
		return false;
	}

	a_oGavityInfo.nVariableIndex = nVariableIndex;

	TxLogDebugEndMsg();
	return true;
}

void KGravityCalibrationDlg::UpdateControlState()
{
	TxLogDebugStartMsg();
	switch(m_nBtnRadioIndex)
	{
	case 0:	
		m_cboIntervalVariable.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);

		m_cboEntropyVariable.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
		break;

	case 1:	
		m_cboIntervalVariable.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);

		m_cboEntropyVariable.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
		break;


	default:
		return;
	}
	UpdateData(FALSE);
	TxLogDebugEndMsg();
}



void KGravityCalibrationDlg::OnBnClickedRadio(UINT nID)
{
	UpdateData(TRUE);
	UpdateControlState();
	UpdateData(FALSE);
}



void KGravityCalibrationDlg::OnCbnSelchangeInterVariableCombo()
{
	this->SetFocus();
}


void KGravityCalibrationDlg::OnCbnSelchangeImpedVariavleCombo()
{
	this->SetFocus();
}


void KGravityCalibrationDlg::OnCbnSelchangeEntropyVariableCombo()
{
	this->SetFocus();
}

void KGravityCalibrationDlg::ClearComboControl()
{
	UpdateData(TRUE);
	m_cboIntervalVariable.SetCurSel(0);
	m_cboEntropyVariable.SetCurSel(0);

	m_dEntropyLBound = 0.0;
	m_dEntropyUBound = 0.0;
	m_nIntervalGrade = 0;
	m_dIntervalError = 0.0;
	UpdateData(FALSE);
}

void KGravityCalibrationDlg::OnBnClickedOk()
{
	TxLogDebugStartMsg();
	try
	{
		ImChampDir::SetCurrentDirectoryToApp();

		ValidateInput();
		RunCalibration();
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("failed to run"));
		return;
	}

	TxLogDebugEndMsg();
}


CString KGravityCalibrationDlg::GetVariableName( int a_nComboIndex, KEMGravityFunction a_eumGravityFuncton )
{
	CString strVarialbeName = _T("");

	if (a_eumGravityFuncton == KEMIntervalRatio)
	{
		m_cboIntervalVariable.GetLBText(a_nComboIndex, strVarialbeName);
	}
	else
	{
		m_cboEntropyVariable.GetLBText(a_nComboIndex, strVarialbeName);
	}
	return strVarialbeName;
}

void KGravityCalibrationDlg::ValidateInput( void )
{
	UpdateData(TRUE);

	m_nSelPurposeCount = 0;

	try
	{
		HTREEITEM hLastItem = m_Tree.GetSelectedItem();
		std::map<HTREEITEM, STGravityCaliInfo>::iterator findIter = m_mapGravityInfo.find(hLastItem);
		std::map<HTREEITEM, STGravityCaliInfo>::iterator endIter  = m_mapGravityInfo.end();
		
		if (findIter != endIter)
		{
 			STGravityCaliInfo& oSTGravityInfo= findIter->second;
			SaveSelectedItem(hLastItem, oSTGravityInfo);
		}
		std::map<HTREEITEM, STGravityCaliInfo>::iterator iter = m_mapGravityInfo.begin();
		std::map<HTREEITEM, STGravityCaliInfo>::iterator end  = m_mapGravityInfo.end();
 		
		while(iter != end)
		{
			HTREEITEM hItem = iter->first;
			bool bUsePurPose = false;
			
			if (m_Tree.GetCheck(hItem) == TRUE)
			{
				++m_nSelPurposeCount;
				bUsePurPose = true;
			}
			STGravityCaliInfo& oGravityInfo = iter->second;
			oGravityInfo.bSelected = bUsePurPose;

			if (bUsePurPose)
			{
				if (KEMIntervalRatio == oGravityInfo.GravityFunction)
				{
					KIOColumn* pColumn = (KIOColumn*)m_cboIntervalVariable.GetItemData(oGravityInfo.nVariableIndex);

					if (NULL == pColumn)
						ThrowException(_T("Parameter 컬럼을 선택하세요."));
				}
				else if(KEMEntropyMaximization == oGravityInfo.GravityFunction)
				{
					KIOColumn* pColumn = (KIOColumn*)m_cboEntropyVariable.GetItemData(oGravityInfo.nVariableIndex);

					if (NULL == pColumn)
						ThrowException(_T("Parameter 컬럼을 선택하세요."));
				}
				else
				{
					ThrowException(_T("method type error"));
				}
			}

			++iter;
		}

		CString strUnselectMsg(_T(""));

		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3);

		if (pButton->GetCheck() == BST_CHECKED)
		{
			strUnselectMsg = _T("선택된 목적이 없습니다.");
		}
		else
		{
			strUnselectMsg = _T("선택된 수단이 없습니다.");
		}

		if (0 == m_nSelPurposeCount)
			ThrowException(strUnselectMsg);

		/*iter = m_mapGravityInfo.begin();*/
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KGravityCalibrationDlg::RunCalibration( void )
{
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(GravityCalibrationThreadCaller, this, false, nLang);
}

void KGravityCalibrationDlg::CreateArgument( void )
{
	ImChampDir::SetCurrentDirectoryToApp();

	TxLogDebugStartMsg();
	CString strArgFileName	= _T("GravityCalibration.arg");
	UINT	nOpenMode		= CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strArgFileName, nOpenMode);

	CString strOutputMsg(_T(""));

	//#1 Total Zone Number
	int nTotalZoneCount = KDBaseCalibration::SelectZoneCount(m_pTarget);

	if(0 == nTotalZoneCount)
		ThrowException(_T("Zone 데이터가 없습니다."));
	strOutputMsg.Format(_T("noZone\t%d\r\n"), nTotalZoneCount);
	of.WriteString(strOutputMsg);

	//#2. Number of purpose
	strOutputMsg.Format(_T("noPurpose\t%d\r\n"), m_nSelPurposeCount);
	of.WriteString(strOutputMsg);


	KIOColumns oMatrixColumns;
	KIOColumns oParameterColumns;

	//#3. Pindex_Pname_Type_Interval_EG_Up_Low
	int nPurposeIndex(0);

	std::map<HTREEITEM, STGravityCaliInfo>::iterator iter	= m_mapGravityInfo.begin();
	std::map<HTREEITEM, STGravityCaliInfo>::iterator end	= m_mapGravityInfo.end();

	CString strTableMatrixOD(_T(""));

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3);

	if (pButton->GetCheck() == BST_CHECKED)	//Purpose
	{
		strTableMatrixOD = TABLE_PURPOSE_OD;
	}
	else
	{
		strTableMatrixOD = TABLE_MODE_OD;
	}

	while(iter != end)
	{
		STGravityCaliInfo& oGravityInfo = iter->second;
		if(!oGravityInfo.bSelected)
		{
			++iter;
			continue;
		}

		if (KEMIntervalRatio == oGravityInfo.GravityFunction)
		{
			KIOColumn* pColumn = (KIOColumn*)m_cboIntervalVariable.GetItemData(oGravityInfo.nVariableIndex);

			if (NULL == pColumn)
				ThrowException(_T("Parameter 컬럼을 선택하세요."));
			oParameterColumns.AddNewColumn(pColumn);
		}
		else if(KEMEntropyMaximization == oGravityInfo.GravityFunction)
		{
			KIOColumn* pColumn = (KIOColumn*)m_cboEntropyVariable.GetItemData(oGravityInfo.nVariableIndex);

			if (NULL == pColumn)
				ThrowException(_T("Parameter 컬럼을 선택하세요."));
			oParameterColumns.AddNewColumn(pColumn);
		}
		else
		{
			ThrowException(_T("method type error"));
		}


		++nPurposeIndex;

		if (KEMIntervalRatio == oGravityInfo.GravityFunction)
		{
			strOutputMsg.Format(_T("%d\t%s\t%d\t%d\t%f\t0\t0\r\n"),
				nPurposeIndex, oGravityInfo.strPurposeName, oGravityInfo.GravityFunction,
				oGravityInfo.nGrade, oGravityInfo.dToleranceError);
		}
		else if(KEMEntropyMaximization == oGravityInfo.GravityFunction)
		{
			strOutputMsg.Format(_T("%d\t%s\t%d\t0\t0\t%f\t%f\r\n"),
				nPurposeIndex, oGravityInfo.strPurposeName, oGravityInfo.GravityFunction,
				oGravityInfo.dUpperBound, oGravityInfo.dLowerBound);
		}
		else
		{
			ThrowException(_T("method type error"));
		}
		of.WriteString(strOutputMsg);

		CString strColumnName = oGravityInfo.strPurposeName;

		KIOTables*        pTables  = m_pTarget->Tables();
		KIOTable*         pTable   = pTables->FindTable(strTableMatrixOD);
		const KIOColumns* pColumns = pTable->Columns();

		int nColumnCount = pColumns->ColumnCount();

		for (int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);

			if(strColumnName.CompareNoCase(pColumn->Caption()) == 0)
				oMatrixColumns.AddNewColumn(pColumn);
		}

		++iter;
	}
	of.Close();
	
	bool bResult = false;
	//#4. write purpose_od
	bResult = KBulkDBase2File::ExportDB2ODFile(
		m_pTarget->GetDBaseConnection(), strTableMatrixOD, oMatrixColumns, FILE_NAME_OUT_PURPOSE_OD);

	if (!bResult)
	{
		TxLogDebugException();
		ThrowException(_T("Argument 생성(ExportDB2PurposeOD) 중 오류가 발생하였습니다. !!!"));	
	}

	//#5. write parameter_od
	bResult = KBulkDBase2File::ExportDB2ODFile(
		m_pTarget->GetDBaseConnection(), TABLE_PARAMETER_OD, oParameterColumns, FILE_NAME_OUT_PARAMETER_OD);

	if (!bResult)
	{
		ThrowException(_T("Argument 생성(ExportDB2ParameterOD) 중 오류가 발생하였습니다. !!!"));
		TxLogDebugException();
	}

	bResult = KBulkDBase2File::ExportDB2ZoneIDFile(m_pTarget->GetDBaseConnection());

	if (!bResult)
	{
		ThrowException(_T("Argument 생성(ExportDB2ZoneID) 중 오류가 발생하였습니다. !!!"));
		TxLogDebugException();
	}
	
	//#6 Run Dll...
	m_nResultCode = DLL_UNEXPECTED_ERROR;
	CString strWindowName;
	GetWindowText(strWindowName);

	AddStatusMessage(_T("> Gravity Calibration Run Start ..."));

	KLauncher::Run(strWindowName, 37);

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		AddStatusMessage(_T("> Gravity Calibration End ..."));
		TxLogDebug(_T("Model Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		AddStatusMessage(_T("> Gravity Calibration Stop ..."));
		TxLogDebug(_T("Model Stop.."));

		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Dll Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));

		return;
	}
	else
	{
		AddStatusMessage(_T("> Gravity Calibration End(Error) ..."));
		TxLogDebug(_T("Gravity Calibration Run Error.."));

		return;
	}

	TxLogDebugEndMsg();
}

unsigned __stdcall KGravityCalibrationDlg::GravityCalibrationThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter	  = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*		  pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("중력정산 모형을 구동중입니다."));

	try	
	{
		KGravityCalibrationDlg* pDlg = (KGravityCalibrationDlg*)pParameter->GetParameter();
		pDlg->CreateArgument();
		pDlg->ViewResultTxt();
	}
	catch (KExceptionPtr ex)
	{
		pProgressWindow->SetStatus(ex->GetErrorMessage());
		Sleep(3000);
	}
	catch (...)
	{
		pProgressWindow->SetStatus(_T("수행 중 오류가 발생하였습니다."));
		Sleep(3000);
	}
	return 0;
}

void KGravityCalibrationDlg::ReceiveStatusMessageFromDll( void* pParam1, int nStatus, char* strMsg )
{
	CString strDllMsg(strMsg);

	QBicSimpleProgressPtr pProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	pProgressWindow->SetStatus(strDllMsg);
}

void KGravityCalibrationDlg::ViewResultTxt( void )
{
	CString   strFileName = _T("IntervalFactor.txt");
	CFileFind cFileFinder;

	if(FALSE == cFileFinder.FindFile(strFileName) )
	{
		CString strMsg(_T(""));
		strMsg.Format(_T("%s 파일이 존재 하지 않습니다."), strFileName);
		ThrowException(strMsg);
	}
	ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOW);    // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
}

void KGravityCalibrationDlg::OnBnClickedMatrixRadio( UINT nID )
{
	switch (nID)
	{
	case IDC_RADIO3:
		InitTreeCtrl();
		InitVariableCombo();
		UpdateControlState();
		break;

	case IDC_RADIO4:
		InitTreeCtrl();
		InitVariableCombo();
		UpdateControlState();
		break;

	default:
		break;
	}
}


BOOL KGravityCalibrationDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	try
	{
		int     nMsgCode = pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}   

	return CXTPResizeDialog::OnCopyData(pWnd, pCopyDataStruct);
}


void KGravityCalibrationDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}