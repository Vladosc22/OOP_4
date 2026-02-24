#include <iostream>
#include <string>
#include <regex>
#include <cstring>
using namespace std;

class Client;
class Sala;

enum StareAbonament { NEPROCURAT, PROCURAT, ACTIV, EXPIRAT };

bool dataValida(const string& data) {
    regex format(R"(\d{4}-\d{2}-\d{2})");
    return regex_match(data, format);
}

// ======================== CLASA ABONAMENT ========================
class Abonament {
private:
    string tip;
    int durata;
    double pret;
    StareAbonament stare;
    string dataProcurarii;
    string dataActivarii;
    string dataExpirarii;

public:
    Abonament() : tip("Standard"), durata(1), pret(0.0), stare(NEPROCURAT) {}
    Abonament(string t, int d, double p)
        : tip(t), durata(d), pret(p), stare(NEPROCURAT) {}

    // Getteri
    string getTip() const { return tip; }
    int getDurata() const { return durata; }
    double getPret() const { return pret; }
    StareAbonament getStare() const { return stare; }
    string getDataProcurarii() const { return dataProcurarii; }
    string getDataActivarii() const { return dataActivarii; }
    string getDataExpirarii() const { return dataExpirarii; }

    // Setteri
    void setTip(const string& t) {
        if (!t.empty()) tip = t;
        else cout << "Tip invalid!" << endl;
    }

    void setDurata(int d) {
        if (d > 0 && d <= 24) durata = d;
        else cout << "Durată invalidă (1-24 luni)!" << endl;
    }

    void setPret(double p) {
        if (p > 0) pret = p;
        else cout << "Preț invalid!" << endl;
    }

    void setDataProcurarii(const string& d) {
        if (dataValida(d)) dataProcurarii = d;
        else cout << "Format dată invalid! (YYYY-MM-DD)" << endl;
    }

    void setDataActivarii(const string& d) {
        if (dataValida(d)) dataActivarii = d;
        else cout << "Format dată invalid! (YYYY-MM-DD)" << endl;
    }

    void setDataExpirarii(const string& d) {
        if (dataValida(d)) dataExpirarii = d;
        else cout << "Format dată invalid! (YYYY-MM-DD)" << endl;
    }

    // Metode stare
    void procura(const string& data) {
        stare = PROCURAT;
        setDataProcurarii(data);
    }

    void activeaza(const string& dataA, const string& dataE) {
        stare = ACTIV;
        setDataActivarii(dataA);
        setDataExpirarii(dataE);
    }

    void expira() { stare = EXPIRAT; }
    bool esteActiv() const { return stare == ACTIV; }

    // ========== OPERATORI SUPRAINCARCAȚI ==========

    // Operator + : Extindere durată abonament
    // Permite adăugarea de luni suplimentare la abonament
    Abonament operator+(int luniExtra) const {
        Abonament nou = *this;
        nou.durata = durata + luniExtra;
        nou.pret = pret + (luniExtra * (pret / durata)); // Pret proportional
        return nou;
    }

    // Operator == : Comparare abonamente
    // Verifică dacă două abonamente sunt de același tip și durată
    bool operator==(const Abonament& other) const {
        return (tip == other.tip && durata == other.durata);
    }

    // Operator > : Comparare după preț
    // Util pentru sortare sau găsirea celui mai scump abonament
    bool operator>(const Abonament& other) const {
        return pret > other.pret;
    }

    // Operator << : Afișare în flux
    // Permite afișarea elegantă a abonamentului: cout << abonament;
    friend ostream& operator<<(ostream& os, const Abonament& ab) {
        os << "Abonament " << ab.tip << " (" << ab.durata << " luni, "
           << ab.pret << " MDL) - Stare: ";
        switch (ab.stare) {
            case NEPROCURAT: os << "NEPROCURAT"; break;
            case PROCURAT: os << "PROCURAT"; break;
            case ACTIV: os << "ACTIV"; break;
            case EXPIRAT: os << "EXPIRAT"; break;
        }
        return os;
    }

    void afiseazaDetalii() const {
        cout << "   Tip: " << tip << endl;
        cout << "   Durată: " << durata << " luni" << endl;
        cout << "   Preț: " << pret << " MDL" << endl;
        cout << "   Stare: ";
        switch (stare) {
            case NEPROCURAT: cout << "NEPROCURAT"; break;
            case PROCURAT: cout << "PROCURAT"; break;
            case ACTIV: cout << "ACTIV"; break;
            case EXPIRAT: cout << "EXPIRAT"; break;
        }
        cout << endl;
        if (!dataProcurarii.empty()) cout << "   Data procurării: " << dataProcurarii << endl;
        if (!dataActivarii.empty()) cout << "   Data activării: " << dataActivarii << endl;
        if (!dataExpirarii.empty()) cout << "   Data expirării: " << dataExpirarii << endl;
    }
};

