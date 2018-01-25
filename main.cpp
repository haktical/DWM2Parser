#include <iostream>
#include <fstream>
#include <list>

#define MON_BYTES 47

using namespace std;


 typedef struct {                               // Create type for enemy stats to track

    unsigned char data[MON_BYTES];

} monster ;


typedef struct {

} spell ;


typedef struct {


} encounter ;

// Declare functions, they're at the bottom of the file
void parseMonsters( FILE* fin, FILE* fout, std::list<monster>* monsterList);
void parseEncounters( FILE* fin, FILE* fout, std::list<encounter>* encounterList);
void parseSpells( FILE* fin, FILE* fout, std::list<spell>* spellList);

int main()
{

    // The ROM should have this file name and be moved into the folder with the .exe
    const char* IN_FILE = "DWM2_T.gbc";
    const char* OUT_FILE_MON = "monster.csv";
    const char* OUT_FILE_SPELL = "spell.csv";
    const char* OUT_FILE_ENKA = "encounter.csv";


    // Set up some constant data

    const int numEncounters = 614;
    const int encounterSize = 26;

    const int addrMonster = 0xD4368;


    // This will store our enemy info
    std::list<monster> monsterList;
    std::list<spell> spellList;
    std::list<encounter> encounterList;

    FILE* fin;
    FILE* fout;


    // Open file - Binary Mode is required
    fin = fopen( IN_FILE, "rb" );


    // Try open monster output file
    fout = fopen ( OUT_FILE_MON, "w" );

    // If file open, parse monsters
    if( fin != NULL && fout != NULL )
    {
		parseMonsters(fin, fout, &monsterList);
    }

	// Monsters are done, close output
	fclose(fout);


	// If file open, parse spells
	fout = fopen( OUT_FILE_SPELL, "w" );

	if( fin != NULL && fout != NULL )
    {
		parseSpells(fin, fout, &spellList);
    }

	fclose(fout);


	// If file open, parse encounters
	fout = fopen( OUT_FILE_ENKA, "w" );

	if( fin != NULL && fout != NULL )
    {
		parseEncounters(fin, fout, &encounterList);
    }

	fclose(fout);


    // Close file handles
    fclose(fin);


    return 0;
}


void parseMonsters( FILE* fin, FILE* fout, std::list<monster>* monsterList)
{

	const int numMonsters = 313;
    const int sizeMonster = 47;
	const int addrMonster = 0xD4368;

	const int numUnknown = 10;
    const int numSkill = 3;
    const int numStat = 6;
    const int numResist = 27;


	// This will store the monster families. Pointer to an array of c-string helps with fprintf later
	char** monFamily = new char*[11];

	// Initialize the c-strings to new char size 10, no family name is longer than that
	for(int i = 0; i < 11; ++i)
	{
		*(monFamily + (sizeof(char*) * i) ) = new char[10];
	}

	// Declare the families.. no idea what the last one is?
	{
	*(monFamily + sizeof(char*) * 0) = "Slime";
	*(monFamily + sizeof(char*) * 1) = "Dragon";
	*(monFamily + sizeof(char*) * 2) = "Beast";
	*(monFamily + sizeof(char*) * 3) = "Bird";
	*(monFamily + sizeof(char*) * 4) = "Plant";
	*(monFamily + sizeof(char*) * 5) = "Bug";
	*(monFamily + sizeof(char*) * 6) = "Devil";
	*(monFamily + sizeof(char*) * 7) = "Zombie";
	*(monFamily + sizeof(char*) * 8) = "Material";
	*(monFamily + sizeof(char*) * 9) = "Water";
	*(monFamily + sizeof(char*) * 10) = "???";
	}


	// Go to monster address
	fseek( fin, addrMonster, SEEK_SET);

	// Loop through monsters
	for ( int monsters = 0; monsters < numMonsters; ++monsters )
	{

		monster mon;

		for( int i = 0; i < MON_BYTES; ++i)
		{
			fread(&mon.data[i], 1, 1, fin);
		}

		(*monsterList).push_back(mon);

	}

	// Output to csv file for analysis

	// Resist headers taken from CT's monster resist sheet, see that for "full" details
	fprintf( fout, "index, u0, u1, Family, u3, u4, u5, u6, u7, MAX LVL, XP Type,\
			sk0, sk1, sk2, idk, HP, MP, ATK, DEF, SPD, INT, \
			Blaze, Fireball, Bang, Infernos, Lightning, Icebolt, Surround, Sleep, Beat, RobMagic,\
			Stopspell, Panic, Sap, Slow, Sacrifice, MegaMagic, FireAir, FrigidAir, PoisonAir,\
			Pralyze, Curse, LureDance, DanceShut, MouthShut, RockThrow, GigaSlash, Geyser\n");

	int monCount = 1;
	for( list<monster>::iterator it = (*monsterList).begin(); it != (*monsterList).end(); ++it)
	{

		// Replace 0x%x with %u if decimal is preferred.. TBD?

		fprintf( fout, "%u,", monCount);

		for( int i = 0; i < MON_BYTES; ++i)
		{
			if(i == 2)
			{
				fprintf( fout, "%s,", *(monFamily+(sizeof(char*) * (int)(*it).data[i])) );
			}
			else
			{
				fprintf( fout, "%u,", (*it).data[i]);
			}

		}

		fprintf( fout, "\n");

		monCount++;


	}

}

void parseEncounters( FILE* fin, FILE* fout, std::list<encounter>* encounterList)
{

}

void parseSpells( FILE* fin, FILE* fout, std::list<spell>* spellList)
{

}
