#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <time.h>

//time. h e window.h são bibliotecas não padrão mas já incluídas no programa

#define LARGURA_TELA 30
#define ALTURA_TELA 15
#define LARGURA_JOGADOR 3
#define MAX_INIMIGOS_FACIL 15
#define MAX_INIMIGOS_MEDIO 17
#define MAX_INIMIGOS_DIFICIL 19
#define INTERVALO_SPAWN_INIMIGO 5

//macros que definem globalmente algumas variáveis

int main();  
int _kbhit(void);
int _getch(void);
int dificuldade;
int larg_inimigo = 1;
int rec_fac = 0;
int rec_med = 0;
int rec_dif = 0;
int skin = 1;
int cont_controle = 0;
int moeda = 0;
int licenca_skin4 = 0;
int licenca_skin5 = 0;
int licenca_skin6 = 0;
//variáveis globais

typedef struct {
    int x, y;
    int lives;
    int score;
} Player;
//estrutura que atribui características ao player

typedef struct {
    int x, y;
    int active;
    int direction;
} Enemy;
//estrutura que atribui características ao inimigo

void inicializarJogador(Player *player) {
    player->x = LARGURA_TELA / 2;
    player->y = ALTURA_TELA / 2;
    player->score = 0;
    
    if(dificuldade == 1){
    	player->lives = 5;
	}else if(dificuldade == 2){
		player->lives = 4;
	}else if(dificuldade == 3){
		player->lives = 3;
	}
}
//void que inicia o jogador centralizado na tela e define vidas beaseadas na dificuldade

void pause() {
	system("pause");
}
void limpT() {
    system("cls");
}
//voids que simplificam a sintaxe do código (limpa a tela e pausa o programa)

void atualizarJogador(Player *player, char entrada) {
    if (entrada == 'a' && player->x > 0) {
        player->x--;
    } else if (entrada == 'd' && player->x < LARGURA_TELA - LARGURA_JOGADOR) {
        player->x++;
    } else if (entrada == 'w' && player->y > 0) {
        player->y--;
    } else if (entrada == 's' && player->y < ALTURA_TELA - 1) {
        player->y++;
    }
}
//void que define a movimentação do jogador e atualiza-o dependendo da ação

void inicializarInimigo(Enemy *enemy, int x, int y) {
    enemy->x = x;
    enemy->y = y;
    enemy->active = 0;
    enemy->direction = rand() % 8;
}
//void que inicializa o inimigo em posições aleatórias (que não o 'x' do player)

void ativarInimigo(Enemy *enemy, int x, int y) {
    enemy->x = x;
    enemy->y = y;
    enemy->active = 1;
    enemy->direction = rand() % 8;
}
//void que ativa a movimentação do inimigo

void atualizarInimigo(Enemy *enemy, Player *player, Enemy *inimigos, int *numInimigos) {
    if (enemy->active) {
        switch (enemy->direction) {
            case 0:
                if (enemy->x < LARGURA_TELA - larg_inimigo) {
                    enemy->x++;
                } else {
                    enemy->direction = rand() % 8;
                }
                break;
            case 1:
                if (enemy->x > 0) {
                    enemy->x--;
                } else {
                    enemy->direction = rand() % 8;
                }
                break;
            case 2:
                if (enemy->y > 0) {
                    enemy->y--;
                } else {
                    enemy->direction = rand() % 8;
                }
                break;
            case 3:
                if (enemy->y < ALTURA_TELA - 1) {
                    enemy->y++;
                } else {
                    enemy->direction = rand() % 8;
                }
                break;
            case 4:
                if (enemy->x < LARGURA_TELA - larg_inimigo && enemy->y > 0) {
                    enemy->x++;
                    enemy->y--;
                } else {
                    enemy->direction = rand() % 8;
                }
                break;
            case 5:
                if (enemy->x > 0 && enemy->y > 0) {
                    enemy->x--;
                    enemy->y--;
                } else {
                    enemy->direction = rand() % 8;
                }
                break;
            case 6:
                if (enemy->x < LARGURA_TELA - larg_inimigo && enemy->y < ALTURA_TELA - 1) {
                    enemy->x++;
                    enemy->y++;
                } else {
                    enemy->direction = rand() % 8;
                }
                break;
            case 7:
                if (enemy->x > 0 && enemy->y < ALTURA_TELA - 1) {
                    enemy->x--;
                    enemy->y++;
                } else {
                    enemy->direction = rand() % 8;
                }
                break;
        }

        if (enemy->x >= player->x && enemy->x < player->x + LARGURA_JOGADOR && enemy->y == player->y) {
            enemy->direction = (enemy->direction + 4) % 8;
            player->lives--;
            enemy->active = 0;
        }
    } else {
        if (*numInimigos < MAX_INIMIGOS_FACIL) {
            ativarInimigo(&inimigos[*numInimigos], rand() % LARGURA_TELA, rand() % ALTURA_TELA);
            (*numInimigos)++;
        }
    }
}
//void que define a movimentação aleatória do inimigo