// ======================== CLASA CLIENT ========================
class Client {
private:
    string nume;
    string telefon;
    string parola;
    int varsta;
    double sold;
    Abonament abonament;

public:
    Client() : nume(""), telefon(""), parola(""), varsta(0), sold(0.0), abonament() {}
    Client(string n, string t, string p, int v)
        : nume(n), telefon(t), parola(p), varsta(v), sold(0.0), abonament() {}

    // Getteri
    string getNume() const { return nume; }
    string getTelefon() const { return telefon; }
    int getVarsta() const { return varsta; }
    double getSold() const { return sold; }
    const Abonament& getAbonament() const { return abonament; }

    void setNume(const string& n) {
        if (!n.empty()) nume = n;
        else cout << "Nume invalid!" << endl;
    }

    void setVarsta(int v) {
        if (v > 0 && v < 120) varsta = v;
        else cout << "Vârstă invalidă!" << endl;
    }

    // Metode pentru sold
    void adaugaSold(double suma) {
        if (suma > 0) {
            sold += suma;
            cout << "Sold adăugat! Sold curent: " << sold << " MDL" << endl;
        } else cout << "Suma invalidă!" << endl;
    }

    bool scadeSold(double suma) {
        if (suma <= sold) {
            sold -= suma;
            return true;
        }
        return false;
    }

    // Metode pentru abonament
    void procuraAbonament(const Abonament& ab, const string& data) {
        abonament = ab;
        abonament.procura(data);
        cout << "Abonament procurat cu succes!" << endl;
    }

    bool activeazaAbonament(const string& dataA, const string& dataE) {
        if (abonament.getStare() != PROCURAT) {
            cout << "Nu aveți un abonament procurat!" << endl;
            return false;
        }

        double pret = abonament.getPret();
        if (sold >= pret) {
            sold -= pret;
            abonament.activeaza(dataA, dataE);
            cout << "Abonament activat cu succes!" << endl;
            cout << "Suma dedusă: " << pret << " MDL" << endl;
            cout << "Sold rămas: " << sold << " MDL" << endl;
            return true;
        } else {
            cout << "Sold insuficient! (" << sold << " / " << pret << ")" << endl;
            return false;
        }
    }

    // Parolă
    bool verificaParola(const string& p) const { return parola == p; }

    void schimbaParola(const string& parolaVeche, const string& parolaNoua) {
        if (verificaParola(parolaVeche)) {
            parola = parolaNoua;
            cout << "Parola schimbată cu succes!" << endl;
        } else {
            cout << "Parola veche incorectă!" << endl;
        }
    }

    // ========== OPERATORI SUPRAINCARCAȚI ==========

    // Operator += : Adăugare sold
    // Permite adăugarea rapidă de fonduri: client += 100;
    Client& operator+=(double suma) {
        if (suma > 0) {
            sold += suma;
        }
        return *this;
    }

    // Operator -= : Scădere sold
    // Permite scăderea fondurilor: client -= 50;
    Client& operator-=(double suma) {
        if (suma > 0 && suma <= sold) {
            sold -= suma;
        }
        return *this;
    }

    // Operator < : Comparare după vârstă
    // Util pentru sortări sau verificări de vârstă
    bool operator<(const Client& other) const {
        return varsta < other.varsta;
    }

    // Operator == : Comparare clienți
    // Verifică dacă doi clienți au același telefon (identificator unic)
    bool operator==(const Client& other) const {
        return telefon == other.telefon;
    }

    // Operator << : Afișare în flux
    // Permite afișarea: cout << client;
    friend ostream& operator<<(ostream& os, const Client& c) {
        os << c.nume << " (" << c.telefon << ") - " << c.varsta
           << " ani, Sold: " << c.sold << " MDL";
        return os;
    }

    // Operator >> : Citire din flux
    // Permite citirea: cin >> client;
    friend istream& operator>>(istream& is, Client& c) {
        cout << "Nume: ";
        is.ignore();
        getline(is, c.nume);
        cout << "Telefon: ";
        is >> c.telefon;
        cout << "Vârstă: ";
        is >> c.varsta;
        return is;
    }

