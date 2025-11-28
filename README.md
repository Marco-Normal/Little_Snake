# Little Snake

Este projeto é uma implementação multithreaded do clássico jogo da cobrinha (Snake) em C, utilizando a biblioteca `ncurses` para a interface gráfica. O objetivo principal deste projeto é demonstrar a aplicação prática de conceitos de Sistemas Operacionais, especificamente o uso de **Threads**, **Semáforos** e **Mutexes** para gerenciamento de concorrência e recursos compartilhados. A implementação é um pouco mais convoluta do que deveria ser, mas muito mais para conseguirmos encaixar uma região crítica do que, necessariamente, para funcionalidades do jogo

## Alunos
*  Augusto Fernandes Ildefonso (15441810)
*  Allan Vitor de Souza Silva (14712657)
*  Lucas Lombardi Castro (13672978)
*  Marco Túlio Mello Silva (12548657)

##  Sobre o Jogo

O jogo consiste em duas cobras competindo por comida em um tabuleiro:
1.  **Jogador (@)**: Controlado pelas teclas `W`, `A`, `S`, `D`.
2.  **Inimigo (E)**: Controlado por uma Inteligência Artificial que roda em uma thread separada e busca sempre a comida mais próxima.

O jogo termina se o jogador colidir consigo mesmo ou se pressionar `q`. Ao final, é exibido o placar comparando a pontuação do jogador com a do inimigo.

## Dependências

Para compilar e rodar este projeto, você precisará das seguintes ferramentas e bibliotecas instaladas no seu sistema Linux:

*   **GCC** 
*   **Make** 
*   **Ncurses** (Biblioteca gráfica para terminal)

**Importante**: O ncurses requer um tamanho mínimo de terminal. Se você estiver rodando em num notebook, para a garantia do funcionamento, garanta que apenas o terminal com o jogo esteja na tela.

Em distribuições baseadas em Debian/Ubuntu, você pode instalar as dependências com:
```bash
sudo apt-get update
sudo apt-get install build-essential libncurses5-dev
```

## Como Compilar e Rodar
### Compilar
Para compilar o projeto, execute o comando:
```bash
make
```
Isso irá gerar o executável `Main` e os arquivos objeto na pasta `build/`.

### Rodar
Para iniciar o jogo:
```bash
make run
```
Ou diretamente:
```bash
./Main
```

### Limpar
Para remover os arquivos compilados:
```bash
make clean
```

## Conceitos de Sistemas Operacionais Utilizados

O projeto foi estruturado para exercitar o controle de concorrência. Abaixo estão os detalhes de como cada componente foi utilizado:

### 1. Threads (`pthread`)
O jogo é dividido em três fluxos de execução principais que rodam concorrentemente:
*   **Thread Principal (Main)**: Responsável por capturar o input do usuário, atualizar a posição do jogador, renderizar o jogo na tela (desenhar o tabuleiro) e checar condições de fim de jogo.
*   **Thread de Comida (`food_routine`)**: Responsável por gerar novas comidas no tabuleiro aleatoriamente.
*   **Thread do Inimigo (`enemy_routine`)**: Responsável por calcular a lógica de movimento da cobra inimiga em direção à comida.

### 2. Semáforos (`sem_t`)
Utilizados para controlar recursos finitos, especificamente a quantidade de comida no tabuleiro.
*   **`n_food`**: Um semáforo inicializado com o valor 20.
    *   A thread de comida espera (`sem_wait`) por esse semáforo antes de colocar uma nova comida.
    *   Quando uma cobra come uma comida, o semáforo é liberado (`sem_post`), permitindo que a thread de comida gere mais um item. Isso implementa um padrão produtor-consumidor limitado.

### 3. Mutexes (`pthread_mutex_t`)
Utilizados para proteger **Regiões Críticas**, garantindo que apenas uma thread acesse dados compartilhados por vez, evitando *Race Conditions* (Condições de Corrida).
*   **`state.mutex`**: O "Big Lock" do jogo. Protege o acesso ao:
    *   **Array de Entidades (`objects`)**: Como as threads de comida, inimigo e principal tentam adicionar ou remover itens do tabuleiro simultaneamente, o acesso a este array deve ser atômico.
    *   **Ncurses**: As funções de desenho da biblioteca ncurses não são *thread-safe*. Portanto, o desenho na tela é protegido por este mutex.
    *   **Variáveis de Estado**: `score`, `game_running`, etc.
*   **`state.rand_mutex`**: Protege a chamada da função `rand()`, garantindo que a geração de números aleatórios seja segura entre threads (caso a implementação da `libc` não garanta thread-safety por padrão).

### 4. Variáveis de Condição (`pthread_cond_t`)
*   **`start_signal`**: Utilizado para sincronizar o início do jogo. As threads auxiliares (comida e inimigo) são criadas, mas esperam (`pthread_cond_wait`) até que a thread principal sinalize (`pthread_cond_broadcast`) que o jogo e a interface gráfica foram totalmente inicializados.
