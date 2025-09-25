#include "../Headers/Test_IOExporters.hpp"

#ifdef __linux__
TEST(Commons, IOExporters_existInitialization) {
    std::map<int, std::unique_ptr<Commons::IOExporters::DescriptorInformation>>& descriptorSet = Commons::IOExporters::descriptorSet;  // Calling by reference
    ASSERT_EQ((int)Commons::IOExporters::OTHERS, (int)descriptorSet.size());
    // Recovering the static map
    Commons::IOExporters::resetDescriptorSet();
}

TEST(Commons, IOExporters_addElementInMap) {
    std::map<int, std::unique_ptr<Commons::IOExporters::DescriptorInformation>>& descriptorSet = Commons::IOExporters::descriptorSet;  // Calling by reference
    FILE* filePointer = fopen("../../../../ExternalData/a.txt", "w+");
    if (filePointer == nullptr) {
        return;
    }
    Commons::IOExporters::changeDescriptor(Commons::IOExporters::STDOUT, fileno(filePointer));
    fprintf(stderr, "The count %d\n", (int)descriptorSet.size());
    ASSERT_EQ((int)Commons::IOExporters::OTHERS + 1, (int)descriptorSet.size());
    fprintf(stderr, "%d\n", descriptorSet[Commons::IOExporters::STDOUT].get()->currentDescriptor);
    for (std::map<int, std::unique_ptr<Commons::IOExporters::DescriptorInformation>>::iterator it = descriptorSet.begin();
         it != descriptorSet.end();
         it++) {
        if (it->first < Commons::IOExporters::STDIN || it->first >= Commons::IOExporters::OTHERS) {
            fprintf(stderr, "The new %d\n", it->first);
        }
    }

    Commons::IOExporters::printFromRegisteredDescriptor(Commons::IOExporters::STDOUT, (const unsigned char*)"this is a [ig]\n");
    Commons::IOExporters::printFromRegisteredDescriptor(Commons::IOExporters::STDOUT, (const unsigned char*)"Apple is a [ig]%d\n", 12);
    Commons::IOExporters::printFromRegisteredDescriptor(-1, (const unsigned char*)"Leo is a [ig]%d\n", 12);

    // Recovering the static map
    Commons::IOExporters::resetDescriptorSet();
    fclose(filePointer);
}

#endif