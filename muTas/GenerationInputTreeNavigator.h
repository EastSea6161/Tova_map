#pragma once
class KGenerationModelInfo;

class KGenerationInputTreeNavigator: public CXTPPropertyPageTreeNavigator
{
public:
	KGenerationInputTreeNavigator(void);
	~KGenerationInputTreeNavigator(void);

protected:
	DECLARE_MESSAGE_MAP()

	
protected:
	std::map<int, KGenerationModelInfo> m_mapGenerationInfo;

public:
	void SetGenerationInfo(std::map<int, KGenerationModelInfo> a_mapGenerationInfo);

protected :
	/**
	* @brief PropertyPageNavigator의 Tree를 생성한다.
	* @return BOOL - 생성 결과
	*/
	virtual BOOL CreateTree(void);

public :
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);

};

