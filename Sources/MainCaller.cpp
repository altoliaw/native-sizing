/**
 * @see MainCaller.hpp
 */
#include "../Headers/MainCaller.hpp"

/**
 * The starting process
 *
 * @param argC [int] The number of the argument
 * @param argV [char**] The array of the argument
 * @return [int]
 */
int start(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;
    // char* interfaceName = (argC <= 1) ? (char*)"ens2f1" : argV[0] ;

    // {
    //     Commons::IOSpecification* instance = Commons::IOSpecification::getSingleton();
    //     std::cerr << "The result:" << instance << "\n";
    //     instance = Commons::IOSpecification::getSingleton();
    //     std::cerr << "The result2:" << instance << "\n";
    // }

    // {
    //     Commons::HashTable* hashtable = new Commons::HashTable();
    //     hashtable->addElementIntoHashTable((char*)"AE.NO", (char*)"3");
    //     hashtable->addElementIntoHashTable((char*)"AE.NO2", (char*)"4");
    //     char* result = nullptr;
    //     int number = hashtable->getValueByName((char*)"AE.NO", &result);
    //     std::cerr << (int)number << "," << ((result == nullptr) ? (char*)"No item" : result) << "\n";
    //     delete hashtable;
    // }

    // {   // Creating an object, opening the interface, executing the packet calculations
    //     // and closing the interface
    //     PCAP::LinuxPCAP pcapObject;
    //     pcapObject.open(interfaceName, BUFSIZ, 1, 1000);
    //     pcapObject.execute();
    //     pcapObject.close();
    // }

    return result;
}