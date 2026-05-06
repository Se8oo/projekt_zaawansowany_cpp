# System Zarządzania Domową Biblioteką

Konsolowa aplikacja C++17 do zarządzania domową biblioteką. Umożliwia rejestrację książek i innych pozycji, przypisywanie autorów i kategorii, ocenianie pozycji oraz przeglądanie statystyk. Dane są automatycznie zapisywane i wczytywane z plików CSV.

---

## Struktura projektu

```
projekt_zaawansowany_cpp/
├── CMakeLists.txt
├── data/
│   ├── authors.csv       # Autorzy
│   ├── categories.csv    # Kategorie
│   └── items.csv         # Pozycje biblioteczne
└── src/
    ├── main.cpp
    ├── models/
    │   ├── Author.h / Author.cpp
    │   ├── Category.h / Category.cpp
    │   └── Item.h / Item.cpp
    ├── core/
    │   ├── Library.h / Library.cpp
    └── utils/
        ├── FileStorage.h / FileStorage.cpp
```

---

## Wymagania

- Kompilator obsługujący C++17 (GCC ≥ 8, Clang ≥ 6, MSVC ≥ 2017)
- CMake ≥ 3.15

---

## Budowanie i uruchamianie

### Linux / macOS

```bash
# Sklonuj repozytorium
git clone https://github.com/Se8oo/projekt_zaawansowany_cpp.git
cd projekt_zaawansowany_cpp

# Utwórz katalog build i skompiluj
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Uruchom — WAŻNE: uruchamiaj z katalogu głównego projektu,
# aby ścieżka data/ była poprawna
./build/HomeLibrary
```

### Windows (MSVC / MinGW)

```bat
cmake -B build
cmake --build build --config Release
build\Release\HomeLibrary.exe
```

---

## Lista funkcjonalności

### Klasy modeli (OOP)
- **Author** — autor z imieniem i nazwiskiem; pełne gettery/settery, serializacja CSV
- **Category** — kategoria (nazwa); pełne gettery/settery, serializacja CSV
- **Item** — pozycja biblioteczna (tytuł, autor, kategoria, ocena 0–10, status, opis); pełne gettery/settery, serializacja CSV

### Zarządzanie autorami (CRUD)
- Dodawanie autora (imię i nazwisko)
- Wyświetlanie listy autorów
- Edycja danych autora
- Usuwanie autora

### Zarządzanie kategoriami (CRUD)
- Dodawanie kategorii
- Wyświetlanie listy kategorii
- Edycja nazwy kategorii
- Usuwanie kategorii

### Zarządzanie pozycjami (CRUD)
- Dodawanie pozycji (z wyborem autora i kategorii ze zdefiniowanych list)
- Wyświetlanie wszystkich pozycji z pełnymi danymi (nazwa autora, nazwa kategorii)
- Edycja pozycji (tytuł, autor, kategoria, ocena, status, opis)
- Usuwanie pozycji
- Wyszukiwanie pozycji (po tytule, autorze lub kategorii)

### Statusy pozycji
- `do przeczytania`
- `w trakcie`
- `przeczytane`

### Statystyki
- Łączna liczba pozycji, autorów i kategorii
- Średnia ocena wszystkich pozycji
- Liczba pozycji per kategoria
- Średnia ocena per kategoria

### Trwałość danych (CSV)
- Automatyczne wczytywanie danych przy starcie aplikacji
- Automatyczny zapis danych przy wyjściu
- Ręczny zapis i wczytanie z menu
- Oddzielne pliki CSV dla autorów, kategorii i pozycji (`data/`)

---

## Użyte technologie i wzorce

- **C++17** — `std::optional`, zakres-for, inicjalizacja w konstruktorze
- **OOP** — enkapsulacja, podział na warstwy (modele / logika / UI / persistencja)
- **CRUD** — pełne operacje Create, Read, Update, Delete dla każdej encji
- **CMake** — wieloplatformowy system budowania