    void afiseazaProfil() const {
        cout << "\n========== PROFIL CLIENT ==========" << endl;
        cout << "Nume: " << nume << endl;
        cout << "Telefon: " << telefon << endl;
        cout << "Vârstă: " << varsta << " ani" << endl;
        cout << "Sold: " << sold << " MDL" << endl;
        cout << "\nDetalii Abonament:" << endl;
        abonament.afiseazaDetalii();
        cout << "===================================" << endl;
    }

    friend class Sala;
};

// ======================== CLASA SALA ========================
class Sala {
private:
    static const int MAX_CLIENTI = 100;
    Client clienti[MAX_CLIENTI];
    int numarClienti;
    string numeSala;

public:
    Sala(string nume = "FitZone") : numarClienti(0), numeSala(nume) {}

    bool inregistreazaClient(const string& nume, const string& telefon,
                             const string& parola, int varsta) {
        regex telefonRegex(R"((\+373\d{8}|0\d{8}))");
        if (!regex_match(telefon, telefonRegex)) {
            cout << "Număr de telefon invalid!" << endl;
            return false;
        }
        for (int i = 0; i < numarClienti; i++)
            if (clienti[i].getTelefon() == telefon) {
                cout << "Telefon deja folosit!" << endl;
                return false;
            }

        if (numarClienti >= MAX_CLIENTI) {
            cout << "Capacitate maximă atinsă!" << endl;
            return false;
        }

        if (varsta < 14 || varsta > 100) {
            cout << "Vârstă invalidă!" << endl;
            return false;
        }

        clienti[numarClienti] = Client(nume, telefon, parola, varsta);
        numarClienti++;
        cout << "Client înregistrat cu succes!" << endl;
        return true;
    }

    Client* autentificare(const string& telefon, const string& parola) {
        for (int i = 0; i < numarClienti; i++) {
            if (clienti[i].getTelefon() == telefon) {
                if (clienti[i].verificaParola(parola)) {
                    cout << "Autentificare reușită! Bun venit, " << clienti[i].getNume() << "!\n";
                    return &clienti[i];
                } else {
                    cout << "Parolă incorectă!" << endl;
                    return nullptr;
                }
            }
        }
        cout << "Client inexistent!" << endl;
        return nullptr;
    }

    void afiseazaTotiClientii() const {
        if (numarClienti == 0) {
            cout << "Nu există clienți înregistrați." << endl;
            return;
        }
        cout << "\n=== LISTA CLIENTI ===" << endl;
        for (int i = 0; i < numarClienti; i++)
            cout << i + 1 << ". " << clienti[i] << endl; // Folosim operator<<
    }

    void ofertaSpeciala(const string& telefon, double procentReducere) {
        for (int i = 0; i < numarClienti; i++) {
            if (clienti[i].getTelefon() == telefon) {
                double bonus = 100.0 * (procentReducere / 100.0);
                clienti[i] += bonus; // Folosim operator+=
                cout << "Bonus aplicat: " << bonus << " MDL. Sold nou: "
                     << clienti[i].getSold() << endl;
                return;
            }
        }
        cout << "Client negăsit!" << endl;
    }

    // ========== OPERATORI SUPRAINCARCAȚI ==========

    // Operator [] : Accesare client după index
    // Permite accesul rapid: sala[0] pentru primul client
    Client& operator[](int index) {
        if (index >= 0 && index < numarClienti) {
            return clienti[index];
        }
        throw out_of_range("Index invalid!");
    }

    // Operator + : Adăugare client nou
    // Permite adăugarea: sala = sala + clientNou;
    Sala operator+(const Client& client) {
        Sala nouaSala = *this;
        if (nouaSala.numarClienti < MAX_CLIENTI) {
            nouaSala.clienti[nouaSala.numarClienti] = client;
            nouaSala.numarClienti++;
        }
        return nouaSala;
    }

    // Operator += : Adăugare client direct
    // Permite adăugarea: sala += clientNou;
    Sala& operator+=(const Client& client) {
        if (numarClienti < MAX_CLIENTI) {
            clienti[numarClienti] = client;
            numarClienti++;
        }
        return *this;
    }

    // Operator () : Căutare client după telefon (functor)
    // Permite căutarea: sala("069123456")
    Client* operator()(const string& telefon) {
        for (int i = 0; i < numarClienti; i++) {
            if (clienti[i].getTelefon() == telefon) {
                return &clienti[i];
            }
        }
        return nullptr;
    }

