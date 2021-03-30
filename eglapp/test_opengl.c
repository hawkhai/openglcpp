#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

//https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp
#define TRUE 1

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 vColor;\n"
"void main()\n"
"{\n"
"    vColor = aColor;\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vColor;\n"
"void main()\n"
"{\n"
"    FragColor = vColor;\n"
"}\n\0";

void clearProgram(unsigned int shaderProgram) {
    glDeleteProgram(shaderProgram);
}

void clearData(unsigned int VBO, unsigned int VAO, unsigned int EBO) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

unsigned int createProgram() {

    unsigned int shaderProgram;

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

unsigned int uploadData(GLuint* pVBO, GLuint* pVAO, GLuint* pEBO, int flag) {

    float vertices[] = {
         0.5f,  0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 0.7f, // top right
         0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 0.7f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.1f, 0.9f, 0.1f, 0.7f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.1f, 0.1f, 0.7f, 0.7f, // top left 
    };
    float vertices2[] = {
     0.5f + 0.2f,  0.5f + 0.2f, 0.0f + 0.2f, 1.0f, 1.0f, 1.0f, 0.2f, // top right
     0.5f + 0.2f, -0.5f + 0.2f, 0.0f + 0.2f, 1.0f, 1.0f, 1.0f, 0.2f, // bottom right
    -0.5f + 0.2f, -0.5f + 0.2f, 0.0f + 0.2f, 1.0f, 1.0f, 1.0f, 0.2f, // bottom left
    -0.5f + 0.2f,  0.5f + 0.2f, 0.0f + 0.2f, 1.0f, 1.0f, 1.0f, 0.2f, // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, pVAO);
    glGenBuffers(1, pVBO);
    glGenBuffers(1, pEBO);

    glBindVertexArray(*pVAO);
    if (TRUE) {
        glBindBuffer(GL_ARRAY_BUFFER, *pVBO);
        if (flag == 2) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
        }
        else {
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }
        // GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer
        // size 每组数据用到的个数
        // stride 一组数据的大小 sizeof(Vertex)
        // pointer 每组数据内部偏移
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    if (TRUE) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer
        // object IS stored in the VAO; keep the EBO bound.
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
    return *pVAO;
}

int main_opengl(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    unsigned int shaderProgram = createProgram();

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    uploadData(&VBO, &VAO, &EBO, 1);
    unsigned int VBO2;
    unsigned int VAO2;
    unsigned int EBO2;
    uploadData(&VBO2, &VAO2, &EBO2, 2);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glEnable(GL_BLEND); // 启用混合
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        //Sleep(20);
    }

    clearProgram(shaderProgram);
    clearData(VBO, VAO, EBO);
    clearData(VBO2, VAO2, EBO2);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
