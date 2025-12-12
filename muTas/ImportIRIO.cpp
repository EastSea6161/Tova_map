// ImportIRIO.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImportIRIO.h"
#include "afxdialogex.h"


// KImportIRIO 대화 상자입니다.

IMPLEMENT_DYNAMIC(KImportIRIO, CDialog)

KImportIRIO::KImportIRIO(CWnd* pParent /*=NULL*/)
	: CDialog(KImportIRIO::IDD, pParent)
{

}

KImportIRIO::~KImportIRIO()
{
}

void KImportIRIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KImportIRIO, CDialog)
END_MESSAGE_MAP()


// KImportIRIO 메시지 처리기입니다.
