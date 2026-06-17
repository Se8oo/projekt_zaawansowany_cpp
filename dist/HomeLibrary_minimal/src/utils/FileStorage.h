#pragma once
#include "../core/Library.h"
#include <string>

class FileStorage {
public:
    static bool saveAuthors(const Library& lib, const std::string& filename);
    static bool saveCategories(const Library& lib, const std::string& filename);
    static bool saveItems(const Library& lib, const std::string& filename);

    static bool loadAuthors(Library& lib, const std::string& filename);
    static bool loadCategories(Library& lib, const std::string& filename);
    static bool loadItems(Library& lib, const std::string& filename);

    static bool saveAll(const Library& lib, const std::string& dataDir);
    static bool loadAll(Library& lib, const std::string& dataDir);
};
