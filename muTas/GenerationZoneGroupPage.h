#pragma once

class KTarget;
class KGenerationModelInfo;

// KGenerationZoneGroupPage 대화 상자입니다.

class KGenerationZoneGroupPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGenerationZoneGroupPage)

public:
	KGenerationZoneGroupPage(KTarget* a_pTarget, KGenerationModelInfo& a_oGenerationModelInfo);
	virtual ~KGenerationZoneGroupPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6003_GZoneGroupPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*				m_pTarget;
	KGenerationModelInfo&	m_oGenerationModelInfo;
};
