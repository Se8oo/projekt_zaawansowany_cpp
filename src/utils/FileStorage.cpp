#include "FileStorage.h"
#include <fstream>

bool FileStorage::saveAuthors(const Library& lib, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (const auto& author : lib.getAllAuthors()) {
        file << author.toCSV() << "\n";
    }
    return true;
}

bool FileStorage::saveCategories(const Library& lib, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (const auto& category : lib.getAllCategories()) {
        file << category.toCSV() << "\n";
    }
    return true;
}

bool FileStorage::saveItems(const Library& lib, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (const auto& item : lib.getAllItems()) {
        file << item.toCSV() << "\n";
    }
    return true;
}

bool FileStorage::loadAuthors(Library& lib, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Author a = Author::fromCSV(line);
            if (a.getId() != 0) lib.addAuthor(a);
        }
    }
    return true;
}

bool FileStorage::loadCategories(Library& lib, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Category c = Category::fromCSV(line);
            if (c.getId() != 0) lib.addCategory(c);
        }
    }
    return true;
}

bool FileStorage::loadItems(Library& lib, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Item item = Item::fromCSV(line);
            if (item.getId() != 0) lib.addItem(item);
        }
    }
    return true;
}

bool FileStorage::saveAll(const Library& lib, const std::string& dataDir) {
    bool ok = true;
    ok &= saveAuthors(lib, dataDir + "/authors.csv");
    ok &= saveCategories(lib, dataDir + "/categories.csv");
    ok &= saveItems(lib, dataDir + "/items.csv");
    return ok;
}

bool FileStorage::loadAll(Library& lib, const std::string& dataDir) {
    bool ok = true;
    ok &= loadAuthors(lib, dataDir + "/authors.csv");
    ok &= loadCategories(lib, dataDir + "/categories.csv");
    ok &= loadItems(lib, dataDir + "/items.csv");
    return ok;
}
