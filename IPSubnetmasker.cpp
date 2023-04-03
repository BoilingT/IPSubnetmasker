#include <iostream>
#include <string>
#include <bitset>
#include <vector>

void printb(int value);
std::vector<std::string> split(std::string str, std::string delimiter);
std::string concatenateString(std::string s, std::string delimiter);
int countChar(std::string str, char c);
int countBits(int* bits, const int limit);

void StringToBits(std::string s, int* out, const int size /*Amount of bits*/);
std::string BitsToString(int* bytes, const int size /*Amount of bits*/);

void bitewiseAND(int* bits1, int* bits2, int* out, const int size);
void bitewiseOR(int* bits1, int* bits2, int* out, const int size);
void bitewiseNOT(int* bits, int* out, const int size);

std::string BinToIP(int* bits, const int size);
void IPtoBin(std::string IP, int* bitsOut, const int size /*Amount of bits*/);
int* SubnetmaskToBin(std::string mask, int size /*Amount of bits*/);

int main()
{
    const unsigned int BITS = 4 * 8;
    std::string SubnetmaskStr     = "255.255.240.0";
    std::string IPstr             = "192.168.200.201";
    
    int IpBits[BITS];
    int SubnetmaskBits[BITS];

    while (countBits(IpBits, BITS) < BITS)
    {
        std::cout << "IP: ";
        std::cin >> IPstr; 
        IPtoBin(IPstr, IpBits, BITS); //Convert IP to binary
        if (countBits(IpBits, BITS) < BITS) std::cout << "Wrong format was entered (expected: x.x.x.x)" << std::endl;
    }
    std::string IpBitString = BitsToString(IpBits, BITS);

    while (countBits(SubnetmaskBits, BITS) < BITS)
    {
        std::cout << "Subnetmask: ";
        std::cin >> SubnetmaskStr;
        IPtoBin(SubnetmaskStr, SubnetmaskBits, BITS); //Convert mask to binary
        if (countBits(SubnetmaskBits, BITS) < BITS) std::cout << "Wrong format was entered (expected: x.x.x.x)" << std::endl;
    }
    std::string SubnetmaskBitString = BitsToString(SubnetmaskBits, BITS);

    system("cls");
    std::cout << "IP: " << std::endl;
    std::cout << IPstr << std::endl;
    std::cout << IpBitString << std::endl;

    std::cout << "\nSubnetmask: " << std::endl;
    std::cout << SubnetmaskStr << std::endl;
    std::cout << SubnetmaskBitString << std::endl;

    int NetworkIdBits[BITS];
    bitewiseAND(IpBits, SubnetmaskBits, NetworkIdBits, BITS);
    std::string NetworkIDBitsString = BitsToString(NetworkIdBits, BITS);
    std::string NetworkId = BinToIP(NetworkIdBits, BITS);

    std::cout << "\nNetwork ID: " << std::endl;
    std::cout << NetworkId << std::endl;
    std::cout << NetworkIDBitsString << std::endl;

    //Invert Subnetmask
    int InvertedSubnetmaskBits[BITS];
    bitewiseNOT(SubnetmaskBits, InvertedSubnetmaskBits, BITS);
    //AND Inverted mask with IP
    int DeviceIdBits[BITS];
    bitewiseAND(IpBits, InvertedSubnetmaskBits, DeviceIdBits, BITS);
    std::string DeviceIDBitsString = BitsToString(DeviceIdBits, BITS);
    std::string DeviceId = BinToIP(DeviceIdBits, BITS);

    std::cout << "\nDevice ID: " << std::endl;
    std::cout << DeviceId << std::endl;
    std::cout << DeviceIDBitsString << std::endl;
    system("pause");
}

void printb(int value) {
    std::cout << std::bitset<8>(value) << std::endl;
}

std::vector<std::string> split(std::string str, std::string delimiter) {
    std::vector<std::string> tokens;

    int lastPos = 0;
    int newPos = 0;
    std::string token;

    while ((newPos = str.find(delimiter, lastPos)) != std::string::npos)
    {
        token = str.substr(lastPos, newPos-lastPos);
        lastPos = newPos+1;
        tokens.push_back(token);
    }
    token = str.substr(lastPos, newPos - lastPos);
    tokens.push_back(token);

    return tokens;
}

