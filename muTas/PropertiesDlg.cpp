// PropertiesDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PropertiesDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
#include "BulkDBaseLink.h"
#include "ImChampFrameWindow.h"
#include "MapView.h"
#include "NetworkShapeInsert.h"
#include "MultiCodeSettingDlg.h"

#include "KBulkDBaseUtil.h"
#include "BulkDBaseDemarcation.h"
#include "NetworkEditLog.h"

// KPropertiesDlg 대화 상자입니다.
#define IDD_PROPERTY_GRID 1000


IMPLEMENT_DYNAMIC(KPropertiesDlg, KResizeDialogEx)

KPropertiesDlg::KPropertiesDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPropertiesDlg::IDD, pParent), m_pTarget(NULL), m_pTable(NULL), m_nxLinkID(-1), m_nEditFlag(0)
	, m_bEditCheck(TRUE)
{
	CategoryName = _T("Properties");
	TxLogDebugVisitor();
}

void KPropertiesDlg::ShowInformation( KTarget* a_pTarget, CString a_strTableName, CString a_strWhere, BOOL a_bInitEdit/*=FALSE*/)
{
	try
	{
        m_nEditFlag    = 0;

		m_strTableName = a_strTableName;
		m_strWhere     = a_strWhere;

		m_pTarget      = a_pTarget;

        if (m_pTable != nullptr)
            m_pTable->Unregister(this);

		m_pTable       = m_pTarget->Tables()->FindTable(a_strTableName);	

        if (m_pTable != nullptr)
            m_pTable->Register(this);

        m_nxLinkID     = -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw;
	}
    
    try
    {
        LoadUpdateInitialControl();              
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

void KPropertiesDlg::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        if (m_nEditFlag == 0)
            LoadUpdateInitialControl(); 	
        else
            LoadInsertInitialControl();
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


KPropertiesDlg::~KPropertiesDlg()
{
    try
    {
        if (m_pTable != nullptr)
            m_pTable->Unregister(this);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
    
	TxLogDebugEndMsg();
}

void KPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_DUMMY, m_stcDummy);
	DDX_Check(pDX, IDC_CHECK1, m_bEditCheck);
}


BEGIN_MESSAGE_MAP(KPropertiesDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KPropertiesDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_PIC_ZOOM, &KPropertiesDlg::OnStnClickedPicZoom)
	ON_BN_CLICKED(IDC_CHECK1, &KPropertiesDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_BUTTON_COPY, &KPropertiesDlg::OnBnClickedButtonCopy)
    ON_BN_CLICKED(IDC_BUTTON_PASTE, &KPropertiesDlg::OnBnClickedButtonPaste)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, &KPropertiesDlg::OnGridNotify)
END_MESSAGE_MAP()


