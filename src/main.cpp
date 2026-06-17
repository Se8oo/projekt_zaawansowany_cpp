#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
#endif
#ifndef _WIN32
    #include <unistd.h>
#endif
#include <cstdio>
#include <stdio.h>
#include "core/Library.h"
#include "utils/FileStorage.h"

// Helper function to check if a directory exists
static bool directoryExists(const std::string& path) {
    #ifdef _WIN32
        DWORD attribs = GetFileAttributesA(path.c_str());
        return (attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY));
    #else
        return false;
    #endif
}

// Helper function to get the data directory path
static std::string getDataDir() {
    // Try 1: "data" relative to current working directory
    if (directoryExists("data")) {
        return "data";
    }
    
    // Try 2: "../data" (up one level - for bin/ subdirectory)
    if (directoryExists("..\\data")) {
        return "..\\data";
    }
    
    // Try 3: Get from executable path
    #ifdef _WIN32
        char buffer[260];
        if (GetModuleFileNameA(NULL, buffer, sizeof(buffer)) > 0) {
            std::string exePath(buffer);
            // Find the last backslash to get the bin directory
            size_t binSlash = exePath.find_last_of("\\/");
            if (binSlash != std::string::npos) {
                // Go up one more level to project root
                std::string binDir = exePath.substr(0, binSlash);
                size_t projSlash = binDir.find_last_of("\\/");
                if (projSlash != std::string::npos) {
                    std::string projectRoot = binDir.substr(0, projSlash);
                    std::string dataPath = projectRoot + "\\data";
                    if (directoryExists(dataPath)) {
                        return dataPath;
                    }
                }
            }
        }
    #endif
    
    // Default fallback
    return "data";
}

static const std::string DATA_DIR = getDataDir();

static void clearInput() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

static bool isBlank(const std::string& text) {
    return text.find_first_not_of(" \t\r\n") == std::string::npos;
}

static bool tryParseInt(const std::string& text, int& value) {
    std::stringstream ss(text);
    ss >> value;
    if (!ss) return false;
    ss >> std::ws;
    return ss.eof();
}

static bool tryParseDouble(const std::string& text, double& value) {
    std::stringstream ss(text);
    ss >> value;
    if (!ss) return false;
    ss >> std::ws;
    return ss.eof();
}

static int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt << std::flush;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        if (std::cin.eof()) {
            return 0;
        }
        std::cout << "  [!] Podaj liczbę całkowitą.\n" << std::flush;
        clearInput();
    }
}

static double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt << std::flush;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        if (std::cin.eof()) {
            return 0.0;
        }
        std::cout << "  [!] Podaj liczbe.\n" << std::flush;
        clearInput();
    }
}

static int readIntInRange(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        int value = readInt(prompt);
        if (std::cin.eof()) return minValue;
        if (value >= minValue && value <= maxValue) return value;
        std::cout << "  [!] Podaj wartość od " << minValue << " do " << maxValue << ".\n";
    }
}

static double readDoubleInRange(const std::string& prompt, double minValue, double maxValue) {
    while (true) {
        double value = readDouble(prompt);
        if (std::cin.eof()) return minValue;
        if (value >= minValue && value <= maxValue) return value;
        std::cout << "  [!] Podaj wartość od " << minValue << " do " << maxValue << ".\n";
    }
}

static std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt << std::flush;
    std::getline(std::cin, value);
    return value;
}

static std::string readRequiredLine(const std::string& prompt) {
    while (true) {
        std::string value = readLine(prompt);
        if (!isBlank(value)) return value;
        if (std::cin.eof()) return "";
        std::cout << "  [!] Pole nie może być puste.\n";
    }
}

static void pressEnterToContinue() {
    // If stdin is a terminal, wait for Enter. When input is piped (non-interactive), skip.
#if defined(_WIN32)
    if (_isatty(0)) {
        std::cout << "\nNaciśnij Enter, aby kontynuować...";
        std::cin.get();
    } else {
        std::cout << "\n";
    }
#else
    if (isatty(0)) {
        std::cout << "\nNaciśnij Enter, aby kontynuować...";
        std::cin.get();
    } else {
        std::cout << "\n";
    }
#endif
}

