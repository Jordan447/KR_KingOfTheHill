class KR_KingOfTheHillSmoke extends M18SmokeGrenade_Green
{
    void KR_KingOfTheHillSmoke()
    {
        SetSmokeGrenadeState(ESmokeGrenadeState.LOOP);
    }

    void ~KR_KingOfTheHillSmoke()
    {
        SetSmokeGrenadeState(ESmokeGrenadeState.NO_SMOKE);
    }

    override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

    override bool DisableVicinityIcon()
    {
        return true;
    }
};