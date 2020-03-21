#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const int MAX_KOMPISAR=15;
const int MAX_PERSONER=15;
const int MAX_TRANSAKTIONER=30;

class Transaktion
{
private:
    string datum;
    string typ;
    string namn;
    int kompisnr;
    double belopp;
    int ant_kompisar;
    string kompisar[MAX_KOMPISAR];

public:
    Transaktion();
    ~Transaktion();
    string haemta_datum();
    string haemta_typ();
    string haemta_namn();
    string haemta_kompis(int kompisnr);
    double haemta_belopp();
    int haemta_ant_kompisar();
    bool finnsKompis( string namnet );
    bool laesEnTrans( istream &is );
    void skrivEnTrans( ostream &os );
};

class Person
{
private:
    string namn;
    double betalat_andras;   // ligger ute med totalt
    double skyldig;          // skyldig totalt

public:
    Person();
    Person( string n, double b, double s );
    string haemta_namn();
    double haemta_betalat();
    double haemta_skyldig();
    void skrivUt();
};

class PersonLista
{
private:
    int antal_pers;
    Person pers[MAX_PERSONER];

public:
    PersonLista();
    ~PersonLista();
    void laggTillEn( Person pny );
    void skrivUtOchFixa();
    double summaSkyldig();
    double summaBetalat();
    bool finnsPerson(const string& namn);
};

class TransaktionsLista
{
private:
    Transaktion trans[MAX_TRANSAKTIONER];
    int antalTrans;

public:
    TransaktionsLista();
    ~TransaktionsLista();
    void laesin( istream & is );
    void skrivut( ostream & os );
    void laggTill( Transaktion & t );
    int haemtaantalTrans();
    double totalkostnad();
    double liggerUteMed( string namnet );
    double aerSkyldig( string namnet );
    PersonLista FixaPersoner();

};

int main()
{
    Transaktion transak;
    TransaktionsLista translista;
    PersonLista personer;
    bool keepon = true;

    ifstream indata("resa.txt");
    translista.laesin(indata);

    cout << "Startar med att läsa in en fil " << endl << endl;
    while(keepon)
    {
        cout << "Välj i menyn nedan: " << endl;
        cout << "0. Avsluta. Alla transaktioner sparas på fil." << endl;
        cout << "1. Läs in en transaktion från tangentbordet." << endl;
        cout << "2. Skriv ut information om alla transaktioner." << endl;
        cout << "3. Beräkna totala kostnaden." << endl;
        cout << "4. Hur mycket är en viss person skyldig?" << endl;
        cout << "5. Hur mycket ligger en viss person ute med?" << endl;
        cout << "6. Lista alla personer mm och FIXA!!!" << endl;

        int i;
        cin >> i;
        switch(i)
        {
            case 0:
            {
                cout << "Vilken fil skall vi skriva ut på?" << endl;
                string filnamnet;
                cin >> filnamnet;

                ofstream utfilen;
                utfilen.open ( filnamnet.c_str() );
                translista.skrivut(utfilen);
                utfilen.close();
                keepon=false;
                break;
            }
            case 1:
            {
                int antal;
                cout << "Skriv antal transaktioner" << endl;
                cin >> antal;
                cout << "Skriv en sträng med datum, typ, namn, belopp, antal kompisar och deras namn, använd mellanslag mellan varje inmatning och även i slutet av strängen!" << endl;
                for (int i=0; i<antal; i++)
                {
                    transak.laesEnTrans( cin );
                    translista.laggTill(transak);
                }
                break;
            }
            case 2:
            {
                translista.skrivut(cout);
                break;
            }
            case 3:
            {
                double totkostnad;
                totkostnad = translista.totalkostnad();
                cout << "Den totala kostnaden är: " << totkostnad << endl << endl;
                break;
            }
            case 4:
            {

                cout << "Vem vill du veta skuld för?" << endl;
                string namn;
                cin >> namn;

                double skuld;
                skuld = translista.aerSkyldig(namn);
                cout << namn << " är skyldig: " << skuld << "kr!" << endl;
                break;
            }
            case 5:
            {
                cout << "Vem vill du veta utlägg för?" << endl;
                string namn;
                cin >> namn;

                double utemed;
                utemed = translista.liggerUteMed(namn);
                cout << namn << " ligger ute med: " << utemed << "kr!" << endl;
                break;
            }
            case 6:
            {

                personer = translista.FixaPersoner();
                personer.skrivUtOchFixa();
                break;
            }
            default:
            {
                cout << "Vilken fil skall vi skriva ut på?" << endl;
                string filnamnet;
                cin >> filnamnet;

                ofstream utfilen;
                utfilen.open ( filnamnet.c_str() );
                translista.skrivut(utfilen);
                utfilen.close();
                keepon=false;
                break;
            }
        }
    }
    return 0;
}

