#pragma once
#include <string>

class Author {
public:
    Author() = default;
    Author(int id, const std::string& firstName, const std::string& lastName);

    int getId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getFullName() const;

    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);

    std::string toCSV() const;
    static Author fromCSV(const std::string& line);

private:
    int id = 0;
    std::string firstName;
    std::string lastName;
};
