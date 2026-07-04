#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include "glad/glad.h"

int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsApplicationWindow = NULL;
SDL_GLContext gOpenGLContext = NULL;
bool gQuit = false;

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

}

void Draw()
{

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

    MainLoop();

    CleanUp();

    return 0;
}
