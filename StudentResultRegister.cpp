#include "DynamicArray.h"
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>

// ---------------------------------------------------------------------
// 3. SORTING & SEARCHING (A2)
// ---------------------------------------------------------------------
template <typename T, typename Compare>
void insertionSort(T* a, int n, Compare less) {
    for (int i = 1; i < n; ++i) {
        T key = a[i];
        int j = i - 1;
        while (j >= 0 && less(key, a[j])) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

template <typename T, typename Compare>
void mergeRuns(T* a, T* tmp, int lo, int mid, int hi, Compare less) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (less(a[j], a[i])) tmp[k++] = a[j++];
        else                  tmp[k++] = a[i++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi)  tmp[k++] = a[j++];
    for (int t = lo; t <= hi; ++t) a[t] = tmp[t];
}

template <typename T, typename Compare>
void mergeSortRec(T* a, T* tmp, int lo, int hi, Compare less) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortRec(a, tmp, lo, mid, less);
    mergeSortRec(a, tmp, mid + 1, hi, less);
    mergeRuns(a, tmp, lo, mid, hi, less);
}

template <typename T, typename Compare>
void mergeSort(T* a, int n, Compare less) {
    if (n < 2) return;
    T* tmp = new T[n];
    mergeSortRec(a, tmp, 0, n - 1, less);
    delete[] tmp;
}

template <typename T, typename Key, typename Compare>
int binarySearch(const T* a, int n, const Key& key, Compare less) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (less(a[mid], key))      lo = mid + 1;
        else if (less(key, a[mid])) hi = mid - 1;
        else                        return mid;
    }
    return -1;
}

// ---------------------------------------------------------------------
// 4. COMPARATORS
// ---------------------------------------------------------------------
struct ByScore { bool operator()(const StudentRecord& x, const StudentRecord& y) const { return x.score < y.score; } };
struct ScoreCompare {
    bool operator()(const StudentRecord& a, double k) const { return a.score < k; }
    bool operator()(double k, const StudentRecord& a) const { return k < a.score; }
};

// ---------------------------------------------------------------------
// 5. MAIN
// ---------------------------------------------------------------------
int main() {
    const int REG_LAST_DIGIT = 0; 
    const bool USE_MERGE = (REG_LAST_DIGIT % 2 == 0); 

    try {
        DynamicArray<StudentRecord> reg;
        reg.loadFromFile("students.txt");
        std::cout << "Loaded " << reg.size()
                  << " records (capacity " << reg.getCapacity() << ")\n";

        insertionSort(reg.raw(), reg.size(), ByScore{});
        std::cout << "After insertionSort (by score):\n";
        for (int i = 0; i < reg.size(); ++i)
            std::cout << "  " << reg.at(i).id << "  " << reg.at(i).name
                      << "  " << reg.at(i).score << "\n";

        double probe = 75.0;
        int idx = binarySearch(reg.raw(), reg.size(), probe, ScoreCompare{});
        std::cout << "binarySearch(score=" << probe << ") -> "
                  << (idx >= 0 ? "found at index " + std::to_string(idx) : "absent") << "\n";

        probe = 999.0;
        idx = binarySearch(reg.raw(), reg.size(), probe, ScoreCompare{});
        std::cout << "binarySearch(score=" << probe << ") -> "
                  << (idx >= 0 ? "found" : "absent (expected)") << "\n";

        std::cout << "Chosen O(n log n) sort: mergeSort\n";
        mergeSort(reg.raw(), reg.size(), ByScore{});

        reg.saveToFile("students_out.txt");
        std::cout << "Saved updated records to students_out.txt\n";

        std::cout << "\n=== A4 TIMING: insertionSort vs mergeSort ===\n";
        std::mt19937 rng(42);
        std::uniform_real_distribution<double> dist(0.0, 100.0);
        std::cout << std::setw(8) << "n"
                  << std::setw(14) << "insertion(ms)"
                  << std::setw(14) << "mergeSort(ms)"
                  << std::setw(10) << "ratio\n";
        double prevIns = 0, prevFast = 0;
        for (int n = 1000; n <= 64000; n *= 2) {
            double insTotal = 0, fastTotal = 0;
            for (int run = 0; run < 3; ++run) {
                DynamicArray<StudentRecord> a, b;
                for (int i = 0; i < n; ++i) {
                    StudentRecord r{i, "S", "C", dist(rng)};
                    a.push_back(r);
                    b.push_back(r);
                }
                auto t0 = std::chrono::high_resolution_clock::now();
                insertionSort(a.raw(), a.size(), ByScore{});
                auto t1 = std::chrono::high_resolution_clock::now();
                
                mergeSort(b.raw(), b.size(), ByScore{});
                
                auto t2 = std::chrono::high_resolution_clock::now();
                insTotal  += std::chrono::duration<double, std::milli>(t1 - t0).count();
                fastTotal += std::chrono::duration<double, std::milli>(t2 - t1).count();
            }
            double insAvg  = insTotal  / 3.0;
            double fastAvg = fastTotal / 3.0;
            double fastRatio = prevFast > 0 ? fastAvg / prevFast : 0;
            std::cout << std::setw(8) << n
                      << std::setw(14) << std::fixed << std::setprecision(3) << insAvg
                      << std::setw(14) << fastAvg
                      << std::setw(10) << fastRatio << "\n";
            prevIns = insAvg; prevFast = fastAvg;
        }

    } catch (const std::exception& ex) {
        std::cerr << "FATAL: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}