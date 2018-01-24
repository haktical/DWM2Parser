#include <iostream>
#include <fstream>
#include <list>

using namespace std;


 typedef struct {                               // Create type for enemy stats to track

    unsigned char unknown[10];

    unsigned char skill[3];
    unsigned char idk;
    unsigned char stat[6];
    unsigned char resist[27];

} monster ;


typedef struct {

} spell ;


typedef struct {


} encounter ;


int main()
{

    // The ROM should have this file name and be moved into the folder with the .exe
    const char* IN_FILE = "DWM2_T.gbc";
    const char* OUT_FILE_MON = "monster.csv";
    const char* OUT_FILE_SPELL = "spell.csv";
    const char* OUT_FILE_ENKA = "encounter.csv";


    // Set up some constant data
    const int numMonsters = 313;
    const int sizeMonster = 47;

    const int numEncounters = 614;
    const int encounterSize = 26;

    const int addrEncounter = 0xD008F;
    const int addrMonster = 0xD4368;

	// -- Might not actually use these --
	/*
    const int skillOffset = 10;
    const int statOffset = 14;
    const int resistOffset = 20;
	*/

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



    // This will store our enemy info
    std::list<monster> monsterList;
    std::list<spell> spellList;
    std::list<encounter> encounterList;

    FILE* fin;
    FILE* fout;


    // Open file - Binary Mode is required
    fin = fopen( IN_FILE, "rb");




    // TODO: Put into functions?

    // Try open monster output file
    fout = fopen ( OUT_FILE_MON, "w");

    // Check if files are open before looping
    if( fin != NULL && fout != NULL )
    {

		// Go to monster address
		fseek( fin, addrMonster, SEEK_SET);

		// Loop through monsters
		for ( int monsters = 0; monsters < numMonsters; ++monsters )
		{

			monster mon;

			// Read these bytes even though we don't know what they are
			for( int unknown = 0; unknown < numUnknown; ++unknown )
			{

				fread(&mon.unknown[unknown], 1, 1, fin);

			}

			// Read skills
			for( int skill = 0; skill < numSkill; ++skill)
			{

				fread(&mon.skill[skill], 1, 1, fin);

			}

			fread(&mon.idk, 1, 1, fin);

			// Read stats
			for( int stat = 0; stat < numStat; ++stat)
			{

				fread(&mon.stat[stat], 1, 1, fin);

			}

			// Read resists
			for( int resist = 0; resist < numResist; ++resist)
			{

				fread(&mon.resist[resist], 1, 1, fin);

			}


			// DEBUG cout statements
			/*
			cout << "unknown0" << mon.unknown[0] << endl;
			cout << "skill0" << mon.skill[0] << endl;
			cout << "stat0" << mon.stat[0] << endl;
			cout << "resist0" << mon.resist[0] << endl;

			if(monsters == 0)
				printf("skill2: %x \n", mon.skill[2]);
			*/

			monsterList.push_back(mon);

		}



		// Output to csv file for analysis

		//

		fprintf( fout, "index, u0, u1, Family, u3, u4, u5, u6, u7, MAX LVL, XP Type, sk0, sk1, sk2, idk, HP, MP, ATK, DEF, SPD, INT, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26\n");

		int monCount = 1;
		for( list<monster>::iterator it = monsterList.begin(); it != monsterList.end(); ++it)
		{

			// Replace 0x%x with %u if decimal is preferred.. TBD?

			fprintf( fout, "%u,", monCount);

			for( int unknown = 0; unknown < numUnknown; ++unknown )
			{

				if(unknown == 2)
				{

					// monFamily is a pointer to an array of c-strings
					// (*it).unknown[unknown] is the (value) of the family, cast it to int
					// sizeof(char*) * value is the offset from monFamily which stores the relevant string

					fprintf( fout, "%s,", *(monFamily+(sizeof(char*) * (int)(*it).unknown[unknown])) );

				}
				else
				{
					fprintf( fout, "%u,", (*it).unknown[unknown]);
				}

			}

			for( int skill = 0; skill < numSkill; ++skill)
			{

				fprintf( fout, "%u,", (*it).skill[skill]);

			}

			fprintf( fout, "%u,", (*it).idk );

			for( int stat = 0; stat < numStat; ++stat)
			{

				fprintf( fout, "%u,", (*it).stat[stat]);

			}

			for( int resist = 0; resist < numResist; ++resist)
			{

				fprintf( fout, "%u,", (*it).resist[resist]);

			}

			fprintf( fout, "\n");

			monCount++;

		}


    }

	// Monsters are done, close output
	fclose(fout);



    // Close file handles
    fclose(fin);



    return 0;
}
