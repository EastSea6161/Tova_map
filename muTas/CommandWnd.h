/**
 * @file StatusView.h
 * @brief KStatusView 선언
 * @author nomplus2@qbicware.com
 * @date 2010.08.01
 * @remark
 */


#pragma once

#include "ImChampCommand.h"

/**
 * @brief kmz 프로그램의 실행상태 등 로그를 표출한다.
 * @remark
 * @warning
 * @version 1.0
 * @author nomplus2@qbicware.com
 * @date 2010.08.01
 */

enum KEMCommandType
{
    KEMModal    = 0,
    KEMModeless = 1,
    KEMCommon   = 2
};

class KCommandEdit : public CEdit
{
    DECLARE_DYNAMIC(KCommandEdit)
private:
    CString         m_strCommand;
    CString         m_strUserInput;
    KEMCommandType  m_emCommandType;


public:
    KCommandEdit();
    virtual ~KCommandEdit();

public:
    void SetCommand(CString a_strCommand, KEMCommandType a_emCommandType = KEMCommon);
    bool IsModal()
    {
        return m_emCommandType ==  KEMModal;
    }
    KEMCommandType GetCommandType()
    {
        return m_emCommandType;
    }
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnChar     (UINT  nChar,  UINT nRepCnt,  UINT nFlags);
    afx_msg void OnKeyDown  (UINT  nChar,  UINT nRepCnt,  UINT nFlags);
    afx_msg void OnKillFocus(CWnd* pNewWnd                           );
};

class KCommandWnd : public CWnd
{
	DECLARE_DYNAMIC(KCommandWnd)

	/**************************************
	* Constructors and Destructor         *
	**************************************/

private:    
    CXTPEdit      m_wndCommandView;
    KCommandEdit  m_wndCommandEdit;

private:
    CFont m_fontCommandEdit;
    CFont m_fontCommandView;
public:
	KCommandWnd();
	virtual ~KCommandWnd();
        
    HWND GetCommandEditHandle() 
    {
        return m_wndCommandEdit.GetSafeHwnd();
    }

    bool IsModal()
    {
        return m_wndCommandEdit.IsModal();
    }

    void CommandMsg         (CString a_strCommand);
    void CommandLogMsg      (CString a_strMsg    );
    void CommandResultReply (CString a_strCommand);

protected:
	DECLARE_MESSAGE_MAP()
    afx_msg int  OnCreate           ( LPCREATESTRUCT lpCreateStruct );
    afx_msg void OnSize             ( UINT nType, int cx, int cy    );
    afx_msg void OnSetFocus         ( CWnd* pOldWnd                 );
    
    afx_msg void OnContextMenu      ( CWnd* pWnd, CPoint pos        );
    virtual BOOL PreTranslateMessage(MSG* pMsg                      );

private:
    ImChampModalCommandPtr m_spModalCommand;
public:
    void Command   (ImChampModalCommandPtr a_spModalCommand, KEMCommandType a_emCommandType = KEMModal);
    void SetDefaultCommand();
};


