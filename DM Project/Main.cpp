#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

#include "Transaction.h"
#include "Utils.h"
#include "DataReader.h"
#include "Apriori.h"
#include "AssociationRules.h"
#include "Visualizer.h"

using namespace std;

// Print 3 lines of "=" between experiments
void printSeparator() {

    cout << "\n";

    for (int i = 0; i < 3; ++i) {
        cout << string(54, '=') << "\n";
    }

    cout << "\n";
}

// Run Apriori Experiment
void runAprioriExperiment(
    const Transactions& transactions,
    double minSup,
    double minConf,
    bool showChart = false
) {

    cout << "\n******************************************************\n";

    cout << "RUNNING APRIORI | Min Support: "
        << (minSup * 100)
        << "% | Min Confidence: "
        << (minConf * 100)
        << "%\n";

    cout << "******************************************************\n";

    // 1. Generate Frequent Itemsets
    FrequentLevels frequentItemsets =
        generateFrequentItemsets(transactions, minSup);

    if (frequentItemsets.empty()) {

        cout << "No frequent itemsets found.\n";

        return;
    }

    // Display L1, L2, ..., Lk
    for (size_t k = 0; k < frequentItemsets.size(); ++k) {

        cout << "\n--- Frequent Itemsets L"
            << (k + 1)
            << " ---\n";

        for (const auto& pair : frequentItemsets[k]) {

            cout << left
                << setw(25)
                << itemsetToString(pair.first)
                << " | Support: "
                << fixed << setprecision(2)
                << (pair.second * 100)
                << "% ("
                << round(pair.second * transactions.size())
                << "/"
                << transactions.size()
                << ")\n";
        }
    }

    // 2. Visualize Support
    if (showChart) {

        visualizeSupportBarChart(frequentItemsets);
    }

    // 3, 4, 5. Generate Association Rules
    vector<AssociationRule> strongRules;

    vector<AssociationRule> allRules =
        generateAssociationRules(
            frequentItemsets,
            minConf,
            strongRules
        );

    cout << "\n--- 3 & 5. All Generated Association Rules & Lift ---\n";

    if (allRules.empty()) {

        cout << "No association rules available.\n";
    }
    else {

        cout << left
            << setw(30) << "Rule (X -> Y)"
            << setw(12) << "Support"
            << setw(14) << "Confidence"
            << setw(10) << "Lift"
            << "Dependency\n";

        cout << string(80, '-') << "\n";

        for (const auto& r : allRules) {

            string ruleStr =
                itemsetToString(r.antecedent)
                + " -> "
                + itemsetToString(r.consequent);

            string dep =
                (r.lift > 1.0001)
                ? "Positive (+)"
                : (
                    (r.lift < 0.9999)
                    ? "Negative (-)"
                    : "Independent"
                    );

            cout << left
                << setw(30) << ruleStr
                << setw(12)
                << (to_string((int)round(r.support * 100)) + "%")
                << setw(14)
                << (to_string((int)round(r.confidence * 100)) + "%")
                << setw(10)
                << fixed << setprecision(2)
                << r.lift
                << dep << "\n";
        }
    }

    // 4. Display Strong Rules
    cout << "\n--- 4. Strong Rules (Confidence >= "
        << (minConf * 100)
        << "%) ---\n";

    if (strongRules.empty()) {

        cout << "No strong rules found.\n";
    }
    else {

        for (const auto& r : strongRules) {

            cout << itemsetToString(r.antecedent)
                << " -> "
                << itemsetToString(r.consequent)

                << " [Sup: "
                << fixed << setprecision(1)
                << (r.support * 100)
                << "%, "

                << "Conf: "
                << (r.confidence * 100)
                << "%, "

                << "Lift: "
                << setprecision(2)
                << r.lift
                << "]\n";
        }
    }
}

// Main Function
int main() {

    string filename = "../Horizontal_Format.xlsx";

    cout << "Loading transactions from: "
        << filename << "...\n";

    Transactions transactions =
        loadTransactions(filename);

    cout << "Successfully loaded "
        << transactions.size()
        << " transactions.\n";

    // Experiment 1
    runAprioriExperiment(transactions, 0.40, 0.70, true);
    printSeparator();

    // Experiment 2
    runAprioriExperiment(transactions, 0.60, 0.80, false);
    printSeparator();

    // Custom Experiment
    double userSup, userConf;

    cout << "Enter custom Min Support (e.g. 0.5 for 50%): ";

    if (cin >> userSup) {

        cout << "Enter custom Min Confidence (e.g. 0.75 for 75%): ";

        if (cin >> userConf) {

            runAprioriExperiment(transactions, userSup, userConf, true);
            printSeparator();
        }
    }

    return 0;
}