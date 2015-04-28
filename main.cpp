#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// Shaders
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec3 position;"
    "void main() {"
    "   gl_Position = vec4(position, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150 core\n"
    "uniform vec3 color;"
    "out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(color, 1.0);"
    "}";

int main () {
    // Start OpenGL, GLEW and SFML
    sf::Window window(sf::VideoMode(800, 600), "OpenGL");
    sf::Clock clock;
    glViewport(0, 0, 800, 600);
    glewExperimental = GL_TRUE;
    glewInit();
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    // Set up variables
    int fade = 0, dfade = 1;
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint ebo;
    glGenBuffers(1, &ebo);
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    GLint uniColor = glGetUniformLocation(shaderProgram, "color");
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    // Set up Main Loop
    bool running = true;
    long long tick = 0;
    clock.restart();
    // Main Loop
    while (running) {
        // Event Handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) running = false;
            else if (event.type == sf::Event::Resized) glViewport(0, 0, event.size.width, event.size.height);
        }
        // 60 FPS
        tick += clock.restart().asMicroseconds();
        if (tick >= 16667) {
            tick -= 16667;
            // Render Frame
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            fade += dfade;
            if (fade == 120 || fade == 0) dfade =- dfade;
            glUniform3f (uniColor, fade/120.0f, 1.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            window.display();
        }
    }
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return 0;
}
