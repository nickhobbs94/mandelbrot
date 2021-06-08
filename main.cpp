#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <complex>
#include <iostream>
#include <cmath>

using namespace sf;

int iterstore[800*800];

int realmap(int i, std::complex<double> r){
    int x = 254 - std::log(std::imag(r)*10);
    if (x < 0)
        x = 0;
    return x;
}

void setpixel(int i, Uint8* pixel, std::complex<double> complexstore[], std::complex<double> complexstore2[], int iter){
    // complex number equal to z = a + ib
    // but we've shifted x and y so 400,400 is zero and 0,400 is -3
    if (std::isnan(std::real(complexstore2[i]))){
        return;
    }
    complexstore2[i] = complexstore2[i] * complexstore2[i] + complexstore[i];

    if (std::abs(complexstore2[i]) < 2){
        pixel[0] = 10;
        pixel[1] = 10;
        pixel[2] = 10;
        pixel[3] = 255;
    }
    else {
        complexstore2[i] = std::complex<double>(nan(""),(double)iter);
        pixel[0] = 200;
        pixel[1] = 230;
        pixel[2] = 200;
        pixel[3] = realmap(i,complexstore2[i]);
    }

    
}



int main()
{
    const unsigned int width = 800;
    const unsigned int height = 800;
    double xCenter = -0.75;
    double yCenter = 0.0;
    double zoomWidth = 3.5;
    double zoomHeight = 3.5;

    Vector2f windowSize(800, 800);
    Vector2f resolution(width,height);

    Uint8* pixels = new Uint8[width*height*4];
    Texture texture;
    texture.create(width, height);

    Sprite sprite(texture);
    sprite.setPosition(0,0);

    RenderWindow window;
    window.setFramerateLimit(60);
    window.create(VideoMode(windowSize.x, windowSize.y, 32), "MyGame", Style::Default);
    
    FloatRect area(0,0, resolution.x, resolution.y);
    window.setView(View(area));

    Clock clock;

    std::complex<double>* complexstore = new std::complex<double>[width*height];
    std::complex<double>* complexstore2 = new std::complex<double>[width*height];

    for (int i=0; i<width*height; i++){
        double a = (1.0*(i%width)-width/2.0) * (zoomWidth / width) + xCenter;
        double b = (1.0*(i/width)-height/2.0) * (-zoomHeight / height) + yCenter;
        complexstore[i] = std::complex<double>(a,b);
        complexstore2[i] = std::complex<double>(0,0);
    }
    
    int iter=0;
    while (window.isOpen())
    {
        iter++;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed){
                Vector2i position = sf::Mouse::getPosition();
                std::cout << event.mouseButton.x << ":" << event.mouseButton.y << std::endl;
                xCenter = (event.mouseButton.x/1000.0*width-width/2.0) * (zoomWidth / width) + xCenter;
                yCenter = (event.mouseButton.y/1000.0*height-height/2.0) * (-zoomHeight / height) + yCenter;

                zoomWidth = zoomWidth / 2.0;
                zoomHeight = zoomHeight / 2.0;

                std::cout << xCenter << std::endl;
                iter=0;
                for (int i=0; i<width*height; i++){
                    double a = (1.0*(i%width)-width/2.0) * (zoomWidth / width) + xCenter;
                    double b = (1.0*(i/width)-height/2.0) * (-zoomHeight / height) + yCenter;
                    complexstore[i] = std::complex<double>(a,b);
                    complexstore2[i] = std::complex<double>(0,0);
                }
            }
        }
        //sprite.setColor(Color(128, 255, 128));
        for (int i=0; i<width*height; i++){
            setpixel(i, pixels+i*4, complexstore, complexstore2, iter);
        }
        
        
        texture.update(pixels);
        
        window.clear();
        
        window.draw(sprite);
        window.display();
    }

    return 0;
}

