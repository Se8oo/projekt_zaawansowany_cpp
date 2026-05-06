#pragma once
#include <vector>
#include <optional>
#include <map>
#include "../models/Author.h"
#include "../models/Category.h"
#include "../models/Item.h"

class Library {
public:
    // ---- Autorzy ----
    void addAuthor(const Author& author);
    bool removeAuthor(int id);
    bool updateAuthor(const Author& author);
    std::optional<Author> getAuthor(int id) const;
    const std::vector<Author>& getAllAuthors() const;
    int getNextAuthorId() const;

    // ---- Kategorie ----
    void addCategory(const Category& category);
    bool removeCategory(int id);
    bool updateCategory(const Category& category);
    std::optional<Category> getCategory(int id) const;
    const std::vector<Category>& getAllCategories() const;
    int getNextCategoryId() const;

    // ---- Pozycje ----
    void addItem(const Item& item);
    bool removeItem(int id);
    bool updateItem(const Item& item);
    std::optional<Item> getItem(int id) const;
    const std::vector<Item>& getAllItems() const;
    int getNextItemId() const;

    // ---- Wyszukiwanie ----
    std::vector<Item> searchByTitle(const std::string& phrase) const;
    std::vector<Item> getItemsByAuthor(int authorId) const;
    std::vector<Item> getItemsByCategory(int categoryId) const;

    // ---- Statystyki ----
    double getAverageRating() const;
    double getAverageRatingByCategory(int categoryId) const;
    int countByCategory(int categoryId) const;
    std::map<int, int> countAllByCategory() const;

private:
    std::vector<Author> authors;
    std::vector<Category> categories;
    std::vector<Item> items;
};
