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
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROMJENLJIVE I BAFERI +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int VAO[6];
    glGenVertexArrays(6, VAO);
    unsigned int VBO[6];
    glGenBuffers(6, VBO);

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

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ NAME TEXTURE +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int nameAndSurnameShader = createShader("basic_texture.vert", "basic_texture.frag"); 

    float vertices[] =
    {   //X    Y      S    T 
        -0.2, -0.55,  0.0, 0.0,
        0.2, -0.55,   1.0, 0.0,
        -0.2, -0.4,    0.0, 1.0,
        0.2, -0.4,    1.0, 1.0,//trece tjeme
    };
    unsigned int nameTextureStride = (2 + 2) * sizeof(float); 

    glBindVertexArray(VAO[0]); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, nameTextureStride, (void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, nameTextureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);*/

    unsigned checkerTexture = loadImageToTexture("res/name_texture.png");
    glBindTexture(GL_TEXTURE_2D, checkerTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned uTexLoc = glGetUniformLocation(nameAndSurnameShader, "uTex");
    glUniform1i(uTexLoc, 0);


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ BUTTON +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int buttonShader = createShader("basic.vert", "uniform_color.frag");
    unsigned int basicShader = createShader("basic.vert", "basic.frag");

    float circle[CRES * 2 + 4];
    float r = 0.08;

    circle[0] = 0.0;
    circle[1] = 0.0;
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

    unsigned int uColorLoc = glGetUniformLocation(buttonShader, "color");
    if (uColorLoc == -1) {
        std::cerr << "Error: Unable to find the uniform location for 'color'" << std::endl;
    }
    glUniform4f(uColorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
    bool colorUpdated = false;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ FRAMES +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int frameStride = 5 * sizeof(float);

    float yellowFrameVertices[] = {
        0.55, 0.7,     yellow_r, yellow_g, yellow_b,
       0.5, 0.65,      yellow_r, yellow_g, yellow_b,
       0.55, -0.7,     yellow_r, yellow_g, yellow_b,
       0.5, -0.65,   yellow_r, yellow_g, yellow_b,
       -0.25, -0.7,    yellow_r, yellow_g, yellow_b,
       -0.2, -0.65,     yellow_r, yellow_g, yellow_b,
       -0.25, 0.7,     yellow_r, yellow_g, yellow_b,
        -0.2, 0.65,      yellow_r, yellow_g, yellow_b,
       0.55, 0.7,      yellow_r, yellow_g, yellow_b,
       0.5, 0.65,      yellow_r, yellow_g, yellow_b
    };

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yellowFrameVertices), yellowFrameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, frameStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, frameStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float redFrameVertices[] = {
      /* 0.55, 0.8,     red_r, red_g, red_b,
       0.5, 0.75,      red_r, red_g, red_b,
       0.55, -0.6,     red_r, red_g, red_b,
       0.5, -0.55,   red_r, red_g, red_b,
       -0.25, -0.6,    red_r, red_g, red_b,
       -0.2, -0.55,     red_r, red_g, red_b,
       -0.25, 0.8,     red_r, red_g, red_b,
        -0.2, 0.75,      red_r, red_g, red_b,
       0.55, 0.8,      red_r, red_g, red_b,
       0.5, 0.75,      red_r, red_g, red_b*/
        0.55, 0.7,     red_r, red_g, red_b,
       0.5, 0.65,      red_r, red_g, red_b,
       0.55, -0.7,     red_r, red_g, red_b,
       0.5, -0.65,   red_r, red_g, red_b,
       -0.25, -0.7,   red_r, red_g, red_b,
       -0.2, -0.65,     red_r, red_g, red_b,
       -0.25, 0.7,     red_r, red_g, red_b,
        -0.2, 0.65,      red_r, red_g, red_b,
       0.55, 0.7,     red_r, red_g, red_b,
       0.5, 0.65,      red_r, red_g, red_b,
    };

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(redFrameVertices), redFrameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, frameStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, frameStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float blueFrameVertices[] = {
          0.25, 0.7,     blue_r, blue_g, blue_b,
         0.2, 0.65,      blue_r, blue_g, blue_b,
         0.25, -0.7,     blue_r, blue_g, blue_b,
         0.2, -0.65,   blue_r, blue_g, blue_b,
         -0.55, -0.7,   blue_r, blue_g, blue_b,
         -0.5, -0.65,     blue_r, blue_g, blue_b,
         -0.55, 0.7,     blue_r, blue_g, blue_b,
          -0.5, 0.65,      blue_r, blue_g, blue_b,
         0.25, 0.7,     blue_r, blue_g, blue_b,
         0.2, 0.65,      blue_r, blue_g, blue_b,
    };

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueFrameVertices), blueFrameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, frameStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, frameStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float purpleFrameVertices[] = {
          0.25, 0.7,     purple_r, purple_g, purple_b,
         0.2, 0.65,      purple_r, purple_g, purple_b,
         0.25, -0.7,     purple_r, purple_g, purple_b,
         0.2, -0.65,   purple_r, purple_g, purple_b,
         -0.55, -0.7,   purple_r, purple_g, purple_b,
         -0.5, -0.65,     purple_r, purple_g, purple_b,
         -0.55, 0.7,     purple_r, purple_g, purple_b,
          -0.5, 0.65,      purple_r, purple_g, purple_b,
         0.25, 0.7,     purple_r, purple_g, purple_b,
         0.2, 0.65,      purple_r, purple_g, purple_b,
    };

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(purpleFrameVertices), purpleFrameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, frameStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, frameStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);*/

    unsigned int rotationShader = createShader("frame_rotation.vert", "frame_rotation.frag");
    unsigned int uPosLoc = glGetUniformLocation(rotationShader, "uPos");
    float rrr = 0.1;      //Poluprecnik kruznice po kojoj se trougao krece, mora biti manji od najmanje apsolutne vrednosti y koordinate temena
    float rotationSpeed = 0.7;

    glClearColor(0.8, 0.8, 0.8, 1.0);
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++

    while (!glfwWindowShouldClose(window)) //Beskonacna petlja iz koje izlazimo tek kada prozor treba da se zatvori
    {
        glfwPollEvents();
        glViewport(0, 0, wWidth, wHeight);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            colorUpdated = true;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            colorUpdated = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ NAME TEXTURE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glUseProgram(nameAndSurnameShader);
        glBindVertexArray(VAO[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, checkerTexture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        // +++++++++++++++++++++++++++++++++++++++++++++++++++ BUTTON COLOR CHANGE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        glUseProgram(buttonShader);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));

        if (!colorUpdated) {
            glUniform4f(uColorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glClearColor(0.8, 0.8, 0.8, 1.0);
        }
        else {
            glUniform4f(uColorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
            glClearColor(0.3, 0.3, 0.3, 1.0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }        

        glUseProgram(rotationShader);

        glViewport(wWidth / 2 + 1, wHeight / 2 + 1, wWidth / 2, wHeight / 2);
        glBindVertexArray(VAO[2]);
        glUniform2f(uPosLoc, 0.15 * cos(glfwGetTime() * rotationSpeed), 0.15 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(yellowFrameVertices) / frameStride);

        glViewport(wWidth / 2 + 1, 0, wWidth / 2, wHeight / 2);
        glBindVertexArray(VAO[3]);
        glUniform2f(uPosLoc, 0.15* cos(glfwGetTime()* rotationSpeed), 0.15* (sin(glfwGetTime()* rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(redFrameVertices) / frameStride);

        glViewport(0, wHeight / 2 + 1, wWidth / 2, wHeight / 2);
        glBindVertexArray(VAO[4]);
        glUniform2f(uPosLoc, 0.15 * cos(glfwGetTime() * rotationSpeed), 0.15 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(yellowFrameVertices) / frameStride);

        glViewport(0, 0, wWidth / 2, wHeight / 2);
        glBindVertexArray(VAO[5]);
        glUniform2f(uPosLoc, 0.15 * cos(glfwGetTime() * rotationSpeed), 0.15 * (sin(glfwGetTime() * rotationSpeed)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(redFrameVertices) / frameStride);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        /*glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);*/

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(6, VBO);
    glDeleteTextures(1, &checkerTexture);
    glDeleteVertexArrays(6, VAO);
    glDeleteProgram(nameAndSurnameShader);
    glDeleteProgram(buttonShader);
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