    // Operator ! : Verificare dacă sala e plină
    // Permite verificarea: if (!sala) { ... }
    bool operator!() const {
        return numarClienti >= MAX_CLIENTI;
    }

    // Operator int() : Conversie la int (număr de clienți)
    // Permite: int total = (int)sala;
    operator int() const {
        return numarClienti;
    }

    int getNumarClienti() const { return numarClienti; }
};

void afiseazaAbonamenteDisponibile() {
    cout << "\n========== ABONAMENTE DISPONIBILE ==========" << endl;
    cout << "1. Standard   - 1 lună  - 300 MDL" << endl;
    cout << "2. Premium    - 3 luni  - 800 MDL" << endl;
    cout << "3. VIP        - 6 luni  - 1500 MDL" << endl;
    cout << "4. Nelimitat  - 12 luni - 2500 MDL" << endl;
    cout << "5. Part-Time  - 1 lună  - 200 MDL (10:00-16:00)" << endl;
    cout << "===========================================" << endl;
}

Abonament alegereAbonament() {
    afiseazaAbonamenteDisponibile();
    int alegere;
    cout << "Alege abonament (1-5): ";
    cin >> alegere;

    switch (alegere) {
        case 1: return Abonament("Standard", 1, 300.0);
        case 2: return Abonament("Premium", 3, 800.0);
        case 3: return Abonament("VIP", 6, 1500.0);
        case 4: return Abonament("Nelimitat", 12, 2500.0);
        case 5: return Abonament("Part-Time", 1, 200.0);
        default:
            cout << "Opțiune invalidă! Standard ales implicit.\n";
            return Abonament("Standard", 1, 300.0);
    }
}

void meniuTestareOperatori(Client* client, Sala& sala) {
    int optiune;
    do {
        cout << "\n========== TESTARE OPERATORI ==========" << endl;
        cout << "1. Test operator += (adăugare sold)" << endl;
        cout << "2. Test operator -= (scădere sold)" << endl;
        cout << "3. Test operator << (afișare client)" << endl;
        cout << "4. Test operator + (extindere abonament)" << endl;
        cout << "5. Test operator == (comparare abonamente)" << endl;
        cout << "6. Test operator [] (acces client din sală)" << endl;
        cout << "7. Test operator () (căutare client)" << endl;
        cout << "8. Test operator int() (număr clienți)" << endl;
        cout << "9. Înapoi" << endl;
        cout << "=======================================" << endl;
        cout << "Alege opțiunea: ";
        cin >> optiune;

        switch (optiune) {
            case 1: {
                double suma;
                cout << "Suma de adăugat: ";
                cin >> suma;
                *client += suma;
                cout << "Sold nou: " << client->getSold() << " MDL" << endl;
                break;
            }
            case 2: {
                double suma;
                cout << "Suma de scăzut: ";
                cin >> suma;
                *client -= suma;
                cout << "Sold nou: " << client->getSold() << " MDL" << endl;
                break;
            }
            case 3: {
                cout << "Afișare client: " << *client << endl;
                break;
            }
            case 4: {
                int luni;
                cout << "Câte luni să adăugăm la abonament? ";
                cin >> luni;
                Abonament abonamentExtins = client->getAbonament() + luni;
                cout << "Abonament extins: " << abonamentExtins << endl;
                break;
            }
            case 5: {
                Abonament ab1 = client->getAbonament();
                Abonament ab2("Standard", 1, 300.0);
                if (ab1 == ab2) {
                    cout << "Abonamentele sunt identice!" << endl;
                } else {
                    cout << "Abonamentele sunt diferite!" << endl;
                }
                break;
            }
            case 6: {
                int index;
                cout << "Index client (0-" << (int)sala - 1 << "): ";
                cin >> index;
                try {
                    Client& c = sala[index];
                    cout << "Client găsit: " << c << endl;
                } catch (out_of_range& e) {
                    cout << "Eroare: " << e.what() << endl;
                }
                break;
            }
            case 7: {
                string telefon;
                cout << "Telefon de căutat: ";
                cin >> telefon;
                Client* c = sala(telefon);
                if (c) {
                    cout << "Client găsit: " << *c << endl;
                } else {
                    cout << "Client negăsit!" << endl;
                }
                break;
            }
            case 8: {
                cout << "Număr total clienți: " << (int)sala << endl;
                if (!sala) {
                    cout << "Sala este plină!" << endl;
                } else {
                    cout << "Mai sunt locuri disponibile." << endl;
                }
                break;
            }
            case 9:
                cout << "Înapoi la meniu...\n";
                break;
            default:
                cout << "Opțiune invalidă!\n";
                break;
        }
    } while (optiune != 9);
}

