/**
 * @file AttributeSetDlg.cpp
 * @brief KAttributeSetDlg 구현
 * @author 
 * @date 2010.08.26
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "AttributeSetDlg.h"
#include "anyconverter.h"
#include "MapView.h"
#include "Target.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
#include "BulkDBaseNode.h"
#include "MultiCodeSettingDlg.h"
#define IDD_PROPERTY_GRID 1000

const CString KAttributeSetDlg::SYSTEMFIELDS = _T("System Fields");
const CString KAttributeSetDlg::USERFIELDS	 = _T("User Fields");
/******************************************************************************
* KAttributeSetDlg															  *
******************************************************************************/
IMPLEMENT_DYNAMIC(KAttributeSetDlg, CXTPResizeDialog)

IEditBase* KAttributeSetDlg::m_pEditBase = NULL;


/**************************************
* Constructors / Destructor           *
**************************************/
KAttributeSetDlg::KAttributeSetDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KAttributeSetDlg::IDD, pParent), m_strTableName(_T(""))
{    
	m_pRow = new KIORow;
    m_bApplyOppositeLink = true;
}


KAttributeSetDlg::~KAttributeSetDlg()
{
	if(NULL != m_pRow)
	{
		m_pRow->Clear();
		delete m_pRow;
	}
}



/**************************************
* Accessors                           *
**************************************/
void KAttributeSetDlg::SetEditBase( IEditBase* pEditBase)
{
	if( m_pEditBase != pEditBase )
	{
		m_pRow->Clear();
		m_pEditBase = pEditBase;
	}
}


void KAttributeSetDlg::SetTarget(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


void KAttributeSetDlg::SetColumns(const KIOColumns* pColumns)
{
	m_pColumns = pColumns;
}


void KAttributeSetDlg::SetValues(const KIORow *pRow)
{
	ASSERT(m_pColumns->ColumnCount() == pRow->ItemCount());

	m_pRow->Clear();
	int nItemCount = pRow->ItemCount();
	for(int i = 0; i < nItemCount; ++i)
	{
		m_pRow->AddNewItem(pRow->GetItem(i));
	}
}


KIORow* KAttributeSetDlg::GetValues(void) const
{
	return m_pRow;
}


/**************************************
* Overrides                           *
**************************************/
void KAttributeSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_DUMMY, m_stcDummy);
}


