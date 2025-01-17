#ifdef GAME_P5
#ifndef KASUMI_H
#define KASUMI_H

typedef struct navi_dialogue_function_a2
{
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0c;
    u32 field10;
    u32 field14;
    u32 field18;
    u32 field1c;
    u32 field20;
    u32 field24;
    u32 field28;
    u32 field2c;
    u32 field30;
    u32 field34;
    u32 field38;
    u32 field3c;
    u32 field40;
    u32 field44;
    u32 field48;
    u32 field4c;
    u32 field50;
    u32 field54;
    u32 field58;
    u32 field5c;
    u32 field60;
    u32 field64;
    u32 field68;
    u32 field6c;
    u32 field70;
    u32 field74;
    u32 field78;
    u32 field7c;
    u32 field80;
    u32 field84;
    u32 field88;
    u32 field8c;
    u32 field90;
    u32 field94;
    u32 field98;
    u32 field9c;
    u32 fielda0;
    u32 fielda4;
    u32 fielda8;
    u32 fieldac;
    u32 fieldb0;
    u32 fieldb4;
    u32 fieldb8;
    u32 fieldbc;
    u32 fieldc0;
    u32 fieldc4;
    u32 fieldc8;
    u32 fieldcc;
    u32 fieldd0;
    u32 fieldd4;
    u32 fieldd8;
    u32 fielddc;
    u32 fielde0;
    u32 fielde4;
    u32 fielde8;
    u32 fieldec;
    u32 fieldf0;
    u32 fieldf4;
    u32 fieldf8;
    u32 fieldfc;
    u32 field100;
    u32 field104;
    u32 field108;
    u32 field10c;
    u32 field110;
    u32 field114;
    u32 field118;
    u32 field11c;
    u32 field120;
    u32 field124;
    u32 EncounterID;
    u32 field12c;
} navi_dialogue_function_a2;

typedef struct
{
    u16 partyMemberID[10];
}partyMemberMenu;

void CheckKasumiEquipment(btlUnit_Unit* a1);

void KasumiInit( void );
void KasumiShutdown( void );

#endif
#endif