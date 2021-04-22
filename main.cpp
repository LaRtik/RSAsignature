#include <iostream>  
#include <cstdlib>
#include <ctime>
#include <time.h>

#include "RSA.h"

int main()
{
    clock_t start, end;
    start = clock(); // to calculate program execution time
    srand(static_cast<unsigned>(time(0))); // better random

    Keys keys = calculateRSAKeys(); // calculate RSA private and public keys
    std::cout << "Calculated RSA keys!" << "\n";

    /*
    ----------------------
     [1] crypting message with RSA
    ---------------------
    */  

    std::cout << "Crypting message using RSA.\n\n";

    std::string message = "Hello text";
    std::cout << "Message: " << message << "\n";

    // also let's confuse message
    std::vector <largeIntegerType> confusedMessage;
    confusedMessage = confuseData(message, keys._public);
    std::cout << "Confused message: ";
    for (auto i : confusedMessage) std::cout << i << " ";
    std::cout << "\n";

    // crypt confused message
    std::vector<largeIntegerType> cryptedConfusedMessage = cryptMessage(confusedMessage, keys._public);
    std::cout << "Crypted confused message: ";
    for (auto i : cryptedConfusedMessage) std::cout << i << " ";
    std::cout << "\n";
    std::string encryptedConfusedMessage = encryptMessage(cryptedConfusedMessage, keys._private);
    std::cout << "Encrypted confused message: " << encryptedConfusedMessage << "\n";
    // don't forget to deconfuse message
    std::cout << "Encrypted original message: " << deconfuseData(encryptedConfusedMessage, keys._public) << "\n";


    // try to encrypt message with invalid secrey key (let use public key instead private key)

    std::cout << "Encrypted original message with invalid private key (let use public key): " << deconfuseData(encryptMessage(cryptedConfusedMessage, keys._public), keys._public) << "\n\n\n"; // encrypring message with bad key



    /* 
    ----------------------
     [2] signing message w/o crypting original message
    ----------------------
    */


    std::cout << "Creating sign without crypting original message\n\n";

    // let's use the same confused message ("Hello text") and the same RSA keys
    std::cout << "Confused message: ";
    for (auto i : confusedMessage) std::cout << i << " ";
    std::cout << "\n";

    // create sign using private key
    std::vector <largeIntegerType> sign = cryptMessage(confusedMessage, keys._private); // creating sign with private key
    std::cout << "Sign: ";
    for (auto i : sign) std::cout << i << " ";
    std::cout << "\n"; 

    // encrypt message with public key (only sender knows private key, everyone else know public key)
    std::string confusedMessagePrototype = encryptMessage(sign, keys._public);
    std::cout << "Confused message prototype (encrypted sign): " << confusedMessagePrototype << "\n";
    std::string originalMessagePrototype = deconfuseData(confusedMessagePrototype, keys._public);
    std::cout << "Original message prototype: " << originalMessagePrototype << "\n";
    if (originalMessagePrototype == message)
    {
        std::cout << "Message and message prototype are equal. Signed succesfully!\n\n"; 
    }
    else
    {
        std::cout << "Sign is bad. Try again =(\n\n";
    }

    /*
    ------------------------
    [2.1] signing crypted message
    ------------------------
    */


    // let's create keys for sender and receiever
    Keys senderKeys = calculateRSAKeys();
    Keys receiverKeys = calculateRSAKeys();

    std::cout << "Creating sign with crypting original message\n\n";

    // let's use new message
    message = "Hello world!";
    std::cout << "Message: " << message << "\n";

    // confuse message
    confusedMessage = confuseData(message, senderKeys._public);
    std::cout << "Confused message: ";
    for (auto i : confusedMessage) std::cout << i << " ";
    std::cout << "\n";

    // create new sign using sender private key
    std::vector <largeIntegerType> senderSign = cryptMessage(confusedMessage, senderKeys._private);
    std::cout << "Sender sign: ";
    for (auto i : senderSign) std::cout << i << " ";
    std::cout << "\n";

    // crypt senderSign and message using receiver public key

    std::vector <largeIntegerType> cryptedSenderSign = cryptMessage(senderSign, receiverKeys._public);
    cryptedConfusedMessage = cryptMessage(confusedMessage, receiverKeys._public);
    std::cout << "Crypted sender sign: ";
    for (auto i:cryptedSenderSign) std::cout << i << " ";
    std::cout << "\n"; 

    std::cout << "Crypted confused message: ";
    for (auto i:cryptedConfusedMessage) std::cout << i << " ";
    std::cout << "\n";
    // encrypt senderSign and message using receiver private key
    std::cout << "Encrypted sender sign: " << encryptMessage(cryptedSenderSign, receiverKeys._private) << "\n";
    std::cout << "Encrypted confused message: " << encryptMessage(cryptedConfusedMessage, receiverKeys._private) << "\n";
    std::cout << "Encrypted original message: " << deconfuseData(encryptMessage(cryptedConfusedMessage, receiverKeys._private), senderKeys._public) << "\n";


    end = clock();
    std::cout << "\n\n\nExecution time: " << double(end - start) / CLOCKS_PER_SEC << " secs.";

	return 0;
}