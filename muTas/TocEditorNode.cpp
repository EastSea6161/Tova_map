// TocEditorNode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocEditorNode.h"
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

// KTocEditorNode 대화 상자입니다.
#define IDD_PROPERTY_GRID 1000

IMPLEMENT_DYNAMIC(KTocEditorNode, KDialogExBase)

KTocEditorNode::KTocEditorNode(CWnd* pParent /*=NULL*/)
	: KDialogExBase(KTocEditorNode::IDD, pParent)
{
	NodeCategoryName     = _T("Node");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		NodeCategoryName     = _T("노드");
	}
	m_nPreNodeTemplateID = -1;
	NodeTemplateType     = 0;
	m_pNodeTable         = nullptr;

    m_nxAddedID = -1;
}

KTocEditorNode::~KTocEditorNode()
{
    try
    {
        if (m_pNodeTable != nullptr)
            m_pNodeTable->Unregister(this);
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

void KTocEditorNode::DoDataExchange(CDataExchange* pDX)
{
	KDialogExBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_DUMMY, m_stcDummy);
	DDX_Control(pDX, IDC_COMBO_TEMPLATE_NODE, m_cboTemplateNode);
}


BEGIN_MESSAGE_MAP(KTocEditorNode, KDialogExBase)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	
	ON_BN_CLICKED(IDC_BUTTON_START, &KTocEditorNode::OnBnClickedButtonEditStart)
	ON_BN_CLICKED(IDC_BUTTON_END,   &KTocEditorNode::OnBnClickedButtonEditEnd)

	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_DELETE_NODE, &KTocEditorNode::OnBnClickedButtonTemplateDeleteNode)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_SAVE_NODE, &KTocEditorNode::OnBnClickedButtonTemplateSaveNode)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATE_NODE, &KTocEditorNode::OnCbnSelchangeComboTemplateNode)

	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, &KTocEditorNode::OnPropertyGridItemValueChange)
    ON_BN_CLICKED(IDC_RADIO_ADD, &KTocEditorNode::OnBnClickedRadioAdd)
    ON_BN_CLICKED(IDC_RADIO_DELETE, &KTocEditorNode::OnBnClickedRadioDelete)
    ON_BN_CLICKED(IDC_RADIO_MODIFY, &KTocEditorNode::OnBnClickedRadioModify)
    ON_BN_CLICKED(IDC_RADIO_MOVE, &KTocEditorNode::OnBnClickedRadioMove)
    ON_BN_CLICKED(IDC_RADIO_ADDZone, &KTocEditorNode::OnBnClickedRadioAdd)
END_MESSAGE_MAP()


// KTocEditorNode 메시지 처리기입니다.
BOOL KTocEditorNode::OnInitDialog()
{
	KDialogExBase::OnInitDialog();
	KDialogExBase::UseKeyEscEnter(false, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.    
    AddImChampTooltip(IDC_RADIO_ADD,    _T("노드 추가"));
    AddImChampTooltip(IDC_RADIO_DELETE, _T("노드 삭제"));
    AddImChampTooltip(IDC_RADIO_MODIFY, _T("노드 속성 편집"));
    AddImChampTooltip(IDC_RADIO_MOVE,   _T("노드 위치 이동"));
    AddImChampTooltip(IDC_RADIO_ADDZone,_T("존노드 추가"));
    
    AddImChampTooltip(IDC_BUTTON_START, _T("편집 시작"));
    AddImChampTooltip(IDC_BUTTON_END,   _T("편집 종료"));

    AddImChampTooltip(IDC_BUTTON_TEMPLATE_DELETE_NODE, _T("노드 입력 템플릿 삭제"));
    AddImChampTooltip(IDC_BUTTON_TEMPLATE_SAVE_NODE,   _T("노드 입력 템플릿 추가"));
    
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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH KTocEditorNode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogExBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	return hbr;
}


void KTocEditorNode::OnSize(UINT nType, int cx, int cy)
{
	KDialogExBase::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KTocEditorNode::SetTargetMap( KTarget* a_pTarget, KMapView* a_pMapView )
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;

    try
    {
    	if (m_pNodeTable != nullptr)
            m_pNodeTable->Unregister(this);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }

	m_pNodeTable = m_pTarget->Tables()->FindTable(TABLE_NODE);	
	m_pNodeTable->Register(this);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ADD);
	pButton->SetCheck(BST_CHECKED);
	
	LoadInitialBlankProperties();
	LoadTemplateComboBoxNode();	
	LoadNodePropertiesValue();
	InitialControl();
}

