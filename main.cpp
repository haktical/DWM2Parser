#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cstring>

#define MON_BYTES 47
#define ENKA_BYTES 26

using namespace std;

// Declare structs for the stuff we're looking for..
// Right now, only contains arrays of bytes. Might add data later..

 typedef struct {                       // Create type for enemy stats to track

    unsigned char data[MON_BYTES];

} monster ;


typedef struct {						// Type for Spells

} spell ;


typedef struct {						// Type for encounters

	unsigned char data[ENKA_BYTES];

} encounter ;


// Declare functions, they're at the bottom of the file
void parseCobi();
void parseTara();
void parseMonsters( FILE* fin, FILE* fout, std::list<monster>* monsterList);
void parseEncounters( FILE* fin, FILE* fout, std::list<encounter>* encounterList);
void parseSpells( FILE* fin, FILE* fout, std::list<spell>* spellList);


int main()
{

    // The ROM should have this file name and be moved into the folder with the .exe
    const char* IN_FILE_T = "DWM2_T.gbc";
    const char* IN_FILE_C = "DWM2_C.gbc";

    // Output files
    const char* OUT_FILE_MON_T = "monster_t.csv";
    const char* OUT_FILE_SPELL_T = "spell_t.csv";
    const char* OUT_FILE_ENKA_T = "encounter_t.csv";

    const char* OUT_FILE_MON_C = "monster_c.csv";
    const char* OUT_FILE_SPELL_C = "spell_c.csv";
    const char* OUT_FILE_ENKA_C = "encounter_c.csv";


    // This will store our enemy info
    std::list<monster> monsterList;
    std::list<spell> spellList;
    std::list<encounter> encounterList;


    FILE* fin_t;
    FILE* fin_c;
    FILE* fout;


    // Open file - Binary Mode is required for ROM
    fin_t = fopen( IN_FILE_T, "rb" );
    fin_c = fopen( IN_FILE_C, "rb" );


	// Start with Tara's
	if( fin_t != NULL)
	{
		cout << "Tara's available, parsing..." << endl;

		// Try open monster output file
		fout = fopen ( OUT_FILE_MON_T, "w" );

		// If file open, parse monsters
		if( fout != NULL )
		{
			parseMonsters(fin_t, fout, &monsterList);
		}

		// Monsters are done, close output
		fclose(fout);


		// If file opens, parse spells
		fout = fopen( OUT_FILE_SPELL_T, "w" );

		if( fout != NULL )
		{
			//parseSpells(fin_t, fout, &spellList);
		}

		fclose(fout);


		// If file opens, parse encounters
		fout = fopen( OUT_FILE_ENKA_T, "w" );

		if( fout != NULL )
		{
			parseEncounters(fin_t, fout, &encounterList);
		}

		fclose(fout);


		// Close ROM file
		fclose(fin_t);

	}

	// Re-initialize variables to clear them
	monsterList.clear();
	spellList.clear();
	encounterList.clear();

	// Parse Cobi if available
	if( fin_c != NULL)
	{

		cout << "Cobi's available, parsing..." << endl;

		// Try open monster output file
		fout = fopen ( OUT_FILE_MON_C, "w" );

		// If file open, parse monsters
		if( fout != NULL )
		{
			parseMonsters(fin_c, fout, &monsterList);
		}

		// Monsters are done, close output
		fclose(fout);


		// If file opens, parse spells
		fout = fopen( OUT_FILE_SPELL_C, "w" );

		if( fout != NULL )
		{
			//parseSpells(fin_c, fout, &spellList);
		}

		fclose(fout);


		// If file opens, parse encounters
		fout = fopen( OUT_FILE_ENKA_C, "w" );

		if( fout != NULL )
		{
			parseEncounters(fin_c, fout, &encounterList);
		}

		fclose(fout);


		// Close ROM file
		fclose(fin_c);

	}


    return 0;
}



