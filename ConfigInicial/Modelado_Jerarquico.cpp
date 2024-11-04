#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);

const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
//Variables que permiten manipular la vista
float	movX = 0.0f, movY = 0.0f, movZ = -5.0f, rot = 0.0f;

//For model
//Esta variable permite generar la articulación
float hombro = 0.0f;
float codo   = 0.0f;
float muneca = 0.0f;

float menique_base    = 0.0f; //pulgar pasa a menique_base 
float menique_extremo = 0.0f; //pulgar 2 pasa a menique_extremo

float anular_base     = 0.0f;
float anular_extremo  = 0.0f;

float medio_base      = 0.0f;
float medio_extremo   = 0.0f;

float indice_base     = 0.0f;
float indice_extremo  = 0.0f;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado jerarquico. Ivan Daniel", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection


	

	// use with Perspective Projection
	//En esta práctica se están utilizando unicamente 3 parámetros, posición x y y z
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 
		 0.5f, -0.5f, 0.5f,  
		 0.5f,  0.5f, 0.5f,  
		 0.5f,  0.5f, 0.5f,  
		-0.5f,  0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, 
		
	    -0.5f, -0.5f,-0.5f, 
		 0.5f, -0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
	    -0.5f,  0.5f,-0.5f, 
	    -0.5f, -0.5f,-0.5f, 
		
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,
      
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
		//Qué hacen estas líneas de código?
		glm::mat4 modelTemp = glm::mat4(1.0f);  //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp


		//View set up 
		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	

		glBindVertexArray(VAO);
		
		//Modelo para el bícep
		//Esta rotación se está aplicando unicamente sobre el eje z
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		//Matriz temporal/auxiliar
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		//Manejo de colores (R G B)
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A


		//Modelo para el antebrazo
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
		//La matriz se debe ahora de ubicar en la posisión del siguiente elemento
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//B


		//Modelo para la palma
		//El primer paso es regresar a la matriz temporal
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//C


		//MODELOS PARA LOS DEDOS
		//Modelo para el meñique (base)
		model = glm::translate(modelTemp, glm::vec3(0.3f, 0.35f, 0.35f)); 
		model = glm::rotate(model, glm::radians(menique_base), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//Pulgar A

		//Modelo para el meñique (extremo)
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(menique_extremo), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//Pulgar
		 

		//Modelo para el anular (base)
		model = glm::translate(modelTemp, glm::vec3(-0.5f, 0.0f, -0.35f)); // Posición del dedo anular
		model = glm::rotate(model, glm::radians(anular_base), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Falange distal
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f); // Color del dedo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Anular A

		//Modelo para el anular (extremo)
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Falange distal
		model = glm::rotate(model, glm::radians(anular_extremo), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f); // Color del dedo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Anular B

		
		//Modelo para el medio (base)
		model = glm::translate(modelTemp, glm::vec3(-0.5f, 0.0f, -0.35f)); // Posición del dedo medio
		model = glm::rotate(model, glm::radians(medio_base), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Falange distal
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f); // Color del dedo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Medio A

		//Modelo para el medio (extremo)
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Falange distal
		model = glm::rotate(model, glm::radians(medio_extremo), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f); // Color del dedo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Medio B

		  
		//Modelo para el índice (base)
		model = glm::translate(modelTemp, glm::vec3(-0.5f, -0.5f, 0.3f)); // Posición del dedo índice
		model = glm::rotate(model, glm::radians(indice_base), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Falange distal
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f); // Color del dedo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Índice A

		//Modelo para el índice (extremo)
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Falange distal
		model = glm::rotate(model, glm::radians(indice_extremo), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f); // Color del dedo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Índice B

		glBindVertexArray(0);

		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 //Manejo de teclas (del teclado)
 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.08f;
	 if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
		 movY += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		 movY -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS)
		 rot -= 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_R)     == GLFW_PRESS)
		 hombro += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		 hombro -= 0.18f;
	 
	 //Para tener movilidad en más articulaciones se deberán de configurar nuevas teclas
	 //ESTAS SON PARA EL CODO
	 if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		 codo += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		 codo -= 0.18f;
	 //ESTAS SON PARA LA MUÑECA
	 if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		 muneca += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		 muneca -= 0.18f;
	 
	 
	 //PARA MANIPULAR LOS DEDOS
	 //Para el meñique (base)
	 if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		 menique_base += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		 menique_base -= 0.18f;

	 //Para el meñique (extremo)
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		 menique_extremo += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		 menique_extremo -= 0.18f;


	 //Para el anular (base)
	 if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	 anular_base += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		 anular_base -= 0.18f;

	 //Para el anular (extremo)
	 if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		 anular_extremo += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		 anular_extremo -= 0.18f;


	 //Para el medio (base)
	 if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		 medio_base += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		 medio_base -= 0.18f;

	 //Para el medio (extremo)
	 if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
		 medio_extremo += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
		 medio_extremo -= 0.18f;


	 //Para el índice (base)
	 if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		 indice_base += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		 indice_base -= 0.18f;

	 //Para el índice (extremo)
	 if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		 indice_extremo += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
		 indice_extremo -= 0.18f;
 }