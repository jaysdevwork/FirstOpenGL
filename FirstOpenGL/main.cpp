// manages driver implementation to our opengl specification
// retrieves locations of all the OpenGL functions need from the graphics driver at runtime
#include <glad/glad.h> 
#include <GLFW/glfw3.h> // gives bare necessities for rendering to screen
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 pointLightColors[] = {
    glm::vec3(0.918f, 0.667f, 0.000f),
    glm::vec3(0.9568f, 0.211f, 0.298f),
    glm::vec3(0.0313f, 0.1529f, 0.9607f),
    glm::vec3(0.0313f, 1.0f, 0.313f)
};

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

// unique vertices of rectangle
float uniqueVertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};



Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// callback function called each time window resize to adjust viewport
// glfw calls and fills proper arguments on its own once binded
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // tell opengl size of rendering windows. 
    // set equal to glfw window size
    glViewport(0, 0, width, height); // start at bottom left corner, draw area is width * height long. map ndc to pixel coordinates
}

/**
    Obtain yaw and pitch values by getting difference between current frame mouse pos
    and las frame mouse pos.
*/
// used for camera's direction vec
float yaw = -90.0f, pitch; // pos degrees rot counter-clockwise, neg clockwise
// last mouse pos, center of screen
float lastX = 400, lastY = 300;
// prevent large movement jump upon mouse first entering window
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) // tell glfw to listen to mouse-movement events.
{   
    // basically makes offset 0 so no movement upon mouse entering
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// y offset how much we scrolled vertically
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true); // returns whether key currently being pressed
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    //cameraPos.y = 0.0f; // make camera stick to xz plane

}


