#ifdef GAME_P5
// PS3 system includes
#include <sys/prx.h>
#include <sys/tty.h>
#include <sys/syscall.h>
// Pre-prepared libraries exist in lib
// Common includes things like printf for printing, strlen, etc.
// PRX dont have access to the usual C libraries, so any functionality that you need from it
// will have to be reimplemented in there.
#include "lib/common.h"

// SHK (Static Hook library)
#include "lib/shk.h"

// Include this to use config variables
#include "lib/config.h"

#include "modules/p5/p5.h"
#include "modules/p5/dc/dc.h"
#include "modules/p5/EXFLW/EXFLW.h"

#include "randomizer.h"

u16 savedParty[3];
// u8 savedJokerStats[12][5];
// u8 savedPartyStats[10][5];
// u32 playerHP[10];
// u32 playerSP[10];

int disallowedEnemies[] = {
  66,  71,  79,  83,  86,  96, 114, 115,
  116, 117, 118, 119, 120, 145, 146, 147,
  148, 149, 150, 151, 152, 153, 154, 155,
  156, 158, 159, 160, 161, 162, 163, 164,
  165, 166, 167, 168, 169, 188, 189, 191,
  192, 193, 200, 276, 332, 334, 350, /* RESERVES */
  //   1,  66,  71,  79,  83,  86,  96, 114, 115, 116, 117, 118,
  // 119, 120, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
  // 155, 156, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
  // 168, 169, 183, 184, 188, 189, 191, 192, 193, 200, 228, 251,
  // 252, 262, 264, 266, 268, 269, 273, 275, 276, 278, 283, 284,
  // 286, 288, 292, 293, 295, 296, 297, 298, 299, 300, 304, 305,
  // 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318,
  // 320, 321, 323, 324, 327, 332, 334, 347, 350, /* RESERVES and Mementos Shadows, both cause instacrash */
  // /* Justine */ 195, /* Caroline */196,
  // /* Snake King (crashes?) */255,
  /* Madarame painting bosses (missing model) */247, 248, 249, 250,
  // /* Other Madarame painting bosses? 0x17c crash but idk if related */206, 207, 208, 209,
  /* Broken unused Akechi */231,
  /* grail & parts, has no model so softlocks */205, 197, 198, 199
};

int disallowedEncounters[] = {
  751, 799, 800, 801, 802, 803, 703, /* unused things missing event scripts */
  788, /* yadabaoth, missing formation */
  702 /* Battle at the start of Yusuke's confidant, missing event script */
};

int doNotRandomize[] = {
  // 97 /* just a Bicorn but for some reason randomizing this has a 50% chance of 0x17c crash for absolutely no reason whatsoever */
};

bool isDisallowedEnemy ( int id ) {
  for (int i = 0; i < sizeof(disallowedEnemies) / sizeof(*disallowedEnemies); i++) {
    if (id == disallowedEnemies[i]) {
      return true;
    }
  }
  return false;
}

// int lastEnemy = -1;
// int lastEnemyRes = -1;

bool isEncounterValid (encounterIDTBL* realtbl, encounterIDTBL* tbl, int encounterID) {
  if (tbl->BattleUnitID[0] == 0) return false;
  // for (int i = 0; i < 5; i++) {
  //   if (realtbl->BattleUnitID[i] == 0 && tbl->BattleUnitID[i] != 0) {
  //     printf("skipping because different enemy amount");
  //     return false;
  //   }
  // // if (realtbl->BattleUnitID[3] == 0 && tbl->BattleUnitID[3] != 0) {
  // //   return false;
  // // }
  // }
  for (int i = 0; i < sizeof(disallowedEncounters) / sizeof(*disallowedEncounters); i++) {
    if (encounterID == disallowedEncounters[i]) {
      return false;
    }
  }
  // if (tbl->flags >> 10 == 0x1e) return false; // event script 30 doesnt exist
  for (int i = 0; i < 5; i++) {
    if (isDisallowedEnemy(tbl->BattleUnitID[i])) {
      return false;
    }
  }
  return true;
}

bool cacheState[815];

