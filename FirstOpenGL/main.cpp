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

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";


// three vertices for triange
float vertices[] =
{
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
};

float vertices2[] =
{
    -0.9f, -0.9f, 0.0f,
    0.1f, -0.9f, 0.0f,
    -0.4f, 0.1f, 0.0f
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

    //glfwSwapInterval(0); UNCOMMENT TO DISABLE V SYNC

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

    // tells opengl how to link vertex data to the vertex shader's shader attributes
    // vbo bound to gl_array_buffer, so vertex attribute 0 now assoc with its vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 1st param is location in vertex shader to pass to
    glEnableVertexAttribArray(0);




    // element buffer object for effecient use of vertices
    // DEFINES DRAWING ORDER OF VERTICES IN VBO
    unsigned int EBO;
    glGenBuffers(1, &EBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);




    // VAO AND VBO FOR SECOND TRIANGLE. 
    //glBindVertexArray(0); // unbind prev vao. not necessary but helps with readability- jay
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2); // can also store vaos as array with 1st param as num of vaos
    glBindVertexArray(VAO2);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2); // generate buffer id
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 1st param is location in vertex shader to pass to
    glEnableVertexAttribArray(0);
    // END



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
    glDeleteShader(fragmentShader);

    // 2nd shader program for 2nd triangle
    // create fragment shader with yellow color
    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    // attach source code to shader object and compile
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    // create another shader program
    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    // attach compiled shaders to program obj and link them
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    glDeleteShader(fragmentShader2);
    glDeleteShader(vertexShader);




    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // to enable wireframe mode
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

        // replace to render from index buffer vs array buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // takes indices from ebo bound to gl_elem_array_buff target
        //glBindVertexArray(0); // UNBIND CURRENTLY BOUND VAO

        // draw second triangle, using shader program with yellow frag shader
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

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

