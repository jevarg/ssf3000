#pragma once

#include <objidl.h>
#include <gdiplus.h>

class F3000 {
public:
    F3000();
    ~F3000();

public:
    void render(HDC);

private:
    Gdiplus::GdiplusStartupInput startupInput;
    ULONG_PTR gdiPlusToken;

    Gdiplus::Bitmap *bmp;
};
