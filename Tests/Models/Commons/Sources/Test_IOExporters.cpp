#include "../Headers/Test_IOExporters.hpp"

#ifdef __linux__

/**
 * Testing the correctness of initialization
 */
TEST(Commons, IOExporters_existInitialization) {
    std::map<int, std::unique_ptr<Commons::IOExporters::DescriptorInformation>>& descriptorSet = Commons::IOExporters::descriptorSet;  // Calling by reference
    ASSERT_EQ((int)Commons::IOExporters::OTHERS, (int)descriptorSet.size());
    // Recovering the static map
    Commons::IOExporters::resetDescriptorSet();
}

/**
 * Testing the correctness of changeDescriptor
 */
TEST(Commons, IOExporters_addElementInMap) {
    std::map<int, std::unique_ptr<Commons::IOExporters::DescriptorInformation>>& descriptorSet = Commons::IOExporters::descriptorSet;  // Calling by reference
    FILE* filePointer = fopen("../../../../Outputs/a.txt", "w+");
    ASSERT_NE(filePointer, nullptr);

    // Using the function, changeDescriptorchangeDescriptor, will register fileno(filePointer) into the map because there is no fileno(filePointer
    // in the map.
    Commons::IOExporters::changeDescriptor(Commons::IOExporters::STDOUT, fileno(filePointer));
    ASSERT_EQ((int)Commons::IOExporters::OTHERS + 1, (int)descriptorSet.size());                                        // The fileno(filePointer) has been registered in the map
    ASSERT_NE(descriptorSet[Commons::IOExporters::STDOUT]->reservedDescriptorInformation.first, -1);                    // The first reserved device will not be equal to -1
    ASSERT_EQ(descriptorSet[Commons::IOExporters::STDOUT]->reservedDescriptorInformation.second, fileno(filePointer));  // The second reserved id will equal to fileno(filePointer)

    // Three writing issues for stdout
    Commons::IOExporters::printFromRegisteredDescriptor(Commons::IOExporters::STDOUT, (const unsigned char*)"This is a boy.\n");
    Commons::IOExporters::printFromRegisteredDescriptor(Commons::IOExporters::STDOUT, (const unsigned char*)"Apple is %d kg.\n", 66);
    Commons::IOExporters::printFromRegisteredDescriptor(-1, (const unsigned char*)"Leo is a %d year-old boy.\n", 18);
    // Commons::IOExporters::printFromRegisteredDescriptor(Commons::IOExporters::STDERR, (const unsigned char*)"%d has another means\n", 888);

    // Recovering the static map
    Commons::IOExporters::resetDescriptorSet();
    fclose(filePointer);  // Users open, users close
}

/**
 * Testing the correctness of recoverDescriptor
 */
TEST(Commons, IOExporters_recoverElementInMap) {
    std::map<int, std::unique_ptr<Commons::IOExporters::DescriptorInformation>>& descriptorSet = Commons::IOExporters::descriptorSet;  // Calling by reference
    FILE* filePointer = fopen("../../../../Outputs/b.txt", "w+");
    if (filePointer == nullptr) {
        return;
    }

    // Using the function, changeDescriptorchangeDescriptor, will register fileno(filePointer) into the map because there is no fileno(filePointer
    // in the map.
    Commons::IOExporters::changeDescriptor(Commons::IOExporters::STDOUT, fileno(filePointer));

    // Recovering the descriptor
    // Using the absent descript for testing how to recover the descript
    std::pair<Commons::POSIXErrors, Commons::IOExporters::DescriptorBehavior> result;
    result = Commons::IOExporters::recoverDescriptor((fileno(filePointer) + 2));
    ASSERT_EQ(result.first, Commons::POSIXErrors::E_AGAIN);  // The element has been ignored, because the (fileno(filePointer) + 2) does not exist in the map
    result = Commons::IOExporters::recoverDescriptor(fileno(filePointer)); // This one will obtain IOExporters::DescriptorBehavior::NONE in the second element
    ASSERT_EQ(result.second, Commons::IOExporters::DescriptorBehavior::NONE);
    result = Commons::IOExporters::recoverDescriptor(Commons::IOExporters::STDOUT); // This one will obtain IOExporters::DescriptorBehavior::SWAP in the second element
    ASSERT_EQ(result.second, Commons::IOExporters::DescriptorBehavior::SWAP);

    // Recovering the static map
    Commons::IOExporters::resetDescriptorSet();
    fclose(filePointer);
}

/**
 * Testing the correctness of releaseDescriptor
 */
TEST(Commons, IOExporters_releaseElementInMap) {
    std::map<int, std::unique_ptr<Commons::IOExporters::DescriptorInformation>>& descriptorSet = Commons::IOExporters::descriptorSet;  // Calling by reference
    FILE* filePointer = fopen("../../../../Outputs/c.txt", "w+");
    if (filePointer == nullptr) {
        return;
    }

    // Using the function, changeDescriptorchangeDescriptor, will register fileno(filePointer) into the map because there is no fileno(filePointer
    // in the map.
    Commons::IOExporters::changeDescriptor(Commons::IOExporters::STDOUT, fileno(filePointer));

    Commons::POSIXErrors result = Commons::IOExporters::releaseDescriptor(Commons::IOExporters::STDOUT); // This descriptor can not be released; in addition, the descriptor will not be recoverred.
    ASSERT_NE(result, Commons::POSIXErrors::OK);

    result = Commons::IOExporters::releaseDescriptor(fileno(filePointer)); // This descriptor can be released; in addition, the descriptor will be recoverred.
    ASSERT_EQ(result, Commons::POSIXErrors::OK);

    // Recovering the static map
    Commons::IOExporters::resetDescriptorSet();
    fclose(filePointer);
}

#endif