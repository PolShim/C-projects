#include<iostream>
#include<iomanip>
#include<vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <stack>
#include <iomanip>
#include <windows.h>

using namespace std;

struct karta
{
    string numer;
    int kolor;  // 1- czerwony  ,  2-czarny
    bool zakryta;
    int wartosc;
};

void init(vector <karta> &,vector< vector<karta> >& VecKolumn, queue <karta>& Dobierane, vector <karta>& VecDobierane);
void print(karta);
void rysowanie(vector<vector <karta> >&, vector<karta> &, vector< stack<karta> > &);
void Rys(stack<karta> &);
void RysStos(stack<karta> &);
void wypiszLinie(vector <karta>,int);
void odkryj(vector <vector <karta> >& VecKolumn);
void przelozenie (queue <karta>& Dobierane, vector <karta>&);
int szukaj(vector<karta> &, string);
bool dostep(karta, string);
int ruch(vector< vector<karta> > &, vector<karta> &,queue <karta>&, vector< stack<karta> > &);
void PrzenoszenieDoKolumny(vector< vector<karta> > &, vector<karta> &, int , int, int);
void PrzenoszenieDoStosu(vector< vector<karta> > &, vector<karta> &, vector< stack<karta> > &, int, int, int);
void KartaDoKolumny(vector< vector<karta> > &, vector<karta> &, int , int);
void KartaZStosu(vector< stack<karta> > &, vector<karta> &, int , int);
bool SprawdzKolumne(vector<karta> &, vector<karta> &, int);
bool SprawdzWartoscKolumna(vector<karta> &VecDo, vector<karta> &VecZ, int);
bool SprawdzKolorKolumna(vector<karta> &VecDo, vector<karta> &VecZ, int);
bool SprawdzStos(stack<karta> &, vector<karta> &, int);
bool SprawdzWartoscStos(stack<karta> &, vector<karta> &, int);
bool SprawdzFigureStos(stack<karta> &, vector<karta> &, int);
void Menu (vector <vector <karta> >&,vector <karta>&, queue <karta>& , vector < stack<karta> >&);

int main()
{
    vector< vector<karta> >VecKolumn;
    vector< stack<karta> >Stos(4);
    vector<karta>talia(52);
    queue<karta>Dobierane;
    vector<karta>VecDobierane;
    init(talia,VecKolumn,Dobierane,VecDobierane);
    odkryj(VecKolumn);
    while(1)
    {
        Menu(VecKolumn,VecDobierane,Dobierane,Stos);
    }
    return 0;
}

void init(vector <karta> &talia,vector< vector<karta> >& VecKolumn, queue <karta>& Dobierane, vector <karta>& VecDobierane)
{
    srand(time(NULL));
    for(int i=0; i<52; i++)
    {
    int n=(i+1)%13;
    if(n==1) {talia.at(i).numer="A"; talia.at(i).wartosc=1;}
    else if(n==2) {talia.at(i).numer="2"; talia.at(i).wartosc=2;}
    else if(n==3) {talia.at(i).numer="3"; talia.at(i).wartosc=3;}
    else if(n==4) {talia.at(i).numer="4"; talia.at(i).wartosc=4;}
    else if(n==5) {talia.at(i).numer="5"; talia.at(i).wartosc=5;}
    else if(n==6) {talia.at(i).numer="6"; talia.at(i).wartosc=6;}
    else if(n==7) {talia.at(i).numer="7"; talia.at(i).wartosc=7;}
    else if(n==8) {talia.at(i).numer="8"; talia.at(i).wartosc=8;}
    else if(n==9) {talia.at(i).numer="9"; talia.at(i).wartosc=9;}
    else if(n==10) {talia.at(i).numer="10"; talia.at(i).wartosc=10;}
    else if(n==11) {talia.at(i).numer="J"; talia.at(i).wartosc=11;}
    else if(n==12) {talia.at(i).numer="Q"; talia.at(i).wartosc=12;}
    else if(n==0) {talia.at(i).numer="K"; talia.at(i).wartosc=13;}
    }

    for(int i=0; i<13; i++)
    {
        talia.at(i).numer+="h";
        talia.at(i).kolor=1;
        talia.at(i).zakryta=true;
        talia.at(i+13).numer+="s";
        talia.at(i+13).kolor=2;
        talia.at(i+13).zakryta=true;
        talia.at(i+26).numer+="d";
        talia.at(i+26).kolor=1;
        talia.at(i+26).zakryta=true;
        talia.at(i+39).numer+="c";
        talia.at(i+39).kolor=2;
        talia.at(i+39).zakryta=true;
    }
    for(int i=0; i<rand()%100; i++)
        random_shuffle(talia.begin(), talia.end());

    for(int i=0; i<24; i++)
    {
        Dobierane.push(talia.back());
        Dobierane.back().zakryta=0;
        talia.pop_back();
    }

    for(int i=0; i<7; i++)
    {
        vector<karta>Vec;
        for(int j=0; j<1+i; j++)
        {
            Vec.push_back(talia.back());
            talia.pop_back();
        }
        VecKolumn.push_back(Vec);
    }

}

