#include "FileStorage.h"
#include <fstream>
#include <sstream>

#ifdef _WIN32
    #define PATH_SEP "\\"
#else
    #define PATH_SEP "/"
#endif

static std::string joinPath(const std::string& dir, const std::string& file) {
    if (dir.empty()) return file;
    char last = dir.back();
    if (last == '/' || last == '\\') {
        return dir + file;
    }
    return dir + PATH_SEP + file;
}

static bool fileHasContent(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    return file.is_open() && file.tellg() > 0;
}

static bool hasBrokenItemReferences(const Library& lib) {
    for (const auto& item : lib.getAllItems()) {
        if (!lib.getAuthor(item.getAuthorId()) || !lib.getCategory(item.getCategoryId())) {
            return true;
        }
    }
    return false;
}

bool FileStorage::saveAuthors(const Library& lib, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (const auto& author : lib.getAllAuthors()) {
        file << author.toCSV() << "\n";
    }
    file.close();
    return true;
}

bool FileStorage::saveCategories(const Library& lib, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (const auto& category : lib.getAllCategories()) {
        file << category.toCSV() << "\n";
    }
    file.close();
    return true;
}

bool FileStorage::saveItems(const Library& lib, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (const auto& item : lib.getAllItems()) {
        file << item.toCSV() << "\n";
    }
    file.close();
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
    std::string authorsFile = joinPath(dataDir, "authors.csv");
    std::string categoriesFile = joinPath(dataDir, "categories.csv");
    std::string itemsFile = joinPath(dataDir, "items.csv");

    bool emptyLibrary = lib.getAllAuthors().empty()
                     && lib.getAllCategories().empty()
                     && lib.getAllItems().empty();
    bool existingData = fileHasContent(authorsFile)
                     || fileHasContent(categoriesFile)
                     || fileHasContent(itemsFile);

    if ((emptyLibrary && existingData) || hasBrokenItemReferences(lib)) {
        return false;
    }

    bool ok = true;
    ok &= saveAuthors(lib, authorsFile);
    ok &= saveCategories(lib, categoriesFile);
    ok &= saveItems(lib, itemsFile);
    return ok;
}

bool FileStorage::loadAll(Library& lib, const std::string& dataDir) {
    Library loaded;
    bool ok = true;
    ok &= loadAuthors(loaded, joinPath(dataDir, "authors.csv"));
    ok &= loadCategories(loaded, joinPath(dataDir, "categories.csv"));
    ok &= loadItems(loaded, joinPath(dataDir, "items.csv"));
    if (ok) {
        lib = loaded;
    }
    return ok;
}
