#include <bits/stdc++.h>
using namespace std;

// ------------------ Build Inverted Index from Conflated Files ------------------ //
unordered_map<string, set<int>> buildInvertedIndex(const vector<string> &files) {
    unordered_map<string, set<int>> invertedIndex;

    for (int i = 0; i < files.size(); i++) {
        ifstream in(files[i]);
        string word;
        char colon;
        int freq;

        while (in >> word >> colon >> freq) {
            // normalize word
            transform(word.begin(), word.end(), word.begin(), ::tolower);

            if (!word.empty() && freq > 0) {
                invertedIndex[word].insert(i+1); // doc IDs are 1-based
            }
        }
    }

    return invertedIndex;
}

// ------------------ Print Index File ------------------ //
void printIndex(const unordered_map<string, set<int>> &index) {
    cout << "===== Inverted Index (from Conflated Files) =====\n\n";
    for (auto &entry : index) {
        cout << entry.first << " -> { ";
        for (int docId : entry.second) cout << "doc" << docId << " ";
        cout << "}\n";
    }
    cout << endl;
}

// ------------------ Query Processing ------------------ //
void processQuery(const unordered_map<string, set<int>> &index) {
    string query;
    cout << "Enter query word: ";
    cin >> query;

    // normalize
    transform(query.begin(), query.end(), query.begin(), ::tolower);

    if (index.find(query) != index.end()) {
        cout << "Query Results for \"" << query << "\": ";
        for (int docId : index.at(query)) cout << "doc" << docId << " ";
        cout << endl;
    } else {
        cout << "No documents found for query \"" << query << "\"\n";
    }
}

// ------------------ Main ------------------ //
int main() {
    vector<string> files = {"doc1.txt", "doc2.txt", "doc3.txt"};

    // Build inverted index
    unordered_map<string, set<int>> invertedIndex = buildInvertedIndex(files);

    // Print index file
    printIndex(invertedIndex);

    // Process query
    processQuery(invertedIndex);

    return 0;
}
