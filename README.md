# jogo_da_velha

## Funções necessárias

void makeMove(ALLEGRO_BITMAP *_to_draw, float x, float y, SLOT _slot[], int index); **Executa a jogada do player.**

void initPlayer(PLAYER *_player, char name, int is_player_one, int bolinha); **Inicia um jogador.**

void newGame(PLAYERS *_players, char table[], SLOT _slots[]); **Inicia um novo jogo.**

void drawHUD(char* score_p1, char* score_p2, char *velha); **Desenha na tela os scores e velhas.**

void drawTable(int scr_height, int scr_width); **Desenha o tabuleiro na tela.**

void initSlots(SLOT _slots[]); **inicia as posições como vazias e também o tamanho de cada uma.**

void initResourcesPath(); **Seta a pasta de recursos.**

int validateMove(SLOT _slots[], int x, int y); **Valida o movimento, retorna a posição caso esteja vazio, ou -1 caso não esteja.**

int validateWinner(char *table); **Valida o vencedor, retorna verdadeiro ou falso (1 ou 0).**

int isFullTable(SLOT _slots[]); **Valida se o tabuleiro esta cheio, caso esteja é velha.**

PLAYER getCurrentPlayer(int count, PLAYERS *_players);**Pega o player atual com base no contador, retorna um jogador (X ou O).**


**A pasta de recursos deve estar no mesmo diretório onde está o executavel do jogo*
```
Exemplo: \bin\debug
				|__\resources
				|__\allegro-{version-allegro}-monolith-mt.dll
				|__\jogo_da_velha.exe
```