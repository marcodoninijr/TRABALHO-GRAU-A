/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz e Marco Donini Jr
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 10/08/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB_IMAGE
#include "stb_image.h"

#include "Shader.h"


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();
int setupSprite();
int setupSprite(int nAnimations, int nFrames, float& dx, float& dy);

GLuint createTexture(string filePath);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

// Fun��o MAIN
int main()
{
	srand((int)glfwGetTime);
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Mundo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	float dx, dy;
	GLuint VAO_Fundo = setupSprite(1, 1, dx, dy);
	GLuint VAO_Meteoro = setupSprite(1, 1, dx, dy);
	GLuint VAO_Personagem = setupSprite(1, 5, dx, dy);
	GLuint VAO_GameOver = setupSprite(1, 1, dx, dy);

	int iAnimation = 0;
	int iFrame = 0;
	int nFrames = 5;
	float xDino = 170.0f, yDino = 100.0f;
	float xMeteoro = 400.0f, yMeteoro = 700.0f;
	bool gameOver = false;

	GLuint texID_Fundo = createTexture("../textures/back.jpg");
	GLuint texID_Personagem = createTexture("../textures/dinoanda.png");
	GLuint texID_Personagem2 = createTexture("../textures/dinoanda2.png");
	GLuint texID_Meteoro = createTexture("../textures/flaming_meteor.png");
	GLuint texID_GameOver = createTexture("../textures/gameOver.jpg");

	//Matriz de proje��o
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	//projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
	projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	glUseProgram(shader.ID);

	shader.setMat4("projection", glm::value_ptr(projection));

	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer"), 0);

	glActiveTexture(GL_TEXTURE0);

	//Habilitando o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Habilitando a transpar�ncia
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//----------------FUNDO---------------------
		//Matriz de transforma��o do objeto
		glm::mat4 model = glm::mat4(1); //matriz identidade
		model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
		
		model = glm::scale(model, glm::vec3(1080.0f, 720.0f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		//Mandar para o shader os deslocamentos da texturas (spritesheet)
		shader.setVec2("offsets", 1, 1);
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		if (!gameOver) {
			glBindVertexArray(VAO_Fundo);
			glBindTexture(GL_TEXTURE_2D, texID_Fundo);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//---------------DINO-------------------------
		float xScaleDino = 110.0f, yScaleDino = 72.0f;
		model = glm::mat4(1); //matriz identidade
		model = glm::translate(model, glm::vec3(xDino, yDino, 0.0f));
		//model = glm::rotate(model, /*glm::radians(90.0f)*/(float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(xScaleDino, yScaleDino, 1.0f));  //110.0f 72.0f 1.0f
		shader.setMat4("model", glm::value_ptr(model));

		iFrame = (iFrame + 1) % nFrames;

		if (!gameOver) {
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && xDino <= 740) {
				xDino += 50.0f * 0.2f;
				glBindVertexArray(VAO_Personagem);
				glBindTexture(GL_TEXTURE_2D, texID_Personagem);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && xDino >= 70) {
				xDino -= 50.0f * 0.2f;
				glBindVertexArray(VAO_Personagem);
				glBindTexture(GL_TEXTURE_2D, texID_Personagem2);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && xDino >= 70) {

				glBindVertexArray(VAO_Personagem);
				glBindTexture(GL_TEXTURE_2D, texID_Personagem);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		//--------------METEORO--------------------------
		float xScaleMeteoro = 42.0f, yScaleMeteoro = 64.0f;
		model = glm::mat4(1); //matriz identidade
		model = glm::translate(model, glm::vec3(xMeteoro, yMeteoro, 0.0f));
		
		model = glm::scale(model, glm::vec3(xScaleMeteoro, yScaleMeteoro, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		shader.setVec2("offsets", 1, 1);
		yMeteoro -= 15;
		if (yMeteoro <= 0.0)
		{
			xMeteoro = 64 + rand() % 736;
			yMeteoro = 850;
		}
		if (!gameOver) {
			glBindVertexArray(VAO_Meteoro);
			glBindTexture(GL_TEXTURE_2D, texID_Meteoro);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);
		}
		//--------------COLISAO METEORO-----------------
		
			//Matriz de transforma��o do objeto
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1080.0f, 720.0f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));
		
		shader.setVec2("offsets", 1, 1);
		
		float xDiagDino = xDino + xScaleDino / 2;
		float yDiagDino = yDino + yScaleDino / 2;
		float xDiagMeteoro = xMeteoro + xScaleMeteoro / 2;
		float yDiagMeteoro = yMeteoro + yScaleMeteoro / 2;
		
		bool colisaoX = xDiagDino >= xMeteoro && xDiagMeteoro >= xDino;
		bool colisaoY = yDiagDino >= yMeteoro && yDiagMeteoro >= yDino;

		if (colisaoX && colisaoY) {
			
			glBindVertexArray(VAO_GameOver);
			glBindTexture(GL_TEXTURE_2D, texID_GameOver);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			gameOver = true;
			
		}
		if (gameOver || (colisaoX && colisaoY)) {
			
			glBindVertexArray(VAO_GameOver);
			glBindTexture(GL_TEXTURE_2D, texID_GameOver);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			gameOver = false;
		}

		
		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO_Personagem);
	glDeleteVertexArrays(1, &VAO_Fundo);
	glDeleteVertexArrays(1, &VAO_Meteoro);
	glDeleteVertexArrays(1, &VAO_GameOver);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[] = {
		//x   y     z    s    t
		-0.5, -0.5, 0.0, 0.0, 0.0,
		 0.5, -0.5, 0.0, 1.0, 0.0,
		 0.0,  0.5, 0.0, 0.5, 1.0
		 //outro triangulo vai aqui
	};

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo coord de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

GLuint createTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregando a imagem do filePath e armazenando em data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);



	return texID;
}

int setupSprite()
{
	GLfloat vertices[] = {
		//x   y     z    s    t
		//Primeiro tri�ngulo
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5, -0.5, 0.0, 1.0, 0.0, //v1
		 0.5,  0.5, 0.0, 1.0, 1.0,  //v2
		 //outro triangulo vai aqui
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5,  0.5, 0.0, 1.0, 1.0,  //v2
		-0.5,  0.5, 0.0, 0.0, 1.0, //v3
	};

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo coord de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int setupSprite(int nAnimations, int nFrames, float& dx, float& dy)
{
	dx = 1.0 / nFrames;
	dy = 1.0 / nAnimations;

	GLfloat vertices[] = {
		//x   y     z    s    t
		//Primeiro tri�ngulo
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5, -0.5, 0.0, dx, 0.0, //v1
		 0.5,  0.5, 0.0, dx, dy,  //v2
		 //outro triangulo vai aqui
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5,  0.5, 0.0, dx, dy,  //v2
		-0.5,  0.5, 0.0, 0.0, dy, //v3
	};

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo coord de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