BEGIN_MESSAGE_MAP(KAttributeSetDlg, CXTPResizeDialog)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDOK, &KAttributeSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KAttributeSetDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK1, &KAttributeSetDlg::OnBnClickedCheck1)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
BOOL KAttributeSetDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitPropertyGrid();

	SetResize(IDD_PROPERTY_GRID,	SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC,			SZ_BOTTOM_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDOK,					SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
	
    SetResize(IDC_CHECK1,		    SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
    CButton* pCheckButton = (CButton*)GetDlgItem(IDC_CHECK1);
    pCheckButton->SetCheck(BST_CHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAttributeSetDlg::InitPropertyGrid(void)
{
	CRect rectDummy;
	m_stcDummy.GetWindowRect(&rectDummy);
	ScreenToClient(&rectDummy);

	m_wndPropertyGrid.Create(rectDummy, this, IDD_PROPERTY_GRID);
    m_wndPropertyGrid.HighlightChangedItems(TRUE);
	m_wndPropertyGrid.SetVariableItemsHeight(TRUE);
	m_wndPropertyGrid.SetTheme(xtpGridThemeOffice2007);
	m_wndPropertyGrid.SetShowInplaceButtonsAlways(TRUE);
    m_wndPropertyGrid.NavigateItems(TRUE, FALSE);
	
	CXTPPropertyGridItem* pPropertySystem = m_wndPropertyGrid.AddCategory( SYSTEMFIELDS );
    CXTPPropertyGridItem* pPropertyUser   = m_wndPropertyGrid.AddCategory( USERFIELDS );
       	
	int nCntColumn = m_pColumns->ColumnCount();
	for(int i = 0; i < nCntColumn; i++)
	{
		KIOColumn* pColumn = m_pColumns->GetColumn( i );
        
		if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
			KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( pColumn->CodeGroup() );
			if( NULL == pCodeGroup )
				continue;

			std::vector<int> vecCode;
			pCodeGroup->GetCodes( vecCode );
            CXTPPropertyGridItemEnum* pGridItem = NULL;

            if (pColumn->ColumnType() == KEMIOColumnTypeSystem)
            {
                pGridItem = (CXTPPropertyGridItemEnum*)(pPropertySystem->AddChildItem(new CXTPPropertyGridItemEnum(pColumn->Caption())));
            }
            else
            {
			    pGridItem = (CXTPPropertyGridItemEnum*)(pPropertyUser->AddChildItem(new CXTPPropertyGridItemEnum(pColumn->Caption())));
            }

			pGridItem->SetItemData((DWORD_PTR)pColumn);

			std::vector<int>::iterator iter, end = vecCode.end();
			for(iter = vecCode.begin(); iter != end; ++iter)
			{
				CString strConst = pCodeGroup->SingleCodeValue(*iter, 1);
				pGridItem->GetConstraints()->AddConstraint(strConst, *iter);
			}

			if(pGridItem->GetConstraints()->GetCount() > 0)
			{
				if(NULL != m_pRow)
				{
					KIOItem* pItem = m_pRow->GetItem(i);
					pGridItem->SetEnum( pItem->GetValueToInt() );
				}
				else
				{
                    CXTPPropertyGridItemConstraints* pConstraints = pGridItem->GetConstraints();
                    if (pConstraints->GetCount() > 0)
                    {
                        CXTPPropertyGridItemConstraint* pConstraint = pConstraints->GetConstraintAt(0);
                        pGridItem->SetEnum(pConstraint->m_dwData);
                    }
				}
			}
		}
		else if(pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
		{
			KIOItem*		pItem		= m_pRow->GetItem(i);
			AddMultiCodeItem(pPropertySystem, pColumn, pItem);
		}
		else
		{
			CXTPPropertyGridItem* pGridItem = NULL;
            
			if (pColumn->ColumnType() == KEMIOColumnTypeSystem)
            {
                pGridItem =	pPropertySystem->AddChildItem(new CXTPPropertyGridItem(pColumn->Caption()));
            }
            else
            {
                pGridItem =	pPropertyUser->AddChildItem(new CXTPPropertyGridItem(pColumn->Caption()));
            }
			pGridItem->SetItemData((DWORD_PTR)pColumn);
			KIOItem* pItem = m_pRow->GetItem(i);
			pGridItem->SetValue( pItem->GetValueAsString() );
		}
	}
	
	pPropertySystem->Expand();
    pPropertyUser->Expand();
	//m_wndPropertyGrid.ShowHelp(TRUE);

    EditableCheck();
}


void KAttributeSetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CXTPResizeDialog::OnSysCommand(nID, lParam);
}


void KAttributeSetDlg::OnBnClickedOk()
{
	CXTPPropertyGridItem* pPropertySystem = m_wndPropertyGrid.GetCategories()->FindItem( SYSTEMFIELDS );
    CXTPPropertyGridItem* pPropertyUser   = m_wndPropertyGrid.GetCategories()->FindItem( USERFIELDS );

	CXTPPropertyGridItems* pGridItems = pPropertySystem->GetChilds();
	int nChildCnt = pGridItems->GetCount();

    CString strNodeID  (_T(""));
    CString strNodeType(_T(""));    

    for (int i = 0; i < nChildCnt; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pGridItem = pGridItems->GetAt( i );
        
        KIOColumn* pColumn = (KIOColumn*)pGridItem->GetItemData();
        if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
        {
            int nConst = pGridItem->GetConstraints()->FindConstraint(pGridItem->GetValue());
            if (nConst >= 0)
            {
                CXTPPropertyGridItemConstraint* pGridConst = pGridItem->GetConstraints()->GetConstraintAt( nConst );
                strValue.Format(_T("%d"), pGridConst->m_dwData);
            }            
        }
        else
        {
            strValue = pGridItem->GetValue();
        }

        if (_tcsicmp(m_strTableName, TABLE_NODE) == 0)
        {
            if(_tcsicmp(pColumn->Name(), COLUMN_NODE_ID) == 0)
            {
                strNodeID = strValue;
            }

            if(_tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0)
            {
                strNodeType = strValue;
            }
        }
    }

	for (int i = 0; i < nChildCnt; i++)
	{
		CString strValue(_T(""));
		CXTPPropertyGridItem* pGridItem = pGridItems->GetAt( i );
		if( pGridItem->GetReadOnly() == TRUE)
			continue;
		
		KIOColumn* pColumn = (KIOColumn*)pGridItem->GetItemData();

		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			int nConst = pGridItem->GetConstraints()->FindConstraint(pGridItem->GetValue());
            if (nConst >= 0)
            {
			    CXTPPropertyGridItemConstraint* pGridConst = pGridItem->GetConstraints()->GetConstraintAt( nConst );
			    strValue.Format(_T("%d"), pGridConst->m_dwData);
            }
		}
		else
		{
			strValue = pGridItem->GetValue();
		}
        
        // 존ID 존재여부 체크
        if (_tcsicmp(m_strTableName, TABLE_NODE) == 0)
        {
            KIOItem* pItem = m_pRow->GetItem( m_pColumns->GetColumnIndex(pColumn) );

            int nNodeIDIndex   = m_pColumns->GetColumnIndex(COLUMN_NODE_ID);
            int nNodeTypeIndex = m_pColumns->GetColumnIndex(COLUMN_NODE_TYPE);

            if (nNodeIDIndex < 0 || nNodeTypeIndex < 0)
                return;

            if(_tcsicmp(pColumn->Name(), COLUMN_ZONE_ID) == 0)
            {
                //CString strOriginNodeType = m_pRow->GetItem(nNodeTypeIndex)->GetValueAsString();
                if (_tcsicmp(strNodeType, _T("0")) == 0)
                {
                    strValue = strNodeID;                    
                }
                else
                {
                    if(!KBulkDBaseNode::ExistZone(m_pTarget, _ttoi64(strValue)))
                    {
                        //AfxMessageBox(_T("존재하지 않는 존 정보입니다."));
                        //return;
                    }
                }                
            }
            pItem->SetValue( strValue );
        }
        else
        {
            KIOItem* pItem = m_pRow->GetItem( m_pColumns->GetColumnIndex(pColumn) );
            pItem->SetValue( strValue );
        }
	}

    pGridItems = pPropertyUser->GetChilds();
    nChildCnt = pGridItems->GetCount();
    for (int i = 0; i < nChildCnt; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pGridItem = pGridItems->GetAt( i );
        if( pGridItem->GetReadOnly() == TRUE)
            continue;

        KIOColumn* pColumn = (KIOColumn*)pGridItem->GetItemData();

        if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
        {
            int nConst = pGridItem->GetConstraints()->FindConstraint(pGridItem->GetValue());
            
            if (nConst >= 0)
            {
                CXTPPropertyGridItemConstraint* pGridConst = pGridItem->GetConstraints()->GetConstraintAt( nConst );
                strValue.Format(_T("%d"), pGridConst->m_dwData);
            }            
        }
        else
        {
            strValue = pGridItem->GetValue();
        }

        KIOItem* pItem = m_pRow->GetItem( m_pColumns->GetColumnIndex(pColumn) );
        pItem->SetValue( strValue );
    }
	OnOK();
}


void KAttributeSetDlg::OnBnClickedCancel()
{
	OnCancel();
}

void KAttributeSetDlg::EditableCheck()
{
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( SYSTEMFIELDS );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    CButton* pCheckButton = (CButton*)GetDlgItem(IDC_CHECK1);
    if(_tcsicmp(m_strTableName, TABLE_LINK) == 0)
    {
        if (pCheckButton)
            pCheckButton->ShowWindow(SW_SHOW);
    }
    else
    {
        if (pCheckButton)
        pCheckButton->ShowWindow(SW_HIDE);
    }

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
        KIOColumn* pColumn    = (KIOColumn*)pPropertyGridItem->GetItemData();

        if(_tcsicmp(m_strTableName, TABLE_NODE) == 0)
        {
            if(_tcsicmp(pColumn->Name(), COLUMN_NODE_ID) == 0)
            {
                pPropertyGridItem->SetReadOnly(TRUE);
            }
            else if(_tcsicmp(pColumn->Name(), COLUMN_ZONE_ID) == 0)
            {
                int nTypeIndex = m_pColumns->GetColumnIndex(COLUMN_NODE_TYPE);
                if (nTypeIndex >= 0)
                {
                    int nType = m_pRow->GetItem(nTypeIndex)->GetValueToInt();
                    if (nType == 0)
                    {
                        pPropertyGridItem->SetReadOnly(TRUE);
                    }    
                    else
                    {
                        pPropertyGridItem->SetReadOnly(FALSE);
                    }
                }                
            }
            else if(_tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0)
            {
                int nTypeIndex = m_pColumns->GetColumnIndex(COLUMN_NODE_TYPE);
                if (nTypeIndex >= 0)
                {
                    int nType = m_pRow->GetItem(nTypeIndex)->GetValueToInt();
                    if (nType == 0)
                    {
                        pPropertyGridItem->SetReadOnly(TRUE);
                    } 
                    else
                    {
                        pPropertyGridItem->SetReadOnly(FALSE);
                    }
                }
            }
            else if(_tcsicmp(pColumn->Name(), COLUMN_NODE_X) == 0)
            {
                pPropertyGridItem->SetReadOnly(TRUE);
            }
            else if(_tcsicmp(pColumn->Name(), COLUMN_NODE_Y) == 0)
            {
                pPropertyGridItem->SetReadOnly(TRUE);
            }
            else
            {
                pPropertyGridItem->SetReadOnly(FALSE);
            }
        }
        else if(_tcsicmp(m_strTableName, TABLE_LINK) == 0)
        {
            if(_tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0)
            {
                pPropertyGridItem->SetReadOnly(TRUE);
            }
            else if(_tcsicmp(pColumn->Name(), COLUMN_FNODE_ID) == 0)
            {
                pPropertyGridItem->SetReadOnly(TRUE);
            }
            else if(_tcsicmp(pColumn->Name(), COLUMN_TNODE_ID) == 0)
            {
                pPropertyGridItem->SetReadOnly(TRUE);
            }
            else
            {
                pPropertyGridItem->SetReadOnly(FALSE);
            }
        }
    }
}

