#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
int WyliczDopelnienie(int szerokosc);
void PiszNagl(ofstream &ofs,ifstream &ifs,int poczImg,int NowaSzer,int NowaWys);
void PiszZdj(ifstream &ifs,ofstream &ofs,int poczImg,int x,int y,int Nowaszer,int NowaWys,int biszer,int szer);
int main()
{
    ifstream ifs;
    ofstream ofs;
    ifs.open("systemyhydro1.bmp", ios::binary); // otwarcie wejscia

if(!ifs)
{
    cout<<"Blad w otwieraniu pliku"<<endl;
    system("pause");
    return 0;
}

    ofs.open("systemyhydro1_fragment.bmp", ios::binary); // otwarcie wyjscia

    ifs.seekg(2); //funkcja ustawiajaca kuros w pliku wejsciowym w miejscu podanym w nawaisach

    int rozmiar_pliku;
    ifs.read((char*)&rozmiar_pliku, sizeof(int)); // funkcja czytajaca, ktora za pierwszy argument bierze zmienna do ktorej zapisuje przeczytane informacje, a drugi argument to dlugosc jaka ma czytac z pliku, w tym wypadku rozmiar inta

    ifs.seekg(10);
    int poczImg;
    ifs.read((char*)&poczImg, sizeof(int));

    ifs.seekg(18);
    int Biszer;
    ifs.read((char*)&Biszer, sizeof(int));

    ifs.seekg(22);
    int Biwys;
    ifs.read((char*)&Biwys, sizeof(int));

    int szer;
    szer=Biszer*3; // na kazdy pixel przypadaja 3 miejsca bo R G B
    szer+=(szer*3)%4; // wyrownanie do liczby bajtow podzielnej przez 4


    unsigned int NowaSzer, NowaWys, x, y;

    cout << "Podaj wspolrzedne lewego rogu" << endl;
    cout << "x: ";
    cin >> x;
    cout << "y: ";
    cin >> y;
    cout << "Podaj szerokosc: ";
    cin >> NowaSzer;
    cout << "Podaj wysokosc: ";
    cin >> NowaWys;
    PiszNagl(ofs,ifs,poczImg,NowaSzer,NowaWys);
    PiszZdj(ifs,ofs,poczImg,x,y,NowaSzer,NowaWys,Biszer,szer);
    ofs.close();
    ifs.close();
}
void PiszNagl(ofstream &ofs,ifstream &ifs,int poczImg,int NowaSzer,int NowaWys)
{
    unsigned char Naglowek [poczImg];
    ifs.seekg(0);
    for(int i=0; i<poczImg;i++)
        {
            unsigned char buff;
            ifs.read((char*)&buff,1);
            Naglowek[i]=buff;
        }
    unsigned int NowyRozmiarZdj = NowaWys * (NowaSzer*3 + WyliczDopelnienie(NowaSzer));
    unsigned int NowyRozmiarPliku;
    NowyRozmiarPliku=poczImg+NowyRozmiarZdj;

    for(int i=0; i<poczImg;i++)
        {
            unsigned int buff;
            if(i==2)
            {
                buff=NowyRozmiarPliku;
                i+=3;
                ofs.write((char*)&buff,4);
            }
            else if(i==18)
            {
                buff=NowaSzer;
                i+=3;
                ofs.write((char*)&buff,4);
            }
            else if(i==22)
            {
                buff=NowaWys;
                i+=3;
                ofs.write((char*)&buff,4);
            }
            else if(i==34)
            {
                buff=NowyRozmiarZdj;
                i+=3;
                ofs.write((char*)&buff,4);
            }
            else
            ofs<<Naglowek[i];
        }
}
int WyliczDopelnienie(int szerokosc)
{
    int dopelnienie = 0;
    if(szerokosc*3 % 4 != 0) {
        dopelnienie = 4 - szerokosc*3 % 4;
    }
    return dopelnienie;
}
void PiszZdj(ifstream &ifs,ofstream &ofs,int poczImg,int x,int y,int Nowaszer,int NowaWys,int biszer,int szer)
{
    unsigned int aktualnyBajt = poczImg + y*(3*biszer+ WyliczDopelnienie(biszer));
    char buff;
    unsigned int dopelnienie = WyliczDopelnienie(Nowaszer);
    for(int i=0; i<NowaWys; i++)
        {
        aktualnyBajt += 3*x;
        ifs.seekg(aktualnyBajt);
        for(int j=0; j<Nowaszer*3; j++)
            {
            ifs.get(buff);
            ofs.put(buff);
            }
        for(int k=0; k<dopelnienie; k++)
            {
            ofs<<0;
            }
        aktualnyBajt += szer-3*x;
        }
}
