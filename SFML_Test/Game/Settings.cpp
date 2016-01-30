#include "dependencies.h"
#include "Settings.h"

// instantiate singleton instance
Settings* Settings::instance = NULL;

Settings* Settings::Instance() {
	if (!instance) {
		instance = new Settings();
	}
	return instance;
}