#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include "core/Library.h"
#include "utils/FileStorage.h"

static const std::string DATA_DIR = "data";

// ──────────────────────────────────────────────────────────
// Pomocnicze funkcje wejścia/wyjścia
// ──────────────────────────────────────────────────────────

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        std::cout << "  [!] Podaj liczbę całkowitą.\n";
        clearInput();
    }
}

static double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        std::cout << "  [!] Podaj liczbe.\n";
        clearInput();
    }
}

static std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

static void pressEnterToContinue() {
    std::cout << "\nNaciśnij Enter, aby kontynuować...";
    std::cin.get();
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
    std::string firstName = readLine("Imię: ");
    std::string lastName  = readLine("Nazwisko: ");
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
    if (!firstName.empty()) a.setFirstName(firstName);
    if (!lastName.empty())  a.setLastName(lastName);
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
    std::string name = readLine("Nazwa kategorii: ");
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
    if (!name.empty()) c.setName(name);
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
    int s = readInt("  Wybor: ");
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

    std::string title = readLine("Tytuł: ");

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

    double rating = readDouble("Ocena (0-10): ");
    if (rating < 0.0) rating = 0.0;
    if (rating > 10.0) rating = 10.0;

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
    if (!title.empty()) item.setTitle(title);

    menuWyswietlAutorow(lib);
    std::string authorIdStr = readLine("Nowe ID autora [" + std::to_string(item.getAuthorId()) + "]: ");
    if (!authorIdStr.empty()) {
        int aId = std::stoi(authorIdStr);
        if (lib.getAuthor(aId)) item.setAuthorId(aId);
        else std::cout << "  [!] Autor nie istnieje, pozostawiono bez zmian.\n";
    }

    menuWyswietlKategorie(lib);
    std::string catIdStr = readLine("Nowe ID kategorii [" + std::to_string(item.getCategoryId()) + "]: ");
    if (!catIdStr.empty()) {
        int cId = std::stoi(catIdStr);
        if (lib.getCategory(cId)) item.setCategoryId(cId);
        else std::cout << "  [!] Kategoria nie istnieje, pozostawiono bez zmian.\n";
    }

    std::string ratingStr = readLine("Nowa ocena [" + std::to_string(item.getRating()) + "]: ");
    if (!ratingStr.empty()) {
        double r = std::stod(ratingStr);
        if (r < 0.0) r = 0.0;
        if (r > 10.0) r = 10.0;
        item.setRating(r);
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

static void submenuPozycje(Library& lib) {
    int choice;
    do {
        std::cout << "\n=== ZARZĄDZANIE POZYCJAMI ===\n";
        std::cout << "  1. Dodaj pozycję\n";
        std::cout << "  2. Wyświetl wszystkie pozycje\n";
        std::cout << "  3. Edytuj pozycję\n";
        std::cout << "  4. Usuń pozycję\n";
        std::cout << "  5. Wyszukaj pozycję\n";
        std::cout << "  0. Powrót\n";
        choice = readInt("Wybór: ");

        switch (choice) {
            case 1: menuDodajPozycje(lib); pressEnterToContinue(); break;
            case 2: menuWyswietlPozycje(lib); pressEnterToContinue(); break;
            case 3: menuEdytujPozycje(lib); pressEnterToContinue(); break;
            case 4: menuUsunPozycje(lib); pressEnterToContinue(); break;
            case 5: menuSzukajPozycji(lib); pressEnterToContinue(); break;
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
    Library library;

    // Auto-wczytaj dane przy starcie jeśli pliki istnieją
    FileStorage::loadAll(library, DATA_DIR);

    int choice;
    do {
        std::cout << "\n╔══════════════════════════════════════╗\n";
        std::cout << "║  SYSTEM ZARZĄDZANIA DOMOWĄ BIBLIOTEKĄ  ║\n";
        std::cout << "╚══════════════════════════════════════╝\n";
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
