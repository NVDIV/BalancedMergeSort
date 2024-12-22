#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>

const size_t BUFFER_SIZE = 4096; // Розмір буфера (кількість елементів у пам'яті)

// Структура для мін-купи
struct MinHeapNode {
    int value;        // Значення елемента
    int fileIndex;    // Індекс файлу, з якого прийшло значення
};

// Компаратор для мін-купи
struct Compare {
    bool operator()(const MinHeapNode& a, const MinHeapNode& b) {
        return a.value > b.value; // Найменший елемент зверху
    }
};

// Функція розбиття вхідного файлу на серії
void splitIntoRuns(const std::string& inputFile, size_t memorySize, std::vector<std::string>& tempFiles) {
    std::ifstream input(inputFile);
    std::vector<int> buffer(memorySize);

    int fileIndex = 0;
    while (input) {
        size_t count = 0;
        // Зчитування блоку у буфер
        while (count < memorySize && input >> buffer[count]) {
            count++;
        }
        if (count == 0) break;

        // Сортування серії
        std::sort(buffer.begin(), buffer.begin() + count);

        // Збереження серії у тимчасовий файл
        std::string tempFile = "run_" + std::to_string(fileIndex++) + ".txt";
        tempFiles.push_back(tempFile);

        std::ofstream tempOutput(tempFile);
        for (size_t i = 0; i < count; i++) {
            tempOutput << buffer[i] << " ";
        }
        tempOutput.close();
    }
    input.close();
}

// Функція буферизованого злиття серій
void bufferedMerge(const std::vector<std::string>& tempFiles, const std::string& outputFile) {
    std::ofstream output(outputFile);
    std::priority_queue<MinHeapNode, std::vector<MinHeapNode>, Compare> minHeap;

    // Відкрити всі тимчасові файли
    std::vector<std::ifstream> inputs(tempFiles.size());
    std::vector<std::vector<int>> buffers(tempFiles.size()); // Буфери для файлів
    std::vector<size_t> bufferIndexes(tempFiles.size(), 0);  // Поточні індекси у буферах
    std::vector<size_t> bufferSizes(tempFiles.size(), 0);    // Розміри буферів

    for (int i = 0; i < tempFiles.size(); i++) {
        inputs[i].open(tempFiles[i]);

        // Заповнити початкові буфери
        buffers[i].resize(BUFFER_SIZE);
        bufferSizes[i] = 0;
        bufferIndexes[i] = 0;

        for (size_t j = 0; j < BUFFER_SIZE && inputs[i] >> buffers[i][j]; j++) {
            bufferSizes[i]++;
        }

        // Додати перший елемент буфера до купи
        if (bufferSizes[i] > 0) {
            minHeap.push({buffers[i][0], i});
        }
    }

    // Процес злиття
    while (!minHeap.empty()) {
        MinHeapNode minNode = minHeap.top();
        minHeap.pop();

        // Записати мінімальний елемент у вихідний файл
        output << minNode.value << " ";

        int fileIndex = minNode.fileIndex;
        bufferIndexes[fileIndex]++;

        // Якщо буфер порожній, заповнити його з файлу
        if (bufferIndexes[fileIndex] >= bufferSizes[fileIndex]) {
            bufferIndexes[fileIndex] = 0;
            bufferSizes[fileIndex] = 0;

            for (size_t j = 0; j < BUFFER_SIZE && inputs[fileIndex] >> buffers[fileIndex][j]; j++) {
                bufferSizes[fileIndex]++;
            }
        }

        // Якщо у буфері ще є елементи, додати до купи
        if (bufferIndexes[fileIndex] < bufferSizes[fileIndex]) {
            minHeap.push({buffers[fileIndex][bufferIndexes[fileIndex]], fileIndex});
        }
    }

    // Закрити всі файли
    for (auto& input : inputs) {
        input.close();
    }
    output.close();
}

int main() {

    auto start = std::chrono::high_resolution_clock::now();

    const std::string inputFile = "input.txt";     // Вхідний файл
    const std::string outputFile = "sorted_output.txt"; // Вихідний файл
    const size_t memorySize = 1000; // Максимальний розмір пам'яті для однієї серії

    std::vector<std::string> tempFiles;

    // Крок 1: Розділення на серії
    splitIntoRuns(inputFile, memorySize, tempFiles);

    // Крок 2: Буферизоване багатошляхове злиття
    bufferedMerge(tempFiles, outputFile);

    std::cout << "Sorting completed! Result saved to " << outputFile << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    return 0;
}