static void printSeparator() {
    std::cout << "--------------------------------------------\n";
}

// ──────────────────────────────────────────────────────────
// Wyświetlanie danych
// ──────────────────────────────────────────────────────────

static void printAuthor(const Author& a) {
    std::cout << "  [" << a.getId() << "] "
              << a.getFirstName() << " " << a.getLastName() << "\n";
}

static void printCategory(const Category& c) {
    std::cout << "  [" << c.getId() << "] " << c.getName() << "\n";
}

static void printItem(const Item& item, const Library& lib) {
    auto author = lib.getAuthor(item.getAuthorId());
    auto category = lib.getCategory(item.getCategoryId());

    std::string authorName = author ? author->getFullName() : "(brak)";
    std::string categoryName = category ? category->getName() : "(brak)";

    std::cout << "  [" << item.getId() << "] "
              << item.getTitle() << "\n"
              << "      Autor:     " << authorName << "\n"
              << "      Kategoria: " << categoryName << "\n"
              << "      Ocena:     " << std::fixed << std::setprecision(1) << item.getRating() << "/10\n"
              << "      Status:    " << item.getStatus() << "\n";
    if (!item.getDescription().empty()) {
        std::cout << "      Opis:      " << item.getDescription() << "\n";
    }
}

// ──────────────────────────────────────────────────────────
// Menu autorów
// ──────────────────────────────────────────────────────────

static void menuDodajAutora(Library& lib) {
    std::cout << "\n--- Dodaj autora ---\n";
    std::string firstName = readRequiredLine("Imię: ");
    std::string lastName  = readRequiredLine("Nazwisko: ");
    if (isBlank(firstName) || isBlank(lastName)) {
        std::cout << "  [!] Nie dodano autora - brak wymaganych danych.\n";
        return;
    }
    int newId = lib.getNextAuthorId();
    lib.addAuthor(Author(newId, firstName, lastName));
    std::cout << "  [+] Autor dodany (ID=" << newId << ").\n";
}

static void menuWyswietlAutorow(const Library& lib) {
    std::cout << "\n--- Lista autorów ---\n";
    const auto& authors = lib.getAllAuthors();
    if (authors.empty()) {
        std::cout << "  Brak autorów.\n";
        return;
    }
    for (const auto& a : authors) printAuthor(a);
}

static void menuEdytujAutora(Library& lib) {
    std::cout << "\n--- Edytuj autora ---\n";
    menuWyswietlAutorow(lib);
    if (lib.getAllAuthors().empty()) return;

    int id = readInt("ID autora do edycji: ");
    auto found = lib.getAuthor(id);
    if (!found) {
        std::cout << "  [!] Nie znaleziono autora o ID=" << id << ".\n";
        return;
    }
    Author a = *found;
    std::cout << "  Aktualne dane: " << a.getFullName() << "\n";
    std::cout << "  (Pozostaw puste, aby nie zmieniać)\n";
    std::string firstName = readLine("Nowe imię: ");
    std::string lastName  = readLine("Nowe nazwisko: ");
    if (!isBlank(firstName)) a.setFirstName(firstName);
    if (!isBlank(lastName))  a.setLastName(lastName);
    lib.updateAuthor(a);
    std::cout << "  [OK] Autor zaktualizowany.\n";
}

static void menuUsunAutora(Library& lib) {
    std::cout << "\n--- Usun autora ---\n";
    menuWyswietlAutorow(lib);
    if (lib.getAllAuthors().empty()) return;

    int id = readInt("ID autora do usunięcia: ");
    if (lib.removeAuthor(id)) {
        std::cout << "  [OK] Autor usunięty.\n";
    } else {
        std::cout << "  [!] Nie znaleziono autora o ID=" << id << ".\n";
    }
}