bool KAttributeSetDlg::ApplyOppositeLink()
{
    return m_bApplyOppositeLink;
}


void KAttributeSetDlg::OnBnClickedCheck1()
{
    m_bApplyOppositeLink = false;

    try
    {
        CButton* pCheckButton = (CButton*)GetDlgItem(IDC_CHECK1);
        if (pCheckButton->GetCheck() == BST_CHECKED)
        {
            m_bApplyOppositeLink = true;
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
}


CXTPPropertyGridItem* KAttributeSetDlg::AddMultiCodeItem( CXTPPropertyGridItem* a_pParentGridItem, KIOColumn* a_pIOColumn, KIOItem* a_pIOItem )
{
	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( a_pIOColumn->CodeGroup() );
    
	std::vector<int> vecCode;
	pCodeGroup->GetCodes( vecCode );
	size_t nxCodeCount = vecCode.size();

	CXTPPropertyGridItem* pItem = nullptr;

	pItem = a_pParentGridItem->AddChildItem(new CXTPPropertyGridItem(a_pIOColumn->Caption(), a_pIOItem->GetValueAsString()));
	pItem->SetItemData((DWORD_PTR)a_pIOColumn);
	pItem->SetFlags(xtpGridItemHasExpandButton);

	return pItem;
}


LRESULT KAttributeSetDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	if (wParam == XTP_PGN_INPLACEBUTTONDOWN)
	{
		CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
		CXTPPropertyGridItem* pItem = pButton->GetItem();
		KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
		if (pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
		{
			CString strCodeList = pItem->GetValue();
			KMultiCodeSettingDlg oDlg(m_pTarget, pColumn,strCodeList);
			if (oDlg.DoModal() == IDOK)
			{
				pItem->SetValue(oDlg.GetResult());
			}
		}
	}

	return 0;
}
