#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.04";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 2;
	static const long BUILD = 2959;
	static const long REVISION = 16410;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 6922;
	#define RC_FILEVERSION 0,2,2959,16410
	#define RC_FILEVERSION_STRING "0, 2, 2959, 16410\0"
	static const char FULLVERSION_STRING[] = "0.2.2959.16410";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 99;
	

}
#endif //VERSION_H
