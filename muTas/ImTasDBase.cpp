#include "StdAfx.h"
#include "ImTasDBase.h"
//^#include "DBaseConnector.h"
//^#include "ParseString.h"
#include "model_common.h"
#include "KmzSystem.h"


void KImTasDBase::GenerationFormula( KDBaseConPtr spDBaseConnection, KEMPurposeGroup a_emPurposeGroup, int a_nMasterCode, std::map<int, std::map<int, std::vector<CString>>> &a_mapFormula )
{
	a_mapFormula.clear();
	int nProgramType  = 1;
	if (KmzSystem::IsPassengerSystem())
	{
		nProgramType = 1;
	}
	else
	{
		nProgramType = 2;
	}

	CString strSQL;
	strSQL.Format(
		_T(" SELECT Detail_Object_ID, PA_Flag, Formula_String FROM Generation_Formula ")
		_T(" WHERE Program_Type = %d AND Master_Code = %d AND Object_Group_Code = %d ")
		_T(" ORDER BY Detail_Object_ID, PA_Flag "), nProgramType, a_nMasterCode, a_emPurposeGroup);

	CString strKeyValue;
	try
	{        
		KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			int     nDetailObjectID = spResultSet->GetValueInt   (0);
			CString strPAFlag       = spResultSet->GetValueString(1);
			CString strFormula      = spResultSet->GetValueString(2);

			int nPAType = KEMPATypeProduction;
			if(strPAFlag.CompareNoCase(_T("p")) == 0)
			{
				nPAType = KEMPATypeProduction;
			}
			else
			{
				nPAType = KEMPATypeAttraction;
			}

			AutoType iter  = a_mapFormula.find(nDetailObjectID);
			AutoType itEnd = a_mapFormula.end();

			if(iter != itEnd)
			{
				std::map<int, std::vector<CString>> &mapFormulaInfo = iter->second;

				AutoType iterB  = mapFormulaInfo.find(nPAType);
				AutoType itBEnd = mapFormulaInfo.end();

				if(iterB != itBEnd)
				{
					std::vector<CString> &vecFormulaText = iterB->second;
					vecFormulaText.push_back(strFormula);
				}
				else
				{
					std::vector<CString> vecFormulaText;
					vecFormulaText.push_back(strFormula);
					mapFormulaInfo.insert(std::make_pair(nPAType, vecFormulaText));
				}
			}
			else
			{
				std::map<int, std::vector<CString>> mapFormulaInfo;

				std::vector<CString> vecFormulaText;
				vecFormulaText.push_back(strFormula);
				mapFormulaInfo.insert(std::make_pair(nPAType, vecFormulaText));

				a_mapFormula.insert(std::make_pair(nDetailObjectID, mapFormulaInfo));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_mapFormula.clear();
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		a_mapFormula.clear();
		return;
	} 
}


void KImTasDBase::FormulaMasterData(KDBaseConPtr spDBaseConnection, std::vector<TFormulaMaster> &a_vecTFormulaMaster)
{
	a_vecTFormulaMaster.clear();

	CString strSQL;
	strSQL.Format(_T(" SELECT Master_Code, Master_Name FROM Formula_Master order by Master_Code "));

	int nDataCount = 0;
	try
	{        
		KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TFormulaMaster oTFormulaMaster;

			oTFormulaMaster.TCode  = spResultSet->GetValueInt   (0);
			oTFormulaMaster.TName  = spResultSet->GetValueString(1);

			a_vecTFormulaMaster.push_back(oTFormulaMaster);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_vecTFormulaMaster.clear();
	}
	catch (...)
	{
		TxLogDebugWarning();
		a_vecTFormulaMaster.clear();
	} 
}


void KImTasDBase::DistributionFormula( KDBaseConPtr spDBaseConnection, KEMPurposeGroup a_emPurposeGroup, int a_nMasterCode, std::map<int, std::vector<TImpedanceFunction>> &a_mapImpedanceFunction)
{
	a_mapImpedanceFunction.clear();

	int nProgramType  = 1;
	if (KmzSystem::IsPassengerSystem())
	{
		nProgramType = 1;
	}
	else
	{
		nProgramType = 2;
	}

	CString strSQL;
	strSQL.Format(
		_T(" SELECT Detail_Object_ID, tbl1.Function_Type, tbl2.Name, Param_Alpha, Param_Beta, Param_Theta, Variable_Rij FROM ")
		_T("        (SELECT Detail_Object_ID, Function_Type, Param_Alpha, Param_Beta, Param_Theta, Variable_Rij FROM Distribution_Formula ")
		_T("		 WHERE Program_Type = %d AND Master_Code = %d AND Object_Group_Code = %d ")
		_T("         ORDER BY Detail_Object_ID) TBL1, ")
		_T("         Impedance_Function TBL2 ")
		_T("         WHERE TBL1.Function_Type = TBL2.Type "), nProgramType, a_nMasterCode, a_emPurposeGroup);

	CString strKeyValue;
	try
	{        
		KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TImpedanceFunction oTImpedanceFunction;

			int     nDetailObjectID             = spResultSet->GetValueInt   (0);
			oTImpedanceFunction.nFunctionType   = spResultSet->GetValueInt   (1);
			oTImpedanceFunction.strFunctionName = spResultSet->GetValueString(2);
			oTImpedanceFunction.dAlpha          = spResultSet->GetValueDouble(3);
			oTImpedanceFunction.dBeta           = spResultSet->GetValueDouble(4);
			oTImpedanceFunction.dTheta          = spResultSet->GetValueDouble(5);
			oTImpedanceFunction.strVarRij       = spResultSet->GetValueString(6);

			AutoType iter  = a_mapImpedanceFunction.find(nDetailObjectID);
			AutoType itEnd = a_mapImpedanceFunction.end();

			if(iter != itEnd)
			{
				std::vector<TImpedanceFunction> &vecImpedanceFunction = iter->second;
				vecImpedanceFunction.push_back(oTImpedanceFunction);
			}
			else
			{
				std::vector<TImpedanceFunction> vecImpedanceFunction;
				vecImpedanceFunction.push_back(oTImpedanceFunction);

				a_mapImpedanceFunction.insert(std::make_pair(nDetailObjectID, vecImpedanceFunction));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_mapImpedanceFunction.clear();
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		a_mapImpedanceFunction.clear();
		return;
	} 
}


void KImTasDBase::DistributionFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, int a_nObjectGroupCode, int a_nDetailObejctID, TImpedanceFunction &a_oTImpedanceFunction )
{
	int nProgramType  = 1;
	if (KmzSystem::IsPassengerSystem())
	{
		nProgramType = 1;
	}
	else
	{
		nProgramType = 2;
	}

	CString strSQL;
	strSQL.Format(
		_T(" SELECT tbl1.Function_Type, tbl2.Name, Param_Alpha, Param_Beta, Param_Theta, Variable_Rij FROM ")
		_T("        (SELECT Detail_Object_ID, Function_Type, Param_Alpha, Param_Beta, Param_Theta, Variable_Rij FROM Distribution_Formula ")
		_T("		 WHERE Program_Type = %d AND Master_Code = %d AND Object_Group_Code = %d AND Detail_Object_ID = %d ")
		_T("         ) TBL1, ")
		_T("         Impedance_Function TBL2 ")
		_T("         WHERE TBL1.Function_Type = TBL2.Type "), nProgramType, a_nMasterCode, a_nObjectGroupCode, a_nDetailObejctID);

	CString strKeyValue;
	try
	{        
		KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			a_oTImpedanceFunction.nFunctionType   = spResultSet->GetValueInt   (0);
			a_oTImpedanceFunction.strFunctionName = spResultSet->GetValueString(1);
			a_oTImpedanceFunction.dAlpha          = spResultSet->GetValueDouble(2);
			a_oTImpedanceFunction.dBeta           = spResultSet->GetValueDouble(3);
			a_oTImpedanceFunction.dTheta          = spResultSet->GetValueDouble(4);
			a_oTImpedanceFunction.strVarRij       = spResultSet->GetValueString(5);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		return;
	} 
}


void KImTasDBase::GenerationUnitFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, int a_nObjectGroupCode, int a_nDetailObejctID, CString a_strPAFlag, TGenerationUnitFunction &a_oTGenerationUnitFunction )
{
	int nProgramType  = 1;
	if (KmzSystem::IsPassengerSystem())
	{
		nProgramType = 1;
	}
	else
	{
		nProgramType = 2;
	}

	CString strSQL(_T(" SELECT Unit_Val, Unit_String FROM Generation_Formula "));
	strSQL.AppendFormat(_T(" WHERE Formula_Type = %d "),    KEMGenerationUnit);
	strSQL.AppendFormat(_T(" AND Program_Type = %d "),      nProgramType);
	strSQL.AppendFormat(_T(" AND Master_Code = %d "),       a_nMasterCode);
	strSQL.AppendFormat(_T(" AND Object_Group_Code = %d "), a_nObjectGroupCode);
	strSQL.AppendFormat(_T(" AND Detail_Object_ID = %d "),  a_nDetailObejctID);
	strSQL.AppendFormat(_T(" AND PA_Flag = '%s' "),         a_strPAFlag);

	try
	{
		a_oTGenerationUnitFunction.bResult    = false;

		KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		if ( pResultSet->Next() )
		{
			a_oTGenerationUnitFunction.dValue     = pResultSet->GetValueDouble(0);
			a_oTGenerationUnitFunction.strObject  = pResultSet->GetValueString(1);
			a_oTGenerationUnitFunction.bResult    = true;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		return;
	} 
}


void KImTasDBase::GenerationRegressFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, int a_nObjectGroupCode, int a_nDetailObejctID, CString a_strPAFlag, TGenerationRegressFunction &a_oTGenerationRegressFunction )
{
	int nProgramType  = 1;
	if (KmzSystem::IsPassengerSystem())
	{
		nProgramType = 1;
	}
	else
	{
		nProgramType = 2;
	}

	CString strSQL(_T(" SELECT Regression_String FROM Generation_Formula "));
	strSQL.AppendFormat(_T(" WHERE Formula_Type = %d "),    KEMGenerationRegression);
	strSQL.AppendFormat(_T(" AND Program_Type = %d "),      nProgramType);
	strSQL.AppendFormat(_T(" AND Master_Code = %d "),       a_nMasterCode);
	strSQL.AppendFormat(_T(" AND Object_Group_Code = %d "), a_nObjectGroupCode);
	strSQL.AppendFormat(_T(" AND Detail_Object_ID = %d "),  a_nDetailObejctID);
	strSQL.AppendFormat(_T(" AND PA_Flag = '%s' "),         a_strPAFlag);

	try
	{
		a_oTGenerationRegressFunction.bResult    = false;

		KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		if ( pResultSet->Next() )
		{
			a_oTGenerationRegressFunction.strExpression  = pResultSet->GetValueString(0);
			a_oTGenerationRegressFunction.bResult        = true;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		return;
	} 
}


void KImTasDBase::ModeChoiceFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, KEMPurposeGroup a_emPurposeGroup, KEMModeGroup a_emModeGroup, std::map<int, std::map<int, CString>> &a_mapFunctionStr )
{
	int nProgramType  = 1;
	if (KmzSystem::IsPassengerSystem())
	{
		nProgramType = 1;
	}
	else
	{
		nProgramType = 2;
	}

	CString strSQL(_T(" SELECT Detail_Object_ID, Detail_Mode_ID, Function_String  "));
	strSQL.Append      (_T(" FROM  ModeChoice_Formula "));
	strSQL.AppendFormat(_T(" WHERE Program_Type = %d "),      nProgramType);
	strSQL.AppendFormat(_T(" AND   Master_Code = %d "),       a_nMasterCode);
	strSQL.AppendFormat(_T(" AND   Object_Group_Code = %d "), a_emPurposeGroup);
	strSQL.AppendFormat(_T(" AND   Mode_Group_Code = %d "),   a_emModeGroup);
	strSQL.Append      (_T(" ORDER BY Detail_Object_ID, Detail_Mode_ID "));

	try
	{
		a_mapFunctionStr.clear();

		AutoType iter  = a_mapFunctionStr.begin();
		AutoType itEnd = a_mapFunctionStr.end();

		KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( pResultSet->Next() )
		{
			int nObjectID       = pResultSet->GetValueInt(0);
			int nModeID         = pResultSet->GetValueInt(1);
			CString strFunction = pResultSet->GetValueString(2);

			iter = a_mapFunctionStr.find(nObjectID);
			if (iter == itEnd)
			{
				std::map<int, CString> mapFunction;
				mapFunction.insert(std::make_pair(nModeID, strFunction));

				a_mapFunctionStr.insert(std::make_pair(nObjectID, mapFunction));
			}
			else
			{
				std::map<int, CString> &mapFunction = iter->second;
				mapFunction.insert(std::make_pair(nModeID, strFunction));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapFunctionStr.clear();
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		a_mapFunctionStr.clear();
		TxLogDebugWarning();
		return;
	} 
}


void KImTasDBase::ModeChoiceShotLongFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, KEMPurposeGroup a_emPurposeGroup, std::map<int, double> &a_mapDistance )
{
	int nProgramType  = 1;
	if (KmzSystem::IsPassengerSystem())
	{
		nProgramType = 1;
	}
	else
	{
		nProgramType = 2;
	}

	CString strSQL(_T(" SELECT Detail_Object_ID, short_long_dist  "));
	strSQL.Append      (_T(" FROM  ModeChoice_Short_Long_Formula "));
	strSQL.AppendFormat(_T(" WHERE Program_Type = %d "),      nProgramType);
	strSQL.AppendFormat(_T(" AND   Master_Code = %d "),       a_nMasterCode);
	strSQL.AppendFormat(_T(" AND   Object_Group_Code = %d "), a_emPurposeGroup);
	strSQL.Append      (_T(" ORDER BY Detail_Object_ID "));

	try
	{
		a_mapDistance.clear();

		AutoType iter  = a_mapDistance.begin();
		AutoType itEnd = a_mapDistance.end();

		KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( pResultSet->Next() )
		{
			int    nObjectID			= pResultSet->GetValueInt(0);
			double dShotLongDist        = pResultSet->GetValueDouble(1);
			a_mapDistance.insert(std::make_pair(nObjectID, dShotLongDist));
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapDistance.clear();
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		a_mapDistance.clear();
		TxLogDebugWarning();
		return;
	} 
}


void KImTasDBase::FLogisticModelFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, KEMPurposeGroup a_emPurposeGroup, std::map<int, double> &a_mapRatio )
{
	int nProgramType  = 1;
	if (KmzSystem::IsPassengerSystem())
	{
		nProgramType = 1;
	}
	else
	{
		nProgramType = 2;
	}

	CString strSQL(_T(" SELECT Detail_Object_ID, ratio  "));
	strSQL.Append      (_T(" FROM  FLogistic_Formula "));
	strSQL.AppendFormat(_T(" WHERE Program_Type = %d "),      nProgramType);
	strSQL.AppendFormat(_T(" AND   Master_Code = %d "),       a_nMasterCode);
	strSQL.AppendFormat(_T(" AND   Object_Group_Code = %d "), a_emPurposeGroup);
	strSQL.Append      (_T(" ORDER BY Detail_Object_ID "));

	try
	{
		a_mapRatio.clear();

		KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( pResultSet->Next() )
		{
			int    nObjectID			= pResultSet->GetValueInt(0);
			double dShotLongDist        = pResultSet->GetValueDouble(1);
			a_mapRatio.insert(std::make_pair(nObjectID, dShotLongDist));
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapRatio.clear();
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		a_mapRatio.clear();
		TxLogDebugWarning();
		return;
	} 
}
// 
// void KImTasDBase::VehicleConvertFormula( 
// 	KDBaseConPtr spDBaseConnection, int a_nMasterCode, 
// 	KEMPurposeGroup a_emPurposeGroup, int a_nDetailObejctID, std::vector<TVehicleConvert> &a_vecData )
// {
// 	int nProgramType  = 1;
// 	
// 	if (KmzSystem::IsPassengerSystem())
// 		nProgramType = 1;
// 	else
// 		nProgramType = 2;
// 	
// 	CString strSQL = _T("Select ");
// 	strSQL.Append      (_T("Detail_Object_ID, Service_Type, Vehicle_Type, Alpha, Beta, "));
// 	strSQL.Append      (_T("Weight "));
// 	strSQL.AppendFormat(_T("From VehicleConvert_Formula Where Program_Type = %d "), nProgramType);
// 	strSQL.AppendFormat(_T("And Master_Code = %d "), a_nMasterCode);
// 	strSQL.AppendFormat(_T("And Object_Group_Code = %d "), a_emPurposeGroup);
// 	strSQL.AppendFormat(_T("And Detail_Object_ID = %d "), a_nDetailObejctID);
// 	strSQL.Append      (_T("Order By Detail_Object_ID, Service_Type, Vehicle_Type "));
// 
// 	try
// 	{
// 		a_vecData.clear();
// 
// 		KResultSetPtr pResultSet = spDBaseConnection->ExecuteQuery(strSQL);
// 
// 		while (pResultSet->Next())
// 		{
// 			TVehicleConvert oVehicleConvert = {};
// 
// 			oVehicleConvert.nServiceType = pResultSet->GetValueInt(1);
// 			oVehicleConvert.nVehicleType = pResultSet->GetValueInt(2);
// 			oVehicleConvert.dAlpha       = pResultSet->GetValueDouble(3);
// 			oVehicleConvert.dBeta        = pResultSet->GetValueDouble(4);
// 			oVehicleConvert.dWeight      = pResultSet->GetValueDouble(5);
// 
// 			a_vecData.push_back(oVehicleConvert);
// 		}
// 
// 	}
// 	catch (KExceptionPtr ex)
// 	{
// 		a_vecData.clear();
// 		TxExceptionPrint(ex);
// 		return;
// 	}
// 	catch (...)
// 	{
// 		a_vecData.clear();
// 		TxLogDebugWarning();
// 		return;
// 	}
// }


void KProjectDBase::NodeTemplate( KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TNodeTemplate>& ar_vecTemplate )
{
	ar_vecTemplate.clear();

	CString strSQL;
	strSQL.Format(_T("Select ID, Type, Name From NodeTemplate Where Type = '%d' Order by ID"), a_nType);
	
	try
	{        
		KResultSetPtr  spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TNodeTemplate oTemplate;
			oTemplate.TID   = spResultSet->GetValueInt(0);
			oTemplate.TType = spResultSet->GetValueInt(1);
			oTemplate.TName = spResultSet->GetValueString(2);

			ar_vecTemplate.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KProjectDBase::NodeTemplateType( KDBaseConPtr spDBaseConnection, int a_nID, std::vector<TNodeTemplateType>& ar_vecTemplateType )
{
	ar_vecTemplateType.clear();

	CString strSQL;
	strSQL.Format(_T("Select ID, Seq, Symbol, Size, RGB From NodeTemplateType Where ID = '%d' Order by Seq"), a_nID);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TNodeTemplateType oTemplate;
			oTemplate.TID     = spResultSet->GetValueInt(0);
			oTemplate.TSeq    = spResultSet->GetValueInt(1);
			oTemplate.TSymbol = spResultSet->GetValueInt(2);
			oTemplate.TSize   = spResultSet->GetValueDouble(3);

			CString strRGB    = spResultSet->GetValueString(4);
			oTemplate.TR      = 0;
			oTemplate.TG      = 0;
			oTemplate.TB      = 0;

			std::vector<CString> vecRGB;
			CString strTok;
			int     nTokenPos = 0; 
			while (AfxExtractSubString(strTok, strRGB, nTokenPos++, ','))
			{
				vecRGB.push_back(strTok.MakeLower());
			}

			if (vecRGB.size() == 3)
			{
				int nRValue  =  _ttoi(vecRGB[0]);
				int nGValue  =  _ttoi(vecRGB[1]);
				int nBValue  =  _ttoi(vecRGB[2]);

				oTemplate.TR = nRValue ;
				oTemplate.TG = nGValue ;
				oTemplate.TB = nBValue ;
			}

			ar_vecTemplateType.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KProjectDBase::NodeTemplateValue( KDBaseConPtr spDBaseConnection, int a_nID, std::vector<TNodeTemplateValue>& ar_vecTemplateValue )
{
	ar_vecTemplateValue.clear();
	CString strSQL;
	strSQL.Format(_T("Select ID, Seq, Symbol, Size, RGB, Min, Max From NodeTemplateValue Where ID = '%d' Order by Seq"), a_nID);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TNodeTemplateValue oTemplate;
			oTemplate.TID     = spResultSet->GetValueInt   (0);
			oTemplate.TSeq    = spResultSet->GetValueInt   (1);
			oTemplate.TSymbol = spResultSet->GetValueInt   (2);
			oTemplate.TSize   = spResultSet->GetValueDouble(3);

			CString strRGB    = spResultSet->GetValueString(4);
			oTemplate.TR      = 0; 	
			oTemplate.TG      = 0; 
			oTemplate.TB      = 0;

			oTemplate.TMin    = spResultSet->GetValueString(5);
			oTemplate.TMax    = spResultSet->GetValueString(6);

			std::vector<CString> vecRGB;
			CString strTok;
			int     nTokenPos = 0; 
			while (AfxExtractSubString(strTok, strRGB, nTokenPos++, ','))
			{
				vecRGB.push_back(strTok.MakeLower());
			}

			if (vecRGB.size() == 3)
			{
				int nRValue  =  _ttoi(vecRGB[0]);
				int nGValue  =  _ttoi(vecRGB[1]);
				int nBValue  =  _ttoi(vecRGB[2]);

				oTemplate.TR = nRValue ;
				oTemplate.TG = nGValue ;
				oTemplate.TB = nBValue ;
			}

			ar_vecTemplateValue.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KProjectDBase::LinkTemplate( KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TLinkTemplate>& ar_vecTemplate )
{
	ar_vecTemplate.clear();

	CString strSQL;
	strSQL.Format(_T("Select ID, Type, Name From LinkTemplate Where Type = '%d' Order by ID"), a_nType);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TLinkTemplate oTemplate;
			oTemplate.TID   = spResultSet->GetValueInt(0);
			oTemplate.TType = spResultSet->GetValueInt(1);
			oTemplate.TName = spResultSet->GetValueString(2);

			ar_vecTemplate.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KProjectDBase::LinkTemplateType( KDBaseConPtr spDBaseConnection, int a_nID, std::vector<TLinkTemplateType>& ar_vecTemplateType )
{
	ar_vecTemplateType.clear();

	CString strSQL;
	strSQL.Format(_T("Select ID, Seq, LineType, Width, RGB From LinkTemplateType Where ID = '%d' Order by Seq"), a_nID);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TLinkTemplateType oTemplate;
			oTemplate.TID       = spResultSet->GetValueInt(0);
			oTemplate.TSeq      = spResultSet->GetValueInt(1);
			oTemplate.TLineType = spResultSet->GetValueInt(2);
			oTemplate.TWidth    = spResultSet->GetValueDouble(3);

			CString strRGB    = spResultSet->GetValueString(4);
			oTemplate.TR      = 0;
			oTemplate.TG      = 0;
			oTemplate.TB      = 0;

			std::vector<CString> vecRGB;
			CString strTok;
			int     nTokenPos = 0; 
			while (AfxExtractSubString(strTok, strRGB, nTokenPos++, ','))
			{
				vecRGB.push_back(strTok.MakeLower());
			}

			if (vecRGB.size() == 3)
			{
				int nRValue  =  _ttoi(vecRGB[0]);
				int nGValue  =  _ttoi(vecRGB[1]);
				int nBValue  =  _ttoi(vecRGB[2]);

				oTemplate.TR = nRValue ;
				oTemplate.TG = nGValue ;
				oTemplate.TB = nBValue ;
			}

			ar_vecTemplateType.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KProjectDBase::LinkTemplateValue( KDBaseConPtr spDBaseConnection, int a_nID, std::vector<TLinkTemplateValue>& ar_vecTemplateValue )
{
	ar_vecTemplateValue.clear();
	CString strSQL;
	strSQL.Format(_T("Select ID, Seq, LineType, Width, RGB, Min, Max From LinkTemplateValue Where ID = '%d' Order by Seq"), a_nID);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TLinkTemplateValue oTemplate;
			oTemplate.TID       = spResultSet->GetValueInt   (0);
			oTemplate.TSeq      = spResultSet->GetValueInt   (1);
			oTemplate.TLineType = spResultSet->GetValueInt   (2);
			oTemplate.TWidth    = spResultSet->GetValueDouble(3);

			CString strRGB      = spResultSet->GetValueString(4);
			oTemplate.TR        = 0; 	
			oTemplate.TG        = 0; 
			oTemplate.TB        = 0;

			oTemplate.TMin      = spResultSet->GetValueString(5);
			oTemplate.TMax      = spResultSet->GetValueString(6);

			std::vector<CString> vecRGB;
			CString strTok;
			int     nTokenPos = 0; 
			while (AfxExtractSubString(strTok, strRGB, nTokenPos++, ','))
			{
				vecRGB.push_back(strTok.MakeLower());
			}

			if (vecRGB.size() == 3)
			{
				int nRValue  =  _ttoi(vecRGB[0]);
				int nGValue  =  _ttoi(vecRGB[1]);
				int nBValue  =  _ttoi(vecRGB[2]);

				oTemplate.TR = nRValue ;
				oTemplate.TG = nGValue ;
				oTemplate.TB = nBValue ;
			}

			ar_vecTemplateValue.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KProjectDBase::ZoneTemplate( KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TZoneTemplate>& ar_vecTemplate )
{
	ar_vecTemplate.clear();

	CString strSQL;
	strSQL.Format(_T("Select ID, Type, Name From ZoneTemplate Where Type = '%d' Order by ID"), a_nType);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TZoneTemplate oTemplate;
			oTemplate.TID   = spResultSet->GetValueInt(0);
			oTemplate.TType = spResultSet->GetValueInt(1);
			oTemplate.TName = spResultSet->GetValueString(2);

			ar_vecTemplate.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KProjectDBase::ZoneTemplateType( KDBaseConPtr spDBaseConnection, int a_nID, std::vector<TZoneTemplateType>& ar_vecTemplateType )
{
	ar_vecTemplateType.clear();

	CString strSQL;
	strSQL.Format(_T("Select ID, Seq, RGB From ZoneTemplateType Where ID = '%d' Order by Seq"), a_nID);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TZoneTemplateType oTemplate;
			oTemplate.TID       = spResultSet->GetValueInt(0);
			oTemplate.TSeq      = spResultSet->GetValueInt(1);

			CString strRGB    = spResultSet->GetValueString(2);
			oTemplate.TR      = 0;
			oTemplate.TG      = 0;
			oTemplate.TB      = 0;

			std::vector<CString> vecRGB;
			CString strTok;
			int     nTokenPos = 0; 
			while (AfxExtractSubString(strTok, strRGB, nTokenPos++, ','))
			{
				vecRGB.push_back(strTok.MakeLower());
			}

			if (vecRGB.size() == 3)
			{
				int nRValue  =  _ttoi(vecRGB[0]);
				int nGValue  =  _ttoi(vecRGB[1]);
				int nBValue  =  _ttoi(vecRGB[2]);

				oTemplate.TR = nRValue ;
				oTemplate.TG = nGValue ;
				oTemplate.TB = nBValue ;
			}

			ar_vecTemplateType.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KProjectDBase::ZoneTemplateValue( KDBaseConPtr spDBaseConnection, int a_nID, std::vector<TZoneTemplateValue>& ar_vecTemplateValue )
{
	ar_vecTemplateValue.clear();
	CString strSQL;
	strSQL.Format(_T("Select ID, Seq, RGB, Min, Max From ZoneTemplateValue Where ID = '%d' Order by Seq"), a_nID);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TZoneTemplateValue oTemplate;
			oTemplate.TID       = spResultSet->GetValueInt   (0);
			oTemplate.TSeq      = spResultSet->GetValueInt   (1);			

			CString strRGB      = spResultSet->GetValueString(2);
			oTemplate.TR        = 0; 	
			oTemplate.TG        = 0; 
			oTemplate.TB        = 0;

			oTemplate.TMin      = spResultSet->GetValueString(3);
			oTemplate.TMax      = spResultSet->GetValueString(4);

			std::vector<CString> vecRGB;
			CString strTok;
			int     nTokenPos = 0; 
			while (AfxExtractSubString(strTok, strRGB, nTokenPos++, ','))
			{
				vecRGB.push_back(strTok.MakeLower());
			}

			if (vecRGB.size() == 3)
			{
				int nRValue  =  _ttoi(vecRGB[0]);
				int nGValue  =  _ttoi(vecRGB[1]);
				int nBValue  =  _ttoi(vecRGB[2]);

				oTemplate.TR = nRValue ;
				oTemplate.TG = nGValue ;
				oTemplate.TB = nBValue ;
			}

			ar_vecTemplateValue.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}


int KProjectDBase::NewNodeTemplateID( KDBaseConPtr spDBaseConnection, int a_nType )
{
	CString strSQL;
	strSQL.Format(_T(" Select ifnull(max(ID)+1, 0) as MaxID From NodeTemplate Where Type = '%d'"), a_nType);
	
	int nMaxValue = 0;
	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			nMaxValue = spResultSet->GetValueInt(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 

	return nMaxValue;
}

int KProjectDBase::NewLinkTemplateID( KDBaseConPtr spDBaseConnection, int a_nType )
{
	CString strSQL;
	strSQL.Format(_T(" Select ifnull(max(ID)+1, 0) as MaxID From LinkTemplate Where Type = '%d'"), a_nType);

	int nMaxValue = 0;
	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			nMaxValue = spResultSet->GetValueInt(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 

	return nMaxValue;
}

int KProjectDBase::NewZoneTemplateID( KDBaseConPtr spDBaseConnection, int a_nType )
{
	CString strSQL;
	strSQL.Format(_T(" Select ifnull(max(ID)+1, 0) as MaxID From ZoneTemplate Where Type = '%d'"), a_nType);

	int nMaxValue = 0;
	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			nMaxValue = spResultSet->GetValueInt(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 

	return nMaxValue;
}

void KProjectDBase::NetwrokTemplate( KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TNetworkTemplate>& ar_vecNetworkTemplate )
{
	ar_vecNetworkTemplate.clear();

	CString strSQL;
	strSQL.Format(_T(" Select ID, template_name From NetworkEdit_Template Where network_type = '%d' Order by ID"), a_nType);

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		TNetworkTemplate oTemplate;

		while ( spResultSet->Next() )
		{			
			oTemplate.TID   = spResultSet->GetValueInt(0);
			oTemplate.TName = spResultSet->GetValueString(1);

			ar_vecNetworkTemplate.push_back(oTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		ThrowException(_T("Error-NetwrokTemplate"));
	} 
}

void KProjectDBase::NetwrokTemplate( KDBaseConPtr spDBaseConnection, int a_nID, int a_nType, std::map<CString, CString>& ar_mapTemplateValue )
{
	ar_mapTemplateValue.clear();

	CString strSQL;
	strSQL.Format(_T(" Select key_value From NetworkEdit_Template Where ID = '%d' And network_type = '%d' "), a_nID, a_nType);

	CString strKeyValue;
	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			strKeyValue = spResultSet->GetValueString(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		ThrowException(_T("Error-NetwrokTemplate"));
	} 

	// [컬럼이름@Value]~[컬럼이름@Value] 
	if (strKeyValue.GetLength() < 1)
		return;

	std::vector<CString> vecKeyValueBracket = KParseString::ParseString(strKeyValue, _T('~'));
	size_t nxCount = vecKeyValueBracket.size();

	for (size_t i=0; i<nxCount; i++)
	{
		CString strLine = vecKeyValueBracket[i];
		if (strLine.GetLength() < 3)
			continue;

		strLine.Remove(_T('['));
		strLine.Remove(_T(']'));

		std::vector<CString> vecKeyValue = KParseString::ParseString(strLine, _T('@'));
		if (vecKeyValue.size() != 2)
			continue;

		ar_mapTemplateValue.insert(std::make_pair(vecKeyValue[0], vecKeyValue[1]));
	}
}

void KProjectDBase::UpdateNetworkTemplate( KDBaseConPtr spDBaseConnection, int a_nID, int a_nType, CString a_strValue )
{
	CString strSQL;
	strSQL.Format(_T(" Update NetworkEdit_Template Set key_value='%s' Where ID ='%d' And network_type = '%d' "), a_strValue, a_nID, a_nType);
	try
	{
		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		ThrowException(_T("Error-UpdateNetworkTemplate"));
	} 
}

void KProjectDBase::InsertNetworkTemplate( KDBaseConPtr spDBaseConnection, int a_nType, CString a_strTemplateName, CString a_strValue )
{
	CString strSQL;
	strSQL.Format(_T(" Insert Into NetworkEdit_Template(ID, network_type, template_name, key_value) Values((Select ifnull(Max(ID)+1, 1) From NetworkEdit_Template), '%d', '%s', '%s'); "), a_nType, a_strTemplateName, a_strValue);
	try
	{
		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		ThrowException(_T("Error-InsertNetworkTemplate"));
	} 
}

void KProjectDBase::DeleteNetworkTemplate( KDBaseConPtr spDBaseConnection, int a_nID, int a_nType )
{
	CString strSQL;
	strSQL.Format(_T(" Delete From NetworkEdit_Template Where ID ='%d' And network_type = '%d' "), a_nID, a_nType);
	try
	{
		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		ThrowException(_T("Error-UpdateNetworkTemplate"));
	} 
}


void KProjectDBase::GetNodeRenderInfo( KDBaseConPtr spDBaseConnection, TNodeRenderInfo& ar_oNodeRenderInfo )
{
	try
	{
		int nID(1); { //Flag == 1
            GetUsedNodeTypeDefineID(spDBaseConnection, nID);
        }
		
        std::map<int, TTypeRenderInfo>& mapTypeNodeRender = ar_oNodeRenderInfo.TTypeNodeRenderInfos;
        mapTypeNodeRender.clear();

    	CString strSQL(_T(" Select Symbol, Color, Size From TypeDefineDefault Where Network_Type = '0' "));
        KResultSetPtr   spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

        if ( spResultSet->Next() )
        {
            TTypeRenderInfo& oDefaultNodeRenderInfo = ar_oNodeRenderInfo.TDefaultNodeRenderInfo;
            // 심볼 
            oDefaultNodeRenderInfo.TSymbolType = spResultSet->GetValueInt(0);

            // 색상
            CString strRGB    = spResultSet->GetValueString(1);            
            oDefaultNodeRenderInfo.TColor = GetColorParsing(strRGB);

            // Size
            oDefaultNodeRenderInfo.TSize  = spResultSet->GetValueDouble(2);
        }
        else
        {
            TTypeRenderInfo& oDefaultNodeRenderInfo = ar_oNodeRenderInfo.TDefaultNodeRenderInfo;
            // 심볼 
            oDefaultNodeRenderInfo.TSymbolType = 48;
            // 색상
            int nRValue(217), nGValue(217), nBValue(217);
            oDefaultNodeRenderInfo.TColor = RGB(nRValue, nGValue, nBValue);
            // Size
            oDefaultNodeRenderInfo.TSize  = 4.0;
        }

        strSQL.Format(_T(" Select Node_Type, Symbol, Color, Size, Draw From NodeTypeDefineChild Where ID = '%d' "), nID);
        spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

        while(spResultSet->Next()) {
            // 노드 타입
            int nNodeType = spResultSet->GetValueInt(0);

            TTypeRenderInfo oInfo;
            // 심볼
            oInfo.TSymbolType = spResultSet->GetValueInt(1);
            // 색상
            CString strRGB    = spResultSet->GetValueString(2);    
            oInfo.TColor      = GetColorParsing(strRGB);
            // Size
            oInfo.TSize       = spResultSet->GetValueDouble(3);

            int nDraw         = spResultSet->GetValueInt(4);
            if (nDraw == 0)
                oInfo.TDraw = false;
            else
                oInfo.TDraw = true;

            mapTypeNodeRender.insert(std::make_pair(nNodeType, oInfo));
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

void KProjectDBase::GetLinkRenderInfo( KDBaseConPtr spDBaseConnection, TLinkRenderInfo& ar_oLinkRenderInfo )
{
    try
    {
        int nID(1);
        GetUsedLinkTypeDefineID(spDBaseConnection, nID);
        std::map<int, TTypeRenderInfo>& mapTypeLinkRender = ar_oLinkRenderInfo.TTypeLinkRenderInfos;
        mapTypeLinkRender.clear();

        CString strSQL(_T(" Select Symbol, Color, Size From TypeDefineDefault Where Network_Type = '1' "));
        KResultSetPtr   spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

        if ( spResultSet->Next() )
        {
            TTypeRenderInfo& oDefaultLinkRenderInfo = ar_oLinkRenderInfo.TDefaultLinkRenderInfo;
            // 심볼 
            oDefaultLinkRenderInfo.TSymbolType = spResultSet->GetValueInt(0);

            // 색상
            CString strRGB    = spResultSet->GetValueString(1);            
            oDefaultLinkRenderInfo.TColor = GetColorParsing(strRGB);

            // Size
            oDefaultLinkRenderInfo.TSize  = spResultSet->GetValueDouble(2);
        }
        else
        {
            TTypeRenderInfo& oDefaultLinkRenderInfo = ar_oLinkRenderInfo.TDefaultLinkRenderInfo;
            // 심볼 
            oDefaultLinkRenderInfo.TSymbolType = 0;
            // 색상
            int nRValue(217), nGValue(217), nBValue(217);
            oDefaultLinkRenderInfo.TColor = RGB(nRValue, nGValue, nBValue);
            // Size
            oDefaultLinkRenderInfo.TSize  = 1.0;
        }

        strSQL.Format(_T("Select Link_Type, Symbol, Color, Size, Draw From LinkTypeDefineChild Where ID = '%d'"), nID);
        spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

        while(spResultSet->Next())
        {
            // 링크 타입
            int nLinkType = spResultSet->GetValueInt(0);

            TTypeRenderInfo oInfo;
            // 심볼
            oInfo.TSymbolType = spResultSet->GetValueInt(1);
            // 색상
            CString strRGB    = spResultSet->GetValueString(2);    
            oInfo.TColor      = GetColorParsing(strRGB);
            // Size
            oInfo.TSize       = spResultSet->GetValueDouble(3);

            int nDraw         = spResultSet->GetValueInt(4);
            if (nDraw == 0)
            {
                oInfo.TDraw = false;
            }
            else
            {
                oInfo.TDraw = true;
            }

            mapTypeLinkRender.insert(std::make_pair(nLinkType, oInfo));
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

COLORREF KProjectDBase::GetColorParsing( CString strRGB )
{
    std::vector<CString> vecRGB;
    CString strTok;
    int     nTokenPos = 0; 
    while (AfxExtractSubString(strTok, strRGB, nTokenPos++, ','))
    {
        vecRGB.push_back(strTok.MakeLower());
    }

    int nRValue(217), nGValue(217), nBValue(217);

    if (vecRGB.size() == 3)
    {
        nRValue  =  _ttoi(vecRGB[0]);
        nGValue  =  _ttoi(vecRGB[1]);
        nBValue  =  _ttoi(vecRGB[2]);
    }

    return RGB(nRValue, nGValue, nBValue);
}


void KProjectDBase::GetUsedNodeTypeDefineID(KDBaseConPtr spDBaseConnection, int &a_nDefineTypeID)
{
	CString strSQL(_T(" Select ID, Flag From NodeTypeDefineParent Order By Flag Desc "));
    // Flag 순으로 정렬

	try
	{        
		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() ) 	{
			a_nDefineTypeID = spResultSet->GetValueInt(0);	
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		return;
	} 
}

void KProjectDBase::GetUsedLinkTypeDefineID(KDBaseConPtr spDBaseConnection, int &a_nDefineTypeID)
{
    CString strSQL(_T(" Select ID, Flag From LinkTypeDefineParent Order By Flag Desc "));

    try
    {        
        KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        if ( spResultSet->Next() )
        {
            a_nDefineTypeID = spResultSet->GetValueInt(0);
            //int nFlag       = pResultSet->GetValueInt(1);	
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        return;
    }
    catch (...)
    {
        TxLogDebugWarning();
        return;
    } 
}


void KProjectDBase::GetNodeTypeDefine(KDBaseConPtr spDBaseConnection, int a_nDefineTypeID, std::map<int, TNodeTypeDefine> &a_mapTNodeTypeDefine)
{
	a_mapTNodeTypeDefine.clear();

	CString strSQL;
	strSQL.Format(
		_T(" Select ID, node_type, Symbol, Color, Size, Draw From NodeTypeDefineChild Where ID = %d "), a_nDefineTypeID);

	try
	{        
		KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TNodeTypeDefine oTNodeTypeDefine;    

			oTNodeTypeDefine.nID              = spResultSet->GetValueInt   (0);
			oTNodeTypeDefine.nNodeType        = spResultSet->GetValueInt   (1);
			oTNodeTypeDefine.nCharacterIndex  = spResultSet->GetValueInt   (2);
			CString strColor                  = spResultSet->GetValueString(3);
			ConvertStringToColor(strColor, oTNodeTypeDefine.clrNode);
			oTNodeTypeDefine.dSize            = spResultSet->GetValueDouble(4);

            int nDraw = spResultSet->GetValueInt(5);
            if (nDraw == 0) {
                oTNodeTypeDefine.bDraw = false;
            } else {
                oTNodeTypeDefine.bDraw = true;
            }

			a_mapTNodeTypeDefine.insert(std::make_pair(oTNodeTypeDefine.nNodeType, oTNodeTypeDefine));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		return;
	} 
}


void KProjectDBase::GetDefaultNodeTypeDefine(KDBaseConPtr spDBaseConnection, TNodeTypeDefine &a_oTDefaultNodeTypeDefine)
{
	CString strSQL;
	strSQL.Format(
		_T(" SELECT Symbol, Color, Size FROM TypeDefineDefault WHERE Network_Type = 0 "));

	try
	{        
		KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			a_oTDefaultNodeTypeDefine.nCharacterIndex  = spResultSet->GetValueInt   (0);
			CString strColor                           = spResultSet->GetValueString(1);
			ConvertStringToColor(strColor, a_oTDefaultNodeTypeDefine.clrNode);
			a_oTDefaultNodeTypeDefine.dSize            = spResultSet->GetValueDouble(2);

			a_oTDefaultNodeTypeDefine.nID              = -9;
			a_oTDefaultNodeTypeDefine.nNodeType        = -9;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		return;
	} 
}


void KProjectDBase::ConvertStringToColor(CString strColor, COLORREF &a_clrNode)
{
    try
    {
        std::vector<int> vecTempRGB;
        int     nCount  = 0;
        int     nPos    = 0;
        CString strTemp = strColor;
        strTemp.Trim();
        CString strPart = strTemp.Tokenize( _T(","), nPos );
        while( strPart != _T(""))
        {
            int nColor = _ttoi(strPart);
            vecTempRGB.push_back(nColor);

            strPart = strColor.Tokenize(  _T(","), nPos );
        }

        std::vector<int> vecRGB;
        for (size_t i= 0; i< 3; i++)
        {
            if (i< vecTempRGB.size())
            {
                vecRGB.push_back(vecTempRGB[i]);
            }
            else
            {
                vecRGB.push_back(0);
            }
        }

        a_clrNode = RGB(vecRGB[0], vecRGB[1], vecRGB[2]);
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


void KProjectDBase::NodeTemplateInfo( KDBaseConPtr spDBaseConnection, std::vector<TNodeTemplate>& ar_vecTemplateInfo )
{
	ar_vecTemplateInfo.clear();

	CString strSQL(_T("Select id, name, flag from NodeTypeDefineParent order by id"));

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResultSet->Next())
		{
			TNodeTemplate oNodeTemplate = {};
			oNodeTemplate.TID   = spResultSet->GetValueInt   (0);
			oNodeTemplate.TName = spResultSet->GetValueString(1);
            oNodeTemplate.TFlag = spResultSet->GetValueInt   (2);
			ar_vecTemplateInfo.push_back(oNodeTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("unknown exception"));
	}
}

void KProjectDBase::NodeTemplateChildInfo( KDBaseConPtr spDBaseConnection, int a_nIDTemplate, std::map<int, TNodeTypeDefine>& ar_mapTemplateType )
{
	ar_mapTemplateType.clear();

	CString strSQL;
	strSQL.Format(_T("select id, node_type, symbol, color, size, draw from nodetypedefinechild where id = %d order by node_type"), a_nIDTemplate);

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResultSet->Next())
		{

			TNodeTypeDefine oInfo = {};

			oInfo.nID             = spResultSet->GetValueInt   (0);
			oInfo.nNodeType       = spResultSet->GetValueInt   (1);
			oInfo.nCharacterIndex = spResultSet->GetValueInt   (2);
			CString strColor      = spResultSet->GetValueString(3);
			ConvertStringToColor(strColor, oInfo.clrNode);
			oInfo.dSize = spResultSet->GetValueDouble(4);
            int nDraw   = spResultSet->GetValueInt   (5);
            if (nDraw == 0)
            {
                oInfo.bDraw = false;
            }
            else
            {
                oInfo.bDraw = true;
            }

			ar_mapTemplateType.insert(std::make_pair(oInfo.nNodeType, oInfo));
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		ThrowException(_T("unknown exception"));
	}
}


void KProjectDBase::LinkTemplateInfo( KDBaseConPtr spDBaseConnection, std::vector<TLinkTemplate>& ar_vecTemplateInfo )
{
	ar_vecTemplateInfo.clear();

	CString strSQL(_T("select id, name, flag from LinkTypeDefineParent order by id"));

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResultSet->Next())
		{
			TLinkTemplate oLinkTemplate = {};
			oLinkTemplate.TID   = spResultSet->GetValueInt   (0);
			oLinkTemplate.TName = spResultSet->GetValueString(1);
            oLinkTemplate.TFlag = spResultSet->GetValueInt   (2);
			ar_vecTemplateInfo.push_back(oLinkTemplate);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("unknown exception"));
	}

}

void KProjectDBase::LinkTemplateChildInfo( KDBaseConPtr spDBaseConnection, 
	int a_nIDTemplate, std::map<int, TLinkTypeDefine>& ar_mapTemplateType )
{
	ar_mapTemplateType.clear();

	CString strSQL;
	strSQL.Format(_T("select id, link_type, symbol, color, size, draw from linktypedefinechild where id = %d order by link_type"), a_nIDTemplate);

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResultSet->Next())
		{

			TLinkTypeDefine oInfo = {};

            oInfo.nID          = spResultSet->GetValueInt(0);
            oInfo.nLinkType    = spResultSet->GetValueInt(1);
            oInfo.nSymbolIndex = spResultSet->GetValueInt(2);

            CString strColor   = spResultSet->GetValueString(3);
			ConvertStringToColor(strColor, oInfo.clrLink);
			oInfo.dSize        = spResultSet->GetValueDouble(4);

            int nDraw          = spResultSet->GetValueInt(5);
            if (nDraw == 0)
            {
                oInfo.bDraw = false;
            }
            else
            {
                oInfo.bDraw = true;
            }
			ar_mapTemplateType.insert(std::make_pair(oInfo.nLinkType, oInfo));
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		ThrowException(_T("unknown exception"));
	}
}

void KProjectDBase::LinkTemplateDefaultInfo( KDBaseConPtr spDBaseConnection, TLinkTypeDefine& ar_oTemplateType )
{
	CString strSQL(_T("select symbol, color, size from typedefinedefault where network_type = 1"));

	try
	{        
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		if (spResultSet->Next())
		{
			ar_oTemplateType.nSymbolIndex = spResultSet->GetValueInt(0);
			CString strColor = spResultSet->GetValueString(1);
			ConvertStringToColor(strColor, ar_oTemplateType.clrLink);
			ar_oTemplateType.dSize = spResultSet->GetValueDouble(2);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugWarning();
		return;
	} 
}

void KProjectDBase::UpdateTemplateNodeChildInfo( KDBaseConPtr spDBaseConnection, std::vector<TNodeTypeDefine>& ar_vecTemplateType )
{
	CString strSQL(_T("insert or replace into nodetypedefinechild(id, node_type, symbol, color, size, draw) values( ?, ?, ?, ?, ?, ?)"));

	try
	{
		KPreparedStatementPtr spPreparedStatementPtr = spDBaseConnection->PrepareStatement(strSQL);

		int nCntTemplate = ar_vecTemplateType.size();

		CString strRGB;

		for (int i = 0; i < nCntTemplate; ++i)
		{
			TNodeTypeDefine oNodeTypeDefine = ar_vecTemplateType[i];

			strRGB.Format( _T("%d,%d,%d"), 
				GetRValue(oNodeTypeDefine.clrNode), GetGValue(oNodeTypeDefine.clrNode), GetBValue(oNodeTypeDefine.clrNode));

            spPreparedStatementPtr->BindInt   (1, oNodeTypeDefine.nID);
            spPreparedStatementPtr->BindInt   (2, oNodeTypeDefine.nNodeType);
            spPreparedStatementPtr->BindInt   (3, oNodeTypeDefine.nCharacterIndex);
            spPreparedStatementPtr->BindText  (4, strRGB);
            spPreparedStatementPtr->BindDouble(5, oNodeTypeDefine.dSize);

            if (oNodeTypeDefine.bDraw == true)
            {
                spPreparedStatementPtr->BindInt(6, 1);
            }
            else
            {
                spPreparedStatementPtr->BindInt(6, 0);
            }

			spPreparedStatementPtr->ExecuteUpdate();
			spPreparedStatementPtr->Reset();
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

void KProjectDBase::UpdateTemplateLinkChildInfo( KDBaseConPtr spDBaseConnection, std::vector<TLinkTypeDefine>& ar_vecTemplateType )
{
	CString strSQL(_T("insert or replace into linktypedefinechild(id, link_type, symbol, color, size, draw) values( ?, ?, 0, ?, ?, ?)"));

	try
	{
		KPreparedStatementPtr spPreparedStatementPtr = spDBaseConnection->PrepareStatement(strSQL);

		int nCntTemplate = ar_vecTemplateType.size();

		CString strRGB;

		for (int i = 0; i < nCntTemplate; ++i)
		{
			TLinkTypeDefine oLinkTypeDefine = ar_vecTemplateType[i];

			strRGB.Format( _T("%d,%d,%d"), 
				GetRValue(oLinkTypeDefine.clrLink), GetGValue(oLinkTypeDefine.clrLink), GetBValue(oLinkTypeDefine.clrLink));

			spPreparedStatementPtr->BindInt   (1, oLinkTypeDefine.nID);
			spPreparedStatementPtr->BindInt   (2, oLinkTypeDefine.nLinkType);
			spPreparedStatementPtr->BindText  (3, strRGB);
			spPreparedStatementPtr->BindDouble(4, oLinkTypeDefine.dSize);

            if (oLinkTypeDefine.bDraw == true)
            {
                spPreparedStatementPtr->BindInt(5, 1);
            }
            else
            {
                spPreparedStatementPtr->BindInt(5, 0);
            }

			spPreparedStatementPtr->ExecuteUpdate();
			spPreparedStatementPtr->Reset();
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

void KProjectDBase::SetDefaultLinkParent( KDBaseConPtr spDBaseConnection, int a_nID )
{
    try
    {
        CString strSQL(_T("Update LinkTypeDefineParent Set Flag = '0' "));
        spDBaseConnection->ExecuteUpdate(strSQL);

        strSQL.Format(_T("Update LinkTypeDefineParent Set Flag = '1' Where ID = '%d' "), a_nID);
        spDBaseConnection->ExecuteUpdate(strSQL);
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

void KProjectDBase::SetDefaultNodeParent( KDBaseConPtr spDBaseConnection, int a_nID )
{
    try
    {
        CString strSQL(_T("Update NodeTypeDefineParent Set Flag = '0' "));
        spDBaseConnection->ExecuteUpdate(strSQL);

        strSQL.Format(_T("Update NodeTypeDefineParent Set Flag = '1' Where ID = '%d' "), a_nID);
        spDBaseConnection->ExecuteUpdate(strSQL);
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