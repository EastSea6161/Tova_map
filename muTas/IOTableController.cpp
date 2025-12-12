/**
 * @file Controller.cpp
 * @brief KController 구현 파일
 * @author 
 * @date 2011.04.13
 * @remark
 */



#include "StdAfx.h"
#include "KmzApp.h"
#include "IOTableController.h"
#include "ChildFrm.h"
#include "FieldCalculatorDlg.h"
#include "IOTableView.h"
#include "IOTransitTableView.h"
#include "kmz_common.h"
#include "MapView.h"
#include "SelectByAttributeDlg.h"
#include "QuickSearchDlg.h"
#include "TableAddFieldInfo.h"
#include "TableAddFieldDlg.h"
#include "TableModeAddFieldDlg.h"
#include "FTableModeAddFieldDlg.h"
#include "TablePurposeAddFieldDlg.h"
#include "TableZoneAddFieldDlg.h"
#include "DlgExportTable.h"
#include "Target.h"
#include "Mode.h"
#include "Purpose.h"
#include "ImChampFrameWindow.h"
#include "ODMatrixDlg.h"

#include "CalculateZoneDistance.h"
#include "EditAccessLineStation.h"
#include "DefVDFFactorDlg.h"
#include "TableRenameFieldDlg.h"
#include "TableEditFieldInfo.h"
#include "TableEditFieldDlg.h"
#include "TableEditCodeFieldDlg.h"
#include "TablePurposeEditFieldDlg.h"
#include "TableModeEditFieldDlg.h"
#include "TableZoneEditFieldDlg.h"
#include "InitRelationProcess.h"
#include "TableEditVehicleFieldDlg.h"

#include "TableVehicleConvAddFieldDlg.h"
#include "TableVehicleDeleteField.h"
#include "KBulkDBaseUtil.h"
#include "FTableODDeleteField.h"

#include "DlgImportNode.h"
#include "DlgImportLink.h"
#include "DlgImportZone.h"
#include "DlgImportAccessStopNode.h"
#include "DlgImportTerminalSchedule.h"
#include "DlgImportTurn.h"
#include "DlgImportTransit.h"
#include "DlgImportTransitLine.h"
#include "DlgImportTransitLineNode.h"
#include "DlgImportPod.h"
#include "DlgImportMode.h"
#include "DlgImportParaOD.h"
#include "DlgImportSedVector.h"
#include "DlgImportSedPop.h"
#include "DlgExportTurnTable.h"

#include "DlgTransitStyle.h"

#include "DBaseConnector.h"
#include "DefInterUrbanModeChoiceParaTypes.h"
#include "DBaseDefaultParaInterUrbanModeChoice.h"



KIOTableController::KIOTableController(KTarget* pTarget)
	:	m_pTarget(pTarget),
		m_pIOTable(NULL),
		m_pIOTableView(NULL)
{
}


KIOTableController::~KIOTableController(void)
{
}

KMapView* KIOTableController::ShowMapView(bool bShow /* = true */)
{
    KMapView* pMapView = NULL;
    CChildFrame* pChildFrame = ImChampFrameWindow::GetActiveFrame();
    if (pChildFrame) {
        pChildFrame->ShowMapView(bShow);
        pMapView = ImChampFrameWindow::GetActiveMapView();
    }
    else
    {
        TxLogDebugWarning();
        return NULL;
    }
    
    return pMapView;
}

void KIOTableController::Attach(KIOTable* pTable, KIOTableView* pView)
{
	m_pIOTable = pTable;
	if(NULL != m_pIOTable)
	{
		m_strTableName = m_pIOTable->Name();
	}

	m_pIOTableView = pView;
	if(NULL != m_pIOTableView)
	{
		m_pIOTableView->SetController(this);
	}
}


LPCTSTR KIOTableController::Name(void)
{
	return m_strTableName;
}


bool KIOTableController::SelectRow(CString a_strOriginalFilter)
{
	bool bSelect = false;
	KSelectByAttributeDlg dlg(m_pIOTable);
	dlg.SetFilter(a_strOriginalFilter);
	
	if (dlg.DoModal() == IDOK)
		bSelect = true;
	
	return bSelect;
}

bool KIOTableController::QuickSearch( void )
{
	bool bSelect = false;

	KQuickSearchDlg oDlg(m_pIOTable);

	if (oDlg.DoModal() == IDOK)
	{
		CString strFilter = oDlg.GetFilter();
		m_pIOTableView->SetFilter(strFilter);
		bSelect = true;
	}

	return bSelect;
}