void bitewiseAND(int* bits1, int* bits2, int* out, const int size) {
    for (int i = 0; i < size; i++)
    {
        try
        {
            int bit1, bit2;
            bit1 = bits1[i];
            bit2 = bits2[i];

            out[i] = bit1 & bit2;
        }
        catch (const std::exception&)
        {
            std::cout << "ERROR: Could not perform bitewise AND operation" << std::endl;
            return;
        }
    }
}

void bitewiseOR(int* bits1, int* bits2, int* out, const int size) {
    for (int i = 0; i < size; i++)
    {
        try
        {
            int bit1, bit2;
            bit1 = bits1[i];
            bit2 = bits2[i];

            out[i] = bit1 | bit2;
        }
        catch (const std::exception&)
        {
            std::cout << "ERROR: Could not perform bitewise OR operation" << std::endl;
            return;
        }
    }
}

void bitewiseNOT(int* bits, int* out, const int size) {
    for (int i = 0; i < size; i++)
    {
        try
        {
            out[i] = ~bits[i];
        }
        catch (const std::exception&)
        {
            std::cout << "ERROR: Could not perform bitewise NOT operation" << std::endl;
            return;
        }
    }
}

void StringToBits(std::string s, int* out, const int size) {
    const char* characters = s.c_str();
    
    for (int i = 0; i < size; i++)
    {
        try
        {
            out[i] = std::stoi(std::bitset<1>(characters[i]).to_string());
        }
        catch (const std::exception&)
        {
            std::cout << "ERROR: Could not convert char to int" << std::endl;
            return;
        }
    }

}

std::string BitsToString(int* bits, const int size) {
    std::string byteString = "";

    for (int i = 0; i < size; i++)
    {
        try
        {
            byteString.append(std::bitset<1>(bits[i]).to_string());
        }
        catch (const std::exception&)
        {
            std::cout << "ERROR: Could not convert from int to string";
            return "ERROR";
        }
    }

    return byteString;
}

void IPtoBin(std::string ip, int* bitsOut, const int size  /*Amount of bits*/) {
    //Convert string to one continous binary int
    std::vector<std::string> tokens = split(ip, ".");
    std::string ipStr = "";

    for (std::string token : tokens)
    {
        std::string value = std::bitset<8>(std::stoi(token)).to_string();
        ipStr.append(value);
    }

    StringToBits(ipStr, bitsOut, ipStr.length() /*Amount of bits*/);

}

std::string BinToIP(int* bits, const int size) {
    std::string IPstr = "";

    int byte = 0;
    std::string byteStr = "";
    for (int i = 0; i < size/8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int bit = bits[j + i*8];
            byteStr.append(std::to_string(bit));
        }
        byte = stoi(byteStr, 0, 2);
        byteStr = "";
        IPstr.append(std::to_string(byte) + (i+1 < size/8 ? "." : ""));
    }
    return IPstr;
}

int* SubnetmaskToBin(std::string maskStr, const int bytes) {
    //Convert string to one continous binary int
    int* mask = nullptr;

    std::vector<std::string> tokens = split(maskStr, ".");
    std::string fullMaskStr = "";

    for (std::string token : tokens)
    {
        std::string value = std::bitset<8>(std::stoi(token)).to_string();
        fullMaskStr.append(value);
    }
    std::cout << fullMaskStr << std::endl;
    return mask;
}

std::string concatenateString(std::string s, std::string delimiter) {
    std::vector<std::string> tokens = split(s, ".");
    std::string result = "";

    for (std::string token : tokens)
    {
        result.append(token);
    }
    return result;
}

int countChar(std::string str, char c) {
    int amount = 0;
    for (char x : str) {
        if (x == c) amount++;
    }
    return amount;
}

int countBits(int* bits, const int limit) {
    for (unsigned int i = 0; i < limit; i++) {
        int bit = bits[i];
        if (bit != 1 && bit != 0) {
            return i+1;
        }
    }
    return limit;
}