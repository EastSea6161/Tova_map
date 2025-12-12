// TocEditorNode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocEditorLink.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"

#include "ThemeStyleSave.h"
#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "ShapeEditController.h"
#include "XTPPropertyGridUtil.h"
#include "BulkDBaseNode.h"
#include "MultiCodeSettingDlg.h"

// KTocEditorLink 대화 상자입니다.
#define IDD_PROPERTY_GRID 1000

IMPLEMENT_DYNAMIC(KTocEditorLink, KDialogExBase)

KTocEditorLink::KTocEditorLink(CWnd* pParent /*=NULL*/)
	: KDialogExBase(KTocEditorLink::IDD, pParent)
{
	NodeCategoryName     = _T("Node");
	m_nPreNodeTemplateID = -1;
	NodeTemplateType     = 0;

	LinkCategoryName     = _T("Link");
	m_nPreLinkTemplateID = -1;
	LinkTemplateType     = 1;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		NodeCategoryName     = _T("노드");
		LinkCategoryName     = _T("링크");
	}

	m_pNodeTable = nullptr;
	m_pLinkTable = nullptr;

    m_nxNodeAddedID = -1;
}

KTocEditorLink::~KTocEditorLink()
{
	if (m_pNodeTable != nullptr)
		m_pNodeTable->Unregister(this);

	if (m_pLinkTable != nullptr)
		m_pLinkTable->Unregister(this);
}

void KTocEditorLink::DoDataExchange(CDataExchange* pDX)
{
	KDialogExBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_DUMMY, m_stcDummy);
	//DDX_Control(pDX, IDC_COMBO_WAY, m_cboAddLinkMethod);
	DDX_Control(pDX, IDC_COMBO_TEMPLATE_NODE, m_cboTemplateNode);
	DDX_Control(pDX, IDC_COMBO_TEMPLATE_LINK, m_cboTemplateLink);
}


BEGIN_MESSAGE_MAP(KTocEditorLink, KDialogExBase)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_LINK_ADD1,    &KTocEditorLink::OnBnClickedOneWayLinkAdd)	
    ON_BN_CLICKED(IDC_RADIO_LINK_ADD2,    &KTocEditorLink::OnBnClickedTwoWayLinkAdd)	

    ON_BN_CLICKED(IDC_RADIO_LINK_DELETE1, &KTocEditorLink::OnBnClickedLinkSingleDelete)
    ON_BN_CLICKED(IDC_RADIO_LINK_DELETE2, &KTocEditorLink::OnBnClickedLinkMultiDelete)
    
    ON_BN_CLICKED(IDC_RADIO_LINK_VERTEX1, &KTocEditorLink::OnBnClickedEditOneWayVertex)
    ON_BN_CLICKED(IDC_RADIO_LINK_VERTEX2, &KTocEditorLink::OnBnClickedEditTwoWayVertex)

    ON_BN_CLICKED(IDC_RADIO_LINK_MODIFY, &KTocEditorLink::OnBnClickedEditAtt)

    ON_BN_CLICKED(IDC_BUTTON_START, &KTocEditorLink::OnBnClickedButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_END,   &KTocEditorLink::OnBnClickedButtonEnd)
    
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_DELETE_NODE, &KTocEditorLink::OnBnClickedButtonTemplateDeleteNode)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_SAVE_NODE,   &KTocEditorLink::OnBnClickedButtonTemplateSaveNode)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATE_NODE,      &KTocEditorLink::OnCbnSelchangeComboTemplateNode)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_DELETE_LINK, &KTocEditorLink::OnBnClickedButtonTemplateDeleteLink)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_SAVE_LINK,   &KTocEditorLink::OnBnClickedButtonTemplateSaveLink)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATE_LINK,      &KTocEditorLink::OnCbnSelchangeComboTemplateLink)	

	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, &KTocEditorLink::OnPropertyGridItemValueChange)    
    
END_MESSAGE_MAP()


