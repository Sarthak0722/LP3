#include <bits/stdc++.h>
using namespace std;

// -------------------- Utility Functions -------------------- //
unordered_map<string, int> readRepresentative(const string &filename) {
    unordered_map<string, int> freq;
    ifstream in(filename);
    string word;
    int val;
    char colon;
    while (in >> word >> colon >> val) {
        freq[word] = val;
    }
    return freq;
}

double cosineSim(const unordered_map<string, int> &A,
                 const unordered_map<string, int> &B) {
    double dot = 0.0, magA = 0.0, magB = 0.0;
    for (auto &p : A) {
        if (B.count(p.first)) dot += p.second * B.at(p.first);
        magA += p.second * p.second;
    }
    for (auto &p : B) magB += p.second * p.second;

    if (magA == 0 || magB == 0) return 0.0;
    return dot / (sqrt(magA) * sqrt(magB));
}

void updateCentroid(unordered_map<string, double> &centroid,
                    const unordered_map<string, int> &doc,
                    int clusterSize) {
    for (auto &p : doc) {
        centroid[p.first] = ((centroid[p.first] * (clusterSize - 1)) + p.second) / clusterSize;
    }
}

// -------------------- Main Algorithm -------------------- //
int main() {
    vector<string> filenames = {"doc1.txt","doc2.txt","doc3.txt","doc4.txt","doc5.txt"};

    // Load document representatives
    vector<unordered_map<string,int>> docs;
    for (string file : filenames) {
        docs.push_back(readRepresentative(file));
    }

    // Single Pass Clustering
    double threshold = 0.2; // similarity threshold
    vector<vector<int>> clusters;
    vector<unordered_map<string,double>> centroids;

    for (int i = 0; i < docs.size(); i++) {
        bool assigned = false;
        int bestCluster = -1;
        double bestSim = 0.0;

        for (int c = 0; c < centroids.size(); c++) {
            unordered_map<string,int> centroidInt;
            for (auto &p : centroids[c]) centroidInt[p.first] = (int)round(p.second);

            double sim = cosineSim(docs[i], centroidInt);
            if (sim > bestSim) {
                bestSim = sim;
                bestCluster = c;
            }
        }

        if (bestSim >= threshold) {
            clusters[bestCluster].push_back(i);
            updateCentroid(centroids[bestCluster], docs[i], clusters[bestCluster].size());
        } else {
            clusters.push_back({i});
            unordered_map<string,double> newCentroid;
            for (auto &p : docs[i]) newCentroid[p.first] = p.second;
            centroids.push_back(newCentroid);
        }
    }

    // Output
    cout << "===== Document Clusters (Single Pass Algorithm) =====\n\n";
    for (int c = 0; c < clusters.size(); c++) {
        cout << "Cluster " << c+1 << ": ";
        for (int id : clusters[c]) {
            cout << filenames[id] << " ";
        }
        cout << "\n";
    }
}
