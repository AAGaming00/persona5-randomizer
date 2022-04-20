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
#include "p5e.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

#define FUNC_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( functest ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

#define FILE_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( enableFileAccessLogging ) ) printf( msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( void, setBgm, int id );
SHK_HOOK( int, FUN_0072360c, int id );
SHK_HOOK( int, FUN_001cf704, u64 unk, int a1, int a2, int a3 );
SHK_HOOK( int, LoadEPL, char* EPL, u8 a2 );
SHK_HOOK( int, criFs_Initialize );
SHK_HOOK( u16, LoadMeleeWeaponModelTable, int a1 );
SHK_HOOK( u16, LoadGunWeaponModelTable, int a1 );
SHK_HOOK( u16, FUN_0003a658, int a1 );
SHK_HOOK( u16, FUN_0003a698, int a1 );
SHK_HOOK( int, criFsBinder_BindCpk, char* arg );
SHK_HOOK( u64, criFsBinder_SetPriority, u32 a1, u32 a2 );
SHK_HOOK( int, crifsloader_load_registered_file, fileAccessStruct* a1, int a2, int a3, int a4, int a5 );
SHK_HOOK( int, GenericCharacterModelLoader, char* result, u64 modelType, u64 characterID, u64 modelID, u64 modelSubID );
SHK_HOOK( u64, FUN_0004b24c, u64 a1 );
SHK_HOOK( int, FUN_0004951c, int a1, structB* a2, structB* a3 );
SHK_HOOK( int, FUN_002625d4, int a1, int a2 );
SHK_HOOK( int, FUN_00045d24, int unitID, int modelID_base, int a3 );
SHK_HOOK( int, FUN_000bee20, char* a1, u32 a2, u32 a3 );
SHK_HOOK( int, FUN_00332b4c, u32 a1, u32 a2, u32 a3 );

static void setBgmHook( int id )
{
  if ( id == 101 && sequenceIDGlobal == 1 )
  {
    if ( CONFIG_ENABLED( enableExpandedBGM ) && CONFIG_ENABLED( P5RTitleBGM ) )
    {
      if ( titleScreenBGM == 0 ) // P5
      {
        RandomizeTitleScreenBGM();
      }
      else if ( titleScreenBGM == 1 ) // P5R
      {
        id = 901;
        RandomizeTitleScreenBGM();
      }
      else if ( titleScreenBGM == 2 ) // P5S
      {
        id = 970;
        RandomizeTitleScreenBGM();
      }
    }
    if (CONFIG_ENABLED(enableRandomizerModule)) {
      setRandomizerReady(true);
    }
  }
  else if ( id == 340 ) //Victory Screen
  {
    randomizedCombatOutfit = true;
  }

  FILE_LOG("SetBGM Called with BGM ID -> %d\n", id);
  SHK_CALL_HOOK( setBgm, id );
}

static CharModelReplacementTable charModelReplacementTableEntry[] =
{
  { 51, 51, 51, 51, 51 }, // 0
  { 51, 52, 81, 103, 113 }, // 1
  { 51, 81, 103, 106, 51 }, // 2
  { 51, 61, 81, 101, 51 }, // 3
  { 51, 81, 106, 107, 51 }, // 4
  { 51, 81, 103, 106, 51 }, // 5
  { 51, 81, 104, 107, 51 }, // 6
  { 51, 81, 102, 10, 51 }, // 7
  { 51, 81, 102, 106, 51 }, // 8
  { 51, 52, 81, 102, 104 }, // 9
};

