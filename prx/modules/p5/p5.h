#ifdef GAME_P5
#ifndef P5_H
#define P5_H
#pragma pack(1)

// PS3 system includes
#include "lib/common.h"
#include "lib/shk.h"

// Globals
u16 EncounterIDGlobal;
u16 LastUsedEncounterID;
u16 sequenceIDGlobal;
u32 EnemyPersona;
u32 GlobalEnemyID;
u32 GlobalCounts[256];
bool isAmbush;
bool isAmbushed;
bool isMidwinter;
bool hasAkechiEndured;
bool hasSumiEndured;
bool isLoadScheduler;
bool randomizedCombatOutfit;
bool isPreventGameOver;
int rngBGM;
int FrameTiming;
float ColorNew;
int JokerModel;
u32 titleScreenBGM;
u16 ActiveGlobalSkillSlot;
u16 SummonCustomBED;
u16 KasumiModelSubID;

u16 LastLoadedFieldMajorID;
u16 LastLoadedFieldMinorID;

typedef struct
{
  u32 modelID;
  u32 bgmId;
} btlEquipBgmTableEntry;

typedef struct
{
  u32 isEnabled;
  u32 acbType;
  char dlc_acb_path[256];
  char dlc_awb_path[256];
}DLCBGMStruct;

DLCBGMStruct DLCBGMDataLocation;

typedef struct
{
  u32 Field00;     // 00
  u16 Flags;       // 04
  u16 cmmID;       // 06
  u16 cmmRank;     // 08
  u16 cmmAffinity; // 0A
  u32 Field0C;     // 0C
} ConfidantStats;

typedef struct
{
  ConfidantStats Confidants[24];
} ConfidantStatsStruct;

typedef struct
{
  u16 bitflags;
  u8  Arcana;
  u8  BaseLevel;
  u8  Stats[6];
  u16 InheritType;
  u16 Field0C;
} PersonaTBL_Segment0;


typedef struct GFDModelMaterial_Processed
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
  char* MaterialName;
  u32 field88;
  u32 field8c;
} GFDModelMaterial_Processed;


btlEquipBgmTableEntry btlEquipBgmTable[29];

short JokerModelIDs[33];
short SkullModelIDs[27];
short MonaModelIDs[21];
short PantherModelIDs[31];
short FoxModelIDs[26];
short QueenModelIDs[28];
short NoirModelIDs[27];
short OracleModelIDs[26];
short CrowModelIDs[26];
short VioletModelIDs[39];

int sVanillaBits[6];
int sRoyalBits[6];

typedef union
{
  struct
  {
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
    u8 bit8 : 1;
    u8 bit9 : 1;
    u8 bit10 : 1;
    u8 bit11 : 1;
    u8 NonNegotiable : 1;
    u8 bit13 : 1;
    u8 bit14 : 1;
    u8 bit15 : 1;
    u8 bit16 : 1;
    u8 PositionHack : 1;
    u8 bit18 : 1;
    u8 shadownotdisappearinbattle : 1;
    u8 BulletHailonstart : 1;
    u8 bit21 : 1;
    u8 bit22 : 1;
    u8 eventScript : 1;
    u8 bit24 : 1;
    u8 bit25 : 1;
    u8 formation : 1;
    u8 bit27 : 1;
    u8 NoCritical : 1;
    u8 bit29 : 1;
    u8 bit30 : 1;
    u8 TreasureSkimmer : 1;
  };
  u32 byte;
} encounterFlags;

typedef struct
{
  encounterFlags flags;
  u16 Field04;
  u16 Field06;
  u16 BattleUnitID[5];
  u16 FieldID;
  u16 RoomID;
  u16 BGMID;
} encounterIDTBL;


typedef struct
{
  u16 Field00;
  u16 Field02;
  u16 Field04;
  u16 encounterID;
} setSeqEnc;