bool KIOTableController::CalculateField(CString a_strFilter)
{
	bool bCalc = false;
    try
    {
        KFieldCalculatorDlg dlg;
        dlg.SetTable(m_pIOTable, a_strFilter);
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
	return bCalc;
}

bool KIOTableController::AddField(void)
{
    CWaitCursor wc;
	// table 이름에서 제일 큰 숫자를 가져온다.
	int nColumnNumber = 0;
	int nDisplayOrder = 0;
	const KIOColumns* pColumns = m_pIOTable->Columns();
	int nCount = pColumns->ColumnCount();

	std::vector<CString> vecDisplayName;
	for(int i = 0; i < nCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);

		CString strCaption = pColumn->Caption();
		vecDisplayName.push_back(strCaption);

		CString strName = pColumn->Name();
		CString strNumber = _T("");
		for(int j = 0; j < strName.GetLength(); ++j)
		{
			TCHAR c = strName.GetAt(j);
			if((_T('0') <= c) && (_T('9') >= c))
			{
				strNumber.AppendChar(c);
			}
			else
			{
				strNumber = _T("");
			}
		}

		int nNumber = _ttoi(strNumber);
		if(nColumnNumber <  nNumber)
		{
			nColumnNumber = nNumber;
		}

		if(nDisplayOrder < pColumn->DisplayOrder())
		{
			nDisplayOrder = pColumn->DisplayOrder();
		}
	}

	KProject* pProject             = ((CKmzApp*)AfxGetApp())->GetProject();
	bool      bAddColumn           = false;
	CString   strTableName         = m_pIOTable->Name();
	bool      bSetDefaultValueZero = true;	//add Field시 Default 값을 '0'으로 초기화, false 시 null
	
	if (strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
	{
		if (KmzSystem::IsFreightSystem())
		{
			KFTableModeAddFieldDlg oDlg(m_pTarget, pProject, m_pIOTable, nColumnNumber + 1, nDisplayOrder + 1, KEMFreightODModeOD);
			oDlg.SetDisplayNames(vecDisplayName);

			if (oDlg.DoModal() == IDOK)
			{
				KIOColumn &column  = oDlg.m_AddColumn;
				KPurpose* pPurpose = oDlg.m_pPurpose;

				if (pPurpose == nullptr)
				{
					bAddColumn = KTableAddFieldInfo::AddModeField(m_pIOTable, column, nullptr, bSetDefaultValueZero);
				}
				else
				{
					KIOColumns oColumns = oDlg.m_oColumns;
					bAddColumn = KTableAddFieldInfo::AddModeField(m_pIOTable, oColumns, pPurpose, bSetDefaultValueZero);
				}				
			}
		}
		else
		{
			KTableModeAddFieldDlg oModeDlg(m_pTarget, pProject, m_pIOTable, nColumnNumber + 1, nDisplayOrder + 1);
			oModeDlg.SetDisplayNames(vecDisplayName);

			if(oModeDlg.DoModal() == IDOK)
			{
				KIOColumn &column = oModeDlg.m_AddColumn;
				KMode*     pMode  = oModeDlg.m_pMode;
				bAddColumn = KTableAddFieldInfo::AddModeField(m_pIOTable, column, pMode, bSetDefaultValueZero);
			}
		}		
	}
	else if(strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0)
	{
		KTablePurposeAddFieldDlg oPurposeDlg(m_pTarget, pProject, m_pIOTable, nColumnNumber + 1, nDisplayOrder + 1);
		oPurposeDlg.SetDisplayNames(vecDisplayName);

		if(oPurposeDlg.DoModal() == IDOK)
		{
			KIOColumn &column   = oPurposeDlg.m_AddColumn;
			KPurpose*  pPurpose = oPurposeDlg.m_pPurpose;
			bAddColumn = KTableAddFieldInfo::AddPurposeField(m_pIOTable, column, pPurpose, bSetDefaultValueZero);
		}
	}
	else if(strTableName.CompareNoCase(TABLE_ZONE) == 0)
	{
		KTableZoneAddFieldDlg oZoneDlg(m_pTarget, pProject, m_pIOTable, nColumnNumber + 1, nDisplayOrder + 1);
		oZoneDlg.SetDisplayNames(vecDisplayName);

		if(oZoneDlg.DoModal() == IDOK)
		{
			KIOColumn &column    = oZoneDlg.m_AddColumn;
			KPurpose*  pPurpose  = oZoneDlg.m_pPurpose;
			CString    strPAFlag = oZoneDlg.m_strPAFlag;
			bAddColumn = KTableAddFieldInfo::AddZoneField(m_pIOTable, column, pPurpose, strPAFlag, bSetDefaultValueZero);
		}
	}
	else if (strTableName.CompareNoCase(TABLE_VEHICLE_MODE_OD) == 0)
	{
		KTableVehicleConvAddFieldDlg oDlg(m_pTarget);
		oDlg.DoModal();
	}
	else if (strTableName.CompareNoCase(TABLE_CHAIN_OD) == 0 && KmzSystem::IsFreightSystem())
	{
		KFTableModeAddFieldDlg oDlg(m_pTarget, pProject, m_pIOTable, nColumnNumber + 1, nDisplayOrder + 1, KEMFreightODChainOD);
		oDlg.SetDisplayNames(vecDisplayName);

		if (oDlg.DoModal() == IDOK)
		{
			KIOColumn &column  = oDlg.m_AddColumn;
			KPurpose* pPurpose = oDlg.m_pPurpose;

			if (pPurpose == nullptr)
			{
				KIOColumn column;

				CString strName;
				strName.Format(_T("%c%d"), strTableName.GetAt(0), nColumnNumber + 1, nDisplayOrder + 1);
				column.Name(strName);
				column.Caption(oDlg.m_strColumnName);
				column.ColumnInfo(oDlg.m_strColumnInfo);
				column.DataType(oDlg.m_emDataType);

				if (oDlg.m_bCode)
				{
					column.CodeType(KEMIOCodeTypeIsSingleCode);
				}
				else
				{
					column.CodeType(KEMIOCodeTypeIsNothing);
				}

				if(oDlg.m_bCode)
				{
					column.CodeGroup(oDlg.m_nCodeGroup);
				}

				column.Visible(true);
				column.FreezeColumn(false);
				column.DisplayOrder(nDisplayOrder + 1);
				column.ColumnType(KEMIOColumnTypeUserDefine);
				column.PrimaryKey(false);

				try
				{
					m_pIOTable->BeginTransaction();
					if(m_pIOTable->AddColumn(column, bSetDefaultValueZero) != NULL)
					{
						m_pIOTable->Commit();
						bAddColumn = true;
					}
					else
					{
						m_pIOTable->Rollback();
					}
				}
				catch (KExceptionPtr ex)
				{
					TxExceptionPrint(ex);
					m_pIOTable->Rollback();
				}
				catch (...)
				{
					TxLogDebugException();
					m_pIOTable->Rollback();
				}
			}
			else
			{
				KIOColumns oColumns = oDlg.m_oColumns;
				bAddColumn = KTableAddFieldInfo::AddChainField(m_pIOTable, oColumns, pPurpose, bSetDefaultValueZero);
			}
		}
	}
	else
	{
		KTableAddFieldDlg dlg(m_pTarget);
		dlg.SetDisplayNames(vecDisplayName);

		if(dlg.DoModal() == IDOK)
		{
			KIOColumn column;

			CString strName;
			strName.Format(_T("%c%d"), strTableName.GetAt(0), nColumnNumber + 1, nDisplayOrder + 1);
			column.Name(strName);
			column.Caption(dlg.m_strColumnName);
			column.ColumnInfo(dlg.m_strColumnInfo);
			column.DataType(dlg.m_emDataType);

			if (dlg.m_bCode)
			{
				column.CodeType(KEMIOCodeTypeIsSingleCode);
			}
			else
			{
				column.CodeType(KEMIOCodeTypeIsNothing);
			}

			if(dlg.m_bCode)
			{
				column.CodeGroup(dlg.m_nCodeGroup);
			}

			column.Visible(true);
			column.FreezeColumn(false);
			column.DisplayOrder(nDisplayOrder + 1);
			column.ColumnType(KEMIOColumnTypeUserDefine);
			column.PrimaryKey(false);

            try
            {
                m_pIOTable->BeginTransaction();
                if(m_pIOTable->AddColumn(column, bSetDefaultValueZero) != NULL)
                {
                    m_pIOTable->Commit();
                    bAddColumn = true;
                }
                else
                {
                    m_pIOTable->Rollback();
                }
            }
            catch (KExceptionPtr ex)
            {
            	TxExceptionPrint(ex);
            	m_pIOTable->Rollback();
            }
            catch (...)
            {
            	TxLogDebugException();
            	m_pIOTable->Rollback();
            }			
		}
	}

	return bAddColumn;
}


bool KIOTableController::RenameField(LPCTSTR strColumnName)
{
	try
	{
		const KIOColumns* pColumns = m_pIOTable->Columns();

		int nIndex = pColumns->GetColumnIndex(strColumnName);
		if(-1 == nIndex)
		{
			return false;
		}

		KIOColumn* pRenameColumn       = pColumns->GetColumn(nIndex);
		CString    strDisplayName      = pRenameColumn->Caption();
		/*if( KEMIOColumnTypeUserDefine != pRenameColumn->ColumnType())
		{
			AfxMessageBox(_T("시스템에서 정의된 컬럼 정보는 변경 할 수 없습니다."));

			return false;
		}*/

		int nCount = pColumns->ColumnCount();
		std::vector<CString> vecOtherDisplayName;
		for(int i = 0; i < nCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			CString strName    = pColumn->Name();
			CString strCaption = pColumn->Caption();

			if (strName.CompareNoCase(strColumnName) == 0)
			{
				continue;
			}

			vecOtherDisplayName.push_back(strCaption);
		}

		KTableRenameFieldDlg dlg(strDisplayName, vecOtherDisplayName);
		if(dlg.DoModal() == IDOK)
		{
            try
            {
                m_pIOTable->BeginTransaction();

                KIOColumn oUpdateColumn = *pRenameColumn;
                oUpdateColumn.Caption(strDisplayName);
                m_pIOTable->UpdateColumn(oUpdateColumn);

                m_pIOTable->Commit();
            }
            catch (KExceptionPtr ex)
            {
            	TxExceptionPrint(ex);
                m_pIOTable->Rollback();
            	return false;
            }
            catch (...)
            {
            	TxLogDebugException();
                m_pIOTable->Rollback();
            	return false;
            }			
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KIOTableController::EditField(LPCTSTR strColumnName)
{
	try
	{
		KProject*         pProject = ImChampFrameWindow::GetProject();
		const KIOColumns* pColumns = m_pIOTable->Columns();
		int nIndex      = pColumns->GetColumnIndex(strColumnName);
		if(-1 == nIndex)
		{
			return false;
		}

		KIOColumn*        pEditColumn  = pColumns->GetColumn(nIndex);
		/*if( KEMIOColumnTypeUserDefine != pEditColumn->ColumnType())
		{
			AfxMessageBox(_T("시스템에서 정의된 컬럼 정보는 변경 할 수 없습니다."));

			return false;
		}*/

		KEMEditFieldType emEditFieldType = KTableEditFieldInfo::GetEditFieldType(m_pIOTable, *pEditColumn);
		
		if (KEMEditFieldNormal == emEditFieldType)
		{
			KTableEditFieldDlg        oDlg(m_pIOTable, pEditColumn);
			oDlg.DoModal();
		}
		else if (KEMEditFieldCode == emEditFieldType)
		{
			KTableEditCodeFieldDlg    oDlg(m_pIOTable, pEditColumn, m_pTarget);
			oDlg.DoModal();
		}
		else if (KEMEditFieldPurpose == emEditFieldType)
		{
			KTablePurposeEditFieldDlg oDlg(m_pIOTable, pEditColumn, m_pTarget, pProject);
			oDlg.DoModal();
		}
		else if (KEMEditFieldMode == emEditFieldType)
		{
			KTableModeEditFieldDlg    oDlg(m_pIOTable, pEditColumn, m_pTarget, pProject);
			oDlg.DoModal();
		}
		else if (KEMEditFieldZone == emEditFieldType)
		{
			KTableZoneEditFieldDlg    oDlg(m_pIOTable, pEditColumn, m_pTarget, pProject);
			oDlg.DoModal();
		}
		else if (KEMEditFieldFVehicle == emEditFieldType)
		{
			KTableEditVehicleFieldDlg oDlg(m_pTarget, pEditColumn);
			oDlg.DoModal();
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KIOTableController::ImportTable(void)
{
	KIOTable* pIOTable = nullptr; {
		KIOTables* pIOTables = m_pTarget->Tables();
		if (pIOTables == nullptr) {
			AfxMessageBox(_T("관련 테이블 정보를 찾을 수 없습니다."));
			return false;
		}

		pIOTable = pIOTables->FindTable(m_strTableName);
	}

	if (nullptr == pIOTable) {
		AfxMessageBox(_T("관련 테이블 정보를 찾을 수 없습니다."));
		return false;
	}

	if (m_strTableName.CompareNoCase(TABLE_NODE) == 0) {
		KDlgImportNode oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_LINK) == 0) {
		KDlgImportLink oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_ZONE) == 0) {
		KDlgImportZone oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_ACCESS_STOP_NODE) == 0) {
		KDlgImportAccessStopNode oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_TERMINAL_SCHEDULE) == 0) {
		KDlgImportTerminalSchedule oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_TRANSIT) == 0) {
		KDlgImportTransit oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_TRANSITLINKS) == 0) { //★ 불필요
		KDlgImportTransitLine oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_TRANSITNODES) == 0) {
		KDlgImportTransitLineNode oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_TURN) == 0) {
		KDlgImportTurn oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0) {
		KDlgImportPod oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_MODE_OD) == 0) {
		KDlgImportMode oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_PARAMETER_OD) == 0) {
		KDlgImportParaOD oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_SED_VECTOR) == 0) {
		KDlgImportSedVector oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	} else if (m_strTableName.CompareNoCase(TABLE_SED_POP) == 0) {
		KDlgImportSedPop oDlgImport(m_pTarget, pIOTable);
		if (oDlgImport.DoModal() == IDCANCEL) {
			return false;
		}
	}

	// 결과 실패일 경우 DoModal은 OK 가 되지 않기때문에, Cancel을 클릭 할 수 밖에 없다. 즉, return false; 가 실행 될 수 밖에 없다.
	return true;
}