// KTocEditorLink 메시지 처리기입니다.
BOOL KTocEditorLink::OnInitDialog()
{
	KDialogExBase::OnInitDialog();
	KDialogExBase::UseKeyEscEnter(false, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    /*
    AddImChampTooltip(IDC_RADIO_ADD,    _T("링크 추가"));
    AddImChampTooltip(IDC_RADIO_DELETE, _T("링크 삭제"));
    AddImChampTooltip(IDC_RADIO_MODIFY, _T("링크 속성 편집"));
    AddImChampTooltip(IDC_BUTTON_START, _T("편집 시작"));
    AddImChampTooltip(IDC_BUTTON_END,   _T("편집 종료"));

    AddImChampTooltip(IDC_BUTTON_TEMPLATE_DELETE_NODE, _T("노드 입력 템플릿 삭제"));
    AddImChampTooltip(IDC_BUTTON_TEMPLATE_SAVE_NODE,   _T("노드 입력 템플릿 추가"));

    AddImChampTooltip(IDC_BUTTON_TEMPLATE_DELETE_LINK, _T("링크 입력 템플릿 삭제"));
    AddImChampTooltip(IDC_BUTTON_TEMPLATE_SAVE_LINK,   _T("링크 입력 템플릿 추가"));
    */
	SetBackgroundColor(RGB(255,255,255));
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
		
    CheckRadioButton(IDC_RADIO_LINK_ADD1, IDC_RADIO_LINK_MODIFY, IDC_RADIO_LINK_ADD2);

	/*
    m_cboAddLinkMethod.AddString(_T("One-way"));
	m_cboAddLinkMethod.AddString(_T("Two-way"));
	m_cboAddLinkMethod.SetCurSel(1);
    */

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH KTocEditorLink::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogExBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	return hbr;
}


void KTocEditorLink::OnSize(UINT nType, int cx, int cy)
{
	KDialogExBase::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KTocEditorLink::SetTargetMap( KTarget* a_pTarget, KMapView* a_pMapView )
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;
    
    try
    {
        if (m_pNodeTable != nullptr)
            m_pNodeTable->Unregister(this);

        if (m_pLinkTable != nullptr)
            m_pLinkTable->Unregister(this);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }

	m_pNodeTable       = m_pTarget->Tables()->FindTable(TABLE_NODE);	
	m_pNodeTable->Register(this);

	m_pLinkTable       = m_pTarget->Tables()->FindTable(TABLE_LINK);	
	m_pLinkTable->Register(this);

    CheckRadioButton(IDC_RADIO_LINK_ADD1, IDC_RADIO_LINK_MODIFY, IDC_RADIO_LINK_ADD2);

	LoadInitialBlankProperties();

	LoadTemplateComboBoxNode();
	LoadTemplateComboBoxLink();

	LoadNodePropertiesValue();
	LoadLinkPropertiesValue();

	InitialControl();
}

void KTocEditorLink::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        CString strSubjectName(a_strSubjectName);
        if(!(strSubjectName.CompareNoCase(TABLE_NODE) == 0 || strSubjectName.CompareNoCase(TABLE_LINK) == 0))
        {
            return;
        }

        std::map<CString, CString> mapNodeTemplateValue; {
            GetCurrentNodeTemplateValue(mapNodeTemplateValue);
        }        

        std::map<CString, CString> mapLinkTemplateValue; {
            GetCurrentLinkTemplateValue(mapLinkTemplateValue);
        }        

        LoadInitialBlankProperties();
        LoadNodePropertiesValue(mapNodeTemplateValue);
        LoadLinkPropertiesValue(mapLinkTemplateValue);
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

void KTocEditorLink::LoadInitialBlankProperties()
{
	m_wndPropertyGrid.ResetContent();

	try
	{
		const
		KIOColumns*   pIOColumns = m_pNodeTable->Columns();
		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();

		int nColumnCount = pIOColumns->ColumnCount();			

		CXTPPropertyGridItem* pPropertyGridItemParent = m_wndPropertyGrid.AddCategory( NodeCategoryName );
		for(int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pColumn = pIOColumns->GetColumn( i );     
			CXTPPropertyGridItem* pGridItem = NULL;

			if (pColumn->ColumnType() == KEMIOColumnTypeUserDefine) {
				continue;
			}

			if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode) {	
                CString strColumnName(pColumn->Name());
                if (strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0)
                {
                    pGridItem = KxtpPropertyGridUtil::AddNodeTypeItemAtLinkEdit(pPropertyGridItemParent, m_pTarget, pColumn);
                    CXTPPropertyGridItemEnum* pGridItemEnum = static_cast<CXTPPropertyGridItemEnum*>(pGridItem);
                }
                else
                {
                    pGridItem = KxtpPropertyGridUtil::AddCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
                }
			}
			else if(pColumn->CodeType() == KEMIOCodeTypeIsMultiCode) {
				pGridItem = KxtpPropertyGridUtil::AddMultiCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
			}
			else {
                CString strItemCaption(_T(""));
                if (_tcsicmp(pColumn->Name(), COLUMN_NODE_ID) == 0) {
                    strItemCaption.Format(_T("%s"), pColumn->Caption());
                }
                else {
                    strItemCaption = pColumn->Caption();
                }

				pGridItem =	pPropertyGridItemParent->AddChildItem(new CXTPPropertyGridItem(strItemCaption));
				
				if (pColumn->DataType() == KEMIODataTypeDouble) {
					pGridItem->SetValue(_T("0.0"));
				} else if (pColumn->DataType() == KEMIODataTypeInteger) {
					pGridItem->SetValue(_T("0"));
				} else if (pColumn->DataType() == KEMIODataTypeString) {
					pGridItem->SetValue(_T("-"));
				}
			}

			EditableCheck(pGridItem, pColumn);
            pGridItem->SetItemData((DWORD_PTR)pColumn);
		}

		pPropertyGridItemParent->Expand();
        pPropertyGridItemParent->SetItemData((DWORD_PTR)m_pNodeTable);
	}	
	catch (...)
	{
		//AfxMessageBox(_T("Propertis 초기화 중 오류가 발생하였습니다."));
	}	

	try
	{
		const
		KIOColumns*    pIOColumns = m_pLinkTable->Columns();
		KCodeManager*  pCodeMgr   = m_pTarget->CodeManager();

		int nColumnCount = pIOColumns->ColumnCount();			

		CXTPPropertyGridItem* pPropertyGridItemParent = m_wndPropertyGrid.AddCategory( LinkCategoryName );
		for(int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pColumn = pIOColumns->GetColumn( i );     
			CXTPPropertyGridItem* pGridItem = NULL;

			if (pColumn->ColumnType() == KEMIOColumnTypeUserDefine) {
				continue;
			}

			if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
			{				
				pGridItem = KxtpPropertyGridUtil::AddCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
				CString strColumnName(pColumn->Name());
				if (strColumnName.CompareNoCase(COLUMN_LINK_TYPE) == 0)
				{
					CXTPPropertyGridItemEnum* pGridItemEnum = static_cast<CXTPPropertyGridItemEnum*>(pGridItem);
                    CXTPPropertyGridItemConstraints* pConstraints = pGridItem->GetConstraints();
                    if (pConstraints->GetCount() <= 1) {
					    pGridItemEnum->SetEnum(0);  
                    }
                    else {
                        pGridItemEnum->SetEnum(pConstraints->GetConstraintAt(1)); 
                    }
				}
			}
			else if(pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
			{
				pGridItem = KxtpPropertyGridUtil::AddMultiCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
			}
			else
			{
                CString strItemCaption(_T(""));
                if (_tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0) {
                    strItemCaption.Format(_T("%s"), pColumn->Caption());
                }
                else {
                    strItemCaption = pColumn->Caption();
                }

				pGridItem =	pPropertyGridItemParent->AddChildItem(new CXTPPropertyGridItem(strItemCaption));
				
				if (pColumn->DataType() == KEMIODataTypeDouble) {
					pGridItem->SetValue(_T("0.0"));
				} else if (pColumn->DataType() == KEMIODataTypeInteger) {
					pGridItem->SetValue(_T("0"));
				} else if (pColumn->DataType() == KEMIODataTypeString) {
					pGridItem->SetValue(_T("-"));
				}
			}

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_LANE) == 0)
            {
                pGridItem->SetValue(_T("1"));
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_CAPA) == 0)
            {
                pGridItem->SetValue(_T("1500"));
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_SPEED) == 0)
            {
                pGridItem->SetValue(_T("60"));
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_ALPHA) == 0)
            {
                pGridItem->SetValue(_T("0.15"));
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_BETA) == 0)
            {
                pGridItem->SetValue(_T("4.0"));
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_A) == 0)
            {
                pGridItem->SetValue(_T("0.0"));
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_B) == 0)
            {
                pGridItem->SetValue(_T("0.0"));
            }

			EditableCheck(pGridItem, pColumn);
            pGridItem->SetItemData((DWORD_PTR)pColumn);
		}
	
		pPropertyGridItemParent->Expand();
        pPropertyGridItemParent->SetItemData((DWORD_PTR)m_pLinkTable);
	}	
	catch (...)
	{
		//AfxMessageBox(_T("Propertis 초기화 중 오류가 발생하였습니다."));
	}	
}

