#pragma once
#include <map>
#include <string>
#include <vector>

#if defined(__has_include)
#  if __has_include(<optional>)
#    include <optional>
namespace compat {
    template <typename T>
    using optional = std::optional<T>;
}
#  elif __has_include(<experimental/optional>)
#    include <experimental/optional>
namespace compat {
    template <typename T>
    using optional = std::experimental::optional<T>;
}
#  else
#    error "<optional> or <experimental/optional> is required"
#  endif
#else
#  include <experimental/optional>
namespace compat {
    template <typename T>
    using optional = std::experimental::optional<T>;
}
#endif

#include "../models/Author.h"
#include "../models/Category.h"
#include "../models/Item.h"

class Library {
public:
    // ---- Autorzy ----
    void addAuthor(const Author& author);
    bool removeAuthor(int id);
    bool updateAuthor(const Author& author);
    compat::optional<Author> getAuthor(int id) const;
    const std::vector<Author>& getAllAuthors() const;
    int getNextAuthorId() const;

    // ---- Kategorie ----
    void addCategory(const Category& category);
    bool removeCategory(int id);
    bool updateCategory(const Category& category);
    compat::optional<Category> getCategory(int id) const;
    const std::vector<Category>& getAllCategories() const;
    int getNextCategoryId() const;

    // ---- Pozycje ----
    void addItem(const Item& item);
    bool removeItem(int id);
    bool updateItem(const Item& item);
    compat::optional<Item> getItem(int id) const;
    const std::vector<Item>& getAllItems() const;
    int getNextItemId() const;

    // ---- Wyszukiwanie ----
    std::vector<Item> searchByTitle(const std::string& phrase) const;
    std::vector<Item> getItemsByAuthor(int authorId) const;
    std::vector<Item> getItemsByCategory(int categoryId) const;
    std::vector<Item> getItemsByStatus(const std::string& status) const;

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
