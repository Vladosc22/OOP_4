#include <iostream>
#include <string>
#include <regex>
#include <vector>
using namespace std;


enum StareAbonament { NEPROCURAT, PROCURAT, ACTIV, EXPIRAT };

bool dataValida(const string& data) {
    regex format(R"(\d{4}-\d{2}-\d{2})");
    return regex_match(data, format);
}

class Abonament {
protected:
    string tip;
    int durata;
    double pret;
    StareAbonament stare;
    string dataProcurarii;
    string dataActivarii;
    string dataExpirarii;

public:
    Abonament()
        : tip("Standard"), durata(1), pret(0.0), stare(NEPROCURAT) {}

    Abonament(string t, int d, double p)
        : tip(t), durata(d), pret(p), stare(NEPROCURAT) {}

    virtual ~Abonament() {}

    string getTip() const { return tip; }
    int getDurata() const { return durata; }
    double getPret() const { return pret; }
    StareAbonament getStare() const { return stare; }
    string getDataProcurarii() const { return dataProcurarii; }
    string getDataActivarii() const { return dataActivarii; }
    string getDataExpirarii() const { return dataExpirarii; }

    void procura(const string& data) {
        if (dataValida(data)) {
            stare = PROCURAT;
            dataProcurarii = data;
        } else {
            cout << "Format dată invalid! (YYYY-MM-DD)" << endl;
        }
    }

    void activeaza(const string& dataA, const string& dataE) {
        if (dataValida(dataA) && dataValida(dataE)) {
            stare = ACTIV;
            dataActivarii = dataA;
            dataExpirarii = dataE;
        } else {
            cout << "Format dată invalid! (YYYY-MM-DD)" << endl;
        }
    }

    void expira() { stare = EXPIRAT; }
    bool esteActiv() const { return stare == ACTIV; }

    virtual double calculeazaPretDiscount(double procentDiscount) const = 0;
    virtual string afiseazaTip() const = 0;
    virtual void afiseazaBeneficii() const = 0;
    virtual Abonament* clone() const = 0;

    bool operator==(const Abonament& other) const {
        return (tip == other.tip && durata == other.durata);
    }

    bool operator>(const Abonament& other) const {
        return pret > other.pret;
    }

    friend ostream& operator<<(ostream& os, const Abonament& ab) {
        os << "Abonament " << ab.tip << " (" << ab.durata << " luni, "
           << ab.pret << " MDL) - Stare: ";
        switch (ab.stare) {
            case NEPROCURAT: os << "NEPROCURAT"; break;
            case PROCURAT:   os << "PROCURAT";   break;
            case ACTIV:      os << "ACTIV";      break;
            case EXPIRAT:    os << "EXPIRAT";    break;
        }
        return os;
    }

    virtual void afiseazaDetalii() const {
        cout << "   Tip: " << tip << endl;
        cout << "   Durată: " << durata << " luni" << endl;
        cout << "   Preț: " << pret << " MDL" << endl;
        cout << "   Stare: ";
        switch (stare) {
            case NEPROCURAT: cout << "NEPROCURAT"; break;
            case PROCURAT:   cout << "PROCURAT";   break;
            case ACTIV:      cout << "ACTIV";      break;
            case EXPIRAT:    cout << "EXPIRAT";    break;
        }
        cout << endl;
        if (!dataProcurarii.empty()) cout << "   Data procurării: "  << dataProcurarii << endl;
        if (!dataActivarii.empty())  cout << "   Data activării: "   << dataActivarii  << endl;
        if (!dataExpirarii.empty())  cout << "   Data expirării: "   << dataExpirarii  << endl;
    }
};

class AbonamentFitness : public Abonament {
private:
    bool accesSauna;
    bool accesHidroterapie;

public:
    AbonamentFitness()
        : Abonament("Fitness", 1, 300.0), accesSauna(true), accesHidroterapie(false) {}

    AbonamentFitness(int d, double p)
        : Abonament("Fitness", d, p), accesSauna(true), accesHidroterapie(false) {}

    virtual ~AbonamentFitness() {}

    double calculeazaPretDiscount(double procentDiscount) const override {
        return pret * (1.0 - procentDiscount / 100.0);
    }

    string afiseazaTip() const override {
        return "AbonamentFitness - Acces complet sala";
    }

    void afiseazaBeneficii() const override {
        cout << "   + Acces complet la echipamente" << endl;
        cout << "   + Acces sauna" << endl;
        cout << "   + Consultatii cu antrenor (saptamanal)" << endl;
    }