typedef enum {
    Persona_000,
    Persona_Metatron,
    Persona_Beelzebub,
    Persona_CuChulainn,
    Persona_JackoLantern,
    Persona_JackFrost,
    Persona_Pixie,
    Persona_Cerberus,
    Persona_Lilim,
    Persona_Eligor,
    Persona_Odin,
    Persona_HuaPo,
    Persona_Decarabia,
    Persona_Mara,
    Persona_Ose,
    Persona_Thor,
    Persona_Unicorn,
    Persona_Uriel,
    Persona_Sarasvati,
    Persona_Valkyrie,
    Persona_Yaksini,
    Persona_Ganesha,
    Persona_Anubis,
    Persona_Mot,
    Persona_Raphael,
    Persona_Scathach,
    Persona_HighPixie,
    Persona_Barong,
    Persona_Girimehkala,
    Persona_KingFrost,
    Persona_Narcissus,
    Persona_Isis,
    Persona_Lamia,
    Persona_Legion,
    Persona_Rakshasa,
    Persona_Mokoi,
    Persona_Forneus,
    Persona_Setanta,
    Persona_Titania,
    Persona_Incubus,
    Persona_Oni,
    Persona_Lilith,
    Persona_Rangda,
    Persona_Makami,
    Persona_Parvati,
    Persona_Gabriel,
    Persona_ZaouGongen,
    Persona_Alice,
    Persona_Kali,
    Persona_KuramaTengu,
    Persona_Oberon,
    Persona_ShikiOuji,
    Persona_YamatanoOrochi,
    Persona_Orobas,
    Persona_Hanuman,
    Persona_Archangel,
    Persona_Obariyon,
    Persona_QueenMab,
    Persona_Sandalphon,
    Persona_Abaddon,
    Persona_Shiisaa,
    Persona_Sandman,
    Persona_Belial,
    Persona_LeananSidhe,
    Persona_Cybele,
    Persona_Chernobog,
    Persona_Flauros,
    Persona_IpponDatara,
    Persona_Orthrus,
    Persona_Succubus,
    Persona_Mothman,
    Persona_RESERVE,
    Persona_Dominion,
    Persona_Nekomata,
    Persona_BlackFrost,
    Persona_Arahabaki,
    Persona_Angel,
    Persona_Skadi,
    Persona_KikuriHime,
    Persona_ChiYou,
    Persona_Power,
    Persona_Inugami,
    Persona_Nebiros,
    Persona_RESERVE2,
    Persona_Slime,
    Persona_Anzu,
    Persona_Yatagarasu,
    Persona_Yoshitsune,
    Persona_TakeMinakata,
    Persona_AmenoUzume,
    Persona_Kushinada,
    Persona_Kumbhanda,
    Persona_OngyoKi,
    Persona_KinKi,
    Persona_SuiKi,
    Persona_FuuKi,
    Persona_Jatayu,
    Persona_Kaiwan,
    Persona_Kelpie,
    Persona_Thoth,
    Persona_Dionysus,
    Persona_Apsaras,
    Persona_Andras,
    Persona_RESERVE3,
    Persona_Koropokguru,
    Persona_KoppaTengu,
    Persona_Regent,
    Persona_QueensNecklace,
    Persona_StoneofScone,
    Persona_KohiNoor,
    Persona_Orlov,
    Persona_EmperorsAmulet,
    Persona_HopeDiamond,
    Persona_CrystalSkull,
    Persona_RESERVE4,
    Persona_RESERVE5,
    Persona_RESERVE6,
    Persona_RESERVE7,
    Persona_RESERVE8,
    Persona_RESERVE9,
    Persona_RESERVE10,
    Persona_Mandrake,
    Persona_Baal,
    Persona_Dakini,
    Persona_Silky,
    Persona_Bugs,
    Persona_BlackOoze,
    Persona_Bicorn,
    Persona_Mithras,
    Persona_Sudama,
    Persona_Kodama,
    Persona_Agathion,
    Persona_Onmoraki,
    Persona_Nue,
    Persona_Pisaca,
    Persona_Melchizedek,
    Persona_Baphomet,
    Persona_RajaNaga,
    Persona_Naga,
    Persona_Garuda,
    Persona_Moloch,
    Persona_Norn,
    Persona_Belphegor,
    Persona_Berith,
    Persona_Choronzon,
    Persona_RESERVE11,
    Persona_RESERVE12,
    Persona_RESERVE13,
    Persona_RESERVE14,
    Persona_RESERVE15,
    Persona_RESERVE16,
    Persona_NigiMitama,
    Persona_KushiMitama,
    Persona_AraMitama,
    Persona_SakiMitama,
    Persona_RESERVE17,
    Persona_Shiva,
    Persona_Michael,
    Persona_Asura,
    Persona_Mada,
    Persona_MotherHarlot,
    Persona_Clotho,
    Persona_Lachesis,
    Persona_Atropos,
    Persona_Ardha,
    Persona_RESERVE18,
    Persona_Vishnu,
    Persona_Hariti,
    Persona_Yurlungur,
    Persona_Hecatoncheires,
    Persona_Satanael,
    Persona_RESERVE19,
    Persona_RESERVE20,
    Persona_RESERVE21,
    Persona_RESERVE22,
    Persona_RESERVE23,
    Persona_RESERVE24,
    Persona_RESERVE25,
    Persona_RESERVE26,
    Persona_RESERVE27,
    Persona_RESERVE28,
    Persona_Orpheus,
    Persona_Thanatos,
    Persona_Izanagi,
    Persona_MagatsuIzanagi,
    Persona_Kaguya,
    Persona_Ariadne,
    Persona_Asterius,
    Persona_Tsukiyomi,
    Persona_Messiah,
    Persona_MessiahPicaro,
    Persona_OrpheusPicaro,
    Persona_ThanatosPicaro,
    Persona_IzanagiPicaro,
    Persona_MIzanagiPicaro,
    Persona_KaguyaPicaro,
    Persona_AriadnePicaro,
    Persona_AsteriusPicaro,
    Persona_TsukiyomiPicaro,
    Persona_Satanael2,
    Persona_,
    Persona_Arsene,
    Persona_CaptainKidd,
    Persona_Zorro,
    Persona_Carmen,
    Persona_Goemon,
    Persona_Johanna,
    Persona_Milady,
    Persona_Necronomicon,
    Persona_RobinHood,
    Persona_Loki,
    Persona_Satanael3,
    Persona_SeitenTaisei,
    Persona_Mercurius,
    Persona_Hecate,
    Persona_KamuSusanoo,
    Persona_Anat,
    Persona_Astarte,
    Persona_Prometheus,
    Persona_Loki2,
    Persona_Arsene2,
    Persona_CaptainKidd2,
    Persona_Zorro2,
    Persona_Carmen2,
    Persona_Goemon2,
    Persona_Johanna2,
    Persona_Milady2,
    Persona_Necronomicon2,
    Persona_RobinHood2,
    Persona_RESERVE29,
    Persona_Lucifer,
    Persona_SeitenTaisei2,
    Persona_Mercurius2,
    Persona_Hecate2,
    Persona_KamuSusanoo2,
    Persona_Anat2,
    Persona_Astarte2,
    Persona_Prometheus2,
    Persona_Loki3,
    Persona_RESERVE30,
    Persona_RESERVE31,
    Persona_RESERVE32,
    Persona_RESERVE33,
    Persona_RESERVE34,
    Persona_RESERVE35,
    Persona_RESERVE36,
    Persona_RESERVE37,
    Persona_RESERVE38,
    Persona_RESERVE39,
    Persona_RESERVE40,
    Persona_RESERVE41,
    Persona_RESERVE42,
    Persona_Satan,
    Persona_Lucifer2,
    Persona_Kohryu,
    Persona_Okuninushi,
    Persona_Norn2,
    Persona_RESERVE43,
    Persona_RESERVE44,
    Persona_Futsunushi,
    Persona_RESERVE45,
    Persona_Seth,
    Persona_Ishtar,
    Persona_RESERVE46,
    Persona_Surt,
    Persona_Siegfried,
    Persona_Lakshmi,
    Persona_RESERVE47,
    Persona_RESERVE48,
    Persona_ExBelphegor,
    Persona_RESERVE49,
    Persona_Garuda2,
    Persona_Fortuna,
    Persona_Suzaku,
    Persona_Seiryu,
    Persona_Genbu,
    Persona_Byakko,
    Persona_Bishamonten,
    Persona_Koumokuten,
    Persona_Jikokuten,
    Persona_Zouchouten,
    Persona_HellBiker,
    Persona_Daisoujou,
    Persona_Trumpeter,
    Persona_WhiteRider,
    Persona_Matador,
    Persona_PaleRider,
    Persona_Horus,
    Persona_RESERVE50,
    Persona_Attis,
    Persona_RESERVE51,
    Persona_RESERVE52,
    Persona_Sraosha,
    Persona_Berith2,
    Persona_RESERVE53,
    Persona_Mitra,
    Persona_Phoenix,
    Persona_Principality,
    Persona_NekoShogun,
    Persona_Vasuki,
    Persona_Ananta,
    Persona_Throne,
    Persona_RESERVE54,
    Persona_Quetzalcoatl,
    Persona_RedRider,
    Persona_BlackRider,
    Persona_RESERVE55,
    Persona_ExMoloch,
    Persona_Pazuzu,
    Persona_RESERVE56,
    Persona_Slime2,
    Persona_JackoLantern2,
    Persona_Agathion2,
    Persona_Mandrake2,
    Persona_Shiisaa2,
    Persona_JackFrost2,
    Persona_Sudama2,
    Persona_Onmoraki2,
    Persona_Bugs2,
    Persona_Pixie2,
    Persona_Pixie3,
    Persona_Ardha2,
    Persona_RESERVE57,
    Persona_RESERVE58,
    Persona_RESERVE59,
    Persona_RESERVE60,
    Persona_RESERVE61,
    Persona_RESERVE62,
    Persona_RESERVE63,
    Persona_RESERVE64,
    Persona_RESERVE65,
    Persona_RESERVE66,
    Persona_RESERVE67,
    Persona_RESERVE68,
    Persona_RESERVE69,
    Persona_RESERVE70,
    Persona_RESERVE71,
    Persona_RESERVE72,
    Persona_RESERVE73,
    Persona_RESERVE74,
    Persona_RESERVE75,
    Persona_RESERVE76,
    Persona_RESERVE77,
    Persona_RESERVE78,
    Persona_RESERVE79,
    Persona_RESERVE80,
    Persona_RESERVE81,
    Persona_RESERVE82,
    Persona_RESERVE83,
    Persona_RESERVE84,
    Persona_RESERVE85,
    Persona_P5Unused,
    Persona_P5Unused2,
    Persona_P5Unused3,
    Persona_P6Unused,
    Persona_P5Unused4,
    Persona_P5Unused5,
    Persona_P5Unused6,
    Persona_P5Unused7,
    Persona_P5Unused8,
    Persona_P5Unused9,
    Persona_P5Unused10,
    Persona_P5Unused11,
    Persona_P5Unused12,
    Persona_P5Unused13,
    Persona_P5Unused14,
    Persona_P5Unused15,
    Persona_P5Unused16,
    Persona_P5Unused17,
    Persona_P5Unused18,
    Persona_P5Unused19,
    Persona_P5Unused20,
    Persona_P5Unused21,
    Persona_P5Unused22,
    Persona_P5Unused23,
    Persona_P5Unused24,
    Persona_P5Unused25,
    Persona_P5Unused26,
    Persona_P5Unused27,
    Persona_P5Unused28,
    Persona_P5Unused29,
    Persona_P5Unused30,
    Persona_P5Unused31,
    Persona_P5Unused32,
    Persona_P5Unused33,
    Persona_P5Unused34,
    Persona_P5Unused35,
    Persona_P5Unused36,
    Persona_P5Unused37,
    Persona_P5Unused38,
    Persona_P5Unused39,
    Persona_P5Unused40,
    Persona_P5Unused41,
    Persona_P5Unused42,
    Persona_P5Unused43,
    Persona_P5Unused44,
    Persona_P5Unused45,
    Persona_P5Unused46,
    Persona_P5Unused47,
    Persona_P5Unused48,
    Persona_P5Unused49,
    Persona_P5Unused50,
    Persona_P5Unused51,
    Persona_P5Unused52,
    Persona_P5Unused53,
    Persona_P5Unused54,
    Persona_P5Unused55,
    Persona_P5Unused56,
    Persona_P5Unused57,
    Persona_P5Unused58,
    Persona_P5Unused59,
    Persona_P5Unused60,
    Persona_P5Unused61,
    Persona_P5Unused62,
    Persona_P5Unused63,
    Persona_P5Unused64,
    Persona_P5Unused65,
    Persona_P5Unused66,
    Persona_P5Unused67,
    Persona_P5Unused68,
    Persona_P5Unused69,
    Persona_P5Unused70,
    Persona_P5Unused71,
    Persona_P5Unused72,
    Persona_P5Unused73,
    Persona_P5Unused74,
    Persona_P5Unused75,
    Persona_P5Unused76,
    Persona_P5Unused77,
    Persona_P5Unused78,
    Persona_P5Unused79,
    Persona_P5Unused80,
    Persona_P5Unused81,
    Persona_P5Unused82,
    Persona_P5Unused83,
    Persona_P5Unused84,
    Persona_P5Unused85,
    Persona_P5Unused86,
    Persona_P5Unused87,
    Persona_P5Unused88,
    Persona_P5Unused89,
    Persona_P5Unused90,
    Persona_P5Unused91,
    Persona_P5Unused92,
    Persona_P5Unused93,
    Persona_P5Unused94,
    Persona_P5Unused95,
    Persona_P5Unused96,
    Persona_P5Unused97,
    Persona_P5Unused98,
    Persona_P5Unused99,
    Persona_P5Unused100,
    Persona_P5Unused101,
    Persona_P5Unused102,
    Persona_P5Unused103,
    Persona_P5Unused104,
    Persona_P5Unused105,
    Persona_P5Unused106,
    Persona_P5Unused107,
    Persona_P5Unused108,
    Persona_RESERVE86,
    Persona_RESERVE87,
    Persona_RESERVE88,
    Persona_RESERVE89,
} PersonaNames;

