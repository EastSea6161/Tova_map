// TocEditorTransit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocEditorTransit.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"

#include "BulkDBaseTransit.h"
#include "XTPPropertyGridUtil.h"
#include "MultiCodeSettingDlg.h"
#include "ImChampFrameWindow.h"
#include "ImTasMenuPathDefine.h"

// KTocEditorTransit 대화 상자입니다.
#define IDD_PROPERTY_GRID 1000

IMPLEMENT_DYNAMIC(KTocEditorTransit, KDialogEx)

KTocEditorTransit::KTocEditorTransit(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTocEditorTransit::IDD, pParent), TransitCategoryName(_T("Transit")),
	  m_pTarget(nullptr), m_pMapView(nullptr), m_pTransitTable(nullptr)
{
    m_nxAddedID = -1;

	if (KmzSystem::GetLanguage() == KEMKorea) {
		TransitCategoryName     = _T("도시부 대중교통 노선운행 자료");
	}
}

KTocEditorTransit::~KTocEditorTransit()
{
    try
    {
        if (m_pTransitTable != nullptr)
            m_pTransitTable->Unregister(this);
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

void KTocEditorTransit::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TRANSIT_ID, m_cboTransitID);
	DDX_Control(pDX, IDC_STC_DUMMY, m_stcDummy);
}


BEGIN_MESSAGE_MAP(KTocEditorTransit, KDialogEx)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
    ON_BN_CLICKED(IDC_RADIO_ADD, &KTocEditorTransit::OnBnClickedRadioAdd)
    ON_BN_CLICKED(IDC_RADIO_MODIFY, &KTocEditorTransit::OnBnClickedRadioModify)
    ON_CBN_SELCHANGE(IDC_COMBO_TRANSIT_ID, &KTocEditorTransit::OnCbnSelchangeComboTransitId)
    ON_BN_CLICKED(IDC_BUTTON_START, &KTocEditorTransit::OnBnClickedButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_END, &KTocEditorTransit::OnBnClickedButtonEnd)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, &KTocEditorTransit::OnPropertyGridItemValueChange)
    ON_CBN_EDITCHANGE(IDC_COMBO_TRANSIT_ID, &KTocEditorTransit::OnCbnEditchangeComboTransitId)
END_MESSAGE_MAP()


// KTocEditorTransit 메시지 처리기입니다.
BOOL KTocEditorTransit::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(false, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    AddImChampTooltip(IDC_RADIO_ADD,    _T("노선정보 추가"));
    AddImChampTooltip(IDC_RADIO_MODIFY, _T("노선경로 수정"));
    AddImChampTooltip(IDC_BUTTON_START, _T("편집 시작"));
    AddImChampTooltip(IDC_BUTTON_END,   _T("편집 종료"));
       
	SetBackgroundColor(RGB(255,255,255));
	CRect rectDummy;
	m_stcDummy.GetWindowRect(&rectDummy);
	ScreenToClient(&rectDummy);

	m_wndPropertyGrid.Create(rectDummy, this, IDD_PROPERTY_GRID);
	m_wndPropertyGrid.HighlightChangedItems(TRUE);
	m_wndPropertyGrid.SetVariableItemsHeight(TRUE);
	m_wndPropertyGrid.SetTheme(xtpGridThemeOffice2007);
	m_wndPropertyGrid.SetShowInplaceButtonsAlways(TRUE);
	m_wndPropertyGrid.SetHelpHeight(25);
    m_wndPropertyGrid.NavigateItems(TRUE, FALSE);

	return TRUE;  
}

HBRUSH KTocEditorTransit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	return hbr;
}

void KTocEditorTransit::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KTocEditorTransit::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
	try
	{
		if(_tcsicmp(a_strSubjectName, TABLE_TRANSIT) == 0)
		{
			LoadComboTransitID();
            LoadInitialBlankProperties();
            LoadProperties();
            InitialControl();
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}	
}