void meniuClient(Client* client, Sala& sala) {
    int optiune;
    do {
        cout << "\n========== MENIU CLIENT ==========" << endl;
        cout << "1. Vizualizează profil" << endl;
        cout << "2. Adaugă sold" << endl;
        cout << "3. Procură abonament" << endl;
        cout << "4. Activează abonament" << endl;
        cout << "5. Schimbă parolă" << endl;
        cout << "6. Modifică date personale" << endl;
        cout << "7. Testare operatori" << endl;
        cout << "8. Deconectare" << endl;
        cout << "==================================" << endl;
        cout << "Alege opțiunea: ";
        cin >> optiune;

        switch (optiune) {
            case 1:
                client->afiseazaProfil();
                break;
            case 2: {
                double suma;
                cout << "Suma de adăugat: ";
                cin >> suma;
                client->adaugaSold(suma);
                break;
            }
            case 3: {
                Abonament ab = alegereAbonament();
                string data;
                cout << "Introdu data procurării (YYYY-MM-DD): ";
                cin >> data;
                client->procuraAbonament(ab, data);
                break;
            }
            case 4: {
                string dA, dE;
                cout << "Data activării (YYYY-MM-DD): ";
                cin >> dA;
                cout << "Data expirării (YYYY-MM-DD): ";
                cin >> dE;
                client->activeazaAbonament(dA, dE);
                break;
            }
            case 5: {
                string veche, noua;
                cout << "Parola veche: ";
                cin >> veche;
                cout << "Parola nouă: ";
                cin >> noua;
                client->schimbaParola(veche, noua);
                break;
            }
            case 6: {
                string n;
                int v;
                cout << "Nume nou: ";
                cin.ignore();
                getline(cin, n);
                cout << "Vârstă nouă: ";
                cin >> v;
                client->setNume(n);
                client->setVarsta(v);
                break;
            }
            case 7:
                meniuTestareOperatori(client, sala);
                break;
            case 8:
                cout << "Deconectare...\n";
                break;
            default:
                cout << "Opțiune invalidă!\n";
                break;
        }
    } while (optiune != 8);
}

void meniuPrincipal() {
    Sala sala("FitZone Premium");
    int optiune;

    do {
        cout << "\n========== MENIU PRINCIPAL ==========" << endl;
        cout << "1. Înregistrare client nou" << endl;
        cout << "2. Autentificare" << endl;
        cout << "3. Afișează toți clienții" << endl;
        cout << "4. Ofertă specială (admin)" << endl;
        cout << "5. Ieșire" << endl;
        cout << "=====================================" << endl;
        cout << "Alege opțiunea: ";
        cin >> optiune;

        switch (optiune) {
            case 1: {
                string nume, telefon, parola;
                int varsta;
                cout << "\nNume: ";
                cin.ignore();
                getline(cin, nume);
                cout << "Telefon: ";
                cin >> telefon;
                cout << "Parola: ";
                cin >> parola;
                cout << "Vârstă: ";
                cin >> varsta;
                sala.inregistreazaClient(nume, telefon, parola, varsta);
                break;
            }
            case 2: {
                string telefon, parola;
                cout << "\nTelefon: ";
                cin >> telefon;
                cout << "Parola: ";
                cin >> parola;
                Client* c = sala.autentificare(telefon, parola);
                if (c) meniuClient(c, sala);
                break;
            }
            case 3:
                sala.afiseazaTotiClientii();
                break;
            case 4: {
                string tel;
                double p;
                cout << "Telefon client: ";
                cin >> tel;
                cout << "Procent bonus: ";
                cin >> p;
                sala.ofertaSpeciala(tel, p);
                break;
            }
            case 5:
                cout << "La revedere!\n";
                break;
            default:
                cout << "Opțiune invalidă!\n";
                break;
        }
    } while (optiune != 5);
}

int main() {
    meniuPrincipal();
    return 0;
}

// Element
// 🔹 Când
// Când vrem ca operatorii să funcționeze pe clasele noastre
// 🔹 Cum
// Ca metodă membră sau funcție friend
// 🔹 Reguli
// Nu se schimbă nr. operanzilor, prioritatea, sau asociativitatea
// 🔹 Nu se pot supraincarca
// ::, ., .*, sizeof, alignof
// 🔹 Operanz
// Stânga = obiectul apelant (this), Dreapta = argumentul primit