static int GenericCharacterModelLoaderHook( char* result, u64 modelType, u64 characterID, u64 modelID, u64 modelSubID )
{
  FUNC_LOG("Loading GenericCharacterModelLoaderHook\n");
  //printf( "Model Type %d loading for character ID %d\n", modelType, characterID );
  if ( modelType == 2 || modelType == 5 ) // human character models
  {
    //printf("Character ID %d loading model ID %d\n", characterID, modelID);
    if ( characterID == 9 && modelID == 51 && GetEquippedPersonaFunction(9) != Persona_RobinHood ) // Darkechi model
    {
      modelID = 52;
    }
    if ( characterID >= 1 && characterID <= 10 ) // check PT models in cutscenes
    {
      CharModelReplacementTable* pEntry = &charModelReplacementTableEntry[characterID];
      for ( int j = 0; j <= 4; j++ )
      {
        u32 playerOutfitModel = PlayerUnitGetModelMinorID( characterID, 50, 0 );
        if ( modelID == pEntry->modelId[j] && playerOutfitModel != 51 && CONFIG_ENABLED( enableCutsceneOutfits ) )
        {
          modelID = playerOutfitModel;
        }
      }
      if ( modelID == 49 && CONFIG_ENABLED( enableExpandedBGM ) && CONFIG_ENABLED( P5RTitleBGM ) )
      {
        modelSubID = titleScreenBGM;
      }
    }
    if ( characterID == 1 ) // store Joker model for Kasumi model fix
    {
      JokerModel = modelID;
    }
    if ( characterID == 10 && modelID == 50 ) //for now copy Joker outfit to Kasumi
    {
      if ( sequenceIDGlobal == 4 ) // fix for some boss battles not reloading joker model
      {
        modelID = PlayerUnitGetModelMinorID( 1, 50, 0 );
      }
      else modelID = JokerModel;

      if ( CONFIG_ENABLED( enableSumire ) )
      {
        modelSubID = 1;
      }
    }
    if ( characterID >= 1 && characterID <= 10 || characterID == 1011 || characterID == 1003 ) // Midwinter Models
    {
      if ( modelID == 2 && isMidWinterValid() )
      {
        modelID = 5;
      }
      else if ( modelID == 4 && isMidWinterValid() )
      {
        modelID = 6;
      }
    }
    else if ( characterID == 2104 && modelID == 0 && isMidWinterValid() ) //Wakaba loads model 0 for some reason
    {
      modelID = 6;
    }
    if ( characterID == 10 ) //Saving Kasumi model for later use
    {
      KasumiModelSubID = modelID;
    }
    //printf("Character ID %d loading model ID %d\n", characterID, modelID);
  }
  else if ( modelType == 0x1D ) // Persona models in velvet room
  {
    if ( CONFIG_ENABLED( forcePSZModel ) ) // party member reserve personas
    {
      return sprintf( result, "model/character/persona/%04d/psz%04d.GMD", characterID, characterID );
    }
  }
  else if ( modelType == 4 ) // Persona models
  {
    if ( CONFIG_ENABLED( forcePSZModel ) ) // party member reserve personas
    {
      return sprintf( result, "model/character/persona/%04d/psz%04d.GMD", characterID, characterID );
    }
    else if ( characterID >= 239 && characterID <= 250 ) // party member tier 3 personas
    {
      return sprintf( result, "model/character/persona/%04d/psz%04d.GMD", characterID, characterID );
    }
    else if ( characterID >= 221 && characterID <= 238 ) // party member reserve personas
    {
      modelType = 3; // force load regular Persona models instead of using PSZ/Enemy model
    }
    else if ( characterID >= 322 ) // only force this on reserve personas
    {
      modelType = 3; // force load regular Persona models instead of using PSZ/Enemy model
    }
  }
  else if ( modelType == 16 && characterID == 8020 ) //Atlus hardcoded Kasumi bag for whoever knows what fucking reason
  {
    if ( KasumiModelSubID != 1 && KasumiModelSubID != 2 )
    {
      characterID = 659;
    }
  }
  return SHK_CALL_HOOK( GenericCharacterModelLoader, result, modelType, characterID, modelID, modelSubID );
}

static int LoadEPLHook( char* EPL, u8 a2 )
{
  FUNC_LOG("Loading LoadEPLHook\n");
  if ( strcmp( EPL, "battle/event/BCD/j_sien/bes_j_htb.EPL" ) == 0 && CONFIG_ENABLED( enableCutsceneOutfits ) )
  {
    u32 FutabaOutfit = PlayerUnitGetModelMinorID( 8, 50, 0 );
    if ( FutabaOutfit != 51 )
    {
      char newOutfitEPL[64];
      sprintf(newOutfitEPL, "battle/event/BCD/j_sien/bes_j_htb_%03d.EPL", FutabaOutfit);
      return SHK_CALL_HOOK( LoadEPL, newOutfitEPL, a2 );
    }
  }
  return SHK_CALL_HOOK( LoadEPL, EPL, a2 );
}