void KTocEditorLink::EditableCheck( CXTPPropertyGridItem* pPropertyGridItem, KIOColumn* a_pIOColumn )
{
	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_LINK_ADD1, IDC_RADIO_LINK_MODIFY);
	if (!(nCheckedRadioBtn == IDC_RADIO_LINK_ADD1 || nCheckedRadioBtn == IDC_RADIO_LINK_ADD2)) {
		pPropertyGridItem->SetReadOnly();
		return;
	}

	CString strColumnName(a_pIOColumn->Name());

    if (strColumnName.CompareNoCase(COLUMN_NODE_ID) == 0)
    {
        CString strTemp(_T("-"));
        if (m_nxNodeAddedID < 0) {
            CXTPPropertyGridItemMetrics* pItem = pPropertyGridItem->GetValueMetrics();
            pItem->m_clrBack = CXTPPaintManagerColor(RGB(255, 255, 255)); 
        }
        else {
            strTemp.Format(_T("%I64d"), m_nxNodeAddedID);
            CXTPPropertyGridItemMetrics* pItem = pPropertyGridItem->GetValueMetrics();
            pItem->m_clrBack = CXTPPaintManagerColor(RGB(0, 0, 255));          
        }

        pPropertyGridItem->SetValue(strTemp);
        pPropertyGridItem->SetReadOnly();
        return;
    }

    if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0)
    {
        CString strTemp(_T("-"));
        size_t nxCnt = m_vecLinkAddedID.size();
        
        for (size_t i=0; i<nxCnt; i++) {
            if (i == 0) {
                strTemp.Format(_T("%I64d"), m_vecLinkAddedID[i]);
            }
            else {
                strTemp.AppendFormat(_T(", %I64d"), m_vecLinkAddedID[i]);
            }
        }

        pPropertyGridItem->SetValue(strTemp);
        pPropertyGridItem->SetReadOnly();
        return;
    }

    if (strColumnName.CompareNoCase(COLUMN_FNODE_ID) == 0)
    {
        pPropertyGridItem->SetReadOnly();
        pPropertyGridItem->SetValue(TSTAR);
        pPropertyGridItem->SetHidden(TRUE);
        return;
    }

    if (strColumnName.CompareNoCase(COLUMN_TNODE_ID) == 0)
    {
        pPropertyGridItem->SetReadOnly();
        pPropertyGridItem->SetValue(TSTAR);
        pPropertyGridItem->SetHidden(TRUE);
        return;
    }

	if (strColumnName.CompareNoCase(COLUMN_LINK_LENGTH) == 0)
	{
		pPropertyGridItem->SetReadOnly();
		pPropertyGridItem->SetValue(TSTAR);
        pPropertyGridItem->SetHidden(TRUE);
		return;
	}

	// 노드, 링크 분리 필요
	if (strColumnName.CompareNoCase(COLUMN_NODE_X) == 0)
	{
		pPropertyGridItem->SetReadOnly();
		pPropertyGridItem->SetValue(TSTAR);
        pPropertyGridItem->SetHidden(TRUE);
		return;
	}
	
    if (strColumnName.CompareNoCase(COLUMN_NODE_Y) == 0)
	{
		pPropertyGridItem->SetReadOnly();
		pPropertyGridItem->SetValue(TSTAR);
        pPropertyGridItem->SetHidden(TRUE);
		return;
	}

    if (strColumnName.CompareNoCase(COLUMN_ZONE_ID) == 0)
    {
        Integer nxMaxZoneID = KBulkDBaseNode::MaxZoneID(m_pTarget);
        CString strTemp(_T(""));
        if (nxMaxZoneID > 0)
        {
            strTemp.Format(_T("%I64d"), nxMaxZoneID);
            pPropertyGridItem->SetValue(strTemp);
        }        
        return;
    }
}

void KTocEditorLink::InitialControl()
{
    CWnd* pCtrlWindow = NULL;

    int  nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_LINK_ADD1, IDC_RADIO_LINK_MODIFY);
    switch(nCheckedRadioBtn)
    {
    case IDC_RADIO_LINK_ADD1:
    case IDC_RADIO_LINK_ADD2:
        pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_NODE);
        pCtrlWindow->EnableWindow(TRUE);

        pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_NODE);
        pCtrlWindow->EnableWindow(TRUE);

        pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_NODE);
        pCtrlWindow->EnableWindow(TRUE);	

        pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_LINK);
        pCtrlWindow->EnableWindow(TRUE);

        pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_LINK);
        pCtrlWindow->EnableWindow(TRUE);

        pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_LINK);
        pCtrlWindow->EnableWindow(TRUE);		
        break;
    default:        
        pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_NODE);
        pCtrlWindow->EnableWindow(FALSE);

        pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_NODE);
        pCtrlWindow->EnableWindow(FALSE);

        pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_NODE);
        pCtrlWindow->EnableWindow(FALSE);		

        pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_LINK);
        pCtrlWindow->EnableWindow(FALSE);

        pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_LINK);
        pCtrlWindow->EnableWindow(FALSE);

        pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_LINK);
        pCtrlWindow->EnableWindow(FALSE);		
        break;
    }

    if (nCheckedRadioBtn == IDC_RADIO_LINK_ADD1 || nCheckedRadioBtn == IDC_RADIO_LINK_ADD2) {
        if (m_nxNodeAddedID >= 0)
            m_wndPropertyGrid.ShowWindow(SW_SHOW);
        else
            m_wndPropertyGrid.ShowWindow(SW_HIDE);
    }
    else {
        m_wndPropertyGrid.ShowWindow(SW_HIDE);
    }    
}

void KTocEditorLink::OnBnClickedTwoWayLinkAdd()
{
    m_vecLinkAddedID.clear();
    m_nxNodeAddedID = -1;
    
	LoadInitialBlankProperties();
	LoadNodePropertiesValue();
	LoadLinkPropertiesValue();
	InitialControl();
    
	AddLinkStart();
}

void KTocEditorLink::OnBnClickedOneWayLinkAdd()
{
    OnBnClickedTwoWayLinkAdd();
}


void KTocEditorLink::AddLinkStart()
{
    int nChecked = GetCheckedRadioButton(IDC_RADIO_LINK_ADD1, IDC_RADIO_LINK_MODIFY);
    if (!(nChecked == IDC_RADIO_LINK_ADD1 || nChecked == IDC_RADIO_LINK_ADD2))
        return;

	if (NodeTemplateDataCheck() == false) return;
	if (LinkTemplateDataCheck() == false) return;

	std::map<CString, CString> mapNodeKeyValue; {
        GetCurrentNodeTemplateValue(mapNodeKeyValue);
    }	

	std::map<CString, CString> mapLinkKeyValue; {
        GetCurrentLinkTemplateValue(mapLinkKeyValue);
    }	

	KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
	if (spShapeEditController == NULL)
	{
		spShapeEditController = m_pMapView->ShapeEditStart();
	}

	if (spShapeEditController != NULL)
	{
		spShapeEditController->SetEditingTarget(KEMShapeEditingLayerLink);
        CString& strValue = mapNodeKeyValue[COLUMN_NODE_TYPE];
        int nType = _ttoi(strValue);
        if (nType == 0) {
            strValue.Format(_T("%d"), GetNodeDefaultType());
        }

		spShapeEditController->SetDefaultNodeInfo(mapNodeKeyValue);
		spShapeEditController->SetDefaultLinkInfo(mapLinkKeyValue);        
		
		if (nChecked == IDC_RADIO_LINK_ADD1) {
			spShapeEditController->SetEditingTask(KEMShapeEditingTaskLinkOneWayInsert);
		}
		else {
			spShapeEditController->SetEditingTask(KEMShapeEditingTaskLinkTwoWayInsert);
		}		
	}
}

LRESULT KTocEditorLink::OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam)
{
	if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
        CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam; {
            pItem = pItem->GetParentItem();
        }        

        KIOTable* pIOTable = (KIOTable*)pItem->GetItemData();

		KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
		if (true)
		{
			if (NodeTemplateDataCheck()) {
				std::map<CString, CString> mapKeyValue00; {
                    GetCurrentNodeTemplateValue(mapKeyValue00);
                }
				
                if (spShapeEditController != nullptr) {
                    CString& strValue = mapKeyValue00[COLUMN_NODE_TYPE];
                    int nType = _ttoi(strValue);
                    if (nType == 0) {
                        strValue.Format(_T("%d"), GetNodeDefaultType());
                    }

				    spShapeEditController->SetDefaultNodeInfo(mapKeyValue00);
                }

                if (pIOTable) {
                    if (_tcsicmp(pIOTable->Name(), TABLE_NODE) == 0) {
                        std::map<CString, CString> mapKeyValue; {
                            GetCurrentNodeValue(mapKeyValue); 
                        }
                        
                        UpdateNode(mapKeyValue);
                    } 
                }

				if (LinkTemplateDataCheck()) {
					std::map<CString, CString> mapKeyValue;
					GetCurrentLinkTemplateValue(mapKeyValue);
                    if (spShapeEditController != nullptr)
					    spShapeEditController->SetDefaultLinkInfo(mapKeyValue);    

                    if (pIOTable) {
                        if (_tcsicmp(pIOTable->Name(), TABLE_LINK) == 0) {
                            GetCurrentLinkValue(mapKeyValue);
                            UpdateLink(mapKeyValue);
                        }
                    }
				}
                else {
                    StopEditing();
                    return 0;
                }
			}
            else {
                return 0;
            }
		}
		else
		{
			if (!(NodeTemplateDataCheck() && LinkTemplateDataCheck())) {
                return 0;
            }
		}
	}

	return 0;
}

