#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "21";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.04";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 4;
	static const long BUILD = 3075;
	static const long REVISION = 17061;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 7232;
	#define RC_FILEVERSION 0,4,3075,17061
	#define RC_FILEVERSION_STRING "0, 4, 3075, 17061\0"
	static const char FULLVERSION_STRING[] = "0.4.3075.17061";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 15;
	

}
#endif //VERSION_H
