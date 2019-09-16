#include <curses.h>
#include <stdlib.h>

void init();
void init_colors();
void finish();

struct point {
    int x, y;
};

struct snake {
    struct point head;
    struct point* tail;
    size_t max_size_tail;
    int    size_tail;
};

struct snake* create_snake() {
    struct snake* m_snake = (struct snake*) malloc(sizeof(struct snake));
    m_snake->head.x = 10;
    m_snake->head.y = 10;
    m_snake->max_size_tail = 256;
    m_snake->size_tail = 0;

    m_snake->tail = (struct point*) malloc(sizeof(struct point) * m_snake->max_size_tail);
}

void shift_snake(struct snake* snake) {
    for(int i = snake->size_tail - 1; i >= 1; i--) {
        snake->tail[i] = snake->tail[i-1];
    }
    snake->tail[0] = snake->head;
}

void cleanup_snake(struct snake* snake) {
    free(snake->tail);
    free(snake);
}

int main() {
    init();

    struct snake* m_snake = create_snake();
    int c;

    struct point fruit_location;
    fruit_location.x = rand() % 30;
    fruit_location.y = rand() % 30;

    while(true) {
        int w, h;
        getmaxyx(stdscr, h, w);

        char head_char = '#';

        int temp = wgetch(stdscr);
        if (temp != ERR)
            c = temp;
        switch(c) {
            case KEY_LEFT:
            case 'h':
                m_snake->head.x--;
                head_char = '<';
            break;
            case KEY_DOWN:
            case 'j':
                m_snake->head.y++;
                head_char = 'v';
            break;
            case KEY_UP:
            case 'k':
                m_snake->head.y--;
                head_char = '^';
            break;
            case KEY_RIGHT:
            case 'l':
                m_snake->head.x++;
                head_char = '>';
            break;
        }
        clear();

        m_snake->head.x = (m_snake->head.x + w) % w;
        m_snake->head.y = (m_snake->head.y + h) % h;

        //eating fruit
        if(m_snake->head.x == fruit_location.x && m_snake->head.y == fruit_location.y) {
            m_snake->size_tail++;
            fruit_location.x = rand() % w;
            fruit_location.y = rand() % h;
        }

        //printing snake
        attrset(COLOR_PAIR(7));
        mvprintw(m_snake->head.y, m_snake->head.x, "%c", head_char);
        for(int i = 0; i < m_snake->size_tail; i++) {
            mvprintw(m_snake->tail[i].y, m_snake->tail[i].x, "*");
        }

        shift_snake(m_snake);
        
        //printing fruit
        attrset(COLOR_PAIR(4));
        mvprintw(fruit_location.y, fruit_location.x, "@");
        
        refresh();
    }
    
    getch();
        
    cleanup_snake(m_snake);
    m_snake = NULL;
    finish();
    return 0;
}

void init() {
    initscr();
    start_color();
    init_colors();
    curs_set(0);
    keypad(stdscr, 1);
    halfdelay(1);
    noecho();
}

void init_colors() { 
                                              //rgb
    init_pair(1, COLOR_BLUE,    COLOR_BLACK); //001
    init_pair(2, COLOR_GREEN,   COLOR_BLACK); //010
    init_pair(3, COLOR_CYAN,    COLOR_BLACK); //011
    init_pair(4, COLOR_RED,     COLOR_BLACK); //100
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK); //101
    init_pair(6, COLOR_YELLOW,  COLOR_BLACK); //110
    init_pair(7, COLOR_WHITE,   COLOR_BLACK); //111
    init_pair(8, COLOR_BLACK,   COLOR_BLACK); //(1)000
}

void finish() {
    endwin();
}
