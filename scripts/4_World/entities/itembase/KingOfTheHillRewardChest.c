class KingOfTheHillRewardChest: DeployableContainer_Base
{
    override bool CanPutIntoHands(EntityAI parent)
	{
        return false;
    }

    override bool CanPutInCargo(EntityAI parent)
	{
        return false;
    }

    bool IsPrepareToDelete()
	{
		return IsEmpty();
	}
};