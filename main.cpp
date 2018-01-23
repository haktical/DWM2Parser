#include <iostream>
#include <fstream>
#include <list>

using namespace std;

 typedef struct {                               // Create type for enemy stats to track

    int statGrowth[6];
    int resists[27];

} monster ;


typedef struct {

} spell ;


typedef struct {


} encounter ;


int main()
{

    const char* IN_FILE = "DWM2_T.gbc";
    const char* OUT_FILE = "parsed.csv";


    // Set up some constant data
    const int numMonsters = 313;
    const int sizeMonster = 47;

    const int numEncounters = 614;
    const int encounterSize = 26;

    const int addrMonster = 0xD4368;
    const int addrEncounter = 0xD008F;

    const int skillOffset = 10;
    const int statOffset = 14;
    const int resistOffset = 20;



    // This will store our enemy info
    std::list<monster> monsterList;
    std::list<spell> spellList;
    std::list<encounter> encounterList;

    FILE* fin;
    FILE* fout;


    // Open file - Binary Mode is required
    fin = fopen( IN_FILE, "rb");
    fout = fopen ( OUT_FILE, "w");

    // Check if file is open before looping
    if( fin != NULL ) {
    }



    if( fout != NULL ) {
    }

    // Close file handles
    fclose(fin);
    fclose(fout);


    return 0;
}
