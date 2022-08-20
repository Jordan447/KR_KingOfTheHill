modded class MissionGameplay
{
  protected bool RecivedEvents;
  override void OnUpdate(float timeslice)
  {
        super.OnUpdate(timeslice);
        KR_KingOfTheHillClientManager.GetInstance().OnUpdate(timeslice);

        PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
        if(player && !RecivedEvents){
          GetGame().RPCSingleParam(player, KOTH_RPCs.RPC_GETALLEVENTS, null, true);
          RecivedEvents = true;
        }
    }

    override void OnMissionFinish()
	  {
      super.OnMissionFinish();
      RecivedEvents = false;
      KR_KingOfTheHillClientManager.GetInstance().m_ZoneMenu.HandleShow(false);
    }
};