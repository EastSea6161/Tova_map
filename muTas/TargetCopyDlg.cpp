/**
 * @file TargetCopyDlg.cpp
 * @brief KTargetCopyDlg 구현
 * @author 
 * @date 2010.09.07
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "TargetCopyDlg.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "ImChampDir.h"


/******************************************************************************
* KTargetCopyDlg                                                              *
******************************************************************************/
IMPLEMENT_DYNAMIC(KTargetCopyDlg, KDialogEx)


/**************************************
* Constructors / Destructor           *
**************************************/
KTargetCopyDlg::KTargetCopyDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTargetCopyDlg::IDD, pParent)
{

}


KTargetCopyDlg::~KTargetCopyDlg()
{
}



/**************************************
* Operations                          *
**************************************/
void KTargetCopyDlg::addScenarioToTree( KScenario* pScenario )
{
	HTREEITEM hItem = m_treeTarget.InsertItem( pScenario->GetName() );
	m_treeTarget.SetItemData( hItem, (DWORD_PTR)pScenario );

	const KTDTargetMap& targetMap = pScenario->GetTargetMap();
	KTDTargetMap::const_iterator citTargetMap = targetMap.begin();
	while( targetMap.end() != citTargetMap )
	{
		KTarget* pTarget = citTargetMap->second;
		
		if(pTarget != m_pSourceTarget)
		{
			addTargetToTree( hItem, pTarget );
		}
		++citTargetMap;
	}
	m_treeTarget.Expand( hItem, TVE_EXPAND );
}


void KTargetCopyDlg::addTargetToTree( HTREEITEM hScenario, KTarget* pTarget )
{
	HTREEITEM hItem = m_treeTarget.InsertItem( pTarget->GetName(), hScenario );
	m_treeTarget.SetItemData( hItem, (DWORD_PTR)pTarget );
}


bool KTargetCopyDlg::CopyTarget( void )
{
	CWaitCursor wait;

	HTREEITEM hScenario = m_treeTarget.GetRootItem();
	while( NULL != hScenario )
	{
		HTREEITEM hTarget = m_treeTarget.GetChildItem( hScenario );
		while( NULL != hTarget )
		{
			if( m_treeTarget.getStateImage( hTarget ) == TREE_CHECK )
			{
				KTarget* pDestTarget = (KTarget*)m_treeTarget.GetItemData( hTarget );

				if (!pDestTarget->IsEmptyData())
				{
					CString strMsg  = _T("해당 분석 년도에 데이터가 존재합니다.\n분석 년도 복사 실행 후 기존 데이터는 삭제됩니다.\n계속하시겠습니까?");
					int     nResult = AfxMessageBox(strMsg, MB_YESNO);

					if (nResult == IDNO)
						return false;
				}

				CMainFrameWnd* pMainFrame   = (CMainFrameWnd*)(AfxGetMainWnd());
				CChildFrame*   pChildeFrame = pMainFrame->GetChildFrame(pDestTarget->GetObjectID());

				if (pChildeFrame != NULL)
					pChildeFrame->SendMessage(WM_CLOSE);

				// 2019.05.28 : Free 버전 관련 수정
				pDestTarget->CopyFrom( m_pSourceTarget );

				if (IsDlgButtonChecked(IDC_CHECK1))
				{
					CString strFromPath = m_pSourceTarget->GetLocation();
					CString strToPath   = pDestTarget->GetLocation();

					strFromPath.AppendFormat(_T("\\%s"), DLL_MODEL_FOLDER_NAME);

					if (GetFileAttributes(strFromPath) != 0xFFFFFFFF)
					{
						ImChampDir::CopyDirectoryWinSimpleProgress(strFromPath, strToPath);
					}
				}
			}
			hTarget = m_treeTarget.GetNextSiblingItem( hTarget );
		}

		hScenario = m_treeTarget.GetNextSiblingItem( hScenario );
	}

	return true;
}



/**************************************
* Overrides                           *
**************************************/
BOOL KTargetCopyDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if( NULL != m_pSourceTarget )
	{
		CString strSource;
		GetDlgItem( IDC_STATIC )->GetWindowText( strSource );
		KScenario* pSourceScenario = (KScenario*)m_pSourceTarget->GetParentObject();
		strSource += pSourceScenario->GetName();
		strSource += _T(" - ");
		strSource += m_pSourceTarget->GetName();
		GetDlgItem( IDC_STATIC )->SetWindowText( strSource );
	}

	CBitmap bmp;
	bmp.LoadBitmap( IDB_3STATE_CHECK );

	m_ilTreeState.Create( 13, 13, ILC_COLOR24 | ILC_MASK, 4, 1 );
	m_ilTreeState.Add( &bmp, RGB(255, 255, 255) );

	m_treeTarget.SetImageList( &m_ilTreeState, TVSIL_STATE );
	
	CKmzApp* pApp = (CKmzApp*)AfxGetApp();
	KProject* pProject = pApp->GetProject();

	//Add Base logic
	KScenario*          pBaseScenario = pProject->GetBaseScenario();
	const KTDTargetMap& mapBaseTarget = pBaseScenario->GetTargetMap();

	if (mapBaseTarget.size() > 0)
	{
		bool bAdd(false);

		KTDTargetMap::const_iterator citTargetMap = mapBaseTarget.begin();

		while (mapBaseTarget.end() != citTargetMap)
		{
			KTarget* pTarget = citTargetMap->second;

			if(pTarget != m_pSourceTarget)
				bAdd = true;

			++citTargetMap;
		}

		if (bAdd)
		{
			addScenarioToTree( pBaseScenario );
		}
	}
	
	//Add None Base Scenario logic
	const KTDScenarioMap& scenarioMap = pProject->GetScenarioMap();
	KTDScenarioMap::const_iterator citScenarioMap = scenarioMap.begin();
	while( scenarioMap.end() != citScenarioMap )
	{
		KScenario*          pScenario = citScenarioMap->second;
		const KTDTargetMap& targetMap = pScenario->GetTargetMap();

		if (targetMap.size() > 0)
		{
			bool bAdd(false);

			KTDTargetMap::const_iterator citTargetMap = targetMap.begin();
			while( targetMap.end() != citTargetMap )
			{
				KTarget* pTarget = citTargetMap->second;

				if(pTarget != m_pSourceTarget)
				{
					bAdd = true;
				}
				++citTargetMap;
			}

			if (bAdd)
			{
				addScenarioToTree( pScenario );
			}
		}
		++citScenarioMap;
	}

	//화물 프로젝트의 경우 타겟 복사 시 Inclue Model Results 체크박스를 안나오게 수정
	if (KmzSystem::IsFreightSystem())
		GetDlgItem(IDC_CHECK1)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTargetCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeTarget);
}



BEGIN_MESSAGE_MAP(KTargetCopyDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KTargetCopyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTargetCopyDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void KTargetCopyDlg::OnBnClickedOk()
{
	if (CopyTarget())
	{
		OnOK();
	}
	else
	{
		OnCancel();
	}
}


void KTargetCopyDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}
