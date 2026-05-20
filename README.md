# ✈️ Sistema de Malha Aérea ANAC

Sistema desenvolvido em linguagem C para gerenciamento de uma malha aérea utilizando estruturas de dados avançadas, como **grafos**, **vetores dinâmicos** e **matriz esparsa**.

O projeto simula o funcionamento básico de uma rede de aeroportos e voos, permitindo cadastrar aeroportos, cadastrar/remover voos e listar trajetos possíveis entre cidades.

---

# 📌 Sobre o Projeto

O sistema representa:

- **Aeroportos** → vértices do grafo
- **Voos** → arestas direcionadas
- **Matriz esparsa** → armazenamento eficiente das conexões aéreas
- **Busca DFS (Depth First Search)** → listagem de trajetos possíveis

O projeto foi desenvolvido com foco acadêmico para prática de:

- Estruturas de Dados
- Grafos
- Matrizes Esparsas
- Alocação dinâmica de memória
- Manipulação de ponteiros em C
- Algoritmos de busca

---

# 🚀 Tecnologias Utilizadas

- Linguagem C
- GCC
- Estruturas de Dados
- Grafos direcionados
- Matrizes Esparsas
- Busca em profundidade (DFS)

---

# 📂 Estrutura do Projeto

```bash
Sistema-de-aeropoorto-/
│
├── main.c
├── grafo_aeroportos.c
├── grafo_aeroportos.h
├── matriz_esparsa.c
├── matriz_esparsa.h
└── README.md
```

---

# ⚙️ Funcionalidades

## ✅ Cadastro de aeroportos

Permite cadastrar aeroportos utilizando:

- Código IATA
- Nome da cidade

Exemplo:

```txt
GRU → São Paulo
GIG → Rio de Janeiro
BSB → Brasília
```

---

## ✅ Cadastro de voos

Permite cadastrar voos entre aeroportos.

Cada voo possui:

- Aeroporto de origem
- Aeroporto de destino
- Número do voo

---

## ✅ Remoção de voos

Remoção de voos pelo número do voo.

---

## ✅ Listagem de voos

Lista todos os voos que partem de determinado aeroporto.

---

## ✅ Busca de trajetos

Utiliza DFS (Depth First Search) para listar todos os trajetos possíveis entre dois aeroportos.

---

# 🧠 Estruturas Utilizadas

## ✈️ Grafo

O sistema utiliza um grafo direcionado onde:

- Aeroportos = vértices
- Voos = arestas

---

## 📊 Matriz Esparsa

Os voos são armazenados em uma matriz esparsa implementada com listas encadeadas.

Isso melhora o uso de memória, já que apenas posições ocupadas são armazenadas.

---

## 🔎 DFS (Busca em Profundidade)

A busca de trajetos é realizada utilizando DFS recursiva para encontrar todos os caminhos possíveis entre dois aeroportos.

---

# ▶️ Como Executar o Projeto

## 🔹 Compilar

```bash
gcc main.c grafo_aeroportos.c matriz_esparsa.c -o aeroporto
```

---

## 🔹 Executar

### Windows

```bash
aeroporto.exe
```

### Linux / MacOS

```bash
./aeroporto
```

---

# 🖥️ Menu do Sistema

```txt
╔══════════════════════════════════╗
║    SISTEMA DE MALHA AEREA ANAC  ║
╠══════════════════════════════════╣
║ 1. Cadastrar aeroporto           ║
║ 2. Cadastrar voo                 ║
║ 3. Remover voo                   ║
║ 4. Listar voos de um aeroporto   ║
║ 5. Listar trajetos possiveis     ║
║ 6. Listar todos os aeroportos    ║
║ 0. Sair                          ║
╚══════════════════════════════════╝
```

---

# 📌 Exemplo de Funcionamento

## Cadastro de voo

```txt
[OK] Voo 555 cadastrado: CNF (Belo Horizonte) -> GIG (Rio de Janeiro).
```

---

## Listagem de trajetos

```txt
Trajeto: GRU(Sao Paulo) -> CNF(Belo Horizonte) -> SSA(Salvador)
```

---

# 📚 Conceitos Aplicados

- Estruturas de Dados
- Ponteiros
- Alocação dinâmica
- Modularização em C
- Grafos direcionados
- Matrizes esparsas
- DFS recursiva
- Listas encadeadas

---

# 👨‍💻 Autor

Desenvolvido por João Pedro Rocha e Caio Cantarin.

- GitHub: https://github.com/Rnchx
- - GitHub: https://github.com/caio0409

---

# 📄 Licença

Projeto desenvolvido para fins acadêmicos.

---
⭐ Se gostou do projeto, deixe uma estrela no repositório!
