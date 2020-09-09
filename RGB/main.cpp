#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
void R(int,ifstream&,ofstream&);
void G(int,ifstream&,ofstream&);
void B(int,ifstream&,ofstream&);
void Szarosc(int,ifstream&,ofstream&);
void Rozmycie(int rozmiar_zdj,ifstream &ifs,ofstream &ofs,int pocz,int szer);

int main(int argc, char * argv[])
{
ifstream ifs;
ofstream ofs;

ifs.open(argv[1], ios::binary);

if(!ifs)
{
    cout<<"Blad w otwieraniu pliku"<<endl;
    system("pause");
    return 0;
}

ofs.open(argv[2], ios::binary);

ifs.seekg(2);

int rozmiar_pliku;
ifs.read((char*)&rozmiar_pliku, sizeof(int));

ifs.seekg(10);
int pocz;
ifs.read((char*)&pocz, sizeof(int));

ifs.seekg(18);
int szer;
ifs.read((char*)&szer, sizeof(int));

ifs.seekg(22);
int wys;
ifs.read((char*)&wys, sizeof(int));

int rozmiar_zdj=0;
szer+=(3*szer)%4;
rozmiar_zdj=3*szer*wys;
ifs.seekg(0);
char buff;
for(int i=0 ; i<pocz ; i++)
{
    ifs.get(buff);
    ofs.put(buff);
}
ifs.seekg(pocz);
cout<<"MENU"<<endl<<endl<<endl;
cout<<"1.Wyodrebnij czerwony"<<endl;
cout<<"2.Wyodrebnij niebieski"<<endl;
cout<<"3.Wyodrebnij zielony"<<endl;
cout<<"4.Skala szarosci"<<endl;
cout<<"5.Rozmycie"<<endl;
cout<<"Jaka operacje chcesz wykonac?:";
int wybor;
cin>>wybor;
switch(wybor)
{
    case 1:{
        R(rozmiar_zdj,ifs,ofs);
      break;}
    case 2:{
        B(rozmiar_zdj,ifs,ofs);
      break;}
    case 3:{
        G(rozmiar_zdj,ifs,ofs);
      break;}
    case 4:{
        Szarosc(rozmiar_zdj,ifs,ofs);
      break;}
    case 5:{
        Rozmycie(rozmiar_zdj,ifs,ofs,pocz,szer);
      break;}
}
cout<<endl<<endl;
cout<<"Poprawna konwersja"<<endl;
cout<<"Rozmiar pliku:"<<rozmiar_pliku<<endl;
cout<<"Wysokosc i szerokosc grafiki:"<<wys<<"x"<<szer<<endl;
system("pause");
    return 0;
}

void B (int rozmiar_zdj,ifstream &ifs,ofstream &ofs)
{
  unsigned char R,G,B;
  for(int i=0; i<rozmiar_zdj; i+=3)
{
    ifs.read((char*)&B, sizeof(unsigned char));
    ifs.read((char*)&G, sizeof(unsigned char));
    ifs.read((char*)&R, sizeof(unsigned char));
        G=0;
        R=0;
    ofs<<B<<G<<R;
}
}
void G (int rozmiar_zdj,ifstream &ifs,ofstream &ofs)
{
  unsigned char R,G,B;
  for(int i=0; i<rozmiar_zdj; i+=3)
{
    ifs.read((char*)&B, sizeof(unsigned char));
    ifs.read((char*)&G, sizeof(unsigned char));
    ifs.read((char*)&R, sizeof(unsigned char));
        B=0;
        R=0;
    ofs<<B<<G<<R;
}
}
void R (int rozmiar_zdj,ifstream &ifs,ofstream &ofs)
{
  unsigned char R,G,B;
  for(int i=0; i<rozmiar_zdj; i+=3)
{
    ifs.read((char*)&B, sizeof(unsigned char));
    ifs.read((char*)&G, sizeof(unsigned char));
    ifs.read((char*)&R, sizeof(unsigned char));
        G=0;
        B=0;
    ofs<<B<<G<<R;
}
}
void Szarosc(int rozmiar_zdj,ifstream &ifs,ofstream &ofs)
{
      unsigned char R,G,B;

  for(int i=0; i<rozmiar_zdj; i+=3)
{
    ifs.read((char*)&B, sizeof(unsigned char));
    ifs.read((char*)&G, sizeof(unsigned char));
    ifs.read((char*)&R, sizeof(unsigned char));
    R=R/3;
    G=G/3;
    B=B/3;

    ofs<<B<<G<<R;

}

}
void Rozmycie(int rozmiar_zdj,ifstream &ifs,ofstream &ofs,int pocz,int szer)
{
    unsigned char R,G,B,Rg,Gg,Bg,Rd,Gd,Bd,Rl,Gl,Bl,Rp,Gp,Bp;
  for(int i=0; i<rozmiar_zdj; i+=3)
{
    int dzielnik=0;
    ifs.seekg(pocz+i);
    if((pocz+i+3)<rozmiar_zdj)
    {
        ifs.seekg(pocz+i+3);
        ifs.read((char*)&Bp, sizeof(unsigned char));
        ifs.read((char*)&Gp, sizeof(unsigned char));
        ifs.read((char*)&Rp, sizeof(unsigned char));
        dzielnik+=1;
    }
    else
    {
        Bp=Gp=Rp=0;
    }
    if((pocz+i-3)>pocz)
    {
        ifs.seekg(pocz+i-3);
        ifs.read((char*)&Bl, sizeof(unsigned char));
        ifs.read((char*)&Gl, sizeof(unsigned char));
        ifs.read((char*)&Rl, sizeof(unsigned char));
        dzielnik+=1;
    }
    else
    {
        Bl=Gl=Rl=0;
    }
    if((pocz+i+(szer*3))<rozmiar_zdj)
    {
        ifs.seekg(pocz+i+(szer*3));
        ifs.read((char*)&Bd, sizeof(unsigned char));
        ifs.read((char*)&Gd, sizeof(unsigned char));
        ifs.read((char*)&Rd, sizeof(unsigned char));
        dzielnik+=1;
    }
    else
    {
        Bd=Gd=Rd=0;
    }
    if((pocz+i-(szer*3))>pocz)
    {
        ifs.seekg(pocz+i-(szer*3));
        ifs.read((char*)&Bg, sizeof(unsigned char));
        ifs.read((char*)&Gg, sizeof(unsigned char));
        ifs.read((char*)&Rg, sizeof(unsigned char));
        dzielnik+=1;
    }
    else
    {
        Bg=Gg=Rg=0;
    }
    B=(Bl+Bp+Bg+Bd)/dzielnik;
    R=(Rl+Rp+Rg+Rd)/dzielnik;
    G=(Gl+Gp+Gg+Gd)/dzielnik;
    ofs<<B<<G<<R;

}




}