typedef struct
{
  u32 Icons;
  u32 field_0x4;
  u32 Equippable_Users;
  u16 field_0xc;
  u16 field_0xe;
  u16 Attack;
  u16 Accuracy;
  u8 stats[6];
  u16 gear_effect[2];
  u16 field_0x1e;
  u16 field_0x20;
  u16 RESERVE;
  int buy_price;
  int sell_price;
  u8 purchase_month;
  u8 purchase_day;
  u16 field_0x2e;
} ItemTBL_MeleeWeapon;

typedef struct
{
  int Icons;
  u32 field_04;
  u32 Equippable_Users;
  u16 field_0c;
  u16 field_0e;
  u16 Attack;
  u16 Accuracy;
  u16 Mag_Size;
  u8 bonusStats[6];
  u16 GearEffect[3];
  u16 field_22;
  u16 field_24;
  u16 RESERVE;
  int buy_price;
  int sell_price;
  u8 purchase_month;
  u8 purchase_day;
  u16 field_32;
} ItemTBL_RangedWeapon;

typedef struct
{
  u32 icon;
  u32 Field04;
  u32 EquippableUsers;
  u16 Field0C;
  u16 GearEffect[3];
  u32 PurchasePrice;
  u32 SellPrice;
  u8 DateAvailable[2];
  u16 padding;
}ItemTBL_Outfit;


typedef enum
{
  MeleeWeapon,
  Armor,
  Accessory,
  Outfit,
  Gun
} EquipSlot;

typedef struct
{
	u16 Field00;
	u8 Field01;
	u8 Field02;
	u16 ModelMajorID;
	u8 ModelMinorID;
	u8 Field07;
	u16 Field08;
	u8 Field0A;
	u8 Field0B;
	u16 Field0C;
	u8 Field0E;
	u8 Field0F;
	u16 Field10;
	u8 Field12;
	u8 Field13;
	u16 Field14;
	u16 Field16;
	u16 Field18;
	u16 Field1A;
} resrcNPCTblEntry;

typedef struct JokerFieldControl
{
  u32 field00;
  u32 field04;
  u32 JokerAnimationIndex;
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
} JokerFieldControl;

typedef struct JokerFieldControl2
{
  u32 field00;
  u32 JokerTargetAnim;
  u32 field08;
  u32 field0c;
} JokerFieldControl2;

typedef struct
{
  u8 Flags; // 0
  u8 isUnlocked; // 1
  u16 personaID; // 2
  u8 personaLv; // 4
  u8 _x5; // 5
  u16 _x6; // 6, trait in R
  u32 personaExp; // 8
  u16 SkillID[8]; // C
  u8 Stats[ 5 ]; // 1c
  u8 StatsEx[ 5 ]; // 21
  u8 StatsExTemp[ 5 ]; // 26
  u8 _x2B; // 2b
  u32 _x2C; // 2c
} btlUnit_Persona; // total size should be 0x30

typedef struct
{
  u32 AilmentStatus_00_Burn : 1;
  u32 AilmentStatus_01_Freeze : 1;
  u32 AilmentStatus_02_Shock : 1;
  u32 AilmentStatus_03_Dizzy : 1;
  u32 AilmentStatus_04_Confuse : 1;
  u32 AilmentStatus_05_Fear : 1;
  u32 AilmentStatus_06_Forget : 1;
  u32 AilmentStatus_07_Hunger : 1;
  u32 AilmentStatus_08_Sleep : 1;
  u32 AilmentStatus_09_Rage : 1;
  u32 AilmentStatus_10_Despair : 1;
  u32 AilmentStatus_11_Brainwash : 1;
  u32 AilmentStatus_12_Desperation : 1;
  u32 AilmentStatus_13_ : 1;
  u32 AilmentStatus_14_Panic : 1;
  u32 AilmentStatus_15_Lust : 1;
  u32 AilmentStatus_16_Wrath : 1;
  u32 AilmentStatus_17_Envy : 1;
  u32 AilmentStatus_18_Susceptible : 1;
  u32 AilmentStatus_19_Dead : 1;
  u32 AilmentStatus_20_Down : 1;
  u32 AilmentStatus_21_ : 1;
  u32 AilmentStatus_22_Rattled : 1;
  u32 AilmentStatus_23_ : 1;
  u32 AilmentStatus_24_ : 1;
  u32 AilmentStatus_25_ : 1;
  u32 AilmentStatus_26_ : 1;
  u32 AilmentStatus_27_ : 1;
  u32 AilmentStatus_28_ : 1;
  u32 AilmentStatus_29_ : 1;
  u32 AilmentStatus_30_ : 1;
  u32 AilmentStatus_31_ : 1;
} S_AilmentStatus;

