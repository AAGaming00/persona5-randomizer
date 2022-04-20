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
#include "modules/p5/randomizer/randomizer.h"
#include "dc.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

#define FUNC_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( functest ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

#define FILE_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( enableFileAccessLogging ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, setBlackMaskCueID, CueIDThingy* param_1, u32 param_2, u16 skill_ID );
SHK_HOOK( void, IsEncounterEventSoundBankExist, EncounterFuncStruct* a1 );
SHK_HOOK( void*, BlackMaskBossVoice, structA* a1 );
SHK_HOOK( void*, LoadPlayerCombatVoicePack, structA* a1 );
SHK_HOOK( void*, LoadGenericEnemyVoicePack, structA* a1 );
SHK_HOOK( void*, LoadPartyPanelSPD, char* a1 );
SHK_HOOK( void*, LoadPartyPanelPLG, char* a1 , u32 a2, u32 a3, char a4);
SHK_HOOK( void*, LoadBCDFunction, char* a1 , u32 a2, u32 a3, int* a4);
SHK_HOOK( void, Load2DUIDDS, u32* a1 , char* a2);
SHK_HOOK( void*, LoadBEDFile, char* a1 , char a2);
SHK_HOOK( void*, LoadCombatCutin, char* a1 , char a2);
SHK_HOOK( int, LoadAnimationPack, u64 param_1, int animationID, char* result, int animationType );
SHK_HOOK( void, CombatPersonaCueID, CueIDThingy* param_1, int param_2, short param_3, char param_4, char param_5);
SHK_HOOK( void, FUN_00b2cee8, CueIDThingy* a1, int a2, int a3);
SHK_HOOK( void, JokerPersonaNameCueID, CueIDThingy* param_1, int param_2 );
SHK_HOOK( void, FUN_00b25348, CueIDThingy* param_1, int param_2 );
SHK_HOOK( void, FUN_00b24f84, CueIDThingy* param_1, int param_2 );
SHK_HOOK( void, FUN_00b25084, CueIDThingy* param_1, int param_2 );
SHK_HOOK( void, FUN_00b24938, CueIDThingy* a1, int a2, int a3 );
//SHK_HOOK( u64, BtlUnitGetUnitID, btlUnit_Unit* btlUnit );
SHK_HOOK( encounterIDTBL*, FUN_00263b94, int a1 );
SHK_HOOK( resrcNPCTblEntry*, FUN_00043fac, int a1 );
SHK_HOOK( int, SetEnemyAkechiPersona, u64 a1, u64 a2, EnemyPersonaFunctionStruct1* a3 );
SHK_HOOK( bool, EnemyHasCombatCutin, int a1, EnemyPersonaFunctionStruct1* a2 );
SHK_HOOK( bool, FUN_00745e28, u32 a1, int a2 );
SHK_HOOK( int, SetUpEncounterFlags, EncounterFuncStruct* a1, EncounterStructShort* a2);
// EXIST stuff
SHK_HOOK( int, FUN_0026b390, u16 a1 );
SHK_HOOK( int, FUN_0026b360, u16 a1 );
SHK_HOOK( int, FUN_0026b2b0, u16 a1 );
SHK_HOOK( int, FUN_0026b2e0, u16 a1 );
SHK_HOOK( int, FUN_0026b320, u16 a1 );
SHK_HOOK( int, FUN_0026aff0, u16 a1, u16 a2 );
SHK_HOOK( int, FUN_0026b094, u16 a1, u16 a2 );
SHK_HOOK( int, FUN_0026b148, u16 a1, u16 a2 );
SHK_HOOK( int, FUN_0026b1fc, u16 a1, u16 a2 );
// EXIST end
SHK_HOOK( int, FUN_00262258, u16 a1 );
SHK_HOOK( int, ParseUNITTBL, u64 a1 );
SHK_HOOK( int, FUN_00af3cb0, int a1 );
SHK_HOOK( int, FUN_0003a6e4, int a1 );
SHK_HOOK( int, FUN_0003a70c, int a1 );
SHK_HOOK( int, FUN_00829ce8, ActiveCombatUnitStruct* a1 );
SHK_HOOK( u64*, ReturnAddressOfUNITTBL_EnemyStats, s64 a1 );
SHK_HOOK( u16, ReturnAddressOfUNITTBL_EnemyAffinities, u32 a1, u16 a2 );
SHK_HOOK( u16, ReturnAddressOfUNITTBL_PersonaAffinities, u32 a1, u16 a2 );
SHK_HOOK( u64*, ReturnAddressOfUNITTBL_Segment3, s64 a1 );
SHK_HOOK( u64*, ReturnAddressOfELSAITBL_Segment1, u32 a1 );
SHK_HOOK( u64, CalculateShdPersonaEnemyEntry, shdHelper* a1, u32 a2 );
SHK_HOOK( void, FUN_003735d8, fechance* a1, u64 a2, u64 a3, u64 a4, u64 a5 );
SHK_HOOK( bool, FUN_00338a04, void );
SHK_HOOK( bool, FUN_007ed620, structA_2* a1 );
SHK_HOOK( int, FUN_00256830, btlUnit_Unit* a1, u32 skillID );
SHK_HOOK( u32, FUN_007af1c0, u32 a1, u32 a2 );
SHK_HOOK( u32, FUN_00784d18, u32 a1, u32 a2 );
SHK_HOOK( int, FUN_000435c0, int a1, int a2 );
SHK_HOOK( int, FUN_00060b90, void );
SHK_HOOK( bool, FUN_00786a88, int a1, int a2 );
//SHK_HOOK( int, FUN_0054fcb4, void );
SHK_HOOK( int, FUN_00927d50, char* a1 );
SHK_HOOK( int, FUN_00b0f558, CueIDThingy* a1, double a2, double a3 );
SHK_HOOK( int, FUN_00b0f5e8, CueIDThingy* a1, double a2, double a3 );
SHK_HOOK( int, FUN_00aff500, CueIDThingy* a1, double a2, double a3 );
SHK_HOOK( int, FUN_00aff590, CueIDThingy* a1, double a2, double a3 );
//SHK_HOOK( int, FUN_00b03510, CueIDThingy* a1, double a2, double a3 );
//SHK_HOOK( int, FUN_00b0efa8, CueIDThingy* a1, double a2, double a3 );
//SHK_HOOK( int, FUN_00b03248, CueIDThingy* a1, double a2, double a3 );
//SHK_HOOK( u64, BattleAnimations, CueIDThingy* a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6, u64 a7, u64 a8, u64 a9, u64 a10, u64 a11 );
SHK_HOOK( int, FUN_006a75c4, u64 a1 );
SHK_HOOK( bool, FUN_00b25088, void );
SHK_HOOK( int, FUN_00650700, int a1, int a2 );

