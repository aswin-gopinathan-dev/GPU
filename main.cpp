#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "glad/glad.h"

int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsApplicationWindow = NULL;
SDL_GLContext gOpenGLContext = NULL;
bool gQuit = false;
GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;

// Program object for shaders
GLuint gGraphicsPipelineShaderProgram = 0;

const std::string gVertexShaderSource = 
    "#version 410 core\n"
    "in vec4 position;\n"
    "void main()\n"
    "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
    "}\n";

const std::string gFragmentShaderSource = 
    "#version 410 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
        "color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
    "}\n";

void GetOpenGLVersion()
{
    std::cout<< "Vendor: "<< glGetString(GL_VENDOR) << std::endl;
    std::cout<< "Renderer: "<< glGetString(GL_RENDERER) << std::endl;
    std::cout<< "Version: "<< glGetString(GL_VERSION) << std::endl;
    std::cout<< "Shading Language: "<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

// Initialize SDL and GLAD library
void InitializeProgram()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout<<"SDL2 init failed"<<std::endl;
        exit(1);
    }


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // OpenGL version to use
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Use Modern OpenGL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Enable double buufer
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // Depth buffer bits


    gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL Window", 0, 0, 
                                                  gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
    if(gGraphicsApplicationWindow == NULL)
    {
        std::cout<<"SDL Window creation failed\n";
        exit(1);
    }

    // Create OpenGL Context
    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
    if(gOpenGLContext == NULL)
    {
        std::cout << "OpenGL Context creation failed\n";
        exit(1);
    }

    // Initialize GLAD library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout<< "GLAD init failed\n";
        exit(1);
    }

    GetOpenGLVersion();
}

void Input()
{
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
            gQuit = true;
    }
}

void PreDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.0f, 1.0f, 0.0f, 1.1f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw()
{
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void VertexSpecification()
{
    // Lives on CPU
    const std::vector<GLfloat> vertexPosition
    {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };

    
    // Setup the GPU part
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint shaderObject;
    if(type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, NULL);
    glCompileShader(shaderObject);

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string&vs, const std::string& fs)
{
    GLuint programObject = glCreateProgram();

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint fragShader = CompileShader(GL_FRAGMENT_SHADER, fs);

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragShader);
    glLinkProgram(programObject);

    glValidateProgram(programObject);

    return programObject;
}

void CreateGraphicsPipeline()
{
    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}

void MainLoop()
{
    while(!gQuit)
    {
        Input();
        PreDraw();
        Draw();

        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
}

void CleanUp()
{
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main() 
{
    InitializeProgram();

    VertexSpecification();

    CreateGraphicsPipeline();

    MainLoop();

    CleanUp();

    return 0;
}
