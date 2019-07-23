#include<ncurses.h>

void rldnum(int y, int x)       //czysci miejsce na wpisanie liczby zaczynajace sie w (x,y)
{
	move(y,x);
	int a;
	attron(COLOR_PAIR(2));
	for(a=0; a<70; a++)
		printw(" ");
    attron(COLOR_PAIR(1));
        mvprintw(y,x-1," ");
	return;
}
void rldscr()                   //czysci terminal
{
        int a,b,x,y;
	getmaxyx(stdscr,y,x);
    	attron(COLOR_PAIR(1));
    	for(a=0; a<y; a++)
    		for(b=0; b<x; b++)
			printw(" ");
        attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	mvprintw(4,6,"   ");
	attroff(COLOR_PAIR(2));
	rldnum(2,4);
	rldnum(6,4);
    refresh();
	return;
}
void err(int a)                 //wypisuje odpowiedni blad
{
    attron(COLOR_PAIR(1));
    mvprintw(13,20,"                                      ");
    attron(COLOR_PAIR(2));
    move(12,30);
    switch(a)
    {
        case 1:
            printw(" Blad wczytywania! ");
            break;
        case 2:
            printw(" Niewlasciwy symbol! ");
            break;
        case 3:
            mvprintw(12,26,"Wynik przekracza zakres liczenia! ");
            break;
        default:
            printw("BLAD!");
            break;
    }
	mvprintw(13,26,"-NACISNIJ DOWOLNY KLAWISZ-");
	attroff(COLOR_PAIR(2));
	getch();
	attron(COLOR_PAIR(1));
	mvprintw(12,26,"                                                            ");
	mvprintw(13,25,"                             ");
	attroff(COLOR_PAIR(1));
	refresh();
	return;
}
int wc(int pl[],int y,int x)    //wczytuje liczbe
{
    pl[70]=1;
    attron(COLOR_PAIR(2));
	mvprintw(12,31,"Podaj liczbe");
    attron(COLOR_PAIR(1));
    printw("         ");
    attroff(COLOR_PAIR(1));
	int a,b,c,d;
	b=0;
	attron(COLOR_PAIR(2));
	mvprintw(13,20,"Aby zmienic jej znak, nacisnij minus");
	move(y,x);
	while((a=getch())!='\n')
	{
	    if(a=='-')
	    {
	        pl[70]*=-1;
	        getyx(stdscr,c,d);
	        attron(COLOR_PAIR(1));
	        if(pl[70]<0)
                mvprintw(y,x-1,"-");
            else
                mvprintw(y,x-1," ");
            move(c,d);
            attron(COLOR_PAIR(2));
	    }
	    else
	    {
	        if(!(a<='9'&&a>='0')||b>69)
            {
                rldnum(y,x);
                err(1);
                return 0;
            }
            pl[b]=a-'0';
            printw("%d",pl[b]);
            b++;
	    }
	}
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));
    mvprintw(12,20,"                                      ");
    mvprintw(13,20,"                                              ");
    attroff(COLOR_PAIR(1));
    refresh();
	return b;
}
char getop()                    //wczytuje znak operacji
{
	char c;
	while(1)
	{
	    attron(COLOR_PAIR(2));
	    mvprintw(12,31,"Podaj znak dzialania");
	    attroff(COLOR_PAIR(2));
		c=getch();
		if(c=='+'||c=='-'||c=='*'||c=='/'||c=='!')
        {
            attron(COLOR_PAIR(2));
            mvprintw(4,6,"%c",c);
            break;
        }
        else
        {
            err(2);
        }
        attron(COLOR_PAIR(1));
        mvprintw(12,30,"                      ");
        attroff(COLOR_PAIR(1));
        refresh();
	}
	attroff(COLOR_PAIR(2));
	return c;
}
void debug(int pl[], int dl[], int d1, int d2)  //funkcja do debugowania dzialania funkcji operacji
{
    int a,y;
    getmaxyx(stdscr,y,a);
    rldnum(y-2,1);
    rldnum(y-1,1);
    move(y-2,0);
    printw("%d ",pl[70]);
    for(a=0; a<d1; a++)
        printw("%d ",pl[a]);
    printw("%d",d1);
    move(y-1,0);
    printw("%d ",dl[70]);
    for(a=0; a<d2; a++)
        printw("%d ",dl[a]);
    printw("%d",d2);
    getch();
}
void writeout(int pl[], int d1)    //wypisanie wyniku na miejscu pierwszej liczby
{
        int a;
        rldnum(2,4); move(2,4);
        attron(COLOR_PAIR(2));
        for(a=0; a<d1; a++)
            printw("%d",pl[a]);
        attroff(COLOR_PAIR(2));
        move(2,3);
        attron(COLOR_PAIR(1));
        if(pl[70]<0)
            printw("-");
        else
            printw(" ");
        attroff(COLOR_PAIR(1));
        refresh();
}
void zamien(int pl[], int dl[], int *d1, int *d2)   //zamienia liczby przy odejmowaniu
{
    int a;
    a=*d1; *d1=*d2; *d2=a;
    int pom[71];
    for(a=0; a<70; a++)
    {
        pom[a]=pl[a];
        pl[a]=dl[a];
        dl[a]=pom[a];
    }
    pl[70]=-dl[70];
}
bool plus(int pl[], int dl[], int *d1, int d2)  //dodaje pisemnie 2 liczby
{
    int a;
    while(*d1>d2)              //sprowadzenie liczb do takiej samej d³ugoœci (dope³nienie krótszej zerami)
    {
        for(a=d2; a>0; a--)
            dl[a]=dl[a-1];
        d2++;
        dl[0]=0;
    }
    while(*d1<d2)
    {
        for(a=*d1; a>0; a--)
            pl[a]=pl[a-1];
        *d1+=1;
        pl[0]=0;
    }
    int dz=0;
    for(a=*d1-1; a>=0; a--)
    {
        pl[a]+=dl[a]+dz;
        dz=(pl[a]-pl[a]%10)/10;
        pl[a]=pl[a]%10;
    }
    if(dz)
    {
        if(*d1==70)
            return 0;
        for(a=*d1; a>0; a--)
            pl[a]=pl[a-1];
        pl[0]=dz;
        *d1+=1;
    }
    return 1;
}
bool minus(int pl[], int dl[], int *d1, int d2) //odejmuje pisemnie 2 liczby
{
    int a; bool b=0;
    if(*d1<d2) zamien(pl,dl,d1,&d2);
    if(*d1==d2)
    {
        for(a=0; a<d2; a++)
        {
            if(pl[a]!=dl[a])
            {
                b=1;
                if(pl[a]>dl[a])
                    break;
                else
                {
                    zamien(pl,dl,d1,&d2);
                    break;
                }
            }
        }
        if(b==0)
        {
            pl[0]=0; pl[70]=1; *d1=1;
            return 1;
        }
    }
    while(*d1>d2)              //sprowadzenie liczb do takiej samej d³ugoœci (dope³nienie krótszej zerami)
    {
        for(a=d2; a>0; a--)
            dl[a]=dl[a-1];
        d2++;
        dl[0]=0;
    }
    for(a=*d1-1; a>=0; a--)
    {
        pl[a]-=dl[a];
        while(pl[a]<0)
        {
            pl[a]+=10;
            pl[a-1]-=1;
        }
    }
    while(pl[0]==0)
    {
        for(a=0; a<*d1; a++)
        {
            pl[a]=pl[a+1];
        }
        (*d1)--;
    }
    return 1;
}
bool razy(int pl[], int dl[], int *d1, int d2)  //mnozy pisemnie 2 liczby uzywajac algorytmu dodawania
{
    int a,b,dz=0,ol[71],tl[71],od=*d1,td;      //dz-dziesiatki, ol-oryginalna (1-sza) liczba, tl-tymczasowa liczba
    for(a=0; a<71; a++)
        ol[a]=pl[a];
    pl[70]*=dl[70];
    *d1=1; pl[0]=0;
    for(a=d2-1; a>=0; a--)
    {
        for(b=od-1; b>=0; b--)
        {
            tl[b]=(ol[b]*dl[a])+dz;
            dz=(tl[b]-tl[b]%10)/10;
            tl[b]%=10;
        }   td=od;
        while(dz)
        {
            if(td==70)
                return 0;
            for(b=td; b>0; b--)
                tl[b]=tl[b-1];
            tl[0]=dz%10;
            td++;
            dz-=dz%10; dz/=10;
        }
        for(b=a; b<d2-1; b++)
        {
            if(td==70)
                return 0;
            tl[td]=0;
            td++;
        }
        if(!plus(pl,tl,d1,td))
            return 0;
    }
    return 1;
}
bool silnia(int pl[], int *d)       //liczy silnie uzywajac dodawania i mnozenia pisemnego
{
   if(*d>2)
        return 0;
   if(pl[70]<0)
        return 0;
   int a=pl[0];
   if(*d==2) a=10*a+pl[1];
   if(a>53)
        return 0;
    if(a==0)
    {
        pl[0]=1; *d=1;
        writeout(pl,*d);
        return 1;
    }
    int dl[71],d2=1,jeden[71];
    dl[0]=2; pl[0]=1;
    *d=1; a--;
    while(a--)
    {
        jeden[0]=1;
        razy(pl,dl,d,d2);
        plus(dl,jeden,&d2,1);
    }
    pl[70]=1;
    return 1;
}
bool cialo()                //glowna funkcja programu
{
    int pl[71],dl[71];		            //pierwsza i druga liczba
	int d1,d2;			        //dlugosci
	char op; bool b;            //znak operacji i zmienna pomocnicza
	while(!(d1=wc(pl,2,4)));	//wczytywanie pierwszej liczby
	while(1)
	{
	    op=getop();			    //wczytywanie znaku operacji
        if(op=='!')
            b=silnia(pl,&d1);	//silnia
        else
        {
            while(!(d2=wc(dl,6,4)));    //wczytywanie drugiej liczby
            if((pl[70]<0||dl[70]<0)&&!(pl[70]<0&&dl[70]<0)) //sprawdzanie, czy liczby ujemne nie zmieniają
            {                                               //dodawania na odejmowanie lub odwrotnie
                switch(op)
                {
                    case '+':
                        op='-';
                        if(pl[70]==-1) zamien(pl,dl,&d1,&d2);
                        pl[70]=dl[70]=1;
                        break;
                    case '-':
                        op='+';
                        break;
                }
            }
            switch(op)                      //wykonanie właściwego działania
            {
                case '+':
                    b=plus(pl,dl,&d1,d2);
                    break;
                case '-':
                    b=minus(pl,dl,&d1,d2);
                    break;
                case '*':
                    b=razy(pl,dl,&d1,d2);
                    break;
            }
        }
        if(b)                       //jeśli działanie wykonało się bez błędu, wypisanie wyniku
            writeout(pl,d1);
        rldnum(6,4);
        for(d2=0; d2<70; d2++)  //zerowanie drugiej liczby
            dl[d2]=0;
        if(b)                   //jesli wszystko sie poprawnie zrobilo
        {
            attron(COLOR_PAIR(2));
            mvprintw(4,6,"  ");
            mvprintw(12,26,"ABY KONTYNUOWAC, NACISNIJ ENTER");
            mvprintw(13,22,"ABY ZACZAC Z NOWA LICZBA, NACISNIJ KLAWISZ n");
            mvprintw(14,22,"ABY ZAKONCZYC, NACISNIJ DOWOLNY INNY KLAWISZ");
            attroff(COLOR_PAIR(2));
            op=getch();
            attron(COLOR_PAIR(1));
            mvprintw(12,26,"                                    ");
            mvprintw(13,22,"                                                ");
            mvprintw(14,22,"                                                ");
            refresh();
            attroff(COLOR_PAIR(1));
            if(op=='n') return 1;
            if(op!='\n') return 0;
        }
        else                    //jesli liczba przekroczyla maksymalna dlugosc
        {
            err(3);
            rldnum(2,4);
            attron(COLOR_PAIR(2));
            mvprintw(4,6,"  ");
            mvprintw(12,26,"ABY ZACZAC OD NOWA, NACISNIJ ENTER");
            mvprintw(14,22,"ABY ZAKONCZYC, NACISNIJ DOWOLNY INNY KLAWISZ");
            attroff(COLOR_PAIR(2));
            op=getch();
            attron(COLOR_PAIR(1));
            mvprintw(12,26,"                                      ");
            mvprintw(14,22,"                                                ");
            attroff(COLOR_PAIR(1));
            if(op!='\n') return 0;
            else return 1;
        }
	}
}
int main()
{
        initscr();
        start_color();
        noecho();
    	init_pair(1,COLOR_WHITE,COLOR_BLUE);
    	init_pair(2,COLOR_BLACK,COLOR_WHITE); //wstepne przygotowania
    	rldscr();
    	while(cialo())                      //glowna funkcja programu
    	{
    	    rldscr();
    	}
    	endwin();
    	return 0;
}
