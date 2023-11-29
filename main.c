//Codigo feito por Antônio, Giordana, Paula e Thales
//
#include "biblioteca.h"

//Mostrar Amanda
//Lista Simplesmente Encadeada
Ordem* pushLista(Ordem *arr)
{
    Ordem *novo = (Ordem*) malloc (sizeof(Ordem));
    int num = GetRandomValue(0, 3);
    //printf("\n Teste: %d \n", num);
    novo->cod = num;
    novo->prox = NULL;

    if(!arr)
    {
        return novo;
    }
    else
    {
        Ordem *tmp = arr;
        while(tmp->prox != NULL)
        {
            tmp = tmp->prox;
        }
        tmp->prox = novo;
        
        return arr;
    }
}

Ordem* novaLista(Ordem *arr)
{
    arr = NULL;

    for(int i = 0; i < maxOrdem; i++)
    {
        arr = pushLista(arr);
    }

    return arr;
}

void salvarScore()
{

    FILE *fptr;
    int tmpscore;
    char *nomeArq;

    if(modoJogo == 0)
    {
        nomeArq = nomeArq0;
    }
    else
    {
        nomeArq = nomeArq1;
    }

    fptr = fopen(nomeArq, "r+");

    fseek(fptr, 0, SEEK_SET);
    tmpscore = highScore;
    fprintf(fptr,"%d", tmpscore);
    fclose(fptr);

}

void lerArquivo()
{
    FILE *fptr;
    int tmpscore;
    char *nomeArq;

    //printf("modoJogo: %d\n", modoJogo);
    if(modoJogo == 0)
    {
        nomeArq = nomeArq0;
    }
    else
    {
        nomeArq = nomeArq1;
    }

    fptr = fopen(nomeArq, "r+");

    if(fptr==NULL)
    {
        fptr = fopen(nomeArq, "w");
        fprintf(fptr,"%d", 0);
        fclose(fptr);
        fptr = fopen(nomeArq, "r+");
    }

    fscanf(fptr, "%d", &tmpscore);

    //printf("tmpscore: %d\n", tmpscore);
    if(highScore==0 && tmpscore!=highScore)
    {
        highScore = tmpscore;
    }

    fclose(fptr);
}

void IniciarJogo(void)
{   
    waitComecar = true;
    contaFrames = 0;
    pausado = false;

    highScore = 0;
    score = 0;
    ordemCor = NULL;
    tetoOrdem = 1;
    posAtualOrdem = 0;
    posCorPiscando = 0;
    corPiscando = ordemCor;
    countPiscar = 60;

    tamCalda = 1;
    podeAndar = false;

    offset.x = screenHeight%QUADRADO_TAM;
    offset.y = screenWidth%QUADRADO_TAM;

    //Abrir o arquivo
    lerArquivo();

    for (int i = 0; i < COBRA_TAM; i++)
    {
        //Criando a cobra
        cobra[i].posicao = (Vector2){620 + offset.x/2, 341 + offset.y/2};
        cobra[i].tam = (Vector2){QUADRADO_TAM, QUADRADO_TAM};
        cobra[i].velocidade = (Vector2){QUADRADO_TAM, 0 };
        
        //Pintar quadrados da cobra
        if (i == 0)
        {
            cobra[i].cor = WHITE;
        }
        else
        {
            cobra[i].cor = WHITE;
        }
    }

    for (int i = 0; i < COBRA_TAM; i++)
    {
        posCobra[i] = (Vector2){ 0.0f, 0.0f};
    }

    //Criando frutas
    //Maca
    maca.tam = (Vector2){QUADRADO_TAM, QUADRADO_TAM};
    maca.cor = cVermelho;
    maca.ativo = false;
    maca.valor = 0;

    //Banana
    banana.tam = (Vector2){QUADRADO_TAM, QUADRADO_TAM};
    banana.cor = cAmarelo;
    banana.ativo = false;
    banana.valor = 1;
    //Blueberry
    blueberry.tam = (Vector2){QUADRADO_TAM, QUADRADO_TAM};
    blueberry.cor = cAzul;
    blueberry.ativo = false;
    blueberry.valor = 2;
    //Limao
    limao.tam = (Vector2){QUADRADO_TAM, QUADRADO_TAM};
    limao.cor = cVerde;
    limao.ativo = false;
    limao.valor = 3;

    bVermelho.posicao = (Vector2){310, 35};
    bVermelho.tam = (Vector2){BARRASX_TAM, BARRASY_TAM};
    bVermelho.cor = cVermelhoEscuro;

    bAmarelo.posicao = (Vector2){967, 35};
    bAmarelo.tam = (Vector2){BARRASX_TAM, BARRASY_TAM};
    bAmarelo.cor = cAmareloEscuro;

    bAzul.posicao = (Vector2){343, 658};
    bAzul.tam = (Vector2){BARRASY_TAM, BARRASX_TAM};
    bAzul.cor = cAzulEscuro;

    bVerde.posicao = (Vector2){343, 2};
    bVerde.tam = (Vector2){BARRASY_TAM, BARRASX_TAM};
    bVerde.cor = cVerdeEscuro;

    ordemCor = novaLista(ordemCor);

    frutaAtual = ordemCor;
    corPiscando = ordemCor;
    gameOver = false;
}

