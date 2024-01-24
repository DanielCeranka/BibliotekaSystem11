#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>


#define NAZWAPLIKUDANEKLIENTOW "DaneKlientow.txt"
#define NAZWABAZYKSIAZEKDOSTEPNYCH "KsiazkiDostepne.txt"
#define NAZWAKSIAZEKWYPOZYCZONYCH "KsiazkiWypozyczone.txt"
#define NAZWAOCZEKUJACYCHNAKSIAZKI "ListaOczekujacych.txt"
#define NAZWAOSOBWYOPOZYCZAJACYCH "ListaWypozyczajacych.txt"


using namespace std;

struct DaneKlienta {
    int ID = 0;
    string imie;
    string nazwisko;
    string pesel;
    string adres;
    string nr_tel;

    friend ostream& operator<<(ostream& os, const DaneKlienta& klient) {
        os << "ID klienta: " << klient.ID << endl;
        os << "Imie: " << klient.imie << endl;
        os << "Nazwisko: " << klient.nazwisko << endl;
        os << "PESEL: " << klient.pesel << endl;
        os << "Adres: " << klient.adres << endl;
        os << "Nr telefonu: " << klient.nr_tel << endl;
        return os;
    }
};

class System {
public:
    System(){}

    int zapiszKlientaDoPliku(const string& plikDaneKlienta, DaneKlienta nowyKlient) {
        ofstream plik(plikDaneKlienta, ios::out | ios::app);

        if (plik.is_open()) {
            plik << "-----------------------" << endl << nowyKlient << endl;

            plik.close();
            cout << "Dane zapisano pomyslnie do bazy." << endl << endl;
            return 0;
        }
        else {
            cerr << "Nie udalo sie otworzyc pliku do zapisu." << endl;
            return 1;
        }
    }

    bool sprawdzPoprawnoscImienia(const string& imie) {
        return !imie.empty() && all_of(imie.begin(), imie.end(), isalpha);
    }

    bool sprawdzPoprawnoscNazwiska(const string& nazwisko) {
        return !nazwisko.empty() && all_of(nazwisko.begin(), nazwisko.end(), isalpha);
    }

    bool sprawdzPoprawnoscPESEL(const string& pesel) {
        if (pesel.length() != 11) {
            return false;
        }

        if (!all_of(pesel.begin(), pesel.end(), isdigit)) {
            return false;
        }

        return true;
    }

    bool sprawdzPoprawnoscAdresu(const string& adres) {
        return !adres.empty() && all_of(adres.begin(), adres.end(), [](char c) {
            return isalnum(c) || isspace(c) || c == ',';
            });
    }

    bool sprawdzPoprawnoscNumeruTelefonu(const string& numer) {
        if (numer.empty()) {
            return false;
        }

        if (!all_of(numer.begin(), numer.end(), isdigit)) {
            return false;
        }

        if (numer.length() < 9) {
            return false;
        }

        return true;
    }

    bool sprawdzPoprawnoscID(const int id, int IDnast) {
        if (id >= IDnast) {
            return false;
        }
        else {
            return true;
        }
    }

    void usunWiersz(const string& nazwaPliku, const string& liniaDoUsuniecia) {
        ifstream plikWejsciowy(nazwaPliku);
        vector<string> linie;

        if (plikWejsciowy.is_open()) {
            string linia;
            while (getline(plikWejsciowy, linia)) {
                if (linia != liniaDoUsuniecia) {
                    linie.push_back(linia);
                }
            }
            plikWejsciowy.close();

            ofstream plikWyjsciowy(nazwaPliku);
            for (const auto& nowaLinia : linie) {
                plikWyjsciowy << nowaLinia << endl;
            }
            plikWyjsciowy.close();
        }
        else {
            cerr << "Nie można otworzyć pliku do odczytu." << endl;
        }
    }

