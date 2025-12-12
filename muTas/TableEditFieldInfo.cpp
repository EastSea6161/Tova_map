#include "StdAfx.h"
#include "TableEditFieldInfo.h"
#include "Target.h"
#include "model_common.h"
#include "KmzSystem.h"

KTableEditFieldInfo::KTableEditFieldInfo(void)
{
}


KTableEditFieldInfo::~KTableEditFieldInfo(void)
{
}


void KTableEditFieldInfo::InitGroupCode(KTarget* a_pTarget, std::map<int, CString> &a_mapGroupCode)
{
	CString strSQL(_T(""));
	strSQL.Append(_T(" SELECT KCode_Group_Key,	KCode_Group_Name "));
	strSQL.Append(_T(" FROM KCode_Group "));
	strSQL.Append(_T(" ORDER BY KCode_Group_Name "));

	try
	{
		a_mapGroupCode.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			int     nGroupKey = spResultSet->GetValueInt(0);
			CString strName   = spResultSet->GetValueString(1);

			a_mapGroupCode.insert(std::make_pair(nGroupKey, strName));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		a_mapGroupCode.clear();
	}
	catch (...)
	{
		TxLogDebugException();
		a_mapGroupCode.clear();
	}  
}


void KTableEditFieldInfo::DefaultCodeFiledData(KIOTable* a_pIOTable, KIOColumn &a_oIOColumnUpdate)
{
	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" Update %s set %s = '0' "), a_pIOTable->Name(), a_oIOColumnUpdate.Name());

		KDBaseConPtr spDBaseConnection = a_pIOTable->GetDBaseConnection();
		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


