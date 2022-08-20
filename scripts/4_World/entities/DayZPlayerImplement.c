modded class DayZPlayerImplement
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		PlayerBase player = PlayerBase.Cast(this);
		if (GetGame().IsDedicatedServer())
			KingOfTheHillEventManager.GetInstance().OnServerRPCRecived(player, sender, rpc_type, ctx);
		else
			KR_KingOfTheHillClientManager.GetInstance().OnClientRPCRecived(sender, rpc_type, ctx);
	}
};