#include <iostream>
#include <string>
using namespace std;

// Function to perform XOR operation
string XOR(string dividend, string divisor) {
    string result = dividend;
    for (int i = 0; i < divisor.size(); i++) {
        result[i] = (dividend[i] == divisor[i]) ? '0' : '1';
    }
    return result;
}

// Function to perform CRC Division (binary division)
string mod2Division(string dividend, string divisor) {
    int pick = divisor.length();
    string tmp = dividend.substr(0, pick);

    while (pick < dividend.length()) {
        if (tmp[0] == '1') {
            tmp = XOR(divisor, tmp) + dividend[pick];
        } else {
            tmp = XOR(string(pick, '0'), tmp) + dividend[pick];
        }
        pick++;
    }

    if (tmp[0] == '1') {
        tmp = XOR(divisor, tmp);
    } else {
        tmp = XOR(string(pick, '0'), tmp);
    }

    return tmp;
}

// Function to encode data by appending CRC
string encodeData(string data, string key) {
    int keyLen = key.length();
    string appendedData = data + string(keyLen - 1, '0');
    string remainder = mod2Division(appendedData, key);
    return data + remainder;
}

// Function to check if there are errors in the received data
bool detectError(string data, string key) {
    string remainder = mod2Division(data, key);
    for (char c : remainder) {
        if (c != '0') return true; // Error detected
    }
    return false; // No error
}

// Function to convert ASCII to binary
string asciiToBinary(char ch) {
    string binary = "";
    for (int i = 7; i >= 0; i--) {
        binary += ((ch >> i) & 1) ? '1' : '0';
    }
    return binary;
}

int main() {
    string key = "1101"; // Example CRC generator polynomial

    // Input: ASCII character
    char ch;
    cout << "Enter an ASCII character: ";
    cin >> ch;

    // Convert ASCII character to binary
    string binaryData = asciiToBinary(ch);
    cout << "Binary representation of '" << ch << "' is: " << binaryData << endl;

    // Encode the data by appending CRC
    string encodedData = encodeData(binaryData, key);
    cout << "Encoded data (with CRC): " << encodedData << endl;

    // Simulate transmission
    cout << "Simulating data transmission...\n";

    // Check for errors at the receiver end
    bool error = detectError(encodedData, key);
    if (error) {
        cout << "Error detected in the received data!" << endl;
    } else {
        cout << "No error detected in the received data." << endl;
    }

    return 0;
}
