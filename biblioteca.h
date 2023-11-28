#ifndef BIBLIOTECA_FILE
#define BIBLIOTECA_FILE

//Includes
#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"

//Define o tamanho maximo da cobra e o tamanho do quadrado utilizado para desenho e para logica do jogo
#define COBRA_TAM 400
#define QUADRADO_TAM 31
#define BARRASX_TAM 31
#define BARRASY_TAM 620

//Estruturas da cobra e das frutas
typedef struct cobra Cobra;
struct cobra
{
    Vector2 posicao;
    Vector2 tam;
    Vector2 velocidade;
    Color cor;
};

typedef struct fruta Fruta;
struct fruta
{
    Vector2 posicao;
    Vector2 tam;
    bool ativo;
    Color cor;
    int valor;
};

typedef struct barras Barras;
struct barras
{
    Vector2 posicao;
    Vector2 tam;
    Color cor;
};

//Estrutura da ordem das cores
typedef struct ordem Ordem;
struct ordem
{
    int cod;
    Ordem *prox;
};

//Variaveis globais
static const int screenWidth = 1280;
static const int screenHeight = 720;

static bool podeAndar = false;
static bool gameOver = false;
static bool pausado = false;
static bool waitComecar = false;
static bool sairMenu = false;
static bool debug = false;
static bool podePiscar = false;
static bool modoPiscar = true;

static Fruta maca = { 0 };
static Fruta banana = { 0 };
static Fruta blueberry = { 0 };
static Fruta limao = { 0 };

static Barras bVermelho = { 0 };
static Barras bAmarelo = { 0 };
static Barras bAzul = { 0 };
static Barras bVerde = { 0 };

//Cores padroes
static Color cVermelho = (Color){230,41,55,255};
static Color cAmarelo = (Color){253,249,0,255};
static Color cAzul = (Color){0,82,172,255};
static Color cVerde = (Color){0,228,48,255};

static Color cVermelhoEscuro = (Color){92, 16, 22,255};
static Color cAmareloEscuro = (Color){92, 91, 2,255};
static Color cAzulEscuro = (Color){1, 27, 56,255};
static Color cVerdeEscuro = (Color){1, 89, 19,255};
static Color cBranco = WHITE;


static Cobra cobra[COBRA_TAM] = { 0 };

static Vector2 posCobra[COBRA_TAM] = { 0 };
static Vector2 offset = { 0 };

static int tamCalda = 0;
static int modoJogo = 0;
static int contaFrames = 0;
static int countPiscar = 0;
static int score = 0;
static int highScore = 0;
static int frutaComida = -1;

static int maxOrdem = 10;
static int tetoOrdem = 1;
static int posAtualOrdem = 0;
static int posCorPiscando = 0;

static Ordem *ordemCor;
static Ordem *corPiscando;
static Ordem *frutaAtual;
static char nomeArq0[] = "data0.txt";
static char nomeArq1[] = "data1.txt";

#endif