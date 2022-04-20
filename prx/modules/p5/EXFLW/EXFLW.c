#ifdef GAME_P5
// PS3 system includes
#include <sys/prx.h>
#include <sys/tty.h>
#include <sys/syscall.h>
#include "lib/file.h"
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
#include "modules/p5/printf_FLW.h"
#include "EXFLW.h"
#include "modules/p5/randomizer/randomizer.h"

#define DEBUG_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( debug ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

#define FUNC_LOG( msg, ... ) \
  if ( CONFIG_ENABLED( functest ) ) printf( "DEBUG: " msg, ##__VA_ARGS__ )

// You need to declare hooks with SHK_HOOK before you can use them.
SHK_HOOK( int, EX_FLW_setHumanLv );
SHK_HOOK( int, SET_PERSONA_LV );
SHK_HOOK( int, PUT_Function );
SHK_HOOK( int, PUTF_Function );
SHK_HOOK( int, PUTS_Function );
SHK_HOOK( int, FLW_AI_ACT_ATTACK );
SHK_HOOK( int, FLW_AI_ACT_SKILL );
SHK_HOOK( int, PERSONA_EVOLUTION );
SHK_HOOK( int, FUN_3b9644, int charID, int expressionID);
SHK_HOOK( s32, setSeq, s32 seqId, void* params, s32 paramsSize, s32 r6 );
SHK_HOOK( void, SetCountFunction, u32 a1, u32 a2 );
SHK_HOOK( int, GetCountFunction, u32 a1 );
SHK_HOOK( bool, scrGetCommandExist, u32 a1 );
SHK_HOOK( void, LoadSoundByCueIDCombatVoiceFunction, CueIDThingy* a1, u32* a2, u32 cueID, u8 idk );
SHK_HOOK( u32, scrGetCommandArgCount, u32 a1 );
SHK_HOOK( char*, scrGetCommandName, u32 a1 );
SHK_HOOK( scrCommandTableEntry*, scrGetCommandFunc, u32 id );
SHK_HOOK( undefined4*, LoadFutabaNaviBMD, void );
SHK_HOOK( undefined4*, LoadMonaNaviBMD, void );
SHK_HOOK( u64, LoadNaviSoundFile, u64 a1, u64 a2, char* acb_path, char* awb_path, u64 a5 );
SHK_HOOK( void, FUN_0006ccc8, void );

static s32 setSeqHook( s32 seqId, void* params, s32 paramsSize, s32 r6 )
{
  FUNC_LOG("Loading setSeqHook\n");
  sequenceIDGlobal = seqId;
  DEBUG_LOG( "Changing Sequence ID to %d\n", seqId );
  /*
  if ( seqId == 4 )
  {
    if ( paramsSize == 40 )
    {
      setSeqEnc* localStruct = (setSeqEnc*)params;
      EncounterIDGlobal = localStruct->encounterID;
      if ( EncounterIDGlobal > 1000 )
      {
        EncounterIDGlobal = 0;
      }
      //printf("Encounter ID Obtained -> %03d\n", EncounterIDGlobal);
    }
    else DEBUG_LOG("Unknown Params size in seqID 4 -> %d\n", paramsSize);
  }*/
  // Calling the original unhooked function is done like this.
  return SHK_CALL_HOOK( setSeq, seqId, params, paramsSize, r6 );
}

static int EX_FLW_setHumanLvHook( void )
{
  u32 unitID = FLW_GetIntArg(0);
  u32 lv = FLW_GetIntArg(1);
  SetUnitLv(unitID, lv);
  lv = GetJokerTargetLvExp(lv);
  SetUnitEXP(unitID, lv);
  return 1;
}

static int EX_FLW_GetEquippedPersona( void )
{
  u32 unitID = FLW_GetIntArg(0);
  if (unitID <= 10) // up to Kasumi
  {
    unitID = GetEquippedPersonaFunction(unitID);
    FLW_SetIntReturn(unitID);
  }
  return 1;
}

static int EX_FLW_ReturnEncounterID ( void )
{ 
  FLW_SetIntReturn(EncounterIDGlobal);
  return 1;
}

static int EX_FLW_GET_SEQ_ID ( void )
{ 
  FLW_SetIntReturn(sequenceIDGlobal);
  return 1;
}

static int PUT_FunctionHook( void )
{
  printf ("%d\n", FLW_GetIntArg(0));
  return 1;
}

static int PUTF_FunctionHook( void )
{
  printf ("%f\n", FLW_GetFloatArg(0));
  return 1;
}

static int PUTS_FunctionHook( void )
{
  printf ("%s\n", FLW_GetStringArg(0));
  return 1;
}

static int PERSONA_EVOLUTIONHook( void )
{
  int characterID = FLW_GetIntArg(0);
  if ( GetEquippedPersonaFunction( characterID ) > 210 ) // prevent evolution if persona is already evolved
  {
    return 1;
  }
  else return SHK_CALL_HOOK( PERSONA_EVOLUTION );
}

static int EX_FLW_PersonaEvolution( void )
{
  u32 unitID;
  u32 personaID;

  unitID = FLW_GetIntArg(0);
  if ( unitID == 0 || unitID == 1 || 11 <= unitID ) // do not execute on Joker or IDs higher than 10
  {
    return 1;
  }
  personaID = FLW_GetIntArg(1);
  GetBtlPlayerUnitFromID(unitID)->StockPersona[0].personaID = personaID;
  return 1;
}

#define ScriptInterpreter (*((ScriptInterpreter**)0xD59BFC))
static int EX_FLW_PRINTF( void ) //TGE made this into printf lol
{
  char* format = FLW_GetStringArg( 0 );
  u32 scriptArgIndex = 1;

  u64 args[10];
  double arg = 0.0;
  int argCount = 0;
  bool putNewLine = true;
  bool printAsFloat = false;

  if ( *format == '\\' )
  {
      char next = *( format + 1 );
      if ( next != 0 && next == 'n' && *( format + 2 ) == 0 ) // single \n
          putNewLine = false;
  }

  char* curFormat = format;
  for (; *curFormat != 0; ++curFormat )
  {
      char c = *curFormat;

      if ( c == '%' )
      {
          char next = *( ++curFormat );
          if ( next == 0 )
              break;

          // skip number of digits speifier
          while ( charIsDigit( next ) )
          {
              char next = *( ++curFormat );
              if ( next == 0 )
                  goto endloop;
          }

          switch ( next )
          {
          case 'c':
          case 's':
              args[ argCount++ ] = (u64)( FLW_GetStringArg( scriptArgIndex++ ) );
              break;

          case 'd':
          case 'i':
          case 'o':
          case 'x':
          case 'X':
          case 'u':
              args[ argCount++ ] = (u64)( FLW_GetIntArg( scriptArgIndex++ ) );
              break;

          case 'f':
          case 'F':
          case 'e':
          case 'E':
          case 'a':
          case 'A':
          case 'g':
          case 'G':
              printAsFloat = true;
              arg = (double)( FLW_GetFloatArg( scriptArgIndex++ ) );
              args[ argCount ] = *(u64*)( &arg );
              ++scriptArgIndex;
              ++argCount;
              break;
          }
      }
  }

  endloop:

  // Hack in some float prints for now
  if ( printAsFloat )
  {
    // Print everything as floats
    switch ( argCount )
    {
    case 0: printf( format ); break;
    case 1: printf( format, *(f64*)args[0]); break;
    case 2: printf( format, *(f64*)args[0], *(f64*)args[1]); break;
    case 3: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2]); break;
    case 4: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3]); break;
    case 5: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4]); break;
    case 6: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5]); break;
    case 7: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5], *(f64*)args[6]); break;
    case 8: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5], *(f64*)args[6], *(f64*)args[7]); break;
    case 9: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5], *(f64*)args[6], *(f64*)args[7], *(f64*)args[8]); break;
    case 10: printf( format, *(f64*)args[0], *(f64*)args[1], *(f64*)args[2], *(f64*)args[3], *(f64*)args[4], *(f64*)args[5], *(f64*)args[6], *(f64*)args[7], *(f64*)args[8], *(f64*)args[9]); break;
    }
  }
  else
  {
    // Print everything as ints
    switch ( argCount )
    {
    case 0: printf( format ); break;
    case 1: printf( format, args[0]); break;
    case 2: printf( format, args[0], args[1]); break;
    case 3: printf( format, args[0], args[1], args[2]); break;
    case 4: printf( format, args[0], args[1], args[2], args[3]); break;
    case 5: printf( format, args[0], args[1], args[2], args[3], args[4]); break;
    case 6: printf( format, args[0], args[1], args[2], args[3], args[4], args[5]); break;
    case 7: printf( format, args[0], args[1], args[2], args[3], args[4], args[5], args[6]); break;
    case 8: printf( format, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]); break;
    case 9: printf( format, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]); break;
    case 10: printf( format, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]); break;
    }
  }

  if ( putNewLine )
      printf( "\n" );

  ScriptInterpreter->mNumStackValues -= argCount;

  return 1;
}

