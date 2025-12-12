// ZoneGroupGeneration.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ODGroupGeneration.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "CodeManagementDlg.h"
//#include "StringChecker.h"
#include "FormulaInputExp.h"

#include "../kmzModel/KParser.h"
//^#include "DBaseConnector.h"
#include "TClassRange.h"
//^#include "ParseString.h"
#include "ImChampFrameWindow.h"
#include "KxBulkDbase.h"
#include "InformationDlg.h"
//^#include "DevideDataInterval.h"
// KODGroupGeneration 대화 상자입니다.

IMPLEMENT_DYNAMIC(KODGroupGeneration, KResizeDialogEx)

    KODGroupGeneration::KODGroupGeneration(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
    : KResizeDialogEx(KODGroupGeneration::IDD, pParent)
{
    m_pTarget    = a_pTarget;
    m_nRunResult = 0;
    m_bCalcData  = false;
}

KODGroupGeneration::~KODGroupGeneration()
{
}

void KODGroupGeneration::DoDataExchange(CDataExchange* pDX)
{
    KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CBO_GROUP_NAME, m_cboGroupName);
    DDX_Control(pDX, IDC_REPORT1,        m_wndReport);
    DDX_Control(pDX, IDC_REPORT2,        m_wndReport2);
    DDX_Control(pDX, IDC_CBO_OUT_COLUMN, m_cboOutColumnName);
}

BEGIN_MESSAGE_MAP(KODGroupGeneration, KResizeDialogEx)
    ON_CBN_SELCHANGE(IDC_CBO_GROUP_NAME,    &KODGroupGeneration::OnCbnSelchangeCboGroupName)
    ON_BN_CLICKED   (IDC_BUTTON_MODIFY,     &KODGroupGeneration::OnBnClickedButtonModify)
    ON_BN_CLICKED   (IDC_BUTTON_EXPRESSION, &KODGroupGeneration::OnBnClickedButtonExpression)

    ON_NOTIFY(NM_CLICK, IDC_REPORT1,   &KODGroupGeneration::OnReportItemClick)
    ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, &KODGroupGeneration::OnReportItemValueChanged)
    ON_BN_CLICKED(IDOK, &KODGroupGeneration::OnBnClickedOk)
    ON_BN_CLICKED(IDC_CHECK_NEW, &KODGroupGeneration::OnBnClickedCheckNew)
    ON_BN_CLICKED(IDC_BUTTON_CALC, &KODGroupGeneration::OnBnClickedButtonCalc)
END_MESSAGE_MAP()


