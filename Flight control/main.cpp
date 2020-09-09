#include <iostream>
#include <queue>
#include <time.h>
#include <cstdlib>
#include <string>
#include <cstdlib>
using namespace std;
struct samolot{
int pozycjax;
int pozycjay;
char nazwa;
int komenda;
int odleglosc;
int kierunek;
bool isdelay; //opoznienie wykonania rozkazu
};
void setx (samolot&);
void CreateHangar (queue <samolot>&);
void MoveToAir(vector <samolot>&,queue <samolot>&);
void ResetTab(char [10][64]);
void PrintTab(char [10][64]);
void GenTab(char [10][64],vector <samolot>&);
void DoCommand(vector <samolot>&,string);
void DoMove(vector <samolot>&);
bool checkPos(vector <samolot>&);
int main()
{
    char Plansza[10][64];
    queue <samolot> Hangar;
    CreateHangar(Hangar);
    vector <samolot> InAir;

    ResetTab(Plansza);
    MoveToAir(InAir,Hangar);
    GenTab(Plansza,InAir);
    PrintTab(Plansza);
    int delay=0;
    while(checkPos(InAir)) // petla z warunkiem czy samoloty sa w odpowiedniej odleglosci od siebie

    {
        while(1)
        {
            string command="";     // pobieramy komende od uzytkownika i przesylamy do funkcji odczytujacej ja
            cout<<endl<<"Wpisz komende:"<<endl;
            cin>>command;
            cout<<command[1];
            if(command[1]=='*')
            {
                break;
            }
            else  DoCommand(InAir,command); // ta funkcja wykonuje zmiany w samolotach ich kierunki itp.
        }
        DoMove(InAir); // zmiana pozycji samolotow
        ResetTab(Plansza); // czyszczenie tablicy
        if(delay==0)
        {
            delay=(rand()%3)+2;
            MoveToAir(InAir,Hangar); // dodanie samolotu po losowej ilosci tur z zakresu 2-5
        }
        GenTab(Plansza,InAir); // generujemy tablice z samolotami
        PrintTab(Plansza); // rysujemy tablice
        delay-=1; // zmniejszamy ilosc tur do nowego samolotu jak zejdzie do 0 to puszczamy samolot i na nowo losuyjemy
        cout<<endl;

    }
}
void setx (samolot &X)
{
    if(X.kierunek==0)
        {
            X.pozycjax=0;
        }
    else if (X.kierunek==1)
        {
            X.pozycjax=13;
        }
}
void GenTab(char Plansza [10][64],vector <samolot> &InAir)
{
    cout<<InAir.size()<<endl;
    int indeks=200;
    for(int i=0;i<InAir.size();i++) // przechodzimy po wszystkich samolotach w powietrzu czyli w tablicy InAir
      {
            if (InAir[i].pozycjax==0 && InAir[i].kierunek==0)      // ustawiamy je jesli sa na pozycjach startowych
                Plansza[InAir[i].pozycjay-1][0]=InAir[i].nazwa;
            else if (InAir[i].pozycjax==0 && InAir[i].kierunek==1)
                indeks=i;
            else if (InAir[i].pozycjax==13 && InAir[i].kierunek==1)
                Plansza[InAir[i].pozycjay-1][63]=InAir[i].nazwa;
            else if (InAir[i].pozycjax==13 && InAir[i].kierunek==0)
                indeks=i;

            else if (InAir[i].kierunek==0 && InAir[i].pozycjax!=0) // jesli leca w prawo rysujemy odpowiedni
            {
                Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5-3]='(';
                Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5-2]=InAir[i].nazwa;
                Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5-1]=InAir[i].odleglosc+48; // przesuniecie w tablicy asci (bo to char)
                Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5]=')';
                switch (InAir[i].komenda)
                {
                case 1:
                    Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5+1]='/';
                    break;
                case 0:
                    Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5+1]='=';
                    break;
                case -1:
                    Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5+1]= '_' ;
                    break;
                }
            }
            else if (InAir[i].kierunek=1 && InAir[i].pozycjax!=13) // jesli leca w lewo to wypisujemy odpowiednio je do tablicy
            {
                Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5+1]=')';
                Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5-1]=InAir[i].nazwa;
                Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5]=InAir[i].odleglosc+48;
                Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5-2]='(';
                switch (InAir[i].komenda)
                {
                case 1:
                    Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5-3]='/';
                    break;
                case 0:
                    Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5-3]='=';
                    break;
                case -1:
                    Plansza[InAir[i].pozycjay-1][InAir[i].pozycjax*5-3]= '_' ;
                    break;
                }
            }
        }
        if(indeks!=200)
            InAir.erase(InAir.begin()+indeks); // jesli samolot doleci do kranca no to usuwamy go z tablicy samolotow w powietrzu
}

