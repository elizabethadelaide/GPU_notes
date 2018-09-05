/*

OpenGL framework displays simple 2D texture on square

*/

//Using glew
//GLFW
//Cherno tutorials on OpenGL
//docs.gl

#include <GL/glew.h> //Loads opengl from driver, simpler than glad
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window; 

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//Set up version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//set frame rate
	glfwSwapInterval(1);

	//Start glew and check if it opens ok
	//Glew needs to be init after context made
	if (glewInit() != GLEW_OK)
		std::cout << "Error glew failed to initialize" << std::endl;

	//Print version
	//Prints 4.4 -> Implies not using my Nvidia rn :/
	std::cout << glGetString(GL_VERSION) << std::endl;
	{ //avoid opengl error by creating scope
	//Array of vertices of triangle
		
		//Locations and texture locations in 2D:
		float positions[] = {
			100.0f, 100.0f, 0.0f, 0.0f,
			200.0f, 100.f, 1.0f, 0.0f,
			200.0f,	200.0f, 1.0f, 1.0f,
			100.0f, 200.0f, 0.0f, 1.0f
		};

		//index buffer to draw square
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		//Setup blending for clearer textures:
		//TODO: add to renderer:
		GLCall(glEnable(GL_BLEND));

		//Backwards (Blend Alpha reverse)
		GLCall(glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA));
		//Correct:
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//Create Vertex Array
		VertexArray va;
		//Create Vertex Buffer, and bind
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		
		VertexBufferLayout layout;
		layout.Push<float>(2); //Position
		layout.Push<float>(2); //Texture coordinate
		
		va.AddBuffer(vb, layout);

		//Create IndexBuffer:
		IndexBuffer ib(indices, 6);

		//xleft:xright, ylow:yupper, znear:zfar
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f); //simple projection matrix for aspect ratio
		
		//Camera matrix
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100, 0, 0));
		//Model matrix
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 300, 0));

		//Create model view proejection matrix:
		glm::mat4 mvp = proj * view * model;

		//Load shader from file
		Shader shader("res/shaders/Basic.shader");
		shader.Bind(); //bind
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/isa.png"); //load texture from file
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0); //set slot to 0 in shader

		Renderer renderer;
			
		//Play with the color using uniform on each frame
		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);		
			
			renderer.Draw(va, ib, shader);

			//Change red color
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	} //end scope
	glfwTerminate(); //exit window properly

	return 0;
}