static void submenuAutorzy(Library& lib) {
    int choice;
    do {
        std::cout << "\n=== ZARZĄDZANIE AUTORAMI ===\n";
        std::cout << "  1. Dodaj autora\n";
        std::cout << "  2. Wyświetl wszystkich autorów\n";
        std::cout << "  3. Edytuj autora\n";
        std::cout << "  4. Usuń autora\n";
        std::cout << "  0. Powrót\n";
        choice = readInt("Wybór: ");

        switch (choice) {
            case 1: menuDodajAutora(lib); pressEnterToContinue(); break;
            case 2: menuWyswietlAutorow(lib); pressEnterToContinue(); break;
            case 3: menuEdytujAutora(lib); pressEnterToContinue(); break;
            case 4: menuUsunAutora(lib); pressEnterToContinue(); break;
            case 0: break;
            default: std::cout << "  [!] Nieznana opcja.\n"; break;
        }
    } while (choice != 0);
}

// ──────────────────────────────────────────────────────────
// Menu kategorii
// ──────────────────────────────────────────────────────────

static void menuDodajKategorie(Library& lib) {
    std::cout << "\n--- Dodaj kategorię ---\n";
    std::string name = readRequiredLine("Nazwa kategorii: ");
    if (isBlank(name)) {
        std::cout << "  [!] Nie dodano kategorii - nazwa jest wymagana.\n";
        return;
    }
    int newId = lib.getNextCategoryId();
    lib.addCategory(Category(newId, name));
    std::cout << "  [+] Kategoria dodana (ID=" << newId << ").\n";
}

static void menuWyswietlKategorie(const Library& lib) {
    std::cout << "\n--- Lista kategorii ---\n";
    const auto& cats = lib.getAllCategories();
    if (cats.empty()) {
        std::cout << "  Brak kategorii.\n";
        return;
    }
    for (const auto& c : cats) printCategory(c);
}

static void menuEdytujKategorie(Library& lib) {
    std::cout << "\n--- Edytuj kategorię ---\n";
    menuWyswietlKategorie(lib);
    if (lib.getAllCategories().empty()) return;

    int id = readInt("ID kategorii do edycji: ");
    auto found = lib.getCategory(id);
    if (!found) {
        std::cout << "  [!] Nie znaleziono kategorii o ID=" << id << ".\n";
        return;
    }
    Category c = *found;
    std::cout << "  Aktualna nazwa: " << c.getName() << "\n";
    std::string name = readLine("Nowa nazwa: ");
    if (!isBlank(name)) c.setName(name);
    lib.updateCategory(c);
    std::cout << "  [OK] Kategoria zaktualizowana.\n";
}

static void menuUsunKategorie(Library& lib) {
    std::cout << "\n--- Usuń kategorię ---\n";
    menuWyswietlKategorie(lib);
    if (lib.getAllCategories().empty()) return;

    int id = readInt("ID kategorii do usunięcia: ");
    if (lib.removeCategory(id)) {
        std::cout << "  [OK] Kategoria usunięta.\n";
    } else {
        std::cout << "  [!] Nie znaleziono kategorii o ID=" << id << ".\n";
    }
}

static void submenuKategorie(Library& lib) {
    int choice;
    do {
        std::cout << "\n=== ZARZĄDZANIE KATEGORIAMI ===\n";
        std::cout << "  1. Dodaj kategorię\n";
        std::cout << "  2. Wyświetl wszystkie kategorie\n";
        std::cout << "  3. Edytuj kategorię\n";
        std::cout << "  4. Usuń kategorię\n";
        std::cout << "  0. Powrót\n";
        choice = readInt("Wybór: ");

        switch (choice) {
            case 1: menuDodajKategorie(lib); pressEnterToContinue(); break;
            case 2: menuWyswietlKategorie(lib); pressEnterToContinue(); break;
            case 3: menuEdytujKategorie(lib); pressEnterToContinue(); break;
            case 4: menuUsunKategorie(lib); pressEnterToContinue(); break;
            case 0: break;
            default: std::cout << "  [!] Nieznana opcja.\n"; break;
        }
    } while (choice != 0);
}

// ──────────────────────────────────────────────────────────
// Menu pozycji
// ──────────────────────────────────────────────────────────

static std::string chooseStatus() {
    std::cout << "  Status:\n";
    std::cout << "    1. Do przeczytania\n";
    std::cout << "    2. W trakcie\n";
    std::cout << "    3. Przeczytane\n";
    int s = readIntInRange("  Wybor: ", 1, 3);
    switch (s) {
        case 1: return "do przeczytania";
        case 2: return "w trakcie";
        case 3: return "przeczytane";
        default: return "do przeczytania";
    }
}

