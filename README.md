# Mandelbrot_optimization

![Image alt](https://github.com/shaazmik/Mandelbrot_optimization/blob/main/Mandelbrot.PNG)

## Program uses AVX instruction to optimise
____

- graphics:    SFML library
- instructions: AVX and SSE
- OS: Win10
  
compile:
> g++ -I"library path"\SFML-2.5.1\include -c main_optimization.cpp -O2 -mavx -mavx2 -msse4 -lm -o main_optimization.o
  
> g++ -L"library path"\SFML-2.5.1\lib .\main_optimization.o -o name_file.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main -mwindows

compile without instructions:
> g++ -I"library path"\SFML-2.5.1\include -c main_no_optimization.cpp -O2 -o main_optimization.o
  
> g++ -L"library path"\SFML-2.5.1\lib .\main_no_optimization.o -o name_file.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main -mwindows  
____


### Results:

| **COMPILED**        | ___FPS___           |
|---------------------|---------------------|
|**WITH OPTIMIZATION**| **58**              |
|___ONLY ```-O2```___ |  **6**              |

```TOTAL INCREASE: 9,6 TIMES```
