
#include "DataReader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Remove extra spaces and quotation marks
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n\"'");

    if (string::npos == first) {
        return "";
    }

    size_t last = str.find_last_not_of(" \t\r\n\"'");

    return str.substr(first, (last - first + 1));
}

// Load transactions from CSV or Excel
Transactions loadTransactions(const string& filename) {

    Transactions transactions;

    string csvFile = filename;

    // Convert XLSX to CSV using Python
    if (filename.find(".xlsx") != string::npos) {

        csvFile = "temp_data.csv";

        string cmd =
            "python -c \"import pandas as pd; "
            "pd.read_excel('" + filename +
            "').to_csv('" + csvFile +
            "', index=False)\" 2>nul";

        int status = system(cmd.c_str());

        if (status != 0) {
            cout << "[Warning]: Please save the Excel file "
                << "as CSV and pass its name to the program.\n";

            exit(1);
        }
    }

    ifstream file(csvFile);

    if (!file.is_open()) {
        cerr << "Error: Cannot open file "
            << csvFile << endl;

        exit(1);
    }

    string line;
    bool isHeader = true;

    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        // Skip header if detected
        if (isHeader) {

            isHeader = false;

            string lowerLine = line;

            transform(
                lowerLine.begin(),
                lowerLine.end(),
                lowerLine.begin(),
                ::tolower
            );

            if (
                lowerLine.find("tid") != string::npos ||
                lowerLine.find("item") != string::npos ||
                lowerLine.find("trans") != string::npos
                ) {
                continue;
            }
        }

        Itemset currentTransaction;

        stringstream ss(line);

        string cell;

        int colIndex = 0;

        while (getline(ss, cell, ',')) {

            cell = trim(cell);

            if (cell.empty()) {
                continue;
            }

            // Skip first column (TID)
            if (colIndex == 0) {
                colIndex++;
                continue;
            }

            // Split items separated by semicolons
            stringstream itemStream(cell);

            string item;

            while (getline(itemStream, item, ';')) {

                item = trim(item);

                if (!item.empty()) {
                    currentTransaction.insert(item);
                }
            }

            colIndex++;
        }

        if (!currentTransaction.empty()) {
            transactions.push_back(currentTransaction);
        }
    }

    file.close();

    return transactions;
}
