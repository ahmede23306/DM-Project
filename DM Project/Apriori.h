
#pragma once

#include "Transaction.h"

int getSupportCount(
    const Itemset& candidate,
    const Transactions& transactions
);

FrequentLevels generateFrequentItemsets(
    const Transactions& transactions,
    double minSupport
);