    void usunWiersz(const string& nazwaPliku, const int liniaDoUsuniecia) {
        ifstream plikWejsciowy(nazwaPliku);
        vector<string> linie;

        if (plikWejsciowy.is_open()) {
            string linia;
            while (getline(plikWejsciowy, linia)) {
                if (stoi(linia) != liniaDoUsuniecia) {
                    linie.push_back(linia);
                }
            }
            plikWejsciowy.close();

            ofstream plikWyjsciowy(nazwaPliku);
            for (const auto& nowaLinia : linie) {
                plikWyjsciowy << nowaLinia << endl;
            }
            plikWyjsciowy.close();
        }
        else {
            cerr << "Nie można otworzyć pliku do odczytu." << endl;
        }
    }

    void dodajWiersz(const string& nazwaPliku, const string& nowaLinia) {
        ofstream plikWyjsciowy(nazwaPliku, ios::app);

        if (plikWyjsciowy.is_open()) {
            plikWyjsciowy << nowaLinia << endl;
            plikWyjsciowy.close();
        }
        else {
            cerr << "Nie można otworzyć pliku do zapisu." << endl;
        }
    }

    void dodajWiersz(const string& nazwaPliku, const int ID) {
        ofstream plikWyjsciowy(nazwaPliku, ios::app);

        if (plikWyjsciowy.is_open()) {
            plikWyjsciowy << ID << endl;
            plikWyjsciowy.close();
        }
        else {
            cerr << "Nie można otworzyć pliku do zapisu." << endl;
        }
    }
};

class Aplikacja {
private:
    int IDnast;
    System S;

    int ktoreID(const string& plikDaneKlienta) {
        ifstream plik(plikDaneKlienta);

        if (!plik.is_open()) {
            return 1;
        }

        string linia;
        int pom = 0;
        while (getline(plik, linia)) {
            pom++;
        }
        int ostatnieID = pom / 8 + 1;
        plik.close();

        return ostatnieID;
    }// funkcja przy uruchomieniu oblicza odpowiednie ID dla nastepnego klienta

public:
    static Aplikacja& getInstance() {
        static Aplikacja instance;
        return instance;
    }

    Aplikacja() : IDnast(ktoreID(NAZWAPLIKUDANEKLIENTOW)) {}


    int dodajCzytelnika() {
        DaneKlienta nowyKlient;

        cout << "Imie: ";
        cin >> nowyKlient.imie;

        while (!S.sprawdzPoprawnoscImienia(nowyKlient.imie)) {
            cout << "Niepoprawne imie. Wprowadz ponownie: ";
            cin >> nowyKlient.imie;
        }

        cout << "Nazwisko: ";
        cin >> nowyKlient.nazwisko;

        while (!S.sprawdzPoprawnoscNazwiska(nowyKlient.nazwisko)) {
            cout << "Niepoprawne nazwisko. Wprowadz ponownie: ";
            cin >> nowyKlient.nazwisko;
        }

        do {
            cout << "PESEL: ";
            cin >> nowyKlient.pesel;

            if (!S.sprawdzPoprawnoscPESEL(nowyKlient.pesel)) {
                cout << "Niepoprawny PESEL. Wprowadz ponownie." << endl;
            }
        } while (!S.sprawdzPoprawnoscPESEL(nowyKlient.pesel));

        cout << "Adres: ";
        cin.ignore();
        getline(cin, nowyKlient.adres);

        while (!S.sprawdzPoprawnoscAdresu(nowyKlient.adres)) {
            cout << "Niepoprawny adres. Wprowadz ponownie: ";
            getline(cin, nowyKlient.adres);
        }

        cout << "Nr telefonu: ";
        cin >> nowyKlient.nr_tel;

        while (!S.sprawdzPoprawnoscNumeruTelefonu(nowyKlient.nr_tel)) {
            cout << "Niepoprawny numer telefonu. Wprowadz ponownie: ";
            cin >> nowyKlient.nr_tel;
        }

        nowyKlient.ID = IDnast;
        IDnast++;
        cout << endl << "Przyznano numer ID: " << nowyKlient.ID << endl;
        // Dodaj nowego klienta do listy
        S.zapiszKlientaDoPliku(NAZWAPLIKUDANEKLIENTOW, nowyKlient);
        return 1;
    }