typedef struct
{
  u32 BuffStatus_31 : 1;
  u32 BuffStatus_30 : 1;
  u32 BuffStatus_29 : 1;
  u32 BuffStatus_28 : 1;
  u32 BuffStatus_27 : 1;
  u32 BuffStatus_ResistInstaKill : 1;
  u32 BuffStatus_25 : 1;
  u32 BuffStatus_24 : 1;
  u32 BuffStatus_Concentrate : 1;
  u32 BuffStatus_Charge : 1;
  u32 BuffStatus_ResistMagic : 1;
  u32 BuffStatus_ResistPhys : 1;
  u32 BuffStatus_19 : 1;
  u32 BuffStatus_ResistPsy : 1;
  u32 BuffStatus_ResistNuke : 1;
  u32 BuffStatus_ResistWind : 1;
  u32 BuffStatus_ResistElec : 1;
  u32 BuffStatus_ResistIce : 1;
  u32 BuffStatus_ResistFire : 1;
  u32 BuffStatus_AffPsy : 1;
  u32 BuffStatus_AffNuke : 1;
  u32 BuffStatus_AffElec : 1;
  u32 BuffStatus_AffWind : 1;
  u32 BuffStatus_AffIce : 1;
  u32 BuffStatus_AffFire : 1;
  u32 BuffStatus_Susceptibility : 1;
  u32 BuffStatus_Critical2 : 1;
  u32 BuffStatus_Critical : 1;
  u32 BuffStatus_EVA : 1;
  u32 BuffStatus_DEF : 1;
  u32 BuffStatus_ACC : 1;
  u32 BuffStatus_ATK : 1;
} BuffStatus;

typedef struct
{
  u32 BuffDir_ACC : 4;
  u32 BuffDir_ATK : 4;
  u32 BuffDir_EVA : 4;
  u32 BuffDir_DEF : 4;
  u32 BuffDir_Crit : 4;
  u32 BuffDir_Crit2 : 4;
  u32 BuffDir_Sucep : 4;
  u32 BuffDir_AffFire : 4;
  u32 BuffDir_AffIce : 4;
  u32 BuffDir_AffWind : 4;
  u32 BuffDir_AffElec : 4;
  u32 BuffDir_AffNuke : 4;
  u32 BuffDir_AffPsy : 4;
  u32 BuffDir_ResistFire : 4;
  u32 BuffDir_ResistIce : 4;
  u32 BuffDir_ResistElec : 4;
  u32 BuffDir_ResistWind : 4;
  u32 BuffDir_ResistNuke : 4;
  u32 BuffDir_ResistPsy : 4;
  u32 BuffDir_ : 4;
  u32 BuffDur_ACC : 4;
  u32 BuffDur_ATK : 4;
  u32 BuffDur_EVA : 4;
  u32 BuffDur_DEF : 4;
  u32 BuffDur_Crit : 4;
  u32 BuffDur_Crit2 : 4;
  u32 BuffDur_Sucep : 4;
  u32 BuffDur_AffFire : 4;
  u32 BuffDur_AffIce : 4;
  u32 BuffDur_AffWind : 4;
  u32 BuffDur_AffElec : 4;
  u32 BuffDur_AffNuke : 4;
  u32 BuffDur_AffPsy : 4;
  u32 BuffDur_ResistFire : 4;
  u32 BuffDur_ResistIce : 4;
  u32 BuffDur_ResistElec : 4;
  u32 BuffDur_ResistWind : 4;
  u32 BuffDur_ResistNuke : 4;
  u32 BuffDur_ResistPsy : 4;
  u32 BuffDur_ : 4;
} BuffMeta;

typedef struct btlUnit_Unit
{
  u32 Flags; // 0
  u16 unitType; // 4
  u16 Field06; // 6
  u32 unitID; // 8
  u32 currentHP; // C
  u32 currentSP; // 10
  u32 StatusAilments; // 14
  u16 Joker_Lv; // 18
  u16 Field1A; // 1A
  u32 Joker_EXP; // 1C
  u32 PhaseID; // 20
  BuffStatus Buffs; // 24
  u32 BuffStatus2; // 28
  BuffMeta BuffsDirDur; // 2C
  u16 EquippedPersonaIndex; // 40
  u16 Field42;
  btlUnit_Persona StockPersona[12];
  u16 meleeID; // 284
  u16 protectorID; // 286
  u16 accessoryID; // 288
  u16 outfitID; // 28A
  u16 rangedWeaponID; // 28C
  u32 Field28E;
  u16 TacticsState; // 292
  u16 numOfBullets; // 294
  u32 Field296;
  u16 Field29A;
  u16 HPGainNextLv; // 29C
  u16 SPGainNextLv; // 29E
} btlUnit_Unit;

typedef struct
{
  u32 Flags; // 0
  u16 unitType; // 4
  u16 Field06; // 6
  u32 unitID; // 8
  u32 currentHP; // C
  u32 currentSP; // 10
  u32 StatusAilments; // 14
  u16 Joker_Lv; // 18
  u16 Field1A; // 1A
  u32 Joker_EXP; // 1C
  u32 PhaseID; // 20
  BuffStatus Buffs; // 24
  u32 BuffStatus2; // 28
  BuffMeta BuffsDirDur; // 2C
  u16 EquippedPersonaIndex; // 40
  u16 Field42;
  btlUnit_Persona StockPersona;
  u16 meleeID; // 74
  u16 protectorID; // 76
  u16 accessoryID; // 78
  u16 outfitID; // 7A
  u16 rangedWeaponID; // 7C
  u32 Field28E;
  u16 TacticsState; // 82
  u16 numOfBullets; // 84
  u32 Field296;
  u16 Field29A;
  u16 HPGainNextLv; // 8C
  u16 SPGainNextLv; // 8E
} btlUnit_Lite;

typedef struct
{
  s16 FieldMajorID;
  s16 FieldMinorID;
  s16 FieldEnvMajorID;
  s16 FieldEnvMinorID;
  s16 Field08;
  s16 Field0A;
  s16 Field0C;
  s16 Field0E;
  s16 Field10;
  s16 Field12;
  s16 Field14;
  s16 Field16;
  s16 Field18;
  s16 Field1A;
  s16 Field1C;
  s16 Field1E;
}SequenceType3;


typedef struct AI_UnitStruct 
{ 
  u32 field_0x0;
  u32 field_0x4;
  u32 field_0x8;
  struct structA * field_0xc;
  u32 field_0x10;
  u32 field_0x14;
  u32 field_0x18;
  u32 Act_Type;
  u32 useGun;
  u32 field_0x24;
  u32 field_0x28;
  u32 field_0x2c;
  u32 field_0x30;
  u32 ActSkillID;
  u32 unk_0x38;
  u32 field_0x3c;
  u32 field_0x40;
  u32 field_0x44;
  u32 field_0x48;
  u32 field_0x4c;
  u32 field_0x50;
  u32 field_0x54;
  u32 waitMSGID;
  u16 argArray[9]; /* Created by retype action */
  u16 argCount; /* Created by retype action */
} AI_UnitStruct;

typedef struct fieldModelList
{
  u16 FieldMajorID;
  u8 FieldMinorID;
  u8 Amount;
}fieldModelList;

typedef struct structB
{
  u32 field00;
  btlUnit_Unit* btlUnitPointer;
} structB;

typedef struct
{
  u32 animationIndex00;
  u32 animationIndex04;
}AnimVtableFunctionFollower;


