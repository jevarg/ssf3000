#pragma once

#include <objidl.h>
#include <gdiplus.h>
#include <cstdint>

class F3000 {
public:
    F3000();
    ~F3000();

public:
    void update();
    void render(HDC hdc);

private:
    Gdiplus::GdiplusStartupInput startupInput;
    ULONG_PTR gdiPlusToken;

    Gdiplus::Bitmap *bmp;

    std::uint32_t x = 0;
    std::uint32_t y = 0;
};
