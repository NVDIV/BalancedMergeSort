#include <iostream>
#include <cstdlib> // For system()

int main() {
    // Command to remove files named run_*.txt in the current directory
    int result = system("del /Q run_*.txt"); // Use "del /Q run_*.txt" on Windows

    if (result == 0) {
        std::cout << "Temporary run files deleted successfully." << std::endl;
    } else {
        std::cerr << "Failed to delete temporary run files. Check your permissions or file existence." << std::endl;
    }

    return 0;
}
