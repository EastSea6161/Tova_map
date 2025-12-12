#include "stdafx.h"
#include "IThunderMapCommand.h"

void IThunderMapCommand::DebugLabel( Gdiplus::Graphics& graphics, Gdiplus::PointF pointF, CString strDebugLabel )
{
    Gdiplus::FontFamily oFontFamily(L"Tahoma");
    Gdiplus::Font oFont(&oFontFamily, 10, FontStyleBold, UnitPixel);

    SolidBrush oFontBrush(Color(200, 0, 0, 0));
    StringFormat stringFormat;
    stringFormat.SetAlignment(StringAlignmentNear);
    stringFormat.SetLineAlignment(StringAlignmentNear);

    graphics.DrawString(strDebugLabel, (INT)wcslen(strDebugLabel), &oFont, pointF, &stringFormat, &oFontBrush);
}