void KTocEditorLink::OnBnClickedEditAtt()
{
    m_vecLinkAddedID.clear();
    m_nxNodeAddedID = -1;

    //CheckRadioButton(IDC_RADIO_LINK_ADD1, IDC_RADIO_LINK_MODIFY, IDC_RADIO_LINK_MODIFY);
    
	LoadInitialBlankProperties();
	InitialControl();
	LinkEditPropertiesStart();
}

void KTocEditorLink::LinkEditPropertiesStart()
{
	KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
	if (spShapeEditController == NULL)
	{
		spShapeEditController = m_pMapView->ShapeEditStart();
	}

	if (spShapeEditController != NULL)
	{
		spShapeEditController->SetEditingTarget(KEMShapeEditingLayerLink);
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskSingleEdit);
    }
}

void KTocEditorLink::OnCbnSelchangeComboWay()
{
    
}

void KTocEditorLink::OnBnClickedLinkMultiDelete()
{   
    m_vecLinkAddedID.clear();
    m_nxNodeAddedID = -1;

    //CheckRadioButton(IDC_RADIO_ADD, IDC_RADIO_VERTEX, IDC_RADIO_DELETE);

    LoadInitialBlankProperties();
    InitialControl();
    DeleteLinkStart();
}

void KTocEditorLink::DeleteLinkStart()
{
    KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
    if (spShapeEditController == NULL) {
        spShapeEditController = m_pMapView->ShapeEditStart();
    }

    if (spShapeEditController != NULL) {
        spShapeEditController->SetEditingTarget(KEMShapeEditingLayerLink);
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskDelete);
    }
}

void KTocEditorLink::OnBnClickedLinkSingleDelete()
{
    m_vecLinkAddedID.clear();
    m_nxNodeAddedID = -1;

    //CheckRadioButton(IDC_RADIO_ADD, IDC_RADIO_VERTEX, IDC_RADIO_DELETE);

    LoadInitialBlankProperties();
    InitialControl();
    DeleteSingleLinkStart();
}

void KTocEditorLink::DeleteSingleLinkStart()
{
    KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
    if (spShapeEditController == NULL) {
        spShapeEditController = m_pMapView->ShapeEditStart();
    }

    if (spShapeEditController != NULL) {
        spShapeEditController->SetEditingTarget(KEMShapeEditingLayerLink);
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskSingleDel);
    }
}

void KTocEditorLink::OnBnClickedButtonStart()
{
    int nChecked = GetCheckedRadioButton(IDC_RADIO_LINK_ADD1, IDC_RADIO_LINK_MODIFY);

    switch(nChecked)
    {
    case IDC_RADIO_LINK_ADD1:
    case IDC_RADIO_LINK_ADD2:
        AddLinkStart();
        break;
    case IDC_RADIO_LINK_DELETE1:
        DeleteSingleLinkStart();
        break;    
    case IDC_RADIO_LINK_DELETE2:
        DeleteLinkStart();
        break;
    case IDC_RADIO_LINK_VERTEX1:
        LinkOneWayVertexEditStart();
        break;
    case IDC_RADIO_LINK_VERTEX2:
        LinkTwoWayVertexEditStart();
        break;
    case IDC_RADIO_LINK_MODIFY:
        LinkEditPropertiesStart();
        break;
    default:
        LinkOneWayVertexEditStart();
        break;
    }
}

void KTocEditorLink::OnBnClickedButtonEnd()
{
	StopEditing();
    InitialControl();
}

void KTocEditorLink::OnBnClickedButtonTemplateDeleteLink()
{
	int nCurSelectedIndex = m_cboTemplateLink.GetCurSel();
	if (nCurSelectedIndex <= 0)
		return;

	int nID = m_cboTemplateLink.GetItemData(nCurSelectedIndex);
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}
	try
	{
		KProjectDBase::DeleteNetworkTemplate(spDBaseConnection, nID, LinkTemplateType);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		AfxMessageBox(_T("탬플렛 삭제 중 오류가 발생하였습니다"));
	}

	LoadTemplateComboBoxLink();
	LoadLinkPropertiesValue();
}

void KTocEditorLink::OnBnClickedButtonTemplateDeleteNode()
{
	int nCurSelectedIndex = m_cboTemplateNode.GetCurSel();
	if (nCurSelectedIndex <= 0)
		return;

	int nID = m_cboTemplateNode.GetItemData(nCurSelectedIndex);
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}
	try
	{
		KProjectDBase::DeleteNetworkTemplate(spDBaseConnection, nID, NodeTemplateType);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		AfxMessageBox(_T("탬플렛 삭제 중 오류가 발생하였습니다"));
	}

	LoadTemplateComboBoxNode();
	LoadNodePropertiesValue();
}

void KTocEditorLink::OnBnClickedButtonTemplateSaveNode()
{
	if (! NodeTemplateDataCheck())
		return;

	int nCurSelectedIndex = m_cboTemplateNode.GetCurSel();
	KThemeStyleSave dlg;

	if (nCurSelectedIndex > 0)
	{
		CString strExistStyleName;
		m_cboTemplateNode.GetLBText(nCurSelectedIndex, strExistStyleName);
		dlg.SetExistStyleName(strExistStyleName);
	}

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	bool bOverwrite = dlg.IsOverWrite();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	CString strTemplateValue = ConvertTemplateSaveValueNode();

	if (!bOverwrite)
	{
		CString strTemplateName = dlg.GetNewStyleName();
		try
		{
			KProjectDBase::InsertNetworkTemplate(spDBaseConnection, NodeTemplateType, strTemplateName, strTemplateValue);
		}
		catch (KExceptionPtr ex)
		{
			AfxMessageBox(ex->GetErrorMessage());
		}
		catch(...)
		{
			AfxMessageBox(_T("탬플렛 입력 중 오류가 발생하였습니다."));
		}
	}
	else
	{
		int nSelected = m_cboTemplateNode.GetCurSel();
		if (nSelected < 0)
		{
			return;
		}

		int nID = m_cboTemplateNode.GetItemData(nSelected);
		try
		{
			KProjectDBase::UpdateNetworkTemplate(spDBaseConnection, nID, NodeTemplateType, strTemplateValue);
		}
		catch (KExceptionPtr ex)
		{
			AfxMessageBox(ex->GetErrorMessage());
		}
		catch(...)
		{
			AfxMessageBox(_T("탬플렛 입력 중 오류가 발생하였습니다."));
		}
	}

	LoadTemplateComboBoxNode();
}

void KTocEditorLink::LoadTemplateComboBoxNode()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	std::vector<TNetworkTemplate> vecTemplate;
	try
	{
		KProjectDBase::NetwrokTemplate(spDBaseConnection, NodeTemplateType, vecTemplate);
	}
	catch(...)
	{
		AfxMessageBox(_T("NetwrokTemplate Query Exception"));
		return;
	}

	m_cboTemplateNode.ResetContent();

    int nIndex = m_cboTemplateNode.AddString(_T("-------"));
    int nID(-1);
    m_cboTemplateNode.SetItemData(nIndex, nID);

	int nSelectIndex = -1;
	for (size_t i=0; i<vecTemplate.size(); i++)
	{
		TNetworkTemplate& oTemplate = vecTemplate[i];
		nIndex = m_cboTemplateNode.AddString(oTemplate.TName);
		nID    = oTemplate.TID;
		m_cboTemplateNode.SetItemData(nIndex, nID);

		if (m_nPreNodeTemplateID == nID)
		{
			nSelectIndex = nIndex;
		}
	}

	if (m_cboTemplateNode.GetCount() > 0)
	{
		if (nSelectIndex == -1)
		{
			m_cboTemplateNode.SetCurSel(0);
			m_nPreNodeTemplateID = m_cboTemplateNode.GetItemData(0);
		}
		else
		{
			m_cboTemplateNode.SetCurSel(nSelectIndex);
		}
	}
}

