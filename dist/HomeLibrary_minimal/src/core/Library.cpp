#include "Library.h"
#include <algorithm>

// ---- Autorzy ----

void Library::addAuthor(const Author& author) {
    authors.push_back(author);
}

bool Library::removeAuthor(int id) {
    for (auto it = authors.begin(); it != authors.end(); ++it) {
        if (it->getId() == id) {
            authors.erase(it);
            return true;
        }
    }
    return false;
}

bool Library::updateAuthor(const Author& updated) {
    for (auto& a : authors) {
        if (a.getId() == updated.getId()) {
            a = updated;
            return true;
        }
    }
    return false;
}

compat::optional<Author> Library::getAuthor(int id) const {
    for (const auto& a : authors) {
        if (a.getId() == id) return a;
    }
    return {};
}

const std::vector<Author>& Library::getAllAuthors() const {
    return authors;
}

int Library::getNextAuthorId() const {
    int maxId = 0;
    for (const auto& a : authors) {
        if (a.getId() > maxId) maxId = a.getId();
    }
    return maxId + 1;
}

// ---- Kategorie ----

void Library::addCategory(const Category& category) {
    categories.push_back(category);
}

bool Library::removeCategory(int id) {
    for (auto it = categories.begin(); it != categories.end(); ++it) {
        if (it->getId() == id) {
            categories.erase(it);
            return true;
        }
    }
    return false;
}

bool Library::updateCategory(const Category& updated) {
    for (auto& c : categories) {
        if (c.getId() == updated.getId()) {
            c = updated;
            return true;
        }
    }
    return false;
}

compat::optional<Category> Library::getCategory(int id) const {
    for (const auto& c : categories) {
        if (c.getId() == id) return c;
    }
    return {};
}

const std::vector<Category>& Library::getAllCategories() const {
    return categories;
}

int Library::getNextCategoryId() const {
    int maxId = 0;
    for (const auto& c : categories) {
        if (c.getId() > maxId) maxId = c.getId();
    }
    return maxId + 1;
}

// ---- Pozycje ----

void Library::addItem(const Item& item) {
    items.push_back(item);
}

bool Library::removeItem(int id) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->getId() == id) {
            items.erase(it);
            return true;
        }
    }
    return false;
}

bool Library::updateItem(const Item& updated) {
    for (auto& i : items) {
        if (i.getId() == updated.getId()) {
            i = updated;
            return true;
        }
    }
    return false;
}

compat::optional<Item> Library::getItem(int id) const {
    for (const auto& i : items) {
        if (i.getId() == id) return i;
    }
    return {};
}

const std::vector<Item>& Library::getAllItems() const {
    return items;
}

int Library::getNextItemId() const {
    int maxId = 0;
    for (const auto& i : items) {
        if (i.getId() > maxId) maxId = i.getId();
    }
    return maxId + 1;
}

// ---- Wyszukiwanie ----

std::vector<Item> Library::searchByTitle(const std::string& phrase) const {
    std::vector<Item> result;
    std::string lowerPhrase = phrase;
    std::transform(lowerPhrase.begin(), lowerPhrase.end(), lowerPhrase.begin(), ::tolower);
    for (const auto& i : items) {
        std::string lowerTitle = i.getTitle();
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        if (lowerTitle.find(lowerPhrase) != std::string::npos) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<Item> Library::getItemsByAuthor(int authorId) const {
    std::vector<Item> result;
    for (const auto& i : items) {
        if (i.getAuthorId() == authorId) result.push_back(i);
    }
    return result;
}

std::vector<Item> Library::getItemsByCategory(int categoryId) const {
    std::vector<Item> result;
    for (const auto& i : items) {
        if (i.getCategoryId() == categoryId) result.push_back(i);
    }
    return result;
}

std::vector<Item> Library::getItemsByStatus(const std::string& status) const {
    std::vector<Item> result;
    for (const auto& i : items) {
        if (i.getStatus() == status) result.push_back(i);
    }
    return result;
}

// ---- Statystyki ----

double Library::getAverageRating() const {
    if (items.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& i : items) sum += i.getRating();
    return sum / static_cast<double>(items.size());
}

double Library::getAverageRatingByCategory(int categoryId) const {
    double sum = 0.0;
    int count = 0;
    for (const auto& i : items) {
        if (i.getCategoryId() == categoryId) {
            sum += i.getRating();
            ++count;
        }
    }
    return (count > 0) ? sum / static_cast<double>(count) : 0.0;
}

int Library::countByCategory(int categoryId) const {
    int count = 0;
    for (const auto& i : items) {
        if (i.getCategoryId() == categoryId) ++count;
    }
    return count;
}

std::map<int, int> Library::countAllByCategory() const {
    std::map<int, int> result;
    for (const auto& i : items) {
        result[i.getCategoryId()]++;
    }
    return result;
}
