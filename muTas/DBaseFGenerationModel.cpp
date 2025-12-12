#include "StdAfx.h"
#include "DBaseFGenerationModel.h"
#include "Target.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseConnector.h"
#include "model_common.h"

KDBaseFGenerationModel::KDBaseFGenerationModel(void)
{
}


KDBaseFGenerationModel::~KDBaseFGenerationModel(void)
{
}

void KDBaseFGenerationModel::GetDefaultFGenerationParameter( std::map<int, std::map<int, KFGenerationParaInfo>>& ar_mapFGenerationPara )
{
	ar_mapFGenerationPara.clear();

	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Select master_code, object_id, pa_type, unit_variable, unit_column, reg_formula From %s Order by master_code, object_id, pa_type"), KDefaultParaTable::TABLE_DEFAULT_FGENERATIONPARA);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			KFGenerationParaInfo oGInfo;
			oGInfo.TnMasterCode			= spResultset->GetValueInt(0);
			oGInfo.TnPurposeID			= spResultset->GetValueInt(1);
			
			KFPAGenerationparaInfo oPAInfo;
			oPAInfo.TnPAType			= spResultset->GetValueInt(2);
			oPAInfo.TdUnitValue			= spResultset->GetValueDouble(3);
			oPAInfo.TstrUnitColumnName	= spResultset->GetValueString(4);
			oPAInfo.TstrRegresionFomula	= spResultset->GetValueString(5);

			AutoType ofind = ar_mapFGenerationPara.find(oGInfo.TnPurposeID);
			AutoType oend  = ar_mapFGenerationPara.end();
			if (ofind == oend)
			{
				std::map<int, KFGenerationParaInfo> mapPara;
				if (oPAInfo.TnPAType == KEMPATypeProduction)
				{
					oGInfo.TPParameter = oPAInfo;
				}
				else if (oPAInfo.TnPAType == KEMPATypeAttraction)
				{
					oGInfo.TCParameter = oPAInfo;
				}
				mapPara.insert(std::make_pair(oGInfo.TnMasterCode, oGInfo));
				ar_mapFGenerationPara.insert(std::make_pair(oGInfo.TnPurposeID, mapPara));
			}
			else
			{
				std::map<int, KFGenerationParaInfo>& mapPara = ofind->second;
				
				AutoType mfind = mapPara.find(oGInfo.TnMasterCode);
				AutoType mend  = mapPara.end();
				if (mfind != mend)
				{
					if (oPAInfo.TnPAType == KEMPATypeProduction)
					{
						mfind->second.TPParameter = oPAInfo;
					}
					else if (oPAInfo.TnPAType == KEMPATypeAttraction)
					{
						mfind->second.TCParameter = oPAInfo;
					}
				}
				else
				{
					if (oPAInfo.TnPAType == KEMPATypeProduction)
					{
						oGInfo.TPParameter = oPAInfo;
					}
					else if (oPAInfo.TnPAType == KEMPATypeAttraction)
					{
						oGInfo.TCParameter = oPAInfo;
					}
					mapPara.insert(std::make_pair(oGInfo.TnMasterCode, oGInfo));
				}
			}
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

void KDBaseFGenerationModel::SetDefaultFGenerationParameter( std::vector<KFGenerationParaInfo> a_vecKFGenerationPara )
{
	CString strDelSQL(_T(""));
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();
		strDelSQL.Format(_T("Delete From %s Where master_code = %d"), KDefaultParaTable::TABLE_DEFAULT_FGENERATIONPARA, KDefaultParameterMasterCode::USER_MASETER_CODE);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		size_t nCount = a_vecKFGenerationPara.size();
		for (size_t i = 0; i < nCount; i ++)
		{
			KFGenerationParaInfo oInfo = a_vecKFGenerationPara[i];

			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s (master_Code, object_id, pa_type, unit_variable, unit_column, reg_formula ) "), KDefaultParaTable::TABLE_DEFAULT_FGENERATIONPARA);
			strSQL.AppendFormat(_T(" Values "));

			CString strUPSQL(_T(""));
			strUPSQL.Format(_T(" %s ( '%d' , '%d', '%d', '%f', '%s', '%s' ) "), strSQL, KDefaultParameterMasterCode::USER_MASETER_CODE, 
				oInfo.TnPurposeID, KEMPATypeProduction, oInfo.TPParameter.TdUnitValue, oInfo.TPParameter.TstrUnitColumnName, 
				oInfo.TPParameter.TstrRegresionFomula);
			spDBaseConnection->ExecuteUpdate(strUPSQL);

			CString strUCSQL(_T(""));
			strUCSQL.Format(_T(" %s ( '%d' , '%d', '%d', '%f', '%s', '%s' ) "), strSQL, KDefaultParameterMasterCode::USER_MASETER_CODE, 
				oInfo.TnPurposeID, KEMPATypeAttraction, oInfo.TCParameter.TdUnitValue, oInfo.TCParameter.TstrUnitColumnName, 
				oInfo.TCParameter.TstrRegresionFomula);
			spDBaseConnection->ExecuteUpdate(strUCSQL);


			if (oInfo.TnMasterCode == KDefaultParameterMasterCode::NEW_MASTER_CODE)
			{
				strUPSQL.Format(_T(" %s ( '%d' , '%d', '%d', '%f', '%s', '%s' ) "), strSQL, KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, 
					oInfo.TnPurposeID, KEMPATypeProduction, oInfo.TPParameter.TdUnitValue, oInfo.TPParameter.TstrUnitColumnName, 
					oInfo.TPParameter.TstrRegresionFomula);
				spDBaseConnection->ExecuteUpdate(strUPSQL);

				strUCSQL.Format(_T(" %s ( '%d' , '%d', '%d', '%f', '%s', '%s' ) "), strSQL, KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, 
					oInfo.TnPurposeID, KEMPATypeAttraction, oInfo.TCParameter.TdUnitValue, oInfo.TCParameter.TstrUnitColumnName, 
					oInfo.TCParameter.TstrRegresionFomula);
				spDBaseConnection->ExecuteUpdate(strUCSQL);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw -1;
	}
	spDBaseConnection->Commit();
}
