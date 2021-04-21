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

    /*
    ----------------------
     [1] crypting message with RSA
    ---------------------
    */  

    std::cout << "Crypting message using RSA.\n\n";

    std::string message = "Hello text";
    std::cout << "Message: " << message << "\n";

    // also let's confuse message
    message = to_string(confuseData(message, keys._public));
    std::cout << "Confused message: " << message << "\n";
    std::vector<largeIntegerType> cryptedMessage = cryptMessage(message, keys._public); // crypting message using public key
    std::cout << "Crypted message: ";
    for (auto i:cryptedMessage) std::cout << i << " ";
    std::cout << "\n"; 
    std::cout << "Encrypted spoiled message: " << encryptMessage(cryptedMessage, keys._private) << "\n";

    // don't forget to deconfuse message
    std::cout << "Encrypted original message: " << deconfuseData(cryptedMessage, keys._private) << "\n";
    // try to encrypt message with invalid secrey key (let use public key instead private key)

    std::cout << "Encrypted message with invalid private key (let use public key): " << encryptMessage(cryptedMessage, keys._public) << "\n\n"; // encrypring message with bad key



    /* 
    ----------------------
     [2] signing message w/o crypting original message
    ----------------------
    */


    std::cout << "Creating sign without crypting original message\n\n";

    // let's use the same confused message ("Hello text") and the same RSA keys
    std::cout << "Confused message: " << message << "\n";

    
    // create sign using private key
    std::vector <largeIntegerType> sign = cryptMessage(message, keys._private); // creating sign with private key
    std::cout << "Sign: ";
    std::for_each(sign.begin(), sign.end(), [](const largeIntegerType& el) -> void {std::cout << el << " "; }); // printing sign
    std::cout << "\n"; 

    // encrypt message with public key (only sender knows private key, everyone else know public key)
    std::string messagePrototype = encryptMessage(sign, keys._public);

    std::cout << "Confused message prototype (encrypted sign): " << messagePrototype << "\n";
    if (messagePrototype == message)
    {
        std::cout << "Message and message prototype are equal. Signed succesfully!\n\n"; 
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
    message = to_string(confuseData(message, senderKeys._public));
    std::cout << "Confused message: " << message << "\n";


    // create new sign using sender private key
    std::vector <largeIntegerType> senderSign = cryptMessage(message, senderKeys._private);
    std::string senderSignString = to_string(senderSign);
    std::cout << "Sender sign: " << senderSignString << "\n";



    // crypt senderSign and message using receiver public key

    std::vector <largeIntegerType> cryptedSenderSign = cryptMessage(senderSignString, receiverKeys._public);
    std::vector <largeIntegerType> cryptedMessage_1 = cryptMessage(message, receiverKeys._public);
    std::cout << "Crypted sender sign: ";
    for (auto i:cryptedSenderSign) std::cout << i << " ";
    std::cout << "\n"; 

    std::cout << "Crypted message: ";
    for (auto i:cryptedMessage_1) std::cout << i << " ";
    std::cout << "\n";


    // encrypt senderSign and message using receiver private key
    std::cout << "Encrypted sender sign: " << encryptMessage(cryptedSenderSign, receiverKeys._private) << "\n";
    std::cout << "Encrypted confused message: " << encryptMessage(cryptedMessage_1, receiverKeys._private) << "\n";
    std::cout << "Encrypted original message: " << deconfuseData(cryptedMessage_1, receiverKeys._private) << "\n";

    end = clock();
    std::cout << "\n\n\nExecution time: " << double(end - start) / CLOCKS_PER_SEC << " secs.";

	return 0;
}