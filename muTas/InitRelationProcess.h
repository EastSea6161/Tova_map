#pragma once

class KTarget;

class KInitRelationProcess
{
public:
	KInitRelationProcess(void);
	~KInitRelationProcess(void);

private:
	static void DeletePath( CString strPath );

public:
	static void DBRecordNotify         (KTarget* a_pTarget, CString a_strTableName);
	static void DBFieldNotify          (KTarget* a_pTarget, CString a_strTableName);

	static void NetworkTypeDefine      (KTarget* a_pTarget);
	static void UrbanTransitModeDefine (KTarget* a_pTarget);
	static void RegionTransitModeDefine(KTarget* a_pTarget);

private:
	static void HighwayAssignment      (KTarget* a_pTarget);
	static void AccessNodeGenerator    (KTarget* a_pTarget);
	static void TransitPathGenerator   (KTarget* a_pTarget);
	static void MultiModalAssign       (KTarget* a_pTarget);
	static void TargetRegionSet        (KTarget* a_pTarget);
	static void TargetRegionGlobalTerminalSet(KTarget* a_pTarget);
	static void AccessTerminalGenerator(KTarget* a_pTarget);
	static void InterModalPathGenerator(KTarget* a_pTarget);
	static void InterModalAssign       (KTarget* a_pTarget);

public:
	static void DelGroupGeneration     (KTarget* a_pTarget, CString a_strTableName, CString a_strColumnName);
};

