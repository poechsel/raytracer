#ifndef SFMLIMAGE_H
#define SFMLIMAGE_H

#include <SFML/Graphics.hpp>
#include "image.h"

class SFMLImage: public Image
{
    public:
        SFMLImage(int width=0, int height=0, int type = RGBA);
        virtual ~SFMLImage();
        virtual void writeHeader();/*
        virtual void putPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
           Image::putPixel(x, y, r, g, b, a) ;

        }*/
        virtual void writeFooter();
        virtual void saveTo(std::string target = "");
        virtual void load(std::string path);
    protected:
        sf::RenderWindow    _window;
        sf::Texture         _texture;
        sf::Sprite          _sprite;
    private:
};

#endif // SFMLIMAGE_H
