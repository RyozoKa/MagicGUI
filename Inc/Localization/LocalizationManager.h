#pragma once

#include "Template.h"

class MAGICGUIAPI LocalizationManager
{
public:
	//List of maps currently loaded
	static dense_hash_map<HASH, dense_hash_map<HASH, String>*> LocalMaps;
	static dense_hash_map<HASH, String>* CurrentMap;

	static void SetLocale(String Locale);
	static void LoadLocaleFile(String File, String Name);
	static String* GetLocale(HASH ID);

	static String Missing;
};