bool KIOTableController::ExportTable(CString a_strFilter)
{
	bool bExport = false;

	CString strTableName = m_pIOTable->Name();

	if (strTableName.CompareNoCase(TABLE_TURN) == 0) {
		KDlgExportTurnTable oDlg(m_pTarget, m_pIOTable);
		oDlg.SetFilter(a_strFilter);

		if( oDlg.DoModal() == IDOK ) {
			bExport = true;
		}
	} else {
		KDlgExportTable oDlg(m_pTarget, m_pIOTable);
		oDlg.SetFilter(a_strFilter);

		if( oDlg.DoModal() == IDOK ) {
			bExport = true;
		}
	}
    
	return bExport;
}

bool KIOTableController::DeleteColumn(LPCTSTR strColumnName)
{
    try
    {
        CWaitCursor wc;

		CString strTableName = m_pIOTable->Name();

        KIOColumns* pColumns = m_pIOTable->Columns();
        KIOColumn*  pColumn  = pColumns->GetColumn(strColumnName);
		
		if (pColumn == nullptr)
        {
            AfxMessageBox(_T("관련 컬럼 정보를 찾을 수 없습니다."));
            return false;
        }

        if( KEMIOColumnTypeUserDefine != pColumn->ColumnType())
        {
            AfxMessageBox(_T("시스템에서 정의된 컬럼은 삭제 할 수 없습니다."));
            return false;
        }

		// 2019.04.10 수정 
		// 존테이블의 경우 컬럼 삭제시 해당 컬럼이 존특성변수로 설정된 컬럼인지 확인후 처리
		// 삭제시에는 해당 존 특성변수를 설정 해제 함
		CString strpColumnName = pColumn->Name();
		CString strColumnCaption = pColumn->Caption();

		bool bZoneVarCol = false;
		std::vector<int> arrZoneVarId;

		if (strTableName.CompareNoCase(TABLE_ZONE) == 0)
		{
			std::map<int, TZoneVariableInfo> mapZoneVar;
			KDBaseDefaultParaInterUrbanModeChoice::ReadZoneVariableInfo(mapZoneVar);

			for (auto& aZoneVar : mapZoneVar)
			{
				if (aZoneVar.second.strZoneColDispName == strColumnCaption)
				{
					arrZoneVarId.push_back(aZoneVar.second.nId);
					bZoneVarCol = true;
				}
			}
		}

		if (bZoneVarCol && (AfxMessageBox(_T("해당 컬럼은 존특성변수로 설정되어 있습니다.\r\n그래도 삭제하시겠습니까?"), MB_YESNO) == IDNO))
		{
			return false;
		}

        m_pIOTable->BeginTransaction();
        m_pIOTable->RemoveColumn(strColumnName);
        m_pIOTable->Commit();

		if (bZoneVarCol && !arrZoneVarId.empty())
		{
			KDBaseConPtr spDBConn = KDBase::GetProjectDBConnection();
			try
			{
				spDBConn->BeginTransaction();
				if (KDBaseDefaultParaInterUrbanModeChoice::UpdateZoneVariableInfoUnselected(spDBConn, arrZoneVarId)
					&& KDBaseDefaultParaInterUrbanModeChoice::DeleteUrbanModeChoicePara(spDBConn, arrZoneVarId))
				{
					spDBConn->Commit();
				}
				else
				{
					spDBConn->RollBack();
				}
			}
			catch (...)
			{
				spDBConn->RollBack();
			}
		}
		
		if (strTableName.CompareNoCase(TABLE_VEHICLE_MODE_OD) == 0)
		{
			KTableVehicleDeleteField::DeleteVehicleFieldLogic(m_pTarget, strpColumnName);
		}

		if (strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
		{
			KFTableODDeleteField::DeleteModeODFieldLogic(m_pTarget, strpColumnName);
		}

		if (strTableName.CompareNoCase(TABLE_CHAIN_OD) == 0)
		{
			KFTableODDeleteField::DeleteChainODFieldLogic(m_pTarget, strpColumnName);
		}
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        m_pIOTable->Rollback();
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        m_pIOTable->Rollback();
        return false;
    }

	try
	{
		KInitRelationProcess::DelGroupGeneration(m_pTarget, m_pIOTable->Name(), strColumnName);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
	
	return true;
}


void KIOTableController::ZoomTo(const KIORow* pRow)
{
    try
    {
        KMapView* pMapView = ShowMapView();
        if (pMapView == nullptr)
            return;

        CString     strTableName = m_pIOTable->Name();
        KIOColumns* pColumns     = m_pIOTable->Columns(); 
    
        int     nLayerID(-1);
        int     nIdx(-1);
        __int64 nxTxID(-1);

        if (strTableName.CompareNoCase(TABLE_NODE) == 0) {
            nLayerID = KLayerID::Node();
            nIdx     = pColumns->GetColumnIndex(COLUMN_NODE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }        
        }
        else if (strTableName.CompareNoCase(TABLE_LINK) == 0) {
            nLayerID = KLayerID::Link();
            nIdx     = pColumns->GetColumnIndex(COLUMN_LINK_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_ZONE) == 0) {
            nLayerID = KLayerID::Zone();
            nIdx     = pColumns->GetColumnIndex(COLUMN_ZONE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_TURN) == 0) {
            nLayerID = KLayerID::Node();
            nIdx     = pColumns->GetColumnIndex(COLUMN_TURN_NODE);
            nxTxID   = pRow->GetItem(nIdx)->GetValueAsInteger64();
        }
        else if (strTableName.CompareNoCase(TABLE_INTERSECTION) == 0) {
            nLayerID = KLayerID::Node();
            nIdx     = pColumns->GetColumnIndex(COLUMN_NODE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_TRANSITLINKS) == 0) { //★ 불필요
            nLayerID = KLayerID::Link();
            nIdx = pColumns->GetColumnIndex(COLUMN_TRANSIT_LINK_ID);
            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }
        }
        else if (strTableName.CompareNoCase(TABLE_TRANSITNODES) == 0) { 
            nLayerID = KLayerID::Node();
            nIdx = pColumns->GetColumnIndex(COLUMN_TRANSIT_NODE_ID);
            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }
        }
        else if (strTableName.CompareNoCase(TABLE_SED_POP) == 0) {
            nLayerID = KLayerID::Zone();
            nIdx     = pColumns->GetColumnIndex(COLUMN_ZONE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_SED_VECTOR) == 0) {
            nLayerID = KLayerID::Zone();
            nIdx     = pColumns->GetColumnIndex(COLUMN_ZONE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_ACCESS_STOP_NODE) == 0) {
            nLayerID = KLayerID::Node();
            nIdx     = pColumns->GetColumnIndex(COLUMN_NODE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }     
        }
        else {
            if (m_pIOTable->TableType() == KEMIOTableTypeODData)
            {
                nLayerID = KLayerID::Zone();
                nIdx     = pColumns->GetColumnIndex(COLUMN_OZONE);

                KIOItem* pItem = pRow->GetItem(nIdx);
                if (pItem != nullptr) {
                    nxTxID = pItem->GetValueAsInteger64();
                }     
            }
            else {
                AfxMessageBox(_T("Layer 정보를 찾을 수 없습니다."));
                return;
            }
        }

        if (nxTxID < 0) {
            AfxMessageBox(_T("위치 정보를 찾을 수 없습니다."));
            return;
        }

        pMapView->ZoomToFeature(nLayerID, nxTxID);
    }
    catch(...) {}
}