Transaktion::Transaktion()
{
    belopp=0;
    ant_kompisar=0;
    kompisar[MAX_KOMPISAR];
}

Transaktion::~Transaktion()
{
    // cout << "Nu är denna transaktion dod" << endl;
}

string Transaktion::haemta_datum()
{
    return datum;
}

string Transaktion::haemta_typ()
{
    return typ;
}

string Transaktion::haemta_namn()
{
    return namn;
}

double Transaktion::haemta_belopp()
{
    return belopp;
}

int Transaktion::haemta_ant_kompisar()
{
    return ant_kompisar;
}

string Transaktion::haemta_kompis(int kompisnr)
{
    string kompis;
    kompis=kompisar[kompisnr];
    return kompis;
}

bool Transaktion::finnsKompis( string namnet )
{
    bool finns=false;
    string kompis;
    for(int i=0; i<ant_kompisar; i++)
    {
        kompis = kompisar[i];
        if(kompis == namnet)
        {
            finns=true;
        }
    }
    return finns;
}

bool Transaktion::laesEnTrans( istream &is )
{
    string kompis;
    is >> datum;
    is >> typ;
    is >> namn;
    is >> belopp;
    is >> ant_kompisar;

    for(int i=0; i<ant_kompisar; i++)
    {
        is >> kompis;
        kompisar[i] = kompis;
    }
    return !is.eof();
}

void Transaktion::skrivEnTrans( ostream &os )
{
    os << datum << " " << typ << " " << namn << " " << belopp << " " << ant_kompisar << " ";
    for(int i =0; i< ant_kompisar; i++)
    {
        os << kompisar[i] << " ";
    }
    os << "\n";
}

TransaktionsLista::TransaktionsLista()
{
    antalTrans=0;
}

TransaktionsLista::~TransaktionsLista()
{
   // cout << "Nu är denna transaktionslista dod" << endl;
}

int TransaktionsLista::haemtaantalTrans()
{
    return antalTrans;
}

void TransaktionsLista::laesin( istream & is )
{
    Transaktion t;

    while ( t.laesEnTrans( is ) )
    {
        laggTill( t );
    }
}

void TransaktionsLista::skrivut( ostream &os )
{
    cout << "Antal transaktioner: " << antalTrans << endl << endl;
    for(int i=0; i < antalTrans; i++)
    {
        trans[i].skrivEnTrans(os);
    }
    cout << endl << endl;
}

void TransaktionsLista::laggTill( Transaktion & t )
{
    trans[antalTrans] = t;
    antalTrans++;
}

double TransaktionsLista::totalkostnad()
{
    double totkost=0;
    double kost=0;
    for(int i = 0; i<antalTrans; i++)
    {
        kost = trans[i].haemta_belopp();
        cout << "Kostnad nummer " << i+1 << " är "<< kost << endl;
        totkost = totkost + kost;
    }
    return totkost;
}

double TransaktionsLista::liggerUteMed( string namnet )
{
    string namn;
    double belopp=0;
    double liggerute=0;
    double antkomp;

    for(int i =0; i<antalTrans; i++)
    {
        namn = trans[i].haemta_namn();
        if(namn == namnet)
        {
            if(!trans[i].finnsKompis(namnet)){
            belopp = trans[i].haemta_belopp();
            }

            else{
            antkomp = trans[i].haemta_ant_kompisar();
            belopp = trans[i].haemta_belopp()*((antkomp-1)/antkomp);
            }

            liggerute = liggerute + belopp;
        }
    }
    return liggerute;
}

