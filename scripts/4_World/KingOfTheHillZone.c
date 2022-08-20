class KR_KingOfTheHillZone
{
    protected autoptr array<PlayerBase>         m_PlayersInside;
    protected float                             m_CaptureTime;
    protected float                             m_TimeCaptured;
    protected float                             m_CleanupTime;
    protected float                             m_CleanupTick;
    protected bool                              m_IsEventRunning = false;
    protected bool                              m_DeleteZoneNeeded = false;
    protected bool                              m_WasEventCaptured = false;
    protected bool                              m_StartMessageSendet = false;
    protected bool                              m_CanStartEvent = true;
    protected bool                              m_WasWinning;
    protected bool                              m_IsAnyPlayerInCaptureRange;
    protected string                            m_FlagClassName;
    autoptr KR_KingOfTheHillLocation            m_Location;
    autoptr array <EntityAI>                    m_EventObjects;
    autoptr KR_KingOfTheHillBuildingSetsLoader  m_BuildLoader;
    const float                                 m_flagXOffset = 0.7;
    const float                                 m_flagYOffset = 5.96;
    const float                                 m_smokeYOffset = 6.4;
    autoptr KR_KingOfTheHillConfig              m_config;

    #ifdef LBGROUP_SYSTEM
    autoptr LBServerMarker       m_LPMasterMarker;
    #endif

    #ifdef PVEZ
    protected autoptr PVEZ_Zone zone;
    #endif

    void KR_KingOfTheHillZone(KR_KingOfTheHillLocation location)
    {
        m_config                = KR_KingOfTheHillConfig.Load();
        this.m_PlayersInside    = new autoptr array<PlayerBase>;
        this.m_EventObjects     = new array <EntityAI>;
        this.m_Location         = location;
        this.m_CaptureTime      = m_config.m_CaptureTime;
        this.m_FlagClassName    = m_config.m_FlagName;
        this.m_CleanupTime      = m_config.m_EventCleanupTime;
        this.m_CleanupTick      = m_config.m_EventCleanupTime;

        CreateMapMarker();

        if(!m_StartMessageSendet)
        {
            m_StartMessageSendet = true;
            float mins = m_config.m_EventPreStart / 60;
            ServerNotification(ReplaceLocationPlaceHolder(ReplaceTimePlaceHolder(m_config.m_EventPreStartMessage, mins)));
        }

        GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(this.StartEvent, m_config.GetPrestartTime(), false); 
    }

    void ~KR_KingOfTheHillZone()
    {
        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(m_Location);
        rpc.Send(null, KOTH_RPCs.RPC_REMOVEZONE, true);
        #ifdef LBGROUP_SYSTEM
        LBStaticMarkerManager.Get().RemoveServerMarker(m_LPMasterMarker);
        #endif

        #ifdef PVEZ
        if(zone)
            g_Game.pvez_Zones.RemoveZone(zone);
        #endif
    }


    //Serverside Create Map Marker function. If you have a custom Map system on your server just override this function!
    void CreateMapMarker()
    {
        #ifdef LBGROUP_SYSTEM
        m_LPMasterMarker = LBStaticMarkerManager.Get().AddTempServerMarker(" KING OF THE HILL", m_Location.Position, "KR_KingOfTheHill/GUI/images/Flag.paa", ARGB(240, 206, 9, 9), false, true, true, true);
        if(!m_LPMasterMarker)
            Error("Cant create LB Master Server Marker!");
        #endif
    }

    void StartEvent()
    {
        if(!m_IsEventRunning)
        {
            ServerNotification(ReplaceLocationPlaceHolder(m_config.m_EventStartMessage));
            if(m_Location.ObjectListName != "none")
            {
                m_BuildLoader = new KR_KingOfTheHillBuildingSetsLoader();
                m_BuildLoader.ReadFileAndSpawnBuildings(m_Location.ObjectListName);
            }

            SpawnEventObjects();
            SpawnEventCreatures();
            m_IsEventRunning = true;
            m_CanStartEvent = true;
        }   
    }

    void ServerNotification(string message)
    {
        if(message == "")
            return;

        #ifdef NOTIFICATIONS
        NotificationSystem.SimpleNoticiation(message, "KING OF THE HILL", "KR_KingOfTheHill/GUI/images/FlagIcon.edds", ARGB(240, 138, 6, 6), 5);
        #else
        array<Man> onlinePlayers = new array<Man>();
        GetGame().GetPlayers(onlinePlayers);

        foreach(Man player: onlinePlayers)
        {
            if(!player)
                continue;
             NotificationSystem.SendNotificationToPlayerExtended(player, 10, "KING OF THE HILL", message, "KR_KingOfTheHill/GUI/images/FlagIcon.edds");
        }
        #endif
    }


    bool CanStartEvent()
    {
        return !m_IsEventRunning;
    }

    bool NeedsDeleted()
    {
        return m_DeleteZoneNeeded;
    }

    bool IsEventCaptured()
    {
        return m_WasEventCaptured;
    }

    void SpawnEventObjects()
    {
        float flagHightOffset = m_Location.Position[1] + 3;
        EntityAI flagPole = EntityAI.Cast(GetGame().CreateObject("bldr_misc_flagpole", Vector(m_Location.Position[0], flagHightOffset, m_Location.Position[2])));
        m_EventObjects.Insert(flagPole);
        float flagXPos = m_Location.Position[0] + m_flagXOffset;
        float flagYPos = m_Location.Position[1] + m_flagYOffset;
        float smokeYPos =  m_Location.Position[1] + m_smokeYOffset;
        EntityAI flag = EntityAI.Cast(GetGame().CreateObject(m_FlagClassName, Vector(flagXPos, flagYPos, m_Location.Position[2])));
        if(flag)
        {
            m_EventObjects.Insert(flag);
        }
        else
        {
            Error("[King of The Hill] -> Error, can not spawn flag with classname: " + m_FlagClassName);
        }
        EntityAI HillSmoke = EntityAI.Cast(GetGame().CreateObjectEx("KR_KingOfTheHillSmoke", Vector(m_Location.Position[0], smokeYPos, m_Location.Position[2]), ECE_NONE));
		HillSmoke.SetOrientation(Vector(0,90,90));
		m_EventObjects.Insert(HillSmoke);

        #ifdef PVEZ
        //int type, float coordX, float coordZ, float radius, string name, bool showBorder = true, bool showName = false
		zone = g_Game.pvez_Zones.AddZone(
			PVEZ_ZONE_TYPE_AIRDROP, // 1
			flagXPos,
			m_Location.Position[2],
			m_Location.Radius,
			m_Location.Name,
			true,
			true);
        #endif
    }

    void DeleteAllEventObjects()
    {
        foreach(EntityAI obj : m_EventObjects)
        {
            if(!obj)
                continue;
            
            GetGame().ObjectDelete(obj);
        }

        if(m_BuildLoader)
            m_BuildLoader.DeleteAllBuildings();
        m_DeleteZoneNeeded = true;
    }

    bool IsAnyPlayerInZone()
    {
        return m_PlayersInside.Count() > 0;
    }

    void SpawnObjectInLocation(string TypeName, bool AIInit)
    {
        float XRandomOffset = Math.RandomFloat(5, 10);
        float ZRandomOffset = Math.RandomFloat(5, 10);
        float XPos = m_Location.Position[0] + XRandomOffset;
        float ZPos = m_Location.Position[2] + ZRandomOffset;
        float YPos = GetGame().SurfaceY(XPos,ZPos);
        vector FinalPos = Vector(XPos, YPos, ZPos);
        EntityAI EventObj = EntityAI.Cast(GetGame().CreateObject(TypeName, FinalPos, false, AIInit));
        m_EventObjects.Insert(EventObj);
    }

    protected void SpawnEventCreatures()
    {
        for(int i = 0; i < m_Location.AISpawnCount; i++)
        {
            string RandomZombieType = m_config.m_Creatures.GetRandomElement();
            SpawnObjectInLocation(RandomZombieType, true);
        }
    }

    protected bool IsItemBatterieCompatible(EntityAI item)
    {
        return item && item.HasEnergyManager();
    }

    protected void SpawnRewards()
    {
        if(m_config)
        {
            float closest = 0;
            KR_KingOfTheHillRewardPool winningReward = m_config.GetRandomRewardWithTier(m_Location.LootTier);
            
            if(!winningReward)
            {
                Error("[KOTH] -> Can not find any reward with Tier: " + m_Location.LootTier);
                ServerNotification("Error with spawing rewards on Hill: " + m_Location.Name + " please inform the server admins to fix the config!");
                return;
            }


            float ChestXOffset = 0.4;
            float ChestY = m_Location.Position[1];
            float ChestZ = m_Location.Position[2];
    
            vector ChestPosition = Vector(m_Location.Position[0] + ChestXOffset, m_Location.Position[1], m_Location.Position[2]);
            EntityAI RewardContainer = Container_Base.Cast(GetGame().CreateObjectEx(winningReward.RewardContainerName, ChestPosition, ECE_PLACE_ON_SURFACE));
            if(RewardContainer)
            {
                int RandomInt = 0;
                if(winningReward.m_MaxWinningItems == -1 && winningReward.m_MinWinningItems == -1)
                    RandomInt = winningReward.m_Rewards.Count();
                else    
                    RandomInt = Math.RandomInt(winningReward.m_MinWinningItems, winningReward.m_MaxWinningItems);
                
                for(int i = 0; i < RandomInt; i++)
                {
                    KR_KingOfTheHillReward reward;
                    if(winningReward.m_Rewards.Count() == RandomInt)
                        reward = winningReward.m_Rewards.Get(i);
                    else
                        reward = winningReward.m_Rewards.GetRandomElement();
                    
                    EntityAI item = EntityAI.Cast(RewardContainer.GetInventory().CreateInInventory( reward.ItemName));
                    if(!item)
                    {
                        Error("[KOTH] -> Cant create item in Container: " + reward.ItemName + " maybe wrong classname of item in json!");
                        return;
                    }

                    ItemBase itembase = ItemBase.Cast(item);
                    if(itembase)
                    {
                        itembase.SetQuantity(reward.Quantity);
                    }

                    if(reward.Attachments && reward.Attachments.Count() > 0)
                    {
                        for(int y = 0; y < reward.Attachments.Count(); y++)
                        {
                            if(reward.Attachments.Get(y) == "Battery9V")
                                continue;
                            

                            EntityAI att = EntityAI.Cast(item.GetInventory().CreateAttachment(reward.Attachments.Get(y)));
                            if(!att)
                            {
                                Error("[KOTH] -> Can`t create Attachment: " + reward.Attachments.Get(y) + " on item: " + item.GetType() + "maybe wrong classname of item in json! Or maybe not compatible with Item!");
                                return;
                            }
                            
                            if(IsItemBatterieCompatible(att))
                            {
                                //Optic check for Batterie
                                if(reward.Attachments.Find("Battery9V") != -1 && att.HasEnergyManager())
                                {
                                    att.GetInventory().CreateAttachment("Battery9V");
                                }
                            }

                            if(!att)
                            {
                                Error("Can not spawn Attachment: " + reward.Attachments.Get(y) +   " on Item: " + item.GetType());
                            }
                        }
                    }
                }
            }
            else
            {
                Error("Can not Spawn Container! Maybe wrong classname!");
            }
        }
        else
        {
            Error("Can not spawn rewards config is broken!");
        }
    }

    bool CanAnyPlayerCapture()
    {
        return this.m_IsAnyPlayerInCaptureRange;
    }

    void Update(float timeslice, float EventTick)
    {
        if(!m_CanStartEvent && !CanStartEvent())
            return;
        
        if(IsAnyPlayerInZone() && m_CanStartEvent)
        {
            m_CleanupTick = m_CleanupTime;
            if(CanAnyPlayerCapture())
                m_TimeCaptured += EventTick;
            

            if(m_TimeCaptured >= m_CaptureTime)
                m_WasEventCaptured = true;

            float Progress = m_TimeCaptured / m_CaptureTime;
            Progress = Progress * 100;
            bool AnyPlayerInZone = false;
            foreach(PlayerBase player : m_PlayersInside)
            {
                if(!player)
                    continue;
                
                bool IsInZone = IsPlayerInZone(player);
                bool IsInCaptureRangeLoc = IsInCaptureRange(player);
                if(IsInCaptureRangeLoc)
                {
                    AnyPlayerInZone = true;
                }

                if(IsInZone && !IsEventCaptured())
                {
                    KR_KingOfTheHillClientSyncModel data = new KR_KingOfTheHillClientSyncModel();
                    data.SetData(m_PlayersInside.Count().ToString(), Progress, IsInZone);
                    KingOfTheHillEventManager.GetInstance().SyncZone(player, data);
                }
                else
                {
                    if(IsEventCaptured() && !m_WasWinning)
                    {
                        m_WasWinning = true;
                        OnEventWon(FindWinningPlayer());
                    }

                    m_PlayersInside.RemoveItem(player);
                    KingOfTheHillEventManager.GetInstance().RemovePlayerFromZone(player, this);
                }
            }

            m_IsAnyPlayerInCaptureRange = AnyPlayerInZone;
        }
        else
        {
            if(m_TimeCaptured != 0 && m_Location.ResetCaptureTimeAfterAllPlayersLeavedZone)
                m_TimeCaptured = 0;
            
            m_CleanupTick -= EventTick;
            if(m_CleanupTick <= 0)
            {
                if(!IsAnyPlayerInZone() && !m_CanStartEvent)
                {
                    DeleteAllEventObjects();
                    ServerNotification(ReplaceLocationPlaceHolder(m_config.m_EventDespawnedMessage));
                    m_DeleteZoneNeeded = true;
                }
                else
                {
                    m_CleanupTick = m_CleanupTime;
                }
            }
        }
    }

    void OnEventWon(PlayerBase player)
    {
        ServerNotification(ReplaceLocationPlaceHolder(ReplacePlayerPlaceHolder(m_config.m_EventCapturedMessage, player)));
        SpawnRewards();
        DeleteAllEventObjects();
    }

    string ReplacePlayerPlaceHolder(string Placeholder, PlayerBase player)
    {
        if(Placeholder.Contains("%winner%"))
		{
			Placeholder.Replace("%winner%", player.GetIdentity().GetName());
		}

        if(Placeholder.Contains("%winnerWithSteam%"))
		{
			Placeholder.Replace("%winnerWithSteam%", player.GetIdentity().GetName() + " | " + player.GetIdentity().GetPlainId());
		}

        return Placeholder;

    }

    string ReplaceTimePlaceHolder(string Placeholder, float timeleft)
    {
        if(Placeholder.Contains("%mintes%"))
		{
			Placeholder.Replace("%mintes%", Math.Round(timeleft).ToString());
		}

        return Placeholder;
    }

    string ReplaceLocationPlaceHolder(string Placeholder)
    {
        if(Placeholder.Contains("%location%"))
		{
			Placeholder.Replace("%location%", m_Location.Name);
		}

        return Placeholder;
    }

    PlayerBase FindWinningPlayer()
    {
        PlayerBase winner;
        float closest = m_Location.Radius;
        foreach(PlayerBase player : m_PlayersInside)
        {
            float Distance = vector.Distance(player.GetPosition(), m_Location.Position);
            if(IsPlayerInZone(player) && Distance < closest){
                winner = player;
            }
        }
        return winner;
    }

    

    //Adds an player to the sync list
    //This Bool works only when player is in Zone! Returns True when player was added succesfully, otherwise false!
    //This Bool also starts the Update function when first player gets added! or all Player Leaved the Radius.
    bool AddPlayerToSyncList(PlayerBase player)
    {
        if(IsPlayerInZone(player) && m_PlayersInside.Find(player) == -1 && m_IsEventRunning)
        {
            m_PlayersInside.Insert(player);
            return true;
        }
        return false;
    }

    /* Function Returns True when player is in this KingOfTheHill Zone */
    bool IsPlayerInZone(PlayerBase player)
    {
        if(!player)
            return false;
        
        if(!player.IsAlive() || player.IsPlayerDisconnected())
            return false;
        
        float DistanceToZone = vector.Distance(m_Location.Position, player.GetPosition());

        return DistanceToZone <= m_Location.Radius; 
    }

    /* Function Returns True when player is in able to capture KingOfTheHill Zone */
    bool IsInCaptureRange(PlayerBase player)
    {
        if(!player)
            return false;
        
        if(!player.IsAlive() || player.IsPlayerDisconnected())
            return false;
        
        float DistanceToZone = vector.Distance(m_Location.Position, player.GetPosition());

        return DistanceToZone <= m_Location.CaptureRadius; 
    }
};