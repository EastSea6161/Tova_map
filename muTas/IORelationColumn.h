/**
 * @file IORelationColumn.h
 * @brief KIORelationColumn 선언 파일
 * @author jyk@qbicware.com
 * @date 2012.05.29
 * @remark
 */


#pragma once

/**
 * @brief Column 정보와 관련 테이블과 관련 Column에 대한 정보를 반환 한다.
 *          insert, update, delete문의 field 목록과 값으로 변환한다.
 * @remark
 * @warning
 * @version 1.0
 * @author jyk@qbicware.com
 * @date 2012.05.29
 */

struct TRelationColumnInfo
{
	CString strTableName;
	CString strColumnName;
};

class KIORelationColumn
{
public:
	KIORelationColumn(LPCTSTR strTableName);
	~KIORelationColumn(void);

public:
	std::vector<TRelationColumnInfo> GetRelationInfo();

private:
	void InitPassengerRelationInfo();
	void InitFreightRelationInfo();
	void RegisterRelationInfo( CString a_strTableName, CString a_strRelationTableName, CString a_strRelationcolumnName );

private:
	CString m_strTableName;

	std::map< CString, std::vector<TRelationColumnInfo> > m_mapRelationColumnInfo;
};

