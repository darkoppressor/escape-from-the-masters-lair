#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.03";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 8;
	static const long BUILD = 1821;
	static const long REVISION = 10177;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4127;
	#define RC_FILEVERSION 1,8,1821,10177
	#define RC_FILEVERSION_STRING "1, 8, 1821, 10177\0"
	static const char FULLVERSION_STRING[] = "1.8.1821.10177";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 21;
	

}
#endif //VERSION_H
