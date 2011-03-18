#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.03";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 3;
	static const long BUILD = 1330;
	static const long REVISION = 7441;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2789;
	#define RC_FILEVERSION 1,3,1330,7441
	#define RC_FILEVERSION_STRING "1, 3, 1330, 7441\0"
	static const char FULLVERSION_STRING[] = "1.3.1330.7441";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 30;
	

}
#endif //VERSION_H
