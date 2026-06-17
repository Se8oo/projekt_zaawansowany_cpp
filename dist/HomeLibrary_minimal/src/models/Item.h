#pragma once
#include <string>

class Item {
public:
    Item() = default;
    Item(int id, const std::string& title, int authorId, int categoryId,
         double rating, const std::string& status, const std::string& description = "");

    int getId() const;
    std::string getTitle() const;
    int getAuthorId() const;
    int getCategoryId() const;
    double getRating() const;
    std::string getStatus() const;
    std::string getDescription() const;

    void setTitle(const std::string& title);
    void setAuthorId(int authorId);
    void setCategoryId(int categoryId);
    void setRating(double rating);
    void setStatus(const std::string& status);
    void setDescription(const std::string& description);

    std::string toCSV() const;
    static Item fromCSV(const std::string& line);

private:
    int id = 0;
    std::string title;
    int authorId = 0;
    int categoryId = 0;
    double rating = 0.0;
    std::string status;
    std::string description;
};