void CreateHangar (queue <samolot> &Hangar)
{
srand( time( NULL ) );
    samolot SA=
    {
     0,rand()%10+1,'A',0,0,rand()%2,false
    };
    samolot SB=
    {
     0,rand()%10+1,'B',0,0,rand()%2,false
    };
    samolot SC=
    {
     0,rand()%10+1,'C',0,0,rand()%2,false
    };
    samolot SD=
    {
     0,rand()%10+1,'D',0,0,rand()%2,false
    };
    samolot SE=
    {
     0,rand()%10+1,'E',0,0,rand()%2,false
    };
    samolot SF=
    {
     0,rand()%10+1,'F',0,0,rand()%2,false
    };
    samolot SG=
    {
     0,rand()%10+1,'G',0,0,rand()%2,false
    };
    samolot SH=
    {
     0,rand()%10+1,'H',0,0,rand()%2,false
    };
    samolot SI=
    {
     0,rand()%10+1,'I',0,0,rand()%2,false
    };
    samolot SJ=
    {
     0,rand()%10+1,'J',0,0,rand()%2,false
    };
    setx(SA);
    setx(SB);
    setx(SC);
    setx(SD);
    setx(SE);
    setx(SF);
    setx(SG);
    setx(SH);
    setx(SI);
    setx(SJ);
    Hangar.push(SA); Hangar.push(SB); Hangar.push(SC); Hangar.push(SD); Hangar.push(SE); Hangar.push(SF); Hangar.push(SG); Hangar.push(SH); Hangar.push(SI); Hangar.push(SJ);
}
void MoveToAir(vector <samolot>& InAir,queue <samolot>& Hangar) // funkcja zabiera samolot z kolejki i dodaje do wektora po czym usuwa go z kolejki
{
    if(!Hangar.empty())
        {
            InAir.push_back(Hangar.front());
            Hangar.pop();
        }
}
void ResetTab (char Plansza [10][64] )
{
    for (int i=0;i<10;i++)
        for(int j=0;j<64;j++)
            if(j==1 || j==62)
                Plansza[i][j]='|';
            else
                Plansza[i][j]=' ';
}
void PrintTab (char Plansza [10][64])
{
        for (int i=0;i<10;i++)
          {
            for(int j=0;j<64;j++)
                {
                cout<<Plansza[i][j];
                }
            cout<<endl;
          }
}
void DoCommand(vector <samolot> &X,string command) // wykonuje komendy podane przez uzytkownika zmienia paramatry odpowiedniego samolotu
{
    int indeks=0;
    for (int i=0;i<X.size();i++)
        {
            if (X[i].nazwa==command[1]) // znajduje samolot o danej nazwie
            indeks=i;
        }
    switch (command[3]) // w zaleznosci od komendy zmienia komende i włacza opoznienie
    {
        case '/':
            X[indeks].komenda=1;
            X[indeks].isdelay=true;
            break;
        case '_':
            X[indeks].komenda=-1;
            X[indeks].isdelay=true;
            break;
        case 'c':
            X[indeks].komenda=0;
            X[indeks].isdelay=true;
            break;
    }
    X[indeks].odleglosc=command[5];
    X[indeks].odleglosc=X[indeks].odleglosc-48; //przesuniecie w tablicy asci zeby uzyskac liczbe
}
void DoMove(vector <samolot> &X) // w zaleznosci od kierunku i komendy przesywamy wspolrzedne samolotow
{
    for (int i=0;i<X.size();i++) //przechodzimy przez wszystkie samoloty
    {
        if (X[i].kierunek==0)
            X[i].pozycjax+=1;
        else
            X[i].pozycjax-=1;        // jesli nie ma juz opoznienia to wykona komende, a jesli jest opoznienie wlaczone to wtedy zmieni wartosc na false i w nastepnej turze komenda sie wykona
        if(X[i].isdelay==false && X[i].komenda==-1 && X[i].pozycjay<10 && X[i].odleglosc>0) // warunki sprawdzajce zeby nie wylecial poza plansze
            {
                X[i].pozycjay=X[i].pozycjay-X[i].komenda;
                X[i].odleglosc=X[i].odleglosc-1;
            }
        else if(X[i].isdelay==false && X[i].pozycjay>1 && X[i].komenda==1 && X[i].odleglosc>0) // warunki sprawdzajce zeby nie wylecial poza plansze
            {
                X[i].pozycjay=X[i].pozycjay-X[i].komenda;
                X[i].odleglosc=X[i].odleglosc-1;
            }
        else
            X[i].isdelay=false;

    }
}
bool checkPos (vector <samolot> &X) // sprawdzamy czy jakies dwa samoloty nie sa zablisko siebie
{
    for(int i=0;i<X.size();i++)
       {
        for(int j=0;j<X.size();j++) // podwojna petla sprawdzamy kazdy z kazdym
            {
                if(i!=j) // ale nie bierzmy pod uwage dwa razy tego samego samolotu
                {
                    if((X[i].pozycjax-X[j].pozycjax<2 && X[i].pozycjax-X[j].pozycjax>-2) && (X[i].pozycjay-X[j].pozycjay<2 && X[i].pozycjay-X[j].pozycjay>-2))
                    {
                        cout<<"SAMOLOTY S¥ ZBYT BLISKO SIEBIE NIE UDAlO CI SIE!"; // no jesli sa zablisko no to dajemy komunikat i zatrzymujemy program
                        return false;
                    }
                }
            }
       }
    return true;
}

