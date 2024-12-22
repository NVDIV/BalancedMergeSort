#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

int main() {
    std::ofstream file("input.txt");
    std::srand(std::time(nullptr));

    size_t numElements = 2000000; // Наприклад, 1 мільйон чисел
    for (size_t i = 0; i < numElements; i++) {
        file << (std::rand() % 2000000) << " ";
    }

    file.close();
    std::cout << "File generated: input.txt" << std::endl;
    return 0;
}
