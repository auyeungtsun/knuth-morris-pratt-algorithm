#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

/**
 * @brief Computes the Longest Proper Prefix Suffix (LPS) array for a given pattern.
 *
 * The LPS array is used in the KMP string searching algorithm.
 * For a pattern `pattern`, lps[i] stores the length of the longest proper prefix
 * of pattern[0..i] which is also a suffix of pattern[0..i].
 * A proper prefix or suffix of a string is a prefix or suffix that is not equal to the string itself.
 *
 * @param pattern The pattern string for which to compute the LPS array.
 * @return A vector of integers representing the LPS array for the given pattern.
 *
 * @note Time Complexity: O(m), where m is the length of the pattern.
 * @note Space Complexity: O(m) for storing the LPS array.
 */
vector<int> computeLPS(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int i = 1;
    int j = 0;
    while (i < m) {
        if (pattern[i] == pattern[j]) {
            j++;
            lps[i] = j;
            i++;
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

/**
 * @brief Implements the Knuth-Morris-Pratt (KMP) string searching algorithm.
 *
 * The KMP algorithm is an efficient string searching algorithm that searches for occurrences of a
 * "pattern" within a main "text" string by utilizing the LPS (Longest Proper Prefix Suffix) array.
 *
 * @param text The main text string to search within.
 * @param pattern The pattern string to search for.
 * @return A vector of integers representing the LPS array for text string according to pattern.
 *         lps[i] means at i'th pos in text, length of the longest prefix of pattern that matches a suffix of text ending at i.
 *
 * @note Time Complexity: O(n + m), where n is the length of the text and m is the length of the pattern.
 * @note Space Complexity: O(m + n), where m is the length of the pattern and n is the length of the text.
 */
vector<int> KMPSearch(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();
    if (m == 0) {
        return {};
    }
    vector<int> lps_pattern = computeLPS(pattern);
    vector<int> lps(n);
    int i = 0; // index for text
    int j = 0; // index for pattern
    while (i < n) {
        if (pattern[j] == text[i]) {
            j++;
            lps[i] = j;
            i++;
        }
        if (j == m) {
            j = lps_pattern[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps_pattern[j - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

void testComputeLPS() {
    cout << "Testing computeLPS..." << endl;

    // Test case 1: Empty string
    vector<int> expected1 = {};
    vector<int> result1 = computeLPS("");
    assert(result1 == expected1);
    cout << "  Test Case 1 (Empty String): Passed" << endl;

    // Test case 2: Single character
    vector<int> expected2 = {0};
    vector<int> result2 = computeLPS("A");
    assert(result2 == expected2);
    cout << "  Test Case 2 (Single Char): Passed" << endl;

    // Test case 3: No repeating prefix/suffix
    vector<int> expected3 = {0, 0, 0, 0, 0};
    vector<int> result3 = computeLPS("ABCDE");
    assert(result3 == expected3);
    cout << "  Test Case 3 (No Repeats): Passed" << endl;

    // Test case 4: All same characters
    vector<int> expected4 = {0, 1, 2, 3, 4};
    vector<int> result4 = computeLPS("AAAAA");
    assert(result4 == expected4);
    cout << "  Test Case 4 (All Same): Passed" << endl;

    // Test case 5: Simple repeating pattern
    vector<int> expected5 = {0, 0, 1, 2, 3, 4};
    vector<int> result5 = computeLPS("ABABAB");
    assert(result5 == expected5);
    cout << "  Test Case 5 (Simple Repeat): Passed" << endl;

    // Test case 6: More complex pattern
    vector<int> expected6 = {0, 1, 0, 1, 2, 0, 1, 2, 3, 4, 5};
    vector<int> result6 = computeLPS("AABAACAABAA");
    assert(result6 == expected6);
    cout << "  Test Case 6 (Complex 1): Passed" << endl;

    // Test case 7: Another complex pattern from CLRS
    vector<int> expected7 = {0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 2};
    vector<int> result7 = computeLPS("aabaacaadaa");
    assert(result7 == expected7);
    cout << "  Test Case 7 (Complex 2 - aabaacaadaa): Passed" << endl;

    cout << "computeLPS tests finished." << endl << endl;
}

void testKMPSearch() {
    cout << "Testing KMPSearch (Text LPS State Array)..." << endl;

    // Test case 1: Empty text
    vector<int> expected1 = {};
    vector<int> result1 = KMPSearch("", "ABC");
    assert(result1 == expected1);
    cout << "  Test Case 1 (Empty Text): Passed" << endl;

    // Test case 2: Empty pattern
    vector<int> expected2 = {};
    vector<int> result2 = KMPSearch("ABCABC", "");
    assert(result2 == expected2);
    cout << "  Test Case 2 (Empty Pattern): Passed" << endl;

    // Test case 3: Empty text and pattern
    vector<int> expected3 = {};
    vector<int> result3 = KMPSearch("", "");
    assert(result3 == expected3);
    cout << "  Test Case 3 (Empty Both): Passed" << endl;

    // Test case 4: Pattern not found
    string text4 = "ABCDEFG";
    string pattern4 = "XYZ";
    vector<int> expected4 = {0, 0, 0, 0, 0, 0, 0};
    vector<int> result4 = KMPSearch(text4, pattern4);
    assert(result4 == expected4);
    cout << "  Test Case 4 (Pattern Not Found): Passed" << endl;

    // Test case 5: Simple match at beginning
    string text5 = "ABCDEF";
    string pattern5 = "ABC";
    vector<int> expected5 = {1, 2, 3, 0, 0, 0};
    vector<int> result5 = KMPSearch(text5, pattern5);
    assert(result5 == expected5);
    cout << "  Test Case 5 (Match at Start): Passed" << endl;

    // Test case 6: Simple match at end
    string text6 = "XYZABC";
    string pattern6 = "ABC";
    vector<int> expected6 = {0, 0, 0, 1, 2, 3};
    vector<int> result6 = KMPSearch(text6, pattern6);
    assert(result6 == expected6);
    cout << "  Test Case 6 (Match at End): Passed" << endl;

    // Test case 7: Multiple non-overlapping matches
    string text7 = "ABCXYZABC";
    string pattern7 = "ABC";
    vector<int> expected7 = {1, 2, 3, 0, 0, 0, 1, 2, 3};
    vector<int> result7 = KMPSearch(text7, pattern7);
    assert(result7 == expected7);
    cout << "  Test Case 7 (Multiple Non-overlapping): Passed" << endl;

    // Test case 8: Overlapping matches
    string text8 = "ababab";
    string pattern8 = "abab";
    vector<int> expected8 = {1, 2, 3, 4, 3, 4};
    vector<int> result8 = KMPSearch(text8, pattern8);
    assert(result8 == expected8);
    cout << "  Test Case 8 (Overlapping Matches): Passed" << endl;

    // Test case 9: Complex case with resets
    string text9 = "ABABDABACDABABCABAB";
    string pattern9 = "ABABCABAB";
    vector<int> expected9 = {1, 2, 3, 4, 0, 1, 2, 3, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> result9 = KMPSearch(text9, pattern9);
    assert(result9 == expected9);
    cout << "  Test Case 9 (Complex Overlapping): Passed" << endl;

    // Test case 10: Text is shorter than pattern
    string text10 = "ABC";
    string pattern10 = "ABCDE";
    vector<int> expected10 = {1, 2, 3};
    vector<int> result10 = KMPSearch(text10, pattern10);
    assert(result10 == expected10);
    cout << "  Test Case 10 (Text Shorter than Pattern): Passed" << endl;

    cout << "KMPSearch tests finished." << endl << endl;
}

void runComputeLPSSample() {
    string pattern = "AABAACAABAA";
    vector<int> lps = computeLPS(pattern);
    cout << "Pattern: " << pattern << endl;
    cout << "LPS Array: ";
    for (int val : lps) {
        cout << val << " ";
    }
    cout << endl;
}

void runKMPSearchSample() {
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    vector<int> lps = KMPSearch(text, pattern);
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    cout << "LPS Array: ";
    for (int val : lps) {
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    testComputeLPS();
    testKMPSearch();
    runComputeLPSSample();
    runKMPSearchSample();
    return 0;
}