void desenharJogo(Player *player, Enemy *inimigos, int numInimigos) {
    limpT();
	
	if(dificuldade == 3){
		larg_inimigo = 2;
	}else if(dificuldade == 1 || dificuldade == 2){
		larg_inimigo = 1;
	}
	
    for (int i = 0; i < ALTURA_TELA; i++) {
        for (int j = 0; j < LARGURA_TELA; j++) {
            if (i == player->y && j >= player->x && j < player->x + LARGURA_JOGADOR) {
                if(skin == 1){
                	printf("O");
				}else if(skin == 2){
					printf("}");
				}else if(skin == 3){
					printf("]");
				}else if(skin == 4){
					printf("@");
				}else if(skin == 5){
					printf("#");
				}else if(skin == 6){
					printf("$");
				}
            } else {
                int enemyIndex;
                for (enemyIndex = 0; enemyIndex < numInimigos; enemyIndex++) {
                    if (inimigos[enemyIndex].active &&
                        i == inimigos[enemyIndex].y &&
                        j >= inimigos[enemyIndex].x &&
                        j < inimigos[enemyIndex].x + larg_inimigo) {
                        printf("<");
                        break;
                    }
                }
                if (enemyIndex == numInimigos) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    printf("Vidas: %d   Pontos: %d\n", player->lives, player->score);

}
//void que apresenta o jogo (player, inimigos e GUI) e o atualiza conforme o tempo passa

void mostrarControles() {
    limpT();
    printf("\n");
    printf(" ___________________=== CONTROLES ===______________________\n");
    printf("|     Use 'A' e 'D' para mover o jogador para os lados.    |\n");
    printf("|Use 'W' e 'S' para mover o jogador para cima e para baixo.|\n");
    printf(" ----------------------------------------------------------\n");
    printf(" ____________________=== COMANDOS ===______________________\n");
    printf("|              Pressione 'T' para pausar o jogo.           |\n");
    printf(" ----------------------------------------------------------\n");
    printf(" ____________________=== OBJETIVO ===______________________\n");
    printf("|     Evite os inimigos e sobreviva o máximo possível!     |\n");
    printf(" ----------------------------------------------------------\n\n\n");

    pause();
    main();
}
//void da tela 'AJUDA' que mostra os controles, comandos e objetivo do jogo

void mostrarCreditos() {
    limpT();
    printf("\n");
    printf("                  === CRÉDITOS ===\n");
    printf(" ___________CRIADORES E DESENVOLVEDORES_________\n");
    printf("|               - Jhonatan Campioni             |\n");
	printf("|               - Ricardo Guilhen               |\n");
	printf("|               - Matheus Nanuncio              |\n");
	printf(" -----------------------------------------------\n");
	
    printf(" ____________________TESTERS____________________\n");
	printf("|               - Marcos Cardoso                |\n");
    printf("|               - Micaela Dorneles              |\n");
    printf("|               - Henrique Zolin                |\n");
	printf(" -----------------------------------------------\n\n");

    pause();
    main();
}
//void da tela 'CRÉDITOS' que apresenta os desenvolvedores e outros participantes do projeto

void historia(){
	limpT();
	printf("");
	printf("\n");
	printf(" __________________________________________________________HISTORIA____________________________________________________\n");
	printf("|                                                                                                                      |\n");
	printf("| Acordou, sem qualquer ânimo ou percepção mais aguçada, esta não mais se encontrava no seu corpo.                     |\n");
	printf("| Lindas, profundas e vastas imensidões de um oceano - ele não sabe qual.                                              |\n");
	printf("| Quanto mais tempo se passa, mais a esperança se esvai de seus pensamentos.                                           |\n");
	printf("| Nem por isso a desistência se torna uma opção.                                                                       |\n");
	printf("| Por vezes, barbatanas - ou seriam ilusôes da evidente penumbra da morte?                                             |\n");
    printf("| - se colocavam em evidência na já fraca visão do pobre (ou estúpido) homem.                                          |\n");
    printf("| Ele, fraco e desamparado mas um pouco conformado com a situação, tenta se levantar.                                  |\n");
    printf("| Sua falta de esperança quase supera sua vontade de viver.                                                            |\n");
    printf("| Quando ouve, baixo, a aproximação de um animal marinho.                                                              |\n");
    printf("| O quê poderia ser mais inconveniente e clichê que o ser mais temido dos mares?                                       |\n");
    printf("| Mais do que falta de sorte, o homem esbanjava seu azar. Não apenas um mas inúmeros tubarões se aproximavam.          |\n");
    printf("| Estes, agindo como se nunca tivessem visto carne mais saborosa.                                                      |\n");
    printf("| O homem, indeciso, pensa se o esforço poderia oferecer uma recompensa digna, como sua vida.                          |\n");
    printf("| Então, sem prévio aviso, uma voz, bela como os dias de outono, sutil como as sombras de uma árvore em um dia nublado |\n");
    printf("| e aconchegante como uma manta de pelos, surge e ecoa por todo e qualquer espaço que podia se encontrar.              |\n");
    printf("| Calmamente, a voz, agora perceptivelmente feminina, diz:                                                             |\n");
    printf("| 'Melhores destinos o aguardam, caro Capitão. Lute com todas as forças.'                                              |\n");
    printf("| Quase (ou de fato) milagrosamente, as forças crescem e tomam conta do corpo e alma do homem.                         |\n");
    printf("| Sem qualquer medo, como se tivesse passado por isso milhares de vezes e estivesse acostumado,                        |\n");
    printf("| o velho Capitão se levanta e olha com calma para o imenso oceano à sua volta.                                        |\n");
 	printf("| Agora, está determinado e foi destinado a lutar pela própria vida.                                                   |\n");
 	printf("|                                                                                                                      |\n");
 	printf(" ----------------------------------------------------------------------------------------------------------------------\n\n");
 	pause();
	main();
}
//void que apresenta a historia do jogo

void escolha_skin(){
	int op_skin = 0;
		
	limpT();
	printf("\n");
	printf("|-----------=== SKIN ===-----------|                          |Moeda = %d|\n\n", moeda);
	printf("-> Skin Ativada: %d <-\n\n", skin);
	printf(" ________SKINS DE GRAÇA_______\n");
	printf("|            1 - OOO          |\n");
	printf("|-----------------------------|\n");
	printf("|            2 - }}}          |\n");
	printf("|-----------------------------|\n");
	printf("|            3 - ]]]          |\n");
	printf(" -----------------------------\n\n");
	printf(" _______________SKINS PAGAS______________\n");
	printf("|           4 - @@@  ->  10 Moedas       |\n");
	printf("|----------------------------------------|\n");
	printf("|           5 - ###  ->  20 Moedas       |\n");
	printf("|----------------------------------------|\n");
	printf("|           6 - $$$  ->  40 Moedas       |\n");
	printf(" ----------------------------------------\n");
	if(licenca_skin4 == 1){
		printf("Skin 4 : Comprada\n");
	}else{
		printf("Skin 4 : Bloqueada\n");
	}
	if(licenca_skin5 == 1){
		printf("Skin 5 : Comprada\n");
	}else{
		printf("Skin 5 : Bloqueada\n");
	}
	if(licenca_skin6 == 1){
		printf("Skin 6 : Comprada\n");
	}else{
		printf("Skin 6 : Bloqueada\n");
	}
	printf("\n");
	printf("7 - Voltar\n");
	
	while(1){
		scanf("%d", &op_skin);
		if(op_skin >= 1 && op_skin <=3){
			limpT();
			skin = op_skin;
			printf("Skin Alterada...\n\n");
			pause();
			main();	
			break;
		}else if (op_skin == 7){
			main();
			break;
		}else if(op_skin >= 4 && op_skin <=6){
			if(op_skin == 4){
				if(licenca_skin4 == 0){
					if(moeda >= 10){
						moeda = moeda - 10;
						skin = op_skin;
						licenca_skin4 = 1;
						limpT();
						printf("Skin Alterada...\n\n");
						pause();
						main();
						break;
					}else{
						limpT();
						printf("Saldo Insuficiente...\n\n");
						pause();
						escolha_skin();
						break;
					}
				}else{
					skin = op_skin;
					limpT();
					printf("Skin Alterada...\n\n");
					pause();
					main();
				}
			}else if(op_skin == 5){
				if(licenca_skin5 == 0){
					if(moeda >= 20){
						moeda = moeda - 20;
						skin = op_skin;
						licenca_skin5 = 1;
						limpT();
						printf("Skin Alterada...\n\n");
						pause();
						main();
						break;
					}else{
						limpT();
						printf("Saldo Insuficiente...\n\n");
						pause();
						escolha_skin();
						break;
					}	
				}else{
					skin = op_skin;
					limpT();
					printf("Skin Alterada...\n\n");
					pause();
					main();
					break;	
				}
			}else if(op_skin == 6){
				if(licenca_skin6 == 0){
					if(moeda >= 40){
						moeda = moeda - 40;
						skin = op_skin;
						licenca_skin6 = 1;
						limpT();
						printf("Skin Alterada...\n\n");
						pause();
						main();
						break;
					}else{
						limpT();
						printf("Saldo Insuficiente...\n\n");
						pause();
						escolha_skin();
						break;
					}	
				}else{
					skin = op_skin;
					limpT();
					printf("Skin Alterada...\n\n");
					pause();
					main();	
					break;
				}
			}
		}else if(op_skin == 1718){
			licenca_skin4 = 1;
			licenca_skin5 = 1;
			licenca_skin6 = 1;
			limpT();
			printf("RIP... |BARBA NEGRA| 1680 - 1718\n\n");
			printf("Cheat Ativado...\n");
			printf("TODAS SKINS DESBLOQUEADAS!!\n\n");
			pause();
			main();
			break;
		}else{
			printf("Opção inválida. Escolha novamente:\n");
		}
	}	
}
//void que define o sistema de alteração de skins do player
	
void pauseMenu() {
    int escolha;

    printf("=== MENU DE PAUSA ===\n");
    printf("1 - Voltar ao Jogo\n");
    printf("2 - Voltar ao Menu\n");
    printf("3 - Sair do Jogo\n");
	
	while(1){
		scanf("%d", &escolha);
    	switch (escolha) {
        case 1:
            return;
        case 2:
            main();
        	break;
        case 3:
        	limpT();
            printf("Saindo do jogo...\n");
            exit(0);
        default:
            printf("Opção inválida. Escolha novamente:\n");
    	}	
	}
  
}
//void de apresentação do menu de pausa

int escolherDificuldade() {

    printf("Escolha a dificuldade:\n");
    printf("1 - Fácil\n");
    printf("2 - Médio\n");
    printf("3 - Difícil\n");
    printf("4 - Voltar\n");

   while(1){
	   	scanf("%d", &dificuldade);
	    if (dificuldade >= 1 && dificuldade <= 3) {
	        break;
	    } else if(dificuldade == 4){
	    	main();
	    	break;
		}else{
			printf("Opção inválida. Escolha novamente:\n");
		}
   } 
    
    return dificuldade;
}
//void que identifica a dificuldade escolhida pelo player

void game(){
	CONSOLE_CURSOR_INFO cursor = {1,FALSE};
  	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	cont_controle++;
	Player player;
    Enemy inimigos[MAX_INIMIGOS_DIFICIL];
    int numInimigos = 0;
    int maxInimigos = 0;
    int moeda_partida;
    int escolha;

    char entrada;

    srand((unsigned int)time(NULL));

	if(cont_controle == 1){
		int dificuldade = escolherDificuldade();
	}
    
    switch (dificuldade) {
        case 1:
            maxInimigos = MAX_INIMIGOS_FACIL;
            break;
        case 2:
            maxInimigos = MAX_INIMIGOS_MEDIO;
            break;
        case 3:
            maxInimigos = MAX_INIMIGOS_DIFICIL;
            break;
    }

    inicializarJogador(&player);

    clock_t tempoInicio = clock();
    clock_t tempo_Ultimo_Spawn_Inimigo = tempoInicio;
    int movimentosJogador = 0;
    while (player.lives > 0) {
        clock_t currentTime = clock();

        desenharJogo(&player, inimigos, numInimigos);

        if (_kbhit()) {
            char entrada = _getch();
            atualizarJogador(&player, entrada);

            if (entrada == 't') {
                pauseMenu();
            }
        }

        for (int i = 0; i < numInimigos; i++) {
            atualizarInimigo
			(&inimigos[i], &player, inimigos, &numInimigos);
        }

        player.score = (int)((currentTime - tempoInicio) / CLOCKS_PER_SEC);

        movimentosJogador++;
        if (movimentosJogador >= INTERVALO_SPAWN_INIMIGO) {
            movimentosJogador = 0;
            if (numInimigos < maxInimigos) {
                ativarInimigo(&inimigos[numInimigos], rand() % LARGURA_TELA, rand() % ALTURA_TELA);
                numInimigos++;
            }
        }

        Sleep(100);
    }
	
	if(dificuldade == 1 && player.score > rec_fac){
		rec_fac = player.score;
	} else if(dificuldade == 2 && player.score > rec_med){
		rec_med = player.score;
	} else if(dificuldade == 3 && player.score > rec_dif){
		rec_dif = player.score;
	}
	
	if(dificuldade == 1){
		moeda = (int)moeda + player.score/4;
			moeda_partida = player.score/4;
	}else if(dificuldade == 2){
		moeda = (int)moeda + player.score/2;
		moeda_partida = player.score/2;
	}else if(dificuldade == 3){
		moeda = moeda + player.score;
		moeda_partida = player.score;
	}

    limpT();
    printf("Game Over! Pontuação final: %d        Ganhou %d Moedas\n", player.score, moeda_partida);
	printf("\n\n");
	printf("1 - Tentar novamente\n");
    printf("2 - Voltar ao Menu\n");
    printf("3 - Sair do Jogo\n");
	
	while(1){
		scanf("%d", &escolha);
        switch (escolha) {
            case 1:
                game();
            case 2:
                main();
            	break;
            case 3:
            	limpT();
                printf("Saindo do jogo...\n");
                exit(0);
            default:
                printf("Opção inválida. Escolha novamente:\n");
        }	
	}	
}
//voide que define o tamanho do inimigo, as hitbox's (palyer e inimigo), a aplicação da
//biblioteca time.h, o máximo de inimigos e identifica os pontos, recordes do player e moedas
//do player


int main() {
	setlocale(LC_ALL, "Portuguese");
	system("title Sea’s Attack");
    limpT();
    cont_controle = 0;
    int escolha;
    printf("\n");
    printf(" _________=== MENU INICIAL ===_________          |Recordes|   |Moeda = %d|\n", moeda);
    printf("| 1 - Jogar                            |\n"); 
    printf("| 2 - Ajuda                            | Fácil   =   %d\n", rec_fac); 
    printf("| 3 - Créditos                         | Médio   =   %d\n", rec_med);
    printf("| 4 - Skins                            | Difícil =   %d\n", rec_dif);
    printf("| 5 - Historia                         |\n");
    printf("| 6 - Sair                             |\n");
    printf(" --------------------------------------\n");
    
    while(1){
	    scanf("%d", &escolha);
	    if (escolha >= 1 && escolha <= 6) {
	    	switch(escolha){
	    		case 1:
	    			game();
	    			break;
	    		case 2:
					mostrarControles();
					break;
				case 3:
					mostrarCreditos();
					break;
				case 4:
					escolha_skin();
					break;
				case 5:
					historia();
					break;			
				case 6:
					limpT();
	        		printf("Saindo do jogo...\n");
	        		exit(0);					
			}
	    } else {
	        printf("Opção inválida. Escolha novamente:\n");
	    }
 	}
	pause();
    return 0;
}
//função main, basicamente, o programa principal que chama os voids e apresenta o menu
