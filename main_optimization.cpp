#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <emmintrin.h>


static const int WIDTH = 900, HEIGHT = 600;
static const int Fps_buf_size = 1024;
static float Fps = 0;
static int character_size = 15;


typedef struct mandelbrot_painting
{
    double x_center = 0.f;
    double y_center = 0.f;
    double dx = 1/300.f, dy = 1/300.f, scale = 1.f;

    sf::Uint8* pixels = new sf::Uint8[WIDTH*HEIGHT*4];

}mandelbrot_painting;



void printing_madnelbrot(sf::RenderWindow* window, mandelbrot_painting* mandelbrot_struct)
{

    sf::Texture mandelbrot_layer;

    mandelbrot_layer.create(WIDTH, HEIGHT);
    mandelbrot_layer.update(mandelbrot_struct->pixels, WIDTH, HEIGHT, 0, 0);

    sf::Sprite sprite;
    sprite.setTexture(mandelbrot_layer);

    double    rmax  = 100.f;
    const int nMax  = 256;

    double scale    = mandelbrot_struct->scale, 
           dx       = mandelbrot_struct->dx, 
           dy       = mandelbrot_struct->dy,
           x_center = mandelbrot_struct->x_center,
           y_center = mandelbrot_struct->y_center;

    sf::Uint8* pixels = mandelbrot_struct->pixels;


    for (int yi = 0; yi < HEIGHT; yi++)
    {
        double x0 = ((           -  WIDTH/2 ) * scale + x_center) * dx,
               y0 = (((double)yi -  HEIGHT/2) * scale + y_center) * dy;

        for (int xi = 0; xi < 4 * WIDTH; xi = xi + 4, x0 += dx * scale)
        {
            double current_x = x0,
                   current_y = y0;

            int n = 0;

            for (; n < nMax; n++)
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

            float color_base = sqrtf(sqrtf(float(n)/float(nMax))) * 255.f;
            unsigned char intensive = (unsigned char) color_base;

            int offset_Y = WIDTH * 4 * yi;

            pixels[xi + offset_Y + 0] = (unsigned char) intensive / 1.6;
            pixels[xi + offset_Y + 1] = (unsigned char) intensive * 2.1;
            pixels[xi + offset_Y + 2] = (unsigned char) intensive * 4.9;
            pixels[xi + offset_Y + 3] = intensive;
        }
    }

    mandelbrot_layer.update(mandelbrot_struct->pixels, WIDTH, HEIGHT, 0, 0);

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

    mandelbrot_painting mandelbrot_struct = {};

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    mandelbrot_struct.x_center -= mandelbrot_struct.scale* 10.f;
                }
                else if (event.key.code == sf::Keyboard::D)
                {
                    mandelbrot_struct.x_center += mandelbrot_struct.scale* 10.f;
                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    mandelbrot_struct.y_center -= mandelbrot_struct.scale* 10.f;
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    mandelbrot_struct.y_center += mandelbrot_struct.scale* 10.f;
                }
                else if (event.key.code == sf::Keyboard::X)
                {
                    mandelbrot_struct.scale -= mandelbrot_struct.scale * 0.1f;
                }
                else if (event.key.code == sf::Keyboard::Z)
                {
                    mandelbrot_struct.scale += mandelbrot_struct.scale * 0.1f;
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        printing_madnelbrot(&window, &mandelbrot_struct);
        print_fps(&clock, &previousTime, &window, &font);

        window.display();
    }

}