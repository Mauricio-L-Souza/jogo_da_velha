#include <string.h>
#include <allegro5/path.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#define SCR_HEIGHT  480
#define SCR_WIDTH  320

typedef struct{
    int x, y;
}COORDINATE;

typedef struct{
    int index, is_empty_slot;
    COORDINATE position;
    COORDINATE range;
}SLOT;

typedef struct{
    char name;
    int is_player_one;
    ALLEGRO_BITMAP *image;
}PLAYER;

typedef struct{
    PLAYER x, o;
}PLAYERS;

typedef struct{
    char value[3];
}SCORE;

//Functions
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
PLAYER getCurrentPlayer(int count, PLAYERS *_players);
//END FUNCTIONS

//helpers
void error_msg(char *text){
	al_show_native_message_box(NULL,"ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

bool displayGameOver(const char *text){
	return al_show_native_message_box(NULL,"FIM DE JOGO",
            "Deseja jogar novamente?",
            text,NULL,ALLEGRO_MESSAGEBOX_YES_NO);
}

void initALL(){
    al_init();
    al_init_ttf_addon();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    if(!al_init_primitives_addon()){
        error_msg("Não consequi abrir os gráficos!");
    }
    al_install_mouse();
    if (!al_is_mouse_installed()){
        error_msg("mouse não foi instalado!");
    }

    initResourcesPath();
}

SCORE intToString(int value){
    char number = value + '0';

    SCORE _score;
    _score.value[0] = number;
    _score.value[1] = '\0';

    return _score;
}

const char* generateWinnerMsg(char name, int empate){
    static char msg[20];

    if (!empate){
        strcpy(msg,"O jogador   venceu!");
        msg[10] = name;
    }
    else strcpy(msg,"O jogo empatou!");

    return msg;
}
//end helpers

int main()
{
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    ALLEGRO_DISPLAY *window = NULL;
    PLAYER _player;
    PLAYERS _players;

    SLOT _slots[9];

    int mouse_pos_x = 0, mouse_pos_y = 0, count = 0, score_p1 = 0, score_p2 = 0, velhas = 0;
    int bestPlaceToDraw[9][2] = {{30, 212},
                                  {136,212},
                                  {240,212},
                                  {30,318},
                                  {136,318},
                                  {240,318},
                                  {30,424},
                                  {136,424},
                                  {240,424}
                                };
    char table[10];

    initALL();

    window = al_create_display(SCR_WIDTH, SCR_HEIGHT);

    newGame(&_players, table, _slots);
    drawHUD("0", "0", "0");

    queue = al_create_event_queue();// cria a fila de eventos
    if(!queue) {
        al_destroy_display(window);//Destroi a janela
        return -1;
    }

    al_flip_display();// atualiza o display
    al_register_event_source(queue, al_get_display_event_source(window));//faz com que escute um evento especifico
    al_register_event_source(queue, al_get_mouse_event_source());

    while(1){
        al_flip_display();

        ALLEGRO_EVENT event;

        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            break;
        }

        if(!validateWinner(table) && !isFullTable(_slots)){
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                mouse_pos_x = event.mouse.x;
                mouse_pos_y = event.mouse.y;
                int index = validateMove(_slots, mouse_pos_x, mouse_pos_y);

                if (index != -1){
                    _player = getCurrentPlayer(count, &_players);
                    table[index] = _player.name;

                    makeMove(_player.image, bestPlaceToDraw[index][0], bestPlaceToDraw[index][1], _slots, index);
                    al_flip_display();// atualiza o display
                    count++;

                    continue;
                }
                index = NULL;
            }
        }else{
            if(isFullTable(_slots) && !validateWinner(table)){
                if(displayGameOver(generateWinnerMsg(_player.name, 1))){
                    al_flush_event_queue(queue);
                    newGame(&_players, table, _slots);
                    velhas++;
                    drawHUD(intToString(score_p1).value, intToString(score_p2).value, intToString(velhas).value);
                    count = 0;
                }else{
                    al_destroy_display(window);//Destroi a janela
                    return 0;
                }
            }else
                if(displayGameOver(generateWinnerMsg(_player.name, 0))){
                    al_flush_event_queue(queue);
                    newGame(&_players, table, _slots);

                    if (_player.is_player_one){
                        score_p1++;
                    }else{
                        score_p2++;
                    }

                    drawHUD(intToString(score_p1).value, intToString(score_p2).value, intToString(velhas).value);
                    count = 0;
            }else{
                al_destroy_display(window);//Destroi a janela
                return 0;
            }
        }
    }
}

void initResourcesPath()
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);//pega o diretorio principal
    al_append_path_component(path, "resources"); //adiciona e linka a pasta resources ao diretorio principal
    al_change_directory(al_path_cstr(path, '/'));  // change the working directory
    al_destroy_path(path);
}

void initSlots(SLOT _slots[])
{
    int i, j, range_min_x = 106, range_min_y = 260, aux;
    for (i = 0; i < 9; i++)
    {
        aux = i + 3;
        for (j = i; j < aux; j++){
            _slots[j].index = j;
            _slots[j].is_empty_slot = 1;
            _slots[j].position.x = (range_min_x - 106) + 1;
            _slots[j].position.y = (range_min_y - 106) + 1;
            _slots[j].range.x = range_min_x - 1;
            _slots[j].range.y = range_min_y - 1;
            range_min_x += 106;
        }
        range_min_x = 106;
        range_min_y += 106;
        i += 2;
    }
}

