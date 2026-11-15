
#include "Visualizer.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

// Visualize Support using Console and HTML
void visualizeSupportBarChart(
    const FrequentLevels& allFrequentItemsets
) {

    cout << "\n======================================================\n";
    cout << "2. BAR CHART: FREQUENT ITEMSETS SUPPORT\n";
    cout << "======================================================\n";

    ofstream html("support_chart.html");

    html << "<html><head><title>Apriori Support Chart</title><style>"
        << "body{font-family:Arial; padding:20px;}"
        << ".bar-row{display:flex; align-items:center; margin:8px 0;}"
        << ".label{width:200px; font-weight:bold;}"
        << ".bar{background:#4CAF50; height:24px; color:white;"
        << "padding-left:8px; line-height:24px; border-radius:4px;}"
        << "</style></head><body>"
        << "<h2>Frequent Itemsets Support Visualization</h2>";

    for (
        size_t k = 0;
        k < allFrequentItemsets.size();
        ++k
        ) {

        for (const auto& pair : allFrequentItemsets[k]) {

            string name = itemsetToString(pair.first);

            double supPercent = pair.second * 100.0;

            int barLength = (int)(supPercent / 2.0);

            // Console visualization
            cout << left << setw(22) << name << " | ";

            for (int i = 0; i < barLength; ++i) {
                cout << "#";
            }

            cout << " ("
                << fixed << setprecision(1)
                << supPercent << "%)\n";

            // HTML visualization
            html << "<div class='bar-row'>"
                << "<div class='label'>" << name << "</div>"
                << "<div class='bar' style='width:"
                << (supPercent * 5) << "px;'>"
                << fixed << setprecision(1)
                << supPercent << "%</div></div>";
        }
    }

    html << "</body></html>";

    html.close();

    cout << "\n[Chart saved successfully: support_chart.html]\n";
}
