// manages driver implementation to our opengl specification
// retrieves locations of all the OpenGL functions need from the graphics driver at runtime
#include <glad/glad.h> 
#include <GLFW/glfw3.h> // gives bare necessities for rendering to screen
#include <iostream>


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


// three vertices for triange
float vertices[] =
{
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

// unique vertices of rectangle
float uniqueVertices[] =
{
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};
// index order to draw them
unsigned int indicies[] =
{
    0, 1, 3,
    1, 2, 3
};




// callback function called each time window resize to adjust viewport
// glfw calls and fills proper arguments on its own once binded
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // tell opengl size of rendering windows. 
    // set equal to glfw window size
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true); // returns whether key currently being pressed
    }
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

    // register function for window resize
    // IMPORTANT: register callback functions after created window and before render loop is initiated
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD manages function points for OpenGL, so init before call any functs
    // Load address of OpenGL funct points which OS specific
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ALL OF THIS MUST COME AFTER INIT GLFW AND GLAD
    

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

    // from this point on any buffer calls make on GL_ARRAY_BUffer target will be used to config the currently
    // bound buffer, VBO
    // copy vertex data into currently bound buffer memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // create vertex shader object 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // attach shader source code to shader object and compile
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // create fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // create shader program object to link shaders
    // we will activate this shader program when rendering objects
    // links outputs of each shaders to inputs of next
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // attach compiled shaders to program obj and link them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // dont need shader objects once linked to program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // specify how opengl should interpret vertex data
    // tells opengl how to link vertex data to the vertex shader's shader attributes
    // vbo bound to gl_array_buffer, so vertex attribute 0 now assoc with its vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 1st param is location in vertex shader to pass to
    glEnableVertexAttribArray(0);

    // render loop!- iteration of render loop called a FRAME
    // so app keeps drawing images and handle input until told to stop
    while (!glfwWindowShouldClose(window)) // checks if glfw has been told to close
    {
        // INPUT:
        processInput(window);

        // RENDERING COMMANDS HERE:
        // clear screens color buffer at START of frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // whenever call glClear, filled with color configured here. STATE-SETTING
        glClear(GL_COLOR_BUFFER_BIT); // specifcied color buffer. STATE-USING (uses curr state to retrieve clearing color from)

        // activate program. every rendering call after will now use this program
        // object and thus the shaders
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3);
        

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

