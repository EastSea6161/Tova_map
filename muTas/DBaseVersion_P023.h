#pragma once
class KDBaseVersion_P023
{
public:
	KDBaseVersion_P023(void);
	~KDBaseVersion_P023(void);

public:
	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection);
	
private:
	static void TableMultiAssignInfo(KDBaseConPtr spDBaseConnection);
	static void TableMultiAssignData(KDBaseConPtr spDBaseConnection);
};