static void SetCountFunctionHook( u32 COUNT, u32 VALUE )
{
  if ( COUNT >= 256 )
  {
    GlobalCounts[COUNT-256] = VALUE;
  }
  else SHK_CALL_HOOK( SetCountFunction, COUNT, VALUE );
}

static u32 GetCountFunctionHook( u32 COUNT )
{
  if ( COUNT >= 256 )
  {
    return GlobalCounts[COUNT-256];
  }
  else
  {
    u32 returnVal = SHK_CALL_HOOK( GetCountFunction, COUNT );
    return returnVal;
  }
}

static int EX_FLW_AI_ACT_PERSONA_SKILL( void )
{
  AI_UnitStruct* EnemyUnit = FLW_GetBattleUnitStructFromContext();
  DEBUG_LOG("Current AI Flowscript struct at 0x%x\n", EnemyUnit);
  if ( CONFIG_ENABLED( enablePersonaEnemies ) )
  {
    EnemyPersona = FLW_GetIntArg(0);
  }
  else EnemyPersona = 0;

  if ( EnemyPersona > 0 ) DEBUG_LOG("AI_ACT_PERSONA_SKILL Persona ID %03d with skill ID %03d\n", EnemyPersona, FLW_GetIntArg(1));

  EnemyUnit->ActSkillID = FLW_GetIntArg(1);
  EnemyUnit->Act_Type = 1;
  return 1;
}

static int FLW_AI_ACT_SKILLHook( void )
{
  EnemyPersona = 0;
  SummonCustomBED = 0;
  return SHK_CALL_HOOK(FLW_AI_ACT_SKILL);
}

static int FLW_AI_ACT_ATTACKHook( void )
{
  EnemyPersona = 0;
  SummonCustomBED = 0;
  return SHK_CALL_HOOK(FLW_AI_ACT_ATTACK);
}

/*static int EX_FLD_MODEL_ANIM(void)
{
  u32 uVar1;
  int reshnd;
  ResourceHandleStruct *pMVar4;
  int animIndex;
  int isLoop;
  double unk;
  fieldworkdataStruct *pFVar5;
  fieldworkdataStruct *pFVar6;
  u64 uVar2;
  ResourceHandleStruct *pMVar7;
  undefined8 uVar3;
  ResourceHandleStruct *pMVar8;
  bool bVar9;
  bool bVar10;
  int *piVar11;
  double animSpeed;
  
  reshnd = FLW_GetIntArg(0);
  pMVar4 = FUN_00015c34(reshnd);
  if (pMVar4 != (ResourceHandleStruct *)0x0) {
    animIndex = FLW_GetIntArg(1);
    isLoop = FLW_GetIntArg(2);
    unk = FLW_GetFloatArg(3);
    animSpeed = FLW_GetFloatArg(4);
    __builtin_clz(isLoop);
    FUN_00014a38((u32)pMVar4, 1, (u32)((u64)animIndex & 0xffffffffU), unk / 30.0, 1.0);
    FUN_000148dc(animSpeed,(u64 *)pMVar4,0);
    pFVar5 = FUN_00352f40();
    pFVar6 = FUN_00352f40();
    if (((pFVar6 == (fieldworkdataStruct *)0x0) || (isLoop = FUN_003316e0((int)pFVar6), isLoop == 0)) &&
       (pFVar5 != (fieldworkdataStruct *)0x0)) {
      FUN_00324b70((u64 *)pFVar5->field1b8, (u64 *)pMVar4, (s64)animIndex & 0xffffffffU);
      uVar2 = FUN_0032c3d4((int)pFVar5);
      pMVar7 = (ResourceHandleStruct *)uVar2;
      if (pMVar7 == (ResourceHandleStruct *)0x0) {
        pMVar8 = (ResourceHandleStruct *)0x0;
      }
      else {
        pMVar8 = (ResourceHandleStruct *)0x0;
        if ((*(u64 *)&pMVar7->field10 & 0x80000) != 0) {
          pMVar8 = pMVar7;
        }
      }
      if (pMVar4 == pMVar8) {
        FUN_002ab314(pFVar5->fieldd4);
      }
      uVar1 = (u32)((u64)*(undefined8 *)pMVar4 >> 0x3a);
      animIndex = FUN_00320b1c((int)pFVar5);
      if ((animIndex != 0) && ((uVar1 == 2 || (uVar1 == 5)))) {
        uVar2 = FUN_002e1338((u64 *)pFVar5,(u64 *)pMVar4,'\0');
        piVar11 = (int *)uVar2;
        if (piVar11 != (int *)0x0) {
          animIndex = FUN_002d128c((int)piVar11,0x80000);
          if (animIndex == 0) {
            uVar2 = FUN_002d7b6c(piVar11);
            animIndex = (int)uVar2;
            if (animIndex < 5) {
              if (animIndex < 2) {
                return 1;
              }
            }
            else {
              if (animIndex < 7) {
                if (animIndex < 6) {
                  return 1;
                }
              }
              else {
                if (animIndex != 0x12) {
                  return 1;
                }
              }
            }
            FUN_002d7b18(piVar11);
          }
          else {
            FUN_002d7b18(piVar11);
            uVar3 = FUN_002d0870(piVar11);
            if ((int)uVar3 == 0) {
              uVar3 = FUN_002d08b4(piVar11);
              bVar9 = (int)uVar3 != 0;
              bVar10 = bVar9;
            }
            else {
              bVar9 = true;
              bVar10 = false;
            }
            FUN_0003b4b8((int)pMVar4,bVar9);
            FUN_0003b510((int)pMVar4,bVar10);
            FUN_002d9d00(piVar11);
          }
        }
      }
    }
  }
  return 1;
}*/