typedef struct structA
{
  u32 field00;
  u32 field04;
  u32 field08;
  structB* field0C;
  u32 field10;
  u32 Field14;
  structB* Field18;
  u32 Field1C;
  u32 Field20;
  u32 Field24;
  u32 Field28;
  u32 Field2C;
} structA;

typedef struct structA_2
{
  u32 Field00;
  u32 Field04;
  u32 Field08;
  structA* Field0C;
  u32 Field10;
  u32 Field14;
  structB* Field18;
  u32 Field1C;
  u32 Field20;
  u32 Field24;
  u32 Field28;
  u32 Field2C;
} structA_2;

typedef struct AnimVtableFunctionA1
{
  u32 field00;
  u32 field04;
  u32 field08;
  u32 field0c;
  structB* field10;
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
  u32 field128;
  u32 field12c;
  u32 field130;
  u32 field134;
  u32 field138;
  u32 field13c;
  u32 field140;
  u32 field144;
  u32 field148;
  u32 field14c;
  u32 field150;
  u32 field154;
  u32 field158;
  u32 field15c;
  u32 field160;
  u32 field164;
  u32 field168;
  u32 field16c;
  u32 field170;
  u32 field174;
  u32 field178;
  u32 field17c;
  u32 field180;
  u32 field184;
  u32 field188;
  u32 field18c;
  u32 field190;
  u32 field194;
  u32 field198;
  u32 field19c;
  u32 field1a0;
  u32 field1a4;
  u32 field1a8;
  u32 field1ac;
  u32 field1b0;
  u32 field1b4;
  u32 field1b8;
  u32 field1bc;
  u32 field1c0;
  u32 field1c4;
  u32 field1c8;
  u32 field1cc;
  u32 field1d0;
  u32 field1d4;
  u32 field1d8;
  u32 field1dc;
  u32 field1e0;
  u32 field1e4;
  u32 field1e8;
  u32 field1ec;
  u32 field1f0;
  u32 field1f4;
  u32 field1f8;
  u32 field1fc;
  u32 field200;
  u32 field204;
  u32 field208;
  u32 field20c;
  u32 field210;
  u32 field214;
  u32 field218;
  u32 field21c;
  u32 field220;
  u32 field224;
  u32 field228;
  u32 field22c;
  u32 field230;
  u32 field234;
  u32 field238;
  u32 field23c;
  u32 field240;
  u32 field244;
  u32 field248;
  u32 field24c;
  u32 field250;
  u32 field254;
  u32 field258;
  u32 field25c;
  u32 field260;
  u32 field264;
  u32 field268;
  u32 field26c;
  u32 field270;
  u32 field274;
  u32 field278;
  u32 field27c;
  u32 field280;
  u32 field284;
  u32 field288;
  u32 field28c;
  u32 field290;
  u32 field294;
  u32 field298;
  u32 field29c;
  u32 field2a0;
  u32 field2a4;
  u32 field2a8;
  u32 field2ac;
  u32 field2b0;
  u32 field2b4;
  u32 field2b8;
  u32 field2bc;
  u32 field2c0;
  u32 field2c4;
  u32 field2c8;
  u32 field2cc;
  u32 field2d0;
  u32 field2d4;
  u32 field2d8;
  AnimVtableFunctionFollower* field2dc;
  u32 field2e0;
  u32 field2e4;
  u32 field2e8;
  u32 field2ec;
  u32 field2f0;
  u32 field2f4;
  u32 field2f8;
  u32 field2fc;
} AnimVtableFunctionA1;

typedef struct
{
  AnimVtableFunctionA1* Field00;
  u32 Field04;
  u32 Field08;
  u32 Field0C;
  structB* Field10;
  u32 Field14;
  u32 Field18;
  u16 Field1C;
} CueIDThingy;

CueIDThingy* PartyMemberCombatVoiceInstance[11];
int PartyMemberCombatVoiceInstanceB[11];

typedef struct
{
  u32 ptr1;
  u32 ptr2;
  u32 ptr3;
  u32 ptr4;
  u32 ptr5;
  u32 ptr6;
  u32 ptr7;
  u32 ptr8;
} idkman;

typedef struct
{
  u32 Field_00;
  u32 Field_04;
  u32 Field_08;
  u32 Field_0C;
} sample_int_struct;

typedef struct PersonaStats
{
  u8 stat[5];
}PersonaStats;

typedef struct unitTBLStats
{
  u16 Flags;
  u8 Arcana;
  u16 Field03;
  u8 unitLv;
  u16 Field06;
  u32 MaxHP;
  u32 MaxSP;
  PersonaStats stats;
  u8 padding;
  u16 Skills[8];
  u16 ExpReward;
  u16 MoneyReward;
  u8 itemCategory;
  u8 itemID;
  u16 dropChance;
  u8 unknown[18];
  u16 MoreFlags;
  u16 AttackDamage;
} unitTBLStats;

typedef struct
{
  u16 Physical;
  u16 Gun;
  u16 Fire;
  u16 Ice;
  u16 Electric;
  u16 Wind;
  u16 Psy;
  u16 Nuke;
  u16 Bless;
  u16 Curse;
  u16 Almighty;
  u16 Dizzy;
  u16 Confuse;
  u16 Fear;
  u16 Forget;
  u16 Hunger;
  u16 Sleep;
  u16 Rage;
  u16 Despair;
  u16 Brainwash;
} unitAffinity;

typedef struct
{
  u16 affinity[20];
}unitAffinityArray;


typedef struct
{
  u8 VoiceID;
  u8 TALK_PERSON;
  u8 VoicePackABC;
  u8 padding;
  u16 TALK_MONEYMIN;
  u16 TALK_MONEYMAX;
  u16 TALK_ITEM[4];
  u16 TALK_ITEM_RARE[4];
}Segment3;

typedef struct
{
  u16 personaID;
  u16 modelID;
}VisualIndex;

typedef struct
{
  unitTBLStats EnemyStats[1000];
}EnemyUnitStatsTBL;

typedef struct
{
  unitAffinityArray EnemyAffinities[1000];
} EnemyAffinityTBL;

typedef struct
{
  unitAffinityArray PersonaAffinities[1000];
} PersonaAffinityTBL;

typedef struct
{
  Segment3 unit[1000];
}unitTBLSegment3;

typedef struct
{
  VisualIndex unitVisualIndex[1000];
}unitTBLVisualIndex;

typedef struct
{
  u32 ScriptID;
  u8 stuff[0x28];
}ELSAI_Segment2;

typedef struct
{
  ELSAI_Segment2 entry[651];
}ELSAI_Segment2TBL;

typedef struct
{
  u64 fileStatus;
  char filename[128];
  u32 bufferSize;
  u32 unk2;
  u32 unk3;
  u32 pointerToFile;
}fileHandleStruct;

fileHandleStruct* shdEnemyFile;
fileHandleStruct* FieldListFile;

typedef struct
{
  u32 IconBitfield;
  u32 Field04;
  u32 EquippableUsersBitfield;
  u16 Field0C;
  u8 StatBoosts[6];
  u16 AccessoryEffects[3];
  u16 Field1A;
  u16 Field1C;
  u16 RESERVE;
  u32 PurchasePrice;
  u32 SellPrice;
  u8 PurchaseMonth;
  u8 PurchaseDay;
  u32 Field2A[5];
  u16 Field3E;
}itemTBLAccessoryEntry;

typedef struct
{
  u8 idk[0x300];
}FieldGetIDStruct;