void KODGroupGeneration::OnCbnSelchangeCboGroupName()
{
    try
    {
        LoadReportData();
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

void KODGroupGeneration::LoadCodeGroup()
{
    try
    {
        int nPreSelectCodeGroupKey(-99);
        int nCurSel = m_cboGroupName.GetCurSel();        
        if (nCurSel >=0)
        {
            KCodeGroup* pCodeGroup = (KCodeGroup*)m_cboGroupName.GetItemData(nCurSel);
            nPreSelectCodeGroupKey = pCodeGroup->CodeGroupKey();
        }

        KCodeManager* pCodeManager = m_pTarget->CodeManager();        

        m_cboGroupName.ResetContent();

        std::vector<KCodeGroup*> vecCodeGroups;
        pCodeManager->GetCodeGroups(vecCodeGroups);

        nCurSel = 0;
        size_t nxCount = vecCodeGroups.size();
        for (size_t i=0; i<nxCount; i++)
        {
            KCodeGroup* pCodeGroup = vecCodeGroups[i];
            CString strCodeYN = pCodeGroup->SystemCodeYN();
            if ( strCodeYN.CompareNoCase(_T("Y")) == 0 )
                continue;

            int nIndex  = m_cboGroupName.AddString(pCodeGroup->CodeGroupName());
            m_cboGroupName.SetItemData(nIndex, (DWORD_PTR)pCodeGroup);
            int nCodeGroupKey = pCodeGroup->CodeGroupKey();
            if (nCodeGroupKey == nPreSelectCodeGroupKey)
            {
                nCurSel = i;
            }
        }

        m_cboGroupName.SetCurSel(nCurSel);
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


void KODGroupGeneration::OnBnClickedButtonModify()
{
    try
    {
        KCodeManager* pCodeManager = m_pTarget->CodeManager();
        KCodeManagementDlg dlg;

        dlg.SetTarget(m_pTarget);
        dlg.CodeManager(pCodeManager);
        dlg.DoModal();

        LoadCodeGroup();    
        LoadReportData();
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

void KODGroupGeneration::OnBnClickedButtonExpression()
{
    try
    {
        KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_SED_VECTOR);
        const 
            KIOColumns* pIOColumns = pTable->Columns();
        int nCount = pIOColumns->ColumnCount();

        m_vecFomulaField.clear();

        for (int i=0; i <nCount; i++)
        {
            KIOColumn* pIOColumn = pIOColumns->GetColumn(i);

            if (pIOColumn->DataType() != KEMIODataTypeDouble)
                continue;

            TFormulaField oFormulaField;
            oFormulaField.TFieldName    = pIOColumn->Name();
            oFormulaField.TFieldCaption = pIOColumn->Caption();

            m_vecFomulaField.push_back(oFormulaField);
        }

        if (m_vecFomulaField.size() == 0)
        {
            AfxMessageBox(_T("사회경제지표 관련 정보가 존재하지 않습니다."));
            return;
        }

        KFormulaExpression oExpression;
        KFormulaInputExp   oFormulaInputDlg;

        oFormulaInputDlg.UsableFunction(oExpression.Functions());
        oFormulaInputDlg.UsableOperator(oExpression.Operators());
        oFormulaInputDlg.UsableField(m_vecFomulaField);

        if (oFormulaInputDlg.DoModal() != IDOK)
            return;

        CString strFormulaCaption = oFormulaInputDlg.GetFormulaCaption();

        SetDlgItemText(IDC_EDIT_EXPRESSION, strFormulaCaption);
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


BOOL KODGroupGeneration::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    try
    {
        CheckDlgButton(IDC_CHECK_NEW, BST_CHECKED);
        GetDlgItem(IDC_CHECK_NEW)->ShowWindow(SW_HIDE);

        InitReportCtrl();
        LoadCodeGroup();
        LoadReportData();
        LoadOutColumnName();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

    return TRUE;  
}


BOOL KODGroupGeneration::PreTranslateMessage(MSG* pMsg)
{
    if (WM_LBUTTONDOWN == pMsg->message)
    {
        HWND hwnd = pMsg->hwnd;
        if (hwnd == GetDlgItem(IDC_EDIT_EXPRESSION)->m_hWnd)
        {
            OnBnClickedButtonExpression();
            return TRUE;
        }		
    }

    return KResizeDialogEx::PreTranslateMessage(pMsg);
}

void KODGroupGeneration::InitReportCtrl()
{
    try
    {
        KReportCtrlSetting::Default(m_wndReport, TRUE, FALSE, FALSE);
        CXTPReportColumn* pColumn = NULL;
        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Code"), 50, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Name"), 40));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("< Range <="),   65));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(3, _T("Count"),   30));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        m_wndReport.Populate();

        KReportCtrlSetting::Default(m_wndReport2, TRUE, FALSE, FALSE);
        pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(0, _T("Code"), 50, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(1, _T("Name"), 40));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(2, _T("Origin"),   30));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport2.AddColumn(new CXTPReportColumn(3, _T("Destination"),   30));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        m_wndReport2.Populate();
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

void KODGroupGeneration::LoadReportData()
{
    try
    {
        m_wndReport.ResetContent();
        m_wndReport2.ResetContent();

        int nCurSel = m_cboGroupName.GetCurSel();  
        if (nCurSel < 0)
            return;

        KCodeGroup* pCodeGroup = (KCodeGroup*)m_cboGroupName.GetItemData(nCurSel);

        std::map<int, CString> mapCode;
        pCodeGroup->GetCodes(mapCode);

        std::map<int, CString>::iterator iter = mapCode.begin();
        std::map<int, CString>::iterator end  = mapCode.end();

        CXTPReportRecord*     pRecord = NULL;
        CXTPReportRecordItem* pItem   = NULL;

        struct TCodeCaption
        {
            int     nCode;
            CString strCodeName;
        };

        std::vector<TCodeCaption> vecCodeCaption;

        int nIndex(0);
        while (iter != end)
        {
            CString strTemp;
            strTemp.Format(_T("%d"), iter->first);
            pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
            pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
            pItem->SetCaption(strTemp);
            pItem->SetAlignment(DT_CENTER);
            pItem->SetEditable (FALSE);
            pItem->SetItemData(iter->first);

            pItem = pRecord->AddItem(new CXTPReportRecordItemText);
            pItem->SetCaption(iter->second);
            pItem->SetAlignment(DT_CENTER);
            pItem->SetEditable (FALSE);
            pItem->SetItemData(iter->first);

            CString strValue, strCaption;
            strValue.Format(_T("%.1f"), 1000.0 * nIndex);
            strCaption.Format(_T("%.1f-%.1f"), 1000.0 * nIndex, 1000.0 * (nIndex+1));
            if (nIndex==0)
            {
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
            }
            else
            {
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));		
            }		

            pItem->SetCaption(strCaption);

            pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
            pItem->SetAlignment(DT_CENTER);
            pItem->SetEditable (FALSE);
            
            TCodeCaption oCodeCaption;
            oCodeCaption.nCode       = iter->first;
            oCodeCaption.strCodeName = iter->second;
            vecCodeCaption.push_back(oCodeCaption);

            nIndex++;
            ++iter;
        }

        m_wndReport.Populate();
                
        int nCodeValue(1);
        size_t nxCount = vecCodeCaption.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TCodeCaption& oOriginCodeCaption = vecCodeCaption[i];
            
            for (size_t m=0; m<nxCount; m++)
            {
                pRecord = m_wndReport2.AddRecord(new CXTPReportRecord());

                TCodeCaption& oDestCodeCaption = vecCodeCaption[m];

                CString strTemp;
                strTemp.Format(_T("%d"), nCodeValue);
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
                pItem->SetCaption(strTemp);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetEditable (FALSE);
                pItem->SetItemData((DWORD_PTR)nCodeValue);

                strTemp.Format(_T("%s:%s"), oOriginCodeCaption.strCodeName, oDestCodeCaption.strCodeName);
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
                //pItem->SetCaption(strTemp);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetEditable (TRUE);

                strTemp.Format(_T("%d:%s"), oOriginCodeCaption.nCode, oOriginCodeCaption.strCodeName);
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
                pItem->SetItemData((DWORD_PTR)oOriginCodeCaption.nCode);
                pItem->SetCaption(strTemp);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetEditable (FALSE);

                strTemp.Format(_T("%d:%s"), oDestCodeCaption.nCode, oDestCodeCaption.strCodeName);
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
                pItem->SetItemData((DWORD_PTR)oDestCodeCaption.nCode);
                pItem->SetCaption(strTemp);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetEditable (FALSE);

                nCodeValue++;
            }
        }

        m_wndReport2.Populate();
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

void KODGroupGeneration::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
    try
    {
        XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

        if (!pItemNotify->pRow || !pItemNotify->pColumn)
            return;

        int nIndex = pItemNotify->pColumn->GetItemIndex();

        if ( 2 == nIndex )
        {
            CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pItemNotify->pItem;
            m_strPreValue   = pItemText->GetValue();
            m_strPreCaption = pItemText->GetCaption(NULL);
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

void KODGroupGeneration::OnReportItemValueChanged( NMHDR* pNotifyStruct, LRESULT* pResult )
{
    try
    {
        XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

        if (!pItemNotify->pRow || !pItemNotify->pColumn)
            return;

        int nRowIndex    = pItemNotify->pRow->GetIndex();
        int nColumnIndex = pItemNotify->pColumn->GetItemIndex();

        int nRecordCount = m_wndReport.GetRecords()->GetCount();

        CXTPReportRecordItemText* pItemText;

        if ( 2 == nColumnIndex )
        {
            pItemText = (CXTPReportRecordItemText*)pItemNotify->pItem;
            CString strValue   = pItemText->GetValue();
            CString strCaption = pItemText->GetCaption(NULL);

            strValue.Replace(_T(" "), _T(""));
            strCaption.Replace(_T(" "), _T(""));

            std::vector<CString> vecValue;
            CString strTok;
            int     nTokenPos = 0; 

            if (nRowIndex == 0)
            {
                while (AfxExtractSubString(strTok, strValue, nTokenPos++, '-'))
                {
                    vecValue.push_back(strTok);
                }
            }
            else
            {
                while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '-'))
                {
                    vecValue.push_back(strTok);
                }
            }		

            if (vecValue.size() != 2)
            {
                pItemText->SetValue(m_strPreValue);
                pItemText->SetCaption(m_strPreCaption);
                m_wndReport.RedrawControl();
                return;			
            }

            if (nRowIndex == 0)
            {
                if (QBicStringChecker::IsNumeric(vecValue[0]) == false)
                {
                    AfxMessageBox(_T("숫자를 입력해 주세요."));
                    pItemText->SetValue(m_strPreValue);
                    pItemText->SetCaption(m_strPreCaption);
                    m_wndReport.RedrawControl();
                    return;
                }

                if (QBicStringChecker::IsNumeric(vecValue[1]) == false)
                {
                    AfxMessageBox(_T("숫자를 입력해 주세요."));
                    pItemText->SetValue(m_strPreValue);
                    pItemText->SetCaption(m_strPreCaption);
                    m_wndReport.RedrawControl();
                    return;
                }
            }
            else
            {
                if (QBicStringChecker::IsNumeric(strValue) == false)
                {
                    AfxMessageBox(_T("숫자를 입력해 주세요."));
                    pItemText->SetValue(m_strPreValue);
                    pItemText->SetCaption(m_strPreCaption);
                    m_wndReport.RedrawControl();
                    return;
                }
            }

            double dMin = _ttof(vecValue[0]);
            double dMax = 0.0;

            if (nRowIndex == 0) { dMax = _ttof(vecValue[1]); }
            else                { dMax = _ttof(strValue);    }

            if (dMin > dMax)
            {
                CString strOut;
                strOut.Format(_T("%.1f보다 큰 숫자를 입력해 주세요."), dMin);
                AfxMessageBox(strOut);

                pItemText->SetValue(m_strPreValue);
                pItemText->SetCaption(m_strPreCaption);
                m_wndReport.RedrawControl();
                return;
            }
            else
            {
                strCaption.Format(_T("%.1f-%.1f"), dMin, dMax);
                pItemText->SetCaption(strCaption);
            }	
        }

        RecalRange();
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

void KODGroupGeneration::RecalRange()
{
    try
    {
        CXTPReportRecords*    pRecords = NULL;
        CXTPReportRecord*     pRecord  = NULL;

        pRecords = m_wndReport.GetRecords();
        int nRecordCount = pRecords->GetCount();

        CXTPReportRecordItemText* pItemText    = NULL;

        double dPreValue = -9999;	

        for (int i=0; i<nRecordCount; i++)
        {
            pRecord   = pRecords->GetAt(i);
            pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);

            CString strCaption = pItemText->GetCaption(NULL);
            std::vector<CString> vecValue;
            CString strTok;
            int     nTokenPos = 0; 
            while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '-'))
            {
                vecValue.push_back(strTok);
            }

            double dMinValue = _ttof(vecValue[0]);
            double dMaxValue = _ttof(vecValue[1]);

            if (i == 0)
            {
                dPreValue = _ttof(vecValue[1]);
            }
            else
            {
                if (dPreValue != dMinValue)
                {
                    dMinValue = dPreValue;
                }
            }

            if (dPreValue > dMaxValue)
            {
                dMaxValue = dPreValue;				
            }

            CString strValue;
            strValue.Format(_T("%.1f"), dMaxValue);		
            strCaption.Format(_T("%.1f-%.1f"), dMinValue, dMaxValue);

            if (i == 0)
            {
                pItemText->SetValue(strCaption);
            }
            else
            {
                pItemText->SetValue(strValue);
            }

            pItemText->SetCaption(strCaption);		
            dPreValue = dMaxValue;
        }

        m_wndReport.RedrawControl();
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

void KODGroupGeneration::LoadOutColumnName()
{
    try
    {
        CString strPreSelName(_T(""));

        int nPreSel = m_cboOutColumnName.GetCurSel();
        if (nPreSel >=0)
        {
            KIOColumn* pIOColumn = (KIOColumn*)m_cboOutColumnName.GetItemData(nPreSel);
			try // delete Coloumn이후 m_cboOutColumnName에 반영이 안되어
			{
				strPreSelName = pIOColumn->Name();
			}
			catch (...)
			{
				TxLogDebugException();	
				strPreSelName = _T("");
			}
        }
        m_cboOutColumnName.ResetContent();

        KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
        const 
        KIOColumns* pIOColumns = pTable->Columns();
        int nCount = pIOColumns->ColumnCount();

        int nSelectIndex(0);
        int nCodeColumnCount(0);
        for (int i=0; i <nCount; i++)
        {
            KIOColumn* pIOColumn = pIOColumns->GetColumn(i);

            if (pIOColumn->CodeType() != KEMIOCodeTypeIsSingleCode)
                continue;

            int nIndex = m_cboOutColumnName.AddString(pIOColumn->Caption());
            m_cboOutColumnName.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

            if (strPreSelName.CompareNoCase(pIOColumn->Name()) == 0)
            {
                nSelectIndex = nIndex;
            }
            nCodeColumnCount++;
        }

        if ( IsDlgButtonChecked(IDC_CHECK_NEW) == FALSE )
        {
            if (nCodeColumnCount > 0)
            {
                m_cboOutColumnName.ShowWindow(SW_SHOW);
                m_cboOutColumnName.SetCurSel(nSelectIndex);
                GetDlgItem(IDC_EDIT_COLUMN_NAME)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_CHECK_NEW)->EnableWindow(TRUE);
            }
            else
            {
                CheckDlgButton(IDC_CHECK_NEW, BST_CHECKED);
                GetDlgItem(IDC_CHECK_NEW)->EnableWindow(FALSE);
                m_cboOutColumnName.ShowWindow(SW_HIDE);
                GetDlgItem(IDC_EDIT_COLUMN_NAME)->ShowWindow(SW_SHOW);
            }
        }
        else
        {
            m_cboOutColumnName.ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDIT_COLUMN_NAME)->ShowWindow(SW_SHOW);
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

void KODGroupGeneration::OnBnClickedCheckNew()
{
    try
    {
        LoadOutColumnName();
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

void KODGroupGeneration::OnBnClickedOk()
{
    try
    {
        ZoneClass();
        KeyCodeName();

        if (InputCheck() == false)
            return;

        m_nRunResult = 0;
        m_bCalcData  = false;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

        QBicSimpleProgressThread::ExecuteThread(ThreadCaller, this, false, nLang); 

        if (m_nRunResult != 1)
        {
            AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
            return;
        }
        try
        {
            KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
            pTable->Notify();

            ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_PARAMETER_OD);            
        }
        catch (...)
        {
            TxLogDebugException();
        }

        KResizeDialogEx::OnOK();
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

bool KODGroupGeneration::InputCheck(bool bCheckOutColumn)
{
    try
    {
        if ( m_wndReport.GetRecords()->GetCount() == 0)
        {
            AfxMessageBox(_T("코드 그룹 명칭을 선택해 주세요."));
            GetDlgItem(IDC_CBO_GROUP_NAME)->SetFocus();
            return false;
        }

        CString strExpression(_T(""));
        GetDlgItemText(IDC_EDIT_EXPRESSION, strExpression);

        if (strExpression.GetLength() == 0)
        {
            AfxMessageBox(_T("수식을 설정해 주세요."));
            GetDlgItem(IDC_BUTTON_EXPRESSION)->SetFocus();
            return false;
        }

        if (bCheckOutColumn)
        {
            CString strOutColumnName(_T(""));
            if ( IsDlgButtonChecked(IDC_CHECK_NEW) == TRUE )
            {
                GetDlgItemText(IDC_EDIT_COLUMN_NAME, strOutColumnName);
                strOutColumnName.Trim();

                if (strOutColumnName.GetLength() == 0)
                {
                    AfxMessageBox(_T("컬럼명을 입력해 주세요."));
                    GetDlgItem(IDC_EDIT_COLUMN_NAME)->SetFocus();
                    return false;
                }
            }
            else
            {
                int nIndex = m_cboOutColumnName.GetCurSel();
                if (nIndex < 0)
                {
                    AfxMessageBox(_T("컬럼을 선택해 주세요."));
                    m_cboOutColumnName.SetFocus();
                    return false;
                }
            }
        }

        return true;
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
    return false;
}

unsigned __stdcall KODGroupGeneration::ThreadCaller( void* p )
{
    try
    {
        QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
        QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

        KODGroupGeneration* pZoneGroupGeneration = (KODGroupGeneration*)pParameter->GetParameter();
        pZoneGroupGeneration->Run();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

    return 0;
}

void KODGroupGeneration::Run()
{
    try
    {
        CString strExpressionCaption(_T(""));
        GetDlgItemText(IDC_EDIT_EXPRESSION, strExpressionCaption);

        std::vector<CString> vecField;
        CString strRealFormula;

        KFormulaExpression oExpression;
        oExpression.SetCaptionExpression(strExpressionCaption, m_vecFomulaField);
        vecField       = oExpression.GetUsedRealFieldName();
        strRealFormula = oExpression.GetRealExpression();

        int     nCntVecField = vecField.size();

        KIDKeyDoubleRecords valuesRecords(nCntVecField);
        GetSedValue(vecField, valuesRecords);
        double* dValueArray  = new double[nCntVecField];

        KParser oParser;
        oParser.SetExpr(strRealFormula);
        for (int i=0; i<nCntVecField; i++)
        {
            oParser.DefineVar(vecField[i],  &dValueArray[i]);
        }

        m_vecCalcData.clear();
        std::map<Integer, KSDoubleRecord*>::iterator iter = valuesRecords.begin();
        std::map<Integer, KSDoubleRecord*>::iterator end  = valuesRecords.end();

        m_mapResult.clear();
        while (iter != end)
        {
            Integer         nxZoneID = iter->first;
            KSDoubleRecord* pRecord  = iter->second;
            double dResultValue(0.0);
            try
            {
                for (int k=0; k<nCntVecField; k++)
                {
                    dValueArray[k] = pRecord->GetAt(k);
                }

                dResultValue = oParser.Eval();
                m_vecCalcData.push_back(dResultValue);

                int nClass = FindZoneClass(dResultValue);
                m_mapResult.insert(std::make_pair(nxZoneID, nClass));
            }
            catch (...) 
            {
            }    

            ++iter;
        }

        delete[] dValueArray;

        if (m_bCalcData == false)
        {            
            KIOColumn* pOutColumn = nullptr;
            CString strOutColumnName(_T(""));
            if ( IsDlgButtonChecked(IDC_CHECK_NEW) == TRUE )
            {
                GetDlgItemText(IDC_EDIT_COLUMN_NAME, strOutColumnName);
                strOutColumnName.Trim();
                strOutColumnName.Replace(_T(" "), _T(""));

                int nCodeGroupKey = AddCodeGroup();
                pOutColumn = AddParaODCodeColumn(nCodeGroupKey, strOutColumnName);            
            }
            else
            {
                int nIndex = m_cboOutColumnName.GetCurSel();
                pOutColumn = (KIOColumn*)m_cboOutColumnName.GetItemData(nIndex);
            }

            UpdateParaODTable(pOutColumn, m_mapResult);    
        }            

        m_nRunResult = 1;
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

void KODGroupGeneration::GetSedValue( std::vector<CString>& a_vecField, KIDKeyDoubleRecords& ar_valuesRecord )
{
    try
    {   
        ar_valuesRecord.RemoveAll();

        CString strSQL(_T(""));
        strSQL.Append      (_T(" Select zone_id "));

        for (size_t i=0; i<a_vecField.size(); i++)
        {
            strSQL.AppendFormat(_T(" ,%s "), a_vecField[i]);            
        }

        strSQL.AppendFormat(_T(" From  %s "), TABLE_SED_VECTOR);

        KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();   
        KResultSetPtr       spResultset       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultset->Next())
        {
            Integer nxZoneID = spResultset->GetValueInt64(0);
            KSDoubleRecord* pRecord = ar_valuesRecord.AddRecord(nxZoneID);

            int nIndex(1);
            for (size_t i=0; i<a_vecField.size(); i++)
            {
                double dValue = spResultset->GetValueDouble(nIndex);
                pRecord->SetAt(i, dValue);
                nIndex++;
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
}

void KODGroupGeneration::ZoneClass()
{
    try
    {
        m_vecZoneClass.clear();

        CXTPReportRecords*	  pRecords   = NULL;
        CXTPReportRecord*     pRecord    = NULL;
        CXTPReportRecordItem* pItemCode  = NULL;
        CXTPReportRecordItem* pItemRange = NULL;

        pRecords = m_wndReport.GetRecords();
        int nCount = pRecords->GetCount();

        CString strCaption(_T(""));
        for (int i=0; i<nCount; i++)
        {
            pRecord     = pRecords->GetAt(i);
            pItemCode   = pRecord->GetItem(0);
            pItemRange  = pRecord->GetItem(2);

            strCaption = pItemRange->GetCaption();
            std::vector<CString> vecValues = KParseString::ParseString(strCaption, _T('-'));

            if (vecValues.size() == 2)
            {
                TZoneClassRange oClassRange;
                oClassRange.m_nCodeValue = _ttoi(pItemCode->GetCaption());
                oClassRange.m_dMin       = _ttof(vecValues[0]);
                oClassRange.m_dMax       = _ttof(vecValues[1]);

                m_vecZoneClass.push_back(oClassRange);
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
}

int KODGroupGeneration::FindZoneClass( double a_dValue )
{
    size_t nxCount      = m_vecZoneClass.size();;
    int    nReturnValue = 99;

    try
    {
        for(size_t i=0; i<nxCount; i++)
        {
            TZoneClassRange& oClassRange = m_vecZoneClass[i];
            if (i == 0)
            {
                if (oClassRange.m_dMin <= a_dValue && a_dValue <= oClassRange.m_dMax)
                {
                    nReturnValue = oClassRange.m_nCodeValue;
                    break;
                }
            }
            else
            {
                if (oClassRange.m_dMin < a_dValue && a_dValue <= oClassRange.m_dMax)
                {
                    nReturnValue = oClassRange.m_nCodeValue;
                    break;
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

    return nReturnValue;
}

void KODGroupGeneration::UpdateParaODTable( KIOColumn* a_pColumn, std::map<Integer, int>& ar_mapValue )
{
    try
    {
        struct ZoneCode
        {
            Integer ZoneID;
            int     Code;
        };

        std::vector<ZoneCode> vecZoneCode;

        std::map<Integer, int>::iterator iter = ar_mapValue.begin();
        std::map<Integer, int>::iterator end  = ar_mapValue.end();

        while (iter != end)
        {
            ZoneCode oZoneCode;
              oZoneCode.ZoneID = iter->first;
              oZoneCode.Code   = iter->second;
                      
            vecZoneCode.push_back(oZoneCode);

            ++iter;
        }

        std::map<KIntKey, TCodeName>::iterator iterFind;
        std::map<KODKey, int> mapParaResult;
        size_t nxCount = vecZoneCode.size();
        for (size_t i=0; i<nxCount; i++)
        {
            ZoneCode& oOriginZoneCode = vecZoneCode[i];
            for (size_t j=0; j<nxCount; j++)
            {
                ZoneCode& oDestZoneCode = vecZoneCode[j];

                KODKey oKey(oOriginZoneCode.ZoneID, oDestZoneCode.ZoneID);
                iterFind = m_mapKeyCodeName.find(KIntKey(oOriginZoneCode.Code, oDestZoneCode.Code));
                if (iterFind != m_mapKeyCodeName.end())
                {
                    TCodeName& oCodeName = iterFind->second;
                    mapParaResult.insert(std::make_pair(oKey, oCodeName.m_nCode));
                }
            }
        }

        nxCount = mapParaResult.size();
        if (nxCount == 0)
            return;
                
        KIOTable*   pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
        KIOColumns oColumns;
        oColumns.AddNewColumn(a_pColumn);

        QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
        if (spProgressWindow)
        {
            spProgressWindow->SetStatus(_T("Database Insert..."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				spProgressWindow->SetStatus(_T("데이터 저장 중..."));
			}
			else {
				spProgressWindow->SetStatus(_T("Database Insert..."));
			}
        }

        KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

        try
        {
        	spDBaseConnection->BeginTransaction();
            KxBulkDbase oBulkDBase(spDBaseConnection, pTable, &oColumns);
            
            std::map<KODKey, int>::iterator iter = mapParaResult.begin();
            std::map<KODKey, int>::iterator end  = mapParaResult.end();

            while (iter != end)
            {
                oBulkDBase.ImportData(iter->first, iter->second);
                ++iter;
            }

            oBulkDBase.ExecuteFullOuterJoin();
            spDBaseConnection->Commit();
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
            spDBaseConnection->RollBack();
        }
        catch (...)
        {
        	TxLogDebugException();
            spDBaseConnection->RollBack();
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


int KODGroupGeneration::AddCodeGroup()
{
    int nCodeGroup(0);
    try
    {
        CString strGroupCodeName(_T(""));
        GetDlgItemText(IDC_EDIT_COLUMN_NAME, strGroupCodeName);
        strGroupCodeName.Trim();
        strGroupCodeName.Replace(_T(" "), _T(""));


        KCodeManager* pCodeManager = m_pTarget->CodeManager();
        int nNewCodeGroupKey = pCodeManager->NextGroupKey();
    	
        if ( pCodeManager->AddCodeGroup(nNewCodeGroupKey, strGroupCodeName) == false)
        {
            QBicMessageBox::MessageBox(_T("코드 생성에 실패하였습니다."), _T("Information"));
            throw;
        }

        KCodeGroup* pCodeGrup = m_pTarget->CodeManager()->FindCodeGroup(nNewCodeGroupKey);
        nCodeGroup = pCodeGrup->CodeGroupKey();

        std::map<KIntKey, TCodeName>::iterator iter = m_mapKeyCodeName.begin();
        std::map<KIntKey, TCodeName>::iterator end  = m_mapKeyCodeName.end();

        while (iter != end)
        {
            TCodeName& oCodeName = iter->second;
            pCodeGrup->AddCode(oCodeName.m_nCode, oCodeName.m_strName);
            ++iter;
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
        throw;
    }

    return nCodeGroup;
}

KIOColumn* KODGroupGeneration::AddParaODCodeColumn( int a_nCodeGroup, CString a_strColumnName )
{
    KIOColumn* pColumn = nullptr;
    try
    {
        KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);

        const KIOColumns* pColumCollection = pTable->Columns();

        int nMaxDisplayCount(1);
        int nCount = pColumCollection->ColumnCount();

        if (0 == nCount)
        {
            nMaxDisplayCount = 1;
        }
        else
        {
			for (int i = 0; i < nCount; i++)
			{
				KIOColumn* pColumn = pColumCollection->GetColumn(i);
				int nDisplayCount = pColumn->DisplayOrder();
				if (nMaxDisplayCount < nDisplayCount)
				{
					nMaxDisplayCount = nDisplayCount;
				}
			}
        }

        KIOColumn oColumn;
        oColumn.Name(pTable->GetUsableColumnName());
        oColumn.Caption(a_strColumnName);
        oColumn.CodeGroup(a_nCodeGroup);
        oColumn.CodeType(KEMIOCodeTypeIsSingleCode);
        oColumn.ColumnType(KEMIOColumnTypeUserDefine);
        oColumn.DataType(KEMIODataTypeInteger);
        oColumn.DisplayOrder(nMaxDisplayCount+1);
        oColumn.ColumnInfo(a_strColumnName);
        oColumn.PrimaryKey(false);
        oColumn.Visible(true);

        pColumn = pTable->AddColumn(oColumn, true);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        throw;
    }

    return pColumn;
}


void KODGroupGeneration::OnBnClickedButtonCalc()
{
    try
    {
        ZoneClass();

        if (InputCheck(false) == false)
            return;

        m_nRunResult = 0;
        m_bCalcData  = true;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

        QBicSimpleProgressThread::ExecuteThread(ThreadCaller, this, true, nLang); 

        if (m_nRunResult != 1)
        {
            AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
            return;
        }

        std::map<Integer, int>::iterator iter = m_mapResult.begin();
        std::map<Integer, int>::iterator end  = m_mapResult.end();

        std::map<int, int> mapCount;
        while (iter != end)
        {
            int nCodeValue = iter->second;

            std::map<int, int>::iterator iterFind = mapCount.find(nCodeValue);
            if ( iterFind == mapCount.end())
            {
                mapCount.insert(std::make_pair(nCodeValue, 1));
            }
            else
            {
                int& nCodeCount = iterFind->second;
                nCodeCount++;
            }
            ++iter;
        }

        CXTPReportRecords*	  pRecords   = NULL;
        CXTPReportRecord*     pRecord    = NULL;
        CXTPReportRecordItem* pItemCode  = NULL;
        CXTPReportRecordItem* pItemCount = NULL;

        pRecords   = m_wndReport.GetRecords();
        int nCount = pRecords->GetCount();

        CString strCaption(_T(""));
        for (int i=0; i<nCount; i++)
        {
            pRecord     = pRecords->GetAt(i);
            pItemCode   = pRecord->GetItem(0);
            pItemCount  = pRecord->GetItem(3);

            int nReportCodeValue = _ttoi(pItemCode->GetCaption());
            std::map<int, int>::iterator iterFind = mapCount.find(nReportCodeValue);
            if ( iterFind != mapCount.end())
            {
                strCaption.Format(_T("%d"), iterFind->second);
                pItemCount->SetCaption(strCaption);
            }
            else
            {
                pItemCount->SetCaption(_T("-"));
            }
        }

        m_wndReport.RedrawControl();

        KDevideDataInterval oData;
        oData.SetDataCalculate(m_vecCalcData);

        CString strMsg(_T(""));
        strMsg.AppendFormat(_T("Count : %d \r\n"), oData.GetDataCount());
        strMsg.AppendFormat(_T("Sum : %.3f \r\n"), oData.GetSum());
        strMsg.AppendFormat(_T("Average : %.3f \r\n"), oData.GetAverage());
        strMsg.AppendFormat(_T("Min : %.3f \r\n"), oData.GetMinValue());
        strMsg.AppendFormat(_T("Max : %.3f \r\n"), oData.GetMaxValue());
        strMsg.AppendFormat(_T("Variance : %.3f \r\n"), oData.GetVariance());
        strMsg.AppendFormat(_T("Std. Deviation : %.3f \r\n"), oData.GetStdDev());
        strMsg.AppendFormat(_T("Std. Error : %.3f \r\n"), oData.GetStdErr());

        KInformationDlg dlg;
        dlg.SetTextString(strMsg);
        dlg.DoModal();
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

void KODGroupGeneration::KeyCodeName()
{
    try
    {
    	m_mapKeyCodeName.clear();

        CXTPReportRecords*  pRecords = m_wndReport2.GetRecords();
        int nRecordsCount = pRecords->GetCount();
        
        CXTPReportRecord*         pRecord   = NULL;
        CXTPReportRecordItemText* pItemText = NULL;
        CString strValue;
        for (int i=0; i<nRecordsCount; i++)
        {
            pRecord   = pRecords->GetAt(i);
              pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(0);
              strValue  = pItemText->GetValue();
              int nCode = _ttoi(strValue);

              pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
              strValue  = pItemText->GetValue();
              CString strName = strValue;

              pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
              strValue  = pItemText->GetValue();
              int nOriginCode = _ttoi(strValue);

              pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(3);
              strValue  = pItemText->GetValue();
              int nDestCode = _ttoi(strValue);

            KIntKey   oKey(nOriginCode, nDestCode);
            TCodeName oCodeName;
            oCodeName.m_nCode   = nCode;
            oCodeName.m_strName = strName;

            m_mapKeyCodeName.insert(std::make_pair(oKey, oCodeName));
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

