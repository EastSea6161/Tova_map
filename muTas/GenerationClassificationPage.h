#pragma once

class KTarget;
class KGenerationModelInfo;

// KGenerationClassificationPage 대화 상자입니다.

class KGenerationClassificationPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGenerationClassificationPage)

public:
	KGenerationClassificationPage(KTarget* a_pTarget, KGenerationModelInfo& a_oGenerationModelInfo);
	virtual ~KGenerationClassificationPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6005_GSimpleClassificationPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


protected:
	KTarget*				m_pTarget;
	KGenerationModelInfo&	m_oGenerationModelInfo;
};