KEMEditFieldType KTableEditFieldInfo::GetEditFieldType( KIOTable* a_pIOTable, KIOColumn a_oIOColumnUpdate )
{
	KEMEditFieldType  emEditFieldType = KEMEditFieldNormal;

	try
	{
		KEMIOColumnType   emIOColumnType  = a_oIOColumnUpdate.ColumnType();
		KEMIOCodeType     emIOCodeType    = a_oIOColumnUpdate.CodeType() ;
		CString           strTableName    = a_pIOTable->Name();

		if (KEMIOColumnTypeUserDefine != emIOColumnType) // 시스템
		{
			emEditFieldType = KEMEditFieldNormal;
		}
		else // UserDefine
		{
			if (KEMIOCodeTypeIsNothing == emIOCodeType)
			{
				emEditFieldType = KEMEditFieldNormal;

				if (strTableName.CompareNoCase(TABLE_ZONE) == 0)
				{
					if (IsRegisterFieldGenerationModel(a_pIOTable->GetDBaseConnection(), a_oIOColumnUpdate))
						emEditFieldType = KEMEditFieldZone;
					else
						emEditFieldType = KEMEditFieldNormal;
				}
				else if (strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0)
				{
					if (IsRegisterFieldDistiributionModel(a_pIOTable->GetDBaseConnection(), a_oIOColumnUpdate))
						emEditFieldType = KEMEditFieldPurpose;
					else
						emEditFieldType = KEMEditFieldNormal;
				}
				else if (strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
				{
					if (IsRegisterFieldModeChoiceModel(a_pIOTable->GetDBaseConnection(), a_oIOColumnUpdate))
						emEditFieldType = KEMEditFieldMode;
					else
						emEditFieldType = KEMEditFieldNormal;
				}
				else if (strTableName.CompareNoCase(TABLE_VEHICLE_MODE_OD) == 0)
				{
					if (IsRegisterFieldVehicleConverModel(a_pIOTable->GetDBaseConnection(), a_oIOColumnUpdate))
					{
						emEditFieldType = KEMEditFieldFVehicle;
					}
					else
					{
						emEditFieldType = KEMEditFieldNormal;
					}
				}
			}
			else // 코드필드
			{
				emEditFieldType = KEMEditFieldCode; 
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return emEditFieldType;
}


bool KTableEditFieldInfo::IsRegisterFieldDistiributionModel(KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn, int &a_nObjectID)
{
	try
	{
		CString         strSQL(_T(""));
		KEMPurposeGroup emPurposeGroup;

		if (KmzSystem::IsPassengerSystem())
		{
			emPurposeGroup = KEMPurposeGroupODBase;
		}
		else
		{
			emPurposeGroup = KEMPurposeGroupFreightOD;
		}

		strSQL.Format(_T(" SELECT Detail_Object_ID FROM %s where Object_Group_Code = %d and Purpose_OD_Column_Name = '%s' "), 
			TABLE_DISTRIBUTION_MODEL, (int)emPurposeGroup, a_oIOColumn.Name());

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResult->Next())
		{
			a_nObjectID = spResult->GetValueInt(0);
			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	a_nObjectID = -1;
	return false;
}


bool KTableEditFieldInfo::IsRegisterFieldDistiributionModel(KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn)
{
	int nEmpty(0);
	return IsRegisterFieldDistiributionModel(spDBaseConnection, a_oIOColumn, nEmpty);
}


bool KTableEditFieldInfo::IsRegisterFieldModeChoiceModel(KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn, int &a_nModeID)
{
	try
	{
		CString      strSQL(_T(""));
		KEMModeGroup emModeGroup;

		if (KmzSystem::IsPassengerSystem())
		{
			emModeGroup = KEMModeGroupODBase;
		}
		else
		{
			emModeGroup = KEMModeGroupFreightOD;
		}

		strSQL.Format(_T(" SELECT Detail_Mode_ID FROM %s WHERE Mode_Group_Code = %d AND Mode_OD_Column_Name = '%s' "), 
			TABLE_MODECHOICE_MODEL, (int)emModeGroup, a_oIOColumn.Name());

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResult->Next())
		{
			a_nModeID = spResult->GetValueInt(0);
			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	a_nModeID = -1;
	return false;
}


bool KTableEditFieldInfo::IsRegisterFieldModeChoiceModel(KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn)
{
	int nEmpty(0);
	return IsRegisterFieldModeChoiceModel(spDBaseConnection, a_oIOColumn, nEmpty);
}


bool KTableEditFieldInfo::IsRegisterFieldGenerationModel( KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn, int &a_nObjectID, CString &a_strPAFlag )
{
	try
	{
		CString         strSQL(_T(""));
		KEMPurposeGroup emPurposeGroup;

		if (KmzSystem::IsPassengerSystem())
		{
			emPurposeGroup = KEMPurposeGroupODBase;
		}
		else
		{
			emPurposeGroup = KEMPurposeGroupFreightOD;
		}

		strSQL.Format(_T(" SELECT Detail_Object_ID, PA_Flag FROM %s where Object_Group_Code = %d and Zone_Column_Name = '%s' "), 
			TABLE_GENERATION_MODEL, (int)emPurposeGroup, a_oIOColumn.Name());

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResult->Next())
		{
			a_nObjectID = spResult->GetValueInt   (0);
			a_strPAFlag = spResult->GetValueString(1);
			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	a_nObjectID = -1;
	a_strPAFlag = _T("");
	return false;
}


bool KTableEditFieldInfo::IsRegisterFieldGenerationModel( KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn )
{
	int     nEmpty(0);
	CString strEmpty(_T(""));
	return IsRegisterFieldGenerationModel(spDBaseConnection, a_oIOColumn, nEmpty, strEmpty);
}


bool KTableEditFieldInfo::UpdateDistributionModelObjectID(KDBaseConPtr spDBaseConnection, TDistributionModelKey oTDistributionModelKey, int a_nNewObjectID)
{
	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" UPDATE %s SET Detail_Object_ID = %d WHERE Object_Group_Code = %d and Detail_Object_ID = %d and Purpose_OD_Column_Name = '%s' "), 
			TABLE_DISTRIBUTION_MODEL, a_nNewObjectID, 
			oTDistributionModelKey.nObject_Group_Code, oTDistributionModelKey.nDetail_Object_ID, oTDistributionModelKey.strPurpose_OD_Column_Name);

		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


bool KTableEditFieldInfo::UpdateModeChoiceModelModeID( KDBaseConPtr spDBaseConnection, TModeChoiceModelKey oTModeChoiceModelKey, int a_nNewModeID )
{
	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" UPDATE %s SET Detail_Mode_ID = %d WHERE Mode_Group_Code = %d and Detail_Mode_ID = %d and Mode_OD_Column_Name = '%s' "),
			TABLE_MODECHOICE_MODEL, a_nNewModeID,
			oTModeChoiceModelKey.nMode_Group_Code, oTModeChoiceModelKey.nDetail_Mode_ID, oTModeChoiceModelKey.strMode_OD_Column_Name);

		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KTableEditFieldInfo::UpdateGenerationModelObjectID( KDBaseConPtr spDBaseConnection, TGenerationModelKey oTGenerationModelKey, int a_nNewObjectID, CString a_strNewPAFlag )
{
	try
	{
		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" UPDATE %s SET "), TABLE_GENERATION_MODEL);
		strSQL.AppendFormat(_T(" Detail_Object_ID = %d, PA_Flag = '%s' "), a_nNewObjectID, a_strNewPAFlag);
		strSQL.AppendFormat(_T(" WHERE Object_Group_Code = %d AND Detail_Object_ID = %d AND Zone_Column_Name = '%s' AND PA_Flag = '%s' "),
			oTGenerationModelKey.nObject_Group_Code, oTGenerationModelKey.nDetail_Object_ID, oTGenerationModelKey.strZone_Column_Name, oTGenerationModelKey.strPA_Flag);

		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KTableEditFieldInfo::IsRegisterFieldVehicleConverModel( KDBaseConPtr spDbConeeciton, KIOColumn & a_oIOColumn )
{
	int nModeID(0);
	return IsRegisterFieldVehicleConverModel(spDbConeeciton, a_oIOColumn, nModeID);
}

bool KTableEditFieldInfo::IsRegisterFieldVehicleConverModel( KDBaseConPtr spDbConeeciton, KIOColumn & a_oIOColumn, int& a_nModeID )
{
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Select fmode_id From %s where column_name = '%s' "), TABLE_FMODE_MODEL, a_oIOColumn.Name());
		KResultSetPtr spResultset = spDbConeeciton->ExecuteQuery(strSQL);
		if (spResultset->Next())
		{
			a_nModeID = spResultset->GetValueInt(0);
			return true;
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	return false;
}

void KTableEditFieldInfo::GetVehicleModeInfo( KDBaseConPtr spDBaseConnection, CString strColumnName, TVehicleModelKey& a_oVehicleInfo )
{
	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Select fmode_id, column_name, run_seq From %s where column_name = '%s' "), TABLE_FMODE_MODEL, strColumnName);
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		if (spResultset->Next())
		{
			a_oVehicleInfo.nFmode_id		= spResultset->GetValueInt(0);
			a_oVehicleInfo.strColumnName	= spResultset->GetValueString(1);
			a_oVehicleInfo.nRunSeq			= spResultset->GetValueInt(2);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

bool KTableEditFieldInfo::UpdateVehicleModelMode( KDBaseConPtr spDBaseConneciton, TVehicleModelKey oTVehicleInfo, int a_nModeID  )
{
	CString strUpdateModeSQL(_T(""));
	CString strUpdateRunSQL(_T(""));
	CString strDelSQL(_T(""));

	try
	{
		spDBaseConneciton->BeginTransaction();

		strUpdateModeSQL.Format(_T("Update %s Set fmode_id = %d where column_name = '%s' "), TABLE_FMODE_MODEL, a_nModeID, oTVehicleInfo.strColumnName);
		spDBaseConneciton->ExecuteUpdate(strUpdateModeSQL);

		strUpdateRunSQL.Format(_T("Update %s Set run_seq = 0 where run_seq = %d" ), TABLE_FMODE_MODEL, oTVehicleInfo.nRunSeq);
		spDBaseConneciton->ExecuteUpdate(strUpdateRunSQL);

		strDelSQL.Format(_T("Update %s Set small = '' , medium = '', large = '' where run_seq = %d "), TABLE_VEHICLECONV_MODEL, oTVehicleInfo.nRunSeq);
		spDBaseConneciton->ExecuteUpdate(strDelSQL);

		spDBaseConneciton->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConneciton->RollBack();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConneciton->RollBack();
		return false;
	}

	return true;
}