bool KTocEditorLink::NodeTemplateDataCheck()
{
	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
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
            if (_tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0)
            {
                CString strTemp = pPropertyGridItem->GetValue(); {
                    strTemp.Trim();
                }
                
                if (strTemp.GetLength() == 0) {
                    CXTPPropertyGridItemConstraints* pConstraint = pPropertyGridItem->GetConstraints();
                    if (pConstraint->GetCount() > 0) {
                        pPropertyGridItem->SetValue(_T("1"));
                    }
                }

                strTemp = pPropertyGridItem->GetValue(); 
                int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
                if (nConst < 0) {
                    CString strMsg;
                    strMsg.Format(_T("%s - 선택해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->Select();
                    return false;
                }

                CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                strValue.Format(_T("%d"), pGridConst->m_dwData);

                if (strValue.GetLength() == 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 선택해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->Select();
                    return false;
                }

                if (_tcsicmp(strValue, _T("0")) == 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 링크 추가 작업 진행 중 존 정보를 입력할 수 없습니다."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->Select();
                    return false;
                }
            }
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			strValue = pPropertyGridItem->GetValue();
		}
		else
		{
			strValue = pPropertyGridItem->GetValue();
            if (_tcsicmp(pColumn->Name(), COLUMN_ZONE_ID) == 0)
            {                
                // 일반노드
                if (!KBulkDBaseNode::ExistZone(m_pTarget, _ttoi64(strValue)))
                {
                    Integer nxMaxZoneID = KBulkDBaseNode::MaxZoneID(m_pTarget);
                    CString strMsg;

                    if (nxMaxZoneID < 0)
                    {
                        pPropertyGridItem->SetValue(_T("0"));
                        pPropertyGridItem->Select();
                    }
                    else
                    {
                        strMsg.Format(_T("%I64d"), nxMaxZoneID);
                        pPropertyGridItem->SetValue(strMsg);
                        pPropertyGridItem->Select();
                    }
                }
                continue;
            }
            else if (pColumn->DataType() == KEMIODataTypeInteger)
            {
                if (!QBicStringChecker::IsSignedInteger(strValue) && strValue.GetLength() != 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 정수형 숫자를 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }
            else if(pColumn->DataType() == KEMIODataTypeDouble)
            {
                if (!QBicStringChecker::IsNumeric(strValue)  && strValue.GetLength() != 0)
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

	return true;
}

CString KTocEditorLink::ConvertTemplateSaveValueNode()
{
	CString strOut;

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CString strValue(_T(""));
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
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
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			strValue = pPropertyGridItem->GetValue();
		}
		else
		{
			strValue = pPropertyGridItem->GetValue();
		}

		// [컬럼이름@Value]~[컬럼이름@Value] 
		CString strTemp;
		strTemp.Format(_T("[%s@%s]~"), pColumn->Name(), strValue);

		strOut.Append(strTemp);
	}

	return strOut;
}

void KTocEditorLink::LoadNodePropertiesValue()
{
	if (m_cboTemplateNode.GetCount() < 1)
		return;

	int nSelectedIndex = m_cboTemplateNode.GetCurSel();
	if (nSelectedIndex <= 0) {
		nSelectedIndex = 0;
		m_cboTemplateNode.SetCurSel(0);

        return;
	}

	int nID = m_cboTemplateNode.GetItemData(nSelectedIndex);

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}
	std::map<CString, CString> mapTemplateValueNode;
	KProjectDBase::NetwrokTemplate(spDBaseConnection, nID, NodeTemplateType, mapTemplateValueNode);

	if (mapTemplateValueNode.size() < 1)
	{
		return;
	}

	std::map<CString, CString>::iterator iter = mapTemplateValueNode.begin();
	std::map<CString, CString>::iterator end  = mapTemplateValueNode.end();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		iter = mapTemplateValueNode.find(pColumn->Name());
		if (iter == end)
			continue;

		CString strValue = iter->second;

		if (pColumn->PrimaryKey()) // COLUMN_NODE_ID 일때, MaxID를 보여 주기 위함
		{
			EditableCheck(pPropertyGridItem, pColumn);
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			CXTPPropertyGridItemEnum* pGridItem = static_cast<CXTPPropertyGridItemEnum*>(pPropertyGridItem);
			if(pGridItem == NULL)
			{
				continue;
			}

			pGridItem->SetEnum(_ttoi(strValue));
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			pPropertyGridItem->SetValue(strValue);
		}
		else
		{
			pPropertyGridItem->SetValue(strValue);
		}
	}
}

void KTocEditorLink::LoadNodePropertiesValue(std::map<CString, CString>& ar_mapTemplateValueNode)
{
	std::map<CString, CString>::iterator iter = ar_mapTemplateValueNode.begin();
	std::map<CString, CString>::iterator end  = ar_mapTemplateValueNode.end();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

    bool bEnable(true);
	for (int i = 0; i < nChildCount; i++)
	{
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		iter = ar_mapTemplateValueNode.find(pColumn->Name());
		if (iter == end)
			continue;

		CString strValue = iter->second;
		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode) {
			CXTPPropertyGridItemEnum* pGridItem = static_cast<CXTPPropertyGridItemEnum*>(pPropertyGridItem);
			if(pGridItem == NULL) {
				continue;
			}
            
            if (_tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0) {
                int nType = _ttoi(strValue);
                if (nType == 0) {
                    pPropertyGridItem->SetReadOnly(TRUE);
                    pGridItem->SetEnum(0); //★ 선택이 되지 않은 상태로 된다.
                    bEnable = false;
                }
                else {
                    pGridItem->SetEnum(_ttoi(strValue));
                    pPropertyGridItem->SetReadOnly(FALSE);
                }                
            }
            else {
                pGridItem->SetEnum(_ttoi(strValue));
                pPropertyGridItem->SetReadOnly(FALSE);
            }
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			pPropertyGridItem->SetValue(strValue);
		}
		else
		{
			pPropertyGridItem->SetValue(strValue);
		}

        if (_tcsicmp(pColumn->Name(), COLUMN_ZONE_ID) == 0) {
            if (bEnable) {
                pPropertyGridItem->SetReadOnly(FALSE);
            }
            else {
                pPropertyGridItem->SetReadOnly(TRUE);
            }
        }
	}
}

void KTocEditorLink::GetCurrentNodeTemplateValue(std::map<CString, CString>& ar_mapTemplateValue)
{
	ar_mapTemplateValue.clear();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CString strValue(_T(""));
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
		CString    strColumnName = pColumn->Name();

        if (pColumn->PrimaryKey())
            continue;

		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode) {
			int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
            if (nConst >= 0) {
                CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                strValue.Format(_T("%d"), pGridConst->m_dwData);
            }			
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode ) {
			strValue = pPropertyGridItem->GetValue();
		}
		else {
			strValue = pPropertyGridItem->GetValue();
		}

		ar_mapTemplateValue.insert(std::make_pair(strColumnName, strValue));
	}
}

