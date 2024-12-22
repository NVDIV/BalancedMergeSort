#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>

struct MinHeapNode {
    int value;
    int fileIndex;
};

struct Compare {
    bool operator()(const MinHeapNode& a, const MinHeapNode& b) {
        return a.value > b.value;
    }
};

// Function to split the input file into sorted runs
void splitIntoRuns(const std::string& inputFile, size_t memorySize, int numFiles, std::vector<std::string>& tempFiles) {
    std::ifstream input(inputFile);
    std::vector<int> buffer(memorySize);

    int fileIndex = 0;
    while (input) {
        size_t count = 0;
        while (count < memorySize && input >> buffer[count]) {
            count++;
        }
        if (count == 0) break;

        std::sort(buffer.begin(), buffer.begin() + count);

        std::string tempFile = "run_" + std::to_string(fileIndex++) + ".txt";
        tempFiles.push_back(tempFile);

        std::ofstream tempOutput(tempFile);
        for (size_t i = 0; i < count; i++) {
            tempOutput << buffer[i] << " ";
        }
    }
    input.close();
}

// Function to merge runs
void multiwayMerge(const std::vector<std::string>& tempFiles, const std::string& outputFile, int numFiles) {
    std::ofstream output(outputFile);
    std::priority_queue<MinHeapNode, std::vector<MinHeapNode>, Compare> minHeap;

    std::vector<std::ifstream> inputs(tempFiles.size());
    for (int i = 0; i < tempFiles.size(); i++) {
        inputs[i].open(tempFiles[i]);
        int value;
        if (inputs[i] >> value) {
            minHeap.push({value, i});
        }
    }

    while (!minHeap.empty()) {
        MinHeapNode minNode = minHeap.top();
        minHeap.pop();
        output << minNode.value << " ";

        int value;
        if (inputs[minNode.fileIndex] >> value) {
            minHeap.push({value, minNode.fileIndex});
        }
    }

    for (auto& input : inputs) {
        input.close();
    }
    output.close();
}

int main() {

    auto start = std::chrono::high_resolution_clock::now();

    const std::string inputFile = "input.txt";
    const std::string outputFile = "sorted_output.txt";
    const size_t memorySize = 100; // Number of elements in memory
    const int numFiles = 3; // Number of files for multiway merge

    std::vector<std::string> tempFiles;

    // Step 1: Split input file into runs
    splitIntoRuns(inputFile, memorySize, numFiles, tempFiles);

    // Step 2: Perform multiway merge
    multiwayMerge(tempFiles, outputFile, numFiles);

    std::cout << "Sorting completed! Output saved to " << outputFile << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    return 0;
}
