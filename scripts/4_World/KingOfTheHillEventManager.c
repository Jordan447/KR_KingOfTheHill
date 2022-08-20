class KingOfTheHillEventManager
{
    protected autoptr KR_KingOfTheHillConfig m_serverconfig;
    protected autoptr array<autoptr KR_KingOfTheHillZone> m_ActiveEvents;
    protected autoptr array<autoptr KR_KingOfTheHillLocation> m_ActiveEventLocations;
    protected autoptr KR_KingOfTheHillLocation m_LastLocation;

    protected float m_EventUpdateTimer = 1.0;
    protected float m_HillDropTimer = 500.0;
    protected float m_Cooldown;
    void KingOfTheHillEventManager()
    {
        m_serverconfig = KR_KingOfTheHillConfig.Load();
        m_ActiveEvents = new array<autoptr KR_KingOfTheHillZone>;
        m_ActiveEventLocations = new array<autoptr KR_KingOfTheHillLocation>;
        m_EventUpdateTimer = m_serverconfig.m_UpdateInterval;
        m_HillDropTimer = m_serverconfig.m_HillEventInterval;

    }

    void OnServerRPCRecived(PlayerBase player, PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        switch(rpc_type)
        {
            case KOTH_RPCs.RPC_GETALLEVENTS:
            {
                SendPlayerAllEvents(player);
                break;
            }
            case KOTH_RPCs.RPC_ADDTOZONE:
            {
                AddPlayerToCaptureZone(player, ctx);
                break;
            }
        }
    }

    void SendPlayerAllEvents(PlayerBase player)
    {
        GetGame().RPCSingleParam(player, KOTH_RPCs.RPC_RECIVEALLEVENTS, new Param1<array<autoptr KR_KingOfTheHillLocation>>(m_ActiveEventLocations), true, player.GetIdentity());
    }

    void SyncZone(PlayerBase player, KR_KingOfTheHillClientSyncModel syncData)
    {
        GetGame().RPCSingleParam(player, KOTH_RPCs.RPC_SYNCCAPTURE, new Param1<KR_KingOfTheHillClientSyncModel>(syncData), true, player.GetIdentity());
    }

    void RemovePlayerFromZone(PlayerBase player, KR_KingOfTheHillZone zone)
    {
        GetGame().RPCSingleParam(player, KOTH_RPCs.RPC_REMOVEFROMZONE, null, true, player.GetIdentity());
    }

    void AddPlayerToCaptureZone(PlayerBase player, ParamsReadContext ctx)
    {
        if(!player) return;
        if(player.IsPlayerDisconnected()) return;
        if(!player.IsAlive()) return;
        Param1<KR_KingOfTheHillLocation> data;
        if(!ctx.Read(data)) return;
        KR_KingOfTheHillLocation clientLoc = data.param1;
        KR_KingOfTheHillZone serversLoc;
        for(int i = 0; i < m_ActiveEvents.Count(); i++)
        {
            if(m_ActiveEvents.Get(i).m_Location.Position == clientLoc.Position){
                serversLoc = m_ActiveEvents.Get(i);
                break;
            }
        }

        if(!serversLoc) return; 


        float Distance = vector.Distance(clientLoc.Position, player.GetPosition());
        if(Distance > clientLoc.Radius)
        {
            Print(player.GetIdentity().GetPlainId() + " tryed to send an RPC, but is not in Range! This could be an cheater!");
            return;
        }
        else
        {
            bool Result = serversLoc.AddPlayerToSyncList(player);
            GetGame().RPCSingleParam(player, KOTH_RPCs.RPC_SETCLIENTINZONE, new Param1<bool>(Result), true, player.GetIdentity());
            
        }
    }

    void OnUpdate(float timeslice)
    {   
        if(m_EventUpdateTimer >= m_serverconfig.m_UpdateInterval)
        {
            m_Cooldown -=  m_serverconfig.m_UpdateInterval;
            if(m_HillDropTimer >= m_serverconfig.m_HillEventInterval)
            {
                if(CanStartNewHill())
                {
                    CreateHillEvent();
                    m_HillDropTimer = 0;
                }
            }

            //Update
            foreach(KR_KingOfTheHillZone zone : m_ActiveEvents)
            {
                if(!zone)
                    continue;
                
                if(zone.NeedsDeleted())
                    m_ActiveEvents.RemoveItem(zone);
                else
                    zone.Update(timeslice, m_serverconfig.m_UpdateInterval);
            }
            m_EventUpdateTimer = 0;
        }
        else
        {
            m_EventUpdateTimer += timeslice;
            m_HillDropTimer += timeslice;
        }
    }


    bool CanStartNewHill()
    {
        if(m_ActiveEvents.Count() >= m_serverconfig.m_MaxEvents)
            return false;
        
        array<Man> players = new array<Man>;
	    GetGame().GetPlayers( players );

        if(players.Count() < m_serverconfig.m_PlayerPopulationToStartEvents)
            return false;

        if(m_Cooldown > 0)
            return false;
        else
            m_Cooldown = m_serverconfig.m_EveryEventCooldown;
        
        return true;
    }


    void Init()
    {
        if(!m_serverconfig){
            m_serverconfig = KR_KingOfTheHillConfig.Load();
        }

        if(!m_serverconfig) {
            Error("Can not start King of the Hill! Config Module is not working, or maybe not found!");
            return;
        }

        KOTHLoggingService.Log("OnInit Event Manager.");
        KOTHLoggingService.Log("First Hill Spawning in:  " + m_serverconfig.m_HillEventInterval.ToString() + "seconds.");
        Print("[King of the Hill] -> Event Spawn Time every " +  m_serverconfig.m_HillEventInterval.ToString() + "s.");
    }

    void CreateHillEvent()
    {
        //KR_KingOfTheHillLocation location, float CaptureTime, float CleanupTime
        KR_KingOfTheHillLocation newLoc = m_serverconfig.GetRandomLocation();
        if(newLoc)
        {
            if(m_LastLocation)
            {
                while(m_LastLocation.Position == newLoc.Position)
                {
                    newLoc = m_serverconfig.GetRandomLocation();
                }
            }
            else
            {
                m_LastLocation = newLoc;
            }
        }
        else
        {
            Error("[King of the Hill] -> Error can not find any locations in config, please check the JSON Sytnax!");
            return;
        }
        m_ActiveEventLocations.Insert(newLoc);
        KR_KingOfTheHillZone newZone = new KR_KingOfTheHillZone(newLoc);
        m_ActiveEvents.Insert(newZone);
        Print("[King of the Hill] -> Start Fresh Event: " + newLoc.Name);
        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(newLoc);
        rpc.Send(null, KOTH_RPCs.RPC_RECIVEZONE, true);
    }


    static autoptr KingOfTheHillEventManager g_Instance;
    static autoptr KingOfTheHillEventManager GetInstance()
    {
        if(GetGame().IsServer())
        {
            if(!g_Instance)
                g_Instance = new KingOfTheHillEventManager();

            return g_Instance;
        }
        else
        {
            return null;
        }
    }
};