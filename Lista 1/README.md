# Lista 1
- Nome: Ricardo Moreira Gomes.
- Descrição: Repositório para a Lista de Exercícios 1.

### 📁 Estrutura do Projeto
```text
fundcompgraf202501/
├── Lista 1/
│   ├── Common
│   ├── assets/tex
│   ├── include/glad
│   ├── src (Exercícios 6-10 no formato .cpp)
│   ├── .gitignore
│   ├── CMakeLists.txt
│   └── README.md (Explicações + Respostas Exercícios 1-5)
│
├── .gitattributes
├── .gitignore
└── README.md
```

### 🕹️ Respostas Ex. 1-5

#### 1. O que é a GLSL? Quais os dois tipos de shaders obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?
GLSL (OpenGL Shading Language) é a linguagem de programação usada para escrever shaders na OpenGL moderna. Ela permite controlar a forma como os vértices e fragmentos são processados pela GPU.

No pipeline programável da OpenGL moderna, dois shaders são obrigatórios:
* Vertex Shader: Processa cada vértice individualmente. É responsável por transformar a posição dos vértices no espaço da tela e pode manipular atributos como posição, cor, normais, etc.
* Fragment Shader: Processa cada fragmento (potencial pixel) gerado pelas primitivas. Define a cor final de cada pixel na tela, além de outros efeitos como iluminação e texturização.

#### 2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?
Primitivas gráficas são as formas básicas utilizadas para construir objetos gráficos, como pontos, linhas e triângulos.

Na OpenGL, os vértices são armazenados utilizando:
* VBO (Vertex Buffer Object): Buffer que armazena os dados dos vértices (posições, cores, etc.) na memória da GPU.
* VAO (Vertex Array Object): Objeto que guarda a configuração dos atributos de vértice (como interpretar os dados no VBO).

#### 3. Explique o que é VBO e VAO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).
VBO (Vertex Buffer Object): Armazena os dados brutos dos vértices (ex: posição, cor, textura) na memória da GPU. VAO (Vertex Array Object): Define como os dados do VBO são usados durante a renderização. Ele "aponta" para o VBO e descreve como os dados estão organizados (ex: stride, offsets). O VAO encapsula um ou mais VBOs e guarda a configuração necessária para fazer a renderização. Sem um VAO, você precisaria configurar os VBOs manualmente a cada desenho.

#### 4. Como são feitas as chamadas de desenho na OpenGL? Para que servem as primitivas de desenho?
As chamadas de desenho são feitas com funções como: "glDrawArrays(GL_TRIANGLES, 0, 3);" ou "glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);". Essas funções dizem à GPU para desenhar os vértices conforme configurado nos VAOs e VBOs. As primitivas de desenho (triangles, lines, points) definem como a GPU deve interpretar os vértices enviados para compor a geometria.

#### 5. Analise o código fonte do projeto Hello Triangle. Localize e relacione os conceitos de shaders, VBOs e VAO apresentados até então. Não precisa entregar nada neste exercício.