void KIOTableController::ZoomNode( Integer a_nxNodeID )
{
	KMapView* pMapView = ShowMapView();
	if (pMapView != nullptr) {
		pMapView->ZoomToFeature(KLayerID::Node(), a_nxNodeID);
	}
}

void KIOTableController::ExtentToNodes(std::vector<Integer> a_vecNodes)
{
	KMapView* pMapView = ShowMapView();

	if (pMapView != nullptr)
	{
		if (_tcsicmp(m_pIOTable->Name(), TABLE_TERMINAL_SCHEDULE) == 0)
		{
			pMapView->ExtentToFeautre(m_pIOTable->Name(), a_vecNodes);
		}
	}
}

void KIOTableController::PanTo(const KIORow *pRow)
{
    try
    {
        KMapView* pMapView = ShowMapView();
        if (pMapView == nullptr)
            return;

        CString     strTableName = m_pIOTable->Name();
        KIOColumns* pColumns     = m_pIOTable->Columns(); 

        int     nLayerID(-1);
        int     nIdx(-1);
        __int64 nxTxID(-1);

        if (strTableName.CompareNoCase(TABLE_NODE) == 0) {
            nLayerID = KLayerID::Node();
            nIdx     = pColumns->GetColumnIndex(COLUMN_NODE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }        
        }
        else if (strTableName.CompareNoCase(TABLE_LINK) == 0) {
            nLayerID = KLayerID::Link();
            nIdx     = pColumns->GetColumnIndex(COLUMN_LINK_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_ZONE) == 0) {
            nLayerID = KLayerID::Zone();
            nIdx     = pColumns->GetColumnIndex(COLUMN_ZONE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_TURN) == 0) {
            nLayerID = KLayerID::Node();
            nIdx     = pColumns->GetColumnIndex(COLUMN_TURN_NODE);
            nxTxID   = pRow->GetItem(nIdx)->GetValueAsInteger64();
        }
        else if (strTableName.CompareNoCase(TABLE_INTERSECTION) == 0) {
            nLayerID = KLayerID::Node();
            nIdx     = pColumns->GetColumnIndex(COLUMN_NODE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_TRANSITLINKS) == 0) { //★ 불필요
            nLayerID = KLayerID::Link();
            nIdx = pColumns->GetColumnIndex(COLUMN_TRANSIT_LINK_ID);
            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }
        }
        else if (strTableName.CompareNoCase(TABLE_TRANSITNODES) == 0) { //★ 불필요
            nLayerID = KLayerID::Node();
            nIdx = pColumns->GetColumnIndex(COLUMN_TRANSIT_NODE_ID);
            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }
        }
        else if (strTableName.CompareNoCase(TABLE_SED_POP) == 0) {
            nLayerID = KLayerID::Zone();
            nIdx     = pColumns->GetColumnIndex(COLUMN_ZONE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_SED_VECTOR) == 0) {
            nLayerID = KLayerID::Zone();
            nIdx     = pColumns->GetColumnIndex(COLUMN_ZONE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }   
        }
        else if (strTableName.CompareNoCase(TABLE_ACCESS_STOP_NODE) == 0) {
            nLayerID = KLayerID::Node();
            nIdx     = pColumns->GetColumnIndex(COLUMN_NODE_ID);

            KIOItem* pItem = pRow->GetItem(nIdx);
            if (pItem != nullptr) {
                nxTxID = pItem->GetValueAsInteger64();
            }     
        }
        else {
            if (m_pIOTable->TableType() == KEMIOTableTypeODData)
            {
                nLayerID = KLayerID::Zone();
                nIdx     = pColumns->GetColumnIndex(COLUMN_OZONE);

                KIOItem* pItem = pRow->GetItem(nIdx);
                if (pItem != nullptr) {
                    nxTxID = pItem->GetValueAsInteger64();
                }     
            }
            else {
                AfxMessageBox(_T("Layer 정보를 찾을 수 없습니다."));
                return;
            }
        }

        if (nxTxID < 0) {
            AfxMessageBox(_T("위치 정보를 찾을 수 없습니다."));
            return;
        }

        pMapView->PanToFeature(nLayerID, nxTxID);
    }
    catch(...) {}   
}

