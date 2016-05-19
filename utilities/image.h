#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include "../includes.h"

const int RGBA = 4;
const int RGB = 3;

/* classe génerique pour gérer les images */

class Image
{
    public:
        Image(int width=0, int height=0, int type = RGBA);
        virtual ~Image();

        virtual void writeHeader() = 0;
        virtual void writeFooter() = 0;

        //sauvegarde l'image
        virtual void saveTo(std::string target = "") = 0;
        //charge une image
        virtual void load(std::string path) = 0;

        virtual Vector3f getPixel(Real x, Real y);
        //assigne à un pixel la couleur (r, g, b) où r, g et b sont des entiers
        virtual void putPixel(int x, int y
                              , uint8_t r, uint8_t g, uint8_t b
                              , uint8_t a = 255);
        //pareil mais avec des flottants
        virtual void putPixelFloat(int x, int y
                                   , float r, float g, float b
                                   , float a = 1.f);

        uint8_t *data;
        int width;
        int height;
    protected:
        int _type;
    private:
};

#endif // IMAGE_H
