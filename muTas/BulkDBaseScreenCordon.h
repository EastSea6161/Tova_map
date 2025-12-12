#pragma once

#include "TDrawScreenCordonInfo.h"

class KTarget;
class KIOColumns;
class KSCAnalysisInfo;

class KBulkDBaseScreenCordon
{
public:
    KBulkDBaseScreenCordon (void);
    ~KBulkDBaseScreenCordon(void);

    static bool ScreenQuery(KTarget* a_pTarget, Integer a_nxID,    CString& a_strName, std::vector<TDrawScreenCordonXY>& a_vecPoints);
    static bool CordonQuery(KTarget* a_pTarget, Integer a_nxID,    CString& a_strName, std::vector<TDrawScreenCordonXY>& a_vecPoints);
    
    static void SaveScreen (KTarget* a_pTarget, CString a_strName, std::vector<TDrawScreenCordonXY>& a_vecPoints, Integer& ar_nxID);
    static void SaveCordon (KTarget* a_pTarget, CString a_strName, std::vector<TDrawScreenCordonXY>& a_vecPoints, Integer& ar_nxID);

	static bool DeleteScreenCordon( KTarget* a_pTarget, Integer& a_nxID );

	static bool UpdateScreenMaster( KTarget* a_pTarget, std::vector<TScreenDBInfo>& a_vecScreenInfo );
	static bool UpdateCordonMaster( KTarget* a_pTarget, std::vector<TCordonDBInfo>& a_vecCordonInfo );

	static void LinkFNode( KTarget* a_pTarget, std::map<__int64/*Link_ID*/, __int64/*FNode_ID*/>& a_mapLinkFNode);
	static void LinkSurveyVolume( KTarget* a_pTarget, KIOColumns& a_linkColumnCollection, std::map<__int64, KSDoubleRecord*>& a_mapLinkSurveyVolumn);
	static void ALLNode( KTarget* a_pTarget, std::vector<__int64>& a_vecLink);

	static void ApplyModeChoiceResult( 
		KDBaseConPtr spDBaseConnection, std::list<KSCAnalysisInfo*> &a_lstSCAnlaysisInfo, 
		CString &a_strStartTime, CString &a_strEndTime);
};