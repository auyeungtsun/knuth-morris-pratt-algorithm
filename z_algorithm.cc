#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;

/**
 * @brief Computes the Z-array for a given string.
 * 
 * The Z-array is an array of the same length as the string, where each element
 * Z[i] represents the length of the longest substring starting from s[i] which
 * is also a prefix of s.
 * 
 * @param s The input string.
 * @return A vector of integers representing the Z-array.
 * @note Time Complexity: O(n), where n is the length of the string.
 * @note Space Complexity: O(n), where n is the length of the string.
 */
vector<int> computeZArray(const string& s) {
    int n = s.length();
    if (n == 0) {
        return {};
    }
    vector<int> Z(n, 0);
    int L = 0, R = 0; // [L, R] make a window which matches prefix of s

    Z[0] = n;

    for (int i = 1; i < n; ++i) {
        if (i > R) {
            L = R = i;
            while (R < n && s[R - L] == s[R]) {
                R++;
            }
            Z[i] = R - L;
            R--;
        }
        else {
            int k = i - L;

            if (Z[k] < R - i + 1) {
                Z[i] = Z[k];
            }
            else {
                L = i;
                while (R < n && s[R - L] == s[R]) {
                    R++;
                }
                Z[i] = R - L;
                R--;
            }
        }
    }
    return Z;
}

/**
 * @brief Implements the Z-algorithm to search for a pattern within a text.
 * 
 * This function computes an array Z, where Z[i] is the length of the longest
 * substring starting from text[i] that matches a prefix of the pattern.
 * 
 * @param text The text to search within.
 * @param pattern The pattern to search for.
 * @return A vector of integers representing the Z-array for the text relative to the pattern.
 *         Z[i] is the length of the longest substring starting at text[i] that is also a prefix of the pattern.
 *         - If Z[i] == pattern.length(), then the pattern is found at index i in text.
 * @note Time complexity: O(n + m) where n is the length of pattern and m is the length of text
 * @note Space complexity: O(n) where n is the length of the pattern
 */
vector<int> zAlgorithmSearch(const string& text, const string& pattern) {
    int n = pattern.length();
    int m = text.length();
    vector<int> Z(m, 0);
    if (n == 0) {
        return Z;
    }

    vector<int> Z_pattern = computeZArray(pattern);

    int L = 0, R = -1; // [L, R] defines the Z-box within the *text* matching a prefix of *pattern*
    
    for (int i = 0; i < m; ++i) {
        Z[i] = 0;

        if (i > R) {
            L = R = i;
            while (R < m && (R - L) < n && text[R] == pattern[R - L]) {
                R++;
            }
            Z[i] = R-L;
            R--;
        }
        else {
            int k = i - L;

            if (Z_pattern[k] < R - i + 1) {
                Z[i] = Z_pattern[k];
            }
            else {
                L = i;
                 while (R < m && (R - L) < n && text[R] == pattern[R - L]) {
                    R++;
                }
                Z[i] = R - L;
                R--;
            }
        }
        
    }

    return Z;
}

void testComputeZArray() {
    cout << "--- Testing computeZArray ---" << endl;
    vector<int> result;
    vector<int> expected;

    // Test case 1: Empty string
    result = computeZArray("");
    expected = {};
    assert(result == expected);
    cout << "Test Case 1 (Empty String): Passed" << endl;

    // Test case 2: Single character
    result = computeZArray("a");
    expected = {1};
    assert(result == expected);
    cout << "Test Case 2 (Single Char): Passed" << endl;

    // Test case 3: All same characters
    result = computeZArray("aaaaa");
    expected = {5, 4, 3, 2, 1};
    assert(result == expected);
    cout << "Test Case 3 (All Same): Passed" << endl;

    // Test case 4: Distinct characters
    result = computeZArray("abcde");
    expected = {5, 0, 0, 0, 0};
    assert(result == expected);
    cout << "Test Case 4 (Distinct): Passed" << endl;

    // Test case 5: Standard example 1
    result = computeZArray("aabaabcaxaabaabcy");
    expected = {17, 1, 0, 3, 1, 0, 0, 1, 0, 7, 1, 0, 3, 1, 0, 0, 0};
    assert(result == expected);
    cout << "Test Case 5 (Standard 1): Passed" << endl;

    // Test case 6: Standard example 2
    result = computeZArray("ababababa");
    expected = {9, 0, 7, 0, 5, 0, 3, 0, 1};
    assert(result == expected);
    cout << "Test Case 6 (Standard 2): Passed" << endl;

     // Test case 7: Standard example 3
    result = computeZArray("aaabaab");
    expected = {7, 2, 1, 0, 2, 1, 0};
    assert(result == expected);
    cout << "Test Case 7 (Standard 3): Passed" << endl;

    cout << "--- computeZArray tests completed successfully! ---" << endl << endl;
}


