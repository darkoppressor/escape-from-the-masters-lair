#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.03";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 0;
	static const long BUILD = 2106;
	static const long REVISION = 11761;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4915;
	#define RC_FILEVERSION 2,0,2106,11761
	#define RC_FILEVERSION_STRING "2, 0, 2106, 11761\0"
	static const char FULLVERSION_STRING[] = "2.0.2106.11761";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 6;
	

}
#endif //VERSION_H
