/**
 * @file model_common.h
 * @brief model이 공통 type 정의 파일
 * @author 
 * @date 2011.04.21
 * @remark
 */

#pragma once


#include "DefineNames.h"

/* Production / Attraction 구분 */
enum KEMPAType
{
	KEMPATypeProduction = 1,
	KEMPATypeAttraction = 2
};

#define PRODUCTION_STRING	_T("Production")
#define ATTRACTION_STRING	_T("Attraction")
#define CONSUMPTION_STRING  _T("Consumption")

#define GENERATION_PRODUCTION _T("p")


/* 발생 모형의 종류 */ 
enum KEMGenerationModel
{
	KEMGenerationNone = 0,
	KEMGenerationUnit = 1,
	KEMGenerationRegression = 2,
	KEMGenerationCrossClassification = 3,
    KEMGenerationIRO   = 4
};

#define GENERATION_UNIT_STRING	               _T("Unit")
#define GENERATION_REGRESSION_STRING	       _T("Regression")
#define GENERATION_CROSSCLASSIFICATION_STRING  _T("CrossClassification")
#define GENERATION_IRIO_STRING				   _T("IRIO")


///* 모형식 Zone group 종류 */
//enum KEMExpressionType
//{
//	KEMExpressionTypeUnit	= 1,
//	KEMExpressionTypeRegression = 2,
//	KEMExpressionTypeCrossClassification = 3
//};


///* 모형식의 Zone group node의 종류 */
//enum KEMZoneGroupNodeType
//{
//	KEMZoneGroupNodeTypeColumn = 1,
//	KEMZoneGroupNodeTypeValue
//};


/* 모형식 수식의 구성 요소 */
enum KEMExpressionElementType
{
	KEMExpressionElementNumber = 1,
	KEMExpressionElementOperator = 2,
	KEMExpressionElementFunction = 3,
	KEMExpressionElementField = 4
};


/*  발생 모형의 Expression container의 종류 */
enum KEMGExpressionContainerType
{
	KEMGExpressionContainerSimple = 1,
	KEMGExpressionContainerZoneGroup = 2
};


/* Zone group object의 종류 */
enum KEMZoneGroupObject
{
	KEMZoneGroupColumn = 1,
	KEMZoneGroupValue = 2
};


/* Purpose의 Group 종류 */
enum KEMPurposeGroup
{
	KEMPurposeGroupODBase = 11,
	KEMPurposeGroupHomeBase = 21,
	KEMPurposeGroupNoneHomeBase = 22,
	KEMPurposeGroupFreightOD = 61
};

#define PurposeGroupODBase_NAME	   _T("O/D Base")
#define PurposeGroupFreightOD_NAME _T("O/D Base")


/* Mode의 Group 종류 */
enum KEMModeGroup
{
	KEMModeGroupODBase	= 11,
	KEMModeGroupHomeBase = 21,
	KEMModeGroupNoneHomeBase = 22,
	KEMModeGroupFreightOD	= 61
};

#define ModeGroupODBase_NAME	_T("OD")
#define ModeGroupFreightOD_NAME _T("화물수단")


/* Distribution Model type */
enum KEMDistributionModel
{
	KEMDistributionModelFratar = 1,
	KEMDistributionModelGravity = 2
};


/* Distribution Model OD type */
enum KEMDODType
{
	KEMDODTypePurpose = 1,
	KEMDODTypeMode = 2
};


/* Distribution Model Impedance Variable type */
enum KEMImpedanceVariableType
{
	KEMImpedanceVariableTypeTime = 1,
	KEMImpedanceVariableTypeLength = 2,
	KEMImpedanceVariableTypeCost = 3
};


/* Distribution model function type */
enum KEMDistributionFunction
{
	KEMDistributionFunction1 = 1,
	KEMDistributionFunction2 = 2,
	KEMDistributionFunction3 = 3
};


/* Mode Choice model */
enum KEMModeChoiceModel
{
	KEMModeChoiceNotNestedLogit = 1,
	KEMModeChoiceNestedLogit = 2,
};


/* Mode choice model의 tree node type */
enum KEMModeTreeNode
{
	KEMModeTreeNodeMode	= 1,	// Mode node
	KEMModeTreeNodeGroup = 2,	// group node
};


/* Mode choice model의 variable type */
enum KEMModeVariableType
{
	KEMModeVariableTypeSimple = 1,
	KEMModeVariableTypeGroup = 2
};


/* Assignment model */
enum KEMAssignmentModel
{
	KEMAssignmentModelAllorNothing = 1,
	KEMAssignmentModelIncremental = 2,
	KEMAssignmentModelUserEquilibriumFW = 3,
	KEMAssignmentModelUserEquilibriumOB = 4,
	KEMAssignmentModelMultiModal = 5,
	KEMAssignmentModelInterModal = 6,
	KEMAssignmentModelSegment = 7,
	KEMAssignmentModelUserEquilibriumOBMulti = 9
};


/* Assignment model OD type */
enum KEMAssignmentODType
{
	KEMAssignmentODDay = 1,
	KEMAssignmentODTime = 2
};

enum KEMAssignmentBPRFunctionType
{
	KEMAssignmentBPRFunctionAllLink		= 1,
	KEMAssignmentBPRFunctionEachFn		= 2,
	KEMAssignmentBPRFunctionEachLink	= 3
};


/* Assignment model function type */
enum KEMAssignmentFunction
{
	KEMAssignmentFunctionBPR = 1,
	KEMAssignmentFunctionUser = 2	
};

/* Assignment output table type */
enum KEMAssignmentOutputTableType
{
	KEMAssignmentOutputTableLink = 1,
	KEMAssignmentOutputTableOD = 2 
};


//Sed 데이터 유무확인 
enum KEMGHaveDBMode
{
	KEMHaveSed_All = 0,
	KEMHaveSed_OnlyPop = 1,
	KEMHaveSed_OnlyVector = 2,
	KEMHaveSed_None = 3

};

enum KEMInterModalModel
{
	KEMInterModalModelDefault = 1
};


/* InterModal model function type */
enum KEMInterModalFunction
{
	KEMInterModalFunctionBPR = 1,
	KEMInterModalFunctionUser = 2	
};



enum KEMGravityFunction
{
	KEMIntervalRatio       = 1,
	KEMImpedanceFunction   = 2,
	KEMEntropyMaximization = 3
};


/**
 * @brief PurposeGroupCode에 대한 Group Code Name을 반환 한다.
 * @param[in] a_emPurposeGroup : 열거체 형 purpose group Code
 * @return CString : code에 해당 하는 group Code Name
 */
CString GetPurposeGroupName(KEMPurposeGroup& a_emPurposeGroup);


/**
 * @brief ModeGroupCode에 대한 Group Code Name을 반환 한다.
 * @param[in] a_emModeGroup : 열거체 형 mode group Code
 * @return CString : code에 해당 하는 group Code Name
 */
CString GetModeGroupName(KEMModeGroup& a_emModeGroup);

//
///* table name define */
//extern const TCHAR* TABLE_NODE;
//extern const TCHAR* TABLE_LINK;
//extern const TCHAR* TABLE_ZONE;
//extern const TCHAR* TABLE_TURN;
//extern const TCHAR* TABLE_PURPOSE_OD;
//extern const TCHAR* TABLE_PARAMETER_OD;
//extern const TCHAR* TABLE_MODE_OD;
//
///* column name define */
//extern const TCHAR* COLUMN_LINKTYPE;
//extern const TCHAR* COLUMN_OZONE;
//extern const TCHAR* COLUMN_DZONE;

