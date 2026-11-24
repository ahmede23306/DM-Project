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

void printSeparator() {

    cout << "\n";

    for (int i = 0; i < 3; ++i) {
        cout << string(54, '=') << "\n";
    }

    cout << "\n";
}

void runAprioriExperiment(const Transactions& transactions, double minSup, double minConf, bool showChart = false ) {

    cout << "\n******************************************************\n";
    cout << "RUNNING APRIORI | Min Support: "<< (minSup * 100) << "% | Min Confidence: " << (minConf * 100)<< "%\n";
    cout << "******************************************************\n";

    FrequentLevels frequentItemsets = generateFrequentItemsets(transactions,minSup);

    if (frequentItemsets.empty()) {
        cout << "No frequent itemsets found.\n";
        return;
    }

    for (size_t k = 0; k < frequentItemsets.size(); ++k) {

        cout << "\n--- Frequent Itemsets L" << (k + 1)<< " ---\n";

        for (const auto& pair : frequentItemsets[k]) {

            cout << left << setw(25) << itemsetToString(pair.first) << " | Support: " << fixed << setprecision(2) << (pair.second * 100) << "% ("
                << round(pair.second * transactions.size())<< "/" << transactions.size() << ")\n";
        }
    }

    if (showChart) {
        visualizeSupportBarChart(frequentItemsets, minSup);
    }

    vector<AssociationRule> strongRules;
    vector<AssociationRule> allRules = generateAssociationRules( frequentItemsets, minConf, strongRules );

    cout << "\n--- 3 & 5. All Generated Association Rules & Lift ---\n";

    if (allRules.empty()) {
        cout << "No association rules available.\n";
    }
    else {

        cout << left << setw(30) << "Rule (X -> Y)" << setw(12) << "Support" << setw(14) << "Confidence" << setw(10) << "Lift" << "Dependency\n";

        cout << string(80, '-') << "\n";

        for (const auto& r : allRules) {

            string ruleStr = itemsetToString(r.antecedent)+ " -> "+ itemsetToString(r.consequent);

            string dep = (r.lift > 1.0001)? "Positive (+)": ((r.lift < 0.9999)? "Negative (-)": "Independent");

            cout << left<< setw(30) << ruleStr<< setw(12)<< (to_string((int)round(r.support * 100)) + "%")<< setw(14)<< (to_string((int)round(r.confidence * 100)) + "%")
                << setw(10)<< fixed << setprecision(2)<< r.lift<< dep << "\n";
        }
    }

    cout << "\n--- 4. Strong Rules (Confidence >= " << (minConf * 100)<< "%) ---\n";

    if (strongRules.empty()) {

        cout << "No strong rules found.\n";
    }
    else {

        for (const auto& r : strongRules) {
            cout << itemsetToString(r.antecedent)<< " -> "<< itemsetToString(r.consequent)

                << " [Sup: " << fixed << setprecision(1)<< (r.support * 100)<< "%, "
                << "Conf: "<< (r.confidence * 100) << "%, "
                << "Lift: "<< setprecision(2)<< r.lift << "]\n";
        }
    }
}

bool askValue(const string& prompt, double& value) {

    while (true) {
        cout << prompt;
        if (!(cin >> value)) {
            return false;
        }
        if (value > 0.0 && value <= 1.0) {
            return true;
        }
        cout << "Invalid value. Enter a number between 0 and 1.\n";
    }
}

int main() {

    string filename = "../Bank_Horizontal_Format.csv";
    cout << "Loading transactions from: "<< filename << "...\n";
    Transactions transactions = loadTransactions(filename);
    cout << "Successfully loaded " << transactions.size() << " transactions.\n\n";

    char again = 'y';
    while (again == 'y' || again == 'Y') {

        double minSup, minConf;
        if (!askValue("Enter Min Support (e.g. 0.5 for 50%): ", minSup)) {
            break;
        }
        if (!askValue("Enter Min Confidence (e.g. 0.75 for 75%): ", minConf)) {
            break;
        }

        runAprioriExperiment(transactions,minSup,minConf,true);
        printSeparator();

        cout << "Run another trial? (y/n): ";
        if (!(cin >> again)) {
            break;
        }
        cout << "\n";
    }
    return 0;
}