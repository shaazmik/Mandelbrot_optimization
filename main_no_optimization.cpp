#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <string>


static const int WIDTH = 900, HEIGHT = 600;
static const int Fps_buf_size = 1024;
static float Fps = 0;
static int character_size = 15;


typedef struct mandelbrot_painting
{
    double x_center = 0.f;
    double y_center = 0.f;
    
    sf::Uint8* pixels = new sf::Uint8[WIDTH*HEIGHT*4];

}mandelbrot_painting;


// unsigned char calculate_color(double x, double y)
// {

// }

void printing_madnelbrot(sf::RenderWindow* window)
{

    mandelbrot_painting mandelbrot_struct = {};
    sf::Texture mandelbrot_layer;

    mandelbrot_layer.create(WIDTH, HEIGHT);
    mandelbrot_layer.update(mandelbrot_struct.pixels, WIDTH, HEIGHT, 0, 0);

    sf::Sprite sprite;
    sprite.setTexture(mandelbrot_layer);


    double dx = 1/900.f, dy = 1/600.f, scale = 1.f;
    double    rmax  = 100.f;
    const int nMax  = 256;
    

    //key rofles

    for (int yi = 0; yi < HEIGHT; yi++)
    {
        double x0 = ((           -  450.f)*dx + mandelbrot_struct.x_center) * scale,
               y0 = (((double)yi -  300.f)*dy + mandelbrot_struct.y_center) * scale;

        for (int xi = 0; xi < WIDTH; xi++, x0 += dx)
        {
            double current_x = x0,
                   current_y = y0;

            for (int n = 0; n < nMax; n++)
            {
                double sqr_x = current_x * current_x, 
                       sqr_y = current_y * current_y, 
                       xy    = current_x * current_y;
                
                double sqr_r = sqr_x + sqr_y;

                if (sqr_r >= rmax)
                {
                    current_x = 0;
                    current_y = 0;
                    break;
                }

                current_x = sqr_x - sqr_y + x0;
                current_y =  xy   +  xy   + y0;
            }

            if (current_x == 0 && current_y == 0)
            {
                mandelbrot_struct.pixels[xi + yi* WIDTH + 0] = 0;
                mandelbrot_struct.pixels[xi + yi* WIDTH + 1] = 0;
                mandelbrot_struct.pixels[xi + yi* WIDTH + 2] = 0;
                mandelbrot_struct.pixels[xi + yi* WIDTH + 3] = 255;
            }
            else
            {
                mandelbrot_struct.pixels[xi + yi* WIDTH + 0] = 255;
                mandelbrot_struct.pixels[xi + yi* WIDTH + 1] = 255;
                mandelbrot_struct.pixels[xi + yi* WIDTH + 2] = 255;
                mandelbrot_struct.pixels[xi + yi* WIDTH + 3] = 255;
            }
        }
    }

    mandelbrot_layer.update(mandelbrot_struct.pixels, WIDTH, HEIGHT, 0, 0);

    (*window).draw(sprite);
}


void print_fps(sf::Clock* clock, sf::Time* previousTime, sf::RenderWindow* window, sf::Font* font)
{
    sf::Time currentTime;
    sf::Text fps_text;
    char fps_buf[Fps_buf_size] ={0};

    currentTime = (*clock).getElapsedTime();
    Fps = 1.0f / (currentTime.asSeconds() - (*previousTime).asSeconds()); // the asSeconds returns a float
    (*previousTime) = currentTime;
    sprintf(fps_buf, "%04.2f", Fps);

    fps_text.setCharacterSize(character_size);
    fps_text.setFont((*font));
    fps_text.setString(fps_buf);
    fps_text.setPosition(0, HEIGHT - character_size);
    fps_text.setColor(sf::Color::Cyan);

    (*window).draw(fps_text);
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot");
    
    sf::Clock clock;
    sf::Time previousTime = clock.getElapsedTime();
    sf::Font font;
    font.loadFromFile("C:\\WINDOWS\\FONTS\\DEJAVUSANS.ttf");

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        printing_madnelbrot(&window);
        print_fps(&clock, &previousTime, &window, &font);

        window.display();
    }

}