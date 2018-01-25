#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cstring>

#define MON_BYTES 47
#define ENKA_BYTES 26

using namespace std;


 typedef struct {                               // Create type for enemy stats to track

    unsigned char data[MON_BYTES];

} monster ;


typedef struct {

} spell ;


typedef struct {

	unsigned char data[ENKA_BYTES];

} encounter ;

// Declare functions, they're at the bottom of the file
void parseMonsters( FILE* fin, FILE* fout, std::list<monster>* monsterList);
void parseEncounters( FILE* fin, FILE* fout, std::list<encounter>* encounterList);
void parseSpells( FILE* fin, FILE* fout, std::list<spell>* spellList);

int main()
{

    // The ROM should have this file name and be moved into the folder with the .exe
    const char* IN_FILE = "DWM2_T.gbc";

    // Output files
    const char* OUT_FILE_MON = "monster.csv";
    const char* OUT_FILE_SPELL = "spell.csv";
    const char* OUT_FILE_ENKA = "encounter.csv";

    // This will store our enemy info
    std::list<monster> monsterList;
    std::list<spell> spellList;
    std::list<encounter> encounterList;


    FILE* fin;
    FILE* fout;

    // Open file - Binary Mode is required for ROM
    fin = fopen( IN_FILE, "rb" );

	if( fin == NULL )
	{

		cout << "File not open, exiting.." << endl;
		return 0;

	}


    // Try open monster output file
    fout = fopen ( OUT_FILE_MON, "w" );

    // If file open, parse monsters
    if( fout != NULL )
    {
		parseMonsters(fin, fout, &monsterList);
    }

	// Monsters are done, close output
	fclose(fout);


	// If file opens, parse spells
	fout = fopen( OUT_FILE_SPELL, "w" );

	if( fin != NULL && fout != NULL )
    {
		//parseSpells(fin, fout, &spellList);
    }

	fclose(fout);


	// If file opens, parse encounters
	fout = fopen( OUT_FILE_ENKA, "w" );

	if( fin != NULL && fout != NULL )
    {
		parseEncounters(fin, fout, &encounterList);
    }

	fclose(fout);


    // Close ROM file
    fclose(fin);


    return 0;
}


void parseMonsters( FILE* fin, FILE* fout, std::list<monster>* monsterList)
{

	const int numMonsters = 313;
	const int addrMonster = 0xD4368;

	string families[11] = {"Slime", "Dragon", "Beast", "Bird", "Plant", "Bug",
		"Devil", "Zombie", "Material", "Water", "Boss"};

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
	fprintf( fout, "index, u0, u1, Family, u3, u4, u5, u6, u7, MAX LVL, XP Type, "
			"sk0, sk1, sk2, idk, HP, MP, ATK, DEF, SPD, INT,"
			"Blaze, Fireball, Bang, Infernos, Lightning, Icebolt, Surround, Sleep, Beat, RobMagic, "
			"Stopspell, Panic, Sap, Slow, Sacrifice, MegaMagic, FireAir, FrigidAir, PoisonAir, "
			"Paralyze, Curse, LureDance, DanceShut, MouthShut, RockThrow, GigaSlash, Geyser\n");

	int monCount = 1;

	for( list<monster>::iterator it = (*monsterList).begin(); it != (*monsterList).end(); ++it)
	{

		// Replace 0x%x with %u if decimal is preferred.. TBD?

		fprintf( fout, "%u,", monCount);

		for( int i = 0; i < MON_BYTES; ++i)
		{
			if(i == 2)
			{
				fprintf( fout, "%s,", families[(int)(*it).data[i]].c_str() );
			}
			else if(i != MON_BYTES - 1)
			{
				fprintf( fout, "%u,", (*it).data[i]);
			}
			else
			{
				fprintf( fout, "%u", (*it).data[i]);
			}

		}

		fprintf( fout, "\n");

		monCount++;

	}

}

void parseEncounters( FILE* fin, FILE* fout, std::list<encounter>* encounterList)
{

	const int addrEncounter = 0xD008F;
    const int numEncounters = 614;

	// Go to encounter address
	fseek( fin, addrEncounter, SEEK_SET);

	// Loop through encounters
	for ( int encounters = 0; encounters < numEncounters; ++encounters )
	{

		encounter enka;

		for( int i = 0; i < ENKA_BYTES; ++i)
		{
			fread(&enka.data[i], 1, 1, fin);
		}

		(*encounterList).push_back(enka);

	}

	// Need to determine encounter data stored
	fprintf( fout, "index, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13"
			"b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25\n");

	int enkaCount = 1;
	for( list<encounter>::iterator it = (*encounterList).begin(); it != (*encounterList).end(); ++it)
	{

		// Replace 0x%x with %u if decimal is preferred.. TBD?

		fprintf( fout, "%u,", enkaCount);

		for( int i = 0; i < ENKA_BYTES; ++i)
		{
			fprintf( fout, "%u,", (*it).data[i]);
		}

		fprintf( fout, "\n");

		enkaCount++;

	}

}

void parseSpells( FILE* fin, FILE* fout, std::list<spell>* spellList)
{

}