typedef struct
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
  u32 field128;
  u32 field12c;
  u32 field130;
  u32 field134;
  u32 field138;
  u32 field13c;
  u32 field140;
  u16 FieldID_Major;
  u16 FieldID_Minor;
  u32 field148;
  u32 field14c;
  u32 field150;
  u32 field154;
  u32 field158;
  u32 field15c;
  u32 field160;
  u32 field164;
  u32 field168;
  u32 field16c;
  u32 field170;
  u32 field174;
  u32 field178;
  u32 field17c;
  u32 field180;
  u32 field184;
  u32 field188;
  u32 field18c;
  u32 field190;
  u32 field194;
  u32 field198;
  u32 field19c;
  u32 field1a0;
  u32 field1a4;
  u32 field1a8;
  u32 field1ac;
  u32 field1b0;
  u32 field1b4;
  u32 field1b8;
  u32 field1bc;
  u32 field1c0;
  u32 field1c4;
  u32 field1c8;
  u32 field1cc;
  u32 field1d0;
  u32 field1d4;
  u32 field1d8;
  u32 field1dc;
  u32 field1e0;
  u32 field1e4;
  u32 field1e8;
  u32 field1ec;
  u32 field1f0;
  u32 field1f4;
  u32 field1f8;
  u32 field1fc;
  u32 field200;
  u32 field204;
  u32 field208;
  u32 field20c;
  u32 field210;
  u32 field214;
  u32 field218;
  u32 field21c;
  u32 field220;
  u32 field224;
  u32 field228;
  u32 field22c;
  u32 field230;
  u32 field234;
  u32 field238;
  u32 field23c;
  u32 field240;
  u32 field244;
  u32 field248;
  u32 field24c;
  u32 field250;
  u32 field254;
  u32 field258;
  u32 field25c;
  u32 field260;
  u32 field264;
  u32 field268;
  u32 field26c;
  u32 field270;
  u32 field274;
  u32 field278;
  u32 field27c;
  u32 field280;
  u32 field284;
  u32 field288;
  u32 field28c;
  u32 field290;
  u32 field294;
  u32 field298;
  u32 field29c;
  u32 field2a0;
  u32 field2a4;
  u32 field2a8;
  u32 field2ac;
  u32 field2b0;
  u32 field2b4;
  u32 field2b8;
  u32 field2bc;
  u32 field2c0;
  u32 field2c4;
  u32 field2c8;
  u32 field2cc;
  u32 field2d0;
  u32 field2d4;
  u32 field2d8;
  u32 field2dc;
  u32 field2e0;
  u32 field2e4;
  u32 field2e8;
  u32 field2ec;
  u32 field2f0;
  u32 field2f4;
  u32 field2f8;
  u32 field2fc;
} fieldworkdataStruct;

typedef struct btlResult
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
} btlResult;

typedef struct ResourceHandleStruct
{
  u32 field00;
  u32 ModelIDs;
  u32 handle;
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
  u32 field128;
  u32 field12c;
  u32 field130;
  u32 field134;
  u32 field138;
  u32 field13c;
  u32 field140;
  u32 field144;
  u32 field148;
  u32 field14c;
  u32 field150;
  u32 field154;
  u32 field158;
  u32 field15c;
  u32 field160;
  u32 field164;
  u32 field168;
  u32 field16c;
  u32 field170;
  u32 field174;
  u32 field178;
  u32 field17c;
  u32 field180;
  u32 field184;
  u32 field188;
  u32 field18c;
  u32 field190;
  u32 field194;
  u32 field198;
  u32 field19c;
  u32 field1a0;
  u32 field1a4;
  u32 field1a8;
  u32 field1ac;
  u32 field1b0;
  u32 field1b4;
  u32 field1b8;
  u32 field1bc;
  u32 field1c0;
  u32 field1c4;
  u32 field1c8;
  u32 field1cc;
  u32 field1d0;
  u32 field1d4;
  u32 field1d8;
  u32 field1dc;
  u32 field1e0;
  u32 field1e4;
  u32 field1e8;
  u32 field1ec;
  u32 field1f0;
  u32 field1f4;
  u32 field1f8;
  u32 field1fc;
  u32 field200;
  u32 field204;
  u32 field208;
  u32 field20c;
  u32 field210;
  u32 field214;
  u32 field218;
  struct ResourceHandleStruct* next;
} ResourceHandleStruct;

typedef struct NaviSoundSubStruct
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
  u32 naviID;
  u32 field28;
} NaviSoundSubStruct;

typedef struct NaviSoundStructIDK
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
  u32 field128;
  u32 field12c;
  u32 field130;
  u32 field134;
  u32 field138;
  u32 field13c;
  u32 field140;
  u32 field144;
  u32 field148;
  u32 field14c;
  u32 field150;
  u32 field154;
  u32 field158;
  u32 field15c;
  u32 field160;
  u32 field164;
  u32 field168;
  u32 field16c;
  u32 field170;
  u32 field174;
  u32 field178;
  u32 field17c;
  u32 field180;
  u32 field184;
  u32 field188;
  u32 field18c;
  u32 field190;
  u32 field194;
  u32 field198;
  u32 field19c;
  u32 field1a0;
  u32 field1a4;
  u32 field1a8;
  u32 field1ac;
  NaviSoundSubStruct* navisubstruct;
  u32 field1b4;
  u32 field1b8;
  u32 field1bc;
} NaviSoundStructIDK;

typedef struct
{
  int pointer_1;
  int pointer_2;
  structB* pointer_3;
} btlUnitasThirdPointer;

typedef struct
{
  int pointer_1;
  btlUnitasThirdPointer* pointer_2;
} btlUnitasThirdPointerExtra;

typedef struct
{
  int pointer_1;
  btlUnit_Unit* pointer_2;
  int field08;
  int field0c;
  int field10;
  btlUnitasThirdPointerExtra* field14;
  btlUnitasThirdPointer* field18;
  int field1c;
  int field20;
  int field24;
  int field28;
  int field2c;
} struct_2_pointers;

typedef struct
{
  u32 ExpRequired[98];
}PartyMemberLvUpThresholdExp;

typedef struct
{
  u8 LvReq;
  u8 canLearn;
  u16 skillID;
}LearnableSkill;

typedef struct 
{
  u16 charID;
  u8 lv;
  u8 unk;
  LearnableSkill skills[32];
  PersonaStats stats[98];
}PartyMemberPersonaBlock;

typedef struct NodeNext1
{
  int Field00;
  int Field04;
  int Field08;
  int Field0C;
  int Field10;
  int Field14;
  struct_2_pointers* Field18;
} NodeNext1;

typedef struct Node 
{
  struct Node *prev;
  struct Node *next;
  int Field08;
  int Field0C;
  int Field10;
  NodeNext1* Field14;
} Node;

typedef struct UnitListStruct
{
  Node *Last;
  Node *First;
  int ListSize;
  int Field04;
} UnitListStruct;

typedef struct UnitList3
{
  u8 field00[136];
  UnitListStruct UnitList[3];
} UnitList3;

typedef struct UnitList2
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
  UnitList3* field34;
} UnitList2;

typedef struct UnitList1
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
  UnitList2* field54;
} UnitList1;

fileHandleStruct* NaviTestFile;

EnemyUnitStatsTBL NewEnemyStatsTBL;
EnemyAffinityTBL NewEnemyAffinityTBL;
PersonaAffinityTBL NewPersonaAffinityTBL;
unitTBLSegment3 NewSegment3TBL;
unitTBLVisualIndex NewVisualIndexTBL;
ELSAI_Segment2TBL NewSegment2TBL;

btlUnit_Unit* enemyBtlUnit;
btlUnit_Unit* currentActingUnit;

struct
{
  short modelID[255];
}MeleeWeaponModels;

bool hasUzukiAilmentAnnounce[12];
bool UzukiLowHPWarn;
bool UzukiDebuffAttackWarn;
bool UzukiDebuffDeffenseWarn;
bool UzukiDebuffSpeedWarn;


