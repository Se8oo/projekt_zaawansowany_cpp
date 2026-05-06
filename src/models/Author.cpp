#include "Author.h"
#include <sstream>
#include <vector>

Author::Author(int id, const std::string& firstName, const std::string& lastName)
    : id(id), firstName(firstName), lastName(lastName) {}

int Author::getId() const { return id; }
std::string Author::getFirstName() const { return firstName; }
std::string Author::getLastName() const { return lastName; }
std::string Author::getFullName() const { return firstName + " " + lastName; }

void Author::setFirstName(const std::string& fn) { firstName = fn; }
void Author::setLastName(const std::string& ln) { lastName = ln; }

std::string Author::toCSV() const {
    return std::to_string(id) + ";" + firstName + ";" + lastName;
}

Author Author::fromCSV(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ';')) {
        parts.push_back(token);
    }
    if (parts.size() < 3) return Author();
    try {
        return Author(std::stoi(parts[0]), parts[1], parts[2]);
    } catch (...) {
        return Author();
    }
}