void DesenharTela(void)
{
    BeginDrawing();

    ClearBackground(BLACK);  
    if(sairMenu)
    {
        if (!gameOver)
        {
            //Testando
            if(debug)
            {
                DrawText(TextFormat("Cobra.x: %f", cobra[0].posicao.x), 0, 0, 20, RED);
                DrawText(TextFormat("Cobra.y: %f", cobra[0].posicao.y), 0, 20, 20, RED);
                DrawText(TextFormat("Parede.x: %f",  screenWidth - offset.x), 0, 40, 20, RED);
                DrawText(TextFormat("Parede.y: %f",  screenHeight - offset.y), 0, 60, 20, RED);
                DrawText(TextFormat("Maca.x: %f",  maca.posicao.x), 0, 80, 20, RED);
                DrawText(TextFormat("Maca.y: %f",  maca.posicao.y), 0, 100, 20, RED);
                DrawText(TextFormat("Diferenca maca.x - cobra.x: %f",  maca.posicao.x - cobra[0].posicao.x), 0, 120, 20, RED);
                if (modoJogo == 1)
                {
                    Ordem *tmp = ordemCor;
                    for(int i = 0; tmp; i++)
                    {
                        Color cor = RED;
                        if(i==posAtualOrdem) 
                        {
                            cor = YELLOW;
                        }
                        switch(tmp->cod)
                        {
                            case 0:
                                DrawText(TextFormat("OrdemCor: Vermelho"), 0, 220+20*(1+i), 20, cor);
                                break;
                            case 1:
                                DrawText(TextFormat("OrdemCor: Amarelo"), 0, 220+20*(1+i), 20, cor);
                                break;
                            case 2:
                                DrawText(TextFormat("OrdemCor: Azul"), 0, 220+20*(1+i), 20, cor);
                                break;
                            case 3:
                                DrawText(TextFormat("OrdemCor: Verde"), 0, 220+20*(1+i), 20, cor);
                                break;
                        }
                        
                        tmp = tmp->prox;
                    }
                }
            }

            //Desenhar a Pontuacao do lado direito da tela
            DrawText("SCORE", 1050, 0, 40, WHITE);
            if(score <= highScore)
            {
                DrawText(TextFormat("%03d", score), 1085, 40, 40, WHITE);
            }
            else
            {
                DrawText(TextFormat("%03d", score), 1085, 40, 40, cAmarelo);
            }

            //Desenhar o Highscore na tela
            DrawText("RECORD", 1050, 80, 40, WHITE);
            DrawText(TextFormat("%03d", highScore), 1085, 120, 40, WHITE);

            //Desenhar instrucoes do jogo no lado esquerdo da tela
            if(!debug)
            {
                DrawText("Use as |SETAS| ou |WASD|", 0, 100, 20, WHITE);
                DrawText("para controlar a cobra", 0, 120, 20, WHITE);
                DrawText("Aperte |P| para pausar", 0, 180, 20, WHITE);
                DrawText("Aperte |ESC| para sair", 0, 240, 20, WHITE);
            }

            //Desenha as linhas do mapa
            for(int i = 0; i < 620/QUADRADO_TAM + 1; i++)
            {
                DrawLineV((Vector2){QUADRADO_TAM*i + offset.x/2 + 340, offset.y/2 + 31}, (Vector2){QUADRADO_TAM*i + offset.x/2 + 340, 628 - offset.y/2 + 31}, (Color){38, 38, 38, 255});
            }

            for(int i = 0; i < 620/QUADRADO_TAM + 1; i++)
            {
                DrawLineV((Vector2){offset.x/2 + 340, QUADRADO_TAM*i + offset.y/2 + 31}, (Vector2){628 - offset.x/2 + 340, QUADRADO_TAM*i + offset.y/2 + 31}, (Color){38, 38, 38, 255});
            }

            //Desenhar a cobra
            for (int i = 0; i < tamCalda; i++)
            {
                DrawRectangleV(cobra[i].posicao, cobra[i].tam, cobra[i].cor);
            }

            //Desenhar a maca
            DrawRectangleV(maca.posicao, maca.tam, maca.cor);
            if (modoJogo == 1)
            {
                //Se o modo de jogo eh o MEMORY desenha as outras 3 frutas
                DrawRectangleV(banana.posicao, banana.tam, banana.cor);
                DrawRectangleV(blueberry.posicao, blueberry.tam, blueberry.cor);
                DrawRectangleV(limao.posicao, limao.tam, limao.cor);
                
                //Desenhar as barras do modo MEMORY
                DrawRectangleV(bVermelho.posicao, bVermelho.tam, bVermelho.cor);
                DrawRectangleV(bAmarelo.posicao, bAmarelo.tam, bAmarelo.cor);
                DrawRectangleV(bAzul.posicao, bAzul.tam, bAzul.cor);
                DrawRectangleV(bVerde.posicao, bVerde.tam, bVerde.cor);

            }
        
            if(pausado)
            {
                DrawText("PAUSADO", screenWidth/2 - MeasureText("PAUSADO", 40)/2, screenHeight/2 -40, 40, GRAY);
            }

            if(waitComecar)
            {
                DrawText("APERTE [ESPACO] PARA INICIAR", screenWidth/2 - MeasureText("APERTE [ESPACO] PARA INICIAR", 40)/2, screenHeight/2 -40, 40, GRAY);
            }
        }
        else
        {
            DrawText("APERTE [ENTER] PARA JOGAR NOVAMENTE", GetScreenWidth()/2 - MeasureText("APERTE [ENTER] PARA JOGAR NOVAMENTE", 20)/2, GetScreenHeight()/2 -50, 20, LIGHTGRAY);
            DrawText("APERTE [M] PARA VOLTAR PARA O MENU", GetScreenWidth()/2 - MeasureText("APERTE [M] PARA VOLTAR PARA O MENU", 20)/2, GetScreenHeight()/2, 20, LIGHTGRAY);
        }
    }
    else
    {
        DrawText("MEMORY SNAKE", GetScreenWidth()/2 - MeasureText("MEMORY SNAKE", 120)/2, GetScreenHeight()/2 -250, 120, WHITE);
        DrawText("ESCOLHA O MODO DE JOGO:", GetScreenWidth()/2 - MeasureText("ESCOLHA O MODO DE JOGO:", 40)/2, GetScreenHeight()/2 -100, 40, LIGHTGRAY);
        DrawText("[1] MODO CLASSICO", GetScreenWidth()/2 - MeasureText("[1] MODO CLASSICO", 40)/2, GetScreenHeight()/2, 40, RED);
        DrawText("[2] MODO MEMORY", GetScreenWidth()/2 - MeasureText("[2] MODO MEMORY", 40)/2, GetScreenHeight()/2 + 50, 40, RED);
        DrawText("[ESC] SAIR", GetScreenWidth()/2 - MeasureText("[ESC] SAIR", 40)/2, GetScreenHeight()/2 + 100, 40, RED);
        DrawText("FEITO POR:", GetScreenWidth()/2 - MeasureText("FEITO POR:", 20)/2, GetScreenHeight()/2 + 200, 20, WHITE);
        DrawText("Antonio - Giordana - Paula - Thales", GetScreenWidth()/2 - MeasureText("Antonio, Giordana, Paula, Thales", 20)/2, GetScreenHeight()/2 + 230, 20, WHITE);

        if(IsKeyPressed(KEY_ONE))
        {
            modoJogo = 0;
            sairMenu = true;
            IniciarJogo();
        }
        if(IsKeyPressed(KEY_TWO))
        {
            modoJogo = 1;
            sairMenu = true;
            IniciarJogo();
        }

    }
    EndDrawing();

}