// Game functions are declared like this
// R, meaning result, and 6, the number of parameters
// A void function does not return anything, and requires you to use SHK_FUNCTION_V<N> instead.
// If you use vscode youll get descriptive tooltips when you hover these macros.
/**
 * @brief Flowscript Function that restores all player character's hp/sp
 * 
 * @return int 
 */
int RECOVERY_ALL( void );


PartyMemberLvUpThresholdExp* GetPartyMemberLvUpThreshold( int unitID );
PartyMemberPersonaBlock* GetPartyMemberPersonaBlock( int personaID );


/**
 * @brief Flowscript Function that restores all player character's bullets
 * 
 * @return int 
 */
int BULLET_RECOVERY( void );

/**
 * @brief Context based function, in AI functions returns the bltUnit struct of the unit running it
 * 
 * @return AI_UnitStruct struct 
 */
AI_UnitStruct* FLW_GetBattleUnitStructFromContext( void );

/**
 * @brief Returns btlUnit struct from a given player ID
 * 
 * @param u32 player ID whose struct to return
 * @return btlUnit struct
 */
btlUnit_Unit* GetBtlPlayerUnitFromID( u32 id );

/**
 * @brief Get the Unit ID from target party slot
 * 
 * @param u32 party slot to get unit from 
 * @return unit id 
 */
int GetUnitIDFromPartySlot( u32 slotID );

/**
 * @brief Set bullets to max for target player character
 * 
 * @param btlUnit_Unit btlUnit struct of player character 
 */
void SetPlayerBulletsToMax( btlUnit_Unit* player );

bool IsBtlUnitDead( btlUnit_Unit* Unit, u32 a2 );


u32 FUN_0025867c( u16 personaID );

/**
 * @brief Get int argument from FlowScript Interpreter Stack
 * 
 * @param u32 argument number 
 * @return value stored in target stack 
 */
int FLW_GetIntArg( u32 arg );


u64 GetBtlUnitPersona( u32 arg );

/**
 * @brief Get float argument from FlowScript Interpreter Stack
 * 
 * @param u32 argument number 
 * @return value stored in target stack 
 */
f64 FLW_GetFloatArg( u32 arg );

/**
 * @brief Get string argument from FlowScript Interpreter Stack
 * 
 * @param u32 argument number 
 * @return value stored in target stack 
 */
char* FLW_GetStringArg( u32 arg );

/**
 * @brief Sets an int return value to the current flowscript command
 * 
 * @param u32 value to return 
 * @return int 
 */
int FLW_SetIntReturn( u32 arg );


u32 GetJokerTargetLvExp( u32 arg );

/**
 * @brief Returns the currently equipped Persona of the given player ID
 * 
 * @param u32 character ID 
 * @return Persona ID 
 */
u32 GetEquippedPersonaFunction( PersonaNames personaID );

/**
 * @brief Returns the value stored in target COUNT
 * 
 * @param u32 COUNT number to get value from 
 * @return value stored in COUNT 
 */
u32 ActualGetCount( u32 arg );

void ActualSetCount( u32 COUNT, u32 value );


u32 FUN_0025bdf8( u16 unitID );


void* MallocAndReturn( u32 size );

void* malloc( u32 size );


u64 GetSavedataBitflagAlt( u64 flag );


void* SomethingAboutLoadingVoices( void* someAddressPointer );

/**
 * @brief Sets Joker's level
 * 
 * @param int Unit ID (THIS FUNCTION ONLY WORKS ON JOKER) 
 * @param int Level to set Joker to 
 */
void SetUnitLv( u32 unitID, u32 targetLv );


int FUN_002584cc( u32 a1, u32 a2 );


int sub_24B274( u32 a1, u16 a2 );


void FUN_0025cb8c( u32 unitID, u16 personaID );

u64 FUN_00263714( u32 a1 );

/**
 * @brief Sets Joker's EXP based on level
 * 
 * @param int Unit ID (THIS FUNCTION ONLY WORKS ON JOKER) 
 * @param int target level to set EXP for
 */
void SetUnitEXP( u32 ID, u16 Lv );

/**
 * @brief Returns item ID of specified equip slot from a Player Unit
 * 
 * @param btlUnit_Unit btlunit Struct of player character to get item ID from 
 * @param EquipSlot Equipment Slot to get Item ID from
 * @return item ID 
 */
u32 unitGetEquipment( btlUnit_Unit* unit, EquipSlot equipSlotID );


u32 CalculateAddressWithPointer( void* address, structA* unk );

/**
 * @brief returns the given player character's model minor ID based on currently equipped outfit
 * 
 * @param u32 character ID to get model minor ID from 
 * @param u8 always 50 
 * @param u8 always 0 
 * @return model minor ID 
 */
u32 PlayerUnitGetModelMinorID( u32 playerID, u8 a2, u8 a3 );

/**
 * @brief Plays target cue ID
 * 
 * @param unknown_struct
 * @param u32 unknown 
 * @param u32 target Cue ID 
 * @param u8 assumed pre-delay 
 */
void LoadSoundByCueIDCombatVoice( CueIDThingy* a1, u32 a2, u32 cueID, u8 idk );

fileHandleStruct* open_file( char* file_path, u32 a2 );
u64 FUN_00118280( char *param_1, char *param_2, char *param_3, u8 param_4 );
undefined8 FUN_001146d0( int a1 );

s32 setSeq( s32 seqId, void* params, s32 paramsSize, s32 r6 );
void FUN_0074ae50( void* a1, char* acb_string, char* awb_string, u32 idk );
int sndManPlaySfx( int a0, int a1, int a2, int a3, int a4, int a5 );
int FUN_0090053c( u32 a1, u64 a2, u64 a3 );
void FUN_00747f48( int* a1, int a2, int a3 );
u64 fsSync(int a1);
u64 scrRunScript( u32 a1, u32 a2, u32 a3, u32 a4 );
int semaphore_WaitPost( int a1 );
FieldGetIDStruct* FUN_00352f40( void );
ResourceHandleStruct* FUN_00015c34( int a1 );
int FUN_00014a38(u32 param_1, u32 param_2, u32 param_3, float param_4, float param_5);
void FUN_000148dc(double param_1, u64* param_2, int param_3);
void FUN_00324b70(u64* param_1, u64* param_2, u64 param_3);
u64 FUN_0032c3d4( int a1 );
u64 FUN_002ab314( int a1 );
int FUN_00320b1c( int a1 );
u64 FUN_002e1338(u64* param_1, u64* param_2, char param_3);
bool FUN_002d128c(int param_1, u64 param_2);
u64 FUN_002d7b6c(int* param_1);
int FUN_003316e0(int param_1);
int FUN_002d7b18(int* param_1);
undefined8 FUN_002d0870(int* param_1);
undefined8 FUN_002d08b4(int* param_1);
void FUN_0003b4b8(int param_1, char param_2);
void FUN_0003b510(int param_1, char param_2);
void FUN_002d9d00(undefined4* param_1);
char* FUN_00968be8( void );
char* FUN_00968bf4( void );
char* FUN_001a5834( void );
/*int criFsBinder_BindCpk( char* a1 );*/
int FUN_00ab563c( int* a1 );
int FUN_001a52f8( int a1 );
int FUN_2604C4( int arg );
int FUN_748ff0( int arg );
int FUN_0024b28c( int arg );
int FUN_0010fbbc( int arg );
void LoadEncounterEventSoundbank( int encounterID );

/**
 * @brief Returns target btlUnit struct if found
 * 
 * @param unitType 
 * @param unitID 
 * @return btlUnit_Unit* 
 */
