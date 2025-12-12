#pragma once
/**
*@brief 레포트 컨트롤 세팅
*@details 레포트 컨트롤 세팅
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicReportCtrlSetting
{
public:
	QBicReportCtrlSetting()  {}
	~QBicReportCtrlSetting() {}
public:
	/**
	*@brief 기본작업 설정
	*@details 레포트 컨트롤 기본 세팅
	*@param CXTPReportControl& wndReport	대상 컨트롤
	*@param BOOL bEdit						에디팅 선택
	*@param BOOL bArrowColumAction			헤더 관련 
	*@param BOOL bSelectionEnable			선택가능 여부
	*/
	static void Default        (CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE, BOOL bSelectionEnable = TRUE);

    static void DefaultZ       (CXTPReportControl& wndReport, BOOL bEdit = FALSE, BOOL bArrowColumAction = FALSE, BOOL bSelectionEnable = TRUE);

	/**
	*@brief 헤더 숨기기
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*/
	static void HideHeader     (CXTPReportControl& wndReport);
	/**
	*@brief 컬럼 정렬
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*/
	static void AllowColumnsort(CXTPReportControl& wndReport);
	/**
	*@brief 
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*/
	static void AllowColumnReorder(CXTPReportControl& wndReport);
	/**
	*@brief 
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*/
	static void HorizontalNoLine( CXTPReportControl& wndReport );
	/**
	*@brief 
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*/
	static void VerticalNoLine( CXTPReportControl& wndReport );
	/**
	*@brief 
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*/
	static void SelectionEnableFalse( CXTPReportControl& wndReport );
	/**
	*@brief 컬럼크기 자동변환 거부
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*/
	static void AutoColumnSizeFalse( CXTPReportControl& wndReport );

	/**
	*@brief 마음대로 설정
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*/
	static void YouXX  (CXTPReportControl& wndReport);
	/**
	*@brief 기본설정 B
	*@details 에디팅 불가, 줄선택 불가, 
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*@param BOOL bEdit					 에디팅 선택
	*/
	static void DefaultB(CXTPReportControl& wndReport, BOOL bEdit=FALSE);
	/**
	*@brief 기본설정 C
	*@details 컬럼액션 불가, 줄선택 불가
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*@param BOOL bEdit					 에디팅 선택
	*/
	static void DefaultC(CXTPReportControl& wndReport, BOOL bEdit=FALSE);
	/**
	*@brief 기본설정 D
	*@details 그리드 스타일 적용
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*@param BOOL bEdit					 에디팅 선택
	*@param BOOL bArrowColumAction		 컬럼액션
	*/
	static void DefaultD(CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
	/**
	*@brief 기본설정 E
	*@details 그리드 스타일 적용, 자동 크기 변환 비활성화
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*@param BOOL bEdit					 에디팅 선택
	*@param BOOL bArrowColumAction		 컬럼액션
	*/
	static void DefaultE(CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
	/**
	*@brief 기본설정 F
	*@details 복사 가능 여부
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*@param BOOL bEdit					 에디팅 선택
	*@param BOOL bArrowColumAction		 컬럼액션
	*/
	static void DefaultF(CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
	/**
	*@brief 트리형식 기본설정
	*@details 
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*@param BOOL bEdit					 에디팅 선택
	*@param BOOL bArrowColumAction		 컬럼액션
	*/
	static void DefaultTree(CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
	/**
	*@brief 트리형식 크기
	*@details 
	*@param CXTPReportControl& wndReport 대상 컨트롤
	*@param int nSize					 트리 크기
	*@param BOOL bEdit					 에디팅 선택
	*@param BOOL bArrowColumAction		 컬럼액션
	*/
	static void DefaultTreeStructuerDotSize(CXTPReportControl& wndReport, int nSize, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
};
/**
*@brief 레포트 컨트롤
*@details 레포트 컨트롤 유틸 설정
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicReportCtrlUtil
{
public:
	QBicReportCtrlUtil() {}
	~QBicReportCtrlUtil(){}
public:
	/**
	*@brief 헤더 아이콘 
	*@param XTP_NM_REPORTRECORDITEM* pRptRecordItem
	*@param int nIConWidth
	*@return 성공 : ID값 반환 / 실패 -1 반환
	*/
	static int GetHeaderIConID(XTP_NM_REPORTRECORDITEM* pRptRecordItem, int nIConWidth);

    /**
    *@brief CXTPReportControl 리스트의 전체 Row를 선택 (체크박스 아님. 파란 선택 박스)
    *@param CXTPReportControl a_wndReport
    */
    static void SelectAllRows(CXTPReportControl &wndReport);


public:
    /**
    *@brief CXTPReportControl 선택된 Row를 클립보드로 복사
    *@param LPCTSTR a_strSeparator 작성시 구분자
    *@param std::set<int> &a_setExcept 작성시 제외 컬럼 인덱스
    */
    static BOOL CopyReportToClip(CXTPReportControl &a_wndReport, LPCTSTR a_strSeparator = _T("\t"));
    static BOOL CopyReportToClip(CXTPReportControl &a_wndReport, std::set<int> &a_setExcept, LPCTSTR a_strSeparator = _T("\t"));

private:
    static void WriteReportHead(CXTPReportControl &a_wndReport, LPCTSTR a_strSeparator, std::set<int> &a_setExcept, OUT CString &a_strHead);
    static void WriteReportData(CXTPReportControl &a_wndReport, LPCTSTR a_strSeparator, std::set<int> &a_setExcept, OUT CString &a_strData);

};
/**
*@brief 레포트 차트 컨트롤 세팅
*@details 차트 컨트롤 세팅
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicChartCtrlSetting
{
public:
	QBicChartCtrlSetting()  {}
	~QBicChartCtrlSetting() {}
public:
	/**
	*@brief 기본 설정
	*@details 툴팁 사용 가능
	*@param CXTPChartControl &wndChart 대상 차트 컨트롤
	*/
	static void DefaultCtrl        (CXTPChartControl &wndChart);
	/**
	*@brief 차트 제목
	*@details 차트 제목 표시
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*@param CString strTitle		   제목
	*/
	static void DefaultContent     (CXTPChartContent* pContent, CString strTitle=_T(""));
	/**
	*@brief 차트 나열 방식
	*@details 차트 나열 방식(왼쪽)
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*/
	static void LeftLegend         (CXTPChartContent* pContent);
	/**
	*@brief 차트 나열 방식
	*@details 차트 나열 방식(바닥)
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*/
	static void BottomLegend       (CXTPChartContent* pContent);
	/**
	*@brief 차드 모양 설정
	*@details 차트 모양(레이더형)
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*@param CString strSeriesName	   시리즈 명칭 설정
	*/
	static void DefaultSeriesRadar (CXTPChartSeries* pSeries, CString strSeriesName=_T(""));
	/**
	*@brief 차드 모양 설정
	*@details 차트 모양(바형)
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*@param CString strSeriesName	   시리즈 명칭 설정
	*@param BOOL a_bColorEach		   색 설정 여부
	*/
	static void DefaultSeriesBar   (CXTPChartSeries* pSeries, CString strSeriesName=_T(""), BOOL a_bColorEach=FALSE);
	/**
	*@brief 차드 모양 설정
	*@details 차트 모양(선형)
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*@param CString strSeriesName	   시리즈 명칭 설정
	*/
	static void DefaultSeriesLine  (CXTPChartSeries* pSeries, CString strSeriesName);
	/**
	*@brief 차드 모양 설정
	*@details 차트 모양(영역형)
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*@param CString strSeriesName	   시리즈 명칭 설정
	*/
	static void DefaultSeriesArea  (CXTPChartSeries* pSeries, CString strSeriesName=_T(""));
	/**
	*@brief 차드 모양 설정
	*@details 차트 모양(포인트형)
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*@param CString strSeriesName	   시리즈 명칭 설정
	*@param BOOL a_bColorEach		   색 설정 여부
	*/
	static void DefaultSeriesPoint (CXTPChartSeries* pSeries, CString strSeriesName=_T(""), BOOL a_bColorEach=FALSE);
	/**
	*@brief 차드 모양 설정
	*@details 차트 모양(스틱바형)
	*@param CXTPChartControl* pContent 대상 차트 컨트롤
	*@param CString strSeriesName	   시리즈 명칭 설정
	*@param BOOL a_bColorEach		   색 설정 여부
	*/
	static void DefaultSeriesStackedBar(CXTPChartSeries* pSeries, CString strSeriesName=_T(""), BOOL a_bColorEach=FALSE);
	/**
	*@brief 차드 모양 설정
	*@details 차트 모양(레이더형 다이어그램)
	*@param CXTPChartRadarDiagram* pDiagram 차트 다이어그램
	*/
	static void DefaultDiagramRadar(CXTPChartRadarDiagram* pDiagram);
	/**
	*@brief 차드 모양 설정
	*@details 차트 모양(2D 다이어그램)
	*@param CXTPChartDiagram2D* pDiagrm 대상 다이어그램
	*@param CString strTitleX     	    가로측 제목
	*@param CString strTitleY     	    세로측 제목
	*/
	static void DefaultDiagram2D   (CXTPChartDiagram2D*    pDiagram, CString strTitleX, CString strTitleY);
	/**
	*@brief 라벨 수정
	*@param XTPChartControl* pctrlChart 대상 차트
	*@param BOOL a_bShow				라벨 Show
	*@throw	디버그
	*/
	static void UpdateLabels       (CXTPChartControl* pctrlChart, BOOL a_bShow);
	/**
	*@brief X축 수정
	*@param XTPChartControl* pctrlChart 대상 차트
	*@param BOOL a_bShow				라벨 Show
	*/
	static void UpdateAxisX		   (CXTPChartControl* pctrlChart, BOOL a_bShow);
	/**
	*@brief X축 글씨 각도
	*@param XTPChartControl* pctrlChart 대상 차트
	*@param int a_nAngle				각도
	*/
	static void UpdateAxisXLabelAngle(CXTPChartControl* pctrlChart, int a_nAngle);
	/**
	*@brief X축 글씨 크기
	*@param XTPChartControl* pctrlChart 대상 차트
	*@param int a_nSize					크기
	*/
	static void UpdateMarkerSize   (CXTPChartControl* pctrlChart, int a_nSize);
    static void UpdateMarkerBorderVisible(CXTPChartControl* pctrlChart, BOOL a_bVisible);
    static void UpdateMarkerFillStyle(CXTPChartControl* pctrlChart, XTPChartFillMode oFillMode);
    static void UpdateMarkerType(CXTPChartControl* pctrlChart, XTPChartMarkerType emMarkerType);
    /**
    *@brief 마커 사용 여부
    *@param XTPChartControl* pctrlChart 대상 차트
    *@param int a_bVisible					마커 사용 여부
    */
    static void MarkerVisible      (CXTPChartControl* pctrlChart, BOOL a_bVisible);

    
    /**
	*@brief Y축 Zero Level
	*@param XTPChartControl* pctrlChart 대상 차트
	*@param BOOL a_bShowZeroLevel		제로레벨 Show
	*/
	static void UpdateAxisYShowZeroLevel(CXTPChartControl* pctrlChart, BOOL a_bShowZeroLevel);
	/**
	*@brief X축 여백
	*@param XTPChartControl* pctrlChart 대상 차트
	*@param BOOL a_bSideMargins			X축 여백 
	*/
	static void UpdateAxisXSideMargins(CXTPChartControl* pctrlChart, BOOL a_bSideMargins);

    /**
    *@brief 축별 줌 적용 여부
    *@param CXTPChartDiagram2D* pDiagram2D 대상 차트
    *@param BOOL bZoomX, bZoomY			X축, Y축 줌 적용 여부
    */
    static void ZoomDiagram2D(CXTPChartDiagram2D* pDiagram2D, BOOL bZoomX, BOOL bZoomY);
    static void ZoomDiagram2D(CXTPChartControl* pctrlChart, int nDiagramIdx, BOOL bZoomX, BOOL bZoomY);
    
    // 차트 라인 두께
    static void UpdateLineThickness(CXTPChartControl* pctrlChart, int nThickness);
};

/**
*@brief 컬러피커 설정
*@details 컬러피커 설정
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicColorPickerSetting
{
public:
	QBicColorPickerSetting()  {}
	~QBicColorPickerSetting() {}
public:
	/**
	*@brief 기본 설정
	*@param CXTPColorPicker &wndColorPicker 컬러피커 정의
	*@param COLORREF clr					색 가져오기
	*/
	static void DefaultCtrl         (CXTPColorPicker &wndColorPicker, COLORREF clr);
};

/**
*@brief Track 컨트럴 설정
*@details Track 컨트럴 설정
*@author 큐빅웨어
*@date 2018/11/14
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicTrackCtrlSetting
{
public:
    QBicTrackCtrlSetting() {}
    ~QBicTrackCtrlSetting() {}
public:
    /**
    *@brief 기본 설정
    *@param CXTPColorPicker &wndColorPicker 컬러피커 정의
    *@param COLORREF clr					색 가져오기
    */
    static void DefaultCtrl(CXTPTrackControl& wndTrack);
};