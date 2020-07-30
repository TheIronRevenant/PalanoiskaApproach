#ifndef PSETTINGS //PSETTINGS for uniquity
#define PSETTINGS

/*
Used to store various game settings
*/

namespace GameSettings {
	struct WindowSettings {
		unsigned int screenw = 100;
		unsigned int screenh = 100;
		unsigned int resw = 16;
		unsigned int resh = 9;
	};
};

#endif