#pragma once
class KCodeGroup;

/**
*@brief 코드 매니저
*@details 코드들의 편집기능 담당 클래스
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KCodeManager
{
public:
	KCodeManager(KDBaseConnectionPtr a_spDBaseConnection);
	KCodeManager(const KCodeManager& r);
	~KCodeManager(void);

public :
	static const CString TABLE_CODEGROUP;
	static const CString TABLE_CODES;

	KCodeManager& operator=(const KCodeManager& r);
private:
    KDBaseConnectionPtr m_spDBaseConnection;
public:
	/**
	*@brief 코드 그룹 불러오기
	*@details KCode_Group테이블 자료 불러오기
	*@return 성공이면 ture, 실패면 false 반환
	*@throws 디버그
	*/
    bool LoadCodeGroups();
public:
	/**
	*@brief 코드 그룹 개수
	*@return 개수 반환
	*/
	int  CodeGroupCount(void);	
	/**
	*@brief 코드 그룹 추가
	*@param int a_nGroupKey				그룹 키
	*@param LPCTSTR a_strCodeGroupName	그룹 명칭
	*@return 성공이면 ture, 실패면 false 반환
	*/
	bool AddCodeGroup(int a_nGroupKey,    LPCTSTR a_strCodeGroupName);
	/**
	*@brief 코드그룹 명칭 수정
	*@details 코드그룹 키를 조건으로 그룹 명칭을 변경
	*@param int a_nGroupKey				그룹 키
	*@param LPCTSTR a_strCodeGroupName	그룹 명칭
	*@param LPCTSTR strNewName			수정할 그룹 명칭
	*@return 성공이면 ture, 실패면 false 반환
	*@throws false 반환 
	*/
	bool RenameCodeGroup(int a_nGroupKey, LPCTSTR strCodeGroupName, LPCTSTR strNewName);
	/**
	*@brief 코드그룹 전체 삭제
	*@details 코드그룹 테이블의 정보를 전체 삭제
	*@throws 디버그
	*/
	void RemoveAllCodeGroup(void);
	/**
	*@brief 코드그룹 초기화
	*@details 코드그룹 전체를 초기화
	*@throws 디버그 
	*/
	void ResetCodeManager(void);	
public:
	/**
	*@brief 코드그룹 검색
	*@details 코드그룹 키로 코드그룹을 검색
	*@param int nKey	그룹 키
	*@return 성공시 코드데이터를, 실패시 NULL를 반환 
	*/
	KCodeGroup* FindCodeGroup(int nKey);
	/**
	*@brief 코드그룹 데이터 넣기
	*@param std::vector<KCodeGroup*>& codeGroups	그룹 넣기
	*@details 맵 형식으로 코드그룹의 데이터를 넣는다
	*/
	void GetCodeGroups(std::vector<KCodeGroup*>& codeGroups/*out*/);
	/**
	*@brief 코드그룹 키 삭제
	*@param int nGroupKey	그룹 키
	*@details 코드 그룹 키를 삭제 하면서 삭제된 코드그룹에 속한 코드정보들도 삭제한다.
	*@throws 디버그 
	*/
	void RemoveCodeGroupKey(int nGroupKey);

private:
	/**
	*@brief 코드 그룹 입력
	*@details 테이블에 그룹키, 그룹명칭, 시스템 편집가능 여부 입력
	*@return 성공이면 ture, 실패면 false 반환
	*@param int a_nGroupKey				그룹 키
	*@param LPCTSTR a_strCodeGroupName	그룹 명칭
	*@throws false 반환
	*/
	bool InsertCodeGroupData(int a_nGroupKey, LPCTSTR a_strCodeGroupName);

protected :
    std::map<int, KCodeGroup*> m_mapCodeGroup;
public:
	/**
	*@brief 다음순서 그룹 키 보기
	*@return 다음순서의 그룹 코드 키를 반환 
	*@throws 디버그
	*/
    int NextGroupKey();
};