    void afiseazaDetalii() const override {
        Abonament::afiseazaDetalii();
        cout << "   Beneficii speciale:" << endl;
        afiseazaBeneficii();
    }

    Abonament* clone() const override {
        return new AbonamentFitness(*this);
    }
};

class AbonamentPiscina : public Abonament {
private:
    int numarCursuriInot;
    bool accesAquaAerobics;

public:
    AbonamentPiscina()
        : Abonament("Piscina", 1, 250.0), numarCursuriInot(4), accesAquaAerobics(true) {}

    AbonamentPiscina(int d, double p)
        : Abonament("Piscina", d, p), numarCursuriInot(4), accesAquaAerobics(true) {}

    virtual ~AbonamentPiscina() {}

    double calculeazaPretDiscount(double procentDiscount) const override {
        return pret * (1.0 - procentDiscount / 100.0);
    }

    string afiseazaTip() const override {
        return "AbonamentPiscina - Acces la piscina cu cursuri";
    }

    void afiseazaBeneficii() const override {
        cout << "   + Acces nelimitat la piscina" << endl;
        cout << "   + " << numarCursuriInot << " cursuri de inot/luna" << endl;
        cout << "   + Acces la aqua aerobics" << endl;
    }

    void afiseazaDetalii() const override {
        Abonament::afiseazaDetalii();
        cout << "   Beneficii speciale:" << endl;
        afiseazaBeneficii();
    }

    Abonament* clone() const override {
        return new AbonamentPiscina(*this);
    }
};

class AbonamentCombinant : public Abonament {
private:
    int numarCursuriInot;
    int numarSesiuniAntrenor;

public:
    AbonamentCombinant()
        : Abonament("Combinant", 3, 600.0), numarCursuriInot(8), numarSesiuniAntrenor(2) {}

    AbonamentCombinant(int d, double p)
        : Abonament("Combinant", d, p), numarCursuriInot(8), numarSesiuniAntrenor(2) {}

    virtual ~AbonamentCombinant() {}

    double calculeazaPretDiscount(double procentDiscount) const override {
        return pret - (pret * procentDiscount / 100.0);
    }

    string afiseazaTip() const override {
        return "AbonamentCombinant - Acces complet (fitness + piscina)";
    }

    void afiseazaBeneficii() const override {
        cout << "   + Acces complet sala de fitness" << endl;
        cout << "   + Acces nelimitat la piscina" << endl;
        cout << "   + " << numarCursuriInot << " cursuri de inot/luna" << endl;
        cout << "   + " << numarSesiuniAntrenor << " sesiuni antrenor/saptamana" << endl;
        cout << "   + Sauna si hidroterapie" << endl;
    }

    void afiseazaDetalii() const override {
        Abonament::afiseazaDetalii();
        cout << "   Beneficii speciale:" << endl;
        afiseazaBeneficii();
    }

    Abonament* clone() const override {
        return new AbonamentCombinant(*this);
    }
};


class RecomandareAbonament {
public:
    virtual ~RecomandareAbonament() {}
    virtual Abonament* recomanda(int varsta, int nivel) = 0;
};

class MotorRecomandari : public RecomandareAbonament {
private:
    vector<Abonament*> abonamenteDisponibile;

public:
    MotorRecomandari() {
        abonamenteDisponibile.push_back(new AbonamentFitness(1, 300.0));
        abonamenteDisponibile.push_back(new AbonamentPiscina(1, 250.0));
        abonamenteDisponibile.push_back(new AbonamentCombinant(3, 600.0));
    }

    ~MotorRecomandari() {
        for (auto ab : abonamenteDisponibile)
            delete ab;
        abonamenteDisponibile.clear();
    }

    // FIX: return a clone so the caller can safely take ownership
    Abonament* recomanda(int varsta, int nivel) override {
        int idx = 0;
        if (varsta < 16) {
            cout << "   Recomandare: AbonamentPiscina (pentru tineri)" << endl;
            idx = 1;
        } else if (varsta < 30) {
            cout << "   Recomandare: AbonamentFitness (pentru adulti activi)" << endl;
            idx = 0;
        } else {
            cout << "   Recomandare: AbonamentCombinant (pentru cei experimentati)" << endl;
            idx = 2;
        }
        return abonamenteDisponibile[idx]->clone();
    }

