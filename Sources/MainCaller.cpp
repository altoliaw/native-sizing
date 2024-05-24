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

    {
        Commons::IOSpecification* instance = Commons::IOSpecification::getSingleton();
        std::cerr << "The result:" << instance << "\n";
        instance = Commons::IOSpecification::getSingleton();
        std::cerr << "The result2:" << instance << "\n";
    }

    {
        Commons::HashTable* hashtable = new Commons::HashTable();
        delete hashtable;
    }

    {   // Creating an object, opening the interface, executing the packet calculations
        // and closing the interface
        PCAP::LinuxPCAP pcapObject;
        pcapObject.open("ens2f1", BUFSIZ, 1, 1000);
        pcapObject.execute();
        pcapObject.close();
    }

    return result;
}