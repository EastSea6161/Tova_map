/**
 * @file TransitSelection.h
 * @brief KTransitSelection 선언 파일
 * @author 
 * @date 2011.08.02
 * @remark
 */


#pragma once


/**
 * @brief KIOTransitTableView에서 checkbox로 선택된 trinsit 정보를 저장
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.08.02
 */
class KIOTableRecordSelection
{
public:
	KIOTableRecordSelection (void);
	~KIOTableRecordSelection(void);

public :
	bool IsSelected       (Integer a_nxID);
	void AddSelect        (Integer a_nxID);
	void RemoveSelect     (Integer a_nxID);
	void ClearSelection   (void);

protected :
	std::set<Integer>  m_setSelectedID;
public:
    std::set<Integer>  GetSelection(void);
    std::set<Integer>::iterator begin();
    std::set<Integer>::iterator end();

    int SelectedCount();
};

class TBackStyle
{
public:
    TBackStyle()
    {
        bEverCustomclr = false;
        bUseCustomClr = true;
        TRGBColor = RGB(255, 0, 255);
        dWidth = 1.0;
        dOffset = 0.0;
    }

    ~TBackStyle() {}

public:
    bool bEverCustomclr; // 한번이라도 사용자 색상을 적용했었는지 여부
    bool bUseCustomClr; // 사용자 색상을 적용할지 말지 여부
    COLORREF TRGBColor;
    double   dWidth;
    double   dOffset;
};

class KIOTableRecordBackStyle
{
public:
    KIOTableRecordBackStyle() {};
    virtual ~KIOTableRecordBackStyle() {};

public:
    void AddStyle(Integer a_nxID, TBackStyle &a_oStyle);
    void UpdateStyle(Integer a_nxID, TBackStyle &a_oStyle);
    bool IsStyle(Integer a_nxID, TBackStyle &a_oStyle);
    void ClearStyle(void);

private:
    std::map<Integer, TBackStyle> m_mapIDStyle;
};