void KTocEditorNode::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        std::map<CString, CString> mapTemplateValue;
        GetCurrentNodeTemplateValue(mapTemplateValue);

        if(_tcsicmp(a_strSubjectName, TABLE_NODE) == 0)
        {
            LoadInitialBlankProperties();
            LoadNodePropertiesValue(mapTemplateValue);
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

void KTocEditorNode::LoadInitialBlankProperties()
{
    int nButton = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_ADDZone);

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
                    if (nButton == IDC_RADIO_ADDZone) {
                        pGridItem = KxtpPropertyGridUtil::AddNodeTypeItemAtNodeEdit(pPropertyGridItemParent, m_pTarget, pColumn, 1);
                    }
                    else {
                        pGridItem = KxtpPropertyGridUtil::AddNodeTypeItemAtNodeEdit(pPropertyGridItemParent, m_pTarget, pColumn, 0);
                    }
				}
                else {
                    pGridItem = KxtpPropertyGridUtil::AddCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
                }
			}
			else if(pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
			{
				pGridItem = KxtpPropertyGridUtil::AddMultiCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
			}
			else
			{
                CString strItemCaption(_T(""));
                if (_tcsicmp(pColumn->Name(), COLUMN_NODE_ID) == 0)
                {
                    strItemCaption.Format(_T("%s"), pColumn->Caption());
                }
                else
                {
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
	}	
	catch (...)
	{
		//AfxMessageBox(_T("Propertis 초기화 중 오류가 발생하였습니다."));
	}	
}

void KTocEditorNode::EditableCheck( CXTPPropertyGridItem* pPropertyGridItem, KIOColumn* a_pIOColumn )
{
	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_ADDZone);

    bool bEditable(false);
    if (nCheckedRadioBtn == IDC_RADIO_ADD || nCheckedRadioBtn == IDC_RADIO_ADDZone) {
        bEditable = true;
    }

	if (bEditable == false ) {
		pPropertyGridItem->SetReadOnly();
		return;
	}

	CString strColumnName(a_pIOColumn->Name());
	
    if (strColumnName.CompareNoCase(COLUMN_NODE_ID) == 0)
    {
        CString strTemp(_T("-"));
        if (m_nxAddedID < 0) {
            CXTPPropertyGridItemMetrics* pItem = pPropertyGridItem->GetValueMetrics();
            pItem->m_clrBack = CXTPPaintManagerColor(RGB(255, 255, 255)); 
        }
        else {
            strTemp.Format(_T("%I64d"), m_nxAddedID);
            CXTPPropertyGridItemMetrics* pItem = pPropertyGridItem->GetValueMetrics();
            pItem->m_clrBack = CXTPPaintManagerColor(RGB(0, 0, 255));          
        }
                
        pPropertyGridItem->SetValue(strTemp);
        pPropertyGridItem->SetReadOnly();
        return;
    }

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
        if (nxMaxZoneID > 0) {
            strTemp.Format(_T("%I64d"), nxMaxZoneID);
            pPropertyGridItem->SetValue(strTemp);
        }      

        //★ 존노드 추가
        if (nCheckedRadioBtn == IDC_RADIO_ADDZone) {
            pPropertyGridItem->SetHidden(TRUE);
        }
        else {
            pPropertyGridItem->SetHidden(FALSE);
        }

        return;
    }
}

void KTocEditorNode::OnBnClickedRadioAdd()
{
    m_nxAddedID = -1;
    LoadInitialBlankProperties();
    LoadNodePropertiesValue();
    InitialControl();
    OnBnClickedButtonEditStart();
}

void KTocEditorNode::OnBnClickedRadioDelete()
{
    m_nxAddedID = -1;
    LoadInitialBlankProperties();
    InitialControl();
    OnBnClickedButtonEditStart();
}


