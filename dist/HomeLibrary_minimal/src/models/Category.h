#pragma once
#include <string>

class Category {
public:
    Category() = default;
    Category(int id, const std::string& name);

    int getId() const;
    std::string getName() const;

    void setName(const std::string& name);

    std::string toCSV() const;
    static Category fromCSV(const std::string& line);

private:
    int id = 0;
    std::string name;
};
