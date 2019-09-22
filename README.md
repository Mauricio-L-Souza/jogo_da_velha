# jogo_da_velha

#functions

void makeMove(ALLEGRO_BITMAP *_to_draw, float x, float y, SLOT _slot[], int index);
void initPlayer(PLAYER *_player, char name, int is_player_one, int bolinha);
void newGame(PLAYERS *_players, char table[], SLOT _slots[]);
void drawHUD(char* score_p1, char* score_p2, char *velha);
void drawTable(int scr_height, int scr_width);
void initSlots(SLOT _slots[]);
void initResourcesPath();
int validateMove(SLOT _slots[], int x, int y);
int validateWinner(char *table);
int isFullTable(SLOT _slots[]);