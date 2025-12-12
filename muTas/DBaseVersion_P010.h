#pragma once
class KDBaseVersion_P010
{
public:
	KDBaseVersion_P010(void);
	~KDBaseVersion_P010(void);

public:
	static void Upgrade(KDBaseConPtr spDBaseConnection);

	static void GroupGeneration(KDBaseConPtr spDBaseConnection);
};


