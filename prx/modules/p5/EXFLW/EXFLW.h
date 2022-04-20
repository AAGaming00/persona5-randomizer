#ifdef GAME_P5
#ifndef EXFLW_H
#define EXFLW_H

typedef struct
{
    void* function;
    u32 argCount;
    const char* name;
} scrCommandTableEntry;

typedef struct
{
    int Field00;
    int Field04;
    int Field08;
    int Field0C;
    int Field10;
    int Field14;
    int Field18;
    int Field1C;
}COMSE_PLAY_struct;

typedef struct
{
    f32 Field00;
    f32 Field04;
    f32 Field08;
    f32 Field0C;
    f32 Field10;
    f32 Field14;
    f32 Field18;
    f32 Field1C;
    f32 Field20;
    f32 Field24;
    f32 Field28;
    f32 Field2C;
    f32 Field30;
    f32 Field34;
    f32 Field38;
}Type8;



void EXFLWInit( void );
void EXFLWShutdown( void );

void PartyIn( int unitID );
void PartyOut( int unitID );

#endif
#endif