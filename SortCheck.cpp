#include <iostream>
#include <fstream>
#include <vector>

bool verifySorted(const std::string& filename) {
    std::ifstream file(filename);
    int prev, curr;

    if (!(file >> prev)) {
        std::cerr << "File is empty or invalid." << std::endl;
        return false;
    }

    while (file >> curr) {
        if (curr < prev) {
            std::cerr << "File is not sorted correctly." << std::endl;
            return false;
        }
        prev = curr;
    }

    std::cout << "File is correctly sorted." << std::endl;
    return true;
}

int main() {
    verifySorted("sorted_output.txt");
    return 0;
}
