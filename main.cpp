#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>
#include <memory.h>

#include <cmath>
#include <glm/vec4.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Point {
    float x, y, z, radius, multiplier;
    Point(const float x, const float y, const float z,
          const float radius = 1.0f, const float multiplier = 1.0f)
        : x(x), y(y), z(z), radius(radius), multiplier(multiplier) {}
};

/* userspace begin*/

#define WIDTH 600
#define HEIGHT 600
#define TITLE "abracadabra"
#define VERTICES_COUNT 8

// #define VERTICES_DRAW_TYPE GL_POINTS
// #define VERTICES_DRAW_TYPE GL_LINES
#define VERTICES_DRAW_TYPE GL_LINE_LOOP
// #define VERTICES_DRAW_TYPE GL_LINE_STRIP
// #define VERTICES_DRAW_TYPE GL_TRIANGLES
// #define VERTICES_DRAW_TYPE GL_TRIANGLE_STRIP
// #define VERTICES_DRAW_TYPE GL_TRIANGLE_FAN

const glm::vec4 back_color{0.0f, 0.0f, 0.0f, 1.0f};
// const glm::vec4 line_color{0.0f, 1.0f, 0.0f, 1.0f}; // TODO
#define LINE_COLOR "0.0,1.0,0.0,1.0"

float SPEED = .5f;
float SCALE = .5f;
float IMPACT = 1.0f;

const std::vector<Point> points{
    Point(-0.5f, 0.0f, 0.0f, 1.0f, 1.0f),    // left
    Point(-0.5f, 0.5f, 0.0f, 1.0f, 7.0f),    // top-left
    Point(0.0f, 0.5f, 0.0f, 1.0f, 7.0f),     // top
    Point(0.5f, 0.5f, 0.0f, 1.0f, 9.0f),    // top-right
    Point(0.5f, 0.0f, 0.0f, 1.0f, 24.0f),    // right
    Point(0.5f, -0.5f, 0.0f, 1.0f, 13.0f),   // bottom-right
    Point(0.0f, -0.5f, 0.0f, 1.0f, 3.0f),   // bottom
    Point(-0.5f, -0.5f, 0.0f, 1.0f, 14.0f),  // bottom-left
};

/* userspace end*/

std::vector<Point> _points{points};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, char* argv[]) {
    // read arguments
    if (argc > 1) SPEED = atof(argv[1]);
    if (argc > 2) SCALE = atof(argv[2]);
    if (argc > 3) IMPACT = atof(argv[2]);

    // window init
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
    } else {
        std::cout << "Created window" << std::endl;
    }

    glfwMakeContextCurrent(window);

    // graphics init
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    const char* vShaderSrc =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fShaderSrc =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(" LINE_COLOR
        ");\n"
        "}\0";
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSrc, NULL);
    glCompileShader(vShader);
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSrc, NULL);
    glCompileShader(fShader);
    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vShader);
    glAttachShader(shader, fShader);
    glLinkProgram(shader);
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    glUseProgram(shader);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);

    glClearColor(back_color.r, back_color.g, back_color.b, back_color.a);

    // bind everything just to be sure
    glUseProgram(shader);
    glBindVertexArray(VAO);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // compute data
        double time = glfwGetTime();
        for (unsigned int i = 0; i < _points.size(); i++) {
            _points[i].x =
                (points[i].x + _points[i].radius *
                                   sin(_points[i].multiplier * time * SPEED) *
                                   IMPACT) *
                SCALE;
            _points[i].y =
                (points[i].y + _points[i].radius *
                                   cos(_points[i].multiplier * time * SPEED) *
                                   IMPACT) *
                SCALE;
        }

        // send data to the gpu
        glBufferData(GL_ARRAY_BUFFER, _points.size() * sizeof(Point),
                     &_points[0], GL_STREAM_DRAW);

        // draw
        glDrawArrays(VERTICES_DRAW_TYPE, 0, _points.size());

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // exit
    glfwTerminate();
    return 0;
}