void GetEncounterEntryFromTBLHook( int encounterID, encounterIDTBL* tbl, encounterIDTBL* (*originalFnc) (int))
{
  if (isRandomizerEnabled() && CONFIG_ENABLED(randomizeEncounters)) {
    printf("**************************\nRANDOMIZER CALLED ON %d", encounterID);
    if (cacheState[encounterID]) {
      printf("encounter id %d already randomized\n", encounterID);
      return;
    }
    for (int i = 0; i < sizeof(doNotRandomize) / sizeof(*doNotRandomize); i++) {
      if (encounterID == doNotRandomize[i]) return;
    }
    encounterIDTBL* randTbl;
    int randEnc;
    do {
      randEnc = GetRandom(815);
      printf("Getting encounter as replacement %d\n", randEnc);
      randTbl = originalFnc(randEnc);
    } while (!isEncounterValid(tbl, randTbl, randEnc));
    printf("rand flags 0x%x\n", randTbl->flags.byte);
    printf("before flags 0x%x\n", tbl->flags.byte);
    printf("before FieldID %d\n", tbl->FieldID);
    printf("before RoomID %d\n", tbl->RoomID);
    printf("before BGMID %d\n", tbl->BGMID);
    printf("before Field04 %d\n", tbl->Field04);
    printf("before Field06 %d\n", tbl->Field06);
    // if (encounterID != 641) tbl->flags.byte = randTbl->flags.byte; // 641 is the intro fight and will crash without its flags
    // tbl->flags.eventScript = 0x000; // both of theese prevent softlocks
    // tbl->flags.formation = 0x000;
    tbl->FieldID = tbl->FieldID == 0 ? randTbl->FieldID : tbl->FieldID;
    tbl->RoomID = tbl->RoomID == 0 ? randTbl->RoomID : tbl->RoomID;
    tbl->BGMID = randTbl->BGMID;
    tbl->Field04 = randTbl->Field04;
    tbl->Field06 = randTbl->Field06;
    printf("after flags 0x%x\n", tbl->flags.byte);
    printf("after FieldID %d\n", tbl->FieldID);
    printf("after RoomID %d\n", tbl->RoomID);
    printf("after BGMID %d\n", tbl->BGMID);
    printf("after Field04 %d\n", tbl->Field04);
    printf("after Field06 %d\n", tbl->Field06);
    for (int i = 0; i < 5; i++) {
      if (CONFIG_ENABLED(randomizeEnemies)) {
        int chance = 6;
        if (i > 2) {
          chance = GetRandom(6); // 1/6 chance for slots 4 and 5
        }
        if (chance == 6) {
          int val = 349;
          int enemy;
          do {
            enemy = GetRandom(val - 1) + 1;
          } while (isDisallowedEnemy(enemy));
          printf("unit %d originally set to %d set to %d\n", i, tbl->BattleUnitID[i], enemy);
          tbl->BattleUnitID[i] = enemy;
        }
      } else {
        printf("unit %d originally %d set to %d\n", i, tbl->BattleUnitID[i], randTbl->BattleUnitID[i]);
        tbl->BattleUnitID[i] = randTbl->BattleUnitID[i];
      }
    }
    cacheState[encounterID] = true; // no need to store it, the pointers are cached and reused internally apparently.
    printf("encounter id %d randomized\n", encounterID);
    hexDump("Randomized TBL Data", tbl, 24);
  }
}

