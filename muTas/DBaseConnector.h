#pragma once

class KDBase
{
public:
	 KDBase() {};
	~KDBase() {};
public:
	static KDBaseConPtr GetApplicationDBConnection();
	static KDBaseConPtr GetProjectDBConnection();
	static KDBaseConPtr GetSubODDBConnection(CString strDBFilePath);
};