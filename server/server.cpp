#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bitset>
using namespace std;

// Caesar Cipher encryption and decryption functions
std::string caesarCipherDecrypt(const std::string &text, int shift) {
    std::string result = text;
    const int asciiRange = 256; // Extended ASCII range

    for (size_t i = 0; i < text.size(); i++) {
        unsigned char encryptedChar = text[i];
        unsigned char decryptedChar = (encryptedChar - shift + asciiRange) % asciiRange;
        result[i] = decryptedChar;
    }

    return result;
}

// Function to decode AMI string back to binary
std::string amiDecode(const std::string &amiString) {
    std::string binaryString;
    for (char ch : amiString) {
        if (ch == '0') {
            binaryString += '0';
        } else {
            binaryString += '1';
        }
    }
    return binaryString;
}

// Function to convert a binary string back to its character representation
std::string binaryToString(const std::string &binaryString) {
    std::string text;
    for (size_t i = 0; i < binaryString.size(); i += 8) {
        std::bitset<8> charBits(binaryString.substr(i, 8));
        text += static_cast<char>(charBits.to_ulong());
    }
    return text;
}


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const int shift = 3; // Shift value for Caesar Cipher

    ofstream amiDecodedFile("server_ami_encoded.txt");

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    read(new_socket, buffer, 1024);
    std::string amiMessage(buffer);

	int xPos = 0;
	for (char value : amiMessage) {
        if (value == '0') {
            amiDecodedFile << xPos++ << ' ' << '0' << endl;
        } else if(value == '+') {
            amiDecodedFile << xPos++ << ' ' << "-1" << endl;
        } else {
            amiDecodedFile << xPos++ << ' ' << '1' << endl;
        }
    }

    std::string binaryMessage = amiDecode(amiMessage);
	std::cout << "Binary message received: " << binaryMessage << std::endl;
    
	std::string encryptedMessage = binaryToString(binaryMessage);
	std::cout << "Encrypted message received: " << encryptedMessage << std::endl;
    
	std::string decryptedMessage = caesarCipherDecrypt(encryptedMessage, shift);
    std::cout << "Decrypted message received: " << decryptedMessage << std::endl;

    close(new_socket);
    close(server_fd);
	amiDecodedFile.close();

    return 0;
}
