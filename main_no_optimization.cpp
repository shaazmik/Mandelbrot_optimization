#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <string>


static const int WIDTH = 900, HEIGHT = 600;
static const int Fps_buf_size = 1024;
static float Fps = 0;
static int character_size = 15;

void calculate()
{
    
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

        calculate();

        print_fps(&clock, &previousTime, &window, &font);

        window.display();
    }

}