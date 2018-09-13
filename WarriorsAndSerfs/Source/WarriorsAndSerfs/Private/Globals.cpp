// Fill out your copyright notice in the Description page of Project Settings.

#include "Globals.h"

FItemAmount::FItemAmount()
{
}


UGlobals::UGlobals()
{
}

UGlobals::~UGlobals()
{
}


//returns the (player-side) name of the item
FString UGlobals::GetItemName(EItem item)
{
	switch (item)
	{
	case EItem::MissingNo:
		return "MISSING";
		break;
	case EItem::Debug:
		return "DEBUG";
		break;
	case EItem::Log:
		return "Log";
		break;
	case EItem::Lumber:
		return "Lumber";
		break;
	case EItem::Stone:
		return "Stone Block";
		break;
	case EItem::IronOre:
		return "Iron Ore";
		break;
	case EItem::Iron:
		return "Iron Bar";
		break;
	case EItem::GoldOre:
		return "Gold Ore";
		break;
	case EItem::GoldCoins:
		return "Gold Coins";
		break;
	case EItem::Coal:
		return "Coal";
		break;
	case EItem::Wheat:
		return "Grain";
		break;
	case EItem::Flour:
		return "Flour";
		break;
	case EItem::Bread:
		return "Bread";
		break;
	case EItem::Wine:
		return "Wine";
		break;
	case EItem::Fish:
		return "Fish";
		break;
	case EItem::Pig:
		return "Pig";
		break;
	case EItem::Sausages:
		return "Sausages";
		break;
	case EItem::Skin:
		return "Skin";
		break;
	case EItem::Leather:
		return "Leather";
		break;
	case EItem::WoodenShield:
		return "Wooden Shield";
		break;
	case EItem::IronShield:
		return "Iron Shield";
		break;
	case EItem::LeatherArmor:
		return "Leather Armor";
		break;
	case EItem::IronArmor:
		return "Iron Armor";
		break;
	case EItem::HandAxe:
		return "Hand Axe";
		break;
	case EItem::Sword:
		return "Sword";
		break;
	case EItem::Lance:
		return "Lance";
		break;
	case EItem::Halberd:
		return "Halberd";
		break;
	case EItem::Bow:
		return "Bow";
		break;
	case EItem::Crossbow:
		return "Crossbow";
		break;
	case EItem::Horse:
		return "Horse";
		break;
	case EItem::Recruit:
		return "Recruit";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}


//returns the (player-side) name of the Subject
FString UGlobals::GetSubjectType(ESubjectType subject)
{
	switch (subject)
	{
	case ESubjectType::MissingNo:
		return "MISSING";
		break;
	case ESubjectType::Debug:
		return "DEBUG";
		break;
	case ESubjectType::Serf:
		return "Serf";
		break;
	case ESubjectType::Builder:
		return "Builder";
		break;
	case ESubjectType::None:
		return "No one";
		break;
	case ESubjectType::Lumberjack:
		return "Lumberjack";
		break;
	case ESubjectType::Mason:
		return "Mason";
		break;
	case ESubjectType::Farmer:
		return "Farmer";
		break;
	case ESubjectType::Miner:
		return "Miner";
		break;
	case ESubjectType::Fisher:
		return "Fisher";
		break;
	case ESubjectType::Rancher:
		return "Rancher";
		break;
	case ESubjectType::Carpenter:
		return "Carpenter";
		break;
	case ESubjectType::Smelter:
		return "Smelter";
		break;
	case ESubjectType::Smith:
		return "Smith";
		break;
	case ESubjectType::Baker:
		return "Baker";
		break;
	case ESubjectType::Butcher:
		return "Butcher";
		break;
	case ESubjectType::Tanner:
		return "Tanner";
		break;
	case ESubjectType::Recruit:
		return "Recruit";
		break;
	case ESubjectType::Militia:
		return "Militia";
		break;
	case ESubjectType::Axeman:
		return "Axeman";
		break;
	case ESubjectType::Swordsman:
		return "Swordsman";
		break;
	case ESubjectType::Lancer:
		return "Lancer";
		break;
	case ESubjectType::Halbardier:
		return "Halbardier";
		break;
	case ESubjectType::Archer:
		return "Archer";
		break;
	case ESubjectType::Crossbowman:
		return "Crossbowman";
		break;
	case ESubjectType::Scout:
		return "Scout";
		break;
	case ESubjectType::Horseman:
		return "Horseman";
		break;
	case ESubjectType::Knight:
		return "Knight";
		break;
	case ESubjectType::Barbarian:
		return "Barbarian";
		break;
	case ESubjectType::Miller:
		return "Miller";
		break;
	case ESubjectType::Forester:
		return "Forester";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}

//returns the (player-side) name of the Lot
FString UGlobals::GetLotType(ELotType lot)
{
	switch (lot)
	{
	case ELotType::MissingNo:
		return "MISSING";
		break;
	case ELotType::Debug:
		return "DEBUG";
		break;
	case ELotType::Building:
		return "Building";
		break;
	case ELotType::Constructionsite:
		return "Constructionsite";
		break;
	case ELotType::DeadDrop:
		return "Loot Box";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}

//returns the (player-side) name of the Building
FString UGlobals::GetBuildingType(EBuildingType building)
{
	switch (building)
	{
	case EBuildingType::MissingNo:
		return "MISSING";
		break;
	case EBuildingType::Debug:
		return "DEBUG";
		break;
	case EBuildingType::School:
		return "School";
		break;
	case EBuildingType::Warehouse:
		return "Warehouse";
		break;
	case EBuildingType::Inn:
		return "Inn";
		break;
	case EBuildingType::Keep:
		return "Keep";
		break;
	case EBuildingType::DefenseTower:
		return "Defense Tower";
		break;
	case EBuildingType::Woodcutters:
		return "Woodcutters'";
		break;
	case EBuildingType::Quarry:
		return "Quarry";
		break;
	case EBuildingType::Farm:
		return "Farm";
		break;
	case EBuildingType::Vineyard:
		return "Vineyard";
		break;
	case EBuildingType::Coalmine:
		return "Coal Mine";
		break;
	case EBuildingType::Goldmine:
		return "Gold Mine";
		break;
	case EBuildingType::Ironmine:
		return "Iron Mine";
		break;
	case EBuildingType::FishingHut:
		return "Fishing Hut";
		break;
	case EBuildingType::Pigfarm:
		return "Swine Farm";
		break;
	case EBuildingType::Stable:
		return "Stable";
		break;
	case EBuildingType::Sawmill:
		return "Sawmill";
		break;
	case EBuildingType::IronSmeltery:
		return "Iron Smeltery";
		break;
	case EBuildingType::Mint:
		return "Mint";
		break;
	case EBuildingType::Mill:
		return "Mill";
		break;
	case EBuildingType::Bakery:
		return "Bakery";
		break;
	case EBuildingType::Butchery:
		return "Butchery";
		break;
	case EBuildingType::Tannery:
		return "Tannery";
		break;
	case EBuildingType::WeaponSmithy:
		return "Weapon Smithy";
		break;
	case EBuildingType::ArmorSmithy:
		return "Armor Smithy";
		break;
	case EBuildingType::WeaponWorkshop:
		return "Weapons Workshop";
		break;
	case EBuildingType::ArmorWorkshop:
		return "Armor Workshop";
		break;
	case EBuildingType::Road:
		return "Road";
		break;
	case EBuildingType::ForestingLodge:
		return "Foresting Lodge";
		break;
	case EBuildingType::Palace:
		return "Palace";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}

//returns the (player-side) name of the interactable
FString UGlobals::GetInteractableType(EInteractEnvType interact)
{
	switch (interact)
	{
	case EInteractEnvType::MissingNo:
		return "MISSING";
		break;
	case EInteractEnvType::Debug:
		return "DEBUG";
		break;
	case EInteractEnvType::Tree:
		return "Tree";
		break;
	case EInteractEnvType::Stone:
		return "Granite";
		break;
	case EInteractEnvType::Coalmine:
		return "Coal deposit";
		break;
	case EInteractEnvType::Ironmine:
		return "Iron Vein";
		break;
	case EInteractEnvType::Goldmine:
		return "Gold Vein";
		break;
	case EInteractEnvType::Fish:
		return "Fish";
		break;
	case EInteractEnvType::Game:
		return "Game";
		break;
	case EInteractEnvType::Wheat:
		return "Wheat";
		break;
	case EInteractEnvType::Grapes:
		return "Grapes";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}

//returns the (player-side) name of the job
FString UGlobals::GetHarvestingJobType(EHarvestingJobType job)
{
	switch (job)
	{
	case EHarvestingJobType::None:
		return "Idle";
		break;
	case EHarvestingJobType::Planting:
		return "Planting";
		break;
	case EHarvestingJobType::Returning:
		return "Returning";
		break;
	case EHarvestingJobType::Tending:
		return "Tending";
		break;
	case EHarvestingJobType::Harvesting:
		return "Harvesting";
		break;
	case EHarvestingJobType::ReturningWithHarvest:
		return "Returning with Harvest";
		break;
	case EHarvestingJobType::Polishing:
		return "Polishing";
		break;
	case EHarvestingJobType::Resting:
		return "Resting";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}

//returns the (player-side) name of the job
FString UGlobals::GetCraftingJobType(ECraftingJobType job)
{
	switch (job)
	{
	case ECraftingJobType::None:
		return "Idle";
		break;
	case ECraftingJobType::Crafting:
		return "Crafting";
		break;
	case ECraftingJobType::Resting:
		return "Resting";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}

