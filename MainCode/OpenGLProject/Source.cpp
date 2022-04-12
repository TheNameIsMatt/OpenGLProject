#include <Windows.h>
#include <glad/glad.h>	
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Source.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



int main()
{
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	unsigned int VBO;
	unsigned int VAO;

	unsigned int vertexShader;

	unsigned int fragmentShader;

	unsigned int shaderProgram;



	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"

		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";






	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);






	// We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific. GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return -1;
	}
	// The first two parameters of glViewport set the location of the lower left corner of the window. The third and fourth parameter set the width and height of the rendering window in pixels, which we set equal to GLFW's window size.
	// OpenGL starts in the lower left wherea DirectX starts in the top left.
	// 
	// Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates it processed to coordinates on your screen. For example, a processed point of location (-0.5,0.5) would (as its final transformation) be mapped to (200,450) 
	// in screen coordinates. Note that processed coordinates in OpenGL are between -1 and 1 so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600).
	glViewport(0, 0, 800, 600);


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// These create a buffer that is placed on the graphics card, this means that our vertex shader has instant access to this information and can handle it however it wants
	glGenBuffers(1, &VBO);
	

	// BindBuffer specifies what information the second parameter is going to hold, in this case GL_ARRAY_BUFFER is going to be vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Creates and intialises a buffer object's data store 
	// GL_STATIC_DRAW means that the information is going to be accessed many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// The first number is 0 because the only attribute we have in our array is positional data, then the next one is 3 because we have 3 bits of data for our first attribute
	// x, y, z. However if we had another set of attributes such as texture coordinate that had an x,y value, we would add another line of code for glVertexAttribPointer
	// then set the first value to 1, then the second parameter to 2 as it only contains two values for texture coordinates.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	






	// An Int is used as a form of abstraction from the programmer, this means that it can only really be handled by API calls
	// These lines are just creating an object in memory to attach our shader source code to
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	// The glCreateProgram function creates a program and returns the ID reference to the newly created program object. Now we need to attach the previously compiled shaders to the program object and then link them with glLinkProgram:
	shaderProgram = glCreateProgram();


	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

#pragma region CheckShaders

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

#pragma endregion Check Shaders compiled successfully


	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// This code will ensure that ever rendering and shading call will use this object we have created
	glUseProgram(shaderProgram);

	// After we have initialised the shaders into the program we can delete them as they are no longer needed.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);




	//Double buffer
	// When an application draws in a single buffer the resulting image may display flickering issues.This is because the resulting output image is not drawn in an instant, 
	 // but drawn pixel by pixeland usually from left to rightand top to bottom.Because this image is not displayed at an instant to the user while still being rendered to, 
	// the result may contain artifacts.To circumvent these issues, windowing applications apply a double buffer for rendering.The front buffer contains the final output image that is shown at the screen, 
	// while all the rendering commands draw to the back buffer.As soon as all the rendering commands are finished we swap the back buffer to the front buffer so the image can be displayed without still being rendered to, removing all the aforementioned artifacts.
	while (!glfwWindowShouldClose(window)) 
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Because we are assigned values using only the colour buffer with the function above, we only need to GLCLEAR the colour buffer bit, if we added a depth buffer we would need to GL CLEAR that too

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;


}




void processInput(GLFWwindow *window) {
	// Here we check whether the user has pressed the escape key (if it's not pressed, glfwGetKey returns GLFW_RELEASE)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

