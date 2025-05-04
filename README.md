# fundcompgraf202501
- Nome: Ricardo Moreira Gomes.
- Descrição: Repositório para as aulas de Fundamentos de Computação Gráfica 2025/1 - Unisinos.

### 🕹️ Instruções
Este repositório separa cada atividade da cadeira em pastas/diretórios completos com tudo que é necessário. Se apenas deseja visualizar o programa, basta clonar/baixar e seguir as instruções deste [link](https://github.com/fellowsheep/FCG2025-1/blob/main/GettingStarted.md) feito pela Professora Rossana. Se deseja fazer alterações, será necessário fazer algumas coisas como apagar a pasta oculta .git, apagar o diretório oculto build e criar um novo + executar o cmake novamente com o seguinte comando no terminal/bash do MinGW = "cmake .. -G "MinGW Makefiles". Esse processo é necessário pois certas configurações estão atreladas a caminhos absolutos que não irão funcionar na sua máquina/SO.

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

### 📚 Referências
* https://learnopengl.com/#!Getting-started/Hello-Triangle
* https://learnopengl.com/#!Getting-started/Shaders
* https://antongerdelan.net/opengl/hellotriangle.html
* AKENINE-MÖLLER, Tomas; HAINES, Eric; HOFFMAN, Naty. Real-Time Rendering. 4. ed. Natick, MA: A. K. Peters, 2018. Cap. 2 – The Graphics Rendering Pipeline, p. 11–27.
* Playlist sobre OpenGL Moderna: [The Cherno – YouTube – OpenGL](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2)
* Playlist sobre C++: [The Cherno – YouTube – C++](https://www.youtube.com/playlist?list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb)