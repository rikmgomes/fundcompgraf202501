/*
 * Hello Triangle - Código adaptado de:
 *   - https://learnopengl.com/#!Getting-started/Hello-Triangle
 *   - https://antongerdelan.net/opengl/glcontext2.html
 *
 * Adaptado por: Rossana Baptista Queiroz
 *
 * Disciplinas:
 *   - Processamento Gráfico (Ciência da Computação - Híbrido)
 *   - Processamento Gráfico: Fundamentos (Ciência da Computação - Presencial)
 *   - Fundamentos de Computação Gráfica (Jogos Digitais)
 *
 * Descrição:
 *   Este código é o "Olá Mundo" da Computação Gráfica, utilizando OpenGL Moderna.
 *   No pipeline programável, o desenvolvedor pode implementar as etapas de
 *   Processamento de Geometria e Processamento de Pixel utilizando shaders.
 *   Um programa de shader precisa ter, obrigatoriamente, um Vertex Shader e um Fragment Shader,
 *   enquanto outros shaders, como o de geometria, são opcionais.
 *
 * Histórico:
 *   - Versão inicial: 07/04/2017
 *   - Última atualização: 18/03/2025
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Sprite 
{
	GLuint VAO;
	GLuint texID;
	vec3 pos;
	vec3 dimensions;
	float angle;
	float vel; 
	int nAnimations, nFrames;
	int iFrame, iAnimation;
	float ds, dt;
	bool active;      // Para objetos que caem
	bool flipped;     // Flipar horizontalmente
	float animTimer;  // Timer interno de animação (tempo entre frames)
};

// Coisos Objetos em Queda
vector<Sprite> fallingObjects; //vetor de objetos que caem
float spawnTimer = 0.0f; //timer
const float spawnInterval = 1.0f; //intervalo entre spawns
int score = 0; //pontuação

// Protótipos das funções
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int setupShader();
int setupSprite(int nAnimations, int nFrames, float &ds, float &dt);
int loadTexture(string filePath);
void drawSprite(GLuint shaderID, Sprite spr);
void updateSprite(Sprite& sprite, float deltaTime);

// Dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL)
const GLchar *vertexShaderSource = R"(
 #version 400
 layout (location = 0) in vec2 position;
 layout (location = 1) in vec2 texc;
 
 uniform mat4 projection;
 uniform mat4 model;
 out vec2 tex_coord;
 void main()
 {
	tex_coord = vec2(texc.s,1.0-texc.t);
	gl_Position = projection * model * vec4(position, 0.0, 1.0);
 }
 )";

// Código fonte do Fragment Shader (em GLSL)
const GLchar *fragmentShaderSource = R"(
 #version 400
in vec2 tex_coord;
out vec4 color;
uniform sampler2D tex_buff;
uniform vec2 offset_tex;
void main()
{
	 color = texture(tex_buff,tex_coord + offset_tex);
}
)";

bool keys[1024];
float FPS = 12.0;
float lastTime = 0.0;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Muita atenção aqui: alguns ambientes não aceitam essas configurações
	// Você deve adaptar para a versão do OpenGL suportada por sua placa
	// Sugestão: comente essas linhas de código para desobrir a versão e
	// depois atualize (por exemplo: 4.5 com 4 e 5)
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Ativa a suavização de serrilhado (MSAA) com 8 amostras por pixel
	glfwWindowHint(GLFW_SAMPLES, 8);

	// Essencial para computadores da Apple
	// #ifdef __APPLE__
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// #endif

	for(int i=0; i<1024;i++) { keys[i] = false; }

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Falha ao criar a janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}

	// Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	Sprite background, spr1;

	// Carregando BACKGROUNDD
	background.VAO = setupSprite(1,1,background.ds,background.dt);
	background.texID = loadTexture("../assets/tex/backgroundd.png");
	background.pos = vec3(400,300,0);
	background.dimensions = vec3(800, 600, 1);
	background.angle = 0.0;

	// Carregando PÁSSARO
	spr1.VAO = setupSprite(1,2,spr1.ds,spr1.dt);
	spr1.texID = loadTexture("../assets/sprites/passaro.png");
	spr1.pos = vec3(400,150, 0);
	spr1.dimensions = vec3(76.0f * 2.0f, 23.0f * 4.0f, 1.0f);
	spr1.vel = 4.0;
	spr1.nAnimations = 1;
	spr1.nFrames = 2;
	spr1.angle = 0.0;
	spr1.iAnimation = 0;
	spr1.iFrame = 0;
	spr1.animTimer = 0.0f;

	// Inicialização de alguns objetos (ovos) no vetor fallingObjects
	for (int i = 0; i < 5; i++)
	{
		Sprite obj;
		obj.pos = vec3(0, 0, 0);
		obj.dimensions = vec3(28.0f * 2.0f, 20.0f * 4.0f, 1.0f);
		obj.vel = 2;
		obj.active = false;
		obj.texID = loadTexture("../assets/sprites/ovoroxo2.png");
		obj.VAO = setupSprite(1,2,obj.ds,obj.dt);
		obj.nAnimations = 1;
		obj.nFrames = 2;
		obj.angle = 0.0;
		obj.iAnimation = 0;
		obj.iFrame = 0;
		obj.animTimer = 0.0f;
		fallingObjects.push_back(obj); // DEIXAR POR ÚLTIMO
	}

	glUseProgram(shaderID); // Reseta o estado do shader para evitar problemas futuros

	double prev_s = glfwGetTime();	// Define o "tempo anterior" inicial.
	double title_countdown_s = 0.1; // Intervalo para atualizar o título da janela com o FPS.

	float colorValue = 0.0;

	// Ativando o primeiro buffer de textura do OpenGL
	glActiveTexture(GL_TEXTURE0);

	// Criando a variável uniform pra mandar a textura pro shader
	glUniform1i(glGetUniformLocation(shaderID, "tex_buff"), 0);

	// Criação da matriz de projeção paralela ortográfica
	mat4 projection = mat4(1); // matriz identidade
	projection = ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	// Envio para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

	// Habilitando transparência/função de mistura
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Habilitando teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Este trecho de código é totalmente opcional: calcula e mostra a contagem do FPS na barra de título
		{
			double curr_s = glfwGetTime();		// Obtém o tempo atual.
			double elapsed_s = curr_s - prev_s; // Calcula o tempo decorrido desde o último frame.
			prev_s = curr_s;					// Atualiza o "tempo anterior" para o próximo frame.

			// Exibe o FPS, mas não a cada frame, para evitar oscilações excessivas.
			title_countdown_s -= elapsed_s;

			if (title_countdown_s <= 0.0 && elapsed_s > 0.0)
			{
				double fps = 1.0 / elapsed_s; // Calcula o FPS com base no tempo decorrido.

				// Cria uma string e define o FPS como título da janela + score.
				char tmp[256];
				sprintf(tmp, "Trabalho Grau A  ---  Ricardo Moreira Gomes  ---  FPS %.2lf  --  Pontuação %d", fps, score);
				glfwSetWindowTitle(window, tmp);

				title_countdown_s = 0.1; // Reinicia o temporizador para atualizar o título periodicamente.
			}
		}

		// Checa se houveram eventos de input e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Detecção dos inputs
		if (keys[GLFW_KEY_LEFT] == true || keys[GLFW_KEY_A] == true) // Esquerda
		{
			spr1.pos.x -= spr1.vel;
			spr1.flipped = false;
		}
		if (keys[GLFW_KEY_RIGHT] == true || keys[GLFW_KEY_D] == true) // Direita
		{
			spr1.pos.x += spr1.vel;
			spr1.flipped = true;
		}
		if (keys[GLFW_KEY_UP] == true || keys[GLFW_KEY_W] == true) // Cima
		{
			spr1.pos.y += spr1.vel;		
		}
		if (keys[GLFW_KEY_DOWN] == true || keys[GLFW_KEY_S] == true) // Baixo
		{
			spr1.pos.y -= spr1.vel;		
		}

		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Spawn de objetos
		spawnTimer += deltaTime;
		if (spawnTimer >= spawnInterval)
		{
			spawnTimer = 0.0f;
			for (auto& obj : fallingObjects) // Encontra um objeto INATIVO para reutilizar
			{
				if (!obj.active)
				{
					obj.pos.x = rand() % 700 + 50; // Posição x aleatória
					obj.pos.y = 600.0f; // Começa no topo
					obj.active = true; // Volta a ser ativo
					break;
				}
			}
		}

		// Movimento e colisão dos objetos
		for (auto& obj : fallingObjects)
		{
			if (obj.active)
			{
				obj.pos.y -= obj.vel; //* lastTime; (velocidade só cresce)
				
				// Verifica colisão com o jogador (spr1)
				if (obj.pos.y <= spr1.pos.y + spr1.dimensions.y/2 && 
					obj.pos.y >= spr1.pos.y - spr1.dimensions.y/2 &&
					obj.pos.x >= spr1.pos.x - spr1.dimensions.x/2 && 
					obj.pos.x <= spr1.pos.x + spr1.dimensions.x/2)
				{
					obj.active = false; // Apaga o objeto
					score++; // Aumento de pontuação

					if(spr1.vel <= 1); // Diminuição de velocidade até 1 (mínimo)
					else
					{
						spr1.vel -= 0.2f;
					}
				}
				
				// Verifica se saiu da tela -> apaga objeto
				if (obj.pos.y < -50.0f)
				{
					obj.active = false;
				}
			}
		}

		// ID no frag. shader atual sem deslocamento + envio de vértices e textura para gpu
		glUniform2f(glGetUniformLocation(shaderID, "offset_tex"),0.0,0.0);
		drawSprite(shaderID,background);

		// Render/process dos objetos ativos no vetor fallingObjects
		for(auto& objeto : fallingObjects)
		{
			if (objeto.active) // Não coletado nem fora da tela
			{
				// Offset de animação de obj (ovos)
				float offsetSovo = objeto.iFrame * objeto.ds;
				float offsetTovo = objeto.iAnimation * objeto.dt;
				glUniform2f(glGetUniformLocation(shaderID, "offset_tex"), offsetSovo, offsetTovo);
				drawSprite(shaderID, objeto);
				updateSprite(objeto, deltaTime);
			}
		}

		// Offset de animação de spr1 (pássaro)
		float offsetS = spr1.iFrame * spr1.ds;
		float offsetT = spr1.iAnimation * spr1.dt;
		glUniform2f(glGetUniformLocation(shaderID, "offset_tex"),offsetS,offsetT);
		drawSprite(shaderID,spr1);
		updateSprite(spr1, deltaTime);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

// Compila e "builda" um programa de shader simples e único
// A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Cria os buffers que armazenam a geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupSprite(int nAnimations, int nFrames, float &ds, float &dt)
{
	ds = 1.0 / (float) nFrames;
	dt = 1.0 / (float) nAnimations;

	GLfloat vertices[] =
	{
		-0.5f, 0.5f, 0.0f, dt,
		0.5f, 0.5f, ds, dt,
		0.5f, -0.5f, ds, 0.0f,
		
		-0.5f, 0.5f, 0.0f, dt,
		0.5f, -0.5f, ds, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f
	};

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);
	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se está normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero

	// Ponteiro pro atributo 0 - Posição - coordenadas x, y
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Ponteiro pro atributo 2 - Coordenada de textura - coordenadas s,t
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

// Vincula VAO e textura do objeto
// Calcula matriz model (posição, rotação, escala)
// Renderiza sprite na tela
void drawSprite(GLuint shaderID, Sprite spr)
{
	// Neste código, usamos o mesmo buffer de geomtria para todos os sprites
	glBindVertexArray(spr.VAO);	// Conectando ao buffer de geometria
		
	// Desenha o sprite
	glBindTexture(GL_TEXTURE_2D, spr.texID); // Conectando ao buffer de textura

	// Criação da matriz de transformações do objeto
	mat4 model = mat4(1);  // matriz identidade
	model = translate(model, spr.pos);
	model = rotate(model, radians(spr.angle),vec3(0.0,0.0,1.0));

	// Flip horizontal (movimento alinhado a direção do pássaro)
	if(spr.flipped)
	{
		model = scale(model, vec3(-spr.dimensions.x, spr.dimensions.y, 1.0f));
	}
	else
	{
		model = scale(model, spr.dimensions);
	}

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6); // Chamada de desenho - drawcall
	glBindVertexArray(0); // Desnecessário aqui, pois não há múltiplos VAOs
}

// Carrega textura a partir de uma imagem + config
// Retorna identificador da textura carregada (para posterior desenho)
int loadTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;

	// Carregamento dos dados da imagem do arquivo (pixel -> RAM)
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	// Verificações + Envio para GPU
	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Libera RAM
	stbi_image_free(data);

	// Desvincula textura ativa
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

// Função simples para lidar com animações
// Atualiza iFrame com base no tempo passado
void updateSprite(Sprite& sprite, float deltaTime) {
    if(sprite.nFrames <= 1) return; // Sem animação
    
    sprite.animTimer += deltaTime;
    float frameDuration = 1.0f / FPS; // Sincronia de fluidez com FPS
    
    if(sprite.animTimer >= frameDuration) {
        sprite.animTimer = 0.0f;
        sprite.iFrame = (sprite.iFrame + 1) % sprite.nFrames; // Avanço de frames
    }
}