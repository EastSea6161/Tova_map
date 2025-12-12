#pragma once
#include "KBulkDBase.h"
#include "MapViewRenderInfo.h"

struct TNodeTemplate
{
	int     TID;
	int     TType;
	CString TName;
    int     TFlag;
};

struct TNodeTemplateType
{
	int     TID;
	int     TSeq;
	int     TSymbol;
	double  TSize;
	int     TR;
	int     TG;
	int     TB;
};

struct TNodeTemplateValue
{
	int     TID;
	int     TSeq;
	CString TMin;
	CString TMax;
	int     TSymbol;
	double  TSize;
	int     TR;
	int     TG;
	int     TB;
};

struct TLinkTemplate
{
	int     TID;
	int     TType;
	CString TName;
    int     TFlag;
};

struct TLinkTemplateType
{
	int     TID;
	int     TSeq;
	int     TLineType;
	double  TWidth;
	int     TR;
	int     TG;
	int     TB;
};

struct TLinkTemplateValue
{
	int     TID;
	int     TSeq;
	CString TMin;
	CString TMax;
	int     TLineType;
	double  TWidth;
	int     TR;
	int     TG;
	int     TB;
};

struct TZoneTemplate
{
	int     TID;
	int     TType;
	CString TName;
};

struct TZoneTemplateType
{
	int     TID;
	int     TSeq;
	int     TR;
	int     TG;
	int     TB;
};

struct TZoneTemplateValue
{
	int     TID;
	int     TSeq;
	CString TMin;
	CString TMax;
	int     TR;
	int     TG;
	int     TB;
};

struct TNetworkTemplate
{
	int     TID;
	CString TName;
};

struct TFormulaMaster
{
	int     TCode;
	CString TName;
};

struct TImpedanceFunction
{
	int     nFunctionType;
	CString strFunctionName;
	double  dAlpha;
	double  dBeta;
	double  dTheta;
	CString strVarRij;
};

struct TNodeTypeDefine
{
	int      nID;
	int      nNodeType;
	int      nCharacterIndex;
	COLORREF clrNode;
	double   dSize;
    bool     bDraw;
};

struct TLinkTypeDefine
{
	int      nID;
	int      nLinkType;
	int      nSymbolIndex;
	COLORREF clrLink;
	double   dSize;
    bool     bDraw;
};

struct TGenerationUnitFunction
{
	double   dValue;
	CString  strObject;
	bool     bResult;
};

struct TGenerationRegressFunction
{
	CString  strExpression;
	bool    bResult;
};
// 
// struct TCargoVolumeFormula
// {
// 	int    nServiceType;	//0 - 비사업, 1 - 사업
// 	int    nVehicleType;	//0 - 소형, 1 - 중형, 2 - 대형
// 	double dAlpha;
// 	double dBeta;
// 	double dWeight;
// };

class KImTasDBase
{
public:
	KImTasDBase();
	~KImTasDBase();
public:
// project.db로 이관
// 	static void NodeTemplate       (KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TNodeTemplate>&      ar_vecTemplate);
// 	static void NodeTemplateType   (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TNodeTemplateType>&  ar_vecTemplateType);
// 	static void NodeTemplateValue  (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TNodeTemplateValue>& ar_vecTemplateValue);
// 
// 	static void LinkTemplate       (KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TLinkTemplate>&      ar_vecTemplate);
// 	static void LinkTemplateType   (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TLinkTemplateType>&  ar_vecTemplateType);
// 	static void LinkTemplateValue  (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TLinkTemplateValue>& ar_vecTemplateValue);
// 
// 	static void ZoneTemplate       (KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TZoneTemplate>&      ar_vecTemplate);
// 	static void ZoneTemplateType   (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TZoneTemplateType>&  ar_vecTemplateType);
// 	static void ZoneTemplateValue  (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TZoneTemplateValue>& ar_vecTemplateValue);
// 
// 	static int  NewNodeTemplateID  (KDBaseConPtr spDBaseConnection, int a_nType);
// 	static int  NewLinkTemplateID  (KDBaseConPtr spDBaseConnection, int a_nType);
// 	static int  NewZoneTemplateID  (KDBaseConPtr spDBaseConnection, int a_nType);
// 
// 	static void NetwrokTemplate    (KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TNetworkTemplate>& ar_vecNetworkTemplate);
// 	static void NetwrokTemplate    (KDBaseConPtr spDBaseConnection, int a_nID, int a_nType, std::map<CString, CString>& ar_mapTemplateValue);
// 	static void UpdateNetworkTemplate (KDBaseConPtr spDBaseConnection, int a_nID, int a_nType, CString a_strValue);
// 	static void InsertNetworkTemplate (KDBaseConPtr spDBaseConnection, int a_nType, CString a_strTemplateName, CString a_strValue);
// 	static void DeleteNetworkTemplate (KDBaseConPtr spDBaseConnection, int a_nID, int a_nType);