static void menuDodajPozycje(Library& lib) {
    std::cout << "\n--- Dodaj pozycję ---\n";

    if (lib.getAllAuthors().empty()) {
        std::cout << "  [!] Brak autorów. Dodaj najpierw autora.\n";
        return;
    }
    if (lib.getAllCategories().empty()) {
        std::cout << "  [!] Brak kategorii. Dodaj najpierw kategorię.\n";
        return;
    }

    std::string title = readRequiredLine("Tytuł: ");
    if (isBlank(title)) {
        std::cout << "  [!] Nie dodano pozycji - tytuł jest wymagany.\n";
        return;
    }

    menuWyswietlAutorow(lib);
    int authorId = readInt("ID autora: ");
    if (!lib.getAuthor(authorId)) {
        std::cout << "  [!] Autor o ID=" << authorId << " nie istnieje.\n";
        return;
    }

    menuWyswietlKategorie(lib);
    int categoryId = readInt("ID kategorii: ");
    if (!lib.getCategory(categoryId)) {
        std::cout << "  [!] Kategoria o ID=" << categoryId << " nie istnieje.\n";
        return;
    }

    double rating = readDoubleInRange("Ocena (0-10): ", 0.0, 10.0);

    std::string status = chooseStatus();
    std::string description = readLine("Opis (opcjonalnie): ");

    int newId = lib.getNextItemId();
    lib.addItem(Item(newId, title, authorId, categoryId, rating, status, description));
    std::cout << "  [+] Pozycja dodana (ID=" << newId << ").\n";
}

static void menuWyswietlPozycje(const Library& lib) {
    std::cout << "\n--- Lista pozycji ---\n";
    const auto& items = lib.getAllItems();
    if (items.empty()) {
        std::cout << "  Brak pozycji.\n";
        return;
    }
    printSeparator();
    for (const auto& item : items) {
        printItem(item, lib);
        printSeparator();
    }
}

static void menuEdytujPozycje(Library& lib) {
    std::cout << "\n--- Edytuj pozycję ---\n";
    menuWyswietlPozycje(lib);
    if (lib.getAllItems().empty()) return;

    int id = readInt("ID pozycji do edycji: ");
    auto found = lib.getItem(id);
    if (!found) {
        std::cout << "  [!] Nie znaleziono pozycji o ID=" << id << ".\n";
        return;
    }
    Item item = *found;
    std::cout << "  (Pozostaw puste, aby nie zmieniać)\n";

    std::string title = readLine("Nowy tytuł [" + item.getTitle() + "]: ");
    if (!isBlank(title)) item.setTitle(title);

    menuWyswietlAutorow(lib);
    std::string authorIdStr = readLine("Nowe ID autora [" + std::to_string(item.getAuthorId()) + "]: ");
    if (!isBlank(authorIdStr)) {
        int aId;
        if (!tryParseInt(authorIdStr, aId)) {
            std::cout << "  [!] Niepoprawne ID autora, pozostawiono bez zmian.\n";
        } else if (lib.getAuthor(aId)) {
            item.setAuthorId(aId);
        } else {
            std::cout << "  [!] Autor nie istnieje, pozostawiono bez zmian.\n";
        }
    }

    menuWyswietlKategorie(lib);
    std::string catIdStr = readLine("Nowe ID kategorii [" + std::to_string(item.getCategoryId()) + "]: ");
    if (!isBlank(catIdStr)) {
        int cId;
        if (!tryParseInt(catIdStr, cId)) {
            std::cout << "  [!] Niepoprawne ID kategorii, pozostawiono bez zmian.\n";
        } else if (lib.getCategory(cId)) {
            item.setCategoryId(cId);
        } else {
            std::cout << "  [!] Kategoria nie istnieje, pozostawiono bez zmian.\n";
        }
    }

    std::string ratingStr = readLine("Nowa ocena [" + std::to_string(item.getRating()) + "]: ");
    if (!isBlank(ratingStr)) {
        double r;
        if (!tryParseDouble(ratingStr, r) || r < 0.0 || r > 10.0) {
            std::cout << "  [!] Ocena musi być liczbą od 0 do 10, pozostawiono bez zmian.\n";
        } else {
            item.setRating(r);
        }
    }

    std::cout << "  Zmień status? (t/n): ";
    std::string changeStatus;
    std::getline(std::cin, changeStatus);
    if (changeStatus == "t" || changeStatus == "T") {
        item.setStatus(chooseStatus());
    }

    std::string desc = readLine("Nowy opis [" + item.getDescription() + "]: ");
    if (!desc.empty()) item.setDescription(desc);

    lib.updateItem(item);
    std::cout << "  [OK] Pozycja zaktualizowana.\n";
}

