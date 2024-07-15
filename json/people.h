#ifndef PEOPLE_H
#define PEOPLE_H

#include <string>
#include <vector>

class people2
{
public:
    int age;
    people2() = default;
    people2(int Age): age(Age) {}
};

class people
{
public:
    std::string name;
    int age;
    std:: string sex;
    std::vector<std::string>hobby;
    int friend_age;
    std::string friend_name;
    std::string friend_sex;
public:
    people() = default;
    people(std::string, int , std::vector<std::string>, std::string, int ,std::string, std::string);
    void print();
};

people::people(std::string Name, int Age, std::vector<std::string> Hobby, std::string Sex, int Friend_age, std::string Friend_name, std::string Friend_sex)
    :name(Name), age(Age), sex(Sex), hobby(Hobby), friend_age(Friend_age), friend_name(Friend_name), friend_sex(Friend_sex)
{   }

void people::print()
{
    std::cout << "name : " << name << std::endl;
    std::cout << "age : " << age << std::endl;
    std::cout << "sex : " << sex << std::endl;
    std::cout << "hobby : ";
    for(auto h : hobby)
    std::cout << h << " ";
    std::cout << "\n";
    std::cout << "friend_name : " << friend_name << std::endl;
    std::cout << "friend_age : " << friend_age << std::endl;
    std::cout << "friend_sex : " << friend_sex << std::endl;
}

#endif