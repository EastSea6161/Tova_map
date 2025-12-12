#pragma once
#include "ShapeFileInfo.h"
#include "ITxGeometry.h"
#include "TxFeature.h"
#include "TxPoint.h"
#include "TxPolyline.h"
#include "TxPolygon.h"

#include "../QBicExtStaticCLib/ShapeFileLib/shapefil.h"

/**
*@brief shape파일 작성
*@details shape 파일 작성
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ShapeFileWriter
{
public:
    ShapeFileWriter();
    ~ShapeFileWriter();

private: //사용금지    
    static int WriteBulkPoint    (CString strPath, CString strName, std::vector<ITxGeometryPtr>& vecGeometry, std::vector<std::vector<DbfFieldValue>>& vecRecords); 
    static int WriteBulkPolyLine (CString strPath, CString strName, std::vector<ITxGeometryPtr>& vecGeometry, std::vector<std::vector<DbfFieldValue>>& vecRecords);

private:
	/**
	*@brief Shape파일 추가
	*@details 버텍스형 파일 추가
	*@param SHPHandle hSHP
	*@param TxPointPtr spPoint
	*/
    static void AppendShapeFile (SHPHandle hSHP, TxPointPtr    spPoint);
	/**
	*@brief Shape파일 추가
	*@details 폴리라인형 파일 추가
	*@param SHPHandle hSHP
	*@param TxPolylinePtr spPolyline
	*/
    static void AppendShapeFile (SHPHandle hSHP, TxPolylinePtr spPolyline);    
	/**
	*@brief Shape파일 추가
	*@details 폴리곤형 파일 추가
	*@param SHPHandle hSHP
	*@param TxPolygonPtr spPolygon
	*/
    static void AppendShapeFile (SHPHandle hSHP, TxPolygonPtr  spPolygon);    
	/**
	*@brief Shape파일 추가
	*@details 멀티폴리곤형 파일 추가
	*@param SHPHandle hSHP
	*@param TxMPolygonPtr spMPolygon
	*/
    static void AppendShapeFile (SHPHandle hSHP, TxMPolygonPtr spMPolygon);    
	/**
	*@brief DB파일 작성
	*@param DBFHandle hDBF
	*@param int nRecordID
	*@param std::vector<DbfFieldValue>& vecRecord
	*/
    static void WriteDbf(DBFHandle	hDBF, int nRecordID, std::vector<DbfFieldValue>& vecRecord);

private:
    bool m_bOpened;
private:
    int m_nShapeType;
    int m_nRecordID;
private:
    SHPHandle m_hSHP;
    DBFHandle m_hDBF;

public:
	/**
	*@brief shape 파일 생성
	*@param CString strPath 파일 경로
	*@param CString strName 파일 명
	*@param int nShapeType 파일 타입
	*@param std::vector<DbfFieldInfo>& vecFieldInfo DB파일 정보
	*@return 
	*/
    int CreateShapeFile(CString strPath, CString strName, int nShapeType, std::vector<DbfFieldInfo>& vecFieldInfo);
public:
	/**
	*@brief 파일 열기
	*@param CString strPath 파일 경로
	*@param CString strName 파일 명
	*@return 성공시 true, 실패시 false 반환
	*/
    bool Open(CString strPath, CString strName);
public:
	/**
	*@brief shape 형식
	*@param ITxGeometryPtr spGeometry
	*@param std::vector<DbfFieldValue>& vecFieldValue
	*@return 
	*/
    int AppendShape(ITxGeometryPtr spGeometry, std::vector<DbfFieldValue>& vecFieldValue);
public:
	/**
	*@brief 파일 닫기
	*/
    void Close();

public:
	/**
	*@brief 샘플 생성
	*@param CString strFilePath 파일 경로
	*@param CString strFileName 파일 명
	*/
    static void SamplePolyCreate(CString strFilePath = _T("C:\\temp"), CString strFileName = _T("Test")) 
    {
        std::vector<DbfFieldInfo> vecFieldInfo; {
            DbfFieldInfo oFieldInfo;
            oFieldInfo.m_strFieldName = _T("ID");           
            oFieldInfo.m_nFiledType   = 1;

            vecFieldInfo.push_back(oFieldInfo);
        }
        
        ShapeFileWriter oShpWrite;
        if ( oShpWrite.CreateShapeFile(strFilePath, strFileName, 5, vecFieldInfo) != 1 ) {
            AfxMessageBox(_T("Shape 생성 중 오류가 발생하였습니다"));
            return ;
        }

        if (oShpWrite.Open(strFilePath, strFileName) == false) {
            AfxMessageBox (_T("Shape Open Error"));
            return;
        }

        std::vector<TxPoint> vecPt; {
            vecPt.push_back(TxPoint(0, 0));
            vecPt.push_back(TxPoint(0, 100));
            vecPt.push_back(TxPoint(100, 100));
            vecPt.push_back(TxPoint(100, 0));
            vecPt.push_back(TxPoint(0, 0));
        }

        //★ Hole : 반시계
        std::vector<TxPoint> vecSubPt; {            
            vecSubPt.push_back(TxPoint(40, 40));
            vecSubPt.push_back(TxPoint(60, 40));
            vecSubPt.push_back(TxPoint(60, 60));
            vecSubPt.push_back(TxPoint(40, 60));
            vecSubPt.push_back(TxPoint(40, 40));
        }

        TxPolygonPtr spPoly = TxPolygonPtr(new TxPolygon(vecPt));
        std::vector<DbfFieldValue> vecFieldValue; {
            DbfFieldValue oDbfFieldValue;
            oDbfFieldValue.m_nFieldIndex = 0;
            oDbfFieldValue.m_strValue    = _T("100");
            vecFieldValue.push_back(oDbfFieldValue);
        }

        spPoly->AddHolePoint(vecSubPt);
        oShpWrite.AppendShape(spPoly, vecFieldValue);

        TxMPolygonPtr spMPolygon(new TxMPolygon());
        TxPolygon oPoly1; {
            std::vector<TxPoint> vecPt; {
                vecPt.push_back(TxPoint(300, 300));
                vecPt.push_back(TxPoint(300, 400));
                vecPt.push_back(TxPoint(400, 400));
                vecPt.push_back(TxPoint(400, 300));
                vecPt.push_back(TxPoint(300, 300));
            }
            oPoly1.OutRing(vecPt);
        }

        TxPolygon oPoly2; {
            std::vector<TxPoint> vecPt; {
                vecPt.push_back(TxPoint(500, 300));
                vecPt.push_back(TxPoint(500, 600));
                vecPt.push_back(TxPoint(600, 600));
                vecPt.push_back(TxPoint(600, 300));
                vecPt.push_back(TxPoint(500, 300));
            }
            oPoly2.OutRing(vecPt);
        }

        spMPolygon->AddSubPolygon(oPoly1);
        spMPolygon->AddSubPolygon(oPoly2);

        if (true) {
            std::vector<DbfFieldValue> vecFieldValue; {
                DbfFieldValue oDbfFieldValue;
                oDbfFieldValue.m_nFieldIndex = 0;
                oDbfFieldValue.m_strValue    = _T("999");
                vecFieldValue.push_back(oDbfFieldValue);
            }
            oShpWrite.AppendShape(spMPolygon, vecFieldValue);
        }

        oShpWrite.Close();
    }
};