void print(karta p)
{
    if(p.zakryta)
        cout<<setw(4)<<"===";
    else
        cout<<setw(4) << p.numer;
}

void rysowanie(vector <vector <karta> >& Vec, vector<karta> &VecDobierane, vector< stack<karta> > &Stos)
{
    cout << "    0                       8       9      10      11" << endl;
    for(int i=0; i<57; i++)
        cout << "-";
    cout << endl << endl;
    for(int i=0; i<57; i++)
    {
        if(i==2)
        {
            if(VecDobierane.empty()==0)
                print(VecDobierane.at(0));
            else cout << " ===";
            cout << "       ";
        }
        else if(i==17)
            {
                Rys(Stos.at(0));
            }
        else if(i==23)
            {
                Rys(Stos.at(1));
            }
        else if(i==29)
            {
                Rys(Stos.at(2));
            }
        else if(i==35)
            {
                Rys(Stos.at(3));
            }
        else cout << " ";
    }
    cout << endl << endl;
    for(int i=0; i<57; i++)
        cout << "-";
    int w=0;
    for(int i=0; i<7 ; i++)
       {
            if(Vec.at(i).size() > w)
            w=Vec.at(i).size();
        }
    cout<<endl;
    for(int a=0; a<w; a++)
        {
            for (int i=0 ; i<7 ;i++)
                {
                    wypiszLinie(Vec.at(i),a);
                }
            cout << "|" <<endl;
        }
    for(int i=0; i<57; i++)
        cout << "-";
    cout << endl << "    1       2       3       4       5       6       7" << endl;
}

void Rys(stack<karta> &stos)
{
    if(stos.empty()) cout << setw(3) << "===";
    else RysStos(stos);
}

void RysStos(stack<karta> &stos)
{
    karta K;
    K=stos.top();
    cout << setw(3) << K.numer;
}

void wypiszLinie(vector <karta> Vec,int a)
{
    if(a<Vec.size())
        {
            cout<<"| ";print(Vec.at(a));cout<<"  ";
        }
    else
            cout<<"|       ";
}

void odkryj(vector<vector <karta> >& VecKolumn)
{
    for (int i=0 ; i<7 ;i++)
        {
            VecKolumn.at(i).back().zakryta=0;
        }
}

void przelozenie (queue <karta>& Dobierane, vector <karta>& VecDobierane)
{
    if(!Dobierane.empty())
    {
        if(!VecDobierane.empty())
        {
            Dobierane.push(VecDobierane.at(0));
            VecDobierane.clear();
        }
        VecDobierane.push_back(Dobierane.front());
        Dobierane.pop();
    }
}

