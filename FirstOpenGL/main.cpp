// manages driver implementation to our opengl specification
// retrieves locations of all the OpenGL functions need from the graphics driver at runtime
#include <glad/glad.h> 
#include <GLFW/glfw3.h> // gives bare necessities for rendering to screen
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// unique vertices of rectangle
//  storing three vec3 vertex attributes, position, color, texture coords
float uniqueVertices[] = {
    // Back face (red tones)
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.2f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.2f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.9f, 0.1f, 0.1f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    // Front face (green tones)
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.2f, 0.8f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.2f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.2f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.1f, 0.9f, 0.1f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

    // Left face (blue tones)
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.2f, 0.0f, 0.8f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.2f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.2f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.1f, 0.1f, 0.9f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

    // Right face (yellow tones)
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.2f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.8f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.8f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.9f, 0.9f, 0.1f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,

     // Bottom face (purple tones)
     -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.8f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.8f,  1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,  0.9f, 0.1f, 0.9f,  0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,

     // Top face (cyan tones)
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  0.2f, 0.8f, 0.8f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.8f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.8f,  1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  0.1f, 0.9f, 0.9f,  0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f
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

// 6 vertices in a straight line from left to right
float lightningVertices[] = {
    // x,    y,    z
    -1.0f, 0.0f, 0.0f,  // start
    -0.6f, 0.0f, 0.0f,  // point 2
    -0.2f, 0.0f, 0.0f,  // point 3
     0.2f, 0.0f, 0.0f,  // point 4
     0.6f, 0.0f, 0.0f,  // point 5
     1.0f, 0.0f, 0.0f   // end
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

   
    // setup textures
    unsigned int texture[2];
    glGenTextures(2, texture); // generate two texture ids
    // just like other objects, bind so any subsequent texture commands config curr bound txture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // set texture wrapping/filtering options on currently bound txture obj
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // option for txture filtering between mipmap levels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // cant set mipmap filtering options as mag filter
    // LOAD IMAGE using stb_image library
    int width, height, nrChannels;
    unsigned char* data = stbi_load("C:/Users/jay/Documents/OpenGL/Projects/FirstOpenGL/FirstOpenGL/Resources/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // generate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } 
    else 
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); // free image memory

    // setup texture 2
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    // set texture wrapping/filtering options on currently bound txture obj
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // option for txture filtering between mipmap levels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // cant set mipmap filtering options as mag filter
    // LOAD IMAGE 2
    stbi_set_flip_vertically_on_load(true); // flip y axis during image loading bc image has 0 for y as top
    unsigned char* data2 = stbi_load("C:/Users/jay/Documents/OpenGL/Projects/FirstOpenGL/FirstOpenGL/Resources/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data2); // free image memory



    // ALL OF THIS MUST COME AFTER INIT GLFW AND GLAD!!
    // create shader object: reads from disk, compiles, links, and checks for errors for vertex and fragment shaders
    // this encapsulates alot of code
    Shader ourShader("C:/Users/jay/Documents/OpenGL/Projects/FirstOpenGL/FirstOpenGL/shader.vs",
        "C:/Users/jay/Documents/OpenGL/Projects/FirstOpenGL/FirstOpenGL/shader.fs");

    Shader lightningShader("C:/Users/jay/Documents/OpenGL/Projects/FirstOpenGL/FirstOpenGL/lightning.vs",
        "C:/Users/jay/Documents/OpenGL/Projects/FirstOpenGL/FirstOpenGL/lightning.fs");
    

    // create a vertex array object to store configuration
    // i.e. vertex attribute config and which vbo to use
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // to use vao, this point on bind corresponding VBOs and attribute points. then unbind for later use

    // vertex buffer object for storing large num of vertices in GPU memory
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // bind buffer to buffer type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // from this point on any buffer calls make on GL_ARRAY_BUFFER target will be used to config the currently
    // bound buffer, VBO
    // copy vertex data into currently bound buffer memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(uniqueVertices), uniqueVertices, GL_STATIC_DRAW);

    // specify order to draw vertices in with a element buffer object
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // specify how opengl should interpret vertex data
    // tells opengl how to link vertex data to the vertex shader's shader attributes
    // vbo bound to gl_array_buffer, so vertex attribute 0 now assoc with its vertex data
    // POSITION ATTRIBUTE
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 1st param is LOCATION in vertex shader to pass to
    glEnableVertexAttribArray(0);
    // COLOR ATTRIBUTE
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float))); // last param is offset to place after pos data in VBO memory
    glEnableVertexAttribArray(1);
    // TEXTURE COORDINATE ATTRIBUTE 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // lightning bolts vao and vbo setup
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    // buffer type of a vertex buffer 
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    // copy vertex data into current bound buffer memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightningVertices), lightningVertices, GL_STATIC_DRAW);
    // tell opengl how to link vertex data to vertex shader's shader attributes.
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // uses previously bound vbo to take its data from!
    glEnableVertexAttribArray(0);


    // tell opengl which texture unit (location of a texture) each shader sample uniform belongs to. need only do once
    ourShader.use();
    ourShader.setInt("texture1", 0); // 0 is corresponding texture unit of texture[0]. texture1 is sampler var name
    ourShader.setInt("texture2", 1);


    // old glm learning code
    //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //glm::mat4 trans = glm::mat4(1.0f); // init to identity matrix
    //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); // vec is what we want to translate by
    //vec = trans * vec;
    //std::cout << vec.x << vec.y << vec.z << std::endl;
    //glm::mat4 trans = glm::mat4(1.0f);
    // transformation order should be read in reverse
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.05));
    //unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)); // last param convert data with glm's function


    

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

        // INPUT:
        processInput(window);

        // RENDERING COMMANDS HERE:
        // clear screens color buffer at START of frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // whenever call glClear, filled with color configured here. STATE-SETTING
        
        //clear depth and color buffers beach each rendering iteration, otherwise info from previous frame stays in buffer
        // bitwise flag used to combine into one value
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // specifcied color buffer. STATE-USING (uses curr state to retrieve clearing color from)

        // ---------- Draw cubes ----------
        // activate program. every rendering call after will now use this program
        // object and thus the shaders
        ourShader.use();

        // set time for reveal effect
        ourShader.setFloat("ftime", (float)glfwGetTime());

        // camera/view matrix
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat("view", view);
        
        // projection matrix
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), 800.0f/600.0f, 0.1f, 100.0f);
        ourShader.setMat("projection", projection);

        // bind texture, using texture unit (location of a texture)
        glActiveTexture(GL_TEXTURE0); // activate texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        // bind vao for drawing triangle
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            angle = glm::radians(angle);

            if (i % 3 == 0)
            {
                angle = (float)glfwGetTime() * 10.0f;
            }
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.05f)); // rot at angle degrees a second
            ourShader.setMat("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // ---------- Draw lightning ----------
        lightningShader.use();

        lightningShader.setFloat("time", (float)glfwGetTime());

        // model matrix
        glm::mat4 model2 = glm::mat4(1.0f); // identity
        model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
        lightningShader.setMat("model", model2);

        // camera/view matrix
        glm::mat4 view2 = camera.GetViewMatrix();
        lightningShader.setMat("view", view2);

        // projection matrix
        glm::mat4 projection2;
        projection2 = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        lightningShader.setMat("projection", projection2);

        // draw
        glBindVertexArray(VAO2);
        glLineWidth(4.0f);
        const int boltsPerCube = 4; // Number of bolts around each cube
        const float boltDistance = 1.0f; // How far from cube center

        float boltRotationSpeed = 100.0f; // Degrees per second - adjustable
        float currentTime = glfwGetTime(); // Or use your preferred time function

        // Draw lightning bolts around each cube
        for (unsigned int cubeIndex = 0; cubeIndex < 10; cubeIndex++) {
            glm::vec3 cubePos = cubePositions[cubeIndex];

            // Draw bolts in a circle around this cube
            for (int boltIndex = 0; boltIndex < boltsPerCube; boltIndex++) {
                // Calculate base angle for this bolt
                float baseAngle = (float)boltIndex / (float)boltsPerCube * 360.0f; // Degrees

                // Add time-based rotation for movement
                float timeRotation = currentTime * boltRotationSpeed; // Rotation based on time
                float angle = baseAngle + timeRotation;

                // Create model matrix for this bolt
                glm::mat4 model = glm::mat4(1.0f);

                // 1. Move to cube position
                model = glm::translate(model, cubePos);

                // 2. Rotate around Y-axis to position bolt around the cube (now with movement)
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

                // 3. Move away from cube center (in rotated X direction)
                model = glm::translate(model, glm::vec3(boltDistance, 0.0f, 0.0f));

                // 4. Rotate bolt to be vertical (90 degrees around Z-axis)
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

                // Set the model matrix and draw
                lightningShader.setMat("model", model);
                glDrawArrays(GL_LINE_STRIP, 0, 6);
            }
        }


        //  ---------- CHECK AND CALL EVENTS AND SWAP BUFFERS  ----------
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