void parseMonsters( FILE* fin, FILE* fout, std::list<monster>* monsterList)
{

	const int numMonsters = 313;
	const int addrMonster = 0xD4368;

	// Okay, this is a second monster output file
	// Monsters seem to have 2 IDs, the second is from SRAM?
	// THIS second output has monster IDs as referenced in the ENCOUNTER table
	const char* OUT_FILE_MONSTER_T_2 = "monsters_t_2.csv";

	FILE* fout2 = fopen( OUT_FILE_MONSTER_T_2, "w" );

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
	// The u# columns are bytes where I don't know what they represent
	// Resist headings only list 1 resist each, but some are series of resists
	fprintf( fout, "index, u0, u1, Family, u3, u4, u5, u6, u7, MAX LVL, XP Type, "
			"sk0, sk1, sk2, idk, HP, MP, ATK, DEF, SPD, INT,"
			"Blaze, Fireball, Bang, Infernos, Lightning, Icebolt, Surround, Sleep, Beat, RobMagic, "
			"Stopspell, Panic, Sap, Slow, Sacrifice, MegaMagic, FireAir, FrigidAir, PoisonAir, "
			"Paralyze, Curse, LureDance, DanceShut, MouthShut, RockThrow, GigaSlash, Geyser\n");

	// Start these at 1 for readability
	// monCount1 is for the monster table directly
	// monCount2 is for the version with "filler" used in enka table
	int monCount1 = 1;
	int monCount2 = 1;

	// I could probably find a better way to do this..
	// The only thing we skip is Butch anyway..
	int numSkipped1 = 0;

	for( list<monster>::iterator it = (*monsterList).begin(); it != (*monsterList).end(); ++it)
	{

		// This is a list of "junk"
		// If we use these lines as "fill" then IDs match up with encounter table
		while(
				(monCount2 > 0x1B && monCount2 < 0x24) ||
				(monCount2 > 0x42 && monCount2 < 0x47) ||
				(monCount2 > 0x66 && monCount2 < 0x6A) ||
				(monCount2 > 0x84 && monCount2 < 0x8D) ||
				(monCount2 > 0xA7 && monCount2 < 0xB0) ||
				(monCount2 > 0xC9 && monCount2 < 0xD3) ||
				(monCount2 > 0xF0 && monCount2 < 0xF6) ||
				(monCount2 > 0x110 && monCount2 < 0x119) ||
				(monCount2 > 0x138 && monCount2 < 0x13C) ||
				(monCount2 > 0x15B && monCount2 < 0x15F)
			  )
		{
			// Just output a number here.. no data to go with it
			// These lines are just filler so encounter table IDs match up
			fprintf( fout2, "%u\n", monCount2 );
			monCount2++;
		}

		// This is Butch. He.. doesn't count?
		// This is mostly so the IDs match up with other tables e.g. DQRTA data set
		if(monCount1 == 27)
		{
			numSkipped1++;
		}
		// This is outputting monster data
		else
		{

			fprintf( fout, "%u,", monCount1-numSkipped1 );
			fprintf( fout2, "%u,", monCount2 );

			for( int i = 0; i < MON_BYTES; ++i)
			{

					// Parse family from the string array
					if(i == 2)
					{
						fprintf( fout, "%s,", families[(int)(*it).data[i]].c_str() );
						fprintf( fout2, "%s,", families[(int)(*it).data[i]].c_str() );
					}
					else if(i != MON_BYTES - 1)
					{
						fprintf( fout, "%u,", (*it).data[i]);
						fprintf( fout2, "%u,", (*it).data[i]);
					}
					// Don't put the comma on the last one..
					else
					{
						fprintf( fout, "%u", (*it).data[i]);
						fprintf( fout2, "%u", (*it).data[i]);
					}

			}

			// New line
			fprintf( fout, "\n");
			fprintf( fout2, "\n");

		}

		monCount1++;
		monCount2++;

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
	fprintf( fout, "index, Monster LB, Monster UB, Skill 1, Skill 2, Skill 3, Skill 4, XP LB, XP UB, ??, Join rate?, HP LB, HP UB, MP LB, MP UB,"
			"ATK LB, ATK UB, DEF LB, DEF UB, AGL LB, AGL UB, INT LB, INT UB, personality b1?, personality b2?, personality b3?, personality b4?\n");

	int enkaCount = 1;
	for( list<encounter>::iterator it = (*encounterList).begin(); it != (*encounterList).end(); ++it)
	{

		// Replace 0x%x with %u if decimal is preferred.. TBD?

		fprintf( fout, "%u,", enkaCount);

		for( int i = 0; i < ENKA_BYTES; ++i)
		{
			if( i != ENKA_BYTES - 1 )
			{
				fprintf( fout, "%u,", (*it).data[i]);
			}
			else
			{
				fprintf( fout, "%u", (*it).data[i]);
			}
		}

		fprintf( fout, "\n");

		enkaCount++;

	}

}

void parseSpells( FILE* fin, FILE* fout, std::list<spell>* spellList)
{

}