	static void GenerationFormula  (KDBaseConPtr spDBaseConnection, KEMPurposeGroup a_emPurposeGroup, int a_nMaterCode, std::map<int, std::map<int, std::vector<CString>>> &a_mapFormula);
	static void FormulaMasterData  (KDBaseConPtr spDBaseConnection, std::vector<TFormulaMaster> &a_vecTFormulaMaster);
	static void DistributionFormula(KDBaseConPtr spDBaseConnection, KEMPurposeGroup a_emPurposeGroup, int a_nMasterCode, std::map<int, std::vector<TImpedanceFunction>> &a_mapImpedanceFunction);
	static void DistributionFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, int a_nObjectGroupCode, int a_nDetailObejctID, TImpedanceFunction &a_oTImpedanceFunction );
	static void GenerationUnitFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, int a_nObjectGroupCode, int a_nDetailObejctID, CString a_strPAFlag, TGenerationUnitFunction &a_oTGenerationUnitFunction );
	static void GenerationRegressFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, int a_nObjectGroupCode, int a_nDetailObejctID, CString a_strPAFlag, TGenerationRegressFunction &a_oTGenerationRegressFunction );
	static void ModeChoiceFormula  (KDBaseConPtr spDBaseConnection, int a_nMasterCode, KEMPurposeGroup a_emPurposeGroup, KEMModeGroup a_emModeGroup, std::map<int, std::map<int, CString>> &a_mapFunctionStr);
	static void ModeChoiceShotLongFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, KEMPurposeGroup a_emPurposeGroup, std::map<int, double> &a_mapDistance );
	static void FLogisticModelFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, KEMPurposeGroup a_emPurposeGroup, std::map<int, double> &a_mapRatio );
	//static void VehicleConvertFormula( KDBaseConPtr spDBaseConnection, int a_nMasterCode, KEMPurposeGroup a_emPurposeGroup, int a_nDetailObejctID, std::vector<TVehicleConvert> &a_vecData );

// project.db로 이관
//     static void GetNodeRenderInfo  (KDBaseConPtr spDBaseConnection, TNodeRenderInfo& ar_oNodeRenderInfo);
//     static void GetLinkRenderInfo  (KDBaseConPtr spDBaseConnection, TLinkRenderInfo& ar_oLinkRenderInfo);
//     static void SetDefaultLinkParent(KDBaseConPtr spDBaseConnection, int a_nID);
//     static void SetDefaultNodeParent(KDBaseConPtr spDBaseConnection, int a_nID);
// private:
//     static COLORREF GetColorParsing(CString strRGB);
// public:
// 	static void GetUsedNodeTypeDefineID (KDBaseConPtr spDBaseConnection, int &a_nDefineTypeID);
//     static void GetUsedLinkTypeDefineID (KDBaseConPtr spDBaseConnection, int &a_nDefineTypeID);
// 	static void GetNodeTypeDefine       (KDBaseConPtr spDBaseConnection, int a_nDefineTypeID, std::map<int, TNodeTypeDefine> &a_mapTNodeTypeDefine);
// 	static void GetDefaultNodeTypeDefine(KDBaseConPtr spDBaseConnection, TNodeTypeDefine &a_oTNodeTypeDefine);
// 
// 	static void NodeTemplateInfo(KDBaseConPtr spDBaseConnection, std::vector<TNodeTemplate>& ar_vecTemplateInfo);
// 	static void NodeTemplateChildInfo(KDBaseConPtr spDBaseConnection, int a_nIDTemplate, std::map<int, TNodeTypeDefine>& ar_mapTemplateType);
// 
// 	static void LinkTemplateInfo(KDBaseConPtr spDBaseConnection, std::vector<TLinkTemplate>& ar_vecTemplateInfo);
// 	static void LinkTemplateChildInfo(KDBaseConPtr spDBaseConnection, int a_nIDTemplate, std::map<int, TLinkTypeDefine>& ar_mapTemplateType);
// 	static void LinkTemplateDefaultInfo(KDBaseConPtr spDBaseConnection, TLinkTypeDefine& ar_oTemplateType);
// 
// 	static void UpdateTemplateNodeChildInfo(KDBaseConPtr spDBaseConnection, std::vector<TNodeTypeDefine>& ar_vecTemplateType);
// 	static void UpdateTemplateLinkChildInfo(KDBaseConPtr spDBaseConnection, std::vector<TLinkTypeDefine>& ar_vecTemplateType);
// 	
// 
// private:
// 	static void ConvertStringToColor(CString strColor, COLORREF &a_clrNode);
	
};