    // FIX: const-correct view that does NOT clone; only used for display
    void afiseazaAbonamenteDisponibile() const {
        cout << "\n========== ABONAMENTE DISPONIBILE ==========" << endl;
        for (size_t i = 0; i < abonamenteDisponibile.size(); i++) {
            cout << i + 1 << ". " << abonamenteDisponibile[i]->afiseazaTip() << endl;
            cout << "   Pret: " << abonamenteDisponibile[i]->getPret() << " MDL" << endl;
        }
        cout << "==========================================" << endl;
    }
};


class Client {
private:
    string nume;
    string telefon;
    string parola;
    int varsta;
    double sold;
    Abonament* abonament;

public:
    // Default constructor
    Client()
        : nume(""), telefon(""), parola(""), varsta(0), sold(0.0), abonament(nullptr) {}

    Client(string n, string t, string p, int v)
        : nume(n), telefon(t), parola(p), varsta(v), sold(0.0), abonament(nullptr) {}

    // FIX: Copy constructor (Rule of Three) — deep-copy the subscription
    Client(const Client& other)
        : nume(other.nume), telefon(other.telefon), parola(other.parola),
          varsta(other.varsta), sold(other.sold),
          abonament(other.abonament ? other.abonament->clone() : nullptr) {}

    // FIX: Copy-assignment operator (Rule of Three)
    Client& operator=(const Client& other) {
        if (this == &other) return *this;
        delete abonament;
        nume      = other.nume;
        telefon   = other.telefon;
        parola    = other.parola;
        varsta    = other.varsta;
        sold      = other.sold;
        abonament = other.abonament ? other.abonament->clone() : nullptr;
        return *this;
    }

    ~Client() {
        delete abonament;
        abonament = nullptr;
    }

    string getNume()    const { return nume; }
    string getTelefon() const { return telefon; }
    int    getVarsta()  const { return varsta; }
    double getSold()    const { return sold; }
    const Abonament* getAbonament() const { return abonament; }

    void adaugaSold(double suma) {
        if (suma > 0) {
            sold += suma;
            cout << "Sold adaugat! Sold curent: " << sold << " MDL" << endl;
        } else {
            cout << "Suma invalida!" << endl;
        }
    }

    // Takes ownership of ab (caller must pass a heap-allocated object)
    void procuraAbonament(Abonament* ab, const string& data) {
        if (abonament) {
            cout << "   Abonament vechi sters, inlocuit cu noul abonament." << endl;
            delete abonament;
        }
        abonament = ab;
        abonament->procura(data);
        cout << "Abonament procurat cu succes!" << endl;
    }

    bool activeazaAbonament(const string& dataA, const string& dataE) {
        if (!abonament || abonament->getStare() != PROCURAT) {
            cout << "Nu aveti un abonament procurat!" << endl;
            return false;
        }

        double pret = abonament->getPret();
        if (sold >= pret) {
            sold -= pret;
            abonament->activeaza(dataA, dataE);
            cout << "Abonament activat cu succes!" << endl;
            cout << "   Suma dedusa: " << pret << " MDL" << endl;
            cout << "   Sold ramas: "  << sold << " MDL" << endl;
            return true;
        } else {
            cout << "Sold insuficient! (" << sold << " / " << pret << ")" << endl;
            return false;
        }
    }

    bool verificaParola(const string& p) const {
        return parola == p;
    }

    void schimbaParola(const string& parolaVeche, const string& parolaNoua) {
        if (verificaParola(parolaVeche)) {
            parola = parolaNoua;
            cout << "Parola schimbata cu succes!" << endl;
        } else {
            cout << "Parola veche incorecta!" << endl;
        }
    }

    Client& operator+=(double suma) {
        if (suma > 0) sold += suma;
        return *this;
    }

    Client& operator-=(double suma) {
        if (suma > 0 && suma <= sold) sold -= suma;
        return *this;
    }

    bool operator<(const Client& other) const { return varsta < other.varsta; }
    bool operator==(const Client& other) const { return telefon == other.telefon; }

    friend ostream& operator<<(ostream& os, const Client& c) {
        os << c.nume << " (" << c.telefon << ") - " << c.varsta
           << " ani, Sold: " << c.sold << " MDL";
        return os;
    }

    void afiseazaProfil() const {
        cout << "\n========== PROFIL CLIENT ==========" << endl;
        cout << "Nume: "    << nume    << endl;
        cout << "Telefon: " << telefon << endl;
        cout << "Varsta: "  << varsta  << " ani" << endl;
        cout << "Sold: "    << sold    << " MDL" << endl;
        cout << "\nDetalii Abonament:" << endl;
        if (abonament) {
            abonament->afiseazaDetalii();
        } else {
            cout << "   Niciun abonament activ!" << endl;
        }
        cout << "===================================" << endl;
    }