void KIOTableController::SelectHighwayShortestPathStartNode(const KIORow* pRow)
{
    KMapView* pMapView = ImChampFrameWindow::GetActiveMapView();
    //pMapView->
}

void KIOTableController::SelectHighwayShortestPathEndNode(const KIORow* pRow)
{

}

void KIOTableController::ViewClosed(void)
{
	//delete this;
    m_pTarget->UnregisterController(m_strTableName);
}

void KIOTableController::ViewCalculateZoneDistance( void )
{
	KCalculateZoneDistanceDlg oCalculateZoneDistDlg;
	oCalculateZoneDistDlg.SetInfo(m_pTarget, m_pIOTable);
	oCalculateZoneDistDlg.DoModal();
}


void KIOTableController::ShowProperties( const KIORow* pRow )
{
	const KIOColumns* pIOColumns = m_pIOTable->Columns();
	int nColumnCount = pIOColumns->ColumnCount();

	CString strWhere;
	CString strTemp;
	for (int i=0; i<nColumnCount; i++)
	{
		KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
		if (pIOColumn->PrimaryKey())
		{
			strTemp.Format(_T(" %s = '%s' "), pIOColumn->Name(), pRow->GetItem(i)->GetValueAsString());

			if (strWhere.GetLength() > 0)
			{
				strWhere = strWhere + _T(" and ") + strTemp;
			}
			else
			{
				strWhere = strTemp;
			}
		}
	}

	if (strWhere.GetLength() == 0)
		return;

	if (m_spPropertiesDlg != NULL)
	{			
		m_spPropertiesDlg->ShowWindow(SW_SHOW);		
	}
	else
	{
		m_spPropertiesDlg = KPropertiesDlgPtr(new KPropertiesDlg());
		m_spPropertiesDlg->Create(IDD_2200_PROPERTIES);
		m_spPropertiesDlg->ShowWindow(SW_SHOW);
	}		

    m_spPropertiesDlg->ShowInformation(m_pTarget, m_strTableName, strWhere);

    if (_tcsicmp(m_strTableName, TABLE_LINK) == 0)
    {
        int nIndex = pIOColumns->GetColumnIndex(COLUMN_LINK_ID);
        if (nIndex < 0)
            return;

        KIOItem* pItem = pRow->GetItem(nIndex);
        if (pItem == nullptr)
            return;

        Integer nxLinkID = pItem->GetValueAsInteger64();
        m_spPropertiesDlg->SetLinkID(nxLinkID);
    }
}

