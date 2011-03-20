#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.03";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 3;
	static const long BUILD = 1399;
	static const long REVISION = 7841;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2968;
	#define RC_FILEVERSION 1,3,1399,7841
	#define RC_FILEVERSION_STRING "1, 3, 1399, 7841\0"
	static const char FULLVERSION_STRING[] = "1.3.1399.7841";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 99;
	

}
#endif //VERSION_H
