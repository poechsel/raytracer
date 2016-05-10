#include "sfmlimage.h"

SFMLImage::SFMLImage(int width, int height, int type): Image(width, height, type),
_window(sf::VideoMode(this->width, this->height), "image")
{
    _texture.create(this->width, this->height);
    _sprite.setTexture(_texture);
}

SFMLImage::~SFMLImage(){
    while (_window.isOpen()){
        sf::Event event;
        while (_window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                _window.close();
        }
    }
}

void SFMLImage::load(std::string path) {
    sf::Image image;
    image.loadFromFile(path);
    delete[] data;
    this->_type = RGBA;
    this->width = image.getSize().x;
    this->height = image.getSize().y;
    this->data = new uint8_t[this->width * this->height * this->_type];
    for (uint y = 0; y < this->height; ++y) {
        for (uint x = 0; x < this->width; ++x) {
            sf::Color color = image.getPixel(x, y);
            this->putPixel(x, y, color.r, color.g, color.b, color.a);
        }
    }
    //this->data = const_cast<uint8_t *>(image.getPixelsPtr());
}

void SFMLImage::writeHeader(){
}


void SFMLImage::writeFooter(){
}

void SFMLImage::saveTo(std::string target){
    std::cout<<"update texture\n";
    sf::Event event;
        while (_window.pollEvent(event));
    _texture.update(this->data);
    _window.clear();
    _window.draw(_sprite);
    _window.display();

    if (target != ""){
        _window.capture().saveToFile(target);
    }
}