    friend class Sala;
};

class Sala {
private:
    static const int MAX_CLIENTI = 100;
    Client clienti[MAX_CLIENTI];   // safe now that Client has proper copy semantics
    int numarClienti;
    string numeSala;
    MotorRecomandari motorRecomandari;

public:
    Sala(string nume = "FitZone") : numarClienti(0), numeSala(nume) {}
    ~Sala() {}

    bool inregistreazaClient(const string& nume, const string& telefon,
                             const string& parola, int varsta) {
        regex telefonRegex(R"((\+373\d{8}|0\d{8}))");
        if (!regex_match(telefon, telefonRegex)) {
            cout << "Numar de telefon invalid! (Format: 0XXXXXXXXX sau +373XXXXXXXXX)" << endl;
            return false;
        }
        for (int i = 0; i < numarClienti; i++) {
            if (clienti[i].getTelefon() == telefon) {
                cout << "Telefon deja folosit!" << endl;
                return false;
            }
        }
        if (numarClienti >= MAX_CLIENTI) {
            cout << "Capacitate maxima atinsa!" << endl;
            return false;
        }
        if (varsta < 14 || varsta > 100) {
            cout << "Varsta invalida!" << endl;
            return false;
        }

        clienti[numarClienti] = Client(nume, telefon, parola, varsta);
        numarClienti++;
        cout << "Client inregistrat cu succes!" << endl;
        return true;
    }

    Client* autentificare(const string& telefon, const string& parola) {
        for (int i = 0; i < numarClienti; i++) {
            if (clienti[i].getTelefon() == telefon) {
                if (clienti[i].verificaParola(parola)) {
                    cout << "Autentificare reusita! Bun venit, "
                         << clienti[i].getNume() << "!\n";
                    return &clienti[i];
                } else {
                    cout << "Parola incorecta!" << endl;
                    return nullptr;
                }
            }
        }
        cout << "Client inexistent!" << endl;
        return nullptr;
    }

    void afiseazaTotiClientii() const {
        if (numarClienti == 0) {
            cout << "Nu exista clienti inregistrati." << endl;
            return;
        }
        cout << "\n=== LISTA CLIENTI ===" << endl;
        for (int i = 0; i < numarClienti; i++)
            cout << i + 1 << ". " << clienti[i] << endl;
    }

    void ofertaSpeciala(const string& telefon, double procentReducere) {
        for (int i = 0; i < numarClienti; i++) {
            if (clienti[i].getTelefon() == telefon) {
                double bonus = 100.0 * (procentReducere / 100.0);
                clienti[i] += bonus;
                cout << "Bonus aplicat: " << bonus << " MDL. Sold nou: "
                     << clienti[i].getSold() << endl;
                return;
            }
        }
        cout << "Client negasit!" << endl;
    }

    // FIX: recomanda() now returns a clone; we display it then delete it here
    void recomandaAbonament(const string& telefon) {
        for (int i = 0; i < numarClienti; i++) {
            if (clienti[i].getTelefon() == telefon) {
                cout << "\nRECOMANDARE AUTOMATA:" << endl;
                Abonament* ab = motorRecomandari.recomanda(clienti[i].getVarsta(), 0);
                cout << "   Tip: " << ab->afiseazaTip() << endl;
                cout << "   Beneficii:" << endl;
                ab->afiseazaBeneficii();
                delete ab;   // clean up the clone
                return;
            }
        }
        cout << "Client negasit!" << endl;
    }

    Client& operator[](int index) {
        if (index >= 0 && index < numarClienti)
            return clienti[index];
        throw out_of_range("Index invalid!");
    }

    Client* operator()(const string& telefon) {
        for (int i = 0; i < numarClienti; i++)
            if (clienti[i].getTelefon() == telefon)
                return &clienti[i];
        return nullptr;
    }

    bool operator!() const { return numarClienti >= MAX_CLIENTI; }
    operator int()   const { return numarClienti; }
    int getNumarClienti() const { return numarClienti; }

    void afiseazaAbonamentePentruProcurare() {
        motorRecomandari.afiseazaAbonamenteDisponibile();
    }
};

// ======================== MENIURI ========================
void meniuClient(Client* client, Sala& sala);
void meniuPrincipal();

