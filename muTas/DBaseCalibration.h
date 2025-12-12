#pragma once

/*forward declare*/
class KTarget;


class KDBaseCalibration
{
public:
	KDBaseCalibration(void);
	~KDBaseCalibration(void);

public:
	static int SelectZoneCount(KTarget* a_pTarget);
};