double TransaktionsLista::aerSkyldig( string namnet )
{
    string namn;
    double belopp=0;
    double skyldig=0;
    double totskyldig=0;
    int ant_kompisar;
    for(int i = 0; i<antalTrans; i++){
        namn = trans[i].haemta_namn();
        if(!(namn == namnet) && trans[i].finnsKompis(namnet) )
        {
            belopp = trans[i].haemta_belopp();
            ant_kompisar = trans[i].haemta_ant_kompisar();
            skyldig = belopp/ant_kompisar;
            totskyldig = totskyldig + skyldig;
        }
    }
    return totskyldig;
}

PersonLista TransaktionsLista::FixaPersoner()
{
    string namn;
    string kompis;
    double belopp;
    int ant_kompisar;
    double betalat_andras;
    double skyldig;
    PersonLista fixat;

    for(int i=0; i<antalTrans; i++)
    {

        namn = trans[i].haemta_namn();
        if(!fixat.finnsPerson(namn))
        {
            skyldig = aerSkyldig(namn);
            betalat_andras = liggerUteMed(namn);
            fixat.laggTillEn(Person(namn,betalat_andras,skyldig));
        }

        ant_kompisar = trans[i].haemta_ant_kompisar();
        for(int nrkompis=0; nrkompis<ant_kompisar; nrkompis++)
        {
            namn = trans[i].haemta_kompis(nrkompis);
            if(!fixat.finnsPerson(namn))
            {
                skyldig = aerSkyldig(namn);
                betalat_andras = liggerUteMed(namn);
                fixat.laggTillEn(Person(namn,betalat_andras,skyldig));
            }
        }
    }
    return fixat;
}

Person::Person()
{
    betalat_andras=0;
    skyldig=0;
}

Person::Person( string n, double b, double s )
{
    namn = n;
    betalat_andras = b;
    skyldig = s;
}

string Person::haemta_namn()
{
    return namn;
}

double Person::haemta_betalat()
{
    return betalat_andras;
}

double Person::haemta_skyldig()
{
    return skyldig;
}

void Person::skrivUt()
{
    if(betalat_andras-skyldig >0)
    {
        cout << namn << " ligger ute med: " << betalat_andras << " och är skyldig: " << skyldig << " skall ha: " << betalat_andras-skyldig << " från potten!" << endl;
    }
    else
    {
        cout << namn << " ligger ute med: " << betalat_andras << " och är skyldig: " << skyldig << " skall lägga: " << skyldig-betalat_andras << " i potten!" << endl;
    }
}

PersonLista::PersonLista()
{
    antal_pers=0;
}


PersonLista::~PersonLista()
{
   // cout << "Nu ar personlistan borta." << endl;
}

void PersonLista::laggTillEn( Person pny )
{
    pers[antal_pers] = pny;
    antal_pers++;
}

double PersonLista::summaSkyldig()
{
    double skyldig=0;
    double totskyldig=0;
    for(int i=0; i<antal_pers; i++)
    {
        skyldig = pers[i].haemta_skyldig();
        totskyldig = totskyldig + skyldig;
    }
    return totskyldig;
}

double PersonLista::summaBetalat()
{
    double betalat=0;
    double totbetal=0;
    for(int i=0; i<antal_pers; i++)
    {
        betalat = pers[i].haemta_betalat();
        totbetal = totbetal + betalat;
    }
    return totbetal;
}

void PersonLista::skrivUtOchFixa()
{
    cout << "Det folk totalt ligger ute med: " << summaBetalat() << endl;
    cout << "Det folk totalt är skyldiga: " << summaSkyldig() << endl;

    for(int i=0; i<antal_pers; i++)
    {
        pers[i].skrivUt();
    }
}

bool PersonLista::finnsPerson(const string& namn)
{
    bool finns = false;
    string tempnamn;

    for(int i=0; i<antal_pers; i++)
    {
        tempnamn = pers[i].haemta_namn();
        if(tempnamn == namn)
        {
            finns =true;
        }
    }
    return finns;
}