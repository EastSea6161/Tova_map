#pragma once
/**
*@brief 메세지박스
*@details 큐빅웨어 메세지박스
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicMessageBox
{
public:
    QBicMessageBox()  {}
    ~QBicMessageBox() {}

public:
	/**
	*@brief 메세지 박스
	*@param CString a_strMsg     내용
	*@param CString a_strCaption 주석
	*/
    static void MessageBox(CString a_strMsg, CString a_strCaption = _T("Information"));
};