void KTocEditorNode::OnBnClickedRadioModify()
{
    m_nxAddedID = -1;
    LoadInitialBlankProperties();
    InitialControl();
    OnBnClickedButtonEditStart();
}


void KTocEditorNode::OnBnClickedRadioMove()
{
    m_nxAddedID = -1;
    LoadInitialBlankProperties();
    InitialControl();
    OnBnClickedButtonEditStart();
}

void KTocEditorNode::OnBnClickedButtonEditStart()
{	
    m_nxAddedID = -1;
    int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_ADDZone);
        
    if (nCheckedRadioBtn == IDC_RADIO_ADD || nCheckedRadioBtn == IDC_RADIO_ADDZone)
    {
	    if (NodeTemplateDataCheck() == false) {
            StopEditing();
            return;
        }
    }

    KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
    if (spShapeEditController == NULL)   {
        spShapeEditController = m_pMapView->ShapeEditStart();
    }

    if (spShapeEditController == NULL) {
        AfxMessageBox(_T("편집 작업을 실행할 수 없습니다"));
    }

    std::map<CString, CString> mapKeyValue;
    spShapeEditController->SetEditingTarget(KEMShapeEditingLayerNode);

    switch(nCheckedRadioBtn)
    {    
    case IDC_RADIO_DELETE:
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskDelete);
        break;
    case IDC_RADIO_MODIFY:        
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskSingleEdit);
        break;
    case IDC_RADIO_MOVE:
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskMove);
        break;
    case IDC_RADIO_ADD:    
    case IDC_RADIO_ADDZone:    
    default:
        GetCurrentNodeTemplateValue(mapKeyValue);
        spShapeEditController->SetEditingTask(KEMShapeEditingTaskInsert);
        spShapeEditController->SetDefaultNodeInfo(mapKeyValue);
        break;
    }
}

LRESULT KTocEditorNode::OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam)
{
    CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

	if(wParam == XTP_PGN_ITEMVALUE_CHANGED) {
		KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
        /*if (spShapeEditController == nullptr) {
            return 0;
        }*/
        
        if (true/*spShapeEditController->GetEditingTarget() == KEMShapeEditingLayerNode && spShapeEditController->GetEditingTask() == KEMShapeEditingTaskInsert*/)
        {
            std::map<CString, CString> mapKeyValue;
            if (NodeTemplateDataCheck()) {                
                GetCurrentNodeTemplateValue(mapKeyValue);        
                if (spShapeEditController)
                    spShapeEditController->SetDefaultNodeInfo(mapKeyValue);

                //★ 여기서 저장한다.
                Update(mapKeyValue);
            }
        }
	}
    //★ 현재 버전에서는 불필요함.
    /*
	else if (wParam == XTP_PGN_INPLACEBUTTONDOWN)
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
    */

	return 0;
}


void KTocEditorNode::OnBnClickedButtonEditEnd()
{       
	StopEditing();
    InitialControl();
}


void KTocEditorNode::InitialControl()
{
	CWnd* pCtrlWindow = NULL;

	int  nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_ADDZone);
	switch(nCheckedRadioBtn)
	{
	case IDC_RADIO_ADD:
		pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_NODE);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_NODE);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_NODE);
		pCtrlWindow->EnableWindow(TRUE);		
		break;
	case IDC_RADIO_MODIFY:
	default:
		pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_NODE);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_NODE);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_NODE);
		pCtrlWindow->EnableWindow(FALSE);		
		break;
	}

    if (nCheckedRadioBtn == IDC_RADIO_ADD || nCheckedRadioBtn == IDC_RADIO_ADDZone) {
        if (m_nxAddedID >= 0)
            m_wndPropertyGrid.ShowWindow(SW_SHOW);
        else
            m_wndPropertyGrid.ShowWindow(SW_HIDE);
    }
    else {
        m_wndPropertyGrid.ShowWindow(SW_HIDE);
    }    
}

void KTocEditorNode::OnBnClickedButtonTemplateDeleteNode()
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


void KTocEditorNode::OnBnClickedButtonTemplateSaveNode()
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

void KTocEditorNode::LoadTemplateComboBoxNode()
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

void KTocEditorNode::NotifyNodeAdded( __int64 nxID )
{
    //★ 지도에서 Shape이 추가 된경우
    m_nxAddedID = nxID;
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
    }

    InitialControl();
}

