
#include "Apriori.h"
#include <algorithm>

using namespace std;

int getSupportCount( const Itemset& candidate, const Transactions& transactions) {
    int count = 0;
    for (const auto& t : transactions) {
        if (includes(t.begin(),t.end(), candidate.begin(), candidate.end())) {
            count++;
        }

    }

    return count;
}

FrequentLevels generateFrequentItemsets(const Transactions& transactions, double minSupport ) {

    FrequentLevels allFrequentItemsets;
    int totalTrans = transactions.size();
    if (totalTrans == 0) {
        return allFrequentItemsets;
    }

    map<Itemset, int> c1Counts;

    for (const auto& t : transactions) {

        for (const auto& item : t) { c1Counts[{item}]++; }
    }

    map<Itemset, double> l1;

    for (const auto& pair : c1Counts) {
        double sup = (double)pair.second / totalTrans;
        if (sup >= minSupport - 1e-9) {
            l1[pair.first] = sup;
        }
    }

    if (l1.empty()){
        return allFrequentItemsets;
    }

    allFrequentItemsets.push_back(l1);

    int k = 2;
    while (true) {
        const auto& prevL = allFrequentItemsets.back();
        vector<Itemset> prevItemsets;

        for (const auto& pair : prevL) {
            prevItemsets.push_back(pair.first);
        }

        set<Itemset> candidates;
        for (size_t i = 0; i < prevItemsets.size(); ++i) {
            for (size_t j = i + 1; j < prevItemsets.size(); ++j) {
                Itemset joined = prevItemsets[i];

                joined.insert(prevItemsets[j].begin(),prevItemsets[j].end());

                if (joined.size() == (size_t)k) {

                    bool allSubsetsFrequent = true;
                    for (const auto& item : joined) {
                        Itemset subset = joined;
                        subset.erase(item);
                        if (prevL.find(subset) == prevL.end()) {
                            allSubsetsFrequent = false;
                            break;
                        }

                    }

                    if (allSubsetsFrequent) { candidates.insert(joined);}
                }
            }
        }

        map<Itemset, double> currentL;

        for (const auto& cand : candidates) {

            int count = getSupportCount(cand, transactions);

            double sup =(double)count / totalTrans;
            if (sup >= minSupport - 1e-9) {
                currentL[cand] = sup;
            }
        }

        if (currentL.empty()) {
            break;
        }
        allFrequentItemsets.push_back(currentL);
        k++;
    }
    return allFrequentItemsets;
}
