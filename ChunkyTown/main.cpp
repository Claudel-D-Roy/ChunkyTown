#include "engine.h"

#include <SFML/Window.hpp>
#include <iostream>

int main()
{
    Engine engine;
    engine.SetMaxFps(60);
    engine.Start("ChunkyTown", 800, 600, false);
   
}
