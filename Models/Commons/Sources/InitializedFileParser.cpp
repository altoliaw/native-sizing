/**
 * @see InitializedFileParser.hpp
 */
#include "../Headers/InitializedFileParser.hpp"

namespace Commons {
	// Initialization the nullptr to the static variable in the class
	InitializedFileParser* InitializedFileParser::initializedFileParserPointer = nullptr;

	/**
	 * Constructor
	 */
	InitializedFileParser::InitializedFileParser() {
	}

	/**
	 * Destructor
	 */
	InitializedFileParser::~InitializedFileParser() {	
	}

	/**
	 * The static method for initializing the singleton
	 * 
	 * @return [InitializedFileParser*] The pointer to the "InitializedFileParser" instance
	 */
	InitializedFileParser* InitializedFileParser::getInitializedFileParserInitialization() {

		// A static instance created in function
		static InitializedFileParser initializedFileParser;

		// If the pointer is 
		if(InitializedFileParser::initializedFileParserPointer == nullptr) {
			InitializedFileParser::initializedFileParserPointer = &initializedFileParser;
		}

		return InitializedFileParser::initializedFileParserPointer;
	}

}