void KIOTableController::AddRow()
{
    if (m_spPropertiesDlg != NULL)
    {			
        m_spPropertiesDlg->ShowWindow(SW_SHOW);		
    }
    else
    {
        m_spPropertiesDlg = KPropertiesDlgPtr(new KPropertiesDlg());
        m_spPropertiesDlg->Create(IDD_2200_PROPERTIES);
        m_spPropertiesDlg->ShowWindow(SW_SHOW);
    }		

    m_spPropertiesDlg->ShowAddRowData(m_pTarget, m_strTableName);
}

void KIOTableController::DeleteRowData(const KIORow* pIORow)
{   
    try
    {
        KIOColumns* pIOColumns = m_pIOTable->Columns();
        int nColumnCount = pIOColumns->ColumnCount();

        CString strWhere;
        CString strTemp;
        for (int i=0; i<nColumnCount; i++)
        {
            KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
            if (pIOColumn->PrimaryKey())
            {
                strTemp.Format(_T(" %s = '%s' "), pIOColumn->Name(), pIORow->GetItem(i)->GetValueAsString());

                if (strWhere.GetLength() > 0)
                {
                    strWhere = strWhere + _T(" and ") + strTemp;
                }
                else
                {
                    strWhere = strTemp;
                }
            }
        }

        if (strWhere.GetLength() == 0)
            return;

        try
        {
            m_pIOTable->BeginTransaction();
            m_pIOTable->DeleteRow(strWhere);
            m_pIOTable->Commit();	
        }
        catch (KExceptionPtr ex)
        {
        	m_pIOTable->Rollback();
            TxExceptionPrint(ex);            
            throw ex;
        }
        catch (...)
        {
        	m_pIOTable->Rollback();
            TxLogDebugException();            
            ThrowException(_T("At DeleteRowData")); 
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
    	ThrowException(_T("At DeleteRowData")); 
    }
}

void KIOTableController::ShowAccessLineStationModify(Integer &a_nxZoneID, Integer &a_nxLineID)
{
	try
	{
		if( nullptr != m_spEditAccessLineStationDlg)
		{
			m_spEditAccessLineStationDlg->ShowWindow(SW_SHOW);
			m_spEditAccessLineStationDlg->ShowInformation(a_nxZoneID, a_nxLineID);
		}
		else
		{
			m_spEditAccessLineStationDlg = KEditAccessLineStationDlgPtr(new KEditAccessLineStationDlg(m_pTarget));
			m_spEditAccessLineStationDlg->Create(IDD_2300_EditAccessLineStationDlg);
			m_spEditAccessLineStationDlg->ShowWindow(SW_SHOW);

			m_spEditAccessLineStationDlg->ShowInformation(a_nxZoneID, a_nxLineID);
		}
	}
	catch (...)
	{
		return;
	}
}


void KIOTableController::ShowIntersectionModify(TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo)
{
	try
	{
		KMapView* pMapView = ImChampFrameWindow::GetActiveMapView();

		m_spIntersectionEditMainDlg = KIntersectionEditMainDlgPtr(new KIntersectionEditMainDlg(m_pTarget, pMapView, a_oTSelectIntersection, a_mapBoundInfo, m_pIOTable));
		m_spIntersectionEditMainDlg->Create(KIntersectionEditMainDlg::IDD);
		m_spIntersectionEditMainDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		return;
	}
}


void KIOTableController::DeleteAccessLineStation(Integer &a_nxZoneID, Integer &a_nxLineID)
{
	try
	{
		KEditAccessLineStation oEditAccessLineStation(m_pTarget, a_nxZoneID, a_nxLineID);
		oEditAccessLineStation.DeleteAccessLineStation();
	}
	catch (...)
	{
		return;
	}
}


void KIOTableController::ActivatedFrameWndNotifyProcess()
{
	if (nullptr != m_spPropertiesDlg)
	{			
		if (!m_spPropertiesDlg->IsUserClosed())
		{
			if (!m_spPropertiesDlg->IsWindowVisible())
				m_spPropertiesDlg->ShowWindow(SW_SHOW);
		}
	}

    if (nullptr != m_spTransitPathViewDlg)
    {			
        if (!m_spTransitPathViewDlg->IsUserClosed())
        {
            if (!m_spTransitPathViewDlg->IsWindowVisible())
                m_spTransitPathViewDlg->ShowWindow(SW_SHOW);
        }
    }

	if (nullptr != m_spEditAccessLineStationDlg)
	{
		if(!m_spEditAccessLineStationDlg->IsUserClosed())
		{
			if(!m_spEditAccessLineStationDlg->IsWindowVisible())
				m_spEditAccessLineStationDlg->ShowWindow(SW_SHOW);
		}
	}

	if (nullptr != m_spChartTransitDlg)
	{
		if(!m_spChartTransitDlg->IsUserClosed())
		{
			if(!m_spChartTransitDlg->IsWindowVisible())
				m_spChartTransitDlg->ShowWindow(SW_SHOW);
		}
	}

	if (nullptr != m_spIntersectionEditMainDlg)
	{
		if(!m_spIntersectionEditMainDlg->IsUserClosed())
		{
			if(!m_spIntersectionEditMainDlg->IsWindowVisible())
				m_spIntersectionEditMainDlg->ShowWindow(SW_SHOW);
		}
	}

    if (nullptr != m_spTransitStyleDlg)
    {
        if (!m_spTransitStyleDlg->IsUserClosed())
        {
            if (!m_spTransitStyleDlg->IsWindowVisible())
                m_spTransitStyleDlg->ShowWindow(SW_SHOW);
        }
    }
}

void KIOTableController::DeActivatedFrameWndNotifyProcess()
{
	if (nullptr != m_spPropertiesDlg)
	{			
		if (!m_spPropertiesDlg->IsUserClosed())
		{
			if (m_spPropertiesDlg->IsWindowVisible())
				m_spPropertiesDlg->ShowWindow(SW_HIDE);
		}	
	}

    if (nullptr != m_spTransitPathViewDlg)
    {			
        if (!m_spTransitPathViewDlg->IsUserClosed())
        {
            if (m_spTransitPathViewDlg->IsWindowVisible())
                m_spTransitPathViewDlg->ShowWindow(SW_HIDE);
        }	
    }

	if (nullptr != m_spEditAccessLineStationDlg)
	{			
		if (!m_spEditAccessLineStationDlg->IsUserClosed())
		{
			if (m_spEditAccessLineStationDlg->IsWindowVisible())
				m_spEditAccessLineStationDlg->ShowWindow(SW_HIDE);
		}	
	}

	if (nullptr != m_spChartTransitDlg)
	{			
		if (!m_spChartTransitDlg->IsUserClosed())
		{
			if (m_spChartTransitDlg->IsWindowVisible())
				m_spChartTransitDlg->ShowWindow(SW_HIDE);
		}	
	}

	if (nullptr != m_spIntersectionEditMainDlg)
	{			
		if (!m_spIntersectionEditMainDlg->IsUserClosed())
		{
			if (m_spIntersectionEditMainDlg->IsWindowVisible())
				m_spIntersectionEditMainDlg->ShowWindow(SW_HIDE);
		}	
	}

    if (nullptr != m_spTransitStyleDlg)
    {
        if (!m_spTransitStyleDlg->IsUserClosed())
        {
            if (m_spTransitStyleDlg->IsWindowVisible())
                m_spTransitStyleDlg->ShowWindow(SW_HIDE);
        }
    }
}


void KIOTableController::ShowTransitLineInfo( Integer a_nxTransitID )
{
	try
	{
        KMapView* pMapView = ShowMapView();
        if(pMapView == nullptr)
        {
            AfxMessageBox(TransitMessage::MapNotFound);
            return;
        }

        if( nullptr == m_spTransitPathViewDlg)
        {
            m_spTransitPathViewDlg = KTransitPathViewDlgPtr(new KTransitPathViewDlg(m_pTarget, pMapView));
            m_spTransitPathViewDlg->Create(KTransitPathViewDlg::IDD);
        }
        
        pMapView->ZoomTransitExtent(a_nxTransitID);

        m_spTransitPathViewDlg->SetViewTransit(a_nxTransitID);
        m_spTransitPathViewDlg->ShowWindow(SW_SHOW);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}
	

void KIOTableController::ShowTransitChart( Integer a_xTransitID )
{
	try
	{
		if( nullptr != m_spChartTransitDlg)
		{
			m_spChartTransitDlg.reset();
		}
		m_spChartTransitDlg = KPrepareChartTransitDlgPtr(new KPrepareChartTransitDlg(m_pTarget, a_xTransitID));
		m_spChartTransitDlg->Create(KPrepareChartTransitDlg::IDD);
		m_spChartTransitDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		return;
	}
}

void KIOTableController::TransitLinkViaTransit()
{
    try
    {
        ShowMapView(true);
        ImChampFrameWindow::GetMapView(m_pTarget)->SearchTransitByLink();
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

void KIOTableController::TransitNodeViaTransit()
{
    try
    {
        ShowMapView(true);
        ImChampFrameWindow::GetMapView(m_pTarget)->SearchTransitByNode();
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

void KIOTableController::TransitLinkStyle()
{
    try
    {
        ShowMapView(true);

        KIOTransitTableView* pTransitView = (KIOTransitTableView*)m_pIOTableView;
        if (NULL == pTransitView) {
            return;
        }
        std::set<Integer> setSelectedTransit = pTransitView->GetSelectedTransit();

        bool bModal(false);
        if (bModal) {
            KDlgTransitStyle oDlg(m_pTarget, pTransitView);
            oDlg.InitModalID(setSelectedTransit);
            oDlg.DoModal();
        }
        else {
            m_spTransitStyleDlg = std::shared_ptr<KDlgTransitStyle>(new KDlgTransitStyle(m_pTarget, pTransitView));
            m_spTransitStyleDlg->Create(KDlgTransitStyle::IDD);
            m_spTransitStyleDlg->ShowWindow(SW_SHOW);
            m_spTransitStyleDlg->InitID(setSelectedTransit);
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

bool KIOTableController::IsUserDefineColumn( LPCTSTR strColumnName )
{
	try
	{
		KIOColumns* pColumns = m_pIOTable->Columns();
		KIOColumn*  pColumn  = pColumns->GetColumn(strColumnName);
		if (pColumn == nullptr)
		{
			return false;
		}

		if( KEMIOColumnTypeUserDefine != pColumn->ColumnType())
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

void KIOTableController::TableResetData()
{
    try
    {
        if (_tcsicmp(m_strTableName, TABLE_SED_VECTOR) == 0)
        {            
        }
        else if (m_pIOTable->TableType() == KEMIOTableTypeODData)
        {
        }
        else
        {
            AfxMessageBox(_T("현재 기능을 지원하지 않는 테이블입니다."));
            return;
        }

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
        QBicSimpleProgressThread::ExecuteThread(TableResetDataProcessThreadCaller, this, false, nLang);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        throw 1;
    }
}

unsigned __stdcall KIOTableController::TableResetDataProcessThreadCaller( void* p )
{   
    try
    {
        QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
        QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
        if (KmzSystem::GetLanguage() == KEMKorea) {
			pProgressWindow->SetStatus(_T("테이블 초기화 중..."));
		}
		else {
			pProgressWindow->SetStatus(_T("Table Reset..."));
		}

        KIOTableController* pIOTableController = (KIOTableController*)pParameter->GetParameter();
        pIOTableController->TableResetDataProcess();  
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        ThrowException(_T("At TableResetData")); 
    }

    return 1;
}

void KIOTableController::TableResetDataProcess()
{
    if (_tcsicmp(m_strTableName, TABLE_SED_VECTOR) == 0)
    {
        KBulkDBaseUtil::GenerateBlankZoneTypeTable(m_pTarget, m_strTableName);
    }
    else if (m_pIOTable->TableType() == KEMIOTableTypeODData)
    {
        KBulkDBaseUtil::GenerateBlankODTypeTable(m_pTarget, m_strTableName);
    }
    else
    {
        return;
    }
}


