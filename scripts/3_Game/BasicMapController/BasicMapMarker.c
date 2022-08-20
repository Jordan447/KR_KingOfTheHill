#ifdef BASICMAP
class KOTHMapMarker extends BasicMapCircleMarker 
{
	
	float EventRadius = 50;
	
	void KOTHMapMarker(string name, vector pos, string icon = "", array<int> colour = NULL, int alpha = 235, bool onHUD = false) {
		Name = name;
		if (icon != ""){
			Icon = icon;
		}
		Pos = pos;
		if (colour != NULL){
			Colour = colour;
		}
		Alpha = alpha;
		Is3DMarker = onHUD;
		HideIntersects = false;
		ShowCenterMarker = true;
	}
	
	override vector GetPosition(){
		if (Pos){	
			vector HeightAdjustedPos = Pos;
			HeightAdjustedPos[1] = Pos[1]+1;
			return HeightAdjustedPos;
		}
		return vector.Zero;
	}
	
	override int GetColour(){
		return ARGB(240, 206, 9, 9);
	}
	
	override string GetIcon(){
		return "KR_KingOfTheHill\\GUI\\images\\Flag.paa";
	}
	
	override bool Editable(){
		return false;
	}
	
	override string GetGroup(){
		return BasicMap().SERVER_KEY;
	}
}
#endif