// KPropertiesDlg 메시지 처리기입니다.
BOOL KPropertiesDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rectDummy;
	m_stcDummy.GetWindowRect(&rectDummy);
	ScreenToClient(&rectDummy);

	m_wndPropertyGrid.Create(rectDummy, this, IDD_PROPERTY_GRID);
	m_wndPropertyGrid.HighlightChangedItems(TRUE);
	m_wndPropertyGrid.SetVariableItemsHeight(TRUE);
	m_wndPropertyGrid.SetTheme(xtpGridThemeOffice2007);
	m_wndPropertyGrid.SetHelpHeight(25);
	m_wndPropertyGrid.SetShowInplaceButtonsAlways(TRUE);
    m_wndPropertyGrid.NavigateItems(TRUE, FALSE);

	CButton* pCheckButton = (CButton*)GetDlgItem(IDC_CHECK2);
	pCheckButton->SetCheck(BST_CHECKED);

	SetResize(IDC_STATIC_NAME,      SZ_TOP_LEFT,	 SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_COPY,      SZ_TOP_RIGHT,	 SZ_TOP_RIGHT);
    SetResize(IDC_BUTTON_PASTE,     SZ_TOP_RIGHT,	 SZ_TOP_RIGHT);
    SetResize(IDD_PROPERTY_GRID,	SZ_TOP_LEFT,	 SZ_BOTTOM_RIGHT);    
	SetResize(IDC_STATIC_HOR1,		SZ_BOTTOM_LEFT,	 SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK1,           SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);
    SetResize(IDC_CHECK2,           SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);
	SetResize(IDOK,					SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT); 
	SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	AddImChampTooltip(IDC_STATIC_NAME, _T("..."));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPropertiesDlg::LoadUpdateInitialControl()
{
	m_wndPropertyGrid.ResetContent();
    InitialButtonStatus();

	try
	{
        CString strTableName = m_pTable->Name();

		const KIOColumns* pIOColumns = m_pTable->Columns();
		int nColumnCount = pIOColumns->ColumnCount();
		
		typedef std::shared_ptr<KIORecordset> KIORecordsetPtr;
		KIORecordsetPtr spIORecordSet (m_pTable->GetRecordset(KEMIORecordsetTypeStatic, pIOColumns, m_strWhere, _T("")));
		int nRowCount = spIORecordSet->GetRowCount();
		if (nRowCount == 0)
			throw 1;

		const KIORow* pIORow = spIORecordSet->GetRow(0);
		CXTPPropertyGridItem* pPropertyGridItemParent = m_wndPropertyGrid.AddCategory( CategoryName );
       	
		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();

		for(int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pIOColumn = pIOColumns->GetColumn( i );     
			KIOItem*   pIOItem   = pIORow->GetItem(i);

            if (_tcsicmp(strTableName, TABLE_TRANSIT) == 0)
            {
                // COLUMN_TRANSIT_TYPE 정보는 숨김 처리
                if (_tcsicmp(pIOColumn->Name(), COLUMN_TRANSIT_TYPE) == 0)
                {
                    continue;
                }
            }

			CXTPPropertyGridItem* pGridItem = NULL;

			if(pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
			{				
				pGridItem = AddCodeItem(pPropertyGridItemParent, pIOColumn, pIOItem);
			}
			else if(pIOColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
			{
				pGridItem = AddMultiCodeItem(pPropertyGridItemParent, pIOColumn, pIOItem);
			}
			else
			{
				pGridItem =	pPropertyGridItemParent->AddChildItem(new CXTPPropertyGridItem(pIOColumn->Caption()));		
                if (pIOColumn->DataType() == KEMIODataTypeDouble) {
                    double dValue = pIOItem->GetValueAsDouble();
                    CString strValue(_T("")); {
                        strValue.Format(_T("%f"), dValue);
                    }
                    pGridItem->SetValue(strValue);
                }
                else {
				    pGridItem->SetValue( pIOItem->GetValueAsString() );
                }
			}

            pGridItem->SetItemData((DWORD_PTR)pIOColumn);

			UpdateEditableCheck(pGridItem, pIOColumn);
		}
	
		pPropertyGridItemParent->Expand();
	}
	catch (int& e)
	{
		switch (e)
		{
		case 1:
			//AfxMessageBox(_T("Record 값이 존재하지 않습니다."));
			break;
		default:
			//AfxMessageBox(_T("Record 조회 중 오류가 발생하였습니다."));
			break;
		}		
	}
	catch (...)
	{
		AfxMessageBox(_T("Record 조회 중 오류가 발생하였습니다."));
	}
}

CXTPPropertyGridItem* KPropertiesDlg::AddCodeItem( CXTPPropertyGridItem* a_pParentGridItem, KIOColumn* a_pIOColumn, KIOItem* a_pIOItem )
{
	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( a_pIOColumn->CodeGroup() );

	int nValue =  a_pIOItem->GetValueToInt();

	std::vector<int> vecCode;
	pCodeGroup->GetCodes( vecCode );
	size_t nxCodeCount = vecCode.size();

    if (m_nEditFlag != 0)
    {
        // 디폴트 설정
        if (vecCode.size() > 0)
            nValue = vecCode[0];
    }

	CXTPPropertyGridItemEnum* pGridItemEnum = NULL;
	pGridItemEnum = (CXTPPropertyGridItemEnum*)(a_pParentGridItem->AddChildItem(new CXTPPropertyGridItemEnum(a_pIOColumn->Caption())));

	pGridItemEnum->SetItemData((DWORD_PTR)a_pIOColumn);
	bool bExistCodeValue = false;
	for(size_t i=0; i<nxCodeCount; i++)
	{
		CString strCaption = pCodeGroup->SingleCodeValue(vecCode[i], 1);
		pGridItemEnum->GetConstraints()->AddConstraint(strCaption, vecCode[i]);

		if (nValue == vecCode[i])
		{
			bExistCodeValue = true;
		}
	}

	if (!bExistCodeValue)
	{
		CString strCaption;
		strCaption.Format(_T("Unknown(%d)"), a_pIOItem->GetValueToInt());
		pGridItemEnum->GetConstraints()->AddConstraint(strCaption, a_pIOItem->GetValueToInt());
	}

	pGridItemEnum->SetEnum(a_pIOItem->GetValueToInt());
	return pGridItemEnum;
}


CXTPPropertyGridItem* KPropertiesDlg::AddMultiCodeItem( CXTPPropertyGridItem* a_pParentGridItem, KIOColumn* a_pIOColumn, KIOItem* a_pIOItem )
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
	
    //멀티코드 편집방법 변경으로 아래 코드 삭제
    //int nSumFlag = MultiCodeConvertSumFlag(a_pIOItem->GetValueAsString());
	//pItem->SetID(nSumFlag);

	return pItem;
}

void KPropertiesDlg::UpdateEditableCheck( CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn )
{
	if (a_pIOColumn->PrimaryKey())
	{
		a_pPropertyGridItem->SetReadOnly();
		return;
	}

	CString strColumnName = a_pIOColumn->Name();

	if (m_strTableName.CompareNoCase(TABLE_NODE) == 0)
	{
		if (strColumnName.CompareNoCase(COLUMN_NODE_X) == 0)
		{
			a_pPropertyGridItem->SetReadOnly();
			return;
		}
		if (strColumnName.CompareNoCase(COLUMN_NODE_Y) == 0)
		{
			a_pPropertyGridItem->SetReadOnly();
			return;
		}
		if (strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0)
		{
			a_pPropertyGridItem->SetReadOnly();
			return;
		}
	}

	if (m_strTableName.CompareNoCase(TABLE_LINK) == 0)
	{		
        if (strColumnName.CompareNoCase(COLUMN_FNODE_ID) == 0)
        {
            a_pPropertyGridItem->SetReadOnly();
            return;
        }

        if (strColumnName.CompareNoCase(COLUMN_TNODE_ID) == 0)
        {
            a_pPropertyGridItem->SetReadOnly();
            return;
        }

        if (strColumnName.CompareNoCase(COLUMN_LINK_TYPE) == 0)
        {
            a_pPropertyGridItem->SetReadOnly();
            return;
        }
	}
}

void KPropertiesDlg::OnBnClickedOk()
{
    try
    {
    	if (UpdateDataCheck() == false)
            return;

		if (AfxMessageBox(_T("저장하시겠습니까?"), MB_YESNO) == IDNO) {
			return;
		}

        if (m_nEditFlag == 0)
        {
            UpdateSave();
        }
        else
        {
            // 노드, 링크, 존은 추가할 수 없음.
            // 네트워크 편집항목은 제외. 
            // 현재 버전은 네트워크 제외 부분을 고려하고 있지는 않고 있으므로
            // 메뉴 실행 단계에서 제어가 필요
            InsertSave();
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


void KPropertiesDlg::OnStnClickedPicZoom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void KPropertiesDlg::OnBnClickedCheck1()
{
	UpdateData();	
	LoadUpdateInitialControl();
}


bool KPropertiesDlg::UpdateDataCheck()
{
    try
    {
        CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( CategoryName );
        CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
        int nChildCount = pPropertyGridItems->GetCount();

        for (int i = 0; i < nChildCount; i++)
        {
            CString strValue(_T(""));
            CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
            if( pPropertyGridItem->GetReadOnly() == TRUE)
                continue;

            KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();
            if (pColumn->PrimaryKey())
            {
                strValue = pPropertyGridItem->GetValue();
                strValue = strValue.Trim();
                if (strValue.GetLength() <= 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 필수 컬럼 항목에 값을 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);

                    if( pColumn->CodeType() != KEMIOCodeTypeIsSingleCode && pColumn->CodeType() != KEMIOCodeTypeIsMultiCode)
                        pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }

            if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
            {
                int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
                if (nConst >= 0)
                {
                    CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                    strValue.Format(_T("%d"), pGridConst->m_dwData);
                }   
                else
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 값을 선택해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->Select();
                    return false;
                }
            }
            else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
            {
                // 멀티 코드는 에디팅 체크를 하지 않음
                // strValue = SumFlagConvertMultiCode(pPropertyGridItem->GetValue());
            }
            else
            {
                strValue = pPropertyGridItem->GetValue();
                if (pColumn->DataType() == KEMIODataTypeInteger)
                {
                    if (!QBicStringChecker::IsSignedInteger(strValue) && strValue.GetLength() > 0)
                    {
                        CString strMsg;
                        strMsg.Format(_T("%s - 정수형 숫자를 입력해 주세요."), pColumn->Caption());
                        AfxMessageBox(strMsg);
                        pPropertyGridItem->SetValue(_T(""));
                        pPropertyGridItem->Select();
                        return false;
                    }
                }

                if(pColumn->DataType() == KEMIODataTypeDouble)
                {
                    if (!QBicStringChecker::IsNumeric(strValue) && strValue.GetLength() > 0)
                    {
                        CString strMsg;
                        strMsg.Format(_T("%s - 실수형 숫자를 입력해 주세요."), pColumn->Caption());
                        AfxMessageBox(strMsg);
                        pPropertyGridItem->SetValue(_T(""));
                        pPropertyGridItem->Select();
                        return false;
                    }
                }
            }
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

    return true;
}


void KPropertiesDlg::OnBnClickedButtonCopy()
{
    try
    {
    	if (UpdateDataCheck() == false)
            return;

        if (m_strTableName.CompareNoCase(TABLE_NODE) == 0) {
            NodeCopy();
        } else if (m_strTableName.CompareNoCase(TABLE_LINK) == 0) {
            LinkCopy();
        } else {
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
}

void KPropertiesDlg::NodeCopy()
{
    try
    {
        KMapView* pMapView = ImChampFrameWindow::GetMapView(m_pTarget);
        std::map<CString, CString>& mapNodeCopyKeyValue = pMapView->GetNodeCopyKeyValue();

        mapNodeCopyKeyValue.clear();

        CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( CategoryName );
        CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
        int nChildCount = pPropertyGridItems->GetCount();

        for (int i = 0; i < nChildCount; i++)
        {
            CString strValue(_T(""));
            CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );

            KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

            if (pColumn->ColumnType() != KEMIOColumnTypeSystem)
                continue;

            if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
            {
                int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
                if (nConst >= 0)
                {
                    CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                    strValue.Format(_T("%d"), pGridConst->m_dwData);
                }                
            }
            else
            {
                strValue = pPropertyGridItem->GetValue();
            }

            CString strColumnName(pColumn->Name());
            // 아래 정보는 복사하지 않는다.
            if (strColumnName.CompareNoCase(COLUMN_NODE_ID) == 0)
            {
            }
            else if (strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0)
            {
            }
            else if (strColumnName.CompareNoCase(COLUMN_NODE_X) == 0)
            {
            }
            else if (strColumnName.CompareNoCase(COLUMN_NODE_Y) == 0)
            {
            }
            else
            {
                mapNodeCopyKeyValue.insert(std::make_pair(strColumnName, strValue));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
		throw 1;
    }
    catch (...)
    {
        TxLogDebugException();
		throw 1;
    }
}

void KPropertiesDlg::LinkCopy()
{
    try
    {
        KMapView* pMapView = ImChampFrameWindow::GetMapView(m_pTarget);
        std::map<CString, CString>& mapLinkCopyKeyValue = pMapView->GetLinkCopyKeyValue();

        mapLinkCopyKeyValue.clear();

        CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( CategoryName );
        CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
        int nChildCount = pPropertyGridItems->GetCount();

        for (int i = 0; i < nChildCount; i++)
        {
            CString strValue(_T(""));
            CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );

            KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

            if (pColumn->ColumnType() != KEMIOColumnTypeSystem)
                continue;

            if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
            {
                int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
                if (nConst >= 0)
                {
                    CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                    strValue.Format(_T("%d"), pGridConst->m_dwData);
                }                
            }
			else
            {
                strValue = pPropertyGridItem->GetValue();
            }

            CString strColumnName(pColumn->Name());
            // 아래 정보는 복사하지 않는다.
            if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0)
            {
            }
            else if (strColumnName.CompareNoCase(COLUMN_FNODE_ID) == 0)
            {
            }
            else if (strColumnName.CompareNoCase(COLUMN_TNODE_ID) == 0)
            {
            }
            else if (strColumnName.CompareNoCase(COLUMN_LINK_TYPE) == 0)
            {
            }
            else if (strColumnName.CompareNoCase(COLUMN_LINK_LENGTH) == 0)
            {
            }
            else
            {
                mapLinkCopyKeyValue.insert(std::make_pair(strColumnName, strValue));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
		throw 1;
    }
    catch (...)
    {
        TxLogDebugException();
		throw 1;
    }
}

void KPropertiesDlg::OnBnClickedButtonPaste()
{
    try
    {
        KMapView* pMapView = ImChampFrameWindow::GetMapView(m_pTarget);    
        if (m_strTableName.CompareNoCase(TABLE_NODE) == 0) {
            std::map<CString, CString>& mapNodeCopyKeyValue = pMapView->GetNodeCopyKeyValue();
            if (mapNodeCopyKeyValue.size() == 0)
            {
                AfxMessageBox(_T("복사된 정보가 없습니다.\n복사 명령을 먼저 수행해 주세요."));
                return;
            }
            NodePaste();
        } else if (m_strTableName.CompareNoCase(TABLE_LINK) == 0) {
            std::map<CString, CString>& mapLinkCopyKeyValue = pMapView->GetLinkCopyKeyValue();
            if (mapLinkCopyKeyValue.size() == 0)
            {
                AfxMessageBox(_T("복사된 정보가 없습니다.\n복사 명령을 먼저 수행해 주세요."));
                return;
            }
            LinkPaste();
        } else {
			return;
		}

		AfxMessageBox(_T("복사/붙여넣기가 완료되었습니다."));
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

void KPropertiesDlg::NodePaste()
{
    try
    {
        KMapView* pMapView = ImChampFrameWindow::GetMapView(m_pTarget);
        std::map<CString, CString>& mapNodeCopyKeyValue = pMapView->GetNodeCopyKeyValue();
        std::map<CString, CString>::iterator iter = mapNodeCopyKeyValue.begin();
        std::map<CString, CString>::iterator end  = mapNodeCopyKeyValue.end();

        CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( CategoryName );
        CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
        int nChildCount = pPropertyGridItems->GetCount();

        for (int i = 0; i < nChildCount; i++)
        {            
            CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );

            KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();
            CString strColumnName(pColumn->Name());

            iter = mapNodeCopyKeyValue.find(strColumnName);
            if (iter != end)
            {
                CString strValue(iter->second);
                if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
                {
                    int nCode  = _ttoi(strValue);
                    CXTPPropertyGridItemEnum* pGridItemEnum = (CXTPPropertyGridItemEnum*)pPropertyGridItem;
                    int nConst = pGridItemEnum->GetConstraints()->FindConstraint(nCode);

                    if (nConst >= 0)
                        pGridItemEnum->SetEnum(nCode);
                }
                else
                {
                    pPropertyGridItem->SetValue(strValue);
                }      
            }                  
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
		throw 1;
    }
    catch (...)
    {
        TxLogDebugException();
		throw 1;
    }
}

void KPropertiesDlg::LinkPaste()
{
    try
    {
        KMapView* pMapView = ImChampFrameWindow::GetMapView(m_pTarget);
        std::map<CString, CString>& mapLinkCopyKeyValue = pMapView->GetLinkCopyKeyValue();
        std::map<CString, CString>::iterator iter = mapLinkCopyKeyValue.begin();
        std::map<CString, CString>::iterator end  = mapLinkCopyKeyValue.end();

        CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( CategoryName );
        CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
        int nChildCount = pPropertyGridItems->GetCount();

        for (int i = 0; i < nChildCount; i++)
        {            
            CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );

            KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();
            CString strColumnName(pColumn->Name());

            iter = mapLinkCopyKeyValue.find(strColumnName);
            if (iter != end)
            {
                CString strValue(iter->second);
                if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
                {
                    int nCode  = _ttoi(strValue);
                    CXTPPropertyGridItemEnum* pGridItemEnum = (CXTPPropertyGridItemEnum*)pPropertyGridItem;
                    int nConst = pGridItemEnum->GetConstraints()->FindConstraint(nCode);

                    if (nConst >= 0)
                        pGridItemEnum->SetEnum(nCode);
                }
                else
                {
                    pPropertyGridItem->SetValue(strValue);
                }      
            }                  
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
		throw 1;
    }
    catch (...)
    {
        TxLogDebugException();
		throw 1;
    }
}

LRESULT KPropertiesDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
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

void KPropertiesDlg::ShowAddRowData( KTarget* a_pTarget, CString a_strTableName )
{
    try
    {
        m_nEditFlag    = 1;

        m_strTableName = a_strTableName;
        m_pTarget      = a_pTarget;

        if (m_pTable != nullptr)
            m_pTable->Unregister(this);

        m_pTable       = m_pTarget->Tables()->FindTable(a_strTableName);	

        if (m_pTable != nullptr)
            m_pTable->Register(this);

        m_nxLinkID     = -1;
    }
    catch(...)
    {
        TxLogDebugException();
        throw;
    }

    try
    {
        LoadInsertInitialControl();              
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

void KPropertiesDlg::LoadInsertInitialControl()
{
    m_oInsertRow.Clear();
    InitialButtonStatus();
    m_wndPropertyGrid.ResetContent();

    try
    {
        CString strTableName = m_pTable->Name();

        std::vector<Integer> vecPrimaryKey = KBulkDBaseUtil::GetIntegerNewPrimaryKey(m_pTarget, strTableName);

        const KIOColumns* pIOColumns = m_pTable->Columns();
        KCodeManager*     pCodeMgr   = m_pTarget->CodeManager();

        int nColumnCount = pIOColumns->ColumnCount();
        int nPKIndex(0);

        CXTPPropertyGridItem* pPropertyGridItemParent = m_wndPropertyGrid.AddCategory( CategoryName );
        for(int i = 0; i < nColumnCount; i++)
        {
            KIOColumn* pColumn = pIOColumns->GetColumn( i );     
            KIOItem*   pItem   = m_oInsertRow.AddNewItem();

            if (pColumn->PrimaryKey() && (int)vecPrimaryKey.size() > nPKIndex)
            {
                pItem->SetValue(vecPrimaryKey[nPKIndex]);
                nPKIndex++;
            }

            CXTPPropertyGridItem* pGridItem = NULL;

            if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
            {				
                pGridItem = AddCodeItem(pPropertyGridItemParent, pColumn, pItem);
            }
            else if(pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
            {
                pGridItem = AddMultiCodeItem(pPropertyGridItemParent, pColumn, pItem);
            }
            else
            {
                pGridItem =	pPropertyGridItemParent->AddChildItem(new CXTPPropertyGridItem(pColumn->Caption()));                
                pGridItem->SetValue( pItem->GetValueAsString() );
            }

            pGridItem->SetItemData((DWORD_PTR)pColumn);
        }

        pPropertyGridItemParent->Expand();
    }
    catch (int& e)
    {
        switch (e)
        {
        case 1:
            //AfxMessageBox(_T("Record 값이 존재하지 않습니다."));
            break;
        default:
            //AfxMessageBox(_T("Record 조회 중 오류가 발생하였습니다."));
            break;
        }

    }
    catch (...)
    {
        AfxMessageBox(_T("Record 조회 중 오류가 발생하였습니다."));
    }
}

void KPropertiesDlg::InitialButtonStatus()
{
    CString strCaption;
    strCaption.Format(_T("%s"), m_pTable->Caption());

	SetDlgItemText(IDC_STATIC_NAME, strCaption);
	UpdateImChampTooltip(IDC_STATIC_NAME, strCaption);

    CButton* pButtonCopy  = (CButton*)GetDlgItem(IDC_BUTTON_COPY);
    CButton* pButtonPaste = (CButton*)GetDlgItem(IDC_BUTTON_PASTE);
    CButton* pCheckButton = (CButton*)GetDlgItem(IDC_CHECK2);
    CButton* pButtonOK    = (CButton*)GetDlgItem(IDOK);

    if (m_bEditCheck == TRUE)
    {
        pButtonOK->ShowWindow(SW_SHOW);
        pCheckButton->ShowWindow(SW_SHOW);

        if (_tcsicmp(m_strTableName, TABLE_NODE) == 0)
        {
            pButtonCopy->ShowWindow(SW_SHOW);
            pButtonPaste->ShowWindow(SW_SHOW);
            pCheckButton->ShowWindow(SW_HIDE);
        }
        else if (_tcsicmp(m_strTableName, TABLE_LINK) == 0)
        {
            pButtonCopy->ShowWindow(SW_SHOW);
            pButtonPaste->ShowWindow(SW_SHOW);
            pCheckButton->ShowWindow(SW_SHOW);
        }
        else
        {
            pButtonCopy->ShowWindow(SW_HIDE);
            pButtonPaste->ShowWindow(SW_HIDE);
            pCheckButton->ShowWindow(SW_HIDE);
        }
    }
    else
    {
        pButtonCopy->ShowWindow(SW_SHOW);
        pButtonPaste->ShowWindow(SW_HIDE);

        pButtonOK->ShowWindow(SW_HIDE);
        pCheckButton->ShowWindow(SW_HIDE);
    }  
}

void KPropertiesDlg::UpdateSave()
{
    KIOColumns* pIOColumns = m_pTable->Columns();

    typedef std::shared_ptr<KIORecordset> KIORecordsetPtr;
    KIORecordsetPtr spIORecordSet (m_pTable->GetRecordset(KEMIORecordsetTypeStatic, pIOColumns, m_strWhere, _T("")));
    int nRowCount = spIORecordSet->GetRowCount();
    if (nRowCount == 0)
    {
        AfxMessageBox(_T("데이터가 존재하지 않습니다."));
        OnCancel();
    }

    const KIORow* pIORow = spIORecordSet->GetRow(0);

    std::vector<const KIORow*> vecOppositeLink;    
    KIORecordsetPtr spOppositeRecordSet;
    CButton* pCheckButton = (CButton*)GetDlgItem(IDC_CHECK2);
    if (_tcsicmp(m_strTableName, TABLE_LINK) == 0)
    {
        if (pCheckButton->GetCheck() == BST_CHECKED)
        {
            Integer nxFromNodeID(-1), nxToNodeID(-1);
            if (KBulkDBaseLink::GetFTNodeID(m_pTarget, m_nxLinkID, nxFromNodeID, nxToNodeID))
            {
                CString strOppositeWhere;
                strOppositeWhere.Format(_T(" %s = '%I64d' and %s = '%I64d' "), COLUMN_FNODE_ID, nxToNodeID, COLUMN_TNODE_ID, nxFromNodeID);
                spOppositeRecordSet = KIORecordsetPtr(m_pTable->GetRecordset(KEMIORecordsetTypeStatic, pIOColumns, strOppositeWhere, _T("")));
                int nCount = spOppositeRecordSet->GetRowCount();

                for (int i=0; i<nCount; i++)
                {
                    vecOppositeLink.push_back(spOppositeRecordSet->GetRow(i));
                }
            }            
        }
    }

    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( CategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
        if( pPropertyGridItem->GetReadOnly() == TRUE)
            continue;

        KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

        if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
        {
            int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
            if (nConst >= 0)
            {
                CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                strValue.Format(_T("%d"), pGridConst->m_dwData);
            }
        }
        else
        {
            strValue = pPropertyGridItem->GetValue();
        }

        KIOItem* pItem = pIORow->GetItem( pIOColumns->GetColumnIndex(pColumn) );
        pItem->SetValue( strValue );

        for (size_t m=0; m<vecOppositeLink.size(); m++)
        {
            if (pColumn->ColumnType() != KEMIOColumnTypeSystem)
                continue;

            int nIndex = pIOColumns->GetColumnIndex(pColumn);
            if (nIndex < 0)
                continue;

            KIOItem* pItem = vecOppositeLink[m]->GetItem(nIndex);
            if (pItem == nullptr)
                continue;

            pItem->SetValue( strValue );
        }
    }

    try
    {
        //★ 노드와 링크 테이블의 경우 라벨만 업그레이드 하면 된다.
        //★ 노드, 링크 타입은 이 다이얼로그에서 수정이 않되기 때문임
        std::map<__int64, CString> mapNodeLbl;
        std::map<__int64, CString> mapLinkLbl;

        bool bNodeFlag(false);
        bool bLinkFlag(false);

        int nLblIdx(-1);
        KIOColumn* pLblIOColumn = nullptr;
        KMapView* pMapView = ImChampFrameWindow::GetCreatedMapView(m_pTarget);
        if (pMapView != nullptr) {
            if (_tcsicmp(m_strTableName, TABLE_NODE) == 0) {
                bNodeFlag = true;

                KNodeLayerDispInfo oInfo = pMapView->NodeLayerDispInfo();

                CString strLblField = oInfo.TLabelField;
                if (strLblField.GetLength() > 0 && oInfo.TLabelShow == 1) {
                    nLblIdx = pIOColumns->GetColumnIndex(strLblField);
                }
            }

            if (_tcsicmp(m_strTableName, TABLE_LINK) == 0) {
                bLinkFlag = true;

                KLinkLayerDispInfo oInfo = pMapView->LinkLayerDispInfo();

                CString strLblField = oInfo.TLabelField;
                if (strLblField.GetLength() > 0 && oInfo.TLabelShow == 1) {
                    nLblIdx = pIOColumns->GetColumnIndex(strLblField);
                }
            }
        }
        
        m_pTable->BeginTransaction();
        m_pTable->UpdateRow(pIOColumns, pIORow, m_strWhere);

        if (bNodeFlag == true && nLblIdx >= 0) {
            int nKeyIdx = pIOColumns->GetColumnIndex(COLUMN_NODE_ID);
            if (nKeyIdx >= 0) {
                __int64 nxID   = pIORow->GetItem(nKeyIdx)->GetValueAsInteger64();
                CString strLbl = pIORow->GetItem(nLblIdx)->GetValueAsString();
                //★ 
                pLblIOColumn = pIOColumns->GetColumn(nLblIdx);

                mapNodeLbl.insert(std::make_pair(nxID, strLbl));
            }
        }

        if (bLinkFlag == true && nLblIdx >= 0) {
            int nKeyIdx = pIOColumns->GetColumnIndex(COLUMN_LINK_ID);
            if (nKeyIdx >= 0) {
                __int64 nxID   = pIORow->GetItem(nKeyIdx)->GetValueAsInteger64();
                CString strLbl = pIORow->GetItem(nLblIdx)->GetValueAsString();
                //★ 
                pLblIOColumn = pIOColumns->GetColumn(nLblIdx);

                mapLinkLbl.insert(std::make_pair(nxID, strLbl));
            }
        }

        // 링크의 경우 대항 링크 정보의 값을 업데이트 한다.
        for (size_t m=0; m<vecOppositeLink.size(); m++)
        {
            int nIndex = pIOColumns->GetColumnIndex(COLUMN_LINK_ID);
            if (nIndex < 0)
                continue;

            KIOItem* pItem = vecOppositeLink[m]->GetItem(nIndex);
            if (pItem == nullptr)
                continue;

            CString strWhere;
            strWhere.Format(_T(" %s = '%I64d' "), COLUMN_LINK_ID, pItem->GetValueAsInteger64());

            m_pTable->UpdateRow(pIOColumns, vecOppositeLink[m], strWhere);
            //★ 
            if (bLinkFlag == true && nLblIdx >= 0) {
                int nKeyIdx = pIOColumns->GetColumnIndex(COLUMN_LINK_ID);
                if (nKeyIdx >= 0) {

                    auto pIORow = vecOppositeLink[m];
                    __int64 nxID   = pIORow->GetItem(nKeyIdx)->GetValueAsInteger64();
                    CString strLbl = pIORow->GetItem(nLblIdx)->GetValueAsString();
                    //★ 
                    pLblIOColumn = pIOColumns->GetColumn(nLblIdx);

                    mapLinkLbl.insert(std::make_pair(nxID, strLbl));
                }
            }
        }

        m_pTable->Commit();  

        if (bNodeFlag == true) {
            for (auto iter = mapNodeLbl.begin(); iter != mapNodeLbl.end(); ++iter) {
                pMapView->UpdateNodeLinkMapDisp(KLayerID::Node(), iter->first, -1, pLblIOColumn->Name(), iter->second);
            }
            pMapView->MapRefresh();            
        }
        
        if (bLinkFlag == true) {
            for (auto iter = mapLinkLbl.begin(); iter != mapLinkLbl.end(); ++iter) {
                pMapView->UpdateNodeLinkMapDisp(KLayerID::Link(), iter->first, -1, pLblIOColumn->Name(), iter->second);
            }
            pMapView->MapRefresh();
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        m_pTable->Rollback();
		return;
    }
    catch (...)
    {
        TxLogDebugException();
        m_pTable->Rollback();
		return;
    }
}

void KPropertiesDlg::InsertSave()
{
    KIOColumns* pIOColumns = m_pTable->Columns();

    try
    {
        CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( CategoryName );
        CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
        int nChildCount = pPropertyGridItems->GetCount();

        for (int i = 0; i < nChildCount; i++)
        {
            CString strValue(_T(""));
            CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
            if( pPropertyGridItem->GetReadOnly() == TRUE)
                continue;

            KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

            if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
            {
                int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
                if (nConst >= 0)
                {
                    CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                    strValue.Format(_T("%d"), pGridConst->m_dwData);
                }
            }
            else
            {
                strValue = pPropertyGridItem->GetValue();
            }

            KIOItem* pItem = m_oInsertRow.GetItem( pIOColumns->GetColumnIndex(pColumn) );
            pItem->SetValue( strValue );        
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        AfxMessageBox(_T("시스템 오류가 발생하였습니다."));
        return;
    }
    catch (...)
    {
    	TxLogDebugException();
        AfxMessageBox(_T("시스템 오류가 발생하였습니다."));
        return;
    }    

    try
    {
        bool bResult = IsUsablePrimaryKey(m_pTarget, m_strTableName, &m_oInsertRow);
        if ( bResult == false ) return;
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        AfxMessageBox(_T("시스템 오류가 발생하였습니다."));
        return;
    }
    catch (...)
    {
    	TxLogDebugException();
        AfxMessageBox(_T("시스템 오류가 발생하였습니다."));
        return;
    }    

    try
    {
        m_pTable->BeginTransaction();
        m_pTable->InsertRow(pIOColumns, &m_oInsertRow);
        m_pTable->Commit();  
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        m_pTable->Rollback();
    }
    catch (...)
    {
        TxLogDebugException();
        m_pTable->Rollback();
    }
}

bool KPropertiesDlg::IsUsablePrimaryKey( KTarget* a_pTarget, CString a_strTableName, KIORow* a_pRow )
{
    if (a_pTarget == nullptr)
        ThrowException(_T("Target is null"));

    KIOTable*   pIOTable   = a_pTarget->Tables()->FindTable(a_strTableName);
    if (pIOTable == nullptr)
        ThrowException(_T("IOTable is null"));

    KIOColumns* pIOColumns = pIOTable->Columns();

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    bool bZoneIDCheck(false);
    if (_tcsicmp(a_strTableName, TABLE_SED_POP) == 0)
    {
        bZoneIDCheck = true;
    }

    if (_tcsicmp(a_strTableName, TABLE_SED_VECTOR) == 0)
    {
        bZoneIDCheck = true;
    }
    
    if (bZoneIDCheck == true)
    {
        int nColumnIndex = pIOColumns->GetColumnIndex(COLUMN_ZONE_ID);
        if (nColumnIndex >= 0)
        {
            bool bResult = KBulkDBaseDemarcation::ExistZone(spDBaseConnection, a_pRow->GetItem(nColumnIndex)->GetValueAsInteger64());
            if (bResult == false)
            {
                AfxMessageBox(_T("존 정보가 존재하지 않습니다."));
                return false;
            }
        }
    }
    
    bool bODIDCheck(false);
    if (pIOTable->TableType() == KEMIOTableTypeODData)
    {
        bODIDCheck = true;
    }

    if (bODIDCheck == true)
    {
        int nColumnIndex = pIOColumns->GetColumnIndex(COLUMN_OZONE);
        if (nColumnIndex >= 0)
        {
            bool bResult = KBulkDBaseDemarcation::ExistZone(spDBaseConnection, a_pRow->GetItem(nColumnIndex)->GetValueAsInteger64());
            if (bResult == false)
            {
                AfxMessageBox(_T("기점 존 정보가 존재하지 않습니다."));
                return false;
            }
        }

        nColumnIndex = pIOColumns->GetColumnIndex(COLUMN_DZONE);
        if (nColumnIndex >= 0)
        {
            bool bResult = KBulkDBaseDemarcation::ExistZone(spDBaseConnection, a_pRow->GetItem(nColumnIndex)->GetValueAsInteger64());
            if (bResult == false)
            {
                AfxMessageBox(_T("종점 존 정보가 존재하지 않습니다."));
                return false;
            }
        }
    }

    int nColumnCount = pIOColumns->ColumnCount();
    int nPKCount(0);
    CString strSQL;
    strSQL.AppendFormat(_T(" Select count(*) From %s Where "), a_strTableName);

    for (int i=0; i<nColumnCount; i++)
    {
        KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
        CString    strValue  = a_pRow->GetItem(i)->GetValueAsString();
        strValue = strValue.Trim();

        if (pIOColumn->PrimaryKey() == true && strValue.GetLength() > 0)
        {
            if (nPKCount == 0)
            {
                strSQL.AppendFormat(_T(" %s = '%s' "), pIOColumn->Name(), a_pRow->GetItem(i)->GetValueAsString());
            }
            else
            {
                strSQL.AppendFormat(_T(" And %s = '%s' "), pIOColumn->Name(), a_pRow->GetItem(i)->GetValueAsString());
            }

            nPKCount++;
        }
    }

    if (nPKCount == 0)
        return true;
    
    KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
    if (spResultSet->Next())
    {
        Integer nxCount = spResultSet->GetValueInt64(0);
        if (nxCount > 0)
        {
            AfxMessageBox(_T("중복된 ID값이 존재합니다."));
            return false;
        }
    }

    return true;
}

/*
// 멀티 코드 에디팅 방법 변경에 따른 코드 삭제

// int  MultiCodeConvertSumFlag( CString a_strMultiCodeVal );
// CString SumFlagConvertMultiCode( CString a_strSumFlag );
// CXTPPropertyGridItem* AddMultiCodeItemMultipleSample( CXTPPropertyGridItem* a_pParentGridItem, KIOColumn* a_pIOColumn, KIOItem* a_pIOItem );

CString KPropertiesDlg::SumFlagConvertMultiCode( CString a_strSumFlag )
{
	CString strValueMultiCode(_T(""));
	int		nSumFlag	= _ttol(a_strSumFlag);
	int		nLoopCount	= 0;
	bool	bFrist		= true;

	while (true)
	{
		CString strValue(_T(""));

		if( 0 != nSumFlag % 2)
		{
			if(true == bFrist)
				bFrist = false;
			else
				strValueMultiCode.Append(_T(","));

			strValue.Format(_T("%d"), nLoopCount);
			strValueMultiCode.Append(strValue);
		}
		nSumFlag = nSumFlag / 2;

		if ( 0 == nSumFlag )
			break;
		++nLoopCount;
	}

	return strValueMultiCode;
}

int KPropertiesDlg::MultiCodeConvertSumFlag( CString a_strMultiCodeVal )
{
	a_strMultiCodeVal.Remove(' ');

	if(a_strMultiCodeVal.IsEmpty())
	{
		return 0;
	}
	CString strElement(_T(""));
	int nSumAllFlag(0), nSub(0);

	while (AfxExtractSubString(strElement, a_strMultiCodeVal, nSub++, _T(',')))
	{
		if(strElement.IsEmpty() == TRUE)
		{
			continue;
		}
		nSumAllFlag += (int)pow(2.0, _ttoi(strElement));
	}
	return nSumAllFlag;
}

CXTPPropertyGridItem* KPropertiesDlg::AddMultiCodeItemMultipleSample( CXTPPropertyGridItem* a_pParentGridItem, KIOColumn* a_pIOColumn, KIOItem* a_pIOItem )
{
	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( a_pIOColumn->CodeGroup() );

	int nValue =  a_pIOItem->GetValueToInt();

	std::vector<int> vecCode;
	pCodeGroup->GetCodes( vecCode );
	size_t nxCodeCount = vecCode.size();

	CXTPPropertyGridItemOption* pGridItemOption = NULL;
	#pragma region 설명
	//	CXTPPropertyGridItemOption으로 만든 CheckBox는 아래와 같은 규칙을 가짐.
	//	추가한 체크박스의 Index값은 ( 0, 1, 2, 3 ...)이 아닌 2의 제곱근(pow(2.0, 0), pow(2.0, 1), pw(2.0, 2)...)을 Index로 함.
	//	체크박스의 선택을 위해서는 각 체크박스에 Selected 옵션을 주는 것이 아닌 생성자에서 아래의 값을 입력 함.
		new CXTPPropertyGridItemOption( Caption, int(선택하려는 각 Index들의 합) );
	#pragma endregion 설명
				
	pGridItemOption = (CXTPPropertyGridItemOption*)a_pParentGridItem->AddChildItem(new CXTPPropertyGridItemOption( a_pIOColumn->Caption(), MultiCodeConvertSumFlag(a_pIOItem->GetValueAsString()) ));
				
	pGridItemOption->SetItemData((DWORD_PTR)a_pIOColumn);

	std::vector<int>::iterator iter = vecCode.begin(), end = vecCode.end();
	while(iter != end)
	{
		CString strConst = pCodeGroup->SingleCodeValue(*iter, 1);
		pGridItemOption->GetConstraints()->AddConstraint(strConst, (int)pow(2.0, *iter));
		++iter;
	}
	pGridItemOption->SetCheckBoxStyle(TRUE);
	return pGridItemOption;
}
*/
