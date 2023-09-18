#include "F3000.h"

#include <cassert>
#include "bitmap.h"

using namespace Gdiplus;

F3000::F3000() {
    GdiplusStartup(&gdiPlusToken, &startupInput, nullptr);
    HBITMAP hbm = CreateBitmap(imageWidth, imageHeight, 1, 32, f3000ImgData);
    assert(hbm);

    bmp = new Bitmap(hbm, nullptr);
}

F3000::~F3000() {
    GdiplusShutdown(gdiPlusToken);
    delete bmp;
}

void F3000::update() {
    x += 3;
    y += 3;
}

void F3000::render(HDC hdc) {
    Graphics graphics(hdc);
    graphics.DrawImage(bmp, (INT) x, (INT) y);
}