inline void scrPushInt( int val ) 
{
  ScriptInterpreter->mStackValues[ ScriptInterpreter->mNumStackValues ] = val;
  ScriptInterpreter->mStackValueTypes[ ScriptInterpreter->mNumStackValues++ ] = 0;
  ScriptInterpreter->mNumStackValues += 1;
}

inline void scrPushFloat( float val )
{
  ScriptInterpreter->mStackValues[ ScriptInterpreter->mNumStackValues ] = *(u32*)&val;
  ScriptInterpreter->mStackValueTypes[ ScriptInterpreter->mNumStackValues++ ] = 1;
  ScriptInterpreter->mNumStackValues += 1;
}

inline u32 scrGetCommandIntResult()
{
  return ScriptInterpreter->mREGValue;
}

inline f32 scrGetCommandFloatResult()
{
  return *(f32*)&ScriptInterpreter->mREGValue;
}

static TtyCmdStatus ttyAddCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  f32 a = floatParse( args[0] );
  f32 b = floatParse( args[1] );
  printf( "%f\n", a + b );
  return TTY_CMD_STATUS_OK;
}

typedef struct
{
  s16 fieldMajorId;
  s16 fieldMinorId;
  s16 envMajorId;
  s16 envMinorId;
  s16 field08;
  s16 field0a;
  s16 field0c;
  s16 field0e;
  s16 field10;
  s16 field12;
  s16 field14;
  s16 field16;
  s16 field18;
  s16 field1a;
  s16 field1c;
  s16 field1e;
} seqFieldParams;

static TtyCmdStatus ttyEchoCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  for ( u32 i = 0; i < argc; ++i )
  {
      printf( "%s ", args[i] );
  }

  printf( "\n" );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttySetBgmCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  s32 id = intParse( args[0] );

#if GAME_P5
  sndManPlaySfx( 0, 0, id, 0, -1, -1 );
#endif

  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttySetSeqCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  s32 id = intParse( args[0] );

#if GAME_P5
  if ( id == 3 )
  {
    seqFieldParams params = {};
    params.fieldMajorId = intParse( args[1] );
    params.fieldMinorId = intParse( args[2] );
    params.envMajorId = params.fieldMajorId;
    params.envMinorId = params.fieldMinorId;
    params.field08 = 0xb;
    params.field0c = -1;
    setSeq( id, &params, sizeof(seqFieldParams), 0 );
  }
  else
  {
    setSeq( id, NULL, 0, 0 );
  }
