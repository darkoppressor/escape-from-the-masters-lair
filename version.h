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
	static const long MINOR = 3;
	static const long BUILD = 2971;
	static const long REVISION = 16482;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 6946;
	#define RC_FILEVERSION 0,3,2971,16482
	#define RC_FILEVERSION_STRING "0, 3, 2971, 16482\0"
	static const char FULLVERSION_STRING[] = "0.3.2971.16482";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 11;
	

}
#endif //VERSION_H
