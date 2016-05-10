#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include "../includes.h"

const int RGBA = 4;
const int RGB = 3;

class Image
{
    public:
        Image(int width=0, int height=0, int type = RGBA);
        virtual ~Image();

        virtual void writeHeader() = 0;
        virtual void writeFooter() = 0;
        virtual void saveTo(std::string target = "") = 0;
        virtual void load(std::string path) = 0;
        virtual Vector3f getPixel(Real x, Real y);
        virtual void putPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        virtual void putMegaPixel(int s, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        virtual void putPixelFloat(int x, int y, float r, float g, float b, float a = 1.f);
        virtual void putMegaPixelFloat(int s, int x, int y, float r, float g, float b, float a = 1.f);
        uint8_t *data;
        int width;
        int height;
    protected:
        int _type;
    private:
};

#endif // IMAGE_H