#endif

  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttySetCountCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 count = intParse( args[0] );
  if ( count > 512 )
  {
    *error = "Count should not be higher than 512";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  u32 value = intParse( args[1] );
  SetCountFunctionHook( count, value );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyGetCountCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 count = intParse( args[0] );
  if ( count > 512 )
  {
    *error = "Count should not be higher than 512";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  printf("GetCount %d returned %d\n", count, GetCountFunctionHook( count ));
  return TTY_CMD_STATUS_OK;
}

#define BIT_MAX 8959
static TtyCmdStatus ttyBITONCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 bit = intParse( args[0] );
  if ( bit > BIT_MAX )
  {
    *error = "Bit ID should not exceed 8959";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  SetBitflagState( bit, 1 );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyBITOFFCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 bit = intParse( args[0] );
  if ( bit > BIT_MAX )
  {
    *error = "Bit ID should not exceed 8959";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  SetBitflagState( bit, 0 );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyGetBITCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 bit = intParse( args[0] );
  if ( bit > BIT_MAX )
  {
    *error = "Bit ID should not exceed 8959";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  printf( "BIT_CHK Bit %d returned %d\n", bit, GetBitflagState( bit ) );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyHealHPCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  for (int i = 1; i <= 10; i++)
  {
    scrPushInt( i );
    BULLET_RECOVERY();
  }
  RECOVERY_ALL();
  return TTY_CMD_STATUS_OK;
}

void PartyIn( int unitID )
{
  int v0; // r30
  int i; // r30

  v0 = 0;
  while ( GetUnitIDFromPartySlot(v0) != unitID )
  {
    if ( ++v0 >= 4 )
    {
      for ( i = 0; i < 4; ++i )
      {
        if ( !GetUnitIDFromPartySlot(i) )
        {
          sub_24B274(i, unitID);
          return;
        }
      }
      sub_24B274(3, unitID);
      return;
    }
  }
  return;
}

void PartyOut( int unitID )
{
  u16 party2 = GetUnitIDFromPartySlot(1);
  u16 party3 = GetUnitIDFromPartySlot(2);
  u16 party4 = GetUnitIDFromPartySlot(3);

  for ( int partySlot = 1; partySlot <= 3; partySlot++ )
  {
    sub_24B274(partySlot, 0);
  }

  if ( party2 != unitID )
  {
    PartyIn(party2);
  }
  if ( party3 != unitID )
  {
    PartyIn(party3);
  }
  if ( party4 != unitID )
  {
    PartyIn(party4);
  }
}

static TtyCmdStatus ttyPartyInCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 unitID = intParse( args[0] );
  if ( unitID > 10 )
  {
    *error = "Party Member ID should not exceed 10";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  PartyIn( unitID );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyPartyOutCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 unitID = intParse( args[0] );
  if ( unitID > 10 )
  {
    *error = "Party Member ID should not exceed 10";
    return TTY_CMD_STATUS_INVALID_ARG;
  }
  PartyOut( unitID );

  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyGetEnemyBtlUnitCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  hexDump( "Joker BtlUnit", GetBtlPlayerUnitFromID(1), sizeof(btlUnit_Unit) );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyGetDays( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  printf( "Current amount of ingame days passed %d\n", GetTotalDays() );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyGetFieldWorkData( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  fieldworkdataStruct* FieldData = GetFieldWorkData();
  printf( "Current FieldWorkData struct is at 0x%x\n", FieldData );
  hexDump( "FieldWorkData", FieldData, sizeof(fieldworkdataStruct) );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyloadBFFile( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  char bfPath[64];
  sprintf( bfPath, "script/%s.bf", args[0]);
  fileHandleStruct* newBF = open_file( bfPath, 1 );
  u64 fsSyncResult = fsSync((int)newBF);
  printf("Executing BF script %s\n", newBF);
  scrRunScript(0, newBF->pointerToFile, newBF->bufferSize, 0);
  return TTY_CMD_STATUS_OK;
}

fileHandleStruct* FutabaNavi = 0;
static undefined4* LoadFutabaNaviBMDHook(void)
{
  FUNC_LOG("Loading LoadFutabaNaviBMDHook\n");
  idkman* pmVar1;
  int *pmVar2;
  
  pmVar1 = MallocAndReturn(0x34);
  pmVar2 = (int *)0x0;
  
  int customNaviID = GetCountFunctionHook( 9 );
  if ( customNaviID != 9 )
  {
    customNaviID = 8;
    SetCountFunctionHook( 9, customNaviID );
  }
  char naviPath[128];

  if ( FutabaNavi != (void*)0x0  )
  {
    semaphore_WaitPost((int)FutabaNavi);
    FutabaNavi = (void*)0x0;
  }

  sprintf(naviPath, "battle/message/navi_%02d.bmd", customNaviID);
  if (pmVar1 != (idkman*)0x0) {
    FutabaNavi = open_file( naviPath, 0 );
    u64 fsResult = fsSync((int)FutabaNavi);
    if ( fsResult == 1 && CONFIG_ENABLED( enableExternalNavi ) )
    {
      FUN_00747f48((undefined4 *)pmVar1, FutabaNavi->pointerToFile, customNaviID);
    }
    else FUN_00747f48((undefined4 *)pmVar1, 0x00df7fd4, 8); //original functionality
    pmVar1->ptr1 = 0x00ba76c0;
    pmVar2 = (int *)pmVar1;
  }
  return (undefined4 *)pmVar2;
}

static undefined4* LoadMonaNaviBMDHook(void)
{
  FUNC_LOG("Loading LoadMonaNaviBMDHook\n");
  idkman* pmVar1;
  int *pmVar2;
  
  pmVar1 = MallocAndReturn(0x34);
  pmVar2 = (int *)0x0;
  if (pmVar1 != (idkman*)0x0) {
    NaviTestFile = open_file( "battle/message/navi_03.bmd", 0 );
    u64 fsResult = fsSync((int)NaviTestFile);
    SetCountFunctionHook( 9, 3 );
    if ( fsResult == 1 && CONFIG_ENABLED( enableExternalNavi ) )
    {
      FUN_00747f48((undefined4 *)pmVar1, NaviTestFile->pointerToFile, 3);
    }
    else FUN_00747f48((undefined4 *)pmVar1, 0x00ba7568, 3); //original functionality
    pmVar1->ptr1 = 0x00ba7568;
    pmVar2 = (int *)pmVar1;
  }
  return (undefined4 *)pmVar2;
}

ResourceHandleStruct* ModelResourceHandle;
static TtyCmdStatus ttyTestModelResHndCmd( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  int ResHnd = intParse( args[0] );
  ModelResourceHandle = FUN_00015c34(ResHnd);
  hexDump("Model Resource Handle Struct", ModelResourceHandle, sizeof(ResourceHandleStruct));
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttySetEnemyPersona( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  int PersonaID = intParse( args[0] );
  EnemyPersona = PersonaID;
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyFadeIn( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 fadetype = intParse( args[0] );
  FadeInFunction( fadetype, 10 );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyFadeOut( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  u32 fadetype = intParse( args[0] );
  FadeOutFunction( fadetype, 10 );
  return TTY_CMD_STATUS_OK;
}

static TtyCmdStatus ttyCOMSEPLAY( TtyCmd* cmd, const char** args, u32 argc, char** error )
{
  COMSE_PLAY_struct* CurrentCOMSEPLAY = 0x00cff4e4;
  u32 cueID = intParse( args[0] );

  printf("COMSE_PLAY: Playing Sound ID %d\n", cueID);
  
  if (CurrentCOMSEPLAY != 0x0) 
  {
    CurrentCOMSEPLAY->Field18 = cueID;
    CurrentCOMSEPLAY->Field1C = sndManPlaySfx( CurrentCOMSEPLAY->Field04, CurrentCOMSEPLAY->Field08, cueID, 0, -1, 1 );

    * (int *) 0x00cff4d4 = CurrentCOMSEPLAY->Field1C; // how to write to a specific address
  }
  else printf("COMSE_PLAY: Invalid struct at 0x%x\n", CurrentCOMSEPLAY);
  
  return TTY_CMD_STATUS_OK;
}

static u64 LoadNaviSoundFileHook( u64 a1, u64 a2, char* acb_path, char* awb_path, u64 a5 )
{
  FUNC_LOG("Loading LoadNaviSoundFileHook\n");
  char new_acb_path[128];
  char new_awb_path[128];

  int naviID = GetCountFunctionHook(9);

  if ( strcmp( acb_path, "sound/battle/spt02.acb" ) == 0 && naviID != 8 )
  {
    sprintf( new_acb_path, "sound/battle/spt%02d.acb", naviID );
    sprintf( new_awb_path, "sound/battle/spt%02d.awb", naviID );
    return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, new_acb_path, new_awb_path, a5);
  }
  else if ( strcmp( acb_path, "sound_JP/battle/spt02.acb" ) == 0 && naviID != 8 )
  {
    sprintf( new_acb_path, "sound_jp/battle/spt%02d.acb", naviID );
    sprintf( new_awb_path, "sound_jp/battle/spt%02d.awb", naviID );
    return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, new_acb_path, new_awb_path, a5);
  }
  return SHK_CALL_HOOK(LoadNaviSoundFile, a1, a2, acb_path, awb_path, a5);
}

static void LoadDLCBGM( void )
{
  FUNC_LOG("Loading LoadDLCBGM\n");
  
  if ( !CONFIG_ENABLED( ambushOverDLC ) )
  {
    isAmbush = false;
    isAmbushed = false;
  }
  
  char new_acb_path[128];
  char new_awb_path[128];
  u32 targetBGMACBID = 0;

  if ( CONFIG_ENABLED( randomDLCBGM ) ) // randomized DLC music
  {
    targetBGMACBID = randomIntBetween( 1, CONFIG_INT( maxDLCBGM ) );
  }
  else
  {
    u32 btlEquipBgmTableEntryCount = sizeof( btlEquipBgmTable ) / sizeof( btlEquipBgmTableEntry ); // Load DLC Outfit Table
    u32 playerOutfitModel = PlayerUnitGetModelMinorID( 1, 50, 0 ); // Get Joker's model ID

    for ( u32 i = 0; i < btlEquipBgmTableEntryCount; ++i )
    {
      btlEquipBgmTableEntry* pEntry = &btlEquipBgmTable[i]; // Loop through DLC Outfits table
      if ( pEntry->modelID == playerOutfitModel ) // if Joker's model matches an entry in the table, load that DLC music
      {
        targetBGMACBID = i + 1;
        break;
      }
    }

    int targetOutfitBGM = GetBtlPlayerUnitFromID( 1 )->outfitID - 0x7000;
    targetOutfitBGM = GetOutfitTBLEntry( targetOutfitBGM )->GearEffect[1];

    if ( targetOutfitBGM != 0 )
    {
      targetBGMACBID = targetOutfitBGM; // read outfit tbl value and use it as the ACB ID
    }
  }

  sprintf( new_acb_path, "sound/bgm_%02d.acb", targetBGMACBID );
  sprintf( new_awb_path, "sound/bgm_%02d.awb", targetBGMACBID );

  if ( strcmp( new_acb_path, DLCBGMDataLocation.dlc_acb_path ) == 0 )
  {
    DEBUG_LOG("Loading Previous DLC BGM Again, skipping\n");
    return;
  }

  LoadNaviSoundFileHook( 6, &DLCBGMDataLocation, new_acb_path, new_awb_path, 0 ); // this is the function that loads the actual DLC music
  FUN_0010fbbc( &DLCBGMDataLocation ); // what address did we store the data on

  u32 previousBGMID = GetCurrentBGMCueID();
  sndManPlaySfx( 0, 0, previousBGMID, 0, -1, -1 ); // attempt to fix dlc bgm dying if changing between acb with the current bgm missing

  return;
}

// List of commands that can be handled by the command listener
static TtyCmd ttyCommands[] =
{
  TTY_CMD( ttyAddCmd, "add", "Adds 2 numbers and prints the result", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "a", "The first number to add", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_FLOAT ), 
    TTY_CMD_PARAM( "b", "The second number to add", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_FLOAT )),

  TTY_CMD( ttyEchoCmd, "echo", "Prints the given input back to you", TTY_CMD_FLAG_VARARGS ),

  TTY_CMD( ttySetBgmCmd, "setbgm", "Sets the current BGM using the Cue ID", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "cueId", "The Cue ID of the BGM to play", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttySetCountCmd, "setCount", "Sets the specificed COUNT to the specified value", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "count", "COUNT number to modify", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT ), 
    TTY_CMD_PARAM( "value", "Target value for COUNT", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyGetCountCmd, "getCount", "Returns the value of the specified COUNT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "count", "COUNT to get value from", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyBITONCmd, "biton", "Enables the specified BIT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "bit", "BIT to enable", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyBITOFFCmd, "bitoff", "Disables the specified BIT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "bit", "BIT to disable", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyGetBITCmd, "bitget", "Returns the current state of the specified BIT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "bit", "BIT to get status from", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyGetBITCmd, "bitchk", "Returns the current state of the specified BIT", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "bit", "BIT to get status from", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyPartyInCmd, "partyin", "Adds a party member to current active party", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "playerID", "ID of party member to add", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyPartyOutCmd, "partyout", "Removes a party member from current active party", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "playerID", "ID of party member to remove", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyFadeIn, "fadein", "Plays a specific FadeIn", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "fadetype", "fade value", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyFadeOut, "fadeout", "Plays a specific FadeOut", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "fadetype", "fade value", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyCOMSEPLAY, "comseplay", "Plays a given Sound Effect", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "cue ID", "cue ID of sound to play", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyTestModelResHndCmd, "testmodel", "Test Resource handle function", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "int", "resource handle id", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttySetEnemyPersona, "setpersona", "Sets Enemy Persona when AI_ACT_PERSONA_SKILL is not used", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "int", "persona id", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_INT )),

  TTY_CMD( ttyloadBFFile, "loadbf", "Loads and executes a bf file", TTY_CMD_FLAG_NONE,
    TTY_CMD_PARAM( "bf name", "The Cue ID of the BGM to play", TTY_CMD_PARAM_FLAG_REQUIRED, TTY_CMD_PARAM_TYPE_STRING )),
    
  TTY_CMD( ttyGetDays, "getdays", "Get current amount of days passed", TTY_CMD_FLAG_NONE),

  TTY_CMD( ttyGetFieldWorkData, "getfieldworkdata", "Returns and prints current fieldworkdata struct", TTY_CMD_FLAG_NONE),

  TTY_CMD( ttyGetEnemyBtlUnitCmd, "getenemy", "Prints address and contents of currently saved enemy battle struct", TTY_CMD_FLAG_NONE ),

  TTY_CMD_END(), 
};

#ifdef GAME_P5

SHK_HOOK( u64, mainUpdate, f32 deltaTime );
static u64 mainUpdateHook( f32 deltaTime )
{
  if ( CONFIG_ENABLED (enableTTYCommands))
  {
    // Process TTY commands
    ttyCmdProcess( ttyCommands );
  }

  if ( CONFIG_ENABLED(enableRandomizerModule) ) {
    randomizerUpdate( deltaTime );
  }

  u64 result = SHK_CALL_HOOK( mainUpdate, deltaTime );

  return result;
}

#endif

static int EX_GET_LEARNABLE_SKILL( void )
{
  int partyMemberID = FLW_GetIntArg( 0 );
  if ( partyMemberID <= 1 || 11 <= partyMemberID ) // do not execute on Joker or IDs higher than 10
  {
    return 1;
  }

  PartyMemberPersonaBlock* playerPersona = GetPartyMemberPersonaBlock( GetEquippedPersonaFunction( partyMemberID ) );

  int returnValue = 0;
  int targetSkillEntry = FLW_GetIntArg( 1 );
  int previousLv = FLW_GetIntArg( 2 );
  DEBUG_LOG("EX_GET_LEARNABLE_SKILL: Attempting to learn skills starting from Lv %d\n", previousLv);
  btlUnit_Unit* Player = GetBtlPlayerUnitFromID( partyMemberID );

  if ( playerPersona->skills->canLearn && playerPersona->skills[targetSkillEntry].LvReq != 0 && targetSkillEntry <= 31 )
  {
    if ( playerPersona->skills[targetSkillEntry].LvReq >= previousLv && Player->StockPersona[0].personaLv >= playerPersona->skills[targetSkillEntry].LvReq )
    {
      returnValue = playerPersona->skills[targetSkillEntry].skillID;
      DEBUG_LOG("EX_GET_LEARNABLE_SKILL: Returning Skill ID %d\n", returnValue);
    }
  }
  FLW_SetIntReturn( returnValue );
  return 1;
}

static int EX_GET_PLAYER_LV( void )
{
  int partyMemberID = FLW_GetIntArg( 0 );
  if ( partyMemberID == 0 || 11 <= partyMemberID ) // do not execute on Joker or IDs higher than 10
  {
    return 1;
  }
  
  btlUnit_Unit* Player = GetBtlPlayerUnitFromID( partyMemberID );
  if ( partyMemberID == 1)
  {
    DEBUG_LOG("GET_PLAYER_LV: Joker is lv %d\n", Player->Joker_Lv);
    FLW_SetIntReturn( Player->Joker_Lv );
  }
  else
  {
    FLW_SetIntReturn( Player->StockPersona[0].personaLv );
    DEBUG_LOG("GET_PLAYER_LV: Unit ID %d is lv %d\n", partyMemberID, Player->StockPersona[0].personaLv);
  }

  return 1;
}

static int EX_SET_TACTICS_STATE( void )
{
  int partyMemberID = FLW_GetIntArg( 0 );
  if ( partyMemberID == 0 || 11 <= partyMemberID ) // do not execute on ID 0 or IDs higher than 10
  {
    return 1;
  }
  int tacticsID = FLW_GetIntArg( 1 );
  btlUnit_Unit* Player = GetBtlPlayerUnitFromID( partyMemberID );
  if ( partyMemberID == 1)
  {
    DEBUG_LOG("EX_SET_TACTICS_STATE: Warning, Joker tactics have been changed!");
  }

  Player->TacticsState = tacticsID;
  DEBUG_LOG("EX_SET_TACTICS_STATE: Unit ID %d tactics set to %d\n", partyMemberID, tacticsID);

  return 1;
}

static int EX_CALL_NAVI_DIALOGUE( void )
{
  int charID = FLW_GetIntArg(0);
  int expressionID = FLW_GetIntArg(1);
  int msgID = FLW_GetIntArg(2);
  int dialogueBoxType = FLW_GetIntArg(3);
  if ( FUN_0031f9cc() && (charID = isCharacterAssistExpressonValid( (short)charID, (short)expressionID ), -1 < charID))
  {
    FUN_003b9110( charID, msgID, dialogueBoxType, 0, 0 );
  }
  return 1;
}

/*int sVanillaBits = { 0, 2048, 4096, 8192, 8448, 8704, 8960 };
int[] sRoyalBits = { 0, 3072, 6144, 11264, 11776, 12288, 12800 };

static int ConvBit(int flag, int[] source, int[] target) 
{
    var section = -1;
    var section_flag = 0;

    // convert
    for (var i = 1; i < source.Length; i++)
    {
        if (flag < source[i])
        {
            section = i - 1;
            section_flag = flag - source[i - 1];
            break;
        }
    }

    var result = target[section] + section_flag;

    // flag val exceeded max val in source array
    if (section < 0) return -1;

    // overflowed to next section after conversion
    if (result > target[section + 1]) return -1;

    return result;
}

int ConvBitV2R(int flag) => ConvBit(flag, sVanillaBits, sRoyalBits);

int ConvBitR2V(int flag) => ConvBit(flag, sRoyalBits, sVanillaBits);

static int EX_CMM_FLAG_CONVERT( void )
{
  int flag = FLW_GetIntArg( 0 );
  if ( flag >= 0x10000000 )
  {
    flag = sRoyalBits[ flag >> 28 ] + flag & 0x0fffffff; // convert giant p5r flag to readable p5r flag
  }

  int unkVar = FLW_GetIntArg( 1 );

  FLW_SetIntReturn( flag + unkVar );
  return 1;
}*/

static int EX_CLEAR_PERSONA_SKILLS( void )
{
  int charID = FLW_GetIntArg(0);

  if ( 0 > charID || charID > 11 )
  {
    return 1;
  }

  btlUnit_Unit* PlayerUnit = GetBtlPlayerUnitFromID( charID );
  for ( int i = 0; i < 8; i++ )
  {
    PlayerUnit->StockPersona[0].SkillID[i] = 0;
  }
  return 1;
}

static int EX_PLAYER_HAS_INVALID_SKILL( void )
{
  int total = 0;
  for( int i = 0; i <= 3; i++ )
  {
    btlUnit_Unit* PartyMember = GetBtlPlayerUnitFromID( GetUnitIDFromPartySlot(i) );
    /*if ( PartyMember->accessoryID == 232 + 0x2000 ) // Omnipotent Orb
    {
      total += 2;
    }
    if ( GetBtlUnitMaxHP(PartyMember) < PartyMember->currentHP )
    {
      total += 2;
    }
    if ( GetBtlUnitMaxSP(PartyMember) < PartyMember->currentSP )
    {
      total += 2;
    }*/
    if ( BtlUnitCheckHasSkill( PartyMember, 600 ) || // Twins Down Attack
         BtlUnitCheckHasSkill( PartyMember, 603 ) || // 9999 Megidolaon
         BtlUnitCheckHasSkill( PartyMember, 604 ) || // Rays of Control
         BtlUnitCheckHasSkill( PartyMember, 605 ) || // Rays of Control
         BtlUnitCheckHasSkill( PartyMember, 606 ) || // Rays of Control
         BtlUnitCheckHasSkill( PartyMember, 607 ) || // Rays of Control
         BtlUnitCheckHasSkill( PartyMember, 608 ) || // Arrow of Light
         BtlUnitCheckHasSkill( PartyMember, 612 ) || // Eternal Light
         BtlUnitCheckHasSkill( PartyMember, 636 ) || // Rays of Control
         BtlUnitCheckHasSkill( PartyMember, 638 ) || // Will of the People
         BtlUnitCheckHasSkill( PartyMember, 405 ) || // Down Shot
         BtlUnitCheckHasSkill( PartyMember, 658 ) || // Down Shot
         BtlUnitCheckHasSkill( PartyMember, 659 ) || // Down Shot
         BtlUnitCheckHasSkill( PartyMember, 660 ) || // Down Shot
         BtlUnitCheckHasSkill( PartyMember, 768 ) || // Hax Brave Blade
         BtlUnitCheckHasSkill( PartyMember, 434 ) || // Sup All Kaja (Futaba Skill)
         BtlUnitCheckHasSkill( PartyMember, 450 ) || // Libido Boost
         BtlUnitCheckHasSkill( PartyMember, 465 ) || // March of the Piggy
         BtlUnitCheckHasSkill( PartyMember, 469 ) || // Sphinx Dive
         BtlUnitCheckHasSkill( PartyMember, 481 ) || // Berserker Dance
         BtlUnitCheckHasSkill( PartyMember, 477 ) || // Penalty
         BtlUnitCheckHasSkill( PartyMember, 490 ) || // Arm of Destruction
         BtlUnitCheckHasSkill( PartyMember, 493 ) || // Cannon Fire
         BtlUnitCheckHasSkill( PartyMember, 494 ) || // Cannon Barrage
         BtlUnitCheckHasSkill( PartyMember, 496 ) || // Pyramid Blast
         BtlUnitCheckHasSkill( PartyMember, 1501 ) ) // Myriad Truths
    {
      total += 9;
    }
  }

  if ( total >= 7 )
  {
    FLW_SetIntReturn( 1 );
  }
  else FLW_SetIntReturn( 0 );
  
  return 1;
}

static int EX_AI_SUMMON_UNITS( void )
{
  AI_UnitStruct* EnemyUnit = FLW_GetBattleUnitStructFromContext();
  int enemyID1 = FLW_GetIntArg( 0 );
  int enemyID2 = FLW_GetIntArg( 1 );
  int enemyID3 = FLW_GetIntArg( 2 );
  int enemyID4 = FLW_GetIntArg( 3 );
  SummonCustomBED = FLW_GetIntArg( 4 );

  EnemyUnit->Act_Type = 1;
  EnemyUnit->ActSkillID = 0x196;

  DEBUG_LOG("AI_ACT_SUMMON_UNITS called!\n");

  EnemyUnit->argArray[EnemyUnit->argCount] = enemyID1;
  EnemyUnit->argCount += 1;
  
  if ( enemyID2 > 0 )
  {
    EnemyUnit->argArray[EnemyUnit->argCount] = enemyID2;
    EnemyUnit->argCount += 1;
  }
  else return 1;
  
  if ( enemyID3 > 0 )
  {
    EnemyUnit->argArray[EnemyUnit->argCount] = enemyID3;
    EnemyUnit->argCount += 1;
  }
  else return 1;
  
  if ( enemyID4 > 0 )
  {
    EnemyUnit->argArray[EnemyUnit->argCount] = enemyID4;
    EnemyUnit->argCount += 1;
  }
  else return 1;

  return 1;
}

static int EX_AI_SET_TARGETABLE_STATE( void )
{
  int state = FLW_GetIntArg( 0 );
  AI_CHK_SLIP(); // needed to store btlUnit struct of current acting enemy
  if ( state > 0 )
  {
    state = 1;
  }

  enemyBtlUnit->Flags = (enemyBtlUnit->Flags & ~(1UL << 15)) | (state << 15);
  
  return 1;
}

static int EX_AI_SET_ENID_TARGETABLE_STATE( void )
{
  int enemyID = FLW_GetIntArg( 0 );
  int state = FLW_GetIntArg( 1 );

  btlUnit_Unit* ENID_enemyBtlUnit = GetBtlUnitInCombat( 2, enemyID );
  if ( ENID_enemyBtlUnit == 0 )
  {
    return 1;
  }

  if ( state > 0 )
  {
    state = 1;
  }

  DEBUG_LOG("Setting Target status of enemy ID %d to %d\n", enemyID, state);

  ENID_enemyBtlUnit->Flags = (ENID_enemyBtlUnit->Flags & ~(1UL << 15)) | (state << 15);
  
  return 1;
}

static int EX_AI_SET_ENID_ENDURE_STATE( void )
{
  int enemyID = FLW_GetIntArg( 0 );
  int state = FLW_GetIntArg( 1 );

  btlUnit_Unit* ENID_enemyBtlUnit = GetBtlUnitInCombat( 2, enemyID );
  if ( ENID_enemyBtlUnit == 0 )
  {
    return 1;
  }

  if ( state > 0 )
  {
    state = 1;
  }

  ENID_enemyBtlUnit->Flags = (ENID_enemyBtlUnit->Flags & ~(1UL << 5)) | (state << 5);
  
  return 1;
}

static int EX_SET_JOKER_ENDURE_STATE( void )
{
  int state = FLW_GetIntArg( 0 );
  if ( state > 0 )
  {
    state = 1;
  }
  btlUnit_Unit* Joker = GetBtlPlayerUnitFromID( 1 );

  Joker->Flags = (Joker->Flags & ~(1UL << 5)) | (state << 5);
  
  return 1;
}

static int EX_SET_ENID_STATS( void )
{
  int enemyID = FLW_GetIntArg( 0 );
  if (enemyID > 999) //
  {
    return 1;
  }
  DEBUG_LOG("EX_SET_ENID_STATS Called\n");
  DEBUG_LOG("Original stats for Enemy ID %d; %d %d %d %d %d\n", enemyID, 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[0], 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[1], 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[2], 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[3], 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[4] );
  u32 targetStats[5];
  for ( int i = 1; i <= 5; i++ )
  {
    NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[i - 1] = FLW_GetIntArg( i );
  }
  DEBUG_LOG("New stats for Enemy ID %d; %d %d %d %d %d\n", enemyID, 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[0], 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[1], 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[2], 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[3], 
  NewEnemyStatsTBL.EnemyStats[enemyID].stats.stat[4] );
  
  return 1;
}

static int EX_GET_PLAYER_MAX_HP( void )
{
  u32 unitID = FLW_GetIntArg(0);
  if (unitID <= 10) // up to Kasumi
  {
    FLW_SetIntReturn( GetBtlUnitMaxHP( GetBtlPlayerUnitFromID( unitID ) ) );
  }
  else FLW_SetIntReturn( 0 );
  return 1;
}

static int EX_SET_ENID_MAX_HP( void )
{
  u32 enemyID = FLW_GetIntArg(0);
  if (enemyID > 999) //
  {
    return 1;
  }
  u32 NewMaxHP = FLW_GetIntArg(1);
  NewEnemyStatsTBL.EnemyStats[enemyID].MaxHP = NewMaxHP;
  return 1;
}

static int EX_SET_ENID_TACTICS( void )
{
  u32 enemyID = FLW_GetIntArg(0);
  if (enemyID > 999) //
  {
    return 1;
  }
  int state = FLW_GetIntArg( 1 );
  
  btlUnit_Unit* ENID_enemyBtlUnit = GetBtlUnitInCombat( 2, enemyID );
  if ( ENID_enemyBtlUnit == 0 )
  {
    return 1;
  }

  ENID_enemyBtlUnit->TacticsState = state;
  
  return 1;
}

static int EX_PLAYER_HAS_SKILL( void )
{
  FLW_SetIntReturn( BtlUnitCheckHasSkill( GetBtlPlayerUnitFromID( FLW_GetIntArg( 0 ) ), FLW_GetIntArg( 1 ) ) );
  
  return 1;
}

static int EX_GET_PERSONA_INHERITANCE_TYPE( void )
{
  FLW_SetIntReturn( PersonaTBL_GetPersonaStatsSegment0( FLW_GetIntArg( 0 ) )->InheritType );
  
  return 1;
}

static int EX_SET_ENEMY_SKILL( void )
{
  int enemyID = FLW_GetIntArg( 0 );
  int skillSlot = FLW_GetIntArg( 1 );
  int skillID = FLW_GetIntArg( 2 );

  NewEnemyStatsTBL.EnemyStats[enemyID].Skills[skillSlot] = skillID;
  
  return 1;
}

static int EX_PREVENT_PLAYER_LOSS( void )
{
  isPreventGameOver = true;

  return 1;
}

scrCommandTableEntry exCommandTable[] =
{
  { EX_FLW_PRINTF, 1, "EX_PRINTF" },
  { EX_FLW_AI_ACT_PERSONA_SKILL, 2, "AI_ACT_PERSONA_SKILL" },
  { EX_FLW_ReturnEncounterID, 0, "GET_ENCOUNTER_ID" },
  { EX_FLW_GetEquippedPersona, 1, "GET_EQUIPPED_PERSONA" },
  { EX_FLW_PersonaEvolution, 2, "PERSONA_EVOLUTION2" },
  { EX_FLW_GET_SEQ_ID, 0, "GET_SEQ_ID" },
  { EX_GET_LEARNABLE_SKILL, 3, "GET_LEARNABLE_SKILL" },
  { EX_GET_PLAYER_LV, 1, "GET_PLAYER_LV" },
  { EX_SET_TACTICS_STATE, 2, "SET_TACTICS_STATE" },
  { EX_CALL_NAVI_DIALOGUE, 4, "CALL_NAVI_DIALOGUE" },
  { EX_CLEAR_PERSONA_SKILLS, 1, "CLEAR_PERSONA_SKILLS" },
  { EX_PLAYER_HAS_INVALID_SKILL, 0, "PLAYER_HAS_INVALID_SKILL" },
  { EX_AI_SUMMON_UNITS, 5, "AI_ACT_SUMMON_UNITS" },
  { EX_AI_SET_TARGETABLE_STATE, 1, "AI_SET_TARGETABLE_STATE" },
  { EX_AI_SET_ENID_TARGETABLE_STATE, 2, "AI_SET_ENID_TARGETABLE_STATE" },
  { EX_AI_SET_ENID_ENDURE_STATE, 2, "AI_SET_ENID_ENDURE_STATE" },
  { EX_SET_JOKER_ENDURE_STATE, 1, "SET_JOKER_ENDURE_STATE" },
  { EX_SET_ENID_STATS, 6, "SET_ENID_STATS" },
  { EX_GET_PLAYER_MAX_HP, 1, "GET_PLAYER_MAX_HP" },
  { EX_SET_ENID_MAX_HP, 2, "SET_ENID_MAX_HP" },
  { EX_SET_ENID_TACTICS, 2, "SET_ENID_TACTICS" },
  { EX_PLAYER_HAS_SKILL, 2, "PLAYER_HAS_SKILL" },
  { EX_GET_PERSONA_INHERITANCE_TYPE, 1, "GET_PERSONA_INHERITANCE_TYPE" },
  { EX_SET_ENEMY_SKILL, 3, "SET_ENEMY_SKILL" },
  { EX_PREVENT_PLAYER_LOSS, 0, "PREVENT_PLAYER_LOSS" },
};

static scrCommandTableEntry* scrGetCommandFuncHook( u32 id )
{
  // DEBUG_LOG("scrGetCommandFunc called on function ID 0x%04x\n", id);
  if ( id >= 0x6000 )
  {
    // DEBUG_LOG("function ID 0x%x called\nName %s\nnumOfArgs %02d\n",
    // id, exCommandTable[id - 0x6000].name, 
    // exCommandTable[id - 0x6000].argCount);
    return exCommandTable[id & 0x0FFF].function;
  }
  else
  {
    return SHK_CALL_HOOK(scrGetCommandFunc, id);
  }
}

static bool scrGetCommandExistHook( u32 functionID )
{
  // DEBUG_LOG("scrGetCommandExist called on function ID 0x%04x\n", functionID);
  if ( functionID >= 0x6000 )
  {
    return true;
  }
  else
  {
    return SHK_CALL_HOOK(scrGetCommandExist, functionID);
  }
}

static char* scrGetCommandNameHook( u32 functionID )
{
  // DEBUG_LOG("scrGetCommandName called on function ID 0x%04x\n", functionID);
  if ( functionID >= 0x6000 )
  {
    return exCommandTable[functionID & 0x0FFF].name;
  }
  else
  {
    return SHK_CALL_HOOK(scrGetCommandName, functionID);
  }
}

static u32 scrGetCommandArgCountHook( u32 functionID )
{
  // DEBUG_LOG("scrGetCommandArgCount called on function ID 0x%04x\n", functionID);
  if ( functionID >= 0x6000 )
  {
    return exCommandTable[functionID & 0x0FFF].argCount;
  }
  else
  {
    return SHK_CALL_HOOK(scrGetCommandArgCount, functionID);
  }
}

static int FUN_3b9644Hook(int charID, int expressionID)
{
  //FUNC_LOG("Loading FUN_3b9644Hook\n");
  if ( charID == 9 && GetEquippedPersonaFunction(9) != Persona_RobinHood && CONFIG_ENABLED( enableAkechiMod ) )
  {
    expressionID += 100;
  }
  return SHK_CALL_HOOK(FUN_3b9644, charID, expressionID);
}

char UnitTypeNames[4][7] = { "Null", "Player", "Enemy", "Persona" };

static void LoadSoundByCueIDCombatVoiceFunctionHook( CueIDThingy* a1, u32* a2, u32 cueID, u8 idk )
{
  FUNC_LOG("Loading LoadSoundByCueIDCombatVoiceFunctionHook\n");
  DEBUG_LOG( "%s ID %d is using Voice Cue ID %d\n", UnitTypeNames + a1->Field10->btlUnitPointer->unitType, a1->Field10->btlUnitPointer->unitID, cueID );

  if ( a1->Field10->btlUnitPointer->unitType == 1 ) // player unit
  {
    PartyMemberCombatVoiceInstance[a1->Field10->btlUnitPointer->unitID] = a1; // store in global array for later use
    PartyMemberCombatVoiceInstanceB[a1->Field10->btlUnitPointer->unitID] = a2; // store in global array for later use
  }
  
  return SHK_CALL_HOOK( LoadSoundByCueIDCombatVoiceFunction, a1, a2, cueID, idk );
}

static int EX_SET_PERSONA_LV ( void )
{
  int partyMemberID = FLW_GetIntArg( 0 );
  //printf("EX_SET_PERSONA_LV called on unitID %d\n", partyMemberID);
  if ( partyMemberID <= 1 || 11 <= partyMemberID ) // do not execute on Joker or IDs higher than 10
  {
    return 1;
  }

  int targetLv = FLW_GetIntArg( 1 );
  //printf("EX_SET_PERSONA_LV called, target lv %d\n", targetLv);
  if ( targetLv <= 0 || 100 <= targetLv ) // do not execute if target lv is 0 or higher than 100
  {
    return 1;
  }

  int ExpNeeded = 0;

  PartyMemberLvUpThresholdExp* ExpThreshold = GetPartyMemberLvUpThreshold( partyMemberID );

  if ( targetLv > 1 )
  {
    ExpNeeded = ExpThreshold->ExpRequired[targetLv-2];
  }
  //printf("EX_SET_PERSONA_LV called, target exp -> %d\n", ExpNeeded);

  btlUnit_Unit* playerUnit = GetBtlPlayerUnitFromID( partyMemberID );
  int unitLv = playerUnit->StockPersona[0].personaLv;

  if ( unitLv >= targetLv )
  {
    DEBUG_LOG("EX_SET_PERSONA_LV: Unit is already at target level or higher!\n");
    return 1;
  }
  playerUnit->StockPersona[0].personaLv = targetLv;
  playerUnit->StockPersona[0].personaExp = ExpNeeded;
  PartyMemberPersonaBlock* playerPersona = GetPartyMemberPersonaBlock( GetEquippedPersonaFunction( partyMemberID ) );

  u32 targetStats[5];

  for ( int i = 0; i <= 4; i++ )
  {
    targetStats[i] = playerUnit->StockPersona[0].Stats[i];
  }

  for ( int i = ( unitLv - 1 ); i < targetLv - 1; i++ )
  {
    for ( int j = 0; j <= 4; j++ )
    {
      targetStats[j] += playerPersona->stats[i].stat[j];
    }
  }

  for ( int i = 0; i <= 4; i++ )
  {
    playerUnit->StockPersona[0].Stats[i] = targetStats[i];
  }
  
  return 1;
}

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot,
// hook a function that is called after initialisation.
void EXFLWInit( void )
{
  randomSetSeed( getTicks() );
  printf("Expanded Flowscript Module loaded.\n");
  // Hooks must be 'bound' to a handler like this in the start function.
  // If you don't do this, the game will crash.
  SHK_BIND_HOOK( EX_FLW_setHumanLv, EX_FLW_setHumanLvHook );
  SHK_BIND_HOOK( SET_PERSONA_LV, EX_SET_PERSONA_LV );
  SHK_BIND_HOOK( PUT_Function, PUT_FunctionHook );
  SHK_BIND_HOOK( PUTF_Function, PUTF_FunctionHook );
  SHK_BIND_HOOK( PUTS_Function, PUTS_FunctionHook );
  SHK_BIND_HOOK( setSeq, setSeqHook );
  SHK_BIND_HOOK( SetCountFunction, SetCountFunctionHook );
  SHK_BIND_HOOK( GetCountFunction, GetCountFunctionHook );
  SHK_BIND_HOOK( PERSONA_EVOLUTION, PERSONA_EVOLUTIONHook );
  SHK_BIND_HOOK( FLW_AI_ACT_SKILL, FLW_AI_ACT_SKILLHook );
  SHK_BIND_HOOK( FLW_AI_ACT_ATTACK, FLW_AI_ACT_ATTACKHook );
  SHK_BIND_HOOK( scrGetCommandFunc, scrGetCommandFuncHook );
  SHK_BIND_HOOK( scrGetCommandExist, scrGetCommandExistHook );
  SHK_BIND_HOOK( scrGetCommandName, scrGetCommandNameHook );
  SHK_BIND_HOOK( scrGetCommandArgCount, scrGetCommandArgCountHook );
  SHK_BIND_HOOK( FUN_3b9644, FUN_3b9644Hook );
  SHK_BIND_HOOK( LoadSoundByCueIDCombatVoiceFunction, LoadSoundByCueIDCombatVoiceFunctionHook );
  // Handle command handling in main update function
  SHK_BIND_HOOK( mainUpdate, mainUpdateHook );
  // Load Custom Navigator file
  SHK_BIND_HOOK( LoadFutabaNaviBMD, LoadFutabaNaviBMDHook );
  SHK_BIND_HOOK( LoadMonaNaviBMD, LoadMonaNaviBMDHook );
  SHK_BIND_HOOK( LoadNaviSoundFile, LoadNaviSoundFileHook );
  SHK_BIND_HOOK( FUN_0006ccc8, LoadDLCBGM );

  memset( &DLCBGMDataLocation, 0x0, sizeof( DLCBGMStruct ) );
  DEBUG_LOG("DLC BGM Data address is 0x%x\n", &DLCBGMDataLocation);
}

void EXFLWShutdown( void )
{
  // Executed when the PRX module is unloaded.    
}
#endif