int szukaj(vector<karta> &Kolumna, string szukane)
{
    for(int i=0; i<Kolumna.size(); i++)
    {
        if(dostep(Kolumna.at(i), szukane))
        {
            return i;
        }
    }
    return -1;
}

bool dostep(karta K, string szukane)
{
    if(K.numer==szukane && K.zakryta==0)
        return 1;
    return 0;
}

int ruch(vector< vector<karta> > &VecKolumn, vector<karta> &VecDobierane,queue <karta> &Dobierane, vector < stack<karta> >& Stos)
{
    int Z;
    int Do;
    string card;
    int indeks=0;
    int check=1;

        do
        {
            if(indeks==-1) cout << "Nie ma takiej karty w podanej kolumnie. Podaj jeszcze raz: " << endl;
            while(1)
            {
                cout<<"Z jakiej kolumny chcesz przeniesc karte (0-7) z 0 mozna wziac tylko 1. karte: ";
                cin>>Z;
                if(Z<0 || Z>7) cout << "Nie ma takiej kolumny. Podaj jeszcze raz: ";
                else break;
            }
            if(Z){cout<<"Jaka karte chcesz przeniesc: ";
            cin>>card;}
            if(Z==0)
            {
                if(VecDobierane.size())
                    indeks=0;
                else
                {
                    cout << "Nie ma tu zadnych kart." << endl; indeks=-1;
                }

            }
        else
        {
            indeks=szukaj(VecKolumn.at(Z-1), card);
        }
        }while(indeks==-1);
        int x=2;
        do{

            cout<<"Do jakiej kolumny chcesz ja przeniesc (0-7) lub stosu?: (8-11): ";
            cin>>Do;
            if(Do<1 || Do>11) {cout << "Nie ma takiej kolumny. Podaj jeszcze raz: "; Sleep(2000); return 0;}
            if(Z==0 && Do<8)
            {
                if(SprawdzKolumne(VecKolumn.at(Do-1), VecDobierane, indeks))
                    break;
                else cout << "Nie mozna do tej kolumny. "<<endl; Sleep(2000); return 0;
            }
            else if(Z==0 && Do>7)
            {
                if(SprawdzStos(Stos.at(Do-8), VecDobierane, indeks))
                    break;
                else cout << "Nie mozna do tej kolumny. "<<endl; Sleep(2000); return 0;
            }
            else if(Z!=0 && Do<8)
            {
                if(SprawdzKolumne(VecKolumn.at(Do-1), VecKolumn.at(Z-1), indeks))
                    break;
                else cout << "Nie mozna do tej kolumny. "<<endl; Sleep(2000); return 0;
            }
            else
            {
                if(SprawdzStos(Stos.at(Do-8), VecKolumn.at(Z-1), indeks))
                    break;
                else cout << "Nie mozna do tej kolumny. " <<endl; Sleep(2000); return 0;
            }
        }
        while(x=2);

    if(Do<8) PrzenoszenieDoKolumny(VecKolumn, VecDobierane, indeks, Z, Do);
    else
    {
        Do-=8;
        PrzenoszenieDoStosu(VecKolumn, VecDobierane, Stos, indeks, Z, Do);
    }
}

bool SprawdzKolumne(vector<karta> &VecDo, vector<karta> &VecZ, int indeks)
{
    bool a=SprawdzWartoscKolumna(VecDo, VecZ, indeks);
    bool b=SprawdzKolorKolumna(VecDo, VecZ, indeks);
    if(a==1 && b==1)
        return 1;
    return 0;
}

bool SprawdzWartoscKolumna(vector<karta> &VecDo, vector<karta> &VecZ, int indeks)
{
    if(VecDo.empty())
        {
            if(VecZ.at(indeks).wartosc==13)
                return 1;
            else
                return 0;
        }
    if(VecDo.back().wartosc==VecZ.at(indeks).wartosc + 1)
        return 1;
    return 0;
}