void AtualizarJogo(void)
{

    
    if(sairMenu)
    {
        if(!gameOver)
        {
            //("ContaFrames: %d\n", contaFrames);
            if (contaFrames == 0)
            {
                waitComecar = true;
            }
            //printf("waitComecar: %d\n", waitComecar);

            if(!waitComecar)
            {
                if(IsKeyPressed(KEY_INSERT))
                {
                    debug = !debug;
                }

                if (IsKeyPressed(KEY_P))
                {
                    pausado = !pausado;
                }

                if (!pausado)
                {

                    //printf("countPiscar: %d , PodePiscar: %d\n", countPiscar, podePiscar);
                    //Controle do Jogador
                    //Controla apenas o primeiro quadrado da cobra
                    
                    if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && (cobra[0].velocidade.x == 0) && podeAndar)
                    {
                        //Velocidade da cobra eh de 1 quadrado por vez
                        cobra[0].velocidade = (Vector2) { QUADRADO_TAM, 0 };
                        //pondeAndar precisa voltar para falso porque a cobra ja andou nesse frame
                        podeAndar = false;
                    }
                    if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && (cobra[0].velocidade.x == 0) && podeAndar)
                    {
                        cobra[0].velocidade = (Vector2) { -QUADRADO_TAM, 0 };
                        podeAndar = false;
                    }
                    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && (cobra[0].velocidade.y == 0) && podeAndar)
                    {
                        cobra[0].velocidade = (Vector2) { 0, -QUADRADO_TAM};
                        podeAndar = false;
                    }
                    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && (cobra[0].velocidade.y == 0) && podeAndar)
                    {
                        cobra[0].velocidade = (Vector2) { 0, QUADRADO_TAM};
                        podeAndar = false;
                    }

                    //Faz a cobra andar
                    for(int i = 0; i < tamCalda; i++)
                    {
                        posCobra[i] = cobra[i].posicao;
                    }

                    if ((contaFrames%8) == 0)
                    {
                        for(int i = 0; i < tamCalda; i++)
                        {
                            if (i == 0)
                            {
                                cobra[i].posicao.x += cobra[i].velocidade.x;
                                cobra[i].posicao.y += cobra[i].velocidade.y;
                                podeAndar = true;
                            }
                            else
                            {
                                cobra[i].posicao = posCobra[i-1];
                            }
                        }
                    }

                    //Interacao com a parede do mapa
                    if (((cobra[0].posicao.x) > (screenWidth - 340)) || ((cobra[0].posicao.y) > 625) || (cobra[0].posicao.x < 340) || (cobra[0].posicao.y < 35))
                    {
                        gameOver = true;
                    }

                    //Colisao da cobra com a calda
                    for(int i = 1; i < tamCalda; i++)
                    {
                        if((cobra[0].posicao.x == cobra[i].posicao.x) && (cobra[0].posicao.y == cobra[i].posicao.y))
                        {
                            gameOver = true;
                        }
                    }

                    //Spawnar as frutas
                    //Maca
                    if(!maca.ativo)
                    {
                        //Sempre tem que ter uma fruta na tela
                        maca.ativo = true;
                        //Coloca a maca em algum quadrado da tela
                        maca.posicao = (Vector2){ GetRandomValue(344.5/QUADRADO_TAM, 933.5/QUADRADO_TAM) * QUADRADO_TAM + offset.x/2, GetRandomValue(35.5/QUADRADO_TAM, 624.5/QUADRADO_TAM) * QUADRADO_TAM + offset.y/2};

                        //maca.posicao = (Vector2){(933.5/QUADRADO_TAM) * QUADRADO_TAM, 624.5/QUADRADO_TAM *QUADRADO_TAM};

                        for(int i = 0; i < tamCalda; i++)
                        {
                            //Verifica se a maca nao ira spawnar em uma posicao que a cobra ja ocupa
                            while(((maca.posicao.x == cobra[i].posicao.x) && ((((maca.posicao.x) - (cobra[0].posicao.x) > 62 || (maca.posicao.x) - (cobra[0].posicao.x) < -62) && ((maca.posicao.y) - (cobra[0].posicao.y) > 62 || (maca.posicao.y) - (cobra[0].posicao.y) < -62)) || score >= 300) && (maca.posicao.y == cobra[i].posicao.y)) || ((maca.posicao.x == banana.posicao.x) && (maca.posicao.y == banana.posicao.y)) || ((maca.posicao.x == blueberry.posicao.x) && (maca.posicao.y == blueberry.posicao.y)) || ((maca.posicao.x == limao.posicao.x) && (maca.posicao.y == limao.posicao.y)))
                            {
                                maca.posicao = (Vector2){ GetRandomValue(344.5/QUADRADO_TAM, 933.5/QUADRADO_TAM) * QUADRADO_TAM + offset.x/2, GetRandomValue(35.5/QUADRADO_TAM, 624.5/QUADRADO_TAM) * QUADRADO_TAM + offset.y/2};
                                i = 0;
                                //Com a nova posicao faz o check inteiro novamente
                            }
                        }

                    }

                    if(modoJogo == 1)
                    {
                        //Banana
                        if(!banana.ativo)
                        {
                            //Sempre tem que ter uma fruta na tela
                            banana.ativo = true;
                            //Coloca a maca em algum quadrado da tela
                            banana.posicao = (Vector2){ GetRandomValue(344.5/QUADRADO_TAM, 933.5/QUADRADO_TAM) * QUADRADO_TAM + offset.x/2, GetRandomValue(35.5/QUADRADO_TAM, 624.5/QUADRADO_TAM) * QUADRADO_TAM + offset.y/2};

                            //maca.posicao = (Vector2){(933.5/QUADRADO_TAM) * QUADRADO_TAM, 624.5/QUADRADO_TAM *QUADRADO_TAM};

                            for(int i = 0; i < tamCalda; i++)
                            {
                                //Verifica se a maca nao ira spawnar em uma posicao que a cobra ja ocupa
                                while(((banana.posicao.x == cobra[i].posicao.x) && ((((banana.posicao.x) - (cobra[0].posicao.x) > 62 || (banana.posicao.x) - (cobra[0].posicao.x) < -62) && ((banana.posicao.y) - (cobra[0].posicao.y) > 62 || (banana.posicao.y) - (cobra[0].posicao.y) < -62)) || score >= 300) && (banana.posicao.y == cobra[i].posicao.y)) || ((banana.posicao.x == maca.posicao.x) && (banana.posicao.y == maca.posicao.y)) || ((banana.posicao.x == blueberry.posicao.x) && (banana.posicao.y == blueberry.posicao.y)) || ((banana.posicao.x == limao.posicao.x) && (banana.posicao.y == limao.posicao.y)))
                                {
                                    banana.posicao = (Vector2){ GetRandomValue(344.5/QUADRADO_TAM, 933.5/QUADRADO_TAM) * QUADRADO_TAM + offset.x/2, GetRandomValue(35.5/QUADRADO_TAM, 624.5/QUADRADO_TAM) * QUADRADO_TAM + offset.y/2};
                                    i = 0;
                                    //Com a nova posicao faz o check inteiro novamente
                                }
                            }

                        }
                        //Blueberry
                        if(!blueberry.ativo)
                        {
                            //Sempre tem que ter uma fruta na tela
                            blueberry.ativo = true;
                            //Coloca a maca em algum quadrado da tela
                            blueberry.posicao = (Vector2){ GetRandomValue(344.5/QUADRADO_TAM, 933.5/QUADRADO_TAM) * QUADRADO_TAM + offset.x/2, GetRandomValue(35.5/QUADRADO_TAM, 624.5/QUADRADO_TAM) * QUADRADO_TAM + offset.y/2};

                            //maca.posicao = (Vector2){(933.5/QUADRADO_TAM) * QUADRADO_TAM, 624.5/QUADRADO_TAM *QUADRADO_TAM};

                            for(int i = 0; i < tamCalda; i++)
                            {
                                //Verifica se a maca nao ira spawnar em uma posicao que a cobra ja ocupa
                                while(((blueberry.posicao.x == cobra[i].posicao.x) && ((((blueberry.posicao.x) - (cobra[0].posicao.x) > 62 || (blueberry.posicao.x) - (cobra[0].posicao.x) < -62) && ((blueberry.posicao.y) - (cobra[0].posicao.y) > 62 || (blueberry.posicao.y) - (cobra[0].posicao.y) < -62)) || score >= 300) && (blueberry.posicao.y == cobra[i].posicao.y)) || ((blueberry.posicao.x == banana.posicao.x) && (blueberry.posicao.y == banana.posicao.y)) || ((blueberry.posicao.x == maca.posicao.x) && (blueberry.posicao.y == maca.posicao.y)) || ((blueberry.posicao.x == limao.posicao.x) && (blueberry.posicao.y == limao.posicao.y)))
                                {
                                    blueberry.posicao = (Vector2){ GetRandomValue(344.5/QUADRADO_TAM, 933.5/QUADRADO_TAM) * QUADRADO_TAM + offset.x/2, GetRandomValue(35.5/QUADRADO_TAM, 624.5/QUADRADO_TAM) * QUADRADO_TAM + offset.y/2};
                                    i = 0;
                                    //Com a nova posicao faz o check inteiro novamente
                                }
                            }

                        }
                        //Limao
                        if(!limao.ativo)
                        {
                            //Sempre tem que ter uma fruta na tela
                            limao.ativo = true;
                            //Coloca a maca em algum quadrado da tela
                            limao.posicao = (Vector2){ GetRandomValue(344.5/QUADRADO_TAM, 933.5/QUADRADO_TAM) * QUADRADO_TAM + offset.x/2, GetRandomValue(35.5/QUADRADO_TAM, 624.5/QUADRADO_TAM) * QUADRADO_TAM + offset.y/2};

                            //maca.posicao = (Vector2){(933.5/QUADRADO_TAM) * QUADRADO_TAM, 624.5/QUADRADO_TAM *QUADRADO_TAM};

                            for(int i = 0; i < tamCalda; i++)
                            {
                                //Verifica se a maca nao ira spawnar em uma posicao que a cobra ja ocupa
                                while(((limao.posicao.x == cobra[i].posicao.x) && ((((limao.posicao.x) - (cobra[0].posicao.x) > 62 || (limao.posicao.x) - (cobra[0].posicao.x) < -62) && ((limao.posicao.y) - (cobra[0].posicao.y) > 62 || (limao.posicao.y) - (cobra[0].posicao.y) < -62)) || score >= 300) && (limao.posicao.y == cobra[i].posicao.y)) || ((limao.posicao.x == banana.posicao.x) && (limao.posicao.y == banana.posicao.y)) || ((limao.posicao.x == blueberry.posicao.x) && (limao.posicao.y == blueberry.posicao.y)) || ((limao.posicao.x == maca.posicao.x) && (limao.posicao.y == maca.posicao.y)))
                                {
                                    limao.posicao = (Vector2){ GetRandomValue(344.5/QUADRADO_TAM, 933.5/QUADRADO_TAM) * QUADRADO_TAM + offset.x/2, GetRandomValue(35.5/QUADRADO_TAM, 624.5/QUADRADO_TAM) * QUADRADO_TAM + offset.y/2};
                                    i = 0;
                                    //Com a nova posicao faz o check inteiro novamente
                                }
                            }

                        }

                    }

                    //Colisao da cobra com a maca
                    if ((cobra[0].posicao.x < (maca.posicao.x + maca.tam.x) && (cobra[0].posicao.x + cobra[0].tam.x) > maca.posicao.x) && cobra[0].posicao.y < (maca.posicao.y + maca.tam.y) && (cobra[0].posicao.y + cobra[0].tam.y) > maca.posicao.y)
                    {
                        //A cobra vai aumentar 1 posicao, por isso ela precisa ir 1 quadrado para tras
                        cobra[tamCalda].posicao = posCobra[tamCalda - 1];
                        tamCalda += 1;
                        maca.ativo = false;
                        if(modoJogo == 0)
                        {
                            score++;
                        }
                        else
                        {
                            frutaComida = maca.valor;
                        }
                    }
                    if(modoJogo == 1)
                    {
                        //Colisao da cobra com a banana
                        if ((cobra[0].posicao.x < (banana.posicao.x + banana.tam.x) && (cobra[0].posicao.x + cobra[0].tam.x) > banana.posicao.x) && cobra[0].posicao.y < (banana.posicao.y + banana.tam.y) && (cobra[0].posicao.y + cobra[0].tam.y) > banana.posicao.y)
                        {
                            //A cobra vai aumentar 1 posicao, por isso ela precisa ir 1 quadrado para tras
                            cobra[tamCalda].posicao = posCobra[tamCalda - 1];
                            tamCalda += 1;
                            banana.ativo = false;
                            frutaComida = banana.valor;
                        }

                        //Colisao da cobra com a blueberry
                        if ((cobra[0].posicao.x < (blueberry.posicao.x + blueberry.tam.x) && (cobra[0].posicao.x + cobra[0].tam.x) > blueberry.posicao.x) && cobra[0].posicao.y < (blueberry.posicao.y + maca.tam.y) && (cobra[0].posicao.y + cobra[0].tam.y) > blueberry.posicao.y)
                        {
                            //A cobra vai aumentar 1 posicao, por isso ela precisa ir 1 quadrado para tras
                            cobra[tamCalda].posicao = posCobra[tamCalda - 1];
                            tamCalda += 1;
                            blueberry.ativo = false;
                            frutaComida = blueberry.valor;
                        }

                        //Colisao da cobra com o limao
                        if ((cobra[0].posicao.x < (limao.posicao.x + limao.tam.x) && (cobra[0].posicao.x + cobra[0].tam.x) > limao.posicao.x) && cobra[0].posicao.y < (limao.posicao.y + limao.tam.y) && (cobra[0].posicao.y + cobra[0].tam.y) > limao.posicao.y)
                        {
                            //A cobra vai aumentar 1 posicao, por isso ela precisa ir 1 quadrado para tras
                            cobra[tamCalda].posicao = posCobra[tamCalda - 1];
                            tamCalda += 1;
                            limao.ativo = false;
                            frutaComida = limao.valor;
                        }

                        //Pisca as barras
                        if(podePiscar && posCorPiscando < tetoOrdem)
                        {
                            switch(corPiscando->cod)
                            {
                                case 0:
                                    bVermelho.cor = cVermelho;
                                    break;
                                case 1:
                                    bAmarelo.cor = cAmarelo;
                                    break;
                                case 2:
                                    bAzul.cor = cAzul;
                                    break;
                                case 3:
                                    bVerde.cor = cVerde;
                                    break;
                            }
                            if(posCorPiscando < tetoOrdem)
                            {
                                corPiscando = corPiscando->prox;
                                posCorPiscando++;
                                podePiscar = false;
                            }
                        }

                        //printf("modoPiscar: %d\n", modoPiscar);

                        //Voltar a cor das barras pro normal
                        if(countPiscar == 0)
                        {
                            bVermelho.cor = cVermelhoEscuro;
                            bAmarelo.cor = cAmareloEscuro;
                            bAzul.cor = cAzulEscuro;
                            bVerde.cor = cVerdeEscuro;
                            countPiscar--;
                        }
                        else if(countPiscar <= -30)
                        {
                            //printf("Entrou\n");
                            countPiscar = 120;
                            podePiscar = true;

                            if(posCorPiscando >= tetoOrdem)
                            {
                                modoPiscar = false;
                            }
                        }
                        else if(!podePiscar)
                        {
                            countPiscar--;
                        }
                        
                        //Mostrar as cores das frutas apenas quando a ordem acabar de ser mostrada
                        if(modoPiscar)
                        {
                            maca.cor = cBranco;
                            banana.cor = cBranco;
                            blueberry.cor = cBranco;
                            limao.cor = cBranco;
                        }
                        else
                        {
                            maca.cor = cVermelho;
                            banana.cor = cAmarelo;
                            blueberry.cor = cAzul;
                            limao.cor = cVerde;
                        }

                    }

                    //Avanca a mostragem da ordem apos o jogador acertar a ordem
                    if(frutaComida != -1)
                    {
                        if (frutaComida == frutaAtual->cod)
                        {
                            score++;
                            posAtualOrdem++;
                            if(posAtualOrdem >= tetoOrdem)
                            {
                                posCorPiscando = 0;
                                modoPiscar = true;
                                corPiscando = ordemCor;
                                tetoOrdem++;
                                
                                if(tetoOrdem > maxOrdem)
                                {
                                    free(ordemCor);
                                    ordemCor = novaLista(ordemCor);
                                    tetoOrdem = 1;
                                }
                                posAtualOrdem = 0;
                                frutaAtual = ordemCor;
                            }
                            else
                            {
                                frutaAtual = frutaAtual->prox;
                            }
                        }
                        else
                        {
                            gameOver = true;
                        }
                    }


                    frutaComida = -1;
                    contaFrames++;
                }
            }
            else
            {
                if(IsKeyPressed(KEY_SPACE))
                {
                    waitComecar = false;
                    contaFrames++;
                }
            }

        }
        else
        {
            if(score>highScore)
            {
                highScore = score;
                salvarScore();
            }
            if(IsKeyPressed(KEY_ENTER))
            {
                free(ordemCor);
                IniciarJogo();
            }
            if(IsKeyPressed(KEY_M))
            {
                free(ordemCor);
                sairMenu = false;
                gameOver = true;
                IniciarJogo();
            }
        }
    }
}

void AtualizarTela(void)
{
    DesenharTela();
    AtualizarJogo();
}

int main(void)
{
    // Inicializa a janela do jogo
    
    InitWindow(screenWidth, screenHeight, "Memory Snake");

    // Faz o jogo rodar a 60 frames por segundo
    SetTargetFPS(60);               

    // Loop primario do jogo

    ordemCor = NULL;

    IniciarJogo();

    while(!WindowShouldClose())
    {
        AtualizarTela();
    }

    free(ordemCor);

    CloseWindow();

    return 0;
}
