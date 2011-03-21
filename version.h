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
	static const long BUILD = 1573;
	static const long REVISION = 8791;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3333;
	#define RC_FILEVERSION 1,5,1573,8791
	#define RC_FILEVERSION_STRING "1, 5, 1573, 8791\0"
	static const char FULLVERSION_STRING[] = "1.5.1573.8791";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 73;
	

}
#endif //VERSION_H