bool SprawdzKolorKolumna(vector<karta> &VecDo, vector<karta> &VecZ, int indeks)
{
    if(VecDo.back().kolor!=VecZ.at(indeks).kolor)
        return 1;
    return 0;
}

bool SprawdzStos(stack<karta> &stos, vector<karta> &VecZ, int indeks)
{
    if(stos.empty())
    {
        if(VecZ.at(indeks).wartosc==1)
            return 1;
        else
            return 0;
    }
    bool a=SprawdzWartoscStos(stos, VecZ, indeks);
    bool b=SprawdzFigureStos(stos, VecZ, indeks);
    if(a==1 && b==1)
        return 1;
    return 0;
}

bool SprawdzWartoscStos(stack<karta> &stos, vector<karta> &VecZ, int indeks)
{
    if(VecZ.at(indeks).wartosc==stos.top().wartosc+1)
        return 1;
    return 0;
}

bool SprawdzFigureStos(stack<karta> &stos, vector<karta> &VecZ, int indeks)
{
    int DlStos=stos.top().numer.length();
    int DlKolumna=VecZ.at(indeks).numer.length();
    if(stos.top().numer[DlStos-1]==VecZ.at(indeks).numer[DlKolumna-1])
        return 1;
    return 0;
}

void PrzenoszenieDoKolumny(vector< vector<karta> > &VecKolumn, vector<karta> &VecDobieranie, int indeks,int Z,int Do)
{
    if(Z==0)
    {
        VecKolumn.at(Do-1).push_back(VecDobieranie.at(indeks));
        VecDobieranie.erase(VecDobieranie.begin());
    }
    else
    {
        for (int i=indeks; i<VecKolumn.at(Z-1).size(); i++)
        {
            KartaDoKolumny(VecKolumn ,VecKolumn.at(Z-1), i, Do);
        }
        int w=VecKolumn.at(Z-1).size()-indeks;
        for (int i=0;i<w;i++)
        {
            VecKolumn.at(Z-1).pop_back();
        }
        if(VecKolumn.at(Z-1).back().zakryta==1 && VecKolumn.at(Z-1).empty()!=1)
            VecKolumn.at(Z-1).back().zakryta=0;
    }
}

void PrzenoszenieDoStosu(vector< vector<karta> > &VecKolumn, vector<karta> &VecDobieranie, vector< stack<karta> > &Stos, int indeks,int Z,int Do)
{
    if (Z==0)
    {
        Stos.at(Do).push(VecDobieranie.at(indeks));
        VecDobieranie.erase(VecDobieranie.begin());
    }
    else
    {
        KartaZStosu(Stos, VecKolumn.at(Z-1), indeks, Do);
    }
}

void KartaDoKolumny(vector< vector<karta> > &VecKolumn, vector<karta> &Vec, int i, int Do)
{
    VecKolumn.at(Do-1).push_back(Vec.at(i));
}

void KartaZStosu (vector< stack<karta> > &Stos, vector<karta> &Vec, int indeks, int Do)
{
    Stos.at(Do).push(Vec.at(indeks));
    Vec.pop_back();
    if(Vec.back().zakryta==1 && Vec.empty()!=1)
        Vec.back().zakryta=0;
}
void Menu (vector <vector <karta> >& VecKolumn,vector <karta>& VecDobierane, queue <karta>& Dobierane, vector < stack<karta> >& Stos)
{
        int n=1;
        rysowanie(VecKolumn,VecDobierane, Stos);
        cout<<"Jaki ruch chcesz wykonac?"<<endl;
        cout<<"1.Przeloz karte w kolejce."<<endl;
        cout<<"2.Rusz karta."<<endl;
        cout<<"0.Zakoncz gre"<<endl;
        do  {
                cin>>n;
            }
        while (n>2 || n<0);
        switch(n)
           {
            case 0: exit(0); break;
            case 1: przelozenie(Dobierane,VecDobierane); break;
            case 2: ruch(VecKolumn, VecDobierane, Dobierane,Stos); break;
           }
        system("cls");
}