void KTocEditorTransit::SetTargetMap( KTarget* a_pTarget, KMapView* a_pMapView )
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;

    try
    {
        if (m_pTransitTable != nullptr)
            m_pTransitTable->Unregister(this);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

	m_pTransitTable = m_pTarget->Tables()->FindTable(TABLE_TRANSIT);	
	m_pTransitTable->Register(this);

    CheckRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY, IDC_RADIO_ADD );
	LoadComboTransitID();
    LoadInitialBlankProperties();
    //LoadProperties();
    	
	InitialControl();
}

void KTocEditorTransit::LoadComboTransitID()
{
	try
	{
		int nCurTransitNo = -9;
		int nSelectedItem = 0;
		int nItem         = m_cboTransitID.GetCurSel();

		if (nItem >= 0)
		{
			nCurTransitNo = m_cboTransitID.GetItemData(nItem);
		}	

		m_cboTransitID.ResetContent();
		std::map<int, CString> mapTransitName;
		KBulkDBaseTransit::TransitName(m_pTarget, mapTransitName);

		int nCount = (int)mapTransitName.size();

		m_cboTransitID.SetRedraw(FALSE);
		m_cboTransitID.InitStorage(nCount, 20);
		
		AutoType iter = mapTransitName.begin();
		AutoType end  = mapTransitName.end();

		while (iter != end)
		{
			CString strName;
			strName.Format(_T("%d : %s"), iter->first, iter->second);
			nItem = m_cboTransitID.AddString(strName);
			m_cboTransitID.SetItemData(nItem, iter->first);

			if (iter->first == nCurTransitNo)
			{
				nSelectedItem = nItem;
			}
			++iter;
		}

		m_cboTransitID.SetRedraw(TRUE);

		if (m_cboTransitID.GetCount() > 0)
		{
			m_cboTransitID.SetCurSel(nSelectedItem);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KTocEditorTransit::OnCbnEditchangeComboTransitId()
{
    // 콤보타입이 DropDown으로 정의된 경우 Event 발생함
    // 한글입력은 허용되지 않는다. 오류가 있음.
    // 숫자와 영문만 허용
    // 한글문제로 서비스를 하지 않는 것이 타당해 보임.
    // 2014. 09. 05
    
    /*
    CString strValue(_T(""));
    m_cboTransitID.GetWindowText(strValue);

    if (strValue.GetLength() == 0)
    {
        m_cboTransitID.ShowDropDown(FALSE);
    }
    else
    {
        m_cboTransitID.ShowDropDown(TRUE);
        int nFind = m_cboTransitID.FindString(0, strValue);
        if (nFind >= 0)
        {
            m_cboTransitID.SetCurSel(nFind);
        }
        m_cboTransitID.SetWindowText(strValue);
        m_cboTransitID.SetEditSel(strValue.GetLength(), strValue.GetLength());
    }    

    SetCursor(LoadCursor(NULL, IDC_ARROW));
    */
}


void KTocEditorTransit::LoadInitialBlankProperties()
{
    m_wndPropertyGrid.ResetContent();
    if (m_pTransitTable == nullptr)
        return;

    try
    {
        const
        KIOColumns*    pIOColumns = m_pTransitTable->Columns();
        KCodeManager*  pCodeMgr   = m_pTarget->CodeManager();

        int nColumnCount = pIOColumns->ColumnCount();			

        CXTPPropertyGridItem* pPropertyGridItemParent = m_wndPropertyGrid.AddCategory( TransitCategoryName );
        for(int i = 0; i < nColumnCount; i++)
        {
            KIOColumn* pColumn = pIOColumns->GetColumn( i );     
            CXTPPropertyGridItem* pPropertyGridItem = NULL;

            if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_TYPE) == 0)
            {
                continue;
            }
			
			if (pColumn->ColumnType() == KEMIOColumnTypeUserDefine) {
				continue;
			}

            if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
            {				
                pPropertyGridItem = KxtpPropertyGridUtil::AddCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);   
                CXTPPropertyGridItemEnum* pGridItemEnum = static_cast<CXTPPropertyGridItemEnum*>(pPropertyGridItem);

                CXTPPropertyGridItemConstraints* pConstraints = pGridItemEnum->GetConstraints();
                if (pConstraints->GetCount() > 0)
                {
                    CXTPPropertyGridItemConstraint* pConstraint = pConstraints->GetConstraintAt(0);
                    pGridItemEnum->SetEnum(pConstraint->m_dwData);
                }
                //CXTPPropertyGridItemConstraint* pConst = pGridItemEnum->GetConstraints()->GetConstraintAt(0);
                //CXTPPropertyGridItemConstraint* pConst = pGridItemEnum->GetConstraints()->GetConstraintAt(0);
                //pGridItemEnum->SetEnum(pConst->m_dwData);
            }
            else if(pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
            {
                pPropertyGridItem = KxtpPropertyGridUtil::AddMultiCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
            }
            else
            {
                pPropertyGridItem =	pPropertyGridItemParent->AddChildItem(new CXTPPropertyGridItem(pColumn->Caption()));
                pPropertyGridItem->SetItemData((DWORD_PTR)pColumn);

				if (pColumn->DataType() == KEMIODataTypeDouble) {
					pPropertyGridItem->SetValue(_T("0.0"));
				} else if (pColumn->DataType() == KEMIODataTypeInteger) {
					pPropertyGridItem->SetValue(_T("0"));
				} else if (pColumn->DataType() == KEMIODataTypeString) {
					pPropertyGridItem->SetValue(_T("-"));
				}

                if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_PEAK_HEADWAY) == 0)
                {
                    pPropertyGridItem->SetValue(_T("5"));
                }
				
                if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_OFFPEAK_HEADWAY) == 0)
                {
                    pPropertyGridItem->SetValue(_T("15"));
                }

                if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_AVG_HEADWAY) == 0)
                {
                    pPropertyGridItem->SetValue(_T("10"));
                }

                if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_PEAK_FREQUENCY) == 0)
                {
                    pPropertyGridItem->SetValue(_T("12"));
                }

                if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_OFFPEAK_FREQUENCY) == 0)
                {
                    pPropertyGridItem->SetValue(_T("4"));
                }

				if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_TOTAL_FREQUENCY) == 0)
				{
					pPropertyGridItem->SetValue(_T("102"));
				}

				if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_COMMERCIAL_SPEED) == 0)
				{
					pPropertyGridItem->SetValue(_T("30"));
				}

                if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_SEAT_COUNT) == 0)
                {
                    pPropertyGridItem->SetValue(_T("24"));
                }
                if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_CAPACITY) == 0)
                {
                    pPropertyGridItem->SetValue(_T("42"));
                }
            }

            EditableCheck(pPropertyGridItem, pColumn);
        }

        pPropertyGridItemParent->Expand();
    }	
    catch (...)
    {
        //AfxMessageBox(_T("Propertis 초기화 중 오류가 발생하였습니다."));
    }	
}

