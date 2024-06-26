#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bitset>
using namespace std;


// Caesar Cipher encryption and decryption functions
std::string caesarCipherEncrypt(const std::string &text, int shift) {
    std::string result = text;
    const int asciiRange = 256; // Extended ASCII range

    for (size_t i = 0; i < text.size(); i++) {
        unsigned char originalChar = text[i];
        unsigned char encryptedChar = (originalChar + shift) % asciiRange;
        result[i] = encryptedChar;
    }

    return result;
}


// Function to convert a string to its binary representation
std::string stringToBinary(const std::string &text) {
    std::string binaryString;
    for (char ch : text) {
        binaryString += std::bitset<8>(ch).to_string();
    }
    return binaryString;
}

// Function to apply AMI encoding to a binary string
std::string amiEncode(const std::string &binaryString) {
    std::string amiString;
    bool lastMarkPositive = true;  // To alternate the mark signal

    for (char bit : binaryString) {
        if (bit == '0') {
            amiString += '0';
        } else {
            amiString += (lastMarkPositive ? '-' : '+');
            lastMarkPositive = !lastMarkPositive;
        }
    }
    return amiString;
}


int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const int shift = 3; // Shift value for Caesar Cipher
    char buffer[1024] = {0};

    ofstream amiEncodedFile("client_ami_encoded.txt");

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "\n Socket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("10.181.3.70");

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "10.181.3.70", &serv_addr.sin_addr) <= 0) {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "\nConnection Failed \n";
        
        return -1;
    }

    std::string message;
    std::cout << "Enter the message to send: ";
    std::getline(std::cin, message); 

    std::string encryptedMessage = caesarCipherEncrypt(message, shift);
    std::cout << "Encrypted message sent: " << encryptedMessage << std::endl;

    std::string binaryMessage = stringToBinary(encryptedMessage);
    std::cout << "Binary message sent: " << binaryMessage << std::endl;
    
    std::string amiMessage = amiEncode(binaryMessage);
    std::cout << "Encoded message sent: " << amiMessage << std::endl;

    int xPos = 0;
	for (char value : amiMessage) {
        if (value == '0') {
            amiEncodedFile << xPos++ << ' ' << '0' << endl;
        } else if(value == '+') {
            amiEncodedFile << xPos++ << ' ' << "-1" << endl;
        } else {
            amiEncodedFile << xPos++ << ' ' << '1' << endl;
        }
    }
    
    send(sock, amiMessage.c_str(), amiMessage.length(), 0);
    std::cout << "AMI encoded message sent\n";

    close(sock);
    amiEncodedFile.close();

    return 0;
}