    int wyswietlDostepnoscKsiazki() {
        string tytul;
        cout << "Podaj tytul szukanej lektury:" << endl;
        cin.ignore();
        getline(cin, tytul);

        ifstream plikDostepneKsiazki(NAZWABAZYKSIAZEKDOSTEPNYCH);
        ifstream plikWypozyczoneKsiazki(NAZWAKSIAZEKWYPOZYCZONYCH);
        ifstream plikListaOsobOczekujacych(NAZWAOCZEKUJACYCHNAKSIAZKI);

        if (!plikDostepneKsiazki.is_open() || !plikWypozyczoneKsiazki.is_open() || !plikListaOsobOczekujacych.is_open()) {
            cerr << "Nie udalo sie otworzyc plikow do odczytu." << endl;
            return 1;
        }

        string linia;
        bool znaleziono = false;

        // Sprawdź dostępność w bazie książek dostępnych
        while (getline(plikDostepneKsiazki, linia)) {
            if (tytul == linia) {
                cout << endl << "Ksiazka:\"" << tytul << "\" jest dostepna w bazie." << endl << endl;

                // Zapytaj czy chce wypożyczyć
                char wybor;
                int idO = 0;

                cout << "Czy chcesz wypozyczyc te ksiazke? (T/N): ";
                cin >> wybor;

                if (toupper(wybor) == 'T') {
                    // Usuń z bazy dostępnych
                    S.usunWiersz(NAZWABAZYKSIAZEKDOSTEPNYCH, tytul);

                    // Dodaj ksiazke do listy wypożyczonych
                    S.dodajWiersz(NAZWAKSIAZEKWYPOZYCZONYCH, tytul);
                    
                    // Dodaj osobe na listę wypożyczających
                    cout << endl << "Podaj ID osoby ktora wypozycza." << endl;
                    cin >> idO;
                    while (!S.sprawdzPoprawnoscID(idO,IDnast)) {
                        cout << "ID niepoprawne. Sprobuj jeszcze raz." << endl;
                        cin >> idO;
                    }
                    S.dodajWiersz(NAZWAOSOBWYOPOZYCZAJACYCH, idO);
                    cout << endl << "Ksiazka \"" << tytul << "\" zostala wypozyczona dla osoby od ID: " << idO << endl << endl;

                    znaleziono = true;
                    break;

                }
                znaleziono = true;
                break;
            }
        }

        // Sprawdź dostępność na liście wypożyczonych
        if (!znaleziono) {
            while (getline(plikWypozyczoneKsiazki, linia)) {
                if (tytul == linia) {
                    cout << endl << "Ksiazka: \"" << tytul << "\" jest juz wypozyczona." << endl << endl;

                    // Zapytaj czy chce dodać na listę oczekujących
                    char wybor;
                    cout << "Czy chcesz dodac klienta na liste oczekujacych? (T/N): ";
                    cin >> wybor;

                    if (toupper(wybor) == 'T') {
                        // Dodaj na listę oczekujących
                        cout << endl << "Podaj ID osoby ktora wpisac do listy." << endl;
                        int idO;
                        cin >> idO;
                        while (!S.sprawdzPoprawnoscID(idO,IDnast)) {
                            cout << "ID niepoprawne. Sprobuj jeszcze raz." << endl;
                            cin >> idO;
                        }
                        S.dodajWiersz(NAZWAOCZEKUJACYCHNAKSIAZKI, idO);

                        cout << endl << "Osoba o ID: " << idO << " zostala dodana do liste oczekujacych na ksiazke: \"" << tytul <<"\"" << endl << endl;
                    }

                    znaleziono = true;
                    break;
                }
            }
        }

        if (!znaleziono) {
            cout << endl << "Ksiazki: \"" << tytul << " \" nie ma w bazie." << endl << endl;
        }

        plikDostepneKsiazki.close();
        plikWypozyczoneKsiazki.close();
        plikListaOsobOczekujacych.close();
        return 0;
    }

