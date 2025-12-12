#pragma once

class KTarget;
class KGenerationModelInfo;
// KGenerationUnitPage 대화 상자입니다.

class KGenerationUnitPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGenerationUnitPage)

public:
	KGenerationUnitPage(KTarget* a_pTarget, KGenerationModelInfo& a_oGenerationModeInfo);
	virtual ~KGenerationUnitPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6005_GSimpleUnitPage };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()


protected:
	KTarget*				m_pTarget;
	KGenerationModelInfo&	m_oGenerationModeIinfo;

protected:
	virtual BOOL OnInitDialog();

};