void SetUpEncounterFlagsRandomizerHook( EncounterFuncStruct* a1, EncounterStructShort* a2)
{
  if (!CONFIG_ENABLED ( enableRandomizerModule )) return;
  if (isRandomizerEnabled()) {
    printf("field00 0x%x\nfield02 0x%x\nfield04 0x%x\n", a2->field00, a2->field02, a2->field04);
    saveParty();
    randomizeParty();
    btlUnit_Unit* Joker = GetBtlPlayerUnitFromID( 1 );
    for (int i = 0; i < 12; i++) // loop through stock
    {
      if (CONFIG_ENABLED(randomizePersonaStats)) {
        printf("randomizing persona %d\n", i);
        if (Joker->StockPersona[i].isUnlocked == 1) {
          printf("persona %d is unlocked id is %d\n", i, Joker->StockPersona[i].personaID);
          for (int x = 0; x < 5; x++) // loop through stats
          {
            Joker->StockPersona[i].Stats[x] = GetRandom(98) + 1;
          }
        }
      }

      if (CONFIG_ENABLED(randomizePersonaSkills)) {
        for (int x = 0; x < 8; x++) // loop through skills
        {
          int skillID = GetRandom(988) + 10;
          printf("randomizing persona %d skill %d to %d\n", i, x, skillID);
          Joker->StockPersona[i].SkillID[x] = skillID;
        }
      }

      if (CONFIG_ENABLED(giveJokerDebugInstakill)) {
        Joker->StockPersona[i].SkillID[0] = 603;
      }
    }
    for (int i = 1; i <= 9; i++) // ID 1 Joker -> ID 10 Violet
    {
      if (i == 8) i++;
      printf("randomizing player %d\n", i);
      btlUnit_Unit* Player = GetBtlPlayerUnitFromID( i );
      if (CONFIG_ENABLED(randomizeHP)) Player->currentHP = GetRandom(998/*GetBtlUnitMaxHP(Player) - 1*/) + 1;
      if (CONFIG_ENABLED(randomizeSP)) Player->currentSP = GetRandom(GetBtlUnitMaxSP(Player));
      if (i != 1) {
        if (CONFIG_ENABLED(randomizePersonaStats)) {
          if (Player->StockPersona[0].isUnlocked == 1) {
            printf("%d's persona is unlocked id is %d\n", i, Player->StockPersona[0].personaID);
            for (int x = 0; x < 5; x++) // loop through stats
            {
              Player->StockPersona[0].Stats[x] = GetRandom(98) + 1;
            }
          }
        }
        if (CONFIG_ENABLED(randomizePersonaSkills)) {
          for (int x = 0; x < 8; x++) // loop through skills
          {
            int skillID = GetRandom(988) + 10;
            printf("randomizing %d's persona's skill %d to %d\n", i, x, skillID);
            Player->StockPersona[0].SkillID[x] = skillID;
          }
        }
      }
    }
    // for (int i = 0; i <= 321; i++) {
    //   for (int x = 1; x <= 10; x++) {
    //     if (i == 170 && x > 10) {
    //       printf("persona %d affinity of %d is %d", i, x, NewPersonaAffinityTBL.PersonaAffinities[i].affinity[x]);
    //     }
    //     int rand = GetRandom(1);
    //     int a;
    //     switch(rand) {
    //       case 0: a = 4096;
    //       case 1: a = 1024;
    //       // case 2: a = 256;
    //       // case 3: a = 20;
    //     }
    //     if (i == 170) {
    //       printf("persona %d affinity of %d is %d", i, x, NewPersonaAffinityTBL.PersonaAffinities[i].affinity[x]);
    //     }
    //     NewPersonaAffinityTBL.PersonaAffinities[i].affinity[x] = a;
    //   }
    // }
    // if (lastEncID != 47) {
    //   int newID = randomizeEncounter(a2->encounterIDLocal);
    //   // a1->encounterIDLocal = (u32)newID;
    //   a2->encounterIDLocal = (u32)newID;
    // }
    // if (a2->encounterIDLocal == 765) {
    //   // bicorn in 4/15 kamoshida
    //   clearParty();
    //   PartyIn(3);
    //   PartyIn(2);
    // }
  }
}

bool isRandomizerReady = false;
bool randomizerActive = false;

void setRandomizerReady ( bool state )
{
  if (!CONFIG_ENABLED ( enableRandomizerModule )) return;
  isRandomizerReady = state;
  printf("randomizer ready state is %d\n", state);
}

int isRandomizerEnabled ()
{
  return randomizerActive && CONFIG_ENABLED( enableRandomizerModule );
}

void clearParty() {
  PartyOut(2);
  PartyOut(3);
  PartyOut(4);
  PartyOut(5);
  PartyOut(6);
  PartyOut(7);
  PartyOut(8);
  PartyOut(9);
  PartyOut(10);
}

void saveParty() {
  if (!CONFIG_ENABLED(randomizePartyMembers)) return;
  for (int x = 1; x < 4; x++ ) {
    printf("saving %d\n", x);
    savedParty[x-1] = GetUnitIDFromPartySlot(x);
  }
  // btlUnit_Unit* Joker = GetBtlPlayerUnitFromID( 1 );
  // for (int i = 0; i < 12; i++) // loop through stock
  // {
  //   printf("saving persona %d\n", i);
  //   // Joker->StockPersona[i].personaID = GetRandom(320) + 1;
  //   for (int x = 0; x < 5; x++) // loop through stats
  //   {
  //     if (Joker->StockPersona[i].personaID) {
  //       savedJokerStats[i][x] = Joker->StockPersona[i].Stats[x];
  //     }
  //   }
  //   // for (int x = 0; x < 8; x++) // loop through skills
  //   // {
  //   //   Joker->StockPersona[i].SkillID[x] = GetRandom(988) + 10;
  //   // }
  // }
  // for (int i = 2; i <= 10; i++) // ID 1 Joker -> ID 10 Violet
  // {
  //   if (i == 8) i++;
  //   printf("saving player %d\n", i);
  //   btlUnit_Unit* Player = GetBtlPlayerUnitFromID( i );
  //   playerHP[i] = Player->currentHP;
  //   playerSP[i] = Player->currentSP;
  //   for (int x = 0; x < 5; x++) // loop through stats
  //   {
  //     if (Player->StockPersona[0].personaID) {
  //       savedPartyStats[i][x] = Player->StockPersona[0].Stats[x];
  //     }
  //   }
  //   // for (int x = 0; x < 8; x++) // loop through skills
  //   // {
  //   //   Player->StockPersona[0].SkillID[x] = GetRandom(988) + 10;
  //   // }
  // }
}

