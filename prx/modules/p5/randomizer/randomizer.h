#ifdef GAME_P5
#ifndef RANDOMIZER_H
#define RANDOMIZER_H

void randomizerInit( void );
void randomizerShutdown( void );


// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"

#include "modules/p5/p5.h"
#include "modules/p5/dc/dc.h"

int isRandomizerEnabled ();
void setRandomizerState ( bool state );
void setRandomizerReady ( bool state );
void SetUpEncounterFlagsRandomizerHook( EncounterFuncStruct* a1, EncounterStructShort* a2 );
void GetEncounterEntryFromTBLHook( int encounterID, encounterIDTBL* tbl, encounterIDTBL* (*originalFnc) (int) );
void AfterBattleHook();

#endif
#endif