void KTocEditorNode::Update( std::map<CString, CString>& mapKeyValue )
{
    if (m_nxAddedID < 0 || mapKeyValue.size() <= 0 || m_pTarget == nullptr)
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

    strSQL.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_NODE_ID, m_nxAddedID);

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

    m_pMapView->UpdateNodeLinkMapDisp(KLayerID::Node(), m_nxAddedID, nNodeType, strNodeLblColumn, strLbl);
    m_pMapView->MapRefresh();

    KIOTables* pIOTables = m_pTarget->Tables();

    KIOTable*  pIOTable  = NULL;
    pIOTable  = pIOTables->FindTable(TABLE_NODE);
    if (pIOTable) 
        pIOTable->Notify();

}

bool KTocEditorNode::NodeTemplateDataCheck()
{
	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

    int nType(1);

    for (int i = 0; i < nChildCount; i++)
    {
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
        KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

        if (_tcsicmp(pColumn->Name(), COLUMN_NODE_TYPE) == 0)
        {
            int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
            if (nConst >= 0)
            {
                CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
                nType = pGridConst->m_dwData;
            }
        }
    }

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

            if (_tcsicmp(pColumn->Name(), COLUMN_ZONE_ID) == 0)
            {
                //★ 존노드
                if (nType == 0) {
                    if (m_nxAddedID < 0) {
                        pPropertyGridItem->SetValue(_T(""));
                    }
                    else {
                        CString strTemp(_T("")); {
                            strTemp.Format(_T("%I64d"), m_nxAddedID);
                        }
                        pPropertyGridItem->SetValue(strTemp);
                    }

                    pPropertyGridItem->SetReadOnly(TRUE);
                    continue;
                }

                //★ 일반노드
                if (!KBulkDBaseNode::ExistZone(m_pTarget, _ttoi64(strValue)))
                {
                    Integer nxMaxZoneID = KBulkDBaseNode::MaxZoneID(m_pTarget);
                    CString strMsg;

                    if (nxMaxZoneID < 0) {
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
                pPropertyGridItem->SetReadOnly(FALSE);
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

CString KTocEditorNode::ConvertTemplateSaveValueNode()
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

void KTocEditorNode::GetCurrentNodeTemplateValue(std::map<CString, CString>& mapTemplateValue)
{
	mapTemplateValue.clear();

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

		mapTemplateValue.insert(std::make_pair(strColumnName, strValue));
	}
}


void KTocEditorNode::GetNodeStartIDValue( Integer& ar_nxNodeID )
{
    //★ 2016-08-30 : 주석처리
    /*
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
	}*/
}


void KTocEditorNode::LoadNodePropertiesValue()
{
	if (m_cboTemplateNode.GetCount() < 1)
		return;

	int nSelectedIndex = m_cboTemplateNode.GetCurSel();
	if (nSelectedIndex <= 0)
	{
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

	if (mapTemplateValueNode.size() < 1) {
		return;
	}

	LoadNodePropertiesValue(mapTemplateValueNode);
}

void KTocEditorNode::LoadNodePropertiesValue(std::map<CString, CString>& mapTemplateValue)
{
	std::map<CString, CString>::iterator iter = mapTemplateValue.begin();
	std::map<CString, CString>::iterator end  = mapTemplateValue.end();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( NodeCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		iter = mapTemplateValue.find(pColumn->Name());
		if (iter == end)
			continue;

		CString strValue = iter->second;

		if (pColumn->PrimaryKey()) {
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


void KTocEditorNode::OnCbnSelchangeComboTemplateNode()
{
	StopEditing();
	LoadNodePropertiesValue();
}

void KTocEditorNode::ActionFeedback( KDialogFeedback::KEMActionFeedback emActionCode )
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
            SetNodeStartIDValue();
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

void KTocEditorNode::StopEditing()
{
    try
    {
        m_nxAddedID = -1;
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

void KTocEditorNode::SetNodeStartIDValue()
{
    //★ 2016-08-30 : 시작노드, 링크ID 체크는 ID 설정 기능에서 처리하므로 ...
    
    /*★ 2016-08-30
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
    */
}
