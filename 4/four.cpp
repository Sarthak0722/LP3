#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Required for stream manipulators like setw, setprecision
#include <set>
#include <sstream> // Required for stringstream

using namespace std;

int main() {
    // Relevant documents for query q1
    set<string> Rq1 = {"d3", "d5", "d9", "d25", "d39", "d44", "d56", "d71", "d89", "d123"};

    // Documents returned by the IR algorithm in ranked order
    vector<string> retrieved = {"d123", "d84", "d56", "d6", "d8",
                                "d9", "d511", "d129", "d187", "d25",
                                "d38", "d48", "d250", "d113", "d3"};

    int relevantCount = 0;

    // --- MODIFICATION 1: Properly formatted header ---
    // Use left alignment for the 'Documents' header and right alignment for all numerical columns.
    // Consistent setw() values ensure columns are aligned.
    cout << left << setw(65) << "Documents"
         << " | " << right << setw(7) << "|Ra|"
         << " | " << right << setw(7) << "|A|"
         // A wider width for Precision is needed to fit the header and values like 100.00
         << " | " << right << setw(12) << "Precision(%)"
         << " | " << right << setw(10) << "Recall(%)" << " |" << endl;

    for (int i = 0; i < retrieved.size(); i++) {
        // Using .count() is a slightly cleaner way to check for existence in a set
        if (Rq1.count(retrieved[i])) {
            relevantCount++;
        }
        
        double precision = (double)relevantCount / (i + 1) * 100.0;
        double recall = (double)relevantCount / Rq1.size() * 100.0;

        // --- MODIFICATION 2: Build document string first ---
        // Use a stringstream to build the cumulative list of documents.
        // This allows us to print it into a fixed-width column easily.
        stringstream doc_list_ss;
        for (int j = 0; j <= i; j++) {
            doc_list_ss << retrieved[j] << (j < i ? ", " : "");
        }

        // --- MODIFICATION 3: Properly formatted data rows ---
        // Print the document list with left alignment.
        cout << left << setw(65) << doc_list_ss.str();

        // Print all numerical values with right alignment, fixed-point notation, and 2 decimal places.
        // Casting integers to double ensures they are printed in the format 'X.00'.
        cout << " | " << right << fixed << setprecision(2)
             << setw(7) << (double)relevantCount
             << " | " << setw(7) << (double)(i + 1)
             << " | " << setw(12) << precision
             << " | " << setw(10) << recall << " |" << endl;
    }

    return 0;
}