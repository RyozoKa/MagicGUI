#include "Localization/LocalizationManager.h"
#include "HIO.h"

String LocalizationManager::Missing = "Missing Locale";
dense_hash_map<HASH, dense_hash_map<HASH, String>*> LocalizationManager::LocalMaps;
dense_hash_map<HASH, String>* LocalizationManager::CurrentMap = nullptr;

void LocalizationManager::SetLocale(String Locale)
{
	HASH HS = HashName(Locale);
	auto it = LocalMaps.find(HS);
	if(it != LocalMaps.end())
		CurrentMap = (it->second);
}

String * LocalizationManager::GetLocale(HASH ID)
{
	auto it = CurrentMap->find(ID);
	if (it != CurrentMap->end())
		return &it->second;
	return &Missing;
}

void LocalizationManager::LoadLocaleFile(String File, String Name)
{
	TextFileReader FR;
	if(!FR.Open(File))
	{
		printf("LocaleManager Error: Failed to open file %s file not found\n", File.Tochar());
		return;
	}
	dense_hash_map<HASH, String> *Map = new dense_hash_map<HASH, String>;
	while(!FR.IsEof())
	{
		String& Line = FR.ReadLine();
		HDynamicArray<String> Segments = Line.Split("=");	//--	This could be better...
		if(Segments.Size() < 2)
		{
			printf("LocalizationManager Warning: Line %s contains invalid syntax\n", Line.Tochar());
			continue;
		}

		Segments[0].Trim();
		HASH HS = HashName(Segments[0]);
		Segments[1].Trim();
		const unsigned char C = 'Ä';
		(*Map)[HS] = Segments[1];
	}
	HASH H2 = HashName(Name);
	LocalMaps[H2] = Map;
}