int main()
{
    // Initialize glfw and configure it with windhowhint.
    // option values as second arg can be found at https://www.glfw.org/docs/latest/window.html#window_hints
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // holds all windowing data and required by most of glfw's other functions
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Make context of window the main context on current thread
    glfwMakeContextCurrent(window);

    // register callback functions to automatically be called upon their respective events
    // IMPORTANT: register callback functions after created window and before render loop is initiated
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // register callback function- making it so each time mouse moves mouse_callback funct is called
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);


    // GLAD manages function points for OpenGL, so init before call any functs
    // Load address of OpenGL funct points which OS specific
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

   
    //// setup textures
    //unsigned int texture[2];
    //glGenTextures(2, texture); // generate two texture ids
    //// just like other objects, bind so any subsequent texture commands config curr bound txture
    //glBindTexture(GL_TEXTURE_2D, texture[0]);
    //// set texture wrapping/filtering options on currently bound txture obj
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // option for txture filtering between mipmap levels
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // cant set mipmap filtering options as mag filter
    //// LOAD IMAGE using stb_image library
    //int width, height, nrChannels;
    //unsigned char* data = stbi_load("C:/Users/jay/Documents/OpenGL/Projects/FirstOpenGL/FirstOpenGL/Resources/container.jpg", &width, &height, &nrChannels, 0);
    //if (data)
    //{
    //    // generate texture
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //} 
    //else 
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data); // free image memory

    //// setup texture 2
    //glBindTexture(GL_TEXTURE_2D, texture[1]);
    //// set texture wrapping/filtering options on currently bound txture obj
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // option for txture filtering between mipmap levels
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // cant set mipmap filtering options as mag filter
    //// LOAD IMAGE 2
    //stbi_set_flip_vertically_on_load(true); // flip y axis during image loading bc image has 0 for y as top
    //unsigned char* data2 = stbi_load("C:/Users/jay/Documents/OpenGL/Projects/FirstOpenGL/FirstOpenGL/Resources/awesomeface.png", &width, &height, &nrChannels, 0);
    //if (data2)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //stbi_image_free(data2); // free image memory

    unsigned int diffuseTexture;
    glGenTextures(1, &diffuseTexture); // just like other objects, bind so any subsequent texture commands config curr bound txture
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // option for txture filtering between mipmap levels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // cant set mipmap filtering options as mag filter
    // LOAD IMAGE using stb_image library
    int width, height, nrChannels;
    unsigned char* data3 = stbi_load("D:/FirstOpenGLTutorial/FirstOpenGL/Resources/container2.png", &width, &height, &nrChannels, 0);
    if (data3)
    {
        // generate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data3); // free image memory

    unsigned int specularTexture;
    glGenTextures(1, &specularTexture); // just like other objects, bind so any subsequent texture commands config curr bound txture
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // option for txture filtering between mipmap levels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // cant set mipmap filtering options as mag filter
    // LOAD IMAGE using stb_image library
    int width1, height1, nrChannels1;
    unsigned char* data4 = stbi_load("D:/FirstOpenGLTutorial/FirstOpenGL/Resources/container2_specular.png", &width1, &height1, &nrChannels1, 0);
    if (data4)
    {
        // generate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data4);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data4); // free image memory

    unsigned int emissionTexture;
    glGenTextures(1, &emissionTexture); // just like other objects, bind so any subsequent texture commands config curr bound txture
    glBindTexture(GL_TEXTURE_2D, emissionTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // option for txture filtering between mipmap levels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // cant set mipmap filtering options as mag filter
    // LOAD IMAGE using stb_image library
    int width2, height2, nrChannels2;
    unsigned char* data5 = stbi_load("D:/FirstOpenGLTutorial/FirstOpenGL/Resources/matrix.jpg", &width2, &height2, &nrChannels2, 0);
    if (data5)
    {
        // generate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data5); // free image memory

    
   

    //// ALL OF THIS MUST COME AFTER INIT GLFW AND GLAD!!
    //// create shader object: reads from disk, compiles, links, and checks for errors for vertex and fragment shaders
    //// this encapsulates alot of code
    //Shader ourShader("D:/FirstOpenGLTutorial/FirstOpenGL/shader.vs",
    //    "D:/FirstOpenGLTutorial/FirstOpenGL/shader.fs");
    //

    //// create a vertex array object to store configuration
    //// i.e. vertex attribute config and which vbo to use
    //unsigned int VAO;
    //glGenVertexArrays(1, &VAO);
    //glBindVertexArray(VAO); // to use vao, this point on bind corresponding VBOs and attribute points. then unbind for later use

    //// vertex buffer object for storing large num of vertices in GPU memory
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // bind buffer to buffer type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // from this point on any buffer calls make on GL_ARRAY_BUFFER target will be used to config the currently
    // bound buffer, VBO
    // copy vertex data into currently bound buffer memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(uniqueVertices), uniqueVertices, GL_STATIC_DRAW);

    //// specify order to draw vertices in with a element buffer object
    ////unsigned int EBO;
    ////glGenBuffers(1, &EBO);
    ////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    ////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //// specify how opengl should interpret vertex data
    //// tells opengl how to link vertex data to the vertex shader's shader attributes
    //// vbo bound to gl_array_buffer, so vertex attribute 0 now assoc with its vertex data
    //// POSITION ATTRIBUTE
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 1st param is LOCATION in vertex shader to pass to
    //glEnableVertexAttribArray(0);
    //// NORMAL ATTRIBUTE
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float))); // last param is offset to place after pos data in VBO memory
    //glEnableVertexAttribArray(1);
    //// TEXTURE COORDINATE ATTRIBUTE 
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);


    Shader lightingShader("D:/FirstOpenGLTutorial/FirstOpenGL/lightShader.vs",
        "D:/FirstOpenGLTutorial/FirstOpenGL/lightShader.fs");
    lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);

    Shader lightSourceShader("D:/FirstOpenGLTutorial/FirstOpenGL/lightSourceShader.vs",
        "D:/FirstOpenGLTutorial/FirstOpenGL/lightSourceShader.fs");

    // must use shader program first to set uniforms
    lightingShader.use();

    lightingShader.setInt("material.diffuse", 0); // set texture unit
    lightingShader.setInt("material.specular", 1);
    lightingShader.setInt("material.emission", 2);
    lightingShader.setFloat("material.shininess", 64.0f); // radius of specular highlight

    lightSourceShader.use();
    lightSourceShader.setVec3("lightColors[0]", pointLightColors[0]);
    lightSourceShader.setVec3("lightColors[1]", pointLightColors[1]);
    lightSourceShader.setVec3("lightColors[2]", pointLightColors[2]);
    lightSourceShader.setVec3("lightColors[3]", pointLightColors[3]);

    // cube to cast light on 
    unsigned int lightObjectVAO;
    glGenVertexArrays(1, &lightObjectVAO);
    glBindVertexArray(lightObjectVAO); // to use vao, this point on bind corresponding VBOs and attribute points. then unbind for later use

    // setup vertex attribute within vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // strides of 8 bc of total vertex attributes (pos and normal), last param is offset for next v. attribute
    glEnableVertexAttribArray(0);                                                   // offset is space to where next attrib starts
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // unbind vao when done with setup
    


    // light source.
    // efficent to reuse vbo already on gpu memory
    unsigned int lightSourceVAO;
    glGenVertexArrays(1, &lightSourceVAO);
    glBindVertexArray(lightSourceVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // make strides of 8, using only first 3 floats
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); 

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); wireframe mode
    // render loop!- iteration of render loop called a FRAME
    // so app keeps drawing images and handle input until told to stop
    while (!glfwWindowShouldClose(window)) // checks if glfw has been told to close
    {
        // Calculate deltatime each frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        // INPUT:
        processInput(window);

        // RENDERING COMMANDS HERE:
        // clear screens color buffer at START of frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // whenever call glClear, filled with color configured here. STATE-SETTING
        
        //clear depth and color buffers beach each rendering iteration, otherwise info from previous frame stays in buffer
        // bitwise flag used to combine into one value
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // specifcied color buffer. STATE-USING (uses curr state to retrieve clearing color from)


        // activate program. every rendering call after will now use this program
        // object and thus the shaders
        lightSourceShader.use();

        // model matrix for light source cube
        glm::mat4 model = glm::mat4(1.0f);

        // camera/view matrix
        glm::mat4 view = camera.GetViewMatrix();
        lightSourceShader.setMat("view", view);
        
        // projection matrix
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), 800.0f/600.0f, 0.1f, 100.0f);
        lightSourceShader.setMat("projection", projection);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightSourceVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightSourceShader.setMat("model", model);
            lightSourceShader.setInt("lightIndex", i);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0] * 0.1f);
        lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0]);
        lightingShader.setVec3("pointLights[0].specular", pointLightColors[0]);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1] * 0.1f);
        lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1]);
        lightingShader.setVec3("pointLights[1].specular", pointLightColors[1]);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2] * 0.1f);
        lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2]);
        lightingShader.setVec3("pointLights[2].specular", pointLightColors[2]);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09f);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3] * 0.1f);
        lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3]);
        lightingShader.setVec3("pointLights[3].specular", pointLightColors[3]);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09f);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotlight 
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // camera/view matrix
        lightingShader.setMat("view", view);

        // projection matrix
        lightingShader.setMat("projection", projection);

        glActiveTexture(GL_TEXTURE0); // activate texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);

        glActiveTexture(GL_TEXTURE1); // activate texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, specularTexture);

        glActiveTexture(GL_TEXTURE2); // activate texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, emissionTexture);

        glBindVertexArray(lightObjectVAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = i * 20.0f * glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
        }

        // CHECK AND CALL EVENTS AND SWAP BUFFERS:
        // swap color buffer used torender and show as ouput to screen
        // double buffering to prevent artifacts. front buffer is final output, rendering ccmds draw to back buffer
        // soon as rendering cmds are finished, swap back buffer to front so img can be displayed without still being rendered to
        glfwSwapBuffers(window);
        glfwPollEvents(); // check if any events triggered, like keyboard input. calling corresponding functs
    }

    // delete all glfw resources that were allocated, soon as exit render loop.
    glfwTerminate();
    return 0;
}



