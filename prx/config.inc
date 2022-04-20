CONFIG_START()
    CONFIG_SETTING( BOOL, debug, "Debugging features", true )
    CONFIG_SETTING( BOOL, functest, "Print Function Names", false )

#ifdef GAME_P5
    CONFIG_SETTING( BOOL, enableEXFLWModule, "Enable Flowscripts module", true )
    CONFIG_SETTING( BOOL, enableDCModule, "Enable dc module", true )
    CONFIG_SETTING( BOOL, enableP5EModule, "Enable P5 Enhanced module", true )
    CONFIG_SETTING( BOOL, enableKasumiModule, "Enable Kasumi Fixes module", true )
    CONFIG_SETTING( BOOL, enableScarltzModule, "Enable Scarltz module", true )
    CONFIG_SETTING( BOOL, enableRandomizerModule, "Enable Randomizer module", false )
    CONFIG_SETTING( BOOL, enableSecreCModule, "Enable SecreC module", true )
    CONFIG_SETTING( BOOL, enableGlobalBF, "Global Script", true )
    CONFIG_SETTING( BOOL, enableSumire, "Sumire models", false )
    CONFIG_SETTING( BOOL, enableCutsceneOutfits, "Player Outfits in cutscenes", true )
    CONFIG_SETTING( BOOL, enableAkechiMod, "Royal Akechi patches", true )
    CONFIG_SETTING( BOOL, enablePersonaEnemies, "Persona Using Enemies", true )
    CONFIG_SETTING( BOOL, enableExpandedBGM, "Expanded BGM Patches", true )
    CONFIG_SETTING( BOOL, randomDLCBGM, "Randomized DLC BGM", false )
    CONFIG_SETTING( BOOL, randomCombatOutfit, "Randomized Combat Outfits", false )
    CONFIG_SETTING( BOOL, enableExternalNavi, "External navigator text file", false )
    CONFIG_SETTING( BOOL, enableCustomNaviSoundPack, "Custom Navigator SoundPack", false )
    CONFIG_SETTING( BOOL, forceSingleCombatGAP, "Force Single GAP Player Combat Animations", true )
    CONFIG_SETTING( BOOL, forcePSZModel, "Force PSZ Models", false )
    CONFIG_SETTING( BOOL, enableModCPK, "mod cpk", true )
    CONFIG_SETTING( BOOL, enableKasumi, "Kasumi patches", false )
    CONFIG_SETTING( BOOL, enableMidWinterOutfits, "Midwinter Outfits", false )
    CONFIG_SETTING( BOOL, enableTTYCommands, "tty commands", true )
    CONFIG_SETTING( BOOL, royalAmbushBGM, "Use Take Over as Ambush Theme", false )
    CONFIG_SETTING( BOOL, ambushOverDLC, "Ambush themes override DLC Music", false )
    CONFIG_SETTING( BOOL, P5RTitleBGM, "Alternate title screens", true )
    CONFIG_SETTING( BOOL, AutoUnlockBatonPass, "Automatically Unlock Baton Pass", true )
    CONFIG_SETTING( BOOL, DisableInstakill, "Disable Ryuji Instakill", false )
    CONFIG_SETTING( BOOL, DebugModelTesting, "Experimental Model Testing", false )
    CONFIG_SETTING( BOOL, PreventJokerGameOver, "Prevent Game Over on Joker Death", false )
    CONFIG_SETTING( STRING,  modCPKName,"mod cpk name",  "mod" )
    CONFIG_SETTING( INT,  extraModCPK,  "Number of extra mod cpk", 0 )
    CONFIG_SETTING( INT,  maxDLCBGM,  "Maximum random DLC BGM ID", 29 )
    CONFIG_SETTING( BOOL, enableFileAccessLogging, "file access logging", true )
    CONFIG_SETTING_ARRAY( STRING, extraModCPKName, "names of extra mod cpk", 10,
    "mod", "mod", "mod", "mod", "mod", "mod", "mod", "mod", "mod", "mod")

    CONFIG_SETTING( BOOL, randomizeEncounters, "Randomize Encounters", true )
    CONFIG_SETTING( BOOL, randomizeEnemies, "Randomize Enemies", false )
    CONFIG_SETTING( BOOL, randomizePartyMembers, "Randomize Party", true )
    CONFIG_SETTING( BOOL, randomizeHP, "Randomize HP", true )
    CONFIG_SETTING( BOOL, randomizeSP, "Randomize SP", true )
    CONFIG_SETTING( BOOL, randomizePersonaStats, "Randomize Persona Stats", true )
    CONFIG_SETTING( BOOL, randomizePersonaSkills, "Randomize Persona Skills", true )
    CONFIG_SETTING( BOOL, giveJokerDebugInstakill, "Give Joker Instakill skill", false )
        
#endif
#ifdef GAME_TEST
    // Example config setting values
    //             type      short name        long name        default value
    CONFIG_SETTING( BOOL,    enableTestModule, "Test module",   false )
    CONFIG_SETTING( INT,     testInt,          "Test int",      1234 )
    CONFIG_SETTING( FLOAT,   testFloat,        "Test float",    1234.012f )
    CONFIG_SETTING( STRING,  testMessage,      "Test message",  "Hello World" )

    // Example config setting array values
    // Default max array size is 256.
    //                    value type  short name       long name                         # default values  default values
    CONFIG_SETTING_ARRAY( BOOL,       testBoolArray,   "Test bool array",                0 )
    CONFIG_SETTING_ARRAY( INT,        testIntArray,    "Test int array",                 0 )
    CONFIG_SETTING_ARRAY( FLOAT,      testFloatAray,   "Test float array",               0 )
    CONFIG_SETTING_ARRAY( STRING,     testRmotd,       "Test random message of the day", 4,                "Hi", "Hello", "Hello World", "Bonjour" )
#endif
CONFIG_END()