void meniuClient(Client* client, Sala& sala) {
    int optiune;
    do {
        cout << "\n========== MENIU CLIENT ==========" << endl;
        cout << "1. Vizualizeaza profil"    << endl;
        cout << "2. Adauga sold"            << endl;
        cout << "3. Procura abonament"      << endl;
        cout << "4. Activeaza abonament"    << endl;
        cout << "5. Schimba parola"         << endl;
        cout << "6. Obtine recomandare"     << endl;
        cout << "7. Deconectare"            << endl;
        cout << "==================================" << endl;
        cout << "Alege optiunea: ";
        cin >> optiune;
        // FIX: always flush leftover newline after reading an int
        cin.ignore();

        switch (optiune) {
            case 1:
                client->afiseazaProfil();
                break;
            case 2: {
                double suma;
                cout << "Suma de adaugat: ";
                cin >> suma;
                cin.ignore();
                client->adaugaSold(suma);
                break;
            }
            case 3: {
                sala.afiseazaAbonamentePentruProcurare();
                cout << "Introdu numarul abonamentului (1-3): ";
                int alegere;
                cin >> alegere;
                cin.ignore();
                Abonament* ab = nullptr;
                switch (alegere) {
                    case 1: ab = new AbonamentFitness(1, 300.0);    break;
                    case 2: ab = new AbonamentPiscina(1, 250.0);    break;
                    case 3: ab = new AbonamentCombinant(3, 600.0);  break;
                    default: cout << "Optiune invalida!" << endl;   break;
                }
                if (ab) {
                    string data;
                    cout << "Introdu data procurarii (YYYY-MM-DD): ";
                    getline(cin, data);
                    client->procuraAbonament(ab, data);
                }
                break;
            }
            case 4: {
                string dA, dE;
                cout << "Data activarii  (YYYY-MM-DD): ";
                getline(cin, dA);
                cout << "Data expirarii  (YYYY-MM-DD): ";
                getline(cin, dE);
                client->activeazaAbonament(dA, dE);
                break;
            }
            case 5: {
                string veche, noua;
                cout << "Parola veche: ";
                getline(cin, veche);
                cout << "Parola noua: ";
                getline(cin, noua);
                client->schimbaParola(veche, noua);
                break;
            }
            case 6:
                sala.recomandaAbonament(client->getTelefon());
                break;
            case 7:
                cout << "Deconectare...\n";
                break;
            default:
                cout << "Optiune invalida!\n";
                break;
        }
    } while (optiune != 7);
}

void meniuPrincipal() {
    Sala sala("FitZone Premium");
    int optiune;

    do {
        cout << "\n========== MENIU PRINCIPAL ==========" << endl;
        cout << "1. Inregistrare client nou"     << endl;
        cout << "2. Autentificare"               << endl;
        cout << "3. Afiseaza toti clientii"      << endl;
        cout << "4. Oferta speciala (admin)"     << endl;
        cout << "5. Iesire"                      << endl;
        cout << "=====================================" << endl;
        cout << "Alege optiunea: ";
        cin >> optiune;
        cin.ignore();   // FIX: flush newline after every menu read

        switch (optiune) {
            case 1: {
                string nume, telefon, parola;
                int varsta;
                cout << "\nNume: ";
                getline(cin, nume);
                cout << "Telefon: ";
                getline(cin, telefon);
                cout << "Parola: ";
                getline(cin, parola);
                cout << "Varsta: ";
                cin >> varsta;
                cin.ignore();
                sala.inregistreazaClient(nume, telefon, parola, varsta);
                break;
            }
            case 2: {
                string telefon, parola;
                cout << "\nTelefon: ";
                getline(cin, telefon);
                cout << "Parola: ";
                getline(cin, parola);
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
                getline(cin, tel);
                cout << "Procent bonus: ";
                cin >> p;
                cin.ignore();
                sala.ofertaSpeciala(tel, p);
                break;
            }
            case 5:
                cout << "La revedere!\n";
                break;
            default:
                cout << "Optiune invalida!\n";
                break;
        }
    } while (optiune != 5);
}

// ======================== MAIN ========================
int main() {
    cout << "\n+========================================+" << endl;
    cout << "|     BINE VENIT LA FITZONE PREMIUM      |" << endl;
    cout << "|  Sistem de Gestiune a Abonamentelor    |" << endl;
    cout << "+========================================+" << endl;

    meniuPrincipal();

    cout << "\n+========================================+" << endl;
    cout << "|        MULTUMESC CA ATI VIZITAT!       |" << endl;
    cout << "+========================================+" << endl;

    return 0;
}