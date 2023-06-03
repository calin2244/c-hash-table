#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

int main() {
    std::unordered_set<std::string> seen;
    std::unordered_set<std::string> duplicates;
    std::string filename = "entries.txt";
    std::ifstream file(filename);

    if (!file) {
        std::cout << "Failed to open the file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string key = line.substr(0, line.find(' '));

        if (seen.count(key) > 0) {
            duplicates.insert(key);
        } else {
            seen.insert(key);
        }
    }

    file.close();

    if (!duplicates.empty()) {
        std::cout << "Duplicate keys found:\n";
        std::cout << duplicates.size() << '\n';
    } else {
        std::cout << "No duplicate keys found.\n";
    }

    return 0;
}
