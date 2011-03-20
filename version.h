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
	static const long MINOR = 5;
	static const long BUILD = 1532;
	static const long REVISION = 8559;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3235;
	#define RC_FILEVERSION 1,5,1532,8559
	#define RC_FILEVERSION_STRING "1, 5, 1532, 8559\0"
	static const char FULLVERSION_STRING[] = "1.5.1532.8559";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 32;
	

}
#endif //VERSION_H