void loadParty() {
  if (!CONFIG_ENABLED(randomizePartyMembers)) return;
  clearParty();
  for (int x = 0; x < 3; x++ ) {
    PartyIn(savedParty[x]);
  }
  // btlUnit_Unit* Joker = GetBtlPlayerUnitFromID( 1 );
  // for (int i = 0; i < 12; i++) // loop through stock
  // {
  //   printf("loading persona %d\n", i);
  //   // Joker->StockPersona[i].personaID = GetRandom(320) + 1;
  //   for (int x = 0; x < 5; x++) // loop through stats
  //   {
  //     if (Joker->StockPersona[i].personaID) {
  //       Joker->StockPersona[i].Stats[x] = savedJokerStats[i][x];
  //     }
  //   }
  //   // for (int x = 0; x < 8; x++) // loop through skills
  //   // {
  //   //   Joker->StockPersona[i].SkillID[x] = GetRandom(988) + 10;
  //   // }
  // }
  // for (int i = 2; i <= 10; i++) // ID 1 Joker -> ID 10 Violet
  // {
  //   if (i == 8) i++;
  //   printf("loading player %d\n", i);
  //   btlUnit_Unit* Player = GetBtlPlayerUnitFromID( i );
  //   Player->currentHP = playerHP[i];
  //   Player->currentSP = playerSP[i];
  //   for (int x = 0; x < 5; x++) // loop through stats
  //   {
  //     if (Player->StockPersona[0].personaID) {
  //       Player->StockPersona[0].Stats[x] = savedPartyStats[i][x];
  //     }
  //   }
  //   // for (int x = 0; x < 8; x++) // loop through skills
  //   // {
  //   //   Player->StockPersona[0].SkillID[x] = GetRandom(988) + 10;
  //   // }
  // }
}

void randomizeParty() {
  if (!CONFIG_ENABLED(randomizePartyMembers)) return;
  clearParty();
  int memberAmount = 3;
  int members[memberAmount];
  int toCheck[memberAmount - 1];
  for (int x = 0; x < memberAmount; x++) {
    int z;
    for (int y = 0; y < memberAmount; y++) {
      if (y != x) {
        toCheck[z] = members[y];
        z++;
      }
    }

    int rand;
    bool duplicateCheck = true;
    while(duplicateCheck) {
      duplicateCheck = false;
      rand = GetRandom(7) + 2;
      if (rand == 8) duplicateCheck = true; // don't allow futaba to be in the party
      for (int i = 1; i <= z; i++) {
        // printf("checking %d against %d\n", rand, toCheck[i]);
        if (rand == toCheck[i]) {
          duplicateCheck = true;
        }
      }
    }
    members[x] = rand;
    PartyIn(rand);
  }
}

void AfterBattleHook() {
  if (!CONFIG_ENABLED ( enableRandomizerModule )) return;
  printf("unrandomizing party");
  loadParty();
  for (int i = 0; i < sizeof(cacheState) / sizeof(*cacheState); i++) {
    cacheState[i] = false;
  }
}

void setRandomizerState ( bool state )
{
  // if (state) {
  //   saveParty();
  // } else {
  //   loadParty();
  // }
  randomizerActive = state;
  printf("randomizer set to %d", state);

  // loadBF("script/randomizer/init.bf"); // shows futaba cutin, this is a giant hack and needs to be replaced with C
}
int* padPtr = (int*)0x1166b1c;
bool gameHasStarted = false;
// f32 partyRandomizerSeconds = 0;
void randomizerUpdate ( f32 deltaTime )
{
  if (isRandomizerReady) {
    if (!gameHasStarted) {
      int days = GetTotalDays();
      if (days > 0) {
        gameHasStarted = true;
        saveParty();
        randomizerActive = true;
      }
      return;
    }
    // if (randomizerActive) {
    //   if (shouldRandomizeParty) {
    //     if (partyRandomizerSeconds >= 1) {
    //       partyRandomizerSeconds = 0;
    //       int days = GetTotalDays();
    //       if (days > 10) {
    //         randomizeParty();
    //       }
    //     }
    //     partyRandomizerSeconds += deltaTime;
    //   }
    // }
    int pad = *padPtr;
    if (pad != 0) {
      if ((pad & 0x4000000) && (pad & 0x800000)) { // Left + L1
        setRandomizerState(!isRandomizerEnabled());
      }
    }
  }
}

#endif