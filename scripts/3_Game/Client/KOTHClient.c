class KR_KingOfTheHillClientManager
{
    autoptr KR_KingOfTheHillProgressWidget m_ZoneMenu;
    float m_UpdateCooldown = 0.0;
    bool m_IsInAZone = false;
    #ifdef BASICMAP
    autoptr array<autoptr KOTHMapMarker> m_MapMarkers;
    #endif
    protected autoptr array<autoptr KR_KingOfTheHillLocation> m_ActiveEvents;

    void KR_KingOfTheHillClientManager()
    {
        GetDayZGame().Event_OnRPC.Insert( OnRPC );
        m_ActiveEvents = new autoptr array<autoptr KR_KingOfTheHillLocation>;
        #ifdef BASICMAP
        m_MapMarkers = new array<autoptr KOTHMapMarker>;
        #endif

        m_ZoneMenu = new KR_KingOfTheHillProgressWidget();
        m_ZoneMenu.HandleShow(false);

        KOTHLoggingService.Log("Client successfully created!");
    }

    void ~KR_KingOfTheHillClientManager() 
    {
        #ifdef BASICMAP
        m_MapMarkers.Clear();
        #endif

        m_ActiveEvents.Clear();
        delete m_ZoneMenu;

        KOTHLoggingService.Log("Client successfully deleted!");
    }

    void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		if(rpc_type == KOTH_RPCs.RPC_RECIVEZONE)
        {
            KR_KingOfTheHillLocation newLoc;
            if(!ctx.Read(newLoc)) return;
            AddActiveEvent(newLoc);
            KOTHLoggingService.Log("RPC DEBUG : RPC_RECIVEZONE recived! Location Name: " + newLoc.Name);
        }

        if(rpc_type == KOTH_RPCs.RPC_REMOVEZONE)
        {
            KR_KingOfTheHillLocation oldLoc;
            if(!ctx.Read(oldLoc)) return;
            RemoveActiveEvent(oldLoc);
            KOTHLoggingService.Log("RPC DEBUG : RPC_REMOVEZONE recived!");
        }
    }


    void OnClientRPCRecived(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        switch(rpc_type)
        {
            case KOTH_RPCs.RPC_SYNCCAPTURE:
            {

                RefreshCapture(ctx);
                KOTHLoggingService.Log("RPC DEBUG : RPC_SYNCCAPTURE recived!");
                break;
            }
            case KOTH_RPCs.RPC_RECIVEALLEVENTS:
            {
                AddAllEvents(ctx);
                KOTHLoggingService.Log("RPC DEBUG : RPC_RECIVEALLEVENTS recived!");
                break;
            }
            case KOTH_RPCs.RPC_REMOVEFROMZONE:
            {
                RemoveFromCaptureZone();
                KOTHLoggingService.Log("RPC DEBUG : RPC_REMOVEFROMZONE recived!");
                break;
            }
            case KOTH_RPCs.RPC_SETCLIENTINZONE:
            {
                HandleZone(ctx);
            }
        }
    }

    void HandleZone(ParamsReadContext ctx)
    {
        Param1<bool> data;
        if(!ctx.Read(data)) return;
        KOTHLoggingService.Log("RPC DEBUG : RPC_REMOVEFROMZONE recived! Is IN Zone: " + data.param1);
        m_IsInAZone = data.param1;
    }

    void RemoveFromCaptureZone()
    {
        if(m_ZoneMenu)
            m_ZoneMenu.HandleShow(false);
        
        m_IsInAZone = false;
    }

    void AddAllEvents(ParamsReadContext ctx)
    {
        Param1<autoptr array<autoptr KR_KingOfTheHillLocation>> data;
        if(!ctx.Read(data)) return;
        m_ActiveEvents = data.param1;
        CreateMarkers();
    }

    void CreateMarkers()
    {
        for(int i = 0; i < m_ActiveEvents.Count(); i++)
        {
            KR_KingOfTheHillLocation loc = m_ActiveEvents.Get(i);
            if(!loc)
                continue;
            CreateMarker(loc);
        }
    }

    void CreateMarker(KR_KingOfTheHillLocation data)
    {
        #ifdef BASICMAP
        KOTHMapMarker marker = new KOTHMapMarker("KING OF THE HILL", data.Position);
        marker.EventRadius = data.Radius;
		marker.SetPosition(data.Position);
		marker.SetRadius(data.Radius);
        m_MapMarkers.Insert(marker);
        BasicMap().AddMarker(BasicMap().SERVER_KEY , marker);
        #endif
    }

    void AddActiveEvent(KR_KingOfTheHillLocation data)
    {
        m_ActiveEvents.Insert(data);
        CreateMarker(data);
    }

    void RemoveActiveEvent(KR_KingOfTheHillLocation data)
    {
        m_ActiveEvents.RemoveItem(data);
        m_IsInAZone = false;
        #ifdef BASICMAP
        foreach(KOTHMapMarker marker : m_MapMarkers)
        {
            if(!marker)
                continue;
            
            float Distance = vector.Distance(marker.GetPosition(), data.Position);

            if(Distance && Distance < 5)
            {
                BasicMap().RemoveMarker(BasicMap().SERVER_KEY , marker);
                m_MapMarkers.RemoveItem(marker);
                if(marker)
                    delete marker;
                break;
            }
        }
        #endif
    }

    void RefreshCapture(ParamsReadContext ctx)
    {
        Param1<KR_KingOfTheHillClientSyncModel> data;
        if(!ctx.Read(data)) return;
        if(m_ZoneMenu)
        {
            m_ZoneMenu.UpdateInfo(data.param1);
            m_IsInAZone = data.param1.GetLeavedZone();
            m_ZoneMenu.HandleShow(m_IsInAZone);
        }
    }

    void CheckForPossibleZones()
    {
        Man player = GetGame().GetPlayer();
        if(!player) return;
        if(!player.IsAlive()) return;
        foreach(KR_KingOfTheHillLocation loc : m_ActiveEvents)
        {
            if(!loc)
                continue;
            
            float Distance = vector.Distance(player.GetPosition(), loc.Position);
            float Radius = loc.Radius;
            if(Distance <= Radius)
            {
                if(!m_IsInAZone)
                {
                    GetGame().RPCSingleParam(player, KOTH_RPCs.RPC_ADDTOZONE, new Param1<KR_KingOfTheHillLocation>(loc), true);
                }
                break;
            }
        }

        if(!m_IsInAZone)
            m_ZoneMenu.HandleShow(m_IsInAZone);

    }

    void OnUpdate(float timeslice)
    {
        if(m_UpdateCooldown >= 5.0)
        {
            CheckForPossibleZones();
            m_UpdateCooldown = 0;
        }
        else
        {
            m_UpdateCooldown += timeslice;
        }
    }  

    static autoptr KR_KingOfTheHillClientManager g_Instance;
    static autoptr KR_KingOfTheHillClientManager GetInstance()
    {
        if(GetGame().IsClient())
        {
            if(!g_Instance)
                g_Instance = new KR_KingOfTheHillClientManager();
        }
        return g_Instance;
    }
};