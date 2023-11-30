// Autor: Nedeljko Tesanovic
// Opis: Zestoko iskomentarisan program koji crta sareni trougao u OpenGL-u

#define _CRT_SECURE_NO_WARNINGS
 //Biblioteke za stvari iz C++-a (unos, ispis, fajlovi itd) 
#include <iostream>
#include <fstream>
#include <sstream>

//Biblioteke OpenGL-a
#include <GL/glew.h>   //Omogucava upotrebu OpenGL naredbi
#include <GLFW/glfw3.h>//Olaksava pravljenje i otvaranje prozora (konteksta) sa OpenGL sadrzajem

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CRES 30 // Circle Resolution = Rezolucija kruga

unsigned int compileShader(GLenum type, const char* source); //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
unsigned int createShader(const char* vsSource, const char* fsSource); //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource i Fragment sejdera na putanji fsSource
static unsigned loadImageToTexture(const char* filePath);


int main(void)
{
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window; 
    unsigned int wWidth = 800;
    unsigned int wHeight = 800;
    const char wTitle[] = "Exhibition at Louvre";

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int xPos = (mode->width - wWidth) / 2;
    int yPos = (mode->height - wHeight) / 2;

    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    glfwSetWindowPos(window, xPos, yPos);

    if (window == NULL)
    {
        std::cout << "Window hasn't been created! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Couldn't load GLEW! :'(\n";
        return 3;
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ BUFFERS AND VARIABLES +++++++++++++++++++++++++++++++++++++++++++++++++

    float white = 255 / 255.0;

    float yellow_r = 255 / 255.0;
    float yellow_g = 255 / 255.0;
    float yellow_b = 0 / 255.0;

    float red_r = 255 / 255.0;
    float red_g = 0 / 255.0;
    float red_b = 0 / 255.0;

    float purple_r = 255 / 255.0;
    float purple_g = 0 / 255.0;
    float purple_b = 255 / 255.0;

    float blue_r = 0 / 255.0;
    float blue_g = 0 / 255.0;
    float blue_b = 255 / 255.0;
    
    float green_r = 128 / 255.0;
    float green_g = 255 / 255.0;
    float green_b = 128 / 255.0;

    unsigned int VAO[12];
    glGenVertexArrays(12, VAO);
    unsigned int VBO[12];
    glGenBuffers(12, VBO);

    unsigned int textureStride = (2 + 2) * sizeof(float);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ NAME TEXTURE +++++++++++++++++++++++++++++++++++++++++++++++++

    float nameTextureVertices[] =
    {   //X    Y      S    T 
        -0.63, -0.45, 0.0, 1.0,
        0.72, -0.45, 1.0, 1.0,
        -0.63, -0.8, 0.0, 0.0,
        0.72, -0.8, 1.0, 0.0
    };
    

    glBindVertexArray(VAO[0]); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(nameTextureVertices), nameTextureVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned nameTexture = loadImageToTexture("res/name_texture.png");
    glBindTexture(GL_TEXTURE_2D, nameTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROGRESS BAR +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int coordsRGBStride = 5 * sizeof(float);

    float progressBarOuterVertices[] = {
        0.1, -0.76,     white, white, white,
      -0.7, -0.76,      white, white, white,
      0.1, -0.86,     white, white, white,
      -0.7, -0.86,   white, white, white
    };

    glBindVertexArray(VAO[10]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(progressBarOuterVertices), progressBarOuterVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float progressBarInnerVertices[] = {
       -0.65, -0.84, green_r, green_g, green_b,
       -0.65, -0.78, green_r, green_g, green_b,
       -0.475, -0.84, green_r, green_g, green_b,
       -0.475, -0.78, green_r, green_g, green_b,
       -0.3, -0.84, green_r, green_g, green_b,
       -0.3, -0.78, green_r, green_g, green_b,
       -0.1275, -0.84, green_r, green_g, green_b,
       -0.1275, -0.78, green_r, green_g, green_b,
       0.05, -0.84, green_r, green_g, green_b,
       0.05, -0.78, green_r, green_g, green_b,
    };

    glBindVertexArray(VAO[11]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(progressBarInnerVertices), progressBarInnerVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ MONA LISA +++++++++++++++++++++++++++++++++++++++++++++++++

    float monaLisaVertices[] =
    {   //X    Y      S    T 
        0.5, -0.85, 1.0, 0.0,
        -0.75, -0.85, 0.0, 0.0,
        0.5, 0.8, 1.0, 1.0,
        -0.75, 0.8, 0.0, 1.0,
    };

    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(monaLisaVertices), monaLisaVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned monaLisaTexture = loadImageToTexture("res/mona_lisa.jpg");
    glBindTexture(GL_TEXTURE_2D, monaLisaTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 1);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ MEDUSA +++++++++++++++++++++++++++++++++++++++++++++++++

    float medusaVertices[] =
    {   //X    Y      S    T 
       0.5, -0.15, 1.0, 0.0,
        -0.75, -0.15, 0.0, 0.0,
        0.5, 0.55, 1.0, 1.0,
        -0.75, 0.55, 0.0, 1.0, 
    };

    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(medusaVertices), medusaVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned medusaTexture = loadImageToTexture("res/medusaa.jpg");
    glBindTexture(GL_TEXTURE_2D, medusaTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 2);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ NAPOLEON +++++++++++++++++++++++++++++++++++++++++++++++++

    float napoleonVertices[] =
    {   //X    Y      S    T 
       0.7, -0.5, 1.0, 0.0,
        -0.65, -0.5, 0.0, 0.0,
        0.7, 0.4, 1.0, 1.0,
        -0.65, 0.4, 0.0, 1.0,
    };

    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(napoleonVertices), napoleonVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned napoleonTexture = loadImageToTexture("res/napoleon.jpg");
    glBindTexture(GL_TEXTURE_2D, napoleonTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 3);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ FREEDOM +++++++++++++++++++++++++++++++++++++++++++++++++

    float freedomVertices[] =
    {   //X    Y      S    T 
       0.7, -0.15, 1.0, 0.0,
        -0.65, -0.15, 0.0, 0.0,
        0.7, 0.85, 1.0, 1.0,
        -0.65, 0.85, 0.0, 1.0,
    };

    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(freedomVertices), freedomVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned freedomTexture = loadImageToTexture("res/liberty.jpg");
    glBindTexture(GL_TEXTURE_2D, freedomTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 4);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ BUTTON +++++++++++++++++++++++++++++++++++++++++++++++++

    float circle[CRES * 2 + 4];
    float r = 0.15;

    circle[0] = -0.68;
    circle[1] = -0.62;
    int i;
    for (i = 0; i <= CRES; i++)
    {

        circle[2 + 2 * i] = circle[0] + r * cos((3.141592 / 180) * (i * 360 / CRES));
        circle[2 + 2 * i + 1] = circle[1] + r * sin((3.141592 / 180) * (i * 360 / CRES)) * ((float)wWidth / (int)wHeight);
    }

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ YELLOW FRAME +++++++++++++++++++++++++++++++++++++++++++++++++

    float yellowFrameVertices[] = {
        0.55, 0.85,     yellow_r, yellow_g, yellow_b,
      0.5, 0.8,      yellow_r, yellow_g, yellow_b,
      0.55, -0.9,     yellow_r, yellow_g, yellow_b,
      0.5, -0.85,   yellow_r, yellow_g, yellow_b,
      -0.8, -0.9,    yellow_r, yellow_g, yellow_b,
      -0.75, -0.85,     yellow_r, yellow_g, yellow_b,
      -0.8, 0.85,     yellow_r, yellow_g, yellow_b,
       -0.75, 0.8,      yellow_r, yellow_g, yellow_b,
      0.55, 0.85,      yellow_r, yellow_g, yellow_b,
      0.5, 0.8,      yellow_r, yellow_g, yellow_b
    };

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yellowFrameVertices), yellowFrameVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RED FRAME +++++++++++++++++++++++++++++++++++++++++++++++++

    float redFrameVertices[] = {
        0.55, 0.6,     red_r, red_g, red_b,
       0.5, 0.55,      red_r, red_g, red_b,
       0.55, -0.2,     red_r, red_g, red_b,
       0.5, -0.15,   red_r, red_g, red_b,
       -0.8, -0.2,   red_r, red_g, red_b,
       -0.75, -0.15,     red_r, red_g, red_b,
       -0.8, 0.6,     red_r, red_g, red_b,
        -0.75, 0.55,      red_r, red_g, red_b,
       0.55, 0.6,     red_r, red_g, red_b,
       0.5, 0.55,      red_r, red_g, red_b,
    };

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(redFrameVertices), redFrameVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ BLUE FRAME +++++++++++++++++++++++++++++++++++++++++++++++++

    float blueFrameVertices[] = {
          0.75, 0.45,     blue_r, blue_g, blue_b,
         0.7, 0.4,      blue_r, blue_g, blue_b,
         0.75, -0.55,     blue_r, blue_g, blue_b,
         0.7, -0.5,   blue_r, blue_g, blue_b,
         -0.7, -0.55,   blue_r, blue_g, blue_b,
         -0.65, -0.5,     blue_r, blue_g, blue_b,
         -0.7, 0.45,     blue_r, blue_g, blue_b,
          -0.65, 0.4,      blue_r, blue_g, blue_b,
         0.75, 0.45,     blue_r, blue_g, blue_b,
         0.75, 0.40,      blue_r, blue_g, blue_b,
    };

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueFrameVertices), blueFrameVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PURPLE FRAME +++++++++++++++++++++++++++++++++++++++++++++++++

    float purpleFrameVertices[] = {
          0.75, 0.9,     purple_r, purple_g, purple_b,
         0.7, 0.85,      purple_r, purple_g, purple_b,
         0.75, -0.2,     purple_r, purple_g, purple_b,
         0.7, -0.15,   purple_r, purple_g, purple_b,
         -0.7, -0.2,   purple_r, purple_g, purple_b,
         -0.65, -0.15,     purple_r, purple_g, purple_b,
         -0.7, 0.9,     purple_r, purple_g, purple_b,
          -0.65, 0.85,      purple_r, purple_g, purple_b,
         0.75, 0.9,     purple_r, purple_g, purple_b,
         0.7, 0.85,      purple_r, purple_g, purple_b,
    };

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(purpleFrameVertices), purpleFrameVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, coordsRGBStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ SHADERS, UNIFORMS AND VARIABLES +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int basicTextureShader = createShader("basic_texture.vert", "basic_texture.frag");
    unsigned int progressBarShader = createShader("basic.vert", "progress_bar.frag");
    unsigned int buttonShader = createShader("basic.vert", "uniform_color.frag");
    unsigned int basicShader = createShader("basic.vert", "basic.frag");
    unsigned int rotationShader = createShader("frame_rotation.vert", "frame_rotation.frag");

    unsigned uTexLoc = glGetUniformLocation(basicTextureShader, "uTex");
    unsigned int time = glGetUniformLocation(rotationShader, "time");
    unsigned int timeProgress = glGetUniformLocation(progressBarShader, "time");
    unsigned int frameType = glGetUniformLocation(rotationShader, "type");
    unsigned int uPosLoc = glGetUniformLocation(rotationShader, "uPos");
    unsigned int uColorLoc = glGetUniformLocation(buttonShader, "color");

    bool buttonClicked = false;
    float rotationSpeed = 0.7;
    int progressLevel = 4;
    bool zKeyPressed = false;
    bool iKeyPressed = false;
    int isImageMoving = 1;

    glUniform4f(uColorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
    glClearColor(0.9, 0.9, 0.9, 1.0);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP +++++++++++++++++++++++++++++++++++++++++++++++++

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glViewport(0, 0, wWidth, wHeight);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            buttonClicked = true;
            isImageMoving = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            buttonClicked = false;
            isImageMoving = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !zKeyPressed && progressLevel < 10) {
            progressLevel += 2;
            rotationSpeed += 0.5;
            zKeyPressed = true; 
        }
        else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) {
            zKeyPressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !iKeyPressed && progressLevel > 4) {
            progressLevel -= 2;
            rotationSpeed -= 0.5;
            iKeyPressed = true;
        }
        else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) {
            iKeyPressed = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ NAME TEXTURE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(wWidth / 2 + 1, 0, wWidth / 2, wHeight / 2);

        glUseProgram(basicTextureShader);
        glUniform1i(uTexLoc, 0);
        glUniform2f(uPosLoc, 0, 0);

        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, nameTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ BUTTON ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


        glUseProgram(buttonShader);
        if (!buttonClicked) {
            glUniform4f(uColorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
        }
        else {
            glUniform4f(uColorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        }
        glBindVertexArray(VAO[1]);
        glViewport(0, 0, wWidth / 2, wHeight / 2);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ PROGRESS BAR ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glUseProgram(basicTextureShader);

        glViewport(0, 0, wWidth, wHeight);
        glBindVertexArray(VAO[10]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glUseProgram(progressBarShader);

        glUniform1f(timeProgress, abs(sin(glfwGetTime())));
        glBindVertexArray(VAO[11]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, progressLevel);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ TEXTURES ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glUseProgram(basicTextureShader);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ MONA LISA ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(wWidth / 2 + 1, wHeight / 2 + 1, wWidth / 2, wHeight / 2);

        glBindVertexArray(VAO[6]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, monaLisaTexture);
        glUniform1i(uTexLoc, 1);
        glUniform2f(uPosLoc, isImageMoving * 0.05 * cos(glfwGetTime() * rotationSpeed), isImageMoving * 0.05 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ MEDUSA ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(wWidth / 2 + 1, 0, wWidth / 2, wHeight / 2);

        glBindVertexArray(VAO[7]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, medusaTexture);
        glUniform1i(uTexLoc, 2);
        glUniform2f(uPosLoc, isImageMoving * 0.2 * cos(glfwGetTime() * rotationSpeed), isImageMoving * 0.2 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ FREEDOM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(0, 0, wWidth / 2, wHeight / 2);

        glBindVertexArray(VAO[9]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, freedomTexture);
        glUniform1i(uTexLoc, 4);
        glUniform2f(uPosLoc, isImageMoving * 0.05 * cos(glfwGetTime() * rotationSpeed), isImageMoving * 0.05 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ NAPOLEON ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(0, wHeight / 2 + 1, wWidth / 2, wHeight / 2);

        glBindVertexArray(VAO[8]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, napoleonTexture);
        glUniform1i(uTexLoc, 3);
        glUniform2f(uPosLoc, isImageMoving * 0.2 * cos(glfwGetTime() * rotationSpeed), isImageMoving * 0.2 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ BUTTON CLICK ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        if (!buttonClicked) {
            glUniform4f(uColorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glClearColor(0.9, 0.9, 0.9, 1.0);
        }
        else {
            glUniform4f(uColorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
            glClearColor(0.3, 0.3, 0.3, 1.0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }        

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ FRAMES ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glUseProgram(rotationShader);
        glUniform1f(time, abs(sin(glfwGetTime())));

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ YELLOW FRAME ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(wWidth / 2 + 1, wHeight / 2 + 1, wWidth / 2, wHeight / 2);

        glUniform1f(frameType, 1);
        glBindVertexArray(VAO[2]);
        glUniform2f(uPosLoc, isImageMoving * 0.05 * cos(glfwGetTime() * rotationSpeed), isImageMoving * 0.05 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(yellowFrameVertices) / coordsRGBStride);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ RED FRAME ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(wWidth / 2 + 1, 0, wWidth / 2, wHeight / 2);

        glUniform1f(frameType, 2);
        glBindVertexArray(VAO[3]);
        glUniform2f(uPosLoc, isImageMoving * 0.2* cos(glfwGetTime()* rotationSpeed), isImageMoving * 0.2* (sin(glfwGetTime()* rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(redFrameVertices) / coordsRGBStride);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ BLUE FRAME ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(0, wHeight / 2 + 1, wWidth / 2, wHeight / 2);

        glBindVertexArray(VAO[4]);
        glUniform1f(frameType, 3);
        glUniform2f(uPosLoc, isImageMoving * 0.2 * cos(glfwGetTime() * rotationSpeed), isImageMoving * 0.2 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(blueFrameVertices) / coordsRGBStride);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ PURPLE FRAME ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glViewport(0, 0, wWidth / 2, wHeight / 2);

        glBindVertexArray(VAO[5]);
        glUniform1f(frameType, 4);
        glUniform2f(uPosLoc, isImageMoving * 0.05 * cos(glfwGetTime() * rotationSpeed), isImageMoving * 0.05 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(purpleFrameVertices) / coordsRGBStride);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ RESET TO DEFAULT ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glActiveTexture(GL_TEXTURE0);
        glUniform2f(uPosLoc, 0, 0);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(10, VBO);
    glDeleteTextures(1, &nameTexture);
    glDeleteTextures(1, &monaLisaTexture);
    glDeleteTextures(1, &napoleonTexture);
    glDeleteTextures(1, &freedomTexture);
    glDeleteTextures(1, &medusaTexture);

    glDeleteVertexArrays(10, VAO);
    glDeleteProgram(basicTextureShader);
    glDeleteProgram(buttonShader);
    glDeleteProgram(progressBarShader);
    glDeleteProgram(basicShader);
    glDeleteProgram(rotationShader);
    glfwTerminate();

    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    //Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); //Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); //Napravimo prazan sejder odredjenog tipa (vertex ili fragment)
    
    int success; //Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; //Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); //Postavi izvorni kod sejdera
    glCompileShader(shader); //Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); //Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; //Objedinjeni sejder
    unsigned int vertexShader; //Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; //Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); //Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); //Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); //Napravi i kompajliraj fragment sejder

    //Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); //Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); //Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); //Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    //Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo zarad ustede na memoriji
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}