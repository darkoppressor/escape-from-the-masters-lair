#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.10";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 6;
	static const long BUILD = 3117;
	static const long REVISION = 17267;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 7396;
	#define RC_FILEVERSION 0,6,3117,17267
	#define RC_FILEVERSION_STRING "0, 6, 3117, 17267\0"
	static const char FULLVERSION_STRING[] = "0.6.3117.17267";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 57;
	

}
#endif //VERSION_H