static s32 pattern[] = { 0x00, 0xBA, 0x69, 0x98, -1, -1, -1, -1, -1, -1, -1, -1, 0x00, 0xBA, 0x23, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
/*static u64 BtlUnitGetUnitIDHook( btlUnit_Unit* btlUnit  )
{
  u64 unitType = btlUnit->unitType;
  u64 unitID = btlUnit->unitID;
  if ( unitType != 1 )
  {
    //printf( "BtlUnitGetUnitID called; - unit type %d - unit ID %d\n", unitType, unitID );
    lastAccessedUnitType = unitType;
    if ( unitType == 2 )
    {
      GlobalEnemyID = unitID;
    }
    else
    {
      GlobalEnemyID = 0;
    }
  }
  return unitID;
}*/

static void* BlackMaskBossVoiceHook( structA* a1 )
{
  FUNC_LOG("Loading BlackMaskBossVoiceHook\n");
  idkman* someAddressPointer;
  u64 uVar1;
  undefined8 in_r7;
  undefined8 in_r8;
  u32 uVar2;
  idkman* ppuVar3;
  char acStack2080 [128];
  char acStack1056 [128];

  someAddressPointer = MallocAndReturn(0x20);
  if (someAddressPointer != (void*)0x0) 
  {
    SomethingAboutLoadingVoices(someAddressPointer);
    someAddressPointer->ptr1 = 0x00ba90e4;
    ppuVar3 = someAddressPointer;
  }
  CalculateAddressWithPointer( &ppuVar3->ptr2,a1);
  uVar1 = a1->field0C->btlUnitPointer->unitID;
  uVar2 = uVar1;
  uVar1 = GetSavedataBitflagAlt(0x218f);
  if (uVar1 == 0) 
  {
    sprintf(acStack2080,"sound/battle/be_boss_%04d.acb",uVar2);
    sprintf(acStack1056,"sound/battle/be_boss_%04d.awb",uVar2);
  }
  else 
  {
    sprintf(acStack2080,"sound_JP/battle/be_boss_%04d.acb",uVar2);
    sprintf(acStack1056,"sound_JP/battle/be_boss_%04d.awb",uVar2);
  }
  FUN_0074ae50(ppuVar3, acStack2080, acStack1056, 0x100 + a1->field0C->btlUnitPointer->unitID );
    
  return ppuVar3;
}

static void* LoadPlayerCombatVoicePackHook( structA* a1 )
{
  FUNC_LOG("Loading LoadPlayerCombatVoicePackHook\n");
  idkman* someAddressPointer;
  u64 uVar1;
  u32 playerID;
  idkman* pmVar2;
  char acStack2080 [128];
  char acStack1056 [128];

  someAddressPointer = MallocAndReturn(0x20);
  pmVar2 = (void *)0x0;
  if (someAddressPointer != (void *)0x0) 
  {
    SomethingAboutLoadingVoices( someAddressPointer );
    someAddressPointer->ptr1 = 0xba7d30;
    pmVar2 = someAddressPointer;
  }
  CalculateAddressWithPointer( &someAddressPointer->ptr2, a1 );
  uVar1 = a1->field0C->btlUnitPointer->unitID;
  playerID = uVar1;
  uVar1 = GetSavedataBitflagAlt(0x218f);

  DEBUG_LOG("Player Voicepack being loaded\nPlayer ID -> %02d\nPersona ID -> %02d\n", playerID, GetEquippedPersonaFunction(playerID));
  if ( playerID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood && CONFIG_ENABLED( enableAkechiMod ) )
  {
    if ((int)uVar1 == 0) 
    {
      FUN_0074ae50(someAddressPointer, "sound/battle/bp09_01.acb", "sound/battle/bp09_01.awb", playerID);
    }
    else 
    {
      FUN_0074ae50(someAddressPointer, "sound_JP/battle/bp09_01.acb", "sound_JP/battle/bp09_01.awb", playerID);
    }
  }
  else
  {
    if ((int)uVar1 == 0) 
    {
      sprintf(acStack2080,"sound/battle/bp%02d.acb",playerID);
      sprintf(acStack1056,"sound/battle/bp%02d.awb",playerID);
    }
    else 
    {
      sprintf(acStack2080,"sound_JP/battle/bp%02d.acb",playerID);
      sprintf(acStack1056,"sound_JP/battle/bp%02d.awb",playerID);
    }
    FUN_0074ae50(someAddressPointer, acStack2080, acStack1056, playerID);
  }
  return pmVar2;
}

static void* LoadGenericEnemyVoicePackHook( structA* a1 )
{
  FUNC_LOG("Loading LoadGenericEnemyVoicePackHook\n");
  u32 unitID = a1->field0C->btlUnitPointer->unitID;
  u32 voiceID = NewSegment3TBL.unit[unitID].VoiceID + 1;
  u8 a_b_c = NewSegment3TBL.unit[unitID].VoicePackABC;
  idkman* pmVar2;
  char ACBPath [128];
  char AWBPath [128];
  
  if ( unitID == 190 || GetUnitStatsBlockTBLEntry(unitID)->Flags & 0x800 ) // Reaper or Treasure Demon flag
  {
    pmVar2 = MallocAndReturn( 0x20 );
    if ( pmVar2 != (idkman *)0x0 ) 
    {
      SomethingAboutLoadingVoices( pmVar2 );
      pmVar2->ptr1 = 0x00ba7a28;
    }
  }
  else 
  {
    pmVar2 = MallocAndReturn( 0x20 );
    if ( pmVar2 != (idkman *)0x0 ) 
    {
      SomethingAboutLoadingVoices( pmVar2 );
      pmVar2->ptr1 = 0x00ba7eb4;
    }
    CalculateAddressWithPointer( &pmVar2->ptr2, a1 );
    if ( !GetSavedataBitflagAlt( 0x218f ) ) 
    {
      if ( a_b_c == 0 ) 
      {
        sprintf( ACBPath, "sound/battle/be_%02d_%s.acb", voiceID, "a");
        sprintf( AWBPath, "sound/battle/be_%02d_%s.awb", voiceID, "a");
      }
      else if ( a_b_c == 1 ) 
      {
        sprintf( ACBPath, "sound/battle/be_%02d_%s.acb", voiceID, "b");
        sprintf( AWBPath, "sound/battle/be_%02d_%s.awb", voiceID, "b");
      }
      else if ( a_b_c == 2 ) 
      {
        sprintf( ACBPath, "sound/battle/be_%02d_%s.acb", voiceID, "c");
        sprintf( AWBPath, "sound/battle/be_%02d_%s.awb", voiceID, "c");
      }
      else if ( a_b_c == 0xFE ) 
      {
        DEBUG_LOG("Loading Player Voice %d for Enemy %d\n", voiceID, unitID);

        if ( voiceID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood && CONFIG_ENABLED( enableAkechiMod ) )
        {
          sprintf( ACBPath, "sound/battle/bp%02d_01.acb", voiceID );
          sprintf( AWBPath, "sound/battle/bp%02d_01.awb", voiceID );
        }
        else
        {
          sprintf( ACBPath, "sound/battle/bp%02d.acb", voiceID );
          sprintf( AWBPath, "sound/battle/bp%02d.awb", voiceID );
        }

        if ( voiceID == 1 )
        {
          pmVar2->ptr1 = 0xba7bac;
        }
        else pmVar2->ptr1 = 0xba7d30;
      }
      else
      {
        //printf("Loading Boss Voice for Enemy %d\n", unitID);
        sprintf( ACBPath, "sound/battle/be_boss_%04d.acb", unitID );
        sprintf( AWBPath, "sound/battle/be_boss_%04d.awb", unitID );
        pmVar2->ptr1 = 0x00ba90e4;
      }
    }
    else 
    {
      if ( a_b_c == 0 ) 
      {
        sprintf( ACBPath, "sound_jp/battle/be_%02d_%s.acb", voiceID, "a");
        sprintf( AWBPath, "sound_jp/battle/be_%02d_%s.awb", voiceID, "a");
      }
      else if ( a_b_c == 1 ) 
      {
        sprintf( ACBPath, "sound_jp/battle/be_%02d_%s.acb", voiceID, "b");
        sprintf( AWBPath, "sound_jp/battle/be_%02d_%s.awb", voiceID, "b");
      }
      else if ( a_b_c == 2 ) 
      {
        sprintf( ACBPath, "sound_jp/battle/be_%02d_%s.acb", voiceID, "c");
        sprintf( AWBPath, "sound_jp/battle/be_%02d_%s.awb", voiceID, "c");
      }
      else if ( a_b_c == 0xFE ) 
      {
        DEBUG_LOG("Loading Player Voice %d for Enemy %d\n", voiceID, unitID);

        if ( voiceID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood && CONFIG_ENABLED( enableAkechiMod ) )
        {
          sprintf( ACBPath, "sound_jp/battle/bp%02d_01.acb", voiceID );
          sprintf( AWBPath, "sound_jp/battle/bp%02d_01.awb", voiceID );
        }
        else
        {
          sprintf( ACBPath, "sound_jp/battle/bp%02d.acb", voiceID );
          sprintf( AWBPath, "sound_jp/battle/bp%02d.awb", voiceID );
        }

        if ( voiceID == 1 )
        {
          pmVar2->ptr1 = 0xba7bac;
        }
        else pmVar2->ptr1 = 0xba7d30;
      }
      else
      {
        //printf("Loading Boss Voice for Enemy %d\n", unitID);
        sprintf( ACBPath, "sound_jp/battle/be_boss_%04d.acb", unitID );
        sprintf( AWBPath, "sound_jp/battle/be_boss_%04d.awb", unitID );
        pmVar2->ptr1 = 0x00ba90e4;
      }
    }
    FUN_0074ae50( pmVar2, ACBPath, AWBPath, 0x100 + unitID );
  }
  return pmVar2;
}

static void* LoadPartyPanelSPDHook(char* a1)
{
  FUNC_LOG("Loading LoadPartyPanelSPDHook\n");
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/gui/p5_battle_partypanel.spd" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/gui/p5r_battle_partypanel.spd";
  }
  return SHK_CALL_HOOK( LoadPartyPanelSPD, a1 );
}

static void* LoadBEDFileHook(char* a1, char a2)
{
  FUNC_LOG("Loading LoadBEDFileHook\n");
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/effect/bes_gun9.BED" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/effect/bes_gun9_01.BED";
  }
  else if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/skill/0416.BED" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/skill/0416b.BED";
  }
  else if ( SummonCustomBED > 0 && strcmp( a1, "battle/skill/0406.BED" ) == 0 )
  {
    char newBED[32];
    sprintf( newBED, "battle/skill/0406_%d.BED", SummonCustomBED);
    a1 = newBED;
  }
  return SHK_CALL_HOOK( LoadBEDFile, a1, a2 );
}

static void* LoadCombatCutinHook(char* a1 , char a2)
{
  FUNC_LOG("Loading LoadCombatCutinHook\n");
  // Combat Cutin
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/cutin/bct_p_bc0009.dds" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/cutin/bct_p_bc0009_01.dds";
  }
  // AoA Start/Jump image
  if ( CONFIG_ENABLED( enableAkechiMod ) && strcmp( a1, "battle/gui/bksk_p_bc0009.dds" ) == 0  && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/gui/bksk_p_bc0009_01.dds";
  }
  return SHK_CALL_HOOK( LoadCombatCutin, a1, a2 );
}

static void CombatPersonaCueIDHook( CueIDThingy* param_1, int param_2, short param_3, char param_4, char param_5 )
{
  FUNC_LOG("Loading CombatPersonaCueIDHook\n");
  if ( param_1->Field10->btlUnitPointer->unitType == 2 && ActualGetCount( 420 ) > 0 ) // enemy is executing function
  {
    LoadSoundByCueIDCombatVoice( param_1, param_2, ActualGetCount( 420 ), 0 );
    ActualSetCount( 420, 0 );
    return;
  }
  
  u32 uVar1;
  u64 uVar2;
  u64 uVar3;
  u16* puVar5;
  sample_int_struct* puVar4;
  
  uVar1 = (int)param_3 & 0xffff;
  u16 personaID = GetEquippedPersonaFunction(param_1->Field10->btlUnitPointer->unitID);
  DEBUG_LOG("CombatPersonaCueIDHook called, character -> %d, persona -> %d, skill -> %d\n", param_1->Field10->btlUnitPointer->unitID, personaID, param_3);
  if ( personaID == 200 || (personaID & 0x1000) != 0 )
  {
    uVar3 = 0;
  }
  else uVar3 = 1;

  if (((int)uVar3 == 0) || (param_4 != 0)) 
  {
    uVar3 = FUN_002584cc(uVar1,1);
    if (((int)uVar3 == 0) && (uVar3 = FUN_0025867c(uVar1), (int)uVar3 == 0)) 
    {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 0x65, 0);
    }
    else 
    {
      if ( personaID >= 201 && personaID <= 210 ) // tier 1 personas
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0x4f, 0);
      }
      else if ( personaID >= 211 && personaID <= 220 || personaID == 240 ) // tier 2, 240 -> Vanadis 
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0x50, 0);
      }
      else if ( CONFIG_ENABLED( enableAkechiMod ) && personaID >= 241 && personaID <= 250 ) // tier 3 personas
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0x4D, 0);
      }
      else
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0x20, 0); // no valid Persona found, use generic attack line
        DEBUG_LOG("Failed to find valid Party Member persona ID. Result %d\n", personaID);
      }
    }
  }
  else 
  {
    uVar3 = FUN_002584cc(uVar1,1);
    if (((int)uVar3 == 0) && (uVar3 = FUN_0025867c(uVar1), (int)uVar3 == 0)) 
    {
      if ( CONFIG_ENABLED( enableAkechiMod ) && ( param_3 == 360 || param_3 == 361 || param_3 == 1511 || param_3 == 1505 ) && !GetSavedataBitflagAlt( 0x218f ) ) // Charge and Concentrate type skills
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 83, 0);
      }
      else LoadSoundByCueIDCombatVoice(param_1, param_2, 0x65, 0);
    }
    else 
    {
      uVar3 = 0x20;
      if (param_5 != 0) 
      {
        uVar3 = 0x68;
      }
      LoadSoundByCueIDCombatVoice(param_1, param_2, uVar3, 0);
    }
  }
  return;
}

