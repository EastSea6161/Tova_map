#pragma once
class KDBaseVersion_P017
{
public:
	KDBaseVersion_P017(void);
	~KDBaseVersion_P017(void);
	
	static void IolocalDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableTerminalResult(KDBaseConPtr spDBaseConnection);

	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableFunctionGroup(KDBaseConPtr spDBaseConnection);

		static void TableLinkTemplate(KDBaseConPtr spDBaseConnection);
		static void TableLinkTemplateType(KDBaseConPtr spDBaseConnection);
		static void TableLinkTemplateValue(KDBaseConPtr spDBaseConnection);
		static void TableLinkTypeDefineChild(KDBaseConPtr spDBaseConnection);
		static void TableLinkTypeDefineParent(KDBaseConPtr spDBaseConnection);
		static void TableNavigation(KDBaseConPtr spDBaseConnection);
		static void TableNetworkEditTemplate(KDBaseConPtr spDBaseConnection);
		static void TableNodeTemplate(KDBaseConPtr spDBaseConnection);
		static void TableNodeTemplateType(KDBaseConPtr spDBaseConnection);
		static void TableNodeTemplateValue(KDBaseConPtr spDBaseConnection);
		static void TableNodeTypeDefineChild(KDBaseConPtr spDBaseConnection);
		static void TableNodeTypeDefineParent(KDBaseConPtr spDBaseConnection);
		static void TableTypeDefineDefault(KDBaseConPtr spDBaseConnection);
		static void TableZoneTemplate(KDBaseConPtr spDBaseConnection);
		static void TableZoneTemplateType(KDBaseConPtr spDBaseConnection);
		static void TableZoneTemplateValue(KDBaseConPtr spDBaseConnection);


};