void KTocEditorLink::GetCurrentNodeValue( std::map<CString, CString>& ar_mapTemplateValue )
{
    ar_mapTemplateValue.clear();

    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
        KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
        CString    strColumnName = pColumn->Name();

        if (pColumn->PrimaryKey())
            continue;

        if (pPropertyGridItem->IsHidden() == TRUE) 
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
        else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
        {
            strValue = pPropertyGridItem->GetValue();
        }
        else
        {
            strValue = pPropertyGridItem->GetValue();
        }

        ar_mapTemplateValue.insert(std::make_pair(strColumnName, strValue));
    }
}

void KTocEditorLink::GetCurrentLinkValue( std::map<CString, CString>& ar_mapTemplateValue )
{
    ar_mapTemplateValue.clear();

    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
        KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
        CString    strColumnName = pColumn->Name();

        if (pColumn->PrimaryKey())
            continue;

        if (pPropertyGridItem->IsHidden() == TRUE) 
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
        else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
        {
            strValue = pPropertyGridItem->GetValue();
        }
        else
        {
            strValue = pPropertyGridItem->GetValue();
        }

        ar_mapTemplateValue.insert(std::make_pair(strColumnName, strValue));
    }
}

void KTocEditorLink::GetCurrentLinkTemplateValue(std::map<CString, CString>& ar_mapTemplateValue)
{
	ar_mapTemplateValue.clear();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CString strValue(_T(""));
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
		CString    strColumnName = pColumn->Name();

        if (pColumn->PrimaryKey())
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
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			strValue = pPropertyGridItem->GetValue();
		}
		else
		{
			strValue = pPropertyGridItem->GetValue();
		}

		ar_mapTemplateValue.insert(std::make_pair(strColumnName, strValue));
	}
}

void KTocEditorLink::OnCbnSelchangeComboTemplateNode()
{
	StopEditing();
	LoadNodePropertiesValue();
}

void KTocEditorLink::OnBnClickedButtonTemplateSaveLink()
{
	if (! LinkTemplateDataCheck())
		return;

	int nCurSelectedIndex = m_cboTemplateLink.GetCurSel();
	KThemeStyleSave dlg;
	
	if (nCurSelectedIndex > 0)
	{
		CString strExistStyleName;
		m_cboTemplateLink.GetLBText(nCurSelectedIndex, strExistStyleName);
		dlg.SetExistStyleName(strExistStyleName);
	}

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	bool bOverwrite = dlg.IsOverWrite();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	CString strTemplateValue = ConvertTemplateSaveValueLink();

	if (!bOverwrite)
	{
		CString strTemplateName = dlg.GetNewStyleName();
		try
		{
			KProjectDBase::InsertNetworkTemplate(spDBaseConnection, LinkTemplateType, strTemplateName, strTemplateValue);
		}
		catch (KExceptionPtr ex)
		{
			AfxMessageBox(ex->GetErrorMessage());
		}
		catch(...)
		{
			AfxMessageBox(_T("탬플렛 입력 중 오류가 발생하였습니다."));
		}
	}
	else
	{
		int nSelected = m_cboTemplateLink.GetCurSel();
		if (nSelected < 0)
		{
			return;
		}

		int nID = m_cboTemplateLink.GetItemData(nSelected);
		try
		{
			KProjectDBase::UpdateNetworkTemplate(spDBaseConnection, nID, LinkTemplateType, strTemplateValue);
		}
		catch (KExceptionPtr ex)
		{
			AfxMessageBox(ex->GetErrorMessage());
		}
		catch(...)
		{
			AfxMessageBox(_T("탬플렛 입력 중 오류가 발생하였습니다."));
		}
	}
		
	LoadTemplateComboBoxLink();
}

void KTocEditorLink::LoadTemplateComboBoxLink()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	std::vector<TNetworkTemplate> vecTemplate;
	try
	{
		KProjectDBase::NetwrokTemplate(spDBaseConnection, LinkTemplateType, vecTemplate);
	}
	catch(...)
	{
		AfxMessageBox(_T("NetwrokTemplate Query Exception"));
		return;
	}

	m_cboTemplateLink.ResetContent();
    int nIndex = m_cboTemplateLink.AddString(_T("-------"));
    int nID(-1);
    m_cboTemplateLink.SetItemData(nIndex, nID);

	int nSelectIndex = -1;
	for (size_t i=0; i<vecTemplate.size(); i++)
	{
		TNetworkTemplate& oTemplate = vecTemplate[i];
		nIndex = m_cboTemplateLink.AddString(oTemplate.TName);
		nID    = oTemplate.TID;
		m_cboTemplateLink.SetItemData(nIndex, nID);

		if (m_nPreLinkTemplateID == nID)
		{
			nSelectIndex = nIndex;
		}
	}

	if (m_cboTemplateLink.GetCount() > 0)
	{
		if (nSelectIndex == -1)
		{
			m_cboTemplateLink.SetCurSel(0);
			m_nPreLinkTemplateID = m_cboTemplateLink.GetItemData(0);
		}
		else
		{
			m_cboTemplateLink.SetCurSel(nSelectIndex);
		}
	}
}

bool KTocEditorLink::LinkTemplateDataCheck()
{
	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
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
            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_TYPE) == 0)
            {
                int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());

                if (nConst < 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 선택해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->Select();
                    return false;
                }

                CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                strValue.Format(_T("%d"), pGridConst->m_dwData);

                if (strValue.GetLength() == 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 선택해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->Select();
                    return false;
                }
            }
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			strValue = pPropertyGridItem->GetValue();
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

            double dCheckValue = _ttof(strValue);
            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_LANE) == 0)
            {
                if (dCheckValue <= 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 0보다 큰 숫자를 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_CAPA) == 0)
            {
                if (dCheckValue <= 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 0보다 큰 숫자를 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_SPEED) == 0)
            {
                if (dCheckValue <= 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 0보다 큰 숫자를 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_ALPHA) == 0)
            {
                if (dCheckValue <= 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 0보다 큰 숫자를 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_BETA) == 0)
            {
                if (dCheckValue <= 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 0보다 큰 숫자를 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_A) == 0)
            {
                if (dCheckValue < 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 0보다 큰 숫자를 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }

            if (_tcsicmp(pColumn->Name(), COLUMN_LINK_B) == 0)
            {
                if (dCheckValue < 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("%s - 0보다 큰 숫자를 입력해 주세요."), pColumn->Caption());
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }
		}
	}

	return true;
}

CString KTocEditorLink::ConvertTemplateSaveValueLink()
{
	CString strOut;

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CString strValue(_T(""));
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
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
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			strValue = pPropertyGridItem->GetValue();
		}
		else
		{
			strValue = pPropertyGridItem->GetValue();
		}

		// [컬럼이름@Value]~[컬럼이름@Value] 
		CString strTemp;
		strTemp.Format(_T("[%s@%s]~"), pColumn->Name(), strValue);

		strOut.Append(strTemp);
	}

	return strOut;
}

