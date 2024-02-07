#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

struct Adresat
{
    int id=0, idUzytkownika = 0;
    string imie="", nazwisko="", numerTelefonu="", email="", adres="";
};

struct Uzytkownik{
    int id = 0;
    string nazwa = "", haslo = "";
};

string wczytajLinie()
{
    string wejscie = "";
    cin.sync();
    getline(cin, wejscie);
    return wejscie;
}

int wczytajLiczbeCalkowita()
{
    string wejscie ="";
    int liczba = 0;

    while(true)
    {
        getline(cin, wejscie);

        stringstream myStream(wejscie);
        if (myStream >> liczba)
            break;
        cout << "To nie jest liczba. Wpisz ponownie." << endl;
    }
    return liczba;
}

char wczytajZnak()
{
    string wejscie ="";
    char znak = {0};

    while (true)
    {
        getline(cin, wejscie);

        if (wejscie.length() == 1)
        {
            znak = wejscie[0];
            break;
        }
        cout << "To nie jest pojedynczy znak. Wpisz ponownie." << endl;
    }
    return znak;
}

void dodajUzytkownika(vector <Uzytkownik> &uzytkownicy)
{
    string nazwa, haslo;
    fstream plik;
    Uzytkownik dodawanyUzytkownik;

    plik.open("Uzytkownicy.txt", ios::app);

    !uzytkownicy.empty() ? dodawanyUzytkownik.id = uzytkownicy.back().id + 1: dodawanyUzytkownik.id = 1;
    cout << "Podaj login: ";
    dodawanyUzytkownik.nazwa = wczytajLinie();
    cout << "Podaj haslo: ";
    dodawanyUzytkownik.haslo = wczytajLinie();

    uzytkownicy.push_back(dodawanyUzytkownik);

    plik << dodawanyUzytkownik.id << "|";
    plik << dodawanyUzytkownik.nazwa << "|";
    plik << dodawanyUzytkownik.haslo << "|" << endl;
    plik.close();

    cout << "Uzytkownik dodany" << endl;

    Sleep(1000);
}

Uzytkownik odczytajUzytkownika(string linia)
{
    Uzytkownik odczytywanyUzytkownik;
    linia.erase(linia.find_last_of("|"));
    odczytywanyUzytkownik.haslo = linia.substr(linia.find_last_of("|")+1);
    linia.erase(linia.find_last_of("|"));
    odczytywanyUzytkownik.nazwa = linia.substr(linia.find_last_of("|")+1);
    linia.erase(linia.find_last_of("|"));
    odczytywanyUzytkownik.id = stoi(linia);
    return odczytywanyUzytkownik;
}

void wczytajUzytkownikow(vector <Uzytkownik> &uzytkownicy)
{
    string linia = "";
    fstream plik;
    Uzytkownik wczytywanyUzytkownik;

    plik.open("Uzytkownicy.txt", ios::in);

    while(getline(plik, linia))
    {
        wczytywanyUzytkownik = odczytajUzytkownika(linia);
        uzytkownicy.push_back(wczytywanyUzytkownik);
    }
    plik.close();
}

int zalogujUzytkownika(vector <Uzytkownik> &uzytkownicy){
    int liczbaPozostalychProb = 3;
    string nazwa, haslo;
    cout << "Podaj login: ";
    nazwa = wczytajLinie();

        auto iter = find_if(uzytkownicy.begin(), uzytkownicy.end(),
                        [&](Uzytkownik const & uzytkownik)
    {
        return uzytkownik.nazwa == nazwa;
    });

    if ( iter != uzytkownicy.end() )
    {
        while(liczbaPozostalychProb>0){
            cout << "Podaj haslo. Pozostalo prob: " << liczbaPozostalychProb << ": ";
            haslo = wczytajLinie();

            if (iter->haslo == haslo){
                cout << "Zalogowano pomyslnie";
                Sleep(1000);
                return iter->id;
            }
            else {
                cout << "Zle haslo!" << endl;
                liczbaPozostalychProb--;
            }
        }
             cout << "Wykorzystano wszystkie proby. Przerwano operacje";
        Sleep(1000);
        return 0;
    }

    else
    {
        cout << "Brak uzytkownika o podanym loginie. Przerwano operacje";
        Sleep(1000);
        return 0;
    }
}

Adresat odczytajAdresata(string linia)
{
    Adresat adresat;
    linia.erase(linia.find_last_of("|"));
    adresat.adres = linia.substr(linia.find_last_of("|")+1);
    linia.erase(linia.find_last_of("|"));
    adresat.email = linia.substr(linia.find_last_of("|")+1);
    linia.erase(linia.find_last_of("|"));
    adresat.numerTelefonu = linia.substr(linia.find_last_of("|")+1);
    linia.erase(linia.find_last_of("|"));
    adresat.nazwisko = linia.substr(linia.find_last_of("|")+1);
    linia.erase(linia.find_last_of("|"));
    adresat.imie = linia.substr(linia.find_last_of("|")+1);
    linia.erase(linia.find_last_of("|"));
    adresat.idUzytkownika = stoi(linia.substr(linia.find_last_of("|")+1));
    linia.erase(linia.find_last_of("|"));
    adresat.id = stoi(linia);
    return adresat;
}

