#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.04";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 1;
	static const long BUILD = 2484;
	static const long REVISION = 13773;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 5747;
	#define RC_FILEVERSION 0,1,2484,13773
	#define RC_FILEVERSION_STRING "0, 1, 2484, 13773\0"
	static const char FULLVERSION_STRING[] = "0.1.2484.13773";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 284;
	

}
#endif //VERSION_H