void KTocEditorLink::LoadLinkPropertiesValue()
{
	if (m_cboTemplateLink.GetCount() < 1)
		return;

	int nSelectedIndex = m_cboTemplateLink.GetCurSel();
	if (nSelectedIndex <= 0)
	{
		nSelectedIndex = 0;
		m_cboTemplateLink.SetCurSel(0);

        return;
	}

	int nID = m_cboTemplateLink.GetItemData(nSelectedIndex);

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}
	std::map<CString, CString> mapTemplateValueLink;
	KProjectDBase::NetwrokTemplate(spDBaseConnection, nID, LinkTemplateType, mapTemplateValueLink);

	if (mapTemplateValueLink.size() < 1)
	{
		return;
	}

	std::map<CString, CString>::iterator iter = mapTemplateValueLink.begin();
	std::map<CString, CString>::iterator end  = mapTemplateValueLink.end();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		iter = mapTemplateValueLink.find(pColumn->Name());
		if (iter == end)
			continue;

		CString strValue = iter->second;

		if (pColumn->PrimaryKey()) // COLUMN_LINK_ID 일때, MaxID를 보여 주기 위함
		{
			EditableCheck(pPropertyGridItem, pColumn);
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			CXTPPropertyGridItemEnum* pGridItem = static_cast<CXTPPropertyGridItemEnum*>(pPropertyGridItem);
			if(pGridItem == NULL) {
				continue;
			}

			pGridItem->SetEnum(_ttoi(strValue));
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			pPropertyGridItem->SetValue(strValue);
		}
		else
		{
			pPropertyGridItem->SetValue(strValue);
		}
	}
}

void KTocEditorLink::LoadLinkPropertiesValue(std::map<CString, CString>& ar_mapTemplateValueLink)
{
	std::map<CString, CString>::iterator iter = ar_mapTemplateValueLink.begin();
	std::map<CString, CString>::iterator end  = ar_mapTemplateValueLink.end();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		iter = ar_mapTemplateValueLink.find(pColumn->Name());
		if (iter == end)
			continue;

		CString strValue = iter->second;

		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			CXTPPropertyGridItemEnum* pGridItem = static_cast<CXTPPropertyGridItemEnum*>(pPropertyGridItem);
			if(pGridItem == NULL)
			{
				continue;
			}

			pGridItem->SetEnum(_ttoi(strValue));
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			pPropertyGridItem->SetValue(strValue);
		}
		else
		{
			pPropertyGridItem->SetValue(strValue);
		}
	}
}

void KTocEditorLink::OnCbnSelchangeComboTemplateLink()
{
	StopEditing();
	LoadLinkPropertiesValue();
}

void KTocEditorLink::ActionFeedback( KDialogFeedback::KEMActionFeedback emActionCode )
{
    try
    {
        if (emActionCode == KDialogFeedback::KEMActionFeedbackDeActivate)
        {
            StopEditing();
        }
        else if(emActionCode == KDialogFeedback::KEMActionFeedbackActivate)
        {
            LoadTemplateComboBoxNode();	
            LoadTemplateComboBoxLink();

            SetNodeStartIDValue();
            SetLinkStartIDValue();
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}


void KTocEditorLink::StopEditing()
{
    m_vecLinkAddedID.clear();
    m_nxNodeAddedID = -1;

    try
    {
        m_pMapView->StopShapeEdit();
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}

void KTocEditorLink::GetNodeStartIDValue( Integer& ar_nxNodeID )
{
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
        KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
        CString    strColumnName = pColumn->Name();

        if (pColumn->PrimaryKey())
        {
            strValue    = pPropertyGridItem->GetValue();
            ar_nxNodeID = _ttoi64(strValue);
        }
    }
}

void KTocEditorLink::GetLinkStartIDValue( Integer& ar_nxLinkID )
{
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
        KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
        CString    strColumnName = pColumn->Name();

        if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0)
        {
            strValue    = pPropertyGridItem->GetValue();
            ar_nxLinkID = _ttoi64(strValue);
        }
    }
}

void KTocEditorLink::SetNodeStartIDValue()
{
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
        KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
        CString    strColumnName = pColumn->Name();

        if (pColumn->PrimaryKey())
        {
            EditableCheck(pPropertyGridItem, pColumn);
            break;
        }
    }
}

void KTocEditorLink::SetLinkStartIDValue()
{
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
        KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
        CString    strColumnName = pColumn->Name();

        if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0)
        {
            EditableCheck(pPropertyGridItem, pColumn);
            break;
        }
    }
}


void KTocEditorLink::OnBnClickedEditOneWayVertex()
{
    //CheckRadioButton(IDC_RADIO_ADD, IDC_RADIO_VERTEX, IDC_RADIO_VERTEX);
    LoadInitialBlankProperties();
    InitialControl();
    LinkOneWayVertexEditStart();
}

void KTocEditorLink::LinkOneWayVertexEditStart()
{
    KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
    if (spShapeEditController == NULL)
    {
        spShapeEditController = m_pMapView->ShapeEditStart();
    }

    if (spShapeEditController != NULL)
    {
        spShapeEditController->SetEditingTarget(KEMShapeEditingLayerLink);
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskLinkOneWayVertex);
    }
}

void KTocEditorLink::OnBnClickedEditTwoWayVertex()
{
    LoadInitialBlankProperties();
    InitialControl();
    LinkTwoWayVertexEditStart();
}

void KTocEditorLink::LinkTwoWayVertexEditStart()
{
    KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
    if (spShapeEditController == NULL)
    {
        spShapeEditController = m_pMapView->ShapeEditStart();
    }

    if (spShapeEditController != NULL)
    {
        spShapeEditController->SetEditingTarget(KEMShapeEditingLayerLink);
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskLinkTwoWayVertex);
    }
}

void KTocEditorLink::NotifyNodeEdit(int nType, __int64 nxID )
{
    //★ 지도에서 Shape이 추가 된경우
    m_nxNodeAddedID = nxID;
    std::map<CString, CString> mapKeyValue; {
        LoadNode(nxID, mapKeyValue);
    }    

    if (nType == 2) { //★ 기존 노드
        if (mapKeyValue.size() > 0) {
            LoadNodePropertiesValue(mapKeyValue);
        }

        InitialControl();
        return;
    }
    
    //★ 신규 노드
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
        KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

        if (_tcsicmp(pColumn->Name(), COLUMN_NODE_ID) == 0) {
            CString strValue(_T("")); {
                strValue.Format(_T("%I64d"), nxID);
            }
            pPropertyGridItem->SetValue(strValue);
            CXTPPropertyGridItemMetrics* pItem = pPropertyGridItem->GetValueMetrics();
            pItem->m_clrBack = CXTPPaintManagerColor(RGB(0, 0, 255));
        }

        if (_tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0) {
            CString strType = mapKeyValue[COLUMN_NODE_TYPE];
            CXTPPropertyGridItemEnum* pGridItem = static_cast<CXTPPropertyGridItemEnum*>(pPropertyGridItem);
            pGridItem->SetEnum(_ttoi(strType));
            pPropertyGridItem->SetReadOnly(FALSE);
        }

        if (_tcsicmp(pColumn->Name(), COLUMN_ZONE_ID) == 0) {
            pPropertyGridItem->SetReadOnly(FALSE);
        }
    }

    InitialControl();
}

void KTocEditorLink::NotifyLinkEdit( std::vector<__int64>& vecID )
{
    m_vecLinkAddedID = vecID;

    CString strTemp(_T("-"));
    size_t nxCnt = m_vecLinkAddedID.size();

    for (size_t i=0; i<nxCnt; i++) {
        if (i == 0) {
            strTemp.Format(_T("%I64d"), m_vecLinkAddedID[i]);
        }
        else {
            strTemp.AppendFormat(_T(", %I64d"), m_vecLinkAddedID[i]);
        }
    }

    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( LinkCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
        KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

        if (_tcsicmp(pColumn->Name(), COLUMN_LINK_ID) == 0) {
            pPropertyGridItem->SetValue(strTemp);
            CXTPPropertyGridItemMetrics* pItem = pPropertyGridItem->GetValueMetrics();
            pItem->m_clrBack = CXTPPaintManagerColor(RGB(0, 0, 255));
        }
    }
}