void KTocEditorTransit::EditableCheck( CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn )
{
    /*int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);
    if (nCheckedRadioBtn == IDC_RADIO_MODIFY)
    {
        a_pPropertyGridItem->SetReadOnly();
        return;
    }*/

    CString strColumnName(a_pIOColumn->Name());

    if (a_pIOColumn->PrimaryKey())
    {
        a_pPropertyGridItem->SetReadOnly();
        a_pPropertyGridItem->SetValue(TSTAR);
        return;
    }
}

void KTocEditorTransit::InitialControl()
{
	// 기존 노선 정보 로드
	CWnd* pCtrlWindow = NULL;

	int  nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);
	switch(nCheckedRadioBtn)
	{
	case IDC_RADIO_ADD:
		pCtrlWindow = GetDlgItem(IDC_COMBO_TRANSIT_ID);
		pCtrlWindow->EnableWindow(FALSE);
		break;
	case IDC_RADIO_MODIFY:
	default:
        pCtrlWindow = GetDlgItem(IDC_COMBO_TRANSIT_ID);
		pCtrlWindow->EnableWindow(TRUE);
		break;
	}

    if (nCheckedRadioBtn == IDC_RADIO_ADD) {
        if (m_nxAddedID >= 0)
            m_wndPropertyGrid.ShowWindow(SW_SHOW);
        else
            m_wndPropertyGrid.ShowWindow(SW_HIDE);
    }
    else {
        m_wndPropertyGrid.ShowWindow(SW_SHOW);
    }    
}

