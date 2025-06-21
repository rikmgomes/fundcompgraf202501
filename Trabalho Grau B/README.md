# Trabalho Grau B
- Nome: Ricardo Moreira Gomes.
- Descrição: Repositório para o Trabalho do Grau B.

### 📁 Estrutura do Projeto
```text
fundcompgraf202501/
├── Lista 1
├── Trabalho Grau A
├── Trabalho Grau B/
│   ├── Common
│   ├── assets
│   └── build
│     ├── cena.txt (arquivo que para save/load da cena do editor)
│     ├── change_color.wav (sfx)
│     └── show_hide.wav (sfx)
│   ├── include/glad
│   └── src
│     └── Voxel.cpp (entrega do trabalho com o protótipo do Editor Voxel)
│   ├── .gitignore
│   ├── CMakeLists.txt
│   ├── apresentacao_slides
│   └── README.md (Explicações)
│
├── .gitattributes
├── .gitignore
└── README.md
```

### 🧭 Orientações
A pasta funciona como um projeto completo. Para que tudo funcione corretamente, é necessário seguir as instruções contidas nos `READ.me` tanto do repositório como um todo quanto do diretório `Lista 1`.

### 📚 Proposta
O objetivo do trabalho foi desenvolver um protótipo de um editor de cenas voxelizadas 3D, utilizando a API Gráfica OpenGL (moderna), aplicando conceitos vistos em aula sobre câmera sintética e tilemaps.

### 🕹️ Instruções de Uso
- Movimentação da câmera = WASD + Mouse + Scroll (zoom);
- Movimentação pelo grid do editor = setinhas (eixo x e eixo y) + pgUp/Down (eixo z);
- Apagar voxel = DELETE;
- Restaurar voxel = V;
- Trocar cor do voxel = C;
- Salvar cena ("cena.txt") = K;
- Recarregar a cena salva ("cena.txt") = L.

### 🛑 Problemas Enfrentados
- Transparência do grid de voxels (necessários ajustes);
- Movimentação pelo grid pode ser confusa dependendo do ângulo da câmera (wasd x mouse x setas x pgup/down);
- Problema na rotação de cores;
- Dificuldade de usar a biblioteca de UI → topo da janela;
- Dificuldade de encontrar uma biblioteca simples para gerenciamento de áudio entre múltiplos SO → foco no windows;

### 📗 Melhorias Futuras
- Implementar UI via FreeType, se possível;
- Colocar uma textura específica para cada cor da lista;
- Permitir múltiplos salvamentos de cenas.