void KTocEditorLink::LoadNode( __int64 nxID, std::map<CString, CString>& mapKeyValue )
{
    if (m_pNodeTable == nullptr || m_pTarget == nullptr)
        return;
    
    mapKeyValue.clear();

    KIOColumns* pIOColumns = m_pNodeTable->Columns();
    KIOColumn*  pIOColumn  = nullptr;

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        for (int i=0; i<pIOColumns->ColumnCount(); i++) {
            pIOColumn = pIOColumns->GetColumn(i);
            if (i == 0) {
                strSQL.AppendFormat(_T(" %s "), pIOColumn->Name());
            }
            else {
                strSQL.AppendFormat(_T(" ,%s "), pIOColumn->Name());
            }
        }
        strSQL.AppendFormat(_T(" From %s "), TABLE_NODE);
        strSQL.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_NODE_ID, nxID);
    }

    KDBaseConPtr spDBase  = m_pTarget->GetDBaseConnection();
    KResultSetPtr       spResult = spDBase->ExecuteQuery(strSQL);

    if (spResult->Next()) {
        for (int i=0; i<pIOColumns->ColumnCount(); i++) {
            pIOColumn = pIOColumns->GetColumn(i);

            CString strKey   = pIOColumn->Name();
            CString strValue = spResult->GetValueStr(i);

            mapKeyValue.insert(std::make_pair(strKey, strValue));
        }
    }
}

void KTocEditorLink::UpdateNode( std::map<CString, CString>& mapKeyValue )
{
    if (m_nxNodeAddedID < 0 || mapKeyValue.size() <= 0 || m_pTarget == nullptr)
        return;

    int nNodeType(-1);
    for (auto iter = mapKeyValue.begin(); iter != mapKeyValue.end(); ++iter) {
        CString strKey   = iter->first;
        CString strValue = iter->second;

        if (strKey.CompareNoCase(COLUMN_NODE_TYPE) == 0) {
            nNodeType = _ttoi(strValue);
            break;
        }
    }

    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T(" Update %s Set "), TABLE_NODE);
    }

    bool bFirst(true);
    for (auto iter = mapKeyValue.begin(); iter != mapKeyValue.end(); ++iter) {
        CString strKey   = iter->first;
        CString strValue = iter->second;

        if (strKey.CompareNoCase(_T("X")) == 0 || strKey.CompareNoCase(_T("Y")) == 0 )
            continue;

        if (bFirst) {
            strSQL.AppendFormat(_T(" %s = '%s' "), strKey, strValue);
            bFirst = false;
        }
        else {
            strSQL.AppendFormat(_T(", %s = '%s' "), strKey, strValue);
        }        
    }

    if (bFirst) {
        return;
    }

    strSQL.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_NODE_ID, m_nxNodeAddedID);

    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    spDBase->ExecuteUpdate(strSQL);

    CString strLbl(_T(""));
    CString strNodeLblColumn(_T("")); {
        KNodeLayerDispInfo oInfo = m_pMapView->NodeLayerDispInfo(); 
        if (oInfo.TLabelShow == 1) {
            strNodeLblColumn = oInfo.TLabelField;
            auto iter = mapKeyValue.find(strNodeLblColumn);

            if ( iter != mapKeyValue.end()) {
                strLbl.Format(_T("%s"), iter->second);
            }
        }
    }

    m_pMapView->UpdateNodeLinkMapDisp(KLayerID::Node(), m_nxNodeAddedID, nNodeType, strNodeLblColumn, strLbl);
    m_pMapView->MapRefresh();

    KIOTables* pIOTables = m_pTarget->Tables();

    KIOTable*  pIOTable  = NULL;
    pIOTable  = pIOTables->FindTable(TABLE_NODE);
    if (pIOTable) 
        pIOTable->Notify();
}

void KTocEditorLink::UpdateLink( std::map<CString, CString>& mapKeyValue )
{    
    if (m_vecLinkAddedID.size() == 0 || mapKeyValue.size() <= 0 || m_pTarget == nullptr)
        return;

    int nLinkType(-1);
    for (auto iter = mapKeyValue.begin(); iter != mapKeyValue.end(); ++iter) {
        CString strKey   = iter->first;
        CString strValue = iter->second;

        if (strKey.CompareNoCase(COLUMN_LINK_TYPE) == 0) {
            nLinkType = _ttoi(strValue);
            break;
        }
    }

    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T(" Update %s Set "), TABLE_LINK);
    }

    bool bFirst(true);
    for (auto iter = mapKeyValue.begin(); iter != mapKeyValue.end(); ++iter) {
        CString strKey   = iter->first;
        CString strValue = iter->second;

        if (strKey.CompareNoCase(_T("X")) == 0 || strKey.CompareNoCase(_T("Y")) == 0 )
            continue;

        if (bFirst) {
            strSQL.AppendFormat(_T(" %s = '%s' "), strKey, strValue);
            bFirst = false;
        }
        else {
            strSQL.AppendFormat(_T(", %s = '%s' "), strKey, strValue);
        }        
    }

    if (bFirst) {
        return;
    }

    strSQL.AppendFormat(_T(" Where %s In ( "), COLUMN_LINK_ID);

    for (size_t i=0; i<m_vecLinkAddedID.size(); i++) {
        if (i == 0) {
            strSQL.AppendFormat(_T(" '%I64d'"), m_vecLinkAddedID[i]);
        }
        else {
            strSQL.AppendFormat(_T(" ,'%I64d' "), m_vecLinkAddedID[i]);
        }        
    }
    
    strSQL.Append(_T(")"));

    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    spDBase->ExecuteUpdate(strSQL);

    CString strLbl(_T(""));
    CString strLinkLblColumn(_T("")); {
        KLinkLayerDispInfo oInfo = m_pMapView->LinkLayerDispInfo(); 
        if (oInfo.TLabelShow == 1) {
            strLinkLblColumn = oInfo.TLabelField;
            auto iter = mapKeyValue.find(strLinkLblColumn);

            if ( iter != mapKeyValue.end()) {
                strLbl.Format(_T("%s"), iter->second);
            }
        }
    }

    for (size_t i=0; i<m_vecLinkAddedID.size(); i++) {
        m_pMapView->UpdateNodeLinkMapDisp(KLayerID::Link(), m_vecLinkAddedID[i], nLinkType, strLinkLblColumn, strLbl);
    }
    m_pMapView->MapRefresh();

    KIOTables* pIOTables = m_pTarget->Tables();

    KIOTable*  pIOTable  = NULL;
    pIOTable  = pIOTables->FindTable(TABLE_LINK);
    if (pIOTable) 
        pIOTable->Notify();
}

int KTocEditorLink::GetNodeDefaultType()
{
    //★ 존노드 때문에.... 쩝
    int nType(1);

    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++) {
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
        KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

        if (_tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0) {
            CXTPPropertyGridItemConstraints* pConstraint = pPropertyGridItem->GetConstraints();
            if (pConstraint->GetCount() > 0) {
                CString strValue = pConstraint->GetAt(0);
                nType = _ttoi(strValue);
            }
        }
    }

    return nType;
}

void KTocEditorLink::NodeTypeEnable()
{
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++) {
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
        KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

        if (_tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0) {
            pPropertyGridItem->SetReadOnly(FALSE);
        }
    }
}
