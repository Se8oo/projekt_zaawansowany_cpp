#include "Category.h"
#include <sstream>
#include <vector>

Category::Category(int id, const std::string& name)
    : id(id), name(name) {}

int Category::getId() const { return id; }
std::string Category::getName() const { return name; }

void Category::setName(const std::string& n) { name = n; }

std::string Category::toCSV() const {
    return std::to_string(id) + ";" + name;
}

Category Category::fromCSV(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ';')) {
        parts.push_back(token);
    }
    if (parts.size() < 2) return Category();
    try {
        return Category(std::stoi(parts[0]), parts[1]);
    } catch (...) {
        return Category();
    }
}