static int forceSingleGAP( int playerID )
{
  if ( CONFIG_ENABLED( forceSingleCombatGAP ) )
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

static int criFsBinder_BindCpkHook( char* arg )
{
  int result = SHK_CALL_HOOK( criFsBinder_BindCpk, arg );
  DEBUG_LOG("criFsBinder_BindCpk result %d\n", result );
  return result;
}

static int criFs_InitializeHook( void )
{
  if ( !CONFIG_ENABLED( enableModCPK ) )
  {
    return SHK_CALL_HOOK( criFs_Initialize );
  }
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  char acStack288 [264];

  iVar1 = FUN_00ab563c((int)0x00d4bd54);
  iVar1 = FUN_001a52f8(iVar1);
  if (iVar1 != 2) 
  {
    pcVar2 = FUN_001a5834();
    sprintf(acStack288,"%s/hdd.cpk",pcVar2);
    criFsBinder_BindCpkHook(acStack288);
  }

  pcVar2 = FUN_00968be8();
  pcVar3 = FUN_00968bf4();
  
  if ( CONFIG_ENABLED( enableModCPK ) )
  {
    iVar1 = sprintf(acStack288,"%s%s/%s.cpk",pcVar2,pcVar3, CONFIG_STRING(modCPKName));
    iVar1 = criFsBinder_BindCpkHook(acStack288);
    if (iVar1 > 0) criFsBinder_SetPriority(iVar1, 30 + CONFIG_INT( extraModCPK ) + 1);
  }

  u32 extraCPK = CONFIG_INT( extraModCPK );
  if ( CONFIG_ENABLED( enableModCPK ) && extraCPK > 0 )
  {
    for (int i = 0; i < extraCPK; i++)
    {
      iVar1 = sprintf(acStack288,"%s%s/%s_%02d.cpk", pcVar2, pcVar3, CONFIG_STRING_ARRAY(extraModCPKName)[i], i + 1);
      iVar1 = criFsBinder_BindCpkHook(acStack288);
      if (iVar1 > 0) criFsBinder_SetPriority(iVar1, 30 + CONFIG_INT( extraModCPK ) - i);
    }
  }

  iVar1 = sprintf(acStack288,"%s%s/ps3.cpk",pcVar2,pcVar3);
  iVar1 = criFsBinder_BindCpkHook(acStack288);
  
  iVar1 = sprintf(acStack288,"%s%s/data.cpk",pcVar2,pcVar3);
  iVar1 = criFsBinder_BindCpkHook(acStack288);

  return iVar1;
}

static u64 criFsBinder_SetPriorityHook( u32 a1, u32 a2 )
{
  DEBUG_LOG("criFsBinder_SetPriority called; a1 -> 0x%x ;  a2 -> 0x%x\n", a1, a2);
  return SHK_CALL_HOOK( criFsBinder_SetPriority, a1, a2 );
}

static int crifsloader_load_registered_fileHook( fileAccessStruct* a1, int a2, int a3, int a4, int a5 )
{
  FILE_LOG("%s\n", a1->fileNamePtr);

  return SHK_CALL_HOOK( crifsloader_load_registered_file, a1, a2, a3, a4, a5 );
}

static u16 LoadMeleeWeaponModelTableHook( int a1 )
{
  FUNC_LOG("Loading LoadMeleeWeaponModelTableHook\n");
  a1 = FUN_2604C4(a1); // weird thing they do to flip negative values
  u16 result = GetMeleeWeaponTBLEntry(a1)->field_0xe;

  /*printf("Melee weapon model load; a1 -> %d\n", a1);
  printf("Weapon Model ID result -> %d\n", result);*/
  return result;
}

static u16 LoadGunWeaponModelTableHook( int a1 )
{
  FUNC_LOG("Loading LoadGunWeaponModelTableHook\n");
  a1 = FUN_2604C4(a1); // weird thing they do to flip negative values
  u16 result = GetRangedWeaponTBLEntry(a1)->RESERVE;
  /*printf("Gun model load; a1 -> %d\n", a1);
  printf("Gun Model ID result -> %d\n", result);*/
  return result;
}

static u16 FUN_0003a658Hook( int a1 )
{
  FUNC_LOG("Loading FUN_0003a658Hook\n");
  a1 = FUN_2604C4(a1);
  if ( a1 >= 255 )
  {
    return 0;
  }
  else return SHK_CALL_HOOK( FUN_0003a658, a1 );
}

static u16 FUN_0003a698Hook( int a1 )
{
  FUNC_LOG("Loading FUN_0003a698Hook\n");
  a1 = FUN_2604C4(a1);
  if ( a1 >= 255 )
  {
    return 0;
  }
  else return SHK_CALL_HOOK( FUN_0003a698, a1 );
}

static int FUN_001cf704Hook( u64 unk, int charID, int expressionID, int outfitID ) // Bustups function
{
  FUNC_LOG("Loading FUN_001cf704Hook\n");
  //printf("Bustup file loaded\ncharacter ID -> %d\nExpression ID -> %d\nOutfit ID -> %d\n", charID, expressionID, outfitID);
  if ( charID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood && expressionID >= 100 && expressionID < 120 )
  {
    expressionID += 700;
  }

  if ( charID > 0 && charID <= 11 || charID == 111 || charID == 208 || charID == 103 )
  {
    if ( outfitID == 2 && isMidWinterValid() )
    {
      outfitID = 5;
    }
    else if ( outfitID == 4 && isMidWinterValid() )
    {
      outfitID = 6;
    }
  }
  return SHK_CALL_HOOK( FUN_001cf704, unk, charID, expressionID, outfitID );
}

static u64 FUN_0004b24cHook( u64 a1 ) // function that loads scheduler bf
{
  isLoadScheduler = true;
  return SHK_CALL_HOOK( FUN_0004b24c, a1 );
}

static int FUN_0004951cHook( int a1, structB* a2, structB* a3 ) // calculates days, called during scheduler bf load calc
{
  int result = SHK_CALL_HOOK( FUN_0004951c, a1, a2, a3 );
  if ( isLoadScheduler )
  {
    //printf("FUN_0004951cHook called, a1 -> %d, a2 -> %d, a3 -> %d\n", a1, a2->field00, a3->field00);
    if ( GetBitflagState( 2162 ) && ( a2->field00 == 1 || a2->field00 == 2 || a2->field00 == 3 || a2->field00 == 12 ) )
    {
      a2->field00 += 20;
    }
    isLoadScheduler = false;
  }
  return result;
}

static int GetOutfitGearEffects( u32 outfitID, u32 gearEffectSlot )
{
  return 0;
}

static int GetCombatModelMinorIDFromOutfit( int unitID, int modelID_base, int a3 )
{
  FUNC_LOG("Loading GetCombatModelMinorIDFromOutfit\n");
  int result = SHK_CALL_HOOK( FUN_00045d24, unitID, modelID_base, a3 );
  
  if ( unitID < 1 )
  {
    return result;
  }
  
  if ( unitID >= 11 )
  {
    return result;
  }

  if ( unitID == 10)
  {
    result = SHK_CALL_HOOK( FUN_00045d24, 1, modelID_base, a3 );
  }

  if ( modelID_base == 50 ) // combat model
  {
    int targetOutfitID = GetBtlPlayerUnitFromID( unitID )->outfitID - 0x7000;
    targetOutfitID = GetOutfitTBLEntry( targetOutfitID )->GearEffect[0];

    if ( targetOutfitID != 0 )
    {
      result = targetOutfitID; // read outfit tbl value and use it as model minor id
    }
  }

  return result;
}

fileHandleStruct* testBF = 0;

short JokerModelIDs[33] = { 51, 52, 66, 67, 71, 72, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177 };
short SkullModelIDs[27] = { 51, 151, 152, 153, 154, 155, 156, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177 };
short MonaModelIDs[21] = { 51, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177 };
short PantherModelIDs[31] = { 51, 72, 103, 74, 83, 151, 152, 153, 154, 155, 156, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177 };
short FoxModelIDs[26] = { 51, 151, 152, 153, 154, 156, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177 };
short QueenModelIDs[28] = { 51, 83, 151, 152, 153, 154, 155, 156, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177 };
short NoirModelIDs[27] = { 51, 151, 152, 153, 154, 155, 156, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177 };
short OracleModelIDs[26] = { 51, 153, 154, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 180 };
short CrowModelIDs[26] = { 51, 151, 152, 153, 154, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 180 };
short VioletModelIDs[39] = { 5, 6, 51, 151, 152, 153, 154, 155, 156, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 172, 173, 174, 175, 176, 21, 22, 23, 24, 25, 26, 71, 74, 79, 81, 83, 84 };

static int FUN_000bee20Hook( char *a1 , u32 a2, u32 a3 )
{
  FILE_LOG("Loading Field ID %03d %03d\n", a2 % 1000, a3 % 1000 );

  LastLoadedFieldMajorID = a2 % 1000;
  LastLoadedFieldMinorID = a3 % 1000;

  if ( CONFIG_ENABLED( randomCombatOutfit ) && randomizedCombatOutfit ) // dont randomize during victory
  {
    int unitID = 0;
    for (int i = 1; i <= 10; i++) // ID 1 Joker -> ID 10 Violet
    {
      unitID = GetBtlPlayerUnitFromID(i)->unitID;

      int targetOutfitID = GetBtlPlayerUnitFromID( unitID )->outfitID - 0x7000;

      switch( unitID )
      {
        case 1:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = JokerModelIDs[randomIntBetween( 0, 32 )];
          break;
        case 2:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = SkullModelIDs[randomIntBetween( 0, 26 )];
          break;
        case 3:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = MonaModelIDs[randomIntBetween( 0, 20 )];
          break;
        case 4:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = PantherModelIDs[randomIntBetween( 0, 30 )];
          break;
        case 5:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = FoxModelIDs[randomIntBetween( 0, 25 )];
          break;
        case 6:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = QueenModelIDs[randomIntBetween( 0, 27 )];
          break;
        case 7:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = NoirModelIDs[randomIntBetween( 0, 26 )];
          break;
        case 8:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = OracleModelIDs[randomIntBetween( 0, 25 )];
          break;
        case 9:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = CrowModelIDs[randomIntBetween( 0, 25) ];
          break;
        case 10:
          GetOutfitTBLEntry( targetOutfitID )->GearEffect[0] = VioletModelIDs[randomIntBetween( 0, 39) ];
          break;
        default:
          break;
      }
      //printf("Randomizing Player Outfit for unit %d into outfit ID %d\n", unitID, GetOutfitTBLEntry( targetOutfitID )->GearEffect[0]);
    }
  }

  if ( CONFIG_ENABLED (enableGlobalBF))
  {
    if (testBF == (void*)0x0)
    {
      testBF = open_file( "script/test.bf", 0 );
      u64 fsSyncResult = fsSync((int)testBF);
    }
    scrRunScript(0, testBF->pointerToFile, testBF->bufferSize, 0);
  }

  return sprintf( a1 ,"field/f%03d_%03d.pac", a2 % 1000, a3 % 1000);
}

static int FUN_00332b4cHook( u32 a1, u32 a2, u32 a3 )
{
  FUNC_LOG("Loading FUN_00332b4cHook\n");
  if ( a3 == 1 && ActualGetCount(18)/100000 > 0 && CONFIG_ENABLED( DebugModelTesting ))
  {
    a3 = ActualGetCount(18)/100000;
  }
  
  return SHK_CALL_HOOK( FUN_00332b4c, a1, a2, a3 );
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void p5eInit( void )
{
  randomSetSeed( getTicks() );
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( FUN_0072360c, forceSingleGAP );
  SHK_BIND_HOOK( setBgm, setBgmHook );
  SHK_BIND_HOOK( LoadEPL, LoadEPLHook );
  SHK_BIND_HOOK( GenericCharacterModelLoader, GenericCharacterModelLoaderHook );
  SHK_BIND_HOOK( criFsBinder_BindCpk, criFsBinder_BindCpkHook );
  SHK_BIND_HOOK( criFs_Initialize, criFs_InitializeHook );
  SHK_BIND_HOOK( criFsBinder_SetPriority, criFsBinder_SetPriorityHook );
  SHK_BIND_HOOK( crifsloader_load_registered_file, crifsloader_load_registered_fileHook );
  SHK_BIND_HOOK( LoadMeleeWeaponModelTable, LoadMeleeWeaponModelTableHook );
  SHK_BIND_HOOK( LoadGunWeaponModelTable, LoadGunWeaponModelTableHook );
  SHK_BIND_HOOK( FUN_0003a658, FUN_0003a658Hook );
  SHK_BIND_HOOK( FUN_0003a698, FUN_0003a698Hook );
  SHK_BIND_HOOK( FUN_001cf704, FUN_001cf704Hook );
  SHK_BIND_HOOK( FUN_0004b24c, FUN_0004b24cHook );
  SHK_BIND_HOOK( FUN_0004951c, FUN_0004951cHook );
  SHK_BIND_HOOK( FUN_002625d4, GetOutfitGearEffects );
  SHK_BIND_HOOK( FUN_00045d24, GetCombatModelMinorIDFromOutfit );
  SHK_BIND_HOOK( FUN_000bee20, FUN_000bee20Hook );
  SHK_BIND_HOOK( FUN_00332b4c, FUN_00332b4cHook );
  titleScreenBGM = 99;
  RandomizeTitleScreenBGM();
}

void p5eShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}

void RandomizeTitleScreenBGM()
{
  int lastBGM = titleScreenBGM;
  while ( lastBGM == titleScreenBGM )
  {
    titleScreenBGM = randomIntBetween( 0, 2 );
  }
}
#endif