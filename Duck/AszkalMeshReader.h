#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

static class AszklarMeshReader {

public:

    template<typename T>
    static  std::vector<T> ReadLines(int n, std::ifstream& file) {
        std::vector<T> numbers;

        std::string line;
        for (int i = 0; i < n && std::getline(file, line); i++) {
            std::istringstream iss(line);
            T number;

            while (iss >> number) {
                numbers.push_back(number);
            }

            if (!iss.eof() && iss.fail()) {
                std::cerr << "B³¹d podczas odczytu liczb z linii: " << line << std::endl;
                continue;
            }            
        }

        return numbers;
    }

    template<typename T>
    static T ReadOne(std::ifstream& file) {
        std::string line;  // Zmienna do przechowywania ka¿dej linii
        std::getline(file, line);
        std::istringstream iss(line);

        T number;
        iss >> number;

        if (!iss.eof() && iss.fail()) {
            std::cerr << "B³¹d podczas odczytu liczb z linii: " << line << std::endl;
        }

        return number;
    }
};