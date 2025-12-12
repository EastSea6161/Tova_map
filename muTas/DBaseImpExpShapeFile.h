#pragma once

class KTarget;

class KImportShapeMatching
{
public:
	KImportShapeMatching::KImportShapeMatching():
	  pMatColumn(nullptr), nShapeIndex(0), strShapeFieldName(_T(""))
	  {

	  }
	  KImportShapeMatching::~KImportShapeMatching(){}
	  KIOColumn* pMatColumn;
	  long		 nShapeIndex;
	  CString	 strShapeFieldName;
};

class KImportShapeNodeValue
{
public:
	KImportShapeNodeValue::KImportShapeNodeValue():
	nType(1)
	{

	}
	KImportShapeNodeValue::~KImportShapeNodeValue(){}
	std::vector<CString> vecValues;
	int nType;
};

class KImportShapeDefaultZoneDemacation
{
public:
	KImportShapeDefaultZoneDemacation::KImportShapeDefaultZoneDemacation():
	  nxZoneID(0), dx(0), dy(0)
	  {

	  }
	  KImportShapeDefaultZoneDemacation::~KImportShapeDefaultZoneDemacation(){}
	  Integer nxZoneID;
	  double  dx;
	  double  dy;
};

class KImportShapeNodeXYInfo
{
public:
	KImportShapeNodeXYInfo::KImportShapeNodeXYInfo():
	nxNodeID(0), dx(0), dy(0)
	{
		
	}
	KImportShapeNodeXYInfo::~KImportShapeNodeXYInfo(){}

	Integer nxNodeID;
	double dx;
	double dy;

};

class KDBaseImpExpShapeFile
{
public:
	KDBaseImpExpShapeFile(void);
	~KDBaseImpExpShapeFile(void);

	static bool IsExistNodeIDAndGetXY(KDBaseConPtr spDBaseConnection, KImportShapeNodeXYInfo& ar_NodeInfo);
};

