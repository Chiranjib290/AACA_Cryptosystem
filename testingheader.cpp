#include "header.h"

int main(){
    std::string plaintxt = "plaintext.txt";
    encryption_file(plaintxt);
    std::cout<<"Encryption Done\n";

    std::string encname = "ciphertext.txt";
    std::string keys = "key.txt";
    decryption_file(encname,keys);
    std::cout<<"Decryption Done\n";
    
    return 0;
}