static void EnemyAkechiSkillCueID( CueIDThingy* a1, int a2, int a3 )
{
  FUNC_LOG("Loading EnemyAkechiSkillCueID\n");
  if ( a1->Field10->btlUnitPointer->unitType == 2 && ActualGetCount( 420 ) > 0 ) // enemy is executing function
  {
    LoadSoundByCueIDCombatVoice( a1, a2, ActualGetCount( 420 ), 0 );
    ActualSetCount( 420, 0 );
  }
  else SHK_CALL_HOOK( FUN_00b2cee8, a1, a2, a3 );

  return;
}

static void JokerPersonaNameCueIDHook( CueIDThingy* param_1, int param_2 )
{
  FUNC_LOG("Loading JokerPersonaNameCueIDHook\n");
  if ( GetEquippedPersonaFunction( 1 ) >= 309 && GetEquippedPersonaFunction( 1 ) != 463 )
  {
    LoadSoundByCueIDCombatVoice( param_1, param_2, 213, 0 );
  }
  else return SHK_CALL_HOOK( JokerPersonaNameCueID, param_1, param_2 );
}

static void Load2DUIDDSHook(u32* a1, char* a2)
{
  FUNC_LOG("Loading Load2DUIDDSHook\n");
  if ( strcmp( a2, "camp/wanted/wanted_09.dds") == 0 && CONFIG_ENABLED( enableAkechiMod ) && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a2 = "camp/wanted/wanted_09_01.dds";
  }
  else if ( strcmp( a2, "camp/charaTex/c_chara_09.dds") == 0 && CONFIG_ENABLED( enableAkechiMod ) && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a2 = "camp/charaTex/c_chara_09b.dds";
  }
  else if ( strcmp( a2, "camp/charaTex/c_chara_22.dds") == 0 && (GetBitflagState( 3750 ) == 1 || GetBitflagState( 3748 ) == 1 || GetBitflagState( 3746 ) == 1 || GetBitflagState( 3752 ) == 1 ))
  {
	  a2 = "camp/charaTex/c_chara_22b.dds";
  }
  SHK_CALL_HOOK( Load2DUIDDS, a1, a2 );
}

static void* LoadBCDFunctionHook( char* a1 , u32 a2, u32 a3, int* a4 )
{
  FUNC_LOG("Loading LoadBCDFunctionHook\n");
  if ( strcmp( a1, "battle/event/BCD/goodbye/bksk_aketi.BCD" ) == 0 && CONFIG_ENABLED( enableAkechiMod ) && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
    a1 = "battle/event/BCD/goodbye/bksk_aketi_b.BCD";
  }
  else if ( strcmp( a1, "battle/event/BCD/030d/000/bes_030d_000.BCD" ) == 0 && CONFIG_ENABLED( enablePersonaEnemies ) )
  {
    char introBCD[128];
    sprintf(introBCD, "battle/event/BCD/%04x/000/bes_%04x_000.BCD", EncounterIDGlobal, EncounterIDGlobal);
    return SHK_CALL_HOOK( LoadBCDFunction, introBCD, a2, a3, a4 );
  }
  else if ( strcmp( a1, "battle/event/BCD/batontouch/bes_touch_v.BCD" ) == 0 && EncounterIDGlobal >= 830 && CONFIG_ENABLED( enablePersonaEnemies ) )
  {
    a1 = "battle/event/BCD/batontouch/bes_touch1.BCD";
  }
  return SHK_CALL_HOOK( LoadBCDFunction, a1, a2, a3, a4 );
}

static void* LoadPartyPanelPLGHook(char* a1, u32 a2, u32 a3, char a4)
{
  FUNC_LOG("Loading LoadPartyPanelPLGHook\n");
  if ( strcmp( a1, "battle/gui/battle_active_pp.plg" ) == 0 && CONFIG_ENABLED( enableAkechiMod ) && GetEquippedPersonaFunction(9) != Persona_RobinHood )
  {
      a1 = "battle/gui/battle_active_p5.plg";
  }
  return SHK_CALL_HOOK( LoadPartyPanelPLG, a1, a2, a3, a4 );
}

static void setBlackMaskCueIDHook( CueIDThingy* param_1, u32 param_2, u16 skill_ID )
{
  FUNC_LOG("Loading setBlackMaskCueIDHook\n");
  u16 uVar1;
  u64 uVar2;

  u32 Custom = ActualGetCount(420);

  if ( Custom > 0 )
  {
    LoadSoundByCueIDCombatVoice(param_1, param_2, Custom, 0);
    ActualSetCount( 420, 0 );
    return;
  }
  
  if (skill_ID < 0x48) 
  {
    if (skill_ID < 0x18) 
    {
      if (skill_ID < 0xe) 
      {
        if (skill_ID < 10) 
        {
          return;
        }
      }
      else 
      {
        if (0x13 < skill_ID) 
        {
            LoadSoundByCueIDCombatVoice(param_1, param_2, 2, 0);
            return;
        }
        if (0xf < skill_ID) 
        {
          return;
        }
      }
      LoadSoundByCueIDCombatVoice(param_1, param_2, 3, 0);
      return;
    }
    if (skill_ID < 0x40) 
    {
      if (0x3b < skill_ID) 
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 7, 0);
        return;
      }
      if (0x19 < skill_ID) 
      {
        return;
      }
    LoadSoundByCueIDCombatVoice(param_1, param_2, 2, 0);
      return;
    }
    if (skill_ID < 0x43) 
    {
      if (skill_ID < 0x42) 
      {
        return;
      }
      LoadSoundByCueIDCombatVoice(param_1, param_2, 7, 0);
      return;
    }
    if (skill_ID < 0x46) 
    {
      return;
    }
  }
  else 
  {
    if (0x10f < skill_ID) 
    {
      if (skill_ID == 0x2a2) 
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 10, 0);
        return;
      }
      if (skill_ID == 0x2a1) 
      {
        LoadSoundByCueIDCombatVoice(param_1, param_2, 0xb, 0);
        return;
      }
      if (skill_ID == 0x173) 
      {
        uVar2 = 0xb;
        uVar1 = param_1->Field1C;
        param_1->Field1C = uVar1 + 1;
        if (uVar1 & 1)
        {
          uVar2 = 10;
        }
        LoadSoundByCueIDCombatVoice(param_1, param_2, uVar2, 0);
        return;
      }
      if (skill_ID != 0x172) 
      {
        if (skill_ID == 0x15c) 
        {
          LoadSoundByCueIDCombatVoice(param_1, param_2, 8, 0);
          return;
        }
        if (skill_ID != 0x110) 
        {
          return;
        }
        LoadSoundByCueIDCombatVoice(param_1, param_2, 6, 0);
        return;
      }
      uVar2 = 0xb;
      uVar1 = param_1->Field1C;
      param_1->Field1C = uVar1 + 1;
      if (uVar1 & 1)
      {
        uVar2 = 10;
      }
      LoadSoundByCueIDCombatVoice(param_1, param_2, uVar2, 0);
      return;
    }
    if (skill_ID == 0x106) 
    {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 6, 0);
      return;
    }
    if (skill_ID == 0xf3) 
    {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 5, 0);
      return;
    }
    if (skill_ID == 0xd4) 
    {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 4, 0);
      return;
    }
    if (skill_ID == 0xca) 
    {
      LoadSoundByCueIDCombatVoice(param_1, param_2, 4, 0);
      return;
    }
    if (skill_ID != 0x48) 
    {
      return;
    }
  }
  LoadSoundByCueIDCombatVoice(param_1, param_2, 9, 0);
  return;
}

static int LoadAnimationPackHook( u64 param_1, int animationID, char* result, int animationType )
{
  FUNC_LOG("Loading LoadAnimationPackHook\n");
  u64 animType = (u32)(param_1 >> 0x3a);
  u64 unitID = (u32)(param_1 >> 0x14) & 0xffff;
  if ( animType == 2 && animationID == 51 && CONFIG_ENABLED( enableAkechiMod ) )
  {
    if( unitID == 6 && GetEquippedPersonaFunction(6) != Persona_Anat )
    {
      animationID = 52;
    }
    else if( unitID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood )
    {
      animationID = 52;
    }
  }
  return SHK_CALL_HOOK( LoadAnimationPack, param_1, animationID, result, animationType);
}

