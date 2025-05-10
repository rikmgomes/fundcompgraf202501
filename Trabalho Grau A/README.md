# Trabalho Grau A
- Nome: Ricardo Moreira Gomes.
- Descrição: Repositório para o Trabalho do Grau A.

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
├── Trabalho Grau A/
│   ├── Common
│   ├── assets/tex
│   ├── include/glad
│   ├── src (Arquivo TGA.cpp com o protótipo do jogo 2d simples)
│   ├── .gitignore
│   ├── CMakeLists.txt
│   └── README.md (Explicações)
│
├── .gitattributes
├── .gitignore
└── README.md
```

### 🧭 Orientações
A pasta funciona como um projeto completo. Para que tudo funcione corretamente, é necessário seguir as instruções contidas nos `READ.me` tanto do repositório como um todo quanto do diretório `Lista 1`.

### 📚 Proposta
O objetivo do trabalho foi desenvolver um protótipo de jogo 2D, utilizando a API Gráfica OpenGL (moderna), aplicando conceitos vistos em aula sobre desenho de primitivas, transformações geométricas, projeção ortográfica e mapeamento de textura. O jogo que desenvolvi segue uma proposta arcade no estilo do jogo Catch do Atari (1977). Você controla um pássaro (WASD ou setinhas) e seu objetivo é salvar o maior número de ovos em queda possível (pontuação se encontra no título da janela da aplicação). A cada ovo coletado, sua velocidade diminui e fica mais difícil se movimentar pelos céus do cenário. No momento, o jogo ainda não tem um fim, mas atualizações futuras implementarão isso.

### 🛑 Problemas Enfrentados
* Acúmulo de variáveis para timers/delta me fizeram generalizar o processo de animação na função updateSprite();
* Wrap de spritesheet acabava errado no pássaro, precisei fazer correções nos vértices, dimensions e na própria spritesheet para alcançar divisão igual entre os frames;
* Velocidade de queda ovos multiplicava exponencialmente por lastTime quanto tentar manter a conexão com FPS fixo (precisei eliminar tal multiplicação);
* Precisão nos .dimensions nos carregamentos (fiz diversas alterações para tudo ficar certinho dos sprites na tela);
* Ajustes de .vel tanto para obj quanto para spr1.

### 📗 Melhorias Futuras
* Parallax com 3 camadas;
* Pontuação via HUD (possivelmente usando FreeType);
* Estado de Fim/Gameover (quando tempo acabar, mostra pontuação conforme quantidade de ovos coletados).