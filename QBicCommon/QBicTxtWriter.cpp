#include "stdafx.h"
#include "QBicTxtWriter.h"
#include "QBicTextEncoding.h"

QBicTxtWriter::QBicTxtWriter()
{
    m_nEncoding = 1; // UTF-8
}

QBicTxtWriter::~QBicTxtWriter()
{
    Close();
}

bool QBicTxtWriter::Open( CString strFileName, int nEncoding, BOOST_IOS::openmode mode)
{
    m_nEncoding = nEncoding;

    if (m_nEncoding == 1) {
        m_wofstream.open(strFileName, mode);

        if (m_wofstream.is_open() == true) {
            m_wofstream.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
        } else {
            return false;
        }

        return true;
    }

    m_ofstream.open(strFileName, mode);
    return m_ofstream.is_open();    
}

void QBicTxtWriter::Close()
{
    if (m_nEncoding == 1) {
        if (m_wofstream.is_open() == true)
            m_wofstream.close();
    } else {
        if (m_ofstream.is_open() == true)
            m_ofstream.close();
    }
}

void QBicTxtWriter::Write( CString strLine )
{
    if (m_nEncoding == 1) {
        if (m_wofstream.is_open() == false) 
            return;

        m_wofstream << (LPCTSTR)strLine << std::endl;
        m_wofstream.flush();
    } else {
        if (m_ofstream.is_open() == false) 
            return;

        QBicCharPtr spLine = QBicTextEncoding::WChar2Char(strLine);
        m_ofstream << spLine->Char() << std::endl;
        m_ofstream.flush();
    }
}
