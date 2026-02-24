# 🏋️ FitZone Premium — Gym Membership Management System (C++)

FitZone Premium is a console-based C++ application that simulates a gym management system.  
It demonstrates Object-Oriented Programming (OOP), operator overloading, validation using regex, and menu-driven interaction.

This project was developed for educational purposes to practice advanced C++ concepts.

---

## 🚀 Features

### 🔹 Main Menu
- Register new client (with validation)
- Client authentication (phone + password)
- Display all registered clients
- Admin special offer (add bonus to client balance)

### 🔹 Client Menu
- View full profile
- Add funds to balance
- Purchase subscription
- Activate subscription
- Change password
- Modify personal data
- Test overloaded operators

---

## 🧠 Core Concepts Used

- Object-Oriented Programming (Encapsulation, Composition)
- Operator Overloading
- Enums
- Regex validation
- Friend functions
- Exception handling
- Menu-driven program structure

---

## 🏗️ Project Structure

### 📌 Class: `Abonament`
Represents a gym subscription.

**Attributes**
- Type
- Duration (months)
- Price
- Status (`NEPROCURAT`, `PROCURAT`, `ACTIV`, `EXPIRAT`)
- Purchase / activation / expiration dates

**Overloaded Operators**
- `+` → Extend subscription duration
- `==` → Compare type and duration
- `>` → Compare by price
- `<<` → Output subscription details

---

### 📌 Class: `Client`
Represents a gym member.

**Attributes**
- Name
- Phone number (unique identifier)
- Password
- Age
- Balance
- Subscription

**Overloaded Operators**
- `+=` → Add balance
- `-=` → Deduct balance
- `<` → Compare by age
- `==` → Compare by phone
- `<<` → Output client info
- `>>` → Input client data

---

### 📌 Class: `Sala`
Represents the gym.

**Attributes**
- Fixed array of max 100 clients
- Gym name

**Overloaded Operators**
- `[]` → Access client by index
- `+` → Add client (returns new gym object)
- `+=` → Add client directly
- `()` → Search client by phone
- `!` → Check if gym is full
- `int()` → Convert to number of clients

---

## ✅ Input Validation

### 📱 Phone Number
Accepted formats:
- `+373XXXXXXXX`
- `0XXXXXXXX`

### 📅 Date Format
Required format:

YYYY-MM-DD

(Format validation only)

---

## ▶️ How to Compile & Run

### Compile
```bash
g++ -std=c++17 main.cpp -o fitzone
Run
./fitzone
🧪 Operator Testing

The project includes a dedicated Operator Testing Menu where you can test:

+= and -=

<< output

Subscription extension (+)

Subscription comparison (==)

Index access ([])

Functor search (())

Type casting to int

Full capacity check (!)

⚙️ Complexity

Since the maximum number of clients is 100:

Registration: O(n)

Authentication: O(n)

Search by phone: O(n)

Index access: O(1)

📌 Limitations

Date validation checks only format, not real calendar validity.

Passwords are stored in plain text.

Client storage uses a fixed-size array (max 100 clients).

No persistent storage (data resets on program exit).

🔮 Possible Improvements

Replace array with std::vector or unordered_map

Hash passwords

Real date validation

Persistent storage (file or database)

Automatic subscription expiration

Admin authentication system

👨‍💻 Author

Educational C++ project demonstrating:

OOP principles

Operator overloading

Input validation

Structured console applications



Laboratorul nr. 4 – Supraincărcarea operatorilor
Context
În Laboratorul 3 a fost implementat un sistem de gestiune pentru o sală de sport, folosind clasele Client, Abonament și Sala. Au fost aplicate principiile încapsulării, s-au utilizat funcții și clase friend și s-a construit un meniu interactiv pentru gestionarea utilizatorilor.
În acest laborator vom continua dezvoltarea sistemului, introducând supraincărcarea operatorilor pentru a face interacțiunea cu obiectele mai naturală și mai expresivă.

Obiective
1.	Înțelegerea conceptului de operator overloading în C++.
2.	Implementarea de operatori relevanți pentru fiecare clasă.
3.	Integrarea operatorilor în logica aplicației (fluxuri și meniuri).
4.	Exersarea diferenței dintre implementarea operatorilor ca metode membre și ca funcții friend.

Sarcina laboratorului
Supraincărcarea operatorilor
1.	Clasa Client - trebuie să conțină minimum 3 operatori supraincarcați.
2.	Clasa Abonament - trebuie să conțină minimum 3 operatori supraincarcați.
3.	Clasa Sala - trebuie să conțină minimum 3 operatori supraincarcați.

Note
1.	Programul va păstra structura meniurilor din Laboratorul 3 (Meniu de bază și Meniu principal).
2.	În cadrul funcționalităților deja existente, se va integra utilizarea operatorilor supraincarcați acolo unde are sens.

Cerințe finale
1.	Fiecare clasă (Client, Abonament, Sala) trebuie să aibă minim 3 operatori supraincarcați, diferiți între ei.
2.	Alegerea operatorilor se face de către student, dar trebuie argumentată în comentarii de ce operatorii respectivi sunt relevanți.
3.	Operatorii implementați trebuie testați explicit.
4.	Aplicația trebuie să compileze și să ruleze, integrând operatorii în meniurile deja existente.
