#include "Item.h"
#include <sstream>
#include <vector>

Item::Item(int id, const std::string& title, int authorId, int categoryId,
           double rating, const std::string& status, const std::string& description)
    : id(id), title(title), authorId(authorId), categoryId(categoryId),
      rating(rating), status(status), description(description) {}

int Item::getId() const { return id; }
std::string Item::getTitle() const { return title; }
int Item::getAuthorId() const { return authorId; }
int Item::getCategoryId() const { return categoryId; }
double Item::getRating() const { return rating; }
std::string Item::getStatus() const { return status; }
std::string Item::getDescription() const { return description; }

void Item::setTitle(const std::string& t) { title = t; }
void Item::setAuthorId(int id) { authorId = id; }
void Item::setCategoryId(int id) { categoryId = id; }
void Item::setRating(double r) { rating = r; }
void Item::setStatus(const std::string& s) { status = s; }
void Item::setDescription(const std::string& d) { description = d; }

std::string Item::toCSV() const {
    return std::to_string(id) + ";" +
           title + ";" +
           std::to_string(authorId) + ";" +
           std::to_string(categoryId) + ";" +
           std::to_string(rating) + ";" +
           status + ";" +
           description;
}

Item Item::fromCSV(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ';')) {
        parts.push_back(token);
    }
    if (parts.size() < 6) return Item();
    std::string desc = (parts.size() >= 7) ? parts[6] : "";
    try {
        return Item(
            std::stoi(parts[0]),
            parts[1],
            std::stoi(parts[2]),
            std::stoi(parts[3]),
            std::stod(parts[4]),
            parts[5],
            desc
        );
    } catch (...) {
        return Item();
    }
}
