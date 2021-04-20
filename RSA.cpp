#include "RSA.h"

Keys calculateRSAKeys()
{
    largeIntegerType p = rand() % 10000 + 500;
    while (!prime(p)) { ++p; }
    largeIntegerType q = rand() % 10000 + 500;
    while (!prime(q)) { ++q; }

    largeIntegerType n = p * q;

    largeIntegerType functionE = (p - 1) * (q - 1);

    largeIntegerType e = 1;
    for (largeIntegerType i = functionE - 1; i > 0; --i)
    {
        if (gcd(i, functionE) == 1 && prime(i))
        {
            e = i;
            break;
        }
    }

    largeIntegerType d;
    for (largeIntegerType i = 0;; ++i)
    {
        if ((largeIntegerType)i * (largeIntegerType)e % (largeIntegerType)functionE == 1)
        {
            d = i;
            break;
        }
    }

    Keys keys{ std::pair<largeIntegerType, largeIntegerType> {e, n}, std::pair<largeIntegerType, largeIntegerType> {d, n} };
    return keys;
}

largeIntegerType cryptData(largeIntegerType data, std::pair<largeIntegerType, largeIntegerType> _publicKey)
{
    if (data > _publicKey.second)
    {
        throw std::runtime_error("Invalid data");
    }

    return fastPow(data, _publicKey.first, _publicKey.second);
   /* for (largeIntegerType i = 0; i < _publicKey.first; ++i)
    {
        cryptedData *= data;
        cryptedData %= _publicKey.second;
    }

    return cryptedData;*/
}

largeIntegerType encryptData(largeIntegerType data, std::pair<largeIntegerType, largeIntegerType> _privateKey)
{
    return fastPow(data, _privateKey.first, _privateKey.second);
   /* largeIntegerType encryptedData = 1;
    for (largeIntegerType i = 0; i < _privateKey.first; ++i)
    {
        encryptedData *= data;
        encryptedData %= _privateKey.second;
    }

    return encryptedData;*/
}

std::vector<largeIntegerType> cryptMessage(std::string data, std::pair<largeIntegerType, largeIntegerType> _publicKey)
{ 
    std::vector<largeIntegerType> cryptedMessage; 
    cryptedMessage.reserve(data.size());

    for (char element : data)
    {
        cryptedMessage.push_back(cryptData(element, _publicKey));
    }

    return cryptedMessage;
}

std::string encryptMessage(std::vector<largeIntegerType> data, std::pair<largeIntegerType, largeIntegerType> _privateKey)
{
    std::string encryptedMessage;

    for (largeIntegerType element : data)
    {
        encryptedMessage.push_back(encryptData(element, _privateKey));
    }

    return encryptedMessage;
}