static void menuUsunPozycje(Library& lib) {
    std::cout << "\n--- Usuń pozycję ---\n";
    menuWyswietlPozycje(lib);
    if (lib.getAllItems().empty()) return;

    int id = readInt("ID pozycji do usunięcia: ");
    if (lib.removeItem(id)) {
        std::cout << "  [OK] Pozycja usunięta.\n";
    } else {
        std::cout << "  [!] Nie znaleziono pozycji o ID=" << id << ".\n";
    }
}

static void menuSzukajPozycji(const Library& lib) {
    std::cout << "\n--- Wyszukaj pozycję ---\n";
    std::cout << "  1. Szukaj po tytule\n";
    std::cout << "  2. Szukaj po autorze\n";
    std::cout << "  3. Szukaj po kategorii\n";
    int choice = readInt("Wybor: ");

    std::vector<Item> results;
    if (choice == 1) {
        std::string phrase = readLine("Fraza w tytule: ");
        if (isBlank(phrase)) {
            std::cout << "  [!] Fraza nie może być pusta.\n";
            return;
        }
        results = lib.searchByTitle(phrase);
    } else if (choice == 2) {
        menuWyswietlAutorow(lib);
        int authorId = readInt("ID autora: ");
        results = lib.getItemsByAuthor(authorId);
    } else if (choice == 3) {
        menuWyswietlKategorie(lib);
        int categoryId = readInt("ID kategorii: ");
        results = lib.getItemsByCategory(categoryId);
    } else {
        std::cout << "  [!] Nieznana opcja.\n";
        return;
    }

    if (results.empty()) {
        std::cout << "  Brak wyników.\n";
        return;
    }
    std::cout << "  Znaleziono " << results.size() << " pozycji:\n";
    printSeparator();
    for (const auto& item : results) {
        printItem(item, lib);
        printSeparator();
    }
}

static void menuFiltrujPoStatusie(const Library& lib) {
    std::cout << "\n--- Filtruj po statusie ---\n";
    if (lib.getAllItems().empty()) {
        std::cout << "  Brak pozycji.\n";
        return;
    }

    std::string status = chooseStatus();
    std::vector<Item> results = lib.getItemsByStatus(status);
    if (results.empty()) {
        std::cout << "  Brak pozycji o statusie: " << status << ".\n";
        return;
    }

    std::cout << "  Pozycje o statusie: " << status << "\n";
    printSeparator();
    for (const auto& item : results) {
        printItem(item, lib);
        printSeparator();
    }
}

static void submenuPozycje(Library& lib) {
    int choice;
    do {
        std::cout << "\n=== ZARZĄDZANIE POZYCJAMI ===\n";
        std::cout << "  1. Dodaj pozycję\n";
        std::cout << "  2. Wyświetl wszystkie pozycje\n";
        std::cout << "  3. Edytuj pozycję\n";
        std::cout << "  4. Usuń pozycję\n";
        std::cout << "  5. Wyszukaj pozycję\n";
        std::cout << "  6. Filtruj po statusie\n";
        std::cout << "  0. Powrót\n";
        choice = readInt("Wybór: ");

        switch (choice) {
            case 1: menuDodajPozycje(lib); pressEnterToContinue(); break;
            case 2: menuWyswietlPozycje(lib); pressEnterToContinue(); break;
            case 3: menuEdytujPozycje(lib); pressEnterToContinue(); break;
            case 4: menuUsunPozycje(lib); pressEnterToContinue(); break;
            case 5: menuSzukajPozycji(lib); pressEnterToContinue(); break;
            case 6: menuFiltrujPoStatusie(lib); pressEnterToContinue(); break;
            case 0: break;
            default: std::cout << "  [!] Nieznana opcja.\n"; break;
        }
    } while (choice != 0);
}