int odczytajOstatnieIdAdresata(){

    int ostatnieId = 1;
    Adresat wczytywanyAdresat;
    fstream plik;
    string linia;

    plik.open("Adresaci.txt", ios::in);

    while(getline(plik, linia))
    {
        wczytywanyAdresat = odczytajAdresata(linia);
        ostatnieId = wczytywanyAdresat.id;
    }

    plik.close();

    return ostatnieId;
}


void dodajAdresata(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    string imie, nazwisko, numerTelefonu, email, adres;
    fstream plik;
    Adresat dodawanyAdresat;

    plik.open("Adresaci.txt", ios::app);

    dodawanyAdresat.id = odczytajOstatnieIdAdresata() + 1;
    dodawanyAdresat.idUzytkownika = idZalogowanegoUzytkownika;

    cout << "Podaj imie: ";
    dodawanyAdresat.imie = wczytajLinie();
    cout << "Podaj nazwisko: ";
    dodawanyAdresat.nazwisko = wczytajLinie();
    cout << "Podaj numer telefonu: ";
    dodawanyAdresat.numerTelefonu = wczytajLinie();
    cout << "Podaj email: ";
    dodawanyAdresat.email = wczytajLinie();
    cout << "Podaj adres: ";
    dodawanyAdresat.adres = wczytajLinie();

    adresaci.push_back(dodawanyAdresat);

    plik << dodawanyAdresat.id << "|";
    plik << dodawanyAdresat.idUzytkownika << "|";
    plik << dodawanyAdresat.imie << "|";
    plik << dodawanyAdresat.nazwisko << "|";
    plik << dodawanyAdresat.numerTelefonu << "|";
    plik << dodawanyAdresat.email << "|";
    plik << dodawanyAdresat.adres << "|" << endl;
    plik.close();

    cout << "Adresat dodany" << endl;

    Sleep(1000);
}

void zaktualizujKsiazkeAdresowa(vector <Adresat> adresaci)
{
    fstream plik;

    plik.open("Adresaci.txt", ios::out);

    for(Adresat adresat : adresaci)
    {
        plik << adresat.id << "|";
        plik << adresat.idUzytkownika << "|";
        plik << adresat.imie << "|";
        plik << adresat.nazwisko << "|";
        plik << adresat.numerTelefonu << "|";
        plik << adresat.email << "|";
        plik << adresat.adres << "|" << endl;
    }

    plik.close();
}

void wyswietlAdresata(Adresat adresat)
{
    cout << "ID:              " << adresat.id << endl;
    cout << "Imie:            " << adresat.imie << endl;
    cout << "Nazwisko:        " << adresat.nazwisko << endl;
    cout << "Numer telefonu:  " << adresat.numerTelefonu << endl;
    cout << "Email:           " << adresat.email << endl;
    cout << "Adres:           " << adresat.adres << endl << endl;
}

void wyswietlWszystkichAdresatow(vector <Adresat> adresaci)
{
    system("cls");
    cout << "KSIAZKA ADRESOWA" << endl << endl;

    for(Adresat adresat : adresaci)
    {
        wyswietlAdresata(adresat);
    }
    system("pause");
}

void wyszukajAdresataPoImieniu(vector <Adresat> adresaci)
{
    string imie;
    cout << "Podaj imie: ";
    imie = wczytajLinie();

    system("cls");
    cout << "Wyniki wyszukiwania po imieniu: " << imie << endl << endl;

    for(Adresat adresat : adresaci)
    {
        if(adresat.imie == imie)
        {
            wyswietlAdresata(adresat);
        }
    }
    system("pause");
}

void wyszukajAdresataPoNazwisku(vector <Adresat> adresaci)
{
    string nazwisko;
    cout << "Podaj nazwisko: ";
    nazwisko = wczytajLinie();

    system("cls");
    cout << "Wyniki wyszukiwania po nazwisku: " << nazwisko << endl << endl;

    for(Adresat adresat : adresaci)
    {
        if(adresat.nazwisko == nazwisko)
        {
            wyswietlAdresata(adresat);
        }
    }
    system("pause");
}

void wczytajAdresatow(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    string linia = "";
    fstream plik;
    Adresat wczytywanyAdrestat;

    plik.open("Adresaci.txt", ios::in);

    while(getline(plik, linia))
    {
        wczytywanyAdrestat = odczytajAdresata(linia);
        if(wczytywanyAdrestat.idUzytkownika == idZalogowanegoUzytkownika) adresaci.push_back(wczytywanyAdrestat);
    }
    plik.close();
}

