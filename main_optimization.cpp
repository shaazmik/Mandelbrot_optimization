#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <immintrin.h>


static const int WIDTH = 900, HEIGHT = 600;
static const int Fps_buf_size = 1024;
static float Fps = 0;
static const float center_shift = 10.f, scale_shift = 0.1f;
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

    const int nMax  = 256;
    
    double x_center = mandelbrot_struct->x_center,
           y_center = mandelbrot_struct->y_center,
           scale    = mandelbrot_struct->scale,
           dx       = mandelbrot_struct->dx,
           dy       = mandelbrot_struct->dy;


    float scale_dx  = scale * dx;


    const __m256    _rmax     = _mm256_set1_ps(100.f);
    const __m256i   _nMax     = _mm256_set1_epi32(nMax);
    const __m256    _scale_dx = _mm256_set1_ps(scale_dx);
    const __m256    _iter     = _mm256_set_ps(7, 6, 5, 4, 3, 2, 1, 0);
    const __m256    _255      = _mm256_set1_ps(255.f);


    sf::Uint8* pixels = mandelbrot_struct->pixels;


    for (int yi = 0; yi < HEIGHT; yi++)
    {
        float  x0 = (            -  WIDTH/2 ) * scale_dx + x_center  * dx,
               y0 = (((double)yi -  HEIGHT/2) * scale    + y_center) * dy;

        float x0_dif = 8 * scale_dx;

        for (int xi = 0; xi < 4 * WIDTH; xi+= 32, x0 += x0_dif)
        {
            __m256 _x0 = _mm256_add_ps(_mm256_set1_ps(x0), _mm256_mul_ps(_iter, _scale_dx)),
                   _y0 = _mm256_set1_ps(y0);

            __m256i _n = _mm256_setzero_si256();

            __m256 _cur_x = _x0,
                   _cur_y = _y0;

            for (int n = 0; n < nMax; n++)
            {
                __m256 _sqr_x = _mm256_mul_ps(_cur_x, _cur_x), 
                       _sqr_y = _mm256_mul_ps(_cur_y, _cur_y), 
                       _xy    = _mm256_mul_ps(_cur_x, _cur_y);
                
                __m256 _sqr_r = _mm256_add_ps(_sqr_x, _sqr_y);

                __m256 _cmp   = _mm256_cmp_ps(_sqr_r, _rmax, _CMP_LE_OQ);
                int    mask   = _mm256_movemask_ps(_cmp);
                if (!mask) break;

                _n = _mm256_sub_epi32(_n, _mm256_castps_si256(_cmp));

                _cur_x = _mm256_add_ps(_mm256_sub_ps(_sqr_x, _sqr_y), _x0);
                _cur_y = _mm256_add_ps(_mm256_add_ps(_xy, _xy), _y0);                
            }

            __m256 _color_base = _mm256_mul_ps(_mm256_sqrt_ps(_mm256_div_ps(_mm256_cvtepi32_ps(_n), _mm256_cvtepi32_ps(_nMax) ) ), _255);
            for (int i = 0; i < 8; i++)
            {
                int*   p_n          = (int*)   &_n;
                float* p_color_base = (float*) &_color_base;
                
                unsigned char intensive = p_color_base[i];
                int offset_Y = WIDTH * 4 * yi;

                if (p_n[i] < nMax)
                {
                    pixels[xi + offset_Y + 0 + i * 4] = (unsigned char) intensive / 1.6;
                    pixels[xi + offset_Y + 1 + i * 4] = (unsigned char) intensive * 2.1;
                    pixels[xi + offset_Y + 2 + i * 4] = (unsigned char) intensive * 4.9;
                    pixels[xi + offset_Y + 3 + i * 4] = intensive;
                }
                else
                {
                    pixels[xi + offset_Y + 0 + i * 4] = 0;
                    pixels[xi + offset_Y + 1 + i * 4] = 0;
                    pixels[xi + offset_Y + 2 + i * 4] = 0;
                    pixels[xi + offset_Y + 3 + i * 4] = 0;
                }
            }
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
                    mandelbrot_struct.x_center -= mandelbrot_struct.scale * center_shift;
                }
                else if (event.key.code == sf::Keyboard::D)
                {
                    mandelbrot_struct.x_center += mandelbrot_struct.scale * center_shift;
                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    mandelbrot_struct.y_center -= mandelbrot_struct.scale * center_shift;
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    mandelbrot_struct.y_center += mandelbrot_struct.scale * center_shift;
                }
                else if (event.key.code == sf::Keyboard::X)
                {
                    mandelbrot_struct.scale -= mandelbrot_struct.scale * scale_shift;
                }
                else if (event.key.code == sf::Keyboard::Z)
                {
                    mandelbrot_struct.scale += mandelbrot_struct.scale * scale_shift;
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