    int oddajKsiazke() {
        string tytul;
        int idO;
        cout << "Podaj tytul oddawanej lektury:" << endl;
        cin.ignore();
        getline(cin, tytul);

        ifstream plikWypozyczoneKsiazki(NAZWAKSIAZEKWYPOZYCZONYCH);
        ifstream plikListaOsobOczekujacych(NAZWAOCZEKUJACYCHNAKSIAZKI);

        if (!plikWypozyczoneKsiazki.is_open() || !plikListaOsobOczekujacych.is_open()) {
            cerr << "Nie udalo sie otworzyc plikow do odczytu." << endl;
            return 1;
        }

        string linia;
        bool znaleziono = false;

        // Sprawdź dostępność na liście wypożyczonych
        while (getline(plikWypozyczoneKsiazki, linia)) {
            if (tytul == linia) {
                cout << "Podaj ID osoby oddajacej lekture:" << endl;
                cin >> idO;
                while (!S.sprawdzPoprawnoscID(idO,IDnast)) {
                    cout << "ID niepoprawne. Sprobuj jeszcze raz." << endl;
                    cin >> idO;
                }
                // Usuń z listy wypożyczonych
                S.usunWiersz(NAZWAKSIAZEKWYPOZYCZONYCH, tytul);

                // Sprawdź listę wypozyczajacych
                S.usunWiersz(NAZWAOSOBWYOPOZYCZAJACYCH, idO);

                S.dodajWiersz(NAZWABAZYKSIAZEKDOSTEPNYCH, tytul);
                cout << "Ksiazka \"" << tytul << "\" zostala oddana przez osobe o ID: " << idO << endl;
                cout << endl << "Powiadomienie dla osob na liscie:" << endl;
                while (getline(plikListaOsobOczekujacych, linia)) {
                    cout << linia << ", ";
                }
                cout << endl << "Ksiazka: \"" << tytul << "\" jest juz dostepna." << endl << endl;
                znaleziono = true;
                break;
            }
        }

        if (!znaleziono) {
            cout << "Nie ma takiej ksiazki na liscie wypozyczonych." << endl << endl;
        }

        plikWypozyczoneKsiazki.close();
        plikListaOsobOczekujacych.close();
        return 0;
    }
};

int main() {
    Aplikacja& apl = Aplikacja::getInstance();

   // Aplikacja apl;

    int wybor;

    do {
        cout << "Menu:" << endl;
        cout << "1. Dodaj czytelnika" << endl;
        cout << "2. Wyswietl dostepnosc ksiazki" << endl;
        cout << "3. Zwrot ksiazki" << endl;
        cout << "0. Wyjdz" << endl;
        cout << "Wybierz opcje: ";
        bool czy_spelnia_warunek;
        string liczba;
        do
        {
            czy_spelnia_warunek = true;
            cin >> liczba;

            if (liczba.length() > 1)
                czy_spelnia_warunek = false;

            if (!isdigit(liczba[0]))
                czy_spelnia_warunek = false;

            if (czy_spelnia_warunek == true) {
                wybor = stoi(liczba);

                if (wybor < 0 || wybor > 3)
                    czy_spelnia_warunek = false;
            }

            if (!czy_spelnia_warunek)
                cout << "Podano niepoprawna wartosc. Sprobuj jeszcze raz." << endl;

        } while (!czy_spelnia_warunek);


        switch (wybor) {
        case 1:
            system("cls"); 
            apl.dodajCzytelnika();
            break;
        case 2:
            system("cls");
            apl.wyswietlDostepnoscKsiazki();
            break;
        case 3:
            system("cls");
            apl.oddajKsiazke();
            break;
        case 0:
            system("cls");
            cout << "Zakonczono program." << endl;
            break;
        default:
            cout << endl << "Nieprawidlowa opcja. Sprobuj ponownie." << endl;
        }

    } while (wybor != 0);

    return 0;
}