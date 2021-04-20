#include <iostream>  
#include <cstdlib>
#include <ctime>
#include <time.h>

#include "utils.h"
#include "RSA.h"

int main()
{
    clock_t start, end;
    start = clock();
    srand(static_cast<unsigned>(time(0)));

    Keys keys = calculateRSAKeys();

    largeIntegerType data = 19;
    std::cout << "Message: " << data << "\n";
    largeIntegerType crData = cryptData(data, keys._public);
    std::cout << "Crypted message: " << crData << "\n";
    std::cout << "Encrypted message: " << encryptData(crData, keys._private) << "\n\n";

    std::string message = "text!";
    std::cout << "Message: " << message << "\n";
    std::vector<largeIntegerType> cryptedMessage = cryptMessage(message, keys._public);
    std::cout << "Crypted message: ";
    std::for_each(cryptedMessage.begin(), cryptedMessage.end(), [](const largeIntegerType& el) -> void {std::cout << el << " "; });
    std::cout << "\n"; 
    std::cout << "Encrypted message: " << encryptMessage(cryptedMessage, keys._private) << "\n\n";

    // crypt sign like crypting message (with public key)
    std::string sign = "Sign: Ilya Lazuk";
    std::vector <largeIntegerType> cryptedSign = cryptMessage(sign, keys._public);

    std::cout << "Crypted sign: ";
    std::for_each(cryptedSign.begin(), cryptedSign.end(), [](const largeIntegerType& el) -> void {std::cout << el << " "; });
    std::cout << '\n';

    std::cout << "Encrypted message: " << encryptMessage(cryptedMessage, keys._private) << '\n';
    std::cout << "Encrypted sign: " << encryptMessage(cryptedSign, keys._private) << "\n\n";


    // crypt sign w/o crypting message
    std::vector <largeIntegerType> superCryptedSign = cryptMessage(message, keys._private);
    std::cout << "Sign w/o crypting message: ";
    std::for_each(cryptedSign.begin(), cryptedSign.end(), [](const largeIntegerType& el) -> void {std::cout << el << " "; });
    std::cout << '\n';

    std::string messagePrototype = encryptMessage(superCryptedSign, keys._public);
    std::cout << "Message prototype: " << messagePrototype << '\n';
    std::cout << "Message: " << message << "\n\n";

    std::vector <largeIntegerType> fakeCryptedSign = cryptMessage(message, keys._public);
    messagePrototype = encryptMessage(fakeCryptedSign, keys._public);
    std::cout << "fake Message prototype: " << messagePrototype << '\n';
    std::cout << "real Message: " << message << '\n';
    end = clock();
    std::cout << "\n\n\nExecution time: " << double(end - start) / CLOCKS_PER_SEC << " secs.";

	return 0;
}