void KTocEditorTransit::OnBnClickedRadioAdd()
{
	StopEditing();
    CheckRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY, IDC_RADIO_ADD );
    InitialControl();
    LoadInitialBlankProperties();
}


void KTocEditorTransit::OnBnClickedRadioModify()
{
    CheckRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY, IDC_RADIO_MODIFY );
    InitialControl();
    LoadInitialBlankProperties();
    LoadProperties();
}

void KTocEditorTransit::AddTransitStart()
{
    try
    {
        StopEditing();

        if (! TransitTemplateDataCheck())
        {
            return;
        }
        
        ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_TRANSIT);

        KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
        if (spShapeEditController == NULL)
        {
            spShapeEditController = m_pMapView->ShapeEditStart();
        }

        if (spShapeEditController != NULL)
        {
            std::map<CString, CString> mapKeyValue;
            GetCurrentTransitTemplateValue(mapKeyValue);
            spShapeEditController->SetDefaultTransitInfo(mapKeyValue);

            spShapeEditController->SetEditingTarget(KEMShapeEditingLayerTransit);
            spShapeEditController->SetEditingTask(KEMShapeEditingTaskInsert);        
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

void KTocEditorTransit::ModifyTransitStart()
{
	CWaitCursor cw;

    int nItem = m_cboTransitID.GetCurSel();

    if (nItem < 0) {
        return;
    }	

    int nCurTransitNo = m_cboTransitID.GetItemData(nItem);

    try
    {        
        StopEditing();

        ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_TRANSIT);
        KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
        if (spShapeEditController == NULL)
        {
            spShapeEditController = m_pMapView->ShapeEditStart();
        }

        if (spShapeEditController != NULL)
        {
            spShapeEditController->SetEditID((Integer)nCurTransitNo);
            spShapeEditController->SetEditingTarget(KEMShapeEditingLayerTransit);
            spShapeEditController->SetEditingTask  (KEMShapeEditingTaskSingleEdit);        
        }

        m_pMapView->ZoomTransitExtent((Integer)nCurTransitNo);
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

bool KTocEditorTransit::TransitTemplateDataCheck()
{
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TransitCategoryName );
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
            strValue = pPropertyGridItem->GetValue();
            if (strValue.GetLength() == 0)
            {
                CString strMsg;
                if(_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_DETAIL_TYPE) == 0 && pPropertyGridItem->GetConstraints()->GetCount() == 0)
                {
                    strMsg.Format(_T("타입을 설정해 주세요.\n%s"), PM_NETWORK::UrbanTransitModeDefine);
                }
                else
                {
                    strMsg.Format(_T("%s - 값을 선택해 주세요."), pColumn->Caption());
                }
                
                AfxMessageBox(strMsg);
                pPropertyGridItem->Select();
                return false;
            }

            int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(strValue);
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

            /* 2016-09-06 : 이름 체크
            if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_NAME) == 0)
            {
                strValue = strValue.Trim();
                if (strValue.GetLength() == 0)
                {
                    CString strMsg(_T("노선의 이름을 입력해주세요"));
                    AfxMessageBox(strMsg);
                    pPropertyGridItem->SetValue(_T(""));
                    pPropertyGridItem->Select();
                    return false;
                }
            }
            */
        }
    }

    return true;
}

void KTocEditorTransit::OnCbnSelchangeComboTransitId()
{
	StopEditing();
    LoadProperties();
}

void KTocEditorTransit::LoadProperties()
{
    try
    {
        int nTransitID(0);

        int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);
        if (nCheckedRadioBtn == IDC_RADIO_ADD) {
            if (m_nxAddedID < 0)
                return;

            nTransitID = (int)m_nxAddedID;
        }
        else {
            int nIndex = m_cboTransitID.GetCurSel();
            if (nIndex < 0)
                return;

            nTransitID = m_cboTransitID.GetItemData(nIndex);   
        }
                   
        std::map<CString, CString> mapKeyValue;
        KBulkDBaseTransit::TransitInfoQuery(m_pTarget, (Integer)nTransitID, mapKeyValue);

        CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TransitCategoryName );
        CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
        int nChildCount = pPropertyGridItems->GetCount();

        AutoType iter = mapKeyValue.begin();
        AutoType end  = mapKeyValue.end();

        for (int i = 0; i < nChildCount; i++)
        {
            CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
            KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

            iter = mapKeyValue.find(pColumn->Name());
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
    catch (...)
    {
        TxLogDebugException();
    }
}