// ──────────────────────────────────────────────────────────
// Menu statystyk
// ──────────────────────────────────────────────────────────

static void submenuStatystyki(const Library& lib) {
    std::cout << "\n=== STATYSTYKI ===\n";
    printSeparator();

    const auto& items = lib.getAllItems();
    std::cout << "  Liczba pozycji ogółem:   " << items.size() << "\n";
    std::cout << "  Liczba autorów:          " << lib.getAllAuthors().size() << "\n";
    std::cout << "  Liczba kategorii:        " << lib.getAllCategories().size() << "\n";

    if (!items.empty()) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  Średnia ocena ogólna:    " << lib.getAverageRating() << "/10\n";
    }

    printSeparator();
    std::cout << "  Pozycje wg kategorii:\n";
    const auto& categories = lib.getAllCategories();
    if (categories.empty()) {
        std::cout << "    Brak kategorii.\n";
    } else {
        auto counts = lib.countAllByCategory();
        for (const auto& cat : categories) {
            int cnt = 0;
            auto it = counts.find(cat.getId());
            if (it != counts.end()) cnt = it->second;
            double avg = lib.getAverageRatingByCategory(cat.getId());
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "    [" << cat.getId() << "] " << cat.getName()
                      << ": " << cnt << " poz."
                      << " (sr. ocena: " << avg << "/10)\n";
        }
    }
    printSeparator();
    pressEnterToContinue();
}

// ──────────────────────────────────────────────────────────
// Zapis / Odczyt
// ──────────────────────────────────────────────────────────

static void menuZapisz(const Library& lib) {
    if (FileStorage::saveAll(lib, DATA_DIR)) {
        std::cout << "  [OK] Dane zapisane do katalogu '" << DATA_DIR << "'.\n";
    } else {
        std::cout << "  [!] Błąd zapisu danych.\n";
    }
}

static void menuWczytaj(Library& lib) {
    Library fresh;
    if (FileStorage::loadAll(fresh, DATA_DIR)) {
        lib = fresh;
        std::cout << "  [OK] Dane wczytane z katalogu '" << DATA_DIR << "'.\n";
    } else {
        std::cout << "  [!] Błąd wczytywania danych (sprawdź czy pliki istnieją).\n";
    }
}

// ──────────────────────────────────────────────────────────
// Main
// ──────────────────────────────────────────────────────────

int main() {
    // Włącz UTF-8 w konsoli Windows
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    Library library;

    // Auto-wczytaj dane przy starcie jeśli pliki istnieją
    FileStorage::loadAll(library, DATA_DIR);

    int choice;
    do {
        std::cout << "\n============================================\n";
        std::cout << "  SYSTEM ZARZADZĄNIA DOMOWĄ BIBLIOTEKĄ\n";
        std::cout << "============================================\n";
        std::cout << "  1. Zarządzanie autorami\n";
        std::cout << "  2. Zarządzanie kategoriami\n";
        std::cout << "  3. Zarządzanie pozycjami\n";
        std::cout << "  4. Statystyki\n";
        std::cout << "  5. Zapisz dane\n";
        std::cout << "  6. Wczytaj dane\n";
        std::cout << "  0. Wyjdź\n";
        choice = readInt("Wybór: ");

        switch (choice) {
            case 1: submenuAutorzy(library); break;
            case 2: submenuKategorie(library); break;
            case 3: submenuPozycje(library); break;
            case 4: submenuStatystyki(library); break;
            case 5: menuZapisz(library); pressEnterToContinue(); break;
            case 6: menuWczytaj(library); pressEnterToContinue(); break;
            case 0:
                menuZapisz(library);
                std::cout << "  Do widzenia!\n";
                break;
            default: std::cout << "  [!] Nieznana opcja.\n"; break;
        }
    } while (choice != 0);

    return 0;
}