static int ParseUNITTBLHook( u64 param_1 )
{
  FUNC_LOG("Loading ParseUNITTBLHook\n");
  memset( &NewSegment2TBL, 0x0, sizeof( NewSegment2TBL ) );
  memset( &NewEnemyStatsTBL, 0x69, sizeof( NewEnemyStatsTBL ) );
  memset( &NewEnemyAffinityTBL, 0x69, sizeof( NewEnemyAffinityTBL ) );
  memset( &NewPersonaAffinityTBL, 0x69, sizeof( NewPersonaAffinityTBL ) );
  memset( &NewSegment3TBL, 0x69, sizeof( NewSegment3TBL ) );
  memset( &NewVisualIndexTBL, 0x69, sizeof( NewVisualIndexTBL ) );
  
  u16 uVar1;
  u32 uVar2;
  u64 uVar3;
  undefined8 uVar4;
  u16 *puVar5;
  undefined *puVar6;
  int iVar7;
  u64 uVar8;
  s64 lVar9;
  u16 *puVar10;
  u16 *puVar11;
  u16 *puVar12;
  undefined *puVar13;
  u16 *puVar14;
  u16 *puVar15;
  u16 *puVar16;
  u8 bVar17;
  int iVar18;
  s64 lVar19;
  u32 local_40 [2];
  
  FUN_0090053c((u32)local_40,param_1,4);
  iVar18 = 0;
  uVar4 = __builtin_clz(0);
  bVar17 = (u8)((u32)uVar4 >> 5) & 1 ^ 1;
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  uVar3 = param_1 + 4 & 0xffffffff;
  puVar16 = &NewEnemyStatsTBL;
  FUN_0090053c((u32)&NewEnemyStatsTBL,uVar3,(u64)local_40[0]);
  uVar8 = (u64)local_40[0] + 4;
  iVar7 = (int)uVar8;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar8 & 0xf) != 0)) * -0x10
                           );
  lVar9 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar8;
  lVar19 = 0;
  if (0 < (int)lVar9) {
    lVar19 = lVar9;
  }
  uVar3 = uVar3 + local_40[0] + lVar19 & 0xffffffff;
  FUN_0090053c((u32)local_40,uVar3,4);
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  uVar3 = uVar3 + 4 & 0xffffffff;
  puVar15 = &NewEnemyAffinityTBL;
  FUN_0090053c((u32)&NewEnemyAffinityTBL,uVar3,(u64)local_40[0]);
  uVar8 = (u64)local_40[0] + 4;
  iVar7 = (int)uVar8;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar8 & 0xf) != 0)) * -0x10
                           );
  lVar9 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar8;
  lVar19 = 0;
  if (0 < (int)lVar9) {
    lVar19 = lVar9;
  }
  uVar3 = uVar3 + local_40[0] + lVar19 & 0xffffffff;
  FUN_0090053c((u32)local_40,uVar3,4);
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  uVar3 = uVar3 + 4 & 0xffffffff;
  puVar14 = &NewPersonaAffinityTBL;
  FUN_0090053c((u32)&NewPersonaAffinityTBL,uVar3,(u64)local_40[0]);
  uVar8 = (u64)local_40[0] + 4;
  iVar7 = (int)uVar8;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar8 & 0xf) != 0)) * -0x10
                           );
  lVar9 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar8;
  lVar19 = 0;
  if (0 < (int)lVar9) {
    lVar19 = lVar9;
  }
  uVar3 = uVar3 + local_40[0] + lVar19 & 0xffffffff;
  FUN_0090053c((u32)local_40,uVar3,4);
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  uVar3 = uVar3 + 4 & 0xffffffff;
  puVar13 = &NewSegment3TBL;
  FUN_0090053c((u32)&NewSegment3TBL,uVar3,(u64)local_40[0]);
  uVar8 = (u64)local_40[0] + 4;
  iVar7 = (int)uVar8;
  uVar4 = __builtin_clz(iVar7 + ((iVar7 >> 4) + (u32)(iVar7 < 0 && (uVar8 & 0xf) != 0)) * -0x10
                           );
  lVar9 = ((u64)(u32)(iVar7 >> 4) + ((u64)((u32)uVar4 >> 5 & 1) ^ 1)) * 0x10 - uVar8;
  lVar19 = 0;
  if (0 < (int)lVar9) {
    lVar19 = lVar9;
  }
  uVar3 = uVar3 + local_40[0] + lVar19 & 0xffffffff;
  FUN_0090053c((u32)local_40,uVar3,4);
  if (bVar17 != 0) {
    local_40[0] = local_40[0] << 0x18 |
                  (local_40[0] & 0xff00) << 8 | local_40[0] >> 0x18 | local_40[0] >> 8 & 0xff00;
  }
  puVar5 = (u16 *)FUN_0090053c((u32)0x00d79a2c,uVar3 + 4 & 0xffffffff,(u64)local_40[0]);
  if (bVar17 != 0) {
    iVar7 = 0;
    do {
      uVar2 = *(u32 *)(puVar16 + 4);
      *puVar16 = *puVar16 >> 8 | *puVar16 << 8;
      uVar1 = puVar16[2];
      *(u32 *)(puVar16 + 4) =
           uVar2 << 0x18 | (uVar2 & 0xff00) << 8 | uVar2 >> 0x18 | uVar2 >> 8 & 0xff00;
      uVar2 = *(u32 *)(puVar16 + 6);
      puVar16[2] = uVar1 >> 8 | uVar1 << 8;
      lVar19 = 8;
      *(u32 *)(puVar16 + 6) =
           uVar2 << 0x18 | (uVar2 & 0xff00) << 8 | uVar2 >> 0x18 | uVar2 >> 8 & 0xff00;
      puVar5 = puVar16;
      do {
        puVar10 = puVar5 + 0xb;
        puVar5 = puVar5 + 1;
        uVar1 = *puVar10;
        *puVar10 = uVar1 >> 8 | uVar1 << 8;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      uVar1 = puVar16[0x13];
      lVar19 = 4;
      puVar16[0x13] = uVar1 >> 8 | uVar1 << 8;
      uVar1 = puVar16[0x14];
      puVar16[0x14] = uVar1 >> 8 | uVar1 << 8;
      puVar5 = puVar16;
      do {
        puVar10 = puVar5 + 0x15;
        puVar11 = puVar5 + 0x16;
        puVar5 = puVar5 + 2;
        uVar1 = *puVar10;
        *puVar10 = uVar1 >> 8 | uVar1 << 8;
        uVar1 = *puVar11;
        *puVar11 = uVar1 >> 8 | uVar1 << 8;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      puVar10 = puVar16 + 0x1d;
      puVar11 = puVar16 + 0x1e;
      puVar12 = puVar16 + 0x1f;
      puVar5 = puVar16 + 0x21;
      uVar1 = *puVar10;
      iVar7 = iVar7 + 1;
      puVar16 = puVar16 + 0x22;
      *puVar10 = uVar1 >> 8 | uVar1 << 8;
      uVar1 = *puVar11;
      *puVar11 = uVar1 >> 8 | uVar1 << 8;
      uVar1 = *puVar12;
      *puVar12 = uVar1 >> 8 | uVar1 << 8;
      uVar1 = *puVar5;
      *puVar5 = uVar1 >> 8 | uVar1 << 8;
    } while (iVar7 < 999);
    iVar7 = 0;
    do {
      lVar19 = 0x14;
      puVar16 = puVar15;
      do {
        *puVar16 = *puVar16 >> 8 | *puVar16 << 8;
        lVar19 = lVar19 + -1;
        puVar16 = puVar16 + 1;
      } while (lVar19 != 0);
      iVar7 = iVar7 + 1;
      puVar15 = puVar15 + 0x14;
    } while (iVar7 < 999);
    iVar7 = 0;
    do {
      lVar19 = 0x14;
      puVar16 = puVar14;
      do {
        *puVar16 = *puVar16 >> 8 | *puVar16 << 8;
        lVar19 = lVar19 + -1;
        puVar16 = puVar16 + 1;
      } while (lVar19 != 0);
      iVar7 = iVar7 + 1;
      puVar14 = puVar14 + 0x14;
    } while (iVar7 < 999);
    do {
      lVar19 = 4;
      uVar1 = *(u16 *)(puVar13 + 4);
      *(u16 *)(puVar13 + 4) = uVar1 >> 8 | uVar1 << 8;
      uVar1 = *(u16 *)(puVar13 + 6);
      *(u16 *)(puVar13 + 6) = uVar1 >> 8 | uVar1 << 8;
      puVar6 = puVar13;
      do {
        puVar16 = (u16 *)(puVar6 + 8);
        puVar6 = puVar6 + 2;
        uVar1 = *puVar16;
        *puVar16 = uVar1 >> 8 | uVar1 << 8;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      lVar19 = 4;
      puVar6 = puVar13;
      do {
        puVar16 = (u16 *)(puVar6 + 0x10);
        puVar6 = puVar6 + 2;
        uVar1 = *puVar16;
        *puVar16 = uVar1 >> 8 | uVar1 << 8;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      iVar18 = iVar18 + 1;
      puVar13 = puVar13 + 0x18;
    } while (iVar18 < 999);
    lVar19 = 999;
    puVar16 = 0x00d79a2c;
    puVar15 = 0x00d79a2e;
    do {
      puVar5 = puVar15 + 2;
      *puVar16 = *puVar16 >> 8 | *puVar16 << 8;
      *puVar15 = *puVar15 >> 8 | *puVar15 << 8;
      lVar19 = lVar19 + -1;
      puVar16 = puVar16 + 2;
      puVar15 = puVar5;
    } while (lVar19 != 0);
  }
  return puVar5;
}

static u64* ReturnAddressOfUNITTBL_EnemyStatsHook( s64 a1 )
{
  return &NewEnemyStatsTBL.EnemyStats[a1];
}

static u16 ReturnAddressOfUNITTBL_EnemyAffinitiesHook( u32 a1, u16 a2 )
{
  return NewEnemyAffinityTBL.EnemyAffinities[a1].affinity[a2];
}

static u16 ReturnAddressOfUNITTBL_PersonaAffinitiesHook( u32 a1, u16 a2 )
{
  return NewPersonaAffinityTBL.PersonaAffinities[a1].affinity[a2];
}

static u64* ReturnAddressOfUNITTBL_Segment3Hook( s64 a1 )
{
  return &NewSegment3TBL.unit[a1];
}

static u64* ReturnAddressOfELSAITBL_Segment1Hook( u32 a1 )
{
  if ( a1 >= 350 )
  {
    NewSegment2TBL.entry[a1-350].ScriptID = a1;
    return &NewSegment2TBL.entry[a1-350];
  }
  else
  {
    u64* returnVal = SHK_CALL_HOOK( ReturnAddressOfELSAITBL_Segment1, a1);
    //DEBUG_LOG("ReturnAddressOfELSAITBL_Segment1Hook return val from %d (a1)\nExpected -> 0x%08x\n", a1, returnVal);
    return returnVal;
  }
}

static u64 CalculateShdPersonaEnemyEntryHook( shdHelper* param_1, u32 enemyID )
{
  u64 returnVal = SHK_CALL_HOOK( CalculateShdPersonaEnemyEntry, param_1, enemyID);
  if ( param_1->field418 != 0 && param_1->field41c != 0 )
  {
    if ( enemyID >= 350 )
    {
      returnVal = ( shdEnemyFile->pointerToFile + 0x10 ) + ( 0x4C * ( enemyID - 350 ) );
      DEBUG_LOG("shdEnemy Entry %03d\n", enemyID-350);
    }
    else DEBUG_LOG("shdPersonaEnemy Entry %03d\n", enemyID-141);
  }
  else DEBUG_LOG("shdPersona Entry %03d\n", enemyID);

  return returnVal;
}

static int isPersonaExist(u16 a1)
{
  return SHK_CALL_HOOK( FUN_0026b390, 201 );
}

static int isPersonaExist2(u16 a1)
{
  return SHK_CALL_HOOK( FUN_0026b360, 201 );
}

static int isEnemyExist(u16 a1) // motionse dat file
{
  /*int result;
  if ( a1 >= 350 )
  {
    result = SHK_CALL_HOOK( FUN_0026b2b0, 69 );
  }
  else if ( a1 >= 114 && a1 <= 120 ) // original enemies not in exist
  {
    result = SHK_CALL_HOOK( FUN_0026b2b0, 69 );
  }
  else if ( a1 >= 145 && a1 <= 169 && a1 != 157 ) // big range, exclude reaper
  {
    result = SHK_CALL_HOOK( FUN_0026b2b0, 69 );
  }
  else result = SHK_CALL_HOOK( FUN_0026b2b0, a1 );*/

  return SHK_CALL_HOOK( FUN_0026b2b0, 69 );
}

static int isEnemyExist2(u16 a1) // swordtrack pac file
{
  int result;
  if ( a1 >= 254 )
  {
    result = SHK_CALL_HOOK( FUN_0026b2e0, 69 );
  }
  else if ( a1 >= 114 && a1 <= 120 ) // original enemies not in exist
  {
    result = SHK_CALL_HOOK( FUN_0026b2e0, 69 );
  }
  else if ( a1 >= 145 && a1 <= 252 && a1 != 157 ) // big range, exclude reaper
  {
    result = SHK_CALL_HOOK( FUN_0026b2e0, 69 );
  }
  else result = SHK_CALL_HOOK( FUN_0026b2e0, a1 );

  return result;
}

static int isEnemyExist3( u16 a1, u16 a2 ) // motionse acb file
{
  /*int result;
  if ( a1 >= 350 )
  {
    result = SHK_CALL_HOOK( FUN_0026b320, 69 );
  }
  else if ( a1 >= 114 && a1 <= 120 ) // original enemies not in exist
  {
    result = SHK_CALL_HOOK( FUN_0026b320, 69 );
  }
  else if ( a1 >= 145 && a1 <= 169 && a1 != 157 ) // big range, exclude reaper
  {
    result = SHK_CALL_HOOK( FUN_0026b320, 69 );
  }
  else result = SHK_CALL_HOOK( FUN_0026b320, a1 );*/

  return SHK_CALL_HOOK( FUN_0026b320, 69 );
}

static int isPartyMemberExist( u16 a1, u16 a2 )
{
  int result;
  if ( a1 <= 9 )
  {
    result = SHK_CALL_HOOK( FUN_0026aff0, a1, a2 );
  }
  else result = SHK_CALL_HOOK( FUN_0026aff0, 2, a2 );

  return result;
}

static int isPartyMemberExist2( u16 a1, u16 a2 )
{
  int result;
  if ( a1 <= 9 )
  {
    result = SHK_CALL_HOOK( FUN_0026b094, a1, a2 );
  }
  else result = SHK_CALL_HOOK( FUN_0026b094, 2, a2 );

  return result;
}

static int isPartyMemberExist3( u16 a1, u16 a2 )
{
  int result;
  if ( a1 <= 9 )
  {
    result = SHK_CALL_HOOK( FUN_0026b148, a1, a2 );
  }
  else result = SHK_CALL_HOOK( FUN_0026b148, 2, a2 );

  return result;
}

static int isPartyMemberExist4( u16 a1, u16 a2 )
{
  /*int result;
  if ( a1 <= 9 )
  {
    result = SHK_CALL_HOOK( FUN_0026b1fc, a1, a2 );
  }
  else result = SHK_CALL_HOOK( FUN_0026b1fc, 2 , a2);

  printf("Party Member EXIST 4 called on ID %d; result %d\n");
  return result;*/
  return 0;
}

static int SetEnemyAkechiPersonaHook( u64 a1, u64 a2, EnemyPersonaFunctionStruct1* a3 )
{
  FUNC_LOG("Loading SetEnemyAkechiPersonaHook\n");
  //printf("Checking Persona ID for unit ID %d; unit type %d\n", a3->field0c->field18->field04->unitID, a3->field0c->field18->field04->unitType);
  // Field04 is pointer to battle struct of the battle unit currently acting
  if ( a3->field0c->field18->field04->unitType == 2) // only execute on enemy units
  {
    int PersonaEnemyID = a3->field0c->field18->field04->unitID;
    
    int result = SHK_CALL_HOOK( SetEnemyAkechiPersona, a1, a2, a3 ); //call original function and store result

    if ( EnemyPersona > 0 )
    {
      result = EnemyPersona;
      EnemyPersona = 0;
    }

    // black mask conditional to avoid using same Loki ID as player Akechi
    else if ( PersonaEnemyID == 232 && CONFIG_ENABLED( enableAkechiMod ) ) 
    {
      result = 239;
    }

    else if ( PersonaEnemyID == 217 && result == 210 ) 
    {
      result = 240;
    }

    /*else if ( PersonaEnemyID == 228 )
    {
      result = 1;
    }*/

    if ( result > 0 ) DEBUG_LOG("Enemy ID %d summoning Persona %d\n", PersonaEnemyID, result);

    a3->field0c->field18->field04->StockPersona[0].personaID = result;

    return result;
  }
  return SHK_CALL_HOOK( SetEnemyAkechiPersona, a1, a2, a3 ); // Player unit is executing this function, use original function
}

static bool EnemyHasCombatCutinHook( int a1, EnemyPersonaFunctionStruct1* a2 )
{
  FUNC_LOG("Loading EnemyHasCombatCutinHook\n");
  bool result = SHK_CALL_HOOK( EnemyHasCombatCutin, a1, a2 );
  if ( result == false )
  {
    if ( a2->field0c->field18->field04->StockPersona[0].personaID == 0 )
    {
      return false;
    }
    else return true;
  }
  else return result;
}

int SetUpEncounterFlagsHook( EncounterFuncStruct* a1, EncounterStructShort* a2)
{
  //FUNC_LOG("Loading SetUpEncounterFlagsHook\n");
  PrepareDLCBGM(); // Load DLC BGM
  PrepareDLCBGM(); // Load DLC BGM
  SetBitflagState( 8348, 0 ); // disable early end flag
  int encounterIDReal = a2->encounterIDLocal;
  EncounterIDGlobal = encounterIDReal;
  int result = SHK_CALL_HOOK( SetUpEncounterFlags, a1, a2 );

  int CurrentEncounterBGMID = GetEncounterEntryFromTBL(encounterIDReal)->BGMID;
  // Non destructive Unhardcoded BGM
  if ( CurrentEncounterBGMID > 20 )
  {
    a1->BGMID = CurrentEncounterBGMID;
  }
  if ( CONFIG_ENABLED( enableExpandedBGM ) )
  {
    // DLC BGM Stuff
    if ( a1->BGMID == 300 || a1->BGMID == -1 ) // Only Execute DLC Music code on Last Surprise
    {
      if ( isAmbush )
      {
        isAmbush = false;

        if (!CONFIG_ENABLED( royalAmbushBGM ))
        {
          int AmbushRNG = GetRandom( 2 );
          //printf("Ambush RNG -> %d\n", AmbushRNG);
          if ( AmbushRNG == 0 )
          {
            a1->BGMID = 907; // Take Over
          }
          else if ( AmbushRNG == 1 )
          {
            a1->BGMID = 971; // What you wish for
          }
          else
          {
            a1->BGMID = 972; // Axe to grind
          }
        }
        else
        {
          a1->BGMID = 907; // Take Over
        }
      }
      else if ( isAmbushed && CONFIG_ENABLED( ambushOverDLC ) )
      {
        a1->BGMID = 973; // Last Surprise (Strikers)
        isAmbushed = false;
      }
    }
    FILE_LOG("========================================\nStarting Encounter ID %d with BGM ID %d\n========================================\n", encounterIDReal, a1->BGMID);
  }

  if ( shdEnemyFile == 0x0 )
  {
    shdEnemyFile = open_file( "init/shdEnemy.pdd", 0 );
  }
  if ( CONFIG_ENABLED( enableRandomizerModule ) ) {
    SetUpEncounterFlagsRandomizerHook(a1, a2);
  }
  return result;
}

static encounterIDTBL* FUN_00263b94Hook( int a1 )
{
  FUNC_LOG("Loading FUN_00263b94Hook\n");
  encounterIDTBL* result;
  result = SHK_CALL_HOOK( FUN_00263b94, a1 );

  if ( LastUsedEncounterID != a1 && a1 < 1000) // prevent spam
  {
    DEBUG_LOG("Encounter Block %03d loaded\n", a1);
    hexDump("TBL Data", result, 24);
    LastUsedEncounterID = a1;
  }
  if ( CONFIG_ENABLED( enableRandomizerModule ) ) {
    GetEncounterEntryFromTBLHook(a1, result, _shk_prx_trampoline_FUN_00263b94_ptr);
  }

  return result;
}

static void IsEncounterEventSoundBankExistHook( EncounterFuncStruct* a1 )
{
  FUNC_LOG("Loading IsEncounterEventSoundBankExistHook\n");

  if ( EncounterIDGlobal >= 830 ) return LoadEncounterEventSoundbank( EncounterIDGlobal );
  else return SHK_CALL_HOOK( IsEncounterEventSoundBankExist, a1 );
}

static int CheckIsEncounterAmbush( int a1 )
{
  FUNC_LOG("Loading CheckIsEncounterAmbush\n");
  randomizedCombatOutfit = false;

  int result = SHK_CALL_HOOK( FUN_00af3cb0, a1 );
  /*if ( result == 2 || result == 1 )
  {
    isAmbush = true;
    isAmbushed = false;
  }
  else if ( result == 6 )
  {
    isAmbush = false;
    isAmbushed = true;
  }*/
  return result;
}

static void FUN_003735d8Hook( fechance* a1, u64 a2, u64 a3, u64 a4, u64 a5 )
{
  FUNC_LOG("Loading FUN_003735d8Hook\n");
  //printf("FUN_003735d8 a1->field182 -> %d\n", a1->field182 );
  randomizedCombatOutfit = false;
  
  if ( a1->field182 == 2 || a1->field182 == 1 )
  {
    isAmbush = true;
    isAmbushed = false;
  }
  else if ( a1->field182 == 6 )
  {
    isAmbush = false;
    isAmbushed = true;
  }
  return SHK_CALL_HOOK( FUN_003735d8, a1, a2, a3, a4, a5 );
}

static int FUN_00829ce8Hook( ActiveCombatUnitStruct* a1 )
{ 
  FUNC_LOG("Loading FUN_00829ce8Hook\n");
  int currentUnitID = a1->field30->field18->field04->unitID;
  //printf( "FUN_00829ce8 called; a1 -> 0x%x; UnitID -> %d\n", a1, currentUnitID );
  int accessoryID = a1->field30->field18->field04->accessoryID;
  accessoryID = accessoryID - 0x2000;
  //printf("Unit ID %d has accessory %d\n", currentUnitID, accessoryID);
  int skillID = GetAccessoryTBLEntry( accessoryID )->RESERVE;
  //printf("Skill ID %d obtained from accessory\n", skillID);

  int numOfSkills = 0;
  int result = SHK_CALL_HOOK( FUN_00829ce8, a1 );
  if ( skillID != 0 && accessoryID >= 255 && skillID < 2000 )
  {
    //printf("Unit holding accessory ID %d granting skill ID %d\n", accessoryID, skillID );
    for ( int i = 0; i <= 8; i++ )
    {
      if ( a1->SkillID[i] == 0 )
      {
        a1->SkillID[i] = skillID;
        numOfSkills = a1->amountOfSkills + 1;
        a1->amountOfSkills += 1;
        //printf( "Skill ID %d granted on Skill slot %d\n", skillID, i );
        break;
      }
    }
  }
  //printf("Previous Active Skill Slot -> %d\n", ActiveGlobalSkillSlot );
  //printf("Current Skill slot -> %d\n", a1->ActiveSkillID );
  //printf("Amount of Skills in List -> %d\n", a1->amountOfSkills);
  if ( ActiveGlobalSkillSlot == numOfSkills - 2 && a1->ActiveSkillID == 0 )
  {
    //printf("Skill slot looped from %d to %d while skipping new skill\n", ActiveGlobalSkillSlot, a1->ActiveSkillID );
    a1->ActiveSkillID = numOfSkills - 1;
    ActiveGlobalSkillSlot = numOfSkills - 1;
  }
  else if ( ActiveGlobalSkillSlot == 0 && a1->ActiveSkillID == numOfSkills - 2 )
  {
    //printf("Skill slot looped from %d to %d while skipping new skill\n", ActiveGlobalSkillSlot, a1->ActiveSkillID );
    a1->ActiveSkillID = numOfSkills - 1;
    ActiveGlobalSkillSlot = numOfSkills - 1;
  }
  else ActiveGlobalSkillSlot = a1->ActiveSkillID;
  //hexDump( "Active Unit Combat Struct", a1, 0x250 );

  return result;
}

static resrcNPCTblEntry* GetNPCTBLEntry( int a1 )
{
  FUNC_LOG("Loading GetNPCTBLEntry\n");
  //printf("NPC TBL NPC ID %d loaded\n", a1 );
  //printf("NPC TBL Entry %d loaded\n",  ( (int)SHK_CALL_HOOK( FUN_00043fac, a1 ) - (int)SHK_CALL_HOOK( FUN_00043fac, 0 ) ) / 0x1C );
  return SHK_CALL_HOOK( FUN_00043fac, a1 );
}

static int GetItemTBLMeleeWeaponField0E( u16 a1 )
{
  return 0;
}

bool CheckPartyMemberConfidantCombatAbilities(u32 playerID, int param_2)
{
  int iVar3;
  short sVar5;
  bool result = 0;
  btlUnit_Unit* pbVar4;
  
  if (param_2 < 11) 
  {
    if (param_2 < 5) 
    {
      if (1 < param_2) 
      {
        if (2 < param_2) 
        {
          if (param_2 < 4) 
          {
            return IsConfidantBonusObtained(173);
          }
          return IsConfidantBonusObtained(171);
        }
        if (playerID == 9) // Follow Up Attack
        {
          sVar5 = 86;
        }
        /*else if (playerID == 10) // Follow Up Attack
        {
          sVar5 = 12;
        }*/
        else 
        {
          if (playerID == 7) 
          {
            sVar5 = 32;
          }
          else 
          {
            if (playerID == 6) 
            {
              sVar5 = 22;
            }
            else 
            {
              if (playerID == 5) 
              {
                sVar5 = 42;
              }
              else 
              {
                if (playerID == 4) 
                {
                  sVar5 = 63;
                }
                else 
                {
                  if (playerID == 3) 
                  {
                    sVar5 = 12;
                  }
                  else 
                  {
                    if (playerID != 2) 
                    {
                      return 0;
                    }
                    sVar5 = 73;
                  }
                }
              }
            }
          }
        }
        return IsConfidantBonusObtained(sVar5);
      }
      if (-1 < param_2) 
      {
        if (0 < param_2) 
        {
          if ( CONFIG_ENABLED( AutoUnlockBatonPass ) )
          {
            return true;
          }
          if (playerID == 1) // Baton Pass
          {
            return 1;
          }
          if (playerID == 9) 
          {
            sVar5 = 88;
          }
          else if (playerID == 10) // Kasumi, use Morgana as placeholder
          {
            sVar5 = 13;
          }
          else 
          {
            if (playerID == 7) 
            {
              sVar5 = 35;
            }
            else 
            {
              if (playerID == 6) 
              {
                sVar5 = 25;
              }
              else 
              {
                if (playerID == 5) 
                {
                  sVar5 = 45;
                }
                else 
                {
                  if (playerID == 4) 
                  {
                    sVar5 = 65;
                  }
                  else 
                  {
                    if (playerID == 3) 
                    {
                      sVar5 = 13;
                    }
                    else 
                    {
                      if (playerID != 2) 
                      {
                        return 0;
                      }
                      sVar5 = 72;
                    }
                  }
                }
              }
            }
          }
          result = IsConfidantBonusObtained(sVar5);
          return result;
        }
        iVar3 = GetBitflagState(8202);
        if ((iVar3 == 0) && (iVar3 = GetBitflagState(8206), iVar3 == 0)) 
        {
          return 1;
        }
      }
    }
    else 
    {
      if (param_2 < 8) 
      {
        if (param_2 < 6) 
        {
          result = IsConfidantBonusObtained(180);
          return result;
        }
        if (param_2 < 7) 
        {
          result = IsConfidantBonusObtained(175);
          return result;
        }
        result = IsConfidantBonusObtained(179);
        return result;
      }
      if (param_2 < 9) 
      {
        result = IsConfidantBonusObtained(161);
        if ((int)result != 0) 
        {
          pbVar4 = GetBtlPlayerUnitFromID(1);
          FUN_0025b6ac(pbVar4);
          pbVar4 = GetBtlPlayerUnitFromID(1);
          GetNumberOfBullets(pbVar4);
          IsConfidantBonusObtained(166);
          return 1;
        }
      }
      else 
      {
        if (param_2 < 10) 
        {
          result = IsConfidantBonusObtained(30);
          return result;
        }
        result = IsConfidantBonusObtained(163);
        if ((int)result != 0) 
        {
          pbVar4 = GetBtlPlayerUnitFromID(1);
          result = GetNumberOfBullets(pbVar4);
          if (0 < (short)result) 
          {
            return 1;
          }
        }
      }
    }
  }
  else 
  {
    if (param_2 < 17) 
    {
      if (param_2 < 14) 
      {
        if (11 < param_2) 
        {
          if (param_2 < 13) 
          {
            result = CheckPartyMemberConfidantCombatAbilities(playerID,1);
            return result;
          }
          result = IsConfidantBonusObtained(78);
          return result;
        }
      }
      else 
      {
        if (param_2 < 15) 
        {
          if (playerID == 9) // Harisen Recovery
          {
            sVar5 = 85;
          }
          else if (playerID == 10)  // Kasumi, use Morgana as placeholder
          {
            sVar5 = 18;
          }
          else 
          {
            if (playerID == 7) 
            {
              sVar5 = 31;
            }
            else 
            {
              if (playerID == 6) 
              {
                sVar5 = 26;
              }
              else 
              {
                if (playerID == 5) 
                {
                  sVar5 = 46;
                }
                else 
                {
                  if (playerID == 4) 
                  {
                    sVar5 = 61;
                  }
                  else 
                  {
                    if (playerID == 3) 
                    {
                      sVar5 = 18;
                    }
                    else 
                    {
                      if (playerID != 2) 
                      {
                        return 0;
                      }
                      sVar5 = 74;
                    }
                  }
                }
              }
            }
          }
          result = IsConfidantBonusObtained(sVar5);
          return result;
        }
        if (15 < param_2) 
        {
          result = IsConfidantBonusObtained(68);
          return result;
        }
        if (playerID != 0) // Protect
        {
          switch ( playerID )
          {
            case 10:
              sVar5 = 19;
              break;
            case 9:
              sVar5 = 51;
              break;
            case 7:
              sVar5 = 39;
              break;
            case 6:
              sVar5 = 29;
              break;
            case 5:
              sVar5 = 49;
              break;
            case 4:
              sVar5 = 69;
              break;
            case 3:
              sVar5 = 19;
              break;
            case 2:
              sVar5 = 79;
              break;
            default:
              return false;
              break;
          }
          result = IsConfidantBonusObtained(sVar5);
          return result;
        }
      }
    }
    else 
    {
      if (param_2 < 20) 
      {
        if (param_2 < 18) 
        {
          result = IsConfidantBonusObtained(193);
          return result;
        }
        if (param_2 < 19) 
        {
          result = IsConfidantBonusObtained(195);
          return result;
        }
        result = IsConfidantBonusObtained(198);
        return result;
      }
      if (param_2 < 21) 
      {
        result = IsConfidantBonusObtained(200);
        return result;
      }
      if (param_2 < 22) 
      {
        result = IsConfidantBonusObtained(177);
        return result;
      }
    }
  }
  return SHK_CALL_HOOK( FUN_00745e28, playerID, param_2 );
}

static int CheckUnitIsEndure( btlUnit_Unit* btlUnit, u32 skillID )
{
  FUNC_LOG("Loading CheckUnitIsEndure\n");
  //printf("Checking Endure on unit ID %d; unit type %d\n", btlUnit->unitID, btlUnit->unitType);
  int result = SHK_CALL_HOOK( FUN_00256830, btlUnit, skillID );
  if ( btlUnit->unitType == 1 ) // emulate same exact check as original code for consistency
  {
    u32 unitID = btlUnit->unitID;
    u32 confidantEndureBonusID;
    if ( unitID == 9 && !hasAkechiEndured ) // Akechi
    {
      confidantEndureBonusID = 50;
      hasAkechiEndured = true;
    }
    else if ( unitID == 10  && !hasSumiEndured ) // Kasumi
    {
      hasSumiEndured = true;
      confidantEndureBonusID = 17;// Morgana as Placeholder
    }
    if ( IsConfidantBonusObtained(confidantEndureBonusID) )
    {
      result = 805;
    }
  }
  return result;
}

static bool CheckRyujiInstakill( void )
{
  FUNC_LOG("Loading CheckRyujiInstakill\n");
  bool result = IsConfidantBonusObtained( 80 );
  if ( CONFIG_ENABLED( DisableInstakill ) )
  {
    result = false;
  }
  return result;
}

static bool FUN_007ed620Hook( structA_2* a1 )
{
  FUNC_LOG("Loading FUN_007ed620Hook\n");
  printf("Unit Type %d with unit ID %d CHECK_SLIP\n", a1->Field0C->Field18->btlUnitPointer->unitType, a1->Field0C->Field18->btlUnitPointer->unitID);
  enemyBtlUnit = a1->Field0C->Field18->btlUnitPointer;
  printf("Current Active Battle Unit being checked; printing pointer chain\na1 ->0x%x\nField0C -> 0x%x\nField18 -> 0x%x\nbtlUnit Pointer -> 0x%x\n", a1, a1->Field0C, a1->Field0C->Field18, a1->Field0C->Field18->btlUnitPointer);
  return SHK_CALL_HOOK( FUN_007ed620, a1 );
}

static u32 TwinsStoryEncounterEndBattleEarly( u32 a1, u32 a2 )
{
  return SHK_CALL_HOOK( FUN_007af1c0, a1, a2 );
}

static u32 AkechiEncounterCheckEnd( u32 a1, u32 a2 )
{
  FUNC_LOG("Loading AkechiEncounterCheckEnd\n");
  u32 result;
  if ( EncounterIDGlobal >= 830 )
  {
    btlUnit_Unit* Joker = GetBtlPlayerUnitFromID( 1 );
    if ( Joker->Flags & 0x20 && Joker->currentHP == 1 )
    {
      SetBitflagState( 8348, 1 ); // end battle if Joker is 1 hp with endure flag
    }
    result = SHK_CALL_HOOK( FUN_007af1c0, a1, a2 ); // call twins early end function
  }
  else result = SHK_CALL_HOOK( FUN_00784d18, a1, a2 ); // call original

  //printf("FUN_00784d18 called; a1 -> 0x%x; a2 -> 0x%x; result -> 0x%x\n", a1, a2, result);

  return result;
}

static int isMeleeWeaponLeftHanded( int a1 )
{
  int result = SHK_CALL_HOOK( FUN_0003a6e4, a1 );
  //printf("FUN_0003a6e4 called; a1 -> %d; result -> %d\n", a1, result);
  return result;
}

static int isRangedWeaponLeftHanded( int a1 )
{
  int result = SHK_CALL_HOOK( FUN_0003a70c, a1 );
  //printf("FUN_0003a70c called; a1 -> %d; result -> %d\n", a1, result);
  return result;
}

static int FUN_000435c0Hook( int a1, int a2 )
{
  FUNC_LOG("Loading FUN_000435c0Hook\n");
  int result = 1;
  if ( FieldListFile == 0x0 )
  {
    FieldListFile = open_file( "field/fldModelNum.bin", 0 );
    fsSync((int)FieldListFile);
  }

  fieldModelList* fldModelList = FieldListFile->pointerToFile;
  
  while ( fldModelList->FieldMajorID != 0xFFFF )
  {
    //printf("Field Model Amount check; Major ID %d; Minor ID %d; Amount %d\n", fldModelList->FieldMajorID, fldModelList->FieldMinorID, fldModelList->Amount );
    if ( fldModelList->FieldMajorID == a1 && fldModelList->FieldMinorID == a2 )
    {
      return fldModelList->Amount;
    }
    fldModelList++;
  }

  return result;
}

static void PlayJokerDieCueID( CueIDThingy* param_1, int param_2 )
{
  FUNC_LOG("Loading PlayJokerDieCueID\n");
  if ( !btlUnitHasAilment( param_1->Field10->btlUnitPointer, 0x400000 ) )
  {
    LoadSoundByCueIDCombatVoice( param_1, param_2, 10, 0 );
  }
}

static void PlayJokerSkillCueID( CueIDThingy* a1, int a2, int a3 )
{
  FUNC_LOG("Loading PlayJokerSkillCueID\n");
  if ( a1->Field10->btlUnitPointer->unitType == 2 && ActualGetCount( 420 ) > 0 )
  {
    if ( ActualGetCount( 420 ) == 0 )
    {
      JokerPersonaNameCueIDHook( a1, a2 );
    }
    else LoadSoundByCueIDCombatVoice( a1, a2, ActualGetCount( 420 ), 0 );

    ActualSetCount( 420, 0 );
    return;
  }
  else SHK_CALL_HOOK( FUN_00b24938, a1, a2, a3 );
}

static void FUN_00b25348Hook( CueIDThingy* a1, int a2 )
{
  FUNC_LOG("Loading FUN_00b25348Hook\n");
  if ( a1->Field10->btlUnitPointer->unitType == 2 )
  {
    //printf("Enemy %d Executing FUN_00b25348\n", a1->Field10->btlUnitPointer->unitID);
    setBlackMaskCueIDHook( a1, a2, 0 );
  }
  else 
  {
    //printf("Player %d Executing FUN_00b25348\n", a1->Field10->btlUnitPointer->unitID);
    SHK_CALL_HOOK( FUN_00b25348, a1, a2 );
  }
}

static int FUN_00060b90Hook( void )
{
  FUNC_LOG("Loading FUN_00060b90Hook\n");
  int result = SHK_CALL_HOOK( FUN_00060b90 );
  if ( GetBitflagState( 8285 ) )
  {
    result = 9;
  }
  return result;
}

/*static int CMM_RANKUP( void )
{
  int targetConfidant = FLW_GetIntArg( 0 );
  if ( targetConfidant == 33 ) // Faith/Kasumi
  {
    int targetRank = FLW_GetIntArg( 1 );

    if ( 0 > targetRank ) // prevent invalid rank values
    {
      targetRank = 0;
    }
    else if ( targetRank >= 10 )
    {
      targetRank = 10;
    }

    ConfidantStats* CMM_Data_Current = 0x010c1708;
    bool isCMMExist = false;

    for ( int i = 0; i < 22; i++ ) // check for existing confidant
    {
      //printf("CMM Slot %d contains CMM ID %d; rank %d\n", i, CMM_Data_Current->cmmID, CMM_Data_Current->cmmRank);
      if ( CMM_Data_Current->cmmID == 33 && CMM_Data_Current->cmmRank != targetRank )
      {
        CMM_Data_Current->cmmRank = targetRank;
        isCMMExist = true;
      }
      CMM_Data_Current++;
    }

    if ( !isCMMExist )
    {
      CMM_Data_Current = 0x010c1708;
      for ( int i = 0; i < 22; i++ ) // add confidant if not exist
      {
        if ( CMM_Data_Current->cmmID == 0 && CMM_Data_Current->cmmRank == 0 )
        {
          CMM_Data_Current->cmmID = 33;
          CMM_Data_Current->cmmRank = targetRank;
          i = 99;
        }
        CMM_Data_Current++;
      }
    }
  }
  return SHK_CALL_HOOK( FUN_0054fcb4 );
}*/

static int FUN_00927d50Hook( char* a1 ) // split combat GAP animation load
{
  FUNC_LOG("Loading FUN_00927d50Hook\n");
  if ( a1[19] == '9' && a1[33] == '9' && a1[37] == '1' && GetEquippedPersonaFunction(9) != Persona_RobinHood && CONFIG_ENABLED( enableAkechiMod ) )
  {
    a1[37] = '2'; // checking for model/character/0009/battle/bb0009_051_###a.GAP
  }
  return SHK_CALL_HOOK( FUN_00927d50, a1 );
}

/*static u64 BattleAnimationsHook( CueIDThingy* a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6, u64 a7, u64 a8, u64 a9, u64 a10, u64 a11 )
{
  u64 result = SHK_CALL_HOOK( BattleAnimations, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11 );
  if ( a1->Field10->btlUnitPointer->unitType == 1 )
  {
    printf("Anim data Arguments:  \n A1: %X \n ID: %d \n Animation: %d \n A3: %X \n A4: %F \n A5: %X\n", a1, a1->Field10->btlUnitPointer->unitID, a2, a3, a4, a5);
  }
  return result;
}

static int FUN_00b0efa8Hook( CueIDThingy* a1, double a2, double a3 )
{
  int result = SHK_CALL_HOOK( FUN_00b0efa8, a1, a2, a3 );
  printf( "Twins Hurt Animation Index Function called; Function Address 0x00b0efa8\na1 -> 0x%x; unit ID -> %d\n", a1, a1->Field10->btlUnitPointer->unitID );
  return result;
}*/

static int FUN_00b0f558Hook( CueIDThingy* a1, double a2, double a3 )
{
  FUNC_LOG("Loading FUN_00b0f558Hook\n");
  int result = SHK_CALL_HOOK( FUN_00b0f558, a1, a2, a3 );
  //printf( "Baton Pass Out Function called; Function Address 0x00b0f558\na1 -> 0x%x; unit ID -> %d\n", a1, a1->Field10->btlUnitPointer->unitID );
  return result;
}

static int FUN_00b0f5e8Hook( CueIDThingy* a1, double a2, double a3 )
{
  int result = SHK_CALL_HOOK( FUN_00b0f5e8, a1, a2, a3 );
  //printf( "Baton Pass In Function called; Function Address 0x00b0f5e8\na1 -> 0x%x; unit ID -> %d\n", a1, a1->Field10->btlUnitPointer->unitID );
  return result;
}

static int FUN_00aff500Hook( CueIDThingy* a1, double a2, double a3 )
{
  int result = SHK_CALL_HOOK( FUN_00aff500, a1, a2, a3 );
  //printf( "Player Baton Pass In Function called; Function Address 0x00aff500\na1 -> 0x%x; unit ID -> %d\n", a1, a1->Field10->btlUnitPointer->unitID );
  return result;
}

static int FUN_00aff590Hook( CueIDThingy* a1, double a2, double a3 )
{
  int result = SHK_CALL_HOOK( FUN_00aff590, a1, a2, a3 );
  //printf( "Player Baton Pass Out Function called; Function Address 0x00aff590\na1 -> 0x%x; unit ID -> %d\n", a1, a1->Field10->btlUnitPointer->unitID );
  return result;
}

static bool FUN_00786a88Hook( int a1, int a2 )
{
  bool result = SHK_CALL_HOOK( FUN_00786a88, a1, a2 );
  //printf( "FUN_00786a88 called; result -> %d\n", result );
  return result;
}

/*static int FUN_00b03510Hook( CueIDThingy* a1, double a2, double a3 )
{
  printf( "Enemy Dodge Anim Function running for enemy ID %d; Persona ID -> %d\n", a1->Field10->btlUnitPointer->unitID, a1->Field10->btlUnitPointer->StockPersona[0].personaID );
  if ( a1->Field10->btlUnitPointer->StockPersona[0].personaID > 0 )
  {
    printf( "Enemy ID dodging -> %d\n", a1->Field10->btlUnitPointer->unitID );
    SetTwinsDodgeAnimation( a1, a2, a3 );
    return 1;
  }
  else return 0;
}*/

/*
static int FUN_00b03248Hook( CueIDThingy* a1, double a2, double a3 )
{
  //u64 address = a3;
  int result = SHK_CALL_HOOK( FUN_00b03248, a1, a2, a3 );
  printf( "Enemy Hurt Animation Index Function called; Function Address 0xb03248\na1 -> 0x%x; unit ID -> %d\n", a1, a1->Field10->btlUnitPointer->unitID );
  return result;
}*/

/*
static int FUN_0065c7f8Hook( int a1, int a2 )
{
  int result = 0;
  if ( IsBtlUnitDead( GetBtlPlayerUnitFromID(1), 0 ) )
  {
    for ( int i = 1; i <= 3; i++ )
    {
      if ( !IsBtlUnitDead( GetBtlPlayerUnitFromID(GetUnitIDFromPartySlot(i)), 0 ) )
      {
        printf("Found alive party member, no game over\n");
        result = 0;
      }
      else result = SHK_CALL_HOOK( FUN_0065c7f8, a1, a2 );
    }
  }
  else result = SHK_CALL_HOOK( FUN_0065c7f8, a1, a2 );

  return result;
}*/

static int FUN_006a75c4Hook( u64 a1 )
{
  if (!CONFIG_ENABLED( PreventJokerGameOver ))
  {
    return SHK_CALL_HOOK( FUN_006a75c4, a1 );
  }
  
  int result = 0;

  if ( isPlayerUnitDead( 1 ) )
  {
    if (   !isPlayerUnitDead(GetUnitIDFromPartySlot(1))
        || !isPlayerUnitDead(GetUnitIDFromPartySlot(2))
        || !isPlayerUnitDead(GetUnitIDFromPartySlot(3)))
      {
        //DEBUG_LOG("Found alive party member, no game over\n");
        result = 0;
      }
      else result = SHK_CALL_HOOK( FUN_006a75c4, a1 );
  }
  else result = SHK_CALL_HOOK( FUN_006a75c4, a1 );
  
  if ( GetTotalUnitsOfType( 2 ) == 0 ) // failsafe for no enemies, force battle to end
  {
    if( isPlayerUnitDead( 1 ) )
    {
      btlUnit_Unit* Joker = GetBtlPlayerUnitFromID(1);
      Joker->currentHP = 1;
      Joker->StatusAilments = (Joker->StatusAilments & ~(1UL << 19)) | (0 << 19); // clear dead ailment
    }
    return SHK_CALL_HOOK( FUN_006a75c4, a1 );
  }

  return result;
}

static void PlayJokerRevivedCueID( CueIDThingy* param_1, int param_2 )
{
  FUNC_LOG("Loading PlayerJokerRevivedCueID\n");
  if ( !btlUnitHasAilment( param_1->Field10->btlUnitPointer, 0x400000 ) )
  {
    LoadSoundByCueIDCombatVoice( param_1, param_2, 2803, 0 );
  }
}

static bool CheckAllPartyMembersDead( void )
{
  bool isGameOver = false;

  if ( isPlayerUnitDead(GetUnitIDFromPartySlot(0))
    && isPlayerUnitDead(GetUnitIDFromPartySlot(1))
    && isPlayerUnitDead(GetUnitIDFromPartySlot(2))
    && isPlayerUnitDead(GetUnitIDFromPartySlot(3)))
  {
    isGameOver = true; // all party units who were in combat are dead, game over
  }

  if ( isGameOver && isPreventGameOver ) // allow no game over on loss under specific circumstances
  { 
    DEBUG_LOG("Preventing Game Over on player loss\n");
    isGameOver = false;
  }

  isPreventGameOver = false; // clear this even if the player wins

  return isGameOver;
}

int FUN_00650700Hook( int a1, int a2 ) // triggers when player is about to escape
{
  if( isPlayerUnitDead( 1 ) && CONFIG_ENABLED( PreventJokerGameOver ) )
  {
    btlUnit_Unit* Joker = GetBtlPlayerUnitFromID(1); 
    Joker->currentHP = 1;
    Joker->StatusAilments = (Joker->StatusAilments & ~(1UL << 19)) | (0 << 19); // clear dead ailment
  }
  return SHK_CALL_HOOK( FUN_00650700, a1, a2 );
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void dcInit( void )
{
  randomizedCombatOutfit = true;
  randomSetSeed( getTicks() );
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( setBlackMaskCueID, setBlackMaskCueIDHook );
  SHK_BIND_HOOK( BlackMaskBossVoice, BlackMaskBossVoiceHook );
  SHK_BIND_HOOK( LoadPlayerCombatVoicePack, LoadPlayerCombatVoicePackHook );
  SHK_BIND_HOOK( LoadGenericEnemyVoicePack, LoadGenericEnemyVoicePackHook );
  SHK_BIND_HOOK( LoadPartyPanelSPD, LoadPartyPanelSPDHook );
  SHK_BIND_HOOK( LoadPartyPanelPLG, LoadPartyPanelPLGHook );
  SHK_BIND_HOOK( Load2DUIDDS, Load2DUIDDSHook );
  SHK_BIND_HOOK( LoadBEDFile, LoadBEDFileHook );
  SHK_BIND_HOOK( LoadCombatCutin, LoadCombatCutinHook );
  SHK_BIND_HOOK( LoadAnimationPack, LoadAnimationPackHook );
  SHK_BIND_HOOK( LoadBCDFunction, LoadBCDFunctionHook );
  SHK_BIND_HOOK( CombatPersonaCueID, CombatPersonaCueIDHook );
  SHK_BIND_HOOK( FUN_00b2cee8, EnemyAkechiSkillCueID );
  SHK_BIND_HOOK( JokerPersonaNameCueID, JokerPersonaNameCueIDHook );
  //SHK_BIND_HOOK( BtlUnitGetUnitID, BtlUnitGetUnitIDHook );
  SHK_BIND_HOOK( ParseUNITTBL, ParseUNITTBLHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_EnemyStats, ReturnAddressOfUNITTBL_EnemyStatsHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_EnemyAffinities, ReturnAddressOfUNITTBL_EnemyAffinitiesHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_PersonaAffinities, ReturnAddressOfUNITTBL_PersonaAffinitiesHook );
  SHK_BIND_HOOK( ReturnAddressOfUNITTBL_Segment3, ReturnAddressOfUNITTBL_Segment3Hook );
  SHK_BIND_HOOK( ReturnAddressOfELSAITBL_Segment1, ReturnAddressOfELSAITBL_Segment1Hook );
  SHK_BIND_HOOK( CalculateShdPersonaEnemyEntry, CalculateShdPersonaEnemyEntryHook );
  SHK_BIND_HOOK( SetUpEncounterFlags, SetUpEncounterFlagsHook );
  SHK_BIND_HOOK( FUN_0026b390, isPersonaExist );
  SHK_BIND_HOOK( FUN_0026b360, isPersonaExist2 );
  SHK_BIND_HOOK( FUN_0026b2b0, isEnemyExist );
  SHK_BIND_HOOK( FUN_0026b2e0, isEnemyExist2 );
  SHK_BIND_HOOK( FUN_0026b320, isEnemyExist3 );
  SHK_BIND_HOOK( FUN_0026aff0, isPartyMemberExist );
  SHK_BIND_HOOK( FUN_0026b094, isPartyMemberExist2 );
  SHK_BIND_HOOK( FUN_0026b148, isPartyMemberExist3 );
  SHK_BIND_HOOK( FUN_0026b1fc, isPartyMemberExist4 );
  SHK_BIND_HOOK( SetEnemyAkechiPersona, SetEnemyAkechiPersonaHook );
  SHK_BIND_HOOK( EnemyHasCombatCutin, EnemyHasCombatCutinHook );
  SHK_BIND_HOOK( FUN_00263b94, FUN_00263b94Hook );
  SHK_BIND_HOOK( IsEncounterEventSoundBankExist, IsEncounterEventSoundBankExistHook );
  SHK_BIND_HOOK( FUN_00af3cb0, CheckIsEncounterAmbush );
  SHK_BIND_HOOK( FUN_003735d8, FUN_003735d8Hook );
  SHK_BIND_HOOK( FUN_00829ce8, FUN_00829ce8Hook );
  SHK_BIND_HOOK( FUN_00043fac, GetNPCTBLEntry );
  SHK_BIND_HOOK( FUN_00262258, GetItemTBLMeleeWeaponField0E );
  SHK_BIND_HOOK( FUN_00745e28, CheckPartyMemberConfidantCombatAbilities );
  SHK_BIND_HOOK( FUN_00256830, CheckUnitIsEndure );
  SHK_BIND_HOOK( FUN_00338a04, CheckRyujiInstakill );
  SHK_BIND_HOOK( FUN_007ed620, FUN_007ed620Hook );
  SHK_BIND_HOOK( FUN_007af1c0, TwinsStoryEncounterEndBattleEarly );
  SHK_BIND_HOOK( FUN_00784d18, AkechiEncounterCheckEnd );
  SHK_BIND_HOOK( FUN_0003a6e4, isMeleeWeaponLeftHanded );
  SHK_BIND_HOOK( FUN_0003a70c, isRangedWeaponLeftHanded );
  SHK_BIND_HOOK( FUN_000435c0, FUN_000435c0Hook );
  SHK_BIND_HOOK( FUN_00b24f84, PlayJokerDieCueID );
  SHK_BIND_HOOK( FUN_00b24938, PlayJokerSkillCueID );
  SHK_BIND_HOOK( FUN_00b25348, FUN_00b25348Hook );
  SHK_BIND_HOOK( FUN_00060b90, FUN_00060b90Hook );
  //SHK_BIND_HOOK( FUN_0054fcb4, CMM_RANKUP );
  SHK_BIND_HOOK( FUN_00927d50, FUN_00927d50Hook );
  SHK_BIND_HOOK( FUN_00b0f558, FUN_00b0f558Hook );
  SHK_BIND_HOOK( FUN_00b0f5e8, FUN_00b0f5e8Hook );
  SHK_BIND_HOOK( FUN_00aff500, FUN_00aff500Hook );
  SHK_BIND_HOOK( FUN_00aff590, FUN_00aff590Hook );
  SHK_BIND_HOOK( FUN_00786a88, FUN_00786a88Hook );
  //SHK_BIND_HOOK( FUN_00b03510, FUN_00b03510Hook );
  //SHK_BIND_HOOK( FUN_00b0efa8, FUN_00b0efa8Hook );
  //SHK_BIND_HOOK( FUN_00b03248, FUN_00b03248Hook );
  //SHK_BIND_HOOK( BattleAnimations, BattleAnimationsHook );
  SHK_BIND_HOOK( FUN_006a75c4, FUN_006a75c4Hook );
  SHK_BIND_HOOK( FUN_00b25084, PlayJokerRevivedCueID );
  SHK_BIND_HOOK( FUN_00b25088, CheckAllPartyMembersDead );
  SHK_BIND_HOOK( FUN_00650700, FUN_00650700Hook );
}

void dcShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif