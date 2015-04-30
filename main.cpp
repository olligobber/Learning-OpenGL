#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <string>

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

void checkGLerror(std::string context) {
    GLenum error = glGetError();
    while(error!=GL_NO_ERROR){
        std::cout << "Error: " << context << " : " << gluErrorString(error) << std::endl;
        error = glGetError();
    }
}

int main () {
    // Start OpenGL, GLEW and SFML
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
    sf::Clock clock;
    glViewport(0, 0, 800, 600);
    checkGLerror("Initialise window");
    glewExperimental = GL_TRUE;
    glewInit();
    checkGLerror("GLEW Init");
    GLuint vao;
    glGenVertexArrays(1, &vao);
    checkGLerror("Gen Vertex Arrays");
    glBindVertexArray(vao);
    checkGLerror("Bind Vertex Arrays");
    GLuint vbo;
    glGenBuffers(1, &vbo);
    checkGLerror("Gen Vertex Buffers");
    // Set up variables
    int fade = 0, dfade = 1;
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLerror("Bind Vertex Buffer Array");
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    checkGLerror("Buffer Vertex Data");
    GLuint ebo;
    glGenBuffers(1, &ebo);
    checkGLerror("Gen Element Buffers");
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    checkGLerror("Bind Element Buffer Array");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    checkGLerror("Buffer Element Data");
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkGLerror("Compile Vertex Shader");
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        std::cout << "Compilation of Vertex Shader failed:" << std::endl;
        int len, written;
        char* log = new char[len];
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &len);
        glGetShaderInfoLog(vertexShader, len, &written, log);
        std::cout << log << std::endl << "End log" << std::endl;
    }
    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkGLerror("Compile Fragment Shader");
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        std::cout << "Compilation of Fragment Shader failed:" << std::endl;
        int len, written;
        char* log = new char[len];
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &len);
        glGetShaderInfoLog(fragmentShader, len, &written, log);
        std::cout << log << std::endl << "End log" << std::endl;
    }
    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    checkGLerror("Create Shader Program");
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    checkGLerror("Attach Shaders");
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    checkGLerror("Bind Shader's Color Output");
    glLinkProgram(shaderProgram);
    checkGLerror("Link Shader Program");
    glUseProgram(shaderProgram);
    checkGLerror("Use Shader Program");
    GLint uniColor = glGetUniformLocation(shaderProgram, "color");
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    checkGLerror("Enable Vertex Array");
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
            checkGLerror("Clear Screen");
            fade += dfade;
            if (fade == 120 || fade == 0) dfade =- dfade;
            glUniform3f (uniColor, fade/120.0f, 1.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            checkGLerror("Draw to Screen");
            window.display();
        }
    }
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    checkGLerror("End of Program");
    return 0;
}
