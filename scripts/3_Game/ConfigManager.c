class KR_KingOfTheHillConfig
{
    string                                              m_ModVersion;
    float                                               m_CaptureTime;
    float                                               m_UpdateInterval;
    float                                               m_ServerStartDelay;
    float                                               m_HillEventInterval;
    float                                               m_EventCleanupTime;
    float                                               m_EventPreStart;
    float                                               m_EveryEventCooldown;
    string                                              m_EventPreStartMessage;
    string                                              m_EventCapturedMessage;
    string                                              m_EventDespawnedMessage;
    string                                              m_EventStartMessage;
    bool                                                m_DoLogsToCF;
    int                                                 m_PlayerPopulationToStartEvents;
    int                                                 m_MaxEvents;
    string                                              m_FlagName;
    autoptr TStringArray                                m_Creatures;
    autoptr array<autoptr KR_KingOfTheHillLocation>     m_HillLocations;
    autoptr array<autoptr KR_KingOfTheHillRewardPool>   m_RewardPools;

    [NonSerialized()]
    const static string                                 m_CurrentVersion = "2.0";

    void KR_KingOfTheHillConfig()
    {
        m_HillLocations = new autoptr array<autoptr KR_KingOfTheHillLocation>;
        m_RewardPools = new autoptr array<autoptr KR_KingOfTheHillRewardPool>;
        m_Creatures = new TStringArray();
    }

    bool AreOutdated()
    {
        if(m_ModVersion && m_ModVersion != m_CurrentVersion)
            return true;
        return false;
    }

    autoptr KR_KingOfTheHillRewardPool GetRandomRewardWithTier(int Tier)
    {
        array<autoptr KR_KingOfTheHillRewardPool> temp = new array<autoptr KR_KingOfTheHillRewardPool>;

        foreach(KR_KingOfTheHillRewardPool pool: m_RewardPools)
        {
            if(pool.m_LootTier == Tier)
                temp.Insert(pool);
        }

        return temp.GetRandomElement();
    }

    autoptr KR_KingOfTheHillLocation GetRandomLocation()
    {
        return m_HillLocations.GetRandomElement();
    }

    float GetServerStartDelay()
    {
        return m_ServerStartDelay * 1000;
    }

    float GetPrestartTime()
    {
        return m_EventPreStart * 1000;
    }

    float GetEventCooldown()
    {
        return m_EveryEventCooldown;
    }

    void LoadDefaultSettings()
    {
        string WorldName;
        GetGame().GetWorldName(WorldName);
        m_ModVersion = m_CurrentVersion;
        m_CaptureTime = 900;
        m_UpdateInterval = 4;
        m_ServerStartDelay = 600;
        m_HillEventInterval = 1500;
        m_EventCleanupTime = 1800;
        m_EventPreStart = 200;
        m_EveryEventCooldown = 100;
        m_EventPreStartMessage = "A Hill is spawning at %location% in %mintes% Minutes";
        m_EventStartMessage = "A Hill has spawned at %location%";
        m_EventCapturedMessage = "Hill at %location% has been captured by %winner%";
        m_EventDespawnedMessage = "Sorry, the Hill at %location% has despawned";
        m_DoLogsToCF = true;
        m_PlayerPopulationToStartEvents = 1;
        m_MaxEvents = 2;
        m_FlagName = "bldr_flag_chernarus_wall";

        HandleStandartLocations(WorldName);
        //string ZoneName, float Radius, float CaptureRadius, bool ResetCaptureTimeAfterAllPlayersLeavedZone, vector Position, int AISpawnCount, string ObjectListName = "none", int LootTier
        

        TStringArray M4Att = new TStringArray;
        M4Att.Insert("Mag_STANAG_30Rnd");
        M4Att.Insert("ACOGOptic");
        M4Att.Insert("M4_Suppressor");
        M4Att.Insert("M4_CQBBttstck_Black");
        M4Att.Insert("M4_RISHndgrd_Black");
        KR_KingOfTheHillReward M4A1Setup = new KR_KingOfTheHillReward("M4A1", M4Att, 1);
        array<autoptr KR_KingOfTheHillReward> TempRew = new array<autoptr KR_KingOfTheHillReward>;
        TempRew.Insert(M4A1Setup);
        TempRew.Insert(M4A1Setup);
        m_RewardPools.Insert(new autoptr KR_KingOfTheHillRewardPool("KingOfTheHillRewardChest", TempRew, 1, 2, 1));
        m_Creatures.Insert("ZmbM_PatrolNormal_PautRev");
        m_Creatures.Insert("ZmbM_PatrolNormal_Autumn");
        m_Creatures.Insert("ZmbM_PatrolNormal_Flat");
        m_Creatures.Insert("ZmbM_PatrolNormal_Summer");
        m_Creatures.Insert("ZmbM_SoldierNormal");
        m_Creatures.Insert("ZmbM_usSoldier_normal_Woodland");
        m_Creatures.Insert("ZmbM_usSoldier_normal_Desert");
        m_Creatures.Insert("Animal_UrsusArctos");
        Save();
    }

    void HandleChernarus()
    {
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Stary Sobor",  80, 20, true, Vector(6037.03, 300.77, 7828.89), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Nordwest Airfield", 80, 20, true, Vector(4460.908, 342.623, 10455.90), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Zelenogorsk", 80, 20, true, Vector(2256.03, 185.59, 5160.75), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Novy Lug Military", 80, 20, true, Vector(9524.45, 240.15, 11780.40), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("StaryYar", 80, 20, true, Vector(4884.66, 476.26, 14999.48), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Airstrip Balota", 80, 20, true, Vector(4987.45, 9.51, 2476.15), 10, 1));
    }

    void HandleDeerisle()
    {
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Sunset Cross Military",  60, 30, true, Vector( 6651.72, 17.29, 6851.60), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Airfield", 60, 30, true, Vector(5515.50, 67.09, 3799.81), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Airfield", 60, 30, true, Vector( 5523.56, 74.01, 3606.71), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Paris Island", 60, 30, true, Vector(3198.09,18.45, 4150.25), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Prison", 60, 30, true, Vector( 5556.52, 34.84, 732.72), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Camp Eagle", 60, 30, true, Vector(8834.75, 45.41, 3797.18), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("East Harbor", 60, 30, true, Vector( 11415.52, 19.79, 10114.19), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Area 42 Prison", 60, 30, true, Vector( 13328.39, 12.92, 9756.89), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Rockenheim Circuit", 60, 30, true, Vector( 9789.67, 39.98, 11092.82), 10, 1));
        m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Power Plant", 60, 30, true, Vector( 4635.28, 20.42, 7071.34), 10, 1));
    }

    void HandleNamalsk()
    {

    }

    void HandleStandartLocations(string MapName)
    {
        MapName.ToLower();
        switch(MapName)
        {
            case "chernarusplus":
            {
                HandleChernarus();
                break;
            }
            case "chernarusplusgloom":
            {
                HandleChernarus();
                break;
            }

            case "deerisle":
            {
                HandleDeerisle();
                break;
            }
            case "namalsk":
            {
                HandleNamalsk();
                break;
            }
            default:
            {
                m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Unkown Map", 0, 0, false, vector.Zero, 0, 0));
                m_HillLocations.Insert(new autoptr KR_KingOfTheHillLocation("Unkown Map Another Example", 0, 0, false, vector.Zero, 0, 0));
            }
        }
    }

    void InitialNewConfig(KR_KingOfTheHillOldConfig oldCfg)
    {
        this.m_ModVersion = m_CurrentVersion;
        this.m_CaptureTime = oldCfg.m_CaptureTime;
        this.m_UpdateInterval = oldCfg.m_UpdateInterval;
        this.m_ServerStartDelay = oldCfg.m_ServerStartDelay;
        this.m_HillEventInterval = oldCfg.m_HillEventInterval;
        this.m_EventCleanupTime = oldCfg.m_EventCleanupTime;
        this.m_EventPreStart = oldCfg.m_EventPreStart;
        this.m_EventPreStartMessage = oldCfg.m_EventPreStartMessage;
        this.m_EventStartMessage = oldCfg.m_EventStartMessage;
        this.m_EventCapturedMessage = oldCfg.m_EventCapturedMessage;
        this.m_EventDespawnedMessage = oldCfg.m_EventDespawnedMessage;
        this.m_DoLogsToCF = oldCfg.m_DoLogsToCF;
        this.m_PlayerPopulationToStartEvents = oldCfg.m_PlayerPopulationToStartEvents;
        this.m_MaxEvents = oldCfg.m_MaxEvents;
        this.m_FlagName = oldCfg.m_FlagName;
        this.m_HillLocations = oldCfg.m_HillLocations;
        this.m_RewardPools = oldCfg.m_RewardPools;
        this.m_Creatures = oldCfg.m_Creatures;
        SetNewDefaultValues();
        Save();
    }

    void SetNewDefaultValues()
    {
        m_EveryEventCooldown = 100;
        foreach(KR_KingOfTheHillLocation location: m_HillLocations)
        {
            location.SetNewDefaultValues();
        }

        foreach(KR_KingOfTheHillRewardPool pool: m_RewardPools)
        {
            pool.SetNewDefaultValues();
        }
    }

    void Save()
    {
        if(!FileExist("$profile:KingOfTheHill"))
            MakeDirectory("$profile:KingOfTheHill");
        if(!FileExist("$profile:KingOfTheHill/Objects"))
            MakeDirectory("$profile:KingOfTheHill/Objects");
        
        JsonFileLoader<KR_KingOfTheHillConfig>.JsonSaveFile("$profile:KingOfTheHill/server-config.json", this);
    }

    static autoptr KR_KingOfTheHillConfig Load()
    {
        KR_KingOfTheHillConfig settings = new KR_KingOfTheHillConfig();

        if(FileExist("$profile:KingOfTheHill/server-config.json"))
        {
            JsonFileLoader<KR_KingOfTheHillConfig>.JsonLoadFile("$profile:KingOfTheHill/server-config.json", settings);
        }
        else
        {
            settings.LoadDefaultSettings();
        }

        if(settings && settings.AreOutdated() && FileExist("$profile:KingOfTheHill/server-config.json"))
        {
            if(settings.m_ModVersion != m_CurrentVersion)
            {
                KOTHLoggingService.Log("The Old Config is outdated, try to update it!");
                //Update....
                KR_KingOfTheHillOldConfig old = new KR_KingOfTheHillOldConfig();
                JsonFileLoader<KR_KingOfTheHillOldConfig>.JsonLoadFile("$profile:KingOfTheHill/server-config.json", old);
                if(old)
                {
                    //Old config Compatible with known Modal start Compare.
                    KOTHLoggingService.Log("old config is compatible with compare system! Start Compare!");
                    settings.InitialNewConfig(old);
                }
                else
                {
                    //Error with loading old file maybe to long outdated.
                    if(!FileExist("$profile:KingOfTheHill/OldConfig"))
                        MakeDirectory("$profile:KingOfTheHill/OldConfig");
                    CopyFile("$profile:KingOfTheHill/server-config.json", "$profile:KingOfTheHill/OldConfig/server-config.json");
                    DeleteFile("$profile:KingOfTheHill/server-config.json");
                    settings.LoadDefaultSettings();
                    KOTHLoggingService.Log("old config was not compatible with compare system! Old Config moved to folder: OldConfig! Please setup the new config with the old values!");
                }
            }
         }


        return settings;
    }
};