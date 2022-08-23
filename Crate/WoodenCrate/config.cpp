class CfgPatches
{
	class KOTH_WoodenCrate_Base
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Gear_Containers","DZ_Data","Rifles","Attachments"};
	};
};


class CfgVehicles
{
	class Container_Base;
	class KOTH_WoodenCrate_Base: Container_Base
	{
		scope = 2;
		descriptionShort = "A small safe to store your valuables.";
		model = "\CPBStorage\Safes\Small\CPBS_Safe_Small.p3d";
		itemSize[] = {8,8};
		itemBehaviour = 2;
		itemPlacingOri = "90 0 0";
		class Cargo
		{
			itemsCargoSize[] = {10,10};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		class AnimationSources
		{
			class doors1
			{
				source = "doors1";
				initPhase = 0;
				animPeriod = 0.01;
			};
		};
		// attachments[] = {"Att_CombinationLock"};
		// class GUIInventoryAttachmentsProps
		// {
		// 	class CodeLock
		// 	{
		// 		name = "CodeLock";
		// 		description = "";
		// 		attachmentSlots[] = {"Att_CombinationLock"};
		// 		icon = "set:dayz_inventory image:cat_bb_attachments";
		// 		view_index = 3;
		// 	};
		// };
	};
	class KOTH_WoodenCrate: KOTH_WoodenCrate_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"CPBStorage\Safes\Small\Data\dayz_co.paa"};

	};
};