void testZAlgorithmSearch() {
    cout << "--- Testing zAlgorithmSearch ---" << endl;
    vector<int> resultZ;
    vector<int> expectedZ;
    vector<int> resultOccurrences;
    vector<int> expectedOccurrences;
    string text, pattern;

    // Test Case 1: Pattern Found Multiple Times
    text = "GEEKS FOR GEEKS";
    pattern = "GEEK";
    resultZ = zAlgorithmSearch(text, pattern);
    expectedZ = {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0};
    assert(resultZ.size() == text.length());
    assert(resultZ == expectedZ);
    cout << "Test Case 1 (Multiple Found): Passed" << endl;

    // Test Case 2: Pattern Not Found
    text = "ABCDEF";
    pattern = "XYZ";
    resultZ = zAlgorithmSearch(text, pattern);
    expectedZ = {0, 0, 0, 0, 0, 0};
    assert(resultZ.size() == text.length());
    assert(resultZ == expectedZ);
    cout << "Test Case 2 (Not Found): Passed" << endl;

    // Test Case 3: Overlapping Occurrences
    text = "aaaaa";
    pattern = "aa";
    resultZ = zAlgorithmSearch(text, pattern);
    expectedZ = {2, 2, 2, 2, 1};
    assert(resultZ.size() == text.length());
    assert(resultZ == expectedZ);
    cout << "Test Case 3 (Overlapping): Passed" << endl;

     // Test Case 4: Pattern at End
    text = "xyzabc";
    pattern = "abc";
    resultZ = zAlgorithmSearch(text, pattern);
    expectedZ = {0, 0, 0, 3, 0, 0};
    assert(resultZ.size() == text.length());
    assert(resultZ == expectedZ);
    cout << "Test Case 4 (Found at End): Passed" << endl;

    // Test Case 5: Empty Text
    text = "";
    pattern = "abc";
    resultZ = zAlgorithmSearch(text, pattern);
    expectedZ = {};
    assert(resultZ.size() == text.length());
    assert(resultZ == expectedZ);
    cout << "Test Case 5 (Empty Text): Passed" << endl;

    // Test Case 6: Empty Pattern
    text = "abc";
    pattern = "";
    resultZ = zAlgorithmSearch(text, pattern);
    expectedZ = {0, 0, 0};
    assert(resultZ.size() == text.length());
    assert(resultZ == expectedZ);
    cout << "Test Case 6 (Empty Pattern): Passed" << endl;

    // Test Case 7: Pattern Longer than Text
    text = "abc";
    pattern = "abcd";
    resultZ = zAlgorithmSearch(text, pattern);
    assert(resultZ.size() == text.length());
    expectedZ = {3, 0, 0};
    assert(resultZ == expectedZ);
    cout << "Test Case 7 (Pattern Longer): Passed" << endl;

     // Test Case 8: Standard Example
    text = "ABABDABACDABABCABAB";
    pattern = "ABABCABAB";
    resultZ = zAlgorithmSearch(text, pattern);
    expectedZ = {4, 0, 2, 0, 0, 3, 0, 1, 0, 0, 9, 0, 2, 0, 0, 4, 0, 2, 0};
    assert(resultZ.size() == text.length());
    assert(resultZ == expectedZ);
    cout << "Test Case 8 (Standard Complex): Passed" << endl;


    cout << "--- zAlgorithmSearch tests completed successfully! ---" << endl << endl;
}

void computeZArraySample() {
    cout << "--- computeZArray Sample ---" << endl;
    string s = "aabaabcaxaabaabcy";
    vector<int> result = computeZArray(s);
    cout << "String: " << s << endl;
    cout << "Z-array: ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << endl;
    cout << "--- computeZArray Sample Completed ---" << endl << endl;
}

void zAlgorithmSearchSample() {
    cout << "--- zAlgorithmSearch Sample ---" << endl;
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    vector<int> result = zAlgorithmSearch(text, pattern);
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    cout << "Z-array: ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << endl;
     cout << "--- zAlgorithmSearch Sample Completed ---" << endl << endl;
}

int main() {
    testComputeZArray();
    testZAlgorithmSearch();
    computeZArraySample();
    zAlgorithmSearchSample();
    return 0;
}