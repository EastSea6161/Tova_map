#pragma once

class KTarget;

class KEditAccessLineStation
{
public:
	KEditAccessLineStation(KTarget* a_pTarget, Integer a_nxZoneID, Integer a_nxLineID);
	~KEditAccessLineStation(void);

public:
	void           DeleteAccessLineStation();

protected:
	void           DeleteRelationFile();
	CString        ConvertSavePath(CString a_strFileName);

protected:
	KTarget*       m_pTarget;
	Integer        m_nxZoneID;
	Integer        m_nxLineID;
};

