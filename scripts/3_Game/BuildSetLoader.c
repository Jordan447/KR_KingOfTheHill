//Thanks to the Build Loader Team!
class KR_KingOfTheHillBuildingSetsLoader
{
	autoptr array<Object>                           m_Builds;
	const string                                    m_BuildingFiles_Dir = "$profile:KingOfTheHill/Objects/";

    void KR_KingOfTheHillBuildingSetsLoader()
    {
        m_Builds = new array<Object>;
    }


    void ReadFileAndSpawnBuildings(string filename)
    {
        FileHandle handle = OpenFile(m_BuildingFiles_Dir + filename, FileMode.READ);
        if(handle != 0)
        {
            local string line;
            local int lineSize = FGets(handle, line); 

            while(lineSize >= 1)
            {
                line.TrimInPlace();
                if (line.Contains("SpawnObject") && !line.Contains("void") && !line.Contains("static") && line.Substring(0, 2) != "//")
				{
                    array<string> token = {};
                    line.Split("\"", token);

                    KR_KOTH_Building obj = new KR_KOTH_Building();
                    obj.Name = token[1];
                    obj.Position = token[3].ToVector();
                    obj.Orientation = token[5].ToVector();
                    SpawnKothBuildingObject(obj);
                }

                lineSize = FGets(handle, line);
            }

            CloseFile(handle);
        }
        else
        {
            Error("[KOTH] -> Cant find file in Objects folder!");
        }
    }

    void DeleteAllBuildings()
    {
        for(int i = 0; i < m_Builds.Count(); i++)
        {
            GetGame().ObjectDelete(m_Builds.Get(i));
        }
    }

    private void SpawnKothBuildingObject(KR_KOTH_Building building)
	{
		if (GetGame().ConfigGetInt("CfgVehicles " + building.Name + " scope") < 1)
			return;

		Object obj = GetGame().CreateObjectEx(building.Name, building.Position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);

		if (!obj)
			return;
        
        m_Builds.Insert(obj);


		obj.SetPosition(building.Position);
		obj.SetOrientation(building.Orientation);
		obj.SetOrientation(obj.GetOrientation());

		obj.SetFlags(EntityFlags.STATIC, false);
		obj.Update();
		obj.SetAffectPathgraph(true, false);
		if (obj.CanAffectPathgraph())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj); // refresh navmesh
	}
}

class KR_KOTH_Building
{
    string Name;
    vector Position;
    vector Orientation;
};