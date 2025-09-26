#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <set>
#include <sstream>
#include <cmath> // For pow() function

using namespace std;

// Function to calculate the F-measure for a given precision, recall, and beta
double calculate_f_measure(double precision, double recall, double beta = 1.0) {
    if (precision + recall == 0) {
        return 0.0;
    }
    double beta_sq = pow(beta, 2);
    return (1 + beta_sq) * (precision * recall) / ((beta_sq * precision) + recall);
}

int main() {
    // --- Data from the problem ---
    set<string> Rq1 = {"d3", "d5", "d9", "d25", "d39", "d44", "d56", "d71", "d89", "d123"};
    vector<string> retrieved = {"d123", "d84", "d56", "d6", "d8",
                                "d9", "d511", "d129", "d187", "d25",
                                "d38", "d48", "d250", "d113", "d3"};

    // Vectors to store P and R values for later use
    vector<double> precision_vals;
    vector<double> recall_vals;

    // --- Part 1: Generate and Display the Precision/Recall Table ---
    cout << left << setw(65) << "Documents"
         << " | " << right << setw(7) << "|Ra|"
         << " | " << right << setw(7) << "|A|"
         << " | " << right << setw(12) << "Precision(%)"
         << " | " << right << setw(10) << "Recall(%)" << " |" << endl;
    
    int relevantCount = 0;
    for (int i = 0; i < retrieved.size(); i++) {
        if (Rq1.count(retrieved[i])) {
            relevantCount++;
        }
        
        double precision = (i + 1 == 0) ? 0 : (double)relevantCount / (i + 1);
        double recall = (Rq1.empty()) ? 0 : (double)relevantCount / Rq1.size();
        
        // Store raw values (not percentages)
        precision_vals.push_back(precision);
        recall_vals.push_back(recall);

        stringstream doc_list_ss;
        for (int j = 0; j <= i; j++) {
            doc_list_ss << retrieved[j] << (j < i ? ", " : "");
        }

        cout << left << setw(65) << doc_list_ss.str()
             << " | " << right << fixed << setprecision(2)
             << setw(7) << (double)relevantCount
             << " | " << setw(7) << (double)(i + 1)
             << " | " << setw(12) << precision * 100.0
             << " | " << setw(10) << recall * 100.0 << " |" << endl;
    }

    // --- Part 2: Calculate F-Measure and E-Measure ---
    cout << "\n--- Harmonic mean and E-value ---\n";
    cout << "Enter value of j (0 - 14) to find F(j) and E(j): ";
    int j;
    cin >> j;

    if (j >= 0 && j < retrieved.size()) {
        double p = precision_vals[j];
        double r = recall_vals[j];

        // Calculate F1-score (beta = 1), as it's the standard harmonic mean
        double f1_score = calculate_f_measure(p, r, 1.0);
        
        cout << "\nHarmonic mean (F1) is: " << fixed << setprecision(2) << f1_score << endl;
        
        // --- MODIFICATION START ---
        
        // Calculate E-values for the required beta values
        // E = 1 - F_beta
        double e_val_recall_favored = 1.0 - calculate_f_measure(p, r, 2.0);  // beta > 1 (favors recall)
        double e_val_beta_zero = 1.0 - p;                                     // For beta=0, F-measure is just precision 'p'
        double e_val_precision_favored = 1.0 - calculate_f_measure(p, r, 0.5);// beta < 1 (favors precision)

        cout << "\n           E-Value           " << endl;
        cout << "-------------------------------" << endl;
        // Modified the header to show b=0
        cout << "|  b > 1   |  b = 0   |  b < 1   |" << endl;
        cout << "-------------------------------" << endl;
        // Modified the output to print the new e_val_beta_zero
        cout << "|  " << setw(6) << fixed << setprecision(2) << e_val_recall_favored
             << "  |  " << setw(6) << e_val_beta_zero
             << "  |  " << setw(6) << e_val_precision_favored << "  |" << endl;
        cout << "-------------------------------" << endl;

        // --- MODIFICATION END ---

    } else {
        cout << "Invalid input. Please enter a number between 0 and 14." << endl;
    }

    return 0;
}