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

    // Creating an object
    PCAP::LinuxPCAP pcapObject;
    pcapObject.open("ens2f1", BUFSIZ, 1, 1000);
	pcapObject.execute();
	pcapObject.close();

    return result;
}