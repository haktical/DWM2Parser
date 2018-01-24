#include <iostream>
#include <fstream>
#include <list>

using namespace std;


 typedef struct {                               // Create type for enemy stats to track

    unsigned char unknown[10];

    unsigned char skill[4];
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
    const int numSkill = 4;
    const int numStat = 6;
    const int numResist = 27;


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

		fprintf( fout, "u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, sk0, sk1, sk2, sk3, st0, st1, st2, st3, st4, st5, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26\n");

		for( list<monster>::iterator it = monsterList.begin(); it != monsterList.end(); ++it)
		{

			// Replace 0x%x with %u if decimal is preferred.. TBD?

			for( int unknown = 0; unknown < numUnknown; ++unknown )
			{

				fprintf( fout, "0x%x,", (*it).unknown[unknown]);

			}

			for( int skill = 0; skill < numSkill; ++skill)
			{

				fprintf( fout, "%u,", (*it).skill[skill]);

			}

			for( int stat = 0; stat < numStat; ++stat)
			{

				fprintf( fout, "%u,", (*it).stat[stat]);

			}

			for( int resist = 0; resist < numResist; ++resist)
			{

				fprintf( fout, "%u,", (*it).resist[resist]);

			}

			fprintf( fout, "\n");

		}


    }

	// Monsters are done, close output
	fclose(fout);



    // Close file handles
    fclose(fin);



    return 0;
}
