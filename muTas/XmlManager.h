#pragma once

#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"

class KXmlManager
{	
public:
	KXmlManager(void);
	~KXmlManager(void);
	
public:
	/**
	* @brief 특정 Path 에 있는 xml 파일을 로드하는 함수
	* @input LPCTSTR(경로명)
	* @input LPCTSTR(파일명_확장자포함)
	* @warning 
	*/
	bool LoadTargetXml(LPCTSTR a_lpcszTargetPath, LPCTSTR a_lpcszFileName);

	/**
	* @brief 특정 Path 에 있는 xml 파일에 저장하는 함수
	* @input LPCTSTR(경로명)
	* @input LPCTSTR(파일명_확장자포함)
	* @warning 
	*/
	bool SaveTargetXml(LPCTSTR a_lpcszTargetPath, LPCTSTR a_lpcszFileName);

	/**
	* @brief 루트 Elemnent로 접근하는 함수
	* @input LPCTSTR 루트 Elemnent 이름
	* @remark 
	* @warning 
	*/
	TiXmlElement* AccessRootElement(LPCTSTR a_strRootName);
	/**
	* @brief 하위 Elemnent로 접근하는 함수
	* @input LPCTSTR 하위 Elemnent 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @remark 
	* @warning 
	*/
	TiXmlElement* AccessElment(LPCTSTR a_strElementName, TiXmlElement* a_pElement);
	/**
	* @brief 현재 Elemnent의 CString형 속성값을 가져오는 함수
	* @input LPCTSTR  속성의 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @remark 
	* @warning 
	*/
	CString GetAttributeStr(LPCTSTR a_strElementName, TiXmlElement* a_pElement);
	/**
	* @brief 현재 Elemnent의 int형 속성값을 가져오는 함수
	* @input LPCTSTR  속성의 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @remark 
	* @warning 
	*/
	int GetAttributeInt(LPCTSTR a_strElementName, TiXmlElement* a_pElement);
	/**
	* @brief 현재 Elemnent의 double형 속성값을 가져오는 함수
	* @input LPCTSTR  속성의 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @remark 
	* @warning 
	*/
	double GetAttributeDouble(LPCTSTR a_strElementName, TiXmlElement* a_pElement);
	/**
	* @brief 현재 Element의 int형 속성에 값을 Insert 하는 함수
	* @input LPCTSTR  속성의 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @input 현재  Element 에 Insert 하려는 int 형 값
	* @remark 
	* @warning 
	*/
	void SetAttributeInt(LPCTSTR a_lpcszAttributeName, TiXmlElement* a_pElement, int a_nVal);
	/**
	* @brief 현재 Element의 double형 속성에 값을 Insert 하는 함수
	* @input LPCTSTR  속성의 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @input 현재  Element 에 Insert 하려는 double 형 값
	* @remark 
	* @warning 
	*/
	void SetAttributeDouble(LPCTSTR a_lpcszAttrbuteName, TiXmlElement* a_pElement, double a_dVal);
	/**
	* @brief 현재 Element의 string형 속성에 값을 Insert 하는 함수
	* @input LPCTSTR  속성의 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @input 현재  Element 에 Insert 하려는 string 형 값
	* @remark 
	* @warning 
	*/
	void SetAttributeString(LPCTSTR a_lpcszAttrbuteName, TiXmlElement* a_pElement, LPCTSTR strVal);
	/**
	* @brief 동등한 위치의 다음 Elemnent로 접근하는 함수
	* @input LPCTSTR  Element의 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @remark 
	* @warning 
	*/
	TiXmlElement* NextElement(LPCTSTR a_strAttributeName, TiXmlElement* a_pElement);
	/**
	* @brief Element 이름을 가져오는 함수
	* @input LPCTSTR  Element의 이름
	* @input TiXmlElement* 현재의 Element 클래스
	* @remark 
	* @warning 
	*/
	CString GetElementName(TiXmlElement* a_pElement);

	/**
	* @brief Mulitybyte를 Unicode로 변경해주는 함수
	* @input Mulitybyte string 값 
	* @Output Unicode CString 
	* @remark 
	* @warning 
	*/
	CString MulitybyteToUnicode(std::string a_strName);
	std::string UnicodeToMulitybyte(CString a_strConvert);

	TiXmlElement* AccessParentElement(TiXmlElement* a_pChildElement);


	void CreateDeclaration(LPCTSTR a_strVersion, LPCTSTR a_strEncode, LPCTSTR a_strStandAlone);	
	TiXmlElement* CreateRootElement(LPCTSTR a_strRootName);
	TiXmlElement* CreateChildElement(LPCTSTR a_strChdName, TiXmlElement* a_pParentElement);	
	void CreateComment(LPCTSTR a_strContents, TiXmlElement* a_pElement);
	TiXmlElement* CheckRootElement(LPCTSTR a_strRootName);	
	TiXmlElement* CheckChildElement(LPCTSTR a_strChdName, TiXmlElement* a_pParentElement);
protected:
	TiXmlDocument* m_pxmlDocument ;
};
