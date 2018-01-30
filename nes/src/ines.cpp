#include "ines.h"

void ines::load(string path, Memory *mem)
{
	inesPath = path;
	memory = mem;
	inesFile.open(inesPath, ios::binary);

	if(!inesFile.is_open())
		cerr << "Invalid path to .nes file: " << inesPath << "\n";

	loadHeader();
	loadPrgRom();

	memory->isLoaded = true;

	inesFile.close();	
}

void ines::loadHeader()
{
	inesFile.read((char*)inesHeader, 16);
	if(!inesFile)
		cerr << "Failed to read .nes header!\n";
}

void ines::loadPrgRom()
{
	uint8_t sizeOfPrgRom = inesHeader[4]; // Either 1 or 2 in KB units
	int trainerOffset = ((inesHeader[6] & 0x4) == 0) ? 16 : 512 + 16;

	inesFile.seekg(trainerOffset);

	if(sizeOfPrgRom == 1)
	{
		inesFile.read((char*)(memory->programROM), 16384);
		if(!inesFile)
		{
			cerr << "Failed to read program ROM (mirror 1)!\n";
			cerr << "Only " << inesFile.gcount() << " could be read.\n";
		}
		inesFile.seekg(trainerOffset);
		inesFile.read((char*)(&memory->programROM[0x4000]), 16384);
		if(!inesFile)
		{
			cerr << "Failed to read program ROM (mirror 2)!\n";
			cerr << "Only " << inesFile.gcount() << " could be read.\n";
		}
	}
	else
	{
		inesFile.read((char*)(memory->programROM), 32768);
		if(!inesFile)
			cerr << "Failed to read program ROM!\n";
	}
}