void initPlayer(PLAYER *_player, char name, int is_player_one, int bolinha)
{
    _player->is_player_one = is_player_one;

    if (bolinha){
        _player->image = al_load_bitmap("bolinha.png");
        _player->name = 'O';
    }
    else{
        _player->image = al_load_bitmap("x.png");
        _player->name = 'X';
    }
}

void drawTable(int scr_height, int scr_width)
{
     float value_line_pos_y = scr_height - scr_width;
     float value_line_pos_x = scr_width/3;
    // vertical lines
    al_draw_line(value_line_pos_x, value_line_pos_y, value_line_pos_x, scr_height, al_map_rgb(255, 255, 255), 1);
    al_draw_line(value_line_pos_x * 2, value_line_pos_y, value_line_pos_x * 2, scr_height, al_map_rgb(255, 255, 255), 1);

    // horizontal lines
    al_draw_line(0, value_line_pos_y + 106, scr_width, value_line_pos_y + 106, al_map_rgb(255, 255, 255), 2);
    al_draw_line(0, value_line_pos_y + (106*2), scr_width, value_line_pos_y + (106*2), al_map_rgb(255, 255, 255), 2);
}

void drawHUD(char *score_p1, char *score_p2, char *velha)
{
    ALLEGRO_FONT *font = al_load_ttf_font("arial.ttf", 20, 0);
    al_draw_text(font, al_map_rgb(245, 245, 67), 20, 5, 0, "Player 1");
    al_draw_text(font, al_map_rgb(245, 245, 67), 20, 30, 0, "SCORE");
    al_draw_text(font, al_map_rgb(245, 245, 67), 50, 55, 0, score_p1);
    al_draw_text(font, al_map_rgb(245, 245, 67), 225, 5, 0, "Player 2");
    al_draw_text(font, al_map_rgb(245, 245, 67), 225, 30, 0, "SCORE");
    al_draw_text(font, al_map_rgb(245, 245, 67), 255, 55, 0, score_p2);
    al_draw_text(font, al_map_rgb(245, 245, 67), 125, 55, 0, "VELHAS");
    al_draw_text(font, al_map_rgb(245, 245, 67), 155, 80, 0, velha);
    al_draw_line(0, 155, 320, 155, al_map_rgb(0, 4, 255), 2);
}

int validateMove(SLOT _slots[], int x, int y)
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if((x >= _slots[i].position.x+1 && x <= _slots[i].range.x-1)
           && (y >= _slots[i].position.y+1 && y <= _slots[i].range.y-1)
           && _slots[i].is_empty_slot)
        {
            return _slots[i].index;
        }
    }

    return -1;
}

int isFullTable(SLOT _slots[])
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if(_slots[i].is_empty_slot)
        {
            return 0;
        }
    }
    return 1;
}

int validateWinner(char *table)
{
    if((table[0]=='X' && table[1]=='X' && table[2]=='X')||(table[0]=='O' && table[1]=='O' && table[2]=='O'))
        return 1;
    else
        if((table[3]=='X' && table[4]=='X' && table[5]=='X')||(table[3]=='O' && table[4]=='O' && table[5]=='O'))
            return 1;
    else
        if((table[6]=='X' && table[7]=='X' && table[8]=='X')||(table[6]=='O' && table[7]=='O' && table[8]=='O'))
            return 1;
    else
        if((table[0]=='X' && table[3]=='X' && table[6]=='X')||(table[0]=='O' && table[3]=='O' && table[6]=='O'))
            return 1;
    else
        if((table[1]=='X' && table[4]=='X' && table[7]=='X')||(table[1]=='O' && table[4]=='O' && table[7]=='O'))
            return 1;
    else
        if((table[2]=='X' && table[5]=='X' && table[8]=='X')||(table[2]=='O' && table[5]=='O' && table[8]=='O'))
            return 1;
    else
        if((table[0]=='X' && table[4]=='X' && table[8]=='X')||(table[0]=='O' && table[4]=='O' && table[8]=='O'))
            return 1;
    else
        if((table[2]=='X' && table[4]=='X' && table[6]=='X')||(table[2]=='O' && table[4]=='O' && table[6]=='O'))
            return 1;

    return 0;
}

void newGame(PLAYERS *_players, char table[], SLOT _slots[])
{
    strcpy(table, "111111111");
    al_clear_to_color(al_map_rgb(0, 0, 0));
    drawTable(SCR_HEIGHT, SCR_WIDTH);
    initSlots(_slots);

    initPlayer(&_players->x, 'X', 1, 0);
    initPlayer(&_players->o, 'O', 0, 1);
}

void makeMove(ALLEGRO_BITMAP *_to_draw, float x, float y, SLOT _slots[], int index)
{
    al_draw_bitmap(_to_draw, x, y, 0);
    _slots[index].is_empty_slot = 0;
}

PLAYER getCurrentPlayer(int count, PLAYERS *_players)
{
    PLAYER _player;

    if ((count % 2) == 0){
        if (_players->x.is_player_one) _player = _players->x;
        else _player = _players->o;
    }else{
        if (!_players->x.is_player_one) _player = _players->x;
        else _player = _players->o;
    }

    return _player;
}
