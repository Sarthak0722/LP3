#include <bits/stdc++.h>
using namespace std;

// Function to load stopwords into a set
set<string> loadStopWords(const string &filename) {
    set<string> stopWords;
    ifstream file(filename);
    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        stopWords.insert(word);
    }
    return stopWords;
}

// Function to preprocess and stem words (conflation)
string stemWord(string word) {
    // Convert to lowercase
    transform(word.begin(), word.end(), word.begin(), ::tolower);

    // Remove punctuation
    word.erase(remove_if(word.begin(), word.end(),
                         [](char c) { return ispunct(c); }),
               word.end());

    if (word.empty()) return "";

    // Apply simple stemming rules
    if (word.size() > 4) {
        if (word.substr(word.size() - 3) == "ing")
            word = word.substr(0, word.size() - 3);
        else if (word.substr(word.size() - 2) == "ed")
            word = word.substr(0, word.size() - 2);
    }

    if (word.size() > 3 && word.back() == 's') {
        word.pop_back();
    }

    return word;
}

int main() {
    string stopFile = "stopwords.txt";
    string inputFile = "document.txt";
    string outputFile = "output.txt";

    // Load stopwords
    set<string> stopWords = loadStopWords(stopFile);

    // Read input document
    ifstream doc(inputFile);
    unordered_map<string, int> freq;
    string word;

    while (doc >> word) {
        // Preprocess & stem word
        word = stemWord(word);
        if (word.empty()) continue;

        // Skip stopwords
        if (stopWords.find(word) != stopWords.end()) continue;

        // Count frequency
        freq[word]++;
    }

    // Sort words by frequency
    vector<pair<string, int>> sortedFreq(freq.begin(), freq.end());
    sort(sortedFreq.begin(), sortedFreq.end(),
         [](auto &a, auto &b) { return b.second > a.second; });

    // Write output
    ofstream out(outputFile);
    cout << "Representative Words (without stopwords, stemmed):\n\n";
    for (auto &p : sortedFreq) {
        cout << p.first << " : " << p.second << "\n";
        out << p.first << " : " << p.second << "\n";
    }

    cout << "\n Output written to " << outputFile << endl;
    return 0;
}
