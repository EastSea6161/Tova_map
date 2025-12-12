/**
 * @file ModeManager.h
 * @brief KModeManager 선언 파일
 * @author 
 * @date 2011.06.03
 * @remark
 */


#pragma once


/* Forward declarations */
class KMode;


/**
 * @brief 수단정보를 관리하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.03
 */
class KModeManager
{
public:
	KModeManager ();
	~KModeManager();

public :
	void LoadMode(KDBaseConPtr spDBConnection);
private:
	void Clear();

public:
	int GetModeCount();
	KMode* GetMode(int nIndex);

protected :
	std::vector<KMode*> m_vecMode;
};