void KTocEditorTransit::GetCurrentTransitTemplateValue(std::map<CString, CString>& ar_mapTemplateValue)
{
    ar_mapTemplateValue.clear();

    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TransitCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CString strValue(_T(""));
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
        KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
        CString    strColumnName = pColumn->Name();

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

void KTocEditorTransit::ActionFeedback( KDialogFeedback::KEMActionFeedback emActionCode )
{
    try
    {
        if (emActionCode == KDialogFeedback::KEMActionFeedbackDeActivate)
        {
            StopEditing();
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

void KTocEditorTransit::StopEditing()
{
    m_nxAddedID = -1;

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


void KTocEditorTransit::OnBnClickedButtonStart()
{    
    int  nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);
    switch(nCheckedRadioBtn)
    {
    case IDC_RADIO_ADD:
        AddTransitStart();
        break;
    case IDC_RADIO_MODIFY:
    default:
        ModifyTransitStart();
        break;
    }    
}


void KTocEditorTransit::OnBnClickedButtonEnd()
{
    StopEditing();
    InitialControl();
}


LRESULT KTocEditorTransit::OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam)
{
    if(wParam != XTP_PGN_ITEMVALUE_CHANGED)
        return 0;

    KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
    if (TransitTemplateDataCheck()) {
        std::map<CString, CString> mapKeyValue; {
            GetCurrentTransitTemplateValue(mapKeyValue);
        }

        if (spShapeEditController)
            spShapeEditController->SetDefaultTransitInfo(mapKeyValue);

        UpdateTransit(mapKeyValue);
    }

    return 0;
}


void KTocEditorTransit::NotifyRouteAdd( __int64 nxID )
{
    m_nxAddedID = nxID;

    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TransitCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
        KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

        if (_tcsicmp(pColumn->Name(), COLUMN_TRANSIT_ID) == 0) {
            CString strValue(_T("")); {
                strValue.Format(_T("%I64d"), m_nxAddedID);
            }
            pPropertyGridItem->SetValue(strValue);
            CXTPPropertyGridItemMetrics* pItem = pPropertyGridItem->GetValueMetrics();
            pItem->m_clrBack = CXTPPaintManagerColor(RGB(0, 0, 255));
            break;
        }
    }

    InitialControl();
}

void KTocEditorTransit::UpdateTransit( std::map<CString, CString>& mapKeyValue )
{
    if (mapKeyValue.size() <= 0 || m_pTarget == nullptr)
        return;

    __int64 nxID(0);

    int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);
    if (nCheckedRadioBtn == IDC_RADIO_ADD) {
        if (m_nxAddedID < 0)
            return;

        nxID = m_nxAddedID;
    }
    else {
        int nCnt = m_cboTransitID.GetCount();
        if (nCnt <= 0)
            return;

        int nIndex = m_cboTransitID.GetCurSel();
        if (nIndex < 0)
            return;

        int nTransitID = m_cboTransitID.GetItemData(nIndex);      
        nxID = (__int64)nTransitID;
    }

    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T(" Update %s Set "), TABLE_TRANSIT);
    }

    bool bFirst(true);
    for (auto iter = mapKeyValue.begin(); iter != mapKeyValue.end(); ++iter) {
        CString strKey   = iter->first;
        CString strValue = iter->second;

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

    strSQL.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_TRANSIT_ID, nxID);

    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    spDBase->ExecuteUpdate(strSQL);

    KIOTables* pIOTables = m_pTarget->Tables();

    KIOTable*  pIOTable  = NULL;
    pIOTable  = pIOTables->FindTable(TABLE_TRANSIT);
    if (pIOTable) 
        pIOTable->Notify();
}
