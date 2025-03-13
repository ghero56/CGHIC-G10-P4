/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
/*-----------------    2025-2   ---------------------------*/
/*------------- Alumno Arciga Guzmán Fernando -------------*/
/*------------- No. de cuenta:  315285705   ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>
#include <vector>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f;

float angX = 0.0f, angY = 0.0f, angZ = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat verticesCubo[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		//V0 - Frontal
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, 0.5f, 0.5f,		//V5
		-0.5f, -0.5f, 0.5f,		//V0
		-0.5f, 0.5f, 0.5f,		//V4
		0.5f, 0.5f, 0.5f,		//V5

		0.5f, -0.5f, -0.5f,		//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, -0.5f,		//V7

		-0.5f, 0.5f, 0.5f,		//V4 - Izq
		-0.5f, 0.5f, -0.5f,		//V7
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, -0.5f, 0.5f,		//V0

		0.5f, 0.5f, 0.5f,		//V5 - Der
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		0.5f, -0.5f, -0.5f,		//V2

		-0.5f, 0.5f, 0.5f,		//V4 - Sup
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, 0.5f, -0.5f,		//V6

		-0.5f, -0.5f, 0.5f,		//V0 - Inf
		-0.5f, -0.5f, -0.5f,	//V3
		0.5f, -0.5f, -0.5f,		//V2
		-0.5f, -0.5f, 0.5f,		//V0
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, -0.5f, 0.5f,		//V1
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4 2025-2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();

//This is new
	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader myShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	myShader.use();

	glm::mat4 viewOp = glm::mat4(1.0f);			//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	glm::mat4 tempPecho = glm::mat4(1.0f);
	glm::mat4 tempBrazo = glm::mat4(1.0f);
	glm::mat4 tempPierna = glm::mat4(1.0f);
	glm::mat4 tempEspada = glm::mat4(1.0f);
	
	glm::mat4 inicioVaca = glm::mat4(1.0f);
	glm::mat4 cabezaVaca = glm::mat4(1.0f);
	glm::mat4 cuerpoVaca = glm::mat4(1.0f);
	glm::mat4 campanaVaca = glm::mat4(1.0f);
	glm::mat4 cuernaVaca = glm::mat4(1.0f);
	glm::mat4 centroVaca = glm::mat4(1.0f);


	const glm::vec3 colorRosa = glm::vec3(0.7686f,0.537254f, 0.5450f);
	const glm::vec3 colorNego = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 colorAmarillo = glm::vec3(1.0f, 0.898039f, 0.60392f);
	const glm::vec3 colorBlanco = glm::vec3(1.0f, 1.0f, 1.0f);

	const enum color {
		rosa,
		negro,
		amarillo,
		blanco
	};

	const std::vector<glm::vec3> colors = {
		colorRosa,
		colorNego,
		colorAmarillo,
		colorBlanco
	};

	//Use "projection" in order to change how we see the information
	projectionOp = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi bloque de dibujo
		/*******************************************/
		glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
		//Use "view" in order to affect all models
		viewOp = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
		viewOp = glm::rotate(viewOp, glm::radians(angX), glm::vec3(1.0f, 0.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(angY), glm::vec3(0.0f, 1.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(angZ), glm::vec3(0.0f, 0.0f, 1.0f));

		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		myShader.setMat4("projection", projectionOp);

		//Model
		glBindVertexArray(VAO);
		
		// modelado

		// pecho como lugar inicial geométrico
		tempPecho = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.5f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 6.0f, 1.0f));
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// desde el pecho -> cuello
		modelOp = tempPecho;
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, 3.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.5f, 1.0f));
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// desde el origen -> cabeza
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 8.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 2.5f, 1.0f));
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 1.f, 1.0f));
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		int negativo = -1;
		for (size_t i = 0; i < 2; i++) {
			modelOp = tempPecho;
			modelOp = glm::translate(modelOp, glm::vec3(negativo * 3.25f, 2.5f, 0.0f));
			tempBrazo = modelOp;
			modelOp = glm::scale(modelOp, glm::vec3(1.5f, 1.f, 1.0f));
			myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			modelOp = tempBrazo;
			modelOp = glm::translate(modelOp, glm::vec3(negativo * 0.25f, -2.25f, 0.0f));
			tempBrazo = modelOp;
			modelOp = glm::scale(modelOp, glm::vec3(1.f, 4.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
			negativo *= negativo;
		}

		// espada
		tempEspada = glm::translate(tempBrazo, glm::vec3(0.25f, -2.40f, 0.0f));
		modelOp = tempEspada;
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.3f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.5f, 1.0f, 0.7f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		tempEspada = glm::translate(tempEspada, glm::vec3(0.9f, 0.0f, 0.0f));
		modelOp = tempEspada;
		modelOp = glm::scale(modelOp, glm::vec3(0.3f, 0.6f, 1.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		tempEspada = glm::translate(tempEspada, glm::vec3(1.75f, 0.0f, 0.0f));
		modelOp = tempEspada;
		modelOp = glm::scale(modelOp, glm::vec3(3.5f, 0.3f, 1.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// piernas
		negativo = -1;
		for (size_t i = 0; i < 2; i++) {
			modelOp = glm::mat4(1.0f);
			modelOp = glm::translate(modelOp, glm::vec3(negativo * 1.75f, -2.25f, 0.0f));
			tempPierna = modelOp;
			modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
			myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			modelOp = tempPierna;
			modelOp = glm::translate(modelOp, glm::vec3(negativo * 0.5f, -2.15f, 0.0f));
			tempPierna = modelOp;
			modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.8f, 1.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			negativo *= -1;
		}


		// vaca voxel art
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 3.0f, 0.0f));
		inicioVaca = modelOp;
		modelOp = glm::scale(modelOp, glm::vec3(5.0f,3.0f,2.0f));
		myShader.setVec3("aColor", colors[rosa]);
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		float pos = -1.25f;
		myShader.setVec3("aColor", colors[negro]);
		for (size_t i = 0;i<2;i++) {
			modelOp = inicioVaca;
			modelOp = glm::translate(modelOp, glm::vec3(pos, 0.0f, 0.6f));
			modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			pos *= -1.0f;
		}
		
		myShader.setVec3("aColor", colors[blanco]);
		modelOp = inicioVaca;
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, 0.5f, -3.0f));
		cabezaVaca = modelOp;
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 4.0f, 4.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		pos = -2.1f;
		for (size_t i = 0; i < 2; i++) {
			modelOp = cabezaVaca;
			myShader.setVec3("aColor", colors[negro]);
			modelOp = glm::translate(modelOp, glm::vec3(pos, 0.5f, 0.5f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			pos *= -1.0f;
		}

		modelOp = cabezaVaca;
		modelOp = glm::translate(modelOp, glm::vec3(0.f, -3.0f, 0.0f));
		campanaVaca = modelOp;
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 2.0f, 1.0f));
		myShader.setVec3("aColor", colors[amarillo]);
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		std::vector<float> di = {1.0f, -1.0f};
		for (size_t i = 0; i < 4; i++) {
			modelOp = campanaVaca;
			if (i % 2 == 0) {
				di[0] *= -1.0f;
				modelOp = glm::translate(modelOp, glm::vec3(di[0], -1.0f, 0.0f));

			}
			else {
				di[1] *= -1.0f;
				modelOp = glm::translate(modelOp, glm::vec3(0.0f, -1.0f, di[1]));
			}
			modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.0f, 1.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		modelOp = cabezaVaca;
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, 3.0f, -0.5f));
		cuernaVaca = modelOp;
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 2.0f, 3.0f));
		myShader.setVec3("aColor", colors[blanco]);
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		myShader.setVec3("aColor", colors[amarillo]);
		pos = -3.0f;
		for (size_t i = 0; i < 2; i++) {
			modelOp = cuernaVaca;
			modelOp = glm::translate(modelOp, glm::vec3(pos, 0.5f, 0.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			pos *= -1.0f;
		}

		pos = -4.0f;
		for (size_t i = 0; i < 2; i++) {
			modelOp = cuernaVaca;
			modelOp = glm::translate(modelOp, glm::vec3(pos, 2.0f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.0f, 4.0f, 1.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			pos *= -1.0f;
		}

		modelOp = cabezaVaca;
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, -1.0f, -9.f));
		centroVaca = modelOp;
		modelOp = glm::scale(modelOp, glm::vec3(9.0f, 6.0f, 14.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", colors[blanco]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// lomo
		modelOp = centroVaca;
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, 3.5f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(7.0f, 1.0f, 14.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// mancha 1
		modelOp = centroVaca;
		modelOp = glm::translate(modelOp, glm::vec3(4.01f, 1.51f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.0f, 8.0f));
		myShader.setVec3("aColor", colors[negro]);
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		modelOp = centroVaca;
		modelOp = glm::translate(modelOp, glm::vec3(2.51f, 3.51f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 8.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = centroVaca;
		modelOp = glm::translate(modelOp, glm::vec3(-2.51f, 3.51f, -1.5f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 3.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = centroVaca;
		modelOp = glm::translate(modelOp, glm::vec3(-4.01f, 1.51f, -1.5f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 3.0f, 3.0f));
		myShader.setMat4("model", modelOp);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// patas

		di = { 3.0f, 5.5f };
		for (size_t i = 0; i < 4; i++) {
			modelOp = centroVaca;
			myShader.setVec3("aColor", colors[blanco]);

			if (i % 2 == 0) {
				di[0] *= -1.0f;
				modelOp = glm::translate(modelOp, glm::vec3(di[0], -4.5f, di[1]));
			}
			else {
				di[1] *= -1.0f;
				modelOp = glm::translate(modelOp, glm::vec3(di[0], -4.5f, di[1]));
			}
			modelOp = glm::scale(modelOp, glm::vec3(3.0f, 3.0f, 3.0f));
			myShader.setMat4("model", modelOp);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			modelOp = glm::translate(modelOp, glm::vec3(0.0f, -1.0f, 0.0f));
			myShader.setVec3("aColor", colors[negro]);
			myShader.setMat4("model", modelOp);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		glBindVertexArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	float speed = 0.1;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f * speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f * speed;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f * speed;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f * speed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f * speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f * speed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		angX -= speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		angX += speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		angY -= speed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		angY += speed;
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		angZ -= speed;
	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
		angZ += speed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}