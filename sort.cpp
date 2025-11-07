#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

// Чтение чисел из файла
std::vector<int> readNumbersFromFile(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<int> numbers;
    int number;
    while (file >> number) {
        numbers.push_back(number);
    }
    return numbers;
}

// Запись чисел в файл
void writeNumbersToFile(const std::string &filename, const std::vector<int> &numbers) {
    std::ofstream file(filename);
    for (int number : numbers) {
        file << number << " ";
    }
}

// Сортировка чисел в файле
void sortFile(const std::string &filename) {
    std::vector<int> numbers = readNumbersFromFile(filename);
    std::sort(numbers.begin(), numbers.end());
    writeNumbersToFile(filename, numbers);
}

// Чтение порции чисел из файла
std::vector<int> readPartialNumbersFromFile(const std::string &filename, int start, int count) {
    std::ifstream file(filename);
    std::vector<int> numbers;
    int number;
    int currentIndex = 0;
    while (file >> number) {
        if (currentIndex >= start && currentIndex < start + count) {
            numbers.push_back(number);
        }
        currentIndex++;
    }
    return numbers;
}

// Запись части чисел в файл
void writePartialNumbersToFile(const std::string &filename, const std::vector<int> &numbers, bool append = true) {
    std::ofstream file(filename, append ? std::ios::app : std::ios::out);
    for (int number : numbers) {
        file << number << " ";
    }
}

int main() {
    std::string inputFilename = "random_numbers.txt";
    std::string outputFilename = "sorted_numbers.txt";

    // Шаг 1: Чтение чисел из файла
    std::vector<int> numbers = readNumbersFromFile(inputFilename);
    int N = numbers.size();
    int S = N / 4;

    // Шаг 2: Разделение чисел на 4 части и сортировка каждой части
    std::vector<std::string> partFileNames;
    partFileNames.push_back("part1.txt");
    partFileNames.push_back("part2.txt");
    partFileNames.push_back("part3.txt");
    partFileNames.push_back("part4.txt");

    for (int i = 0; i < 4; ++i) {
        int start = i * S;
        int count = (i == 3) ? (N - start) : S; // Последняя часть может быть длиннее
        std::vector<int> part(numbers.begin() + start, numbers.begin() + start + count);
        writeNumbersToFile(partFileNames[i], part);
        sortFile(partFileNames[i]);
    }

    // Шаг 3: Слияние частей в итоговый файл
    std::ofstream outputFile(outputFilename);
    int chunkSize = S / 4;
    std::vector<int> buffer;

    for (int i = 0; i < N; i += chunkSize) {
        buffer.clear();
        for (const std::string &partFileName : partFileNames) {
            std::vector<int> part = readPartialNumbersFromFile(partFileName, i, chunkSize);
            buffer.insert(buffer.end(), part.begin(), part.end());
        }
        std::sort(buffer.begin(), buffer.end());
        writePartialNumbersToFile(outputFilename, buffer);
    }

    std::cout << "Сортировка завершена. Результаты сохранены в " << outputFilename << std::endl;
    return 0;
}