btlUnit_Unit* GetBtlUnitInCombat( u32 unitType, u32 unitID );

/**
 * @brief Returns the state of the input flag
 * 
 * @param bitFlagID 
 * @return true 
 * @return false 
 */
bool GetBitflagState( int bitFlagID );

/**
 * @brief Set the state of the input flag
 * 
 * @param bitFlagID 
 * @param state 
 * @return true 
 * @return false 
 */
bool SetBitflagState( int bitFlagID, u8 state );
int FUN_007489a8( int a1, int a2 );
int FUN_0074805c( int a1, int a2 );
bool FUN_002588b4( btlUnit_Unit* a1 );

/**
 * @brief Returns whether a specific condifant bonus has been obtained or not
 * 
 * @param confidantBonusID 
 * @return true 
 * @return false 
 */
bool IsConfidantBonusObtained( int a1 );


/**
 * @brief Returns whether the given btlUnit currently has the given status ailment
 * 
 * @param btlUnit 
 * @param ailmentID 
 * @return bool 
 */
bool btlUnitHasAilment( btlUnit_Unit* Unit, u32 ailmentID );


/**
 * @brief Calls a given navigator dialogue with custom args
 * 
 * @param character ID
 * @param expression ID 
 * @param message ID 
 * @param dialogueBox Type 
 */
void CALL_NAVI_DIALOGUE( int charID, int expressionID, int msgID, int dialogueBoxType );


bool FUN_0031f9cc( void );
int FUN_0091da04( void );

/**
 * @brief Returns the Current BGM Cue ID
 * 
 * @return int 
 */
int GetCurrentBGMCueID( void );
static s32 sys_time_get_current_time( u64* secs, u64* nsecs );
s32 sys_dbg_read_process_memory(s32 pid, u32 process_ea, u32 size, void* data);
void* sigscan( void* start, void* end, s32* pattern, s32 patternLength );
u64 getTicks(void);
void CallNaviDialogue (struct_2_pointers * param_1, int param_2, int param_3, int param_4, int param_5, int param_6, char param_7, short param_8, double param_9);
bool FUN_007490a4( struct_2_pointers* a1, int a2 );
fieldworkdataStruct* GetFieldWorkData( void );

/**
 * @brief returns how many total ingame days have passed, starting from 4/1
 * 
 * @return u16 
 */
u16 GetTotalDays( void );

int isCharacterAssistExpressonValid( short a1, short a2 );
int FUN_003b9110( int a1, int a2, int a3, int a4, int a5 );

/**
 * @brief Check if specified unit has the specified skill
 * 
 * @param btlUnit 
 * @param SkillID 
 * @return true 
 * @return false 
 */
bool BtlUnitCheckHasSkill ( btlUnit_Unit* btlUnit, int SkillID );
bool GetAccessoryEffect ( btlUnit_Unit* btlUnit, int EffectID );
bool CheckAccessoryEffect ( btlUnit_Unit* btlUnit, int EffectID );
u32 FUN_000442cc ( int param_1,int param_2 );
void FUN_0024c018 ( u64 param_1, char param_2 );
double FUN_00250a10( btlUnit_Unit* btlUnit, u32 param_2, undefined8 param_3 );
bool AilmentCheck(btlUnit_Unit* btlUnit, u32 param_2);
int GetCountEquipHPBonus(btlUnit_Unit* param_1);
int GetCountEquipSPBonus(btlUnit_Unit* param_1);
bool CheckAccessoryEffect(btlUnit_Unit *btlUnit,int EffectID);
int FUN_26AE14(u32 param_1);
undefined4 FUN_25E5D8(int param_1, btlUnit_Unit* btlUnit_1, btlUnit_Unit* btlUnit_2, undefined2 param_4, undefined4 param_5);
u8 FUN_2588B4(btlUnit_Unit *param_1);
short FUN_25893C(btlUnit_Unit *param_1);
u64 GetSkillData(btlUnit_Unit* btlUnit, u32 SkillId);
double FUN_250A10(btlUnit_Unit *param_1, u32 param_2);
bool GetEquipmentEffect(u64 AccessoryID, int param_2);
double CalculateResistEffect(int AccId);
u64 FUN_2595C4(btlUnit_Unit *btlUnit,u32 param_2);
void FUN_25A328(btlUnit_Unit *param_1,u64 buff);
bool FUN_25963C(btlUnit_Unit *param_1);
bool FUN_0025b6ac(btlUnit_Unit *param_1);
bool FUN_00258888(btlUnit_Unit *param_1);
int GetNumberOfBullets(btlUnit_Unit* param_1);
u64 FUN_252980(u16 SkillId,btlUnit_Unit *btlUnit_1,btlUnit_Unit *btlUnit_2,u8 mode);
u64 FUN_25294C(u16 SkillId,btlUnit_Unit *btlUnit_1,btlUnit_Unit *btlUnit_2,u8 mode);
u32 Function_GetCurrentHP(btlUnit_Unit *param_1);
u32 Function_GetCurrentSP(btlUnit_Unit *param_1);
void FadeInFunction( u32 a1, u32 a2 );
void FadeOutFunction( u32 a1, u32 a2 );
bool AICheckSlipfunction( structA_2* a1 );
bool AI_CHK_SLIP( void );
int AI_CHK_ENIDHP( void );
void PrepareDLCBGM( void );
void memzero( void* args, int size );
int GetBtlUnitAilments( btlUnit_Unit* id );

/**
 * @brief Set priority of target CPK
 * 
 * @param cpkBind result ID of bindCPK for desired CPK
 * @param priority target load priority of CPK
 * @return int 
 */
int criFsBinder_SetPriority(int cpkBind, int priority);

/**
 * @brief Returns the Max HP of a Player btlUnit
 * 
 * @param param_1 btlUnit Struct
 * @return MaxHP of player unit 
 */
int GetBtlUnitMaxHP(btlUnit_Unit* param_1);
int GetBtlUnitMaxSP(btlUnit_Unit* param_1);

encounterIDTBL* GetEncounterEntryFromTBL( int encounterID);

int GetRandom ( int MaxValue );
bool isMidWinterValid( void );
itemTBLAccessoryEntry* GetAccessoryTBLEntry( u16 accessoryID );
ItemTBL_MeleeWeapon* GetMeleeWeaponTBLEntry( u16 WeaponID );
ItemTBL_RangedWeapon* GetRangedWeaponTBLEntry( u16 WeaponID );
unitTBLStats* GetUnitStatsBlockTBLEntry( u16 enemyID );
ItemTBL_Outfit* GetOutfitTBLEntry( u16 outfitID );

int SetCombatAnimation( CueIDThingy* a1, int a2, double a3, double a4, double a5);
int SetTwinsDodgeAnimation( CueIDThingy* a1, double a2, double a4);

PersonaTBL_Segment0* PersonaTBL_GetPersonaStatsSegment0( int PersonaID );

int GetTotalUnitsOfType( u32 TargetUnitType );

bool isPlayerUnitDead( u32 unitID );

typedef struct {
  float r;       // a fraction between 0 and 1
  float g;       // a fraction between 0 and 1
  float b;       // a fraction between 0 and 1
} rgb;

typedef struct {
  float h;       // angle in degrees
  float s;       // a fraction between 0 and 1
  float v;       // a fraction between 0 and 1
} hsv;

hsv rgb2hsv(rgb in);
rgb hsv2rgb(hsv in);

float P5_SIN( float in );
float ckit_sin( float in );

void WriteKasumiData( void );
void ReadKasumiData( void );

#pragma pop
#endif
#endif