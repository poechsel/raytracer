#include "image.h"

Image::Image(int width, int height, int type):
    width(width), height(height), _type(type)
{
    data = new uint8_t[width * height * _type];
}

Image::~Image()
{
    delete[] data;
}


void Image::putPixelFloat(unsigned int x, unsigned int y, float r, float g, float b, float a){
    this->putPixel(x, y, r * 255, g * 255, b * 255, a * 255);
}

void Image::putPixel(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    if (x >= this->width || x < 0 || y < 0 || y >= this->height)
        return;
    data[(y * width + x) * _type] = r;
    data[(y * width + x) * _type + 1] = g;
    data[(y * width + x) * _type + 2] = b;
    if (_type == RGBA)
        data[(y * width + x) * _type + 3] = a;
}


Vector3f Image::getPixel(Real x, Real y){
    uint rx = x * width;
    uint ry = y * height;

    Vector3f pixel(data[(ry * width + rx) * _type]
                   , data[(ry * width + rx) * _type + 1]
                   , data[(ry * width + rx) * _type + 2]);

    if (_type == RGBA)
        pixel.w = data[(ry * width + rx) * _type + 3];
    return pixel;
}
