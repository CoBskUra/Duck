#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "RobotsPartInfo.h"

static class AszklarMeshReader {

public:

    static RobotsPartInfo Read(const char* meshSource) {
        RobotsPartInfo output;
        std::ifstream file(meshSource);

        if (!file.is_open()) {
            std::cerr << "Nie mo¿na otworzyæ pliku!" << std::endl;
            exit(1);
        }

        output.vertexPositions_numberOfFloatsInDataUnit = 3;
        int n = ReadOne<int>(file);
        output.vertexPositions = ReadLines<float>(n, file);

        output.vertexId_And_normals_numberOfFloatsInDataUnit = 4;
        n = ReadOne<int>(file);
        output.vertexId_And_normals = ReadLines<float>(n, file);

        output.triangles_numberOfFloatsInDataUnit = 3;
        n = ReadOne<int>(file);
        output.triangles = ReadLines<GLuint>(n, file);

        output.edges_numberOfFloatsInDataUnit = 4;
        n = ReadOne<int>(file);
        output.edges = ReadLines<GLuint>(n, file);

        file.close();

        return output;
    }

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