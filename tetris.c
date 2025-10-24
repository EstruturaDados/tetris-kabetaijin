/**
 * @file tetris_stack_novato.c
 * @brief Implementação do sistema de Fila Circular de Peças Futuras (Nível Novato).
 * O sistema gerencia uma fila circular de 5 peças de Tetris, permitindo
 * a visualização, a remoção da frente ('Jogar') e a inserção no final ('Gerar Nova').
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Definindo o tamanho fixo da fila, conforme o requisito.
#define TAMANHO_FILA 5

// Estrutura para representar uma peça de Tetris (Requisito: Struct)
typedef struct {
    int id;
    char nome[2]; // Suficiente para 'I\0', 'O\0', 'T\0', 'L\0', etc.
} Peca;

// Estrutura para a Fila Circular
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente; // Índice do primeiro elemento
    int tras;   // Índice do último elemento inserido
    int count;  // Número atual de elementos na fila
} FilaPecas;

// Contador global para garantir IDs únicos para cada peça gerada
int next_id = 1;

// -----------------------------------------------------------------------------
// FUNÇÕES AUXILIARES E DE GERAÇÃO
// -----------------------------------------------------------------------------

/**
 * @brief Gera uma nova peça com ID exclusivo e um nome aleatório.
 * @return A nova peça gerada.
 */
Peca gerarPeca() {
    Peca nova_peca;
    nova_peca.id = next_id++; // Atribui ID e incrementa o contador
    
    // Nomes de peças do Tetris (I, O, T, L, J, S, Z)
    char nomes_disponiveis[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int num_tipos = sizeof(nomes_disponiveis) / sizeof(nomes_disponiveis[0]);
    
    // Seleciona um nome aleatoriamente
    int indice_aleatorio = rand() % num_tipos;
    nova_peca.nome[0] = nomes_disponiveis[indice_aleatorio];
    nova_peca.nome[1] = '\0';

    return nova_peca;
}

/**
 * @brief Inicializa a fila circular.
 * @param fila Ponteiro para a estrutura da fila.
 */
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1; // Usamos -1 para indicar que a fila está logicamente vazia
    fila->count = 0;
    
    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));
    
    // Preenche a fila com 5 peças iniciais (Requisito)
    printf("Inicializando fila com %d peças...\n", TAMANHO_FILA);
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca p = gerarPeca();
        
        // Simulação do Enqueue para preenchimento inicial:
        // Atualiza 'tras' e adiciona a peça
        fila->tras = (fila->tras + 1) % TAMANHO_FILA;
        fila->pecas[fila->tras] = p;
        fila->count++;
        printf(" -> Peça inicializada: [%s, ID: %d]\n", p.nome, p.id);
    }
}

// -----------------------------------------------------------------------------
// FUNCIONALIDADES PRINCIPAIS DA FILA
// -----------------------------------------------------------------------------

/**
 * @brief Insere uma nova peça no final da fila (Enqueue).
 * @param fila Ponteiro para a estrutura da fila.
 */
void inserirPeca(FilaPecas *fila) {
    if (fila->count == TAMANHO_FILA) {
        printf("\n[ERRO] A fila está cheia. Não é possível inserir mais peças.\n");
        return;
    }
    
    // 1. Gera a nova peça
    Peca nova_peca = gerarPeca();

    // 2. Atualiza o índice 'tras' para a próxima posição (circular)
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    
    // 3. Insere a peça na posição de 'tras'
    fila->pecas[fila->tras] = nova_peca;
    
    // 4. Incrementa a contagem
    fila->count++;

    printf("\n[SUCESSO] NOVA PEÇA INSERIDA: [%s, ID: %d] no final da fila.\n", 
           nova_peca.nome, nova_peca.id);
}

/**
 * @brief Remove a peça da frente da fila (Dequeue).
 * @param fila Ponteiro para a estrutura da fila.
 * @return A peça removida.
 */
Peca jogarPeca(FilaPecas *fila) {
    // Cria uma peça de retorno padrão (em caso de fila vazia)
    Peca peca_nula = {-1, "N\0"};

    if (fila->count == 0) {
        printf("\n[ERRO] A fila está vazia. Não há peças para jogar.\n");
        return peca_nula;
    }
    
    // 1. Armazena a peça da frente para retorno
    Peca peca_jogada = fila->pecas[fila->frente];

    // 2. Atualiza o índice 'frente' para o próximo elemento (circular)
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    
    // 3. Decrementa a contagem
    fila->count--;

    printf("\n[JOGADA] PEÇA REMOVIDA: [%s, ID: %d] jogada no campo de jogo.\n", 
           peca_jogada.nome, peca_jogada.id);
    
    // Chama automaticamente a inserção para manter o tamanho (com verificação)
    printf("[AUTORUN] Inserindo automaticamente uma nova peça...\n");
    inserirPeca(fila);
    
    return peca_jogada;
}

/**
 * @brief Exibe o estado atual da fila, mostrando a ordem das peças.
 * @param fila Ponteiro para a estrutura da fila.
 */
void visualizarFila(FilaPecas *fila) {
    printf("\n--- VISUALIZAÇÃO DA FILA DE PEÇAS FUTURAS ---\n");
    printf("Capacidade: %d | Itens Atuais: %d\n", TAMANHO_FILA, fila->count);
    
    if (fila->count == 0) {
        printf("[VAZIA] Não há peças futuras.\n");
        return;
    }
    
    printf("Ordem da Fila (FRENTE -> TRÁS):\n");
    
    int i;
    int indice_atual = fila->frente;
    
    // Percorre a fila a partir da 'frente' por 'count' vezes (lógica circular)
    for (i = 0; i < fila->count; i++) {
        Peca p = fila->pecas[indice_atual];
        
        printf(" [%s, ID: %d] ", p.nome, p.id);
        
        // Indica o início e o fim da fila
        if (i == 0) {
            printf("<-- FRENTE (Próxima a ser jogada)");
        } else if (i == fila->count - 1) {
            printf("<-- TRÁS (Última inserida)");
        }
        printf("\n");
        
        // Avança o índice de forma circular
        indice_atual = (indice_atual + 1) % TAMANHO_FILA;
    }
    printf("---------------------------------------------------\n");
}


// -----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL E MENU
// -----------------------------------------------------------------------------

int main() {
    FilaPecas fila;
    inicializarFila(&fila);
    int opcao;

    do {
        visualizarFila(&fila);

        printf("\n--- MENU DE CONTROLE ---\n");
        printf("1. JOGAR PEÇA (Dequeue + Enqueue Automático)\n");
        printf("2. VISUALIZAR FILA (Atualizar)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        // Limpa o buffer antes de ler (para evitar lixo de entradas anteriores)
        if (scanf("%d", &opcao) != 1) {
            // Limpa a entrada inválida
            while (getchar() != '\n'); 
            opcao = -1; // Opção inválida
        }

        switch (opcao) {
            case 1:
                // Joga a peça da frente e automaticamente insere uma nova
                jogarPeca(&fila);
                break;
            case 2:
                // A visualização já ocorre no início do loop, mas é útil para reexibir.
                break;
            case 0:
                printf("\nEncerrando o Desafio Tetris Stack - Nível Novato.\n");
                break;
            default:
                printf("\n[AVISO] Opção inválida. Digite 1, 2 ou 0.\n");
                break;
        }
        
    } while (opcao != 0);

    return 0;
}