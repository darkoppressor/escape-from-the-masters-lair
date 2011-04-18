#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.04";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 2;
	static const long BUILD = 2903;
	static const long REVISION = 16123;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 6702;
	#define RC_FILEVERSION 0,2,2903,16123
	#define RC_FILEVERSION_STRING "0, 2, 2903, 16123\0"
	static const char FULLVERSION_STRING[] = "0.2.2903.16123";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 43;
	

}
#endif //VERSION_H