void usunAdresata(vector<Adresat> &adresaci)
{
    int idAdresata;
    char potwierdzenie;

    cout << "Podaj id adresata, ktorego chcesz usunac: ";
    idAdresata = wczytajLiczbeCalkowita();

    auto iter = find_if(adresaci.begin(), adresaci.end(),
                        [&](Adresat const & adresat)
    {
        return adresat.id == idAdresata;
    });

    if ( iter != adresaci.end() )
    {
        cout << "Wpisz 't' aby usunac adresata o numerze id " << idAdresata << endl;
        potwierdzenie = wczytajZnak();

        if(potwierdzenie == 't')
        {
            adresaci.erase(iter);

            zaktualizujKsiazkeAdresowa(adresaci);
            cout << "Adresat usuniety pomyslnie";
        }

        else
        {
            cout << "Brak potwierdzenia usuniecia adresata. Przerwano operacje";
        }
    }

    else
    {
        cout << "Brak adresata o podanym id. Przerwano operacje";
    }

    Sleep(1000);
}

void edytujAdresata(vector<Adresat> &adresaci)
{
    int idAdresata;
    char wybor;
    string nowaWartosc;

    cout << "Podaj id adresata, ktorego chcesz edytowac: ";
    idAdresata = wczytajLiczbeCalkowita();

    auto iter = find_if(adresaci.begin(), adresaci.end(),
                        [&](Adresat const & adresat)
    {
        return adresat.id == idAdresata;
    });

    if ( iter != adresaci.end() )
    {
        system("cls");
        cout << "1. Imie" << endl;
        cout << "2. Nazwisko" << endl;
        cout << "3. Numer telefonu" << endl;
        cout << "4. Email" << endl;
        cout << "5. Adres" << endl;
        cout << "6. Powrot do menu" << endl << endl;
        cout << "Twoj wybor: ";
        wybor = wczytajZnak();
        if(wybor == '6') return;
        cout << "Podaj nowa wartosc: ";
        nowaWartosc = wczytajLinie();

        switch (wybor)
        {
        case '1':
            iter->imie = nowaWartosc;
            break;
        case '2':
            iter->nazwisko = nowaWartosc;
            break;
        case '3':
            iter->numerTelefonu = nowaWartosc;
            break;
        case '4':
            iter->email = nowaWartosc;
            break;
        case '5':
            iter->adres = nowaWartosc;
            break;
        }
        zaktualizujKsiazkeAdresowa(adresaci);
        cout << "Edycja zakonczona pomyslnie";
    }

    else
    {
        cout << "Brak adresata o podanym id. Przerwano operacje";
    }

    Sleep(1000);

}

int main()
{
    int idZalogowanegoUzytkownika = 0;
    vector <Uzytkownik> uzytkownicy;
    vector <Adresat> adresaci;
    char wybor;

    wczytajUzytkownikow(uzytkownicy);

    while(1)
    {
        if(idZalogowanegoUzytkownika == 0){

     system("cls");
        cout << ">>> MENU  GLOWNE <<<" << endl;
        cout << "--------------------------" << endl;
        cout << "1. Rejestracja" << endl;
        cout << "2. Logowanie" << endl;
        cout << "--------------------------" << endl;
        cout << "9. Zakoncz program" << endl << endl;
        cout << "Twoj wybor: ";
        wybor = wczytajZnak();

        switch (wybor)
            {
            case '1':
                dodajUzytkownika(uzytkownicy);
                break;
            case '2':
                idZalogowanegoUzytkownika = zalogujUzytkownika(uzytkownicy);
                wczytajAdresatow(adresaci, idZalogowanegoUzytkownika);
                break;
            case '9':
                exit(0);
                break;

            }
        }

        else {
     system("cls");
        cout << ">>> MENU  UZYTKOWNIKA <<<" << idZalogowanegoUzytkownika << endl;
        cout << "--------------------------" << endl;
        cout << "1. Dodaj adresata" << endl;
        cout << "2. Wyszukaj adresata po imieniu" << endl;
        cout << "3. Wyszukaj adresata po nazwisku" << endl;
        cout << "4. Wyswietl wszystkich adresatow" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "--------------------------" << endl;
        cout << "8. Wyloguj sie" << endl << endl;
        cout << "9. Zakoncz program" << endl << endl;
        cout << "Twoj wybor: ";
        wybor = wczytajZnak();

        switch (wybor)
            {
            case '1':
                dodajAdresata(adresaci, idZalogowanegoUzytkownika);
                break;
            case '2':
                wyszukajAdresataPoImieniu(adresaci);
                break;
            case '3':
                wyszukajAdresataPoNazwisku(adresaci);
                break;
            case '4':
                wyswietlWszystkichAdresatow(adresaci);
                break;
            case '5':
                usunAdresata(adresaci);
                break;
            case '6':
                edytujAdresata(adresaci);
                break;
            case '8':
                idZalogowanegoUzytkownika = 0;
                adresaci.clear();
                break;
            case '9':
                exit(0);
                break;

            }
        }
    }
    return 0;
}
