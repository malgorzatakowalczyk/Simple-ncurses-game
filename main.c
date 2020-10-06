#include <stdio.h>
#include <ncurses.h>
#include <pthread.h>
#include "function.h"
#include <time.h>

int max_x, max_y, oneThird, twoThirds,half;
pthread_t th,th2;
pthread_mutex_t mutex;
int end=0;
int points=0;
int main()
{
	initscr();
    noecho();
    curs_set(0);
    start_color();
    srand(time(NULL));
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2,COLOR_YELLOW, COLOR_BLACK);
    pthread_mutex_init(&mutex,NULL);
    getmaxyx(stdscr,max_y, max_x);
    oneThird=max_y*0.333;
    twoThirds=max_y*0.333*2;
    
    int speed=50;
    int f=100;
    drawLine();
 
    pthread_create(&th, NULL, movePlayer, NULL); 
    while(1)
    {
    if(end==1) 
    {
             pthread_mutex_destroy(&mutex);
            return 0;
    }
    int los=rand()%10;
    if(points%10==0 && speed>=-50) speed-=25;
    pthread_create(&th2, NULL, moveEnemy, NULL);
    usleep(15000*(f+los+speed));
    }
    endwin();
	return 0;
}

void* movePlayer(void* arg)
{
    keypad(stdscr, TRUE);
    half=max_y*0.5;
    attron(COLOR_PAIR(2));
    mvprintw(half,10,">");
    attroff(COLOR_PAIR(2));
    refresh();
    while(1)
    {
    int sign=getch();
    if(sign==KEY_UP) 
    {
        mvprintw(half,10," ");
        if(half-1>oneThird)half--;
        attron(COLOR_PAIR(2));
        mvprintw(half,10,">");
        attroff(COLOR_PAIR(2));
        refresh();
    }
    if(sign==KEY_DOWN) 
    {
        mvprintw(half,10," ");
        if(half+1<twoThirds)half++;
        attron(COLOR_PAIR(2));
        mvprintw(half,10,">");
        attroff(COLOR_PAIR(2));
        refresh();
    }
    }
}


void* moveEnemy(void* arg)
{
    int position=rand()%(twoThirds-oneThird-1)+oneThird+1;
    int motion=max_x-1;
    mvprintw(0,0,"Punkty: %d",points);
    refresh();
    while(1)
    {
    if(motion==10 && position==half)
    {
        pthread_cancel(th);
        pthread_cancel(th2);
        end=1;
        return;
    }
    else if(motion==10 && position!=half)
    {
        mvprintw(0,0,"             ");
        refresh();
        points++;
        mvprintw(0,0,"Punkty: %d",points);
        refresh();
  
    }
    pthread_mutex_lock(&mutex);
    mvprintw(position,motion,"<");
    refresh();
    pthread_mutex_unlock(&mutex);
    usleep(60000);
    pthread_mutex_lock(&mutex);
    mvprintw(position,motion," ");
    refresh();
    pthread_mutex_unlock(&mutex);
    motion--;
    if(motion==0) break;
    }
}
void drawLine()
{
    attron(COLOR_PAIR(1));
    mvhline(oneThird,0, ACS_HLINE, max_x);
    mvhline(twoThirds,0, ACS_HLINE, max_x);
    attroff(COLOR_PAIR(1));
    refresh();
}