class KProjectDBase
{
public:
	KProjectDBase();
	~KProjectDBase();
public:
	static void NodeTemplate       (KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TNodeTemplate>&      ar_vecTemplate);
	static void NodeTemplateType   (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TNodeTemplateType>&  ar_vecTemplateType);
	static void NodeTemplateValue  (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TNodeTemplateValue>& ar_vecTemplateValue);

	static void LinkTemplate       (KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TLinkTemplate>&      ar_vecTemplate);
	static void LinkTemplateType   (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TLinkTemplateType>&  ar_vecTemplateType);
	static void LinkTemplateValue  (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TLinkTemplateValue>& ar_vecTemplateValue);

	static void ZoneTemplate       (KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TZoneTemplate>&      ar_vecTemplate);
	static void ZoneTemplateType   (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TZoneTemplateType>&  ar_vecTemplateType);
	static void ZoneTemplateValue  (KDBaseConPtr spDBaseConnection, int a_nID,   std::vector<TZoneTemplateValue>& ar_vecTemplateValue);

	static int  NewNodeTemplateID  (KDBaseConPtr spDBaseConnection, int a_nType);
	static int  NewLinkTemplateID  (KDBaseConPtr spDBaseConnection, int a_nType);
	static int  NewZoneTemplateID  (KDBaseConPtr spDBaseConnection, int a_nType);

	static void NetwrokTemplate    (KDBaseConPtr spDBaseConnection, int a_nType, std::vector<TNetworkTemplate>& ar_vecNetworkTemplate);
	static void NetwrokTemplate    (KDBaseConPtr spDBaseConnection, int a_nID, int a_nType, std::map<CString, CString>& ar_mapTemplateValue);
	static void UpdateNetworkTemplate (KDBaseConPtr spDBaseConnection, int a_nID, int a_nType, CString a_strValue);
	static void InsertNetworkTemplate (KDBaseConPtr spDBaseConnection, int a_nType, CString a_strTemplateName, CString a_strValue);
	static void DeleteNetworkTemplate (KDBaseConPtr spDBaseConnection, int a_nID, int a_nType);

	static void GetNodeRenderInfo  (KDBaseConPtr spDBaseConnection, TNodeRenderInfo& ar_oNodeRenderInfo);
	static void GetLinkRenderInfo  (KDBaseConPtr spDBaseConnection, TLinkRenderInfo& ar_oLinkRenderInfo);
	static void SetDefaultLinkParent(KDBaseConPtr spDBaseConnection, int a_nID);
	static void SetDefaultNodeParent(KDBaseConPtr spDBaseConnection, int a_nID);
private:
	static COLORREF GetColorParsing(CString strRGB);
public:
	static void GetUsedNodeTypeDefineID (KDBaseConPtr spDBaseConnection, int &a_nDefineTypeID);
	static void GetUsedLinkTypeDefineID (KDBaseConPtr spDBaseConnection, int &a_nDefineTypeID);
	static void GetNodeTypeDefine       (KDBaseConPtr spDBaseConnection, int a_nDefineTypeID, std::map<int, TNodeTypeDefine> &a_mapTNodeTypeDefine);
	static void GetDefaultNodeTypeDefine(KDBaseConPtr spDBaseConnection, TNodeTypeDefine &a_oTNodeTypeDefine);

	static void NodeTemplateInfo(KDBaseConPtr spDBaseConnection, std::vector<TNodeTemplate>& ar_vecTemplateInfo);
	static void NodeTemplateChildInfo(KDBaseConPtr spDBaseConnection, int a_nIDTemplate, std::map<int, TNodeTypeDefine>& ar_mapTemplateType);

	static void LinkTemplateInfo(KDBaseConPtr spDBaseConnection, std::vector<TLinkTemplate>& ar_vecTemplateInfo);
	static void LinkTemplateChildInfo(KDBaseConPtr spDBaseConnection, int a_nIDTemplate, std::map<int, TLinkTypeDefine>& ar_mapTemplateType);
	static void LinkTemplateDefaultInfo(KDBaseConPtr spDBaseConnection, TLinkTypeDefine& ar_oTemplateType);

	static void UpdateTemplateNodeChildInfo(KDBaseConPtr spDBaseConnection, std::vector<TNodeTypeDefine>& ar_vecTemplateType);
	static void UpdateTemplateLinkChildInfo(KDBaseConPtr spDBaseConnection, std::vector<TLinkTypeDefine>& ar_vecTemplateType);


private:
	static void ConvertStringToColor(CString strColor, COLORREF &a_clrNode);

};