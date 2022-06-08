#include "utils.h"

#include <fstream>
#include <iostream>

std::string readFile(const std::string &filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Error: could not read file " << filePath << ". File does not exist"
                  << std::endl;
        return "";
    }

    std::string line;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

float randomFloat() {
    auto max = static_cast<float>(RAND_MAX);
    return (static_cast<float>(random()) / max);
}
