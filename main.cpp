#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

/// To make executable standalone on Linux, run:
/// export LD_LIBRARY_PATH=/home/oliver/.SFML/lib && ./Project

int main () {
    sf::Window window(sf::VideoMode(800, 600), "OpenGL");
    sf::Clock clock;
    int fade = 0, dfade = 1;
    long long tick = 0;
    bool running = true;
    clock.restart();
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) running = false;
            else if (event.type == sf::Event::Resized) glViewport(0, 0, event.size.width, event.size.height);
        }
        tick += clock.restart().asMicroseconds();
        if (tick >= 16667) {
            tick -= 16667;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            fade += dfade;
            if (fade == 100 || fade == 0) dfade =- dfade;
            glColor3f (fade/100.0, 1.0, 1.0);
            glBegin(GL_POLYGON);
                glVertex3f (-0.5, -0.5, 0.0);
                glVertex3f (-0.5, 0.5, 0.0);
                glVertex3f (0.5, 0.5, 0.5);
                glVertex3f (0.5, -0.5, 0.5);
            glEnd();
            window.display();
        }
    }
    return 0;
}
