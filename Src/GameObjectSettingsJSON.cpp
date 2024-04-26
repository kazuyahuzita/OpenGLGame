#include "GameObjectSettingsJSON.h"
//他のオブジェクトヘッダーから
#include <algorithm>
#include "FireEmitter.h"
#include "FrontBackBoard.h"
#include "Item.h"
#include "MoveBlock.h"
#include "MoveExplorationScene.h"
#include "MoveStage3Scene.h"
#include "RotateBar.h"
#include "TextBlock.h"
#include "Tornado.h"
//Engine経由の機能に関するヘッダーから
#include "Engine/Engine.h"

void GameObjectSettingsJSON::ExplorationSceneMapLoad()
{

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// ゲームオブジェクト配置ファイルを読み込む
	Engine::FilepathMap filepathMap;
	filepathMap["WeedGround"] = "Res/MeshData/Exploration/Ground/Ground.obj";
	filepathMap["Pine_tree"] = "Res/MeshData/a_piece_of_nature/Pine_tree.obj";
	filepathMap["Rock_3"] = "Res/MeshData/a_piece_of_nature/Rock_3.obj";

	//ステージ生成
	engine->LoadGameObjectMap("Res/Json/GameObjectMap.json", filepathMap);

}

void GameObjectSettingsJSON::AthleticSceneMapLoad()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// ゲームオブジェクト配置ファイルを読み込む
	Engine::FilepathMap filepathMap;
	filepathMap["Cube"] = "Res/MeshData/Athletic/Cube/Cube.obj";
	filepathMap["FloatingGround"] = "Res/MeshData/Athletic/FloatingGround/FloatingGround.obj";
	filepathMap["Ground"] = "Res/MeshData/Athletic/Ground/Ground.obj";
	filepathMap["HorizontalMoveBlock"] = "Res/MeshData/Athletic/Gems/Gem.obj";
	filepathMap["JumpPlatForm"] = "Res/MeshData/Athletic/JumpingPlatforms/JumpingPlatforms.obj";
	filepathMap["Gem"] = "Res/MeshData/Athletic/Gems/Gem.obj";
	filepathMap["SlidingFloor"] = "Res/MeshData/Athletic/SlidingBlock/SlidingBlock.obj";
	filepathMap["Wall"] = "Res/MeshData/Athletic/Wall/Wall.obj";
	filepathMap["WeedGround"] = "Res/MeshData/StageBlock/WeedGround/WeedGround.obj";

	//ステージ生成
	std::vector<GameObjectPtr> stageObject = engine->LoadGameObjectMap("Res/Json/Athletic.json", filepathMap);

	for (int i = 0; i < stageObject.size(); i++)
	{
		if (stageObject[i]->name == "fire")
		{
			stageObject[i]->AddComponent<FireEmitter>();
		}
		else if (stageObject[i]->name == "FrontBackBoard")
		{
			stageObject[i]->AddComponent<FrontBackBoard>();
		}
		else if (stageObject[i]->name == "VerticalMoveBlock")
		{
			stageObject[i]->AddComponent<VerticalMoveBlock>();
		}
		else if (stageObject[i]->name == "Item")
		{
			stageObject[i]->AddComponent<GemItem>();
		}
	}
}

void GameObjectSettingsJSON::AthleticStage1MapLoad()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// ゲームオブジェクト配置ファイルを読み込む
	Engine::FilepathMap filepathMap;
	filepathMap["WeedGround"] = "Res/MeshData/StageBlock/WeedGround/WeedGround.obj";
	filepathMap["Cube"] = "Res/MeshData/StageBlock/Obstacles/Cube/Cube.obj";
	filepathMap["Cylinder"] = "Res/MeshData/StageBlock/Obstacles/Cylinder/Cylinder.obj";
	filepathMap["Wall"] = "Res/MeshData/Athletic/Wall/Wall.obj";
	filepathMap["Text_BackScene"] = "Res/MeshData/Athletic/Text_BackScene/BackTextScene.obj";
	filepathMap["Text_NextScene"] = "Res/MeshData/Athletic/Text_NextScene/NextTextScene.obj";
	
	//ステージ生成
	engine->LoadGameObjectMap("Res/Json/Stage1.json", filepathMap);

}

void GameObjectSettingsJSON::AthleticStage2MapLoad()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	Engine::FilepathMap filepathMap;
	filepathMap["Cylinder_Bar"] ="Res/MeshData/Athletic/RotateBar/Wide/RotateBar_Wide.obj";
	filepathMap["Cylinder_Center"] = "Res/MeshData/Athletic/RotateBar/Center/RotateBar_Center.obj";
	filepathMap["HoraizontalMoveBlock"] = "Res/MeshData/Athletic/Wall/Wall.obj";
	filepathMap["Wall"] = "Res/MeshData/StageBlock/Obstacles/Obstacles.obj";
	filepathMap["WeedGround"] = "Res/MeshData/StageBlock/WeedGround/WeedGround.obj";
	filepathMap["VerticalMoveBlock"] = "Res/MeshData/Athletic/Wall/Wall.obj";
	filepathMap["TextBlock"] = "Res/MeshData/Athletic/Wall/Wall.obj";
	filepathMap["Text_BackScene"] = "Res/MeshData/Athletic/Text_BackScene/BackTextScene.obj";
	filepathMap["Text_NextScene"] = "Res/MeshData/Athletic/Text_NextScene/NextTextScene.obj";

	//配列の戻り値を取らないといけないので、同じ型を使って格納
	std::vector<GameObjectPtr> stageObject = engine->LoadGameObjectMap("Res/Json/Stage2.json", filepathMap);

	for (int i = 0; i < stageObject.size(); i++)
	{
		if (stageObject[i]->name == "VerticalMoveBlock")
		{
			stageObject[i]->AddComponent<VerticalMoveBlock>();
		}
		else if (stageObject[i]->name == "XaxisMoveBlock")
		{
			stageObject[i]->AddComponent<XaxisMoveBlock>();

		}
		else if (stageObject[i]->name == "ZaxisMoveBlock")
		{
			stageObject[i]->AddComponent<ZaxisMoveBlock>();

		}
		else if (stageObject[i]->name == "Cylinder_Bar")
		{
			stageObject[i]->AddComponent<RotateBar>();

		}
	}

}

void GameObjectSettingsJSON::AthleticStage3MapLoad()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	// ゲームオブジェクト配置ファイルを読み込む
	Engine::FilepathMap filepathMap;
	filepathMap["Cylinder_Bar"] = "Res/MeshData/Athletic/RotateBar/Wide/RotateBar_Wide.obj";
	filepathMap["Cylinder_Center"] = "Res/MeshData/Athletic/RotateBar/Center/RotateBar_Center.obj";
	filepathMap["HorizontalMoveBlock"] = "Res/MeshData/Athletic/Wall/Wall.obj";
	filepathMap["WeedGround"] = "Res/MeshData/StageBlock/WeedGround/WeedGround.obj";
	filepathMap["Wall"] = "Res/MeshData/Athletic/Wall/Wall.obj";
	filepathMap["VerticalMoveBlock"] = "Res/MeshData/Athletic/Wall/Wall.obj";
	filepathMap["Text_BackScene"] = "Res/MeshData/Athletic/Text_BackScene/BackTextScene.obj";


	//配列の戻り値を取らないといけないので、同じ型を使って格納
	std::vector<GameObjectPtr> stageObject = engine->LoadGameObjectMap("Res/Json/Stage3.json", filepathMap);

	for (int i = 0; i < stageObject.size(); i++)
	{
		if (stageObject[i]->name == "VerticalMoveBlock")
		{
			stageObject[i]->AddComponent<VerticalMoveBlock>();
		}
		else if (stageObject[i]->name == "XaxisMoveBlock")
		{
			stageObject[i]->AddComponent<XaxisMoveBlock>();

		}
		else if (stageObject[i]->name == "ZaxisMoveBlock")
		{
			stageObject[i]->AddComponent<ZaxisMoveBlock>();

		}
		else if (stageObject[i]->name == "Cylinder_Bar")
		{
			stageObject[i]->AddComponent<RotateBar>();

		}
	}

}

void GameObjectSettingsJSON::AsylumMapLoad()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// ゲームオブジェクト配置ファイルを読み込む
	Engine::FilepathMap filepathMap;

	filepathMap["1stFloorCeiling_Crevice"]
		= "Res/MeshData/Asylum/1stFloorCeiling_Crevice/1stFloorCeiling_Crevice.obj";
	filepathMap["Bed_Feather"] = "Res/MeshData/Asylum/Bed_Feather/Bed_Feather.obj";
	filepathMap["Bed_Frame"] = "Res/MeshData/Asylum/Bed_Frame/Bed_Frame.obj";
	filepathMap["Bed_Mattress"] = "Res/MeshData/Asylum/Bed_Mattress/Bed_Mattress.obj";
	filepathMap["Bed_Pillow"] = "Res/MeshData/Asylum/Bed_Pillow/Bed_Pillow.obj";
	filepathMap["BedRoom_DoorFrame"] = "Res/MeshData/Asylum/BedRoom_DoorFrame/BedRoom_DoorFrame.obj";
	filepathMap["BedRoom_DoorFrame_Cervice"] =
		"Res/MeshData/Asylum/BedRoom_DoorFrame_Cervice/BedRoom_DoorFrame_Cervice.obj";
	filepathMap["Bench"] = "Res/MeshData/Asylum/Bench/Bench.obj";
	filepathMap["BigWindow_Hall"] = "Res/MeshData/Asylum/BigWindow_Hall/BigWindow_Hall.obj";
	filepathMap["BigWindow_Suspension"] = "Res/MeshData/Asylum/BigWindow_Suspension/BigWindow_Suspension.obj";
	filepathMap["Ceiling"] = "Res/MeshData/Asylum/Ceiling/Ceiling.obj";
	filepathMap["Chair_Leather"] = "Res/MeshData/Asylum/Chair_Leather/Chair_Leather.obj";
	filepathMap["Chair_White"] = "Res/MeshData/Asylum/Chair_White/Chair_White.obj";
	filepathMap["Chandelier"] = "Res/MeshData/Asylum/Chandelier/Chandelier.obj";
	filepathMap["Cupboard"] = "Res/MeshData/Asylum/Cupboard/Cupboard.obj";
	filepathMap["Cupboard_Door"] = "Res/MeshData/Asylum/Cupboard_Door/Cupboard_Door.obj";
	filepathMap["CupboardLight"] = "Res/MeshData/Asylum/CupboardLight/CupboardLight.obj";
	filepathMap["CupboardLight_Broken"]
		= "Res/MeshData/Asylum/CupboardLight_Broken/CupboardLight_Broken.obj";
	filepathMap["CurtainsMedic"] = "Res/MeshData/Asylum/CurtainsMedic/CurtainsMedic.obj";
	filepathMap["Elevator_Door"] = "Res/MeshData/Asylum/Elevator_Door/Elevator_Door.obj";
	filepathMap["Elevators"] = "Res/MeshData/Asylum/Elevators/Elevator.obj";
	filepathMap["Elevators_Cervice"] = "Res/MeshData/Asylum/Elevators_Cervice/Elevators_Cervice.obj";
	filepathMap["Fence"] = "Res/MeshData/Asylum/Fence/Fence.obj";
	filepathMap["FilmProjector"] = "Res/MeshData/Asylum/FilmProjector/FilmProjector.obj";
	filepathMap["Ground"] = "Res/MeshData/Asylum/Ground/Ground.obj";
	filepathMap["Ground_Lunch"] = "Res/MeshData/Asylum/Ground_Lunch/Ground_Lunch.obj";
	filepathMap["Ground_Operating"] = "Res/MeshData/Asylum/Ground_Operating/Ground_Operating.obj";
	filepathMap["Ground_Tile11"] = "Res/MeshData/Asylum/Ground_Tile11/Ground_Tile11.obj";
	filepathMap["Ground_Wood"] = "Res/MeshData/Asylum/Ground_Wood/Ground_Wood.obj";
	filepathMap["Gym_DoorFrame"] = "Res/MeshData/Asylum/Gym_DoorFrame/Gym_DoorFrame.obj";
	filepathMap["Heator"] = "Res/MeshData/Asylum/Heator/Heator.obj";
	filepathMap["Lamp_Desk"] = "Res/MeshData/Asylum/Lamp_Desk/Lamp_Desk.obj";
	filepathMap["Lamp_Neon"] = "Res/MeshData/Asylum/Lamp_Neon/Lamp_Neon.obj";
	filepathMap["Light_Single"] = "Res/MeshData/Asylum/Light_Single/Light_Single.obj";
	filepathMap["LunchLoom_DoorFrame"] = "Res/MeshData/Asylum/LunchLoom_DoorFrame/LunchLoom_DoorFrame.obj";
	filepathMap["LunchLoom_EnterFrame"] = "Res/MeshData/Asylum/LunchLoom_EnterFrame/LunchLoom_EnterFrame.obj";
	filepathMap["LunchLoom_EnterGlass"] = "Res/MeshData/Asylum/LunchLoom_EnterGlass/LunchLoom_EnterGlass.obj";
	filepathMap["MainDoor"] = "Res/MeshData/Asylum/MainDoor/MainDoor.obj";
	filepathMap["MedRack"] = "Res/MeshData/Asylum/MedRack/MedRack.obj";
	filepathMap["Mug"] = "Res/MeshData/Asylum/Mug/Mug.obj";
	filepathMap["OperatingRoom_DoorFrame"]
		= "Res/MeshData/Asylum/OperatingRoom_DoorFrame/OperatingRoom_DoorFrame.obj";
	filepathMap["OperatingRoom_DoorFrameCervice"]
		= "Res/MeshData/Asylum/OperatingRoom_DoorFrameCervice/OperatingRoom_DoorFrameCervice.obj";
	filepathMap["Paper"] = "Res/MeshData/Asylum/Paper/Paper.obj";
	filepathMap["Pillar"] = "Res/MeshData/Asylum/Pillar/Pillar.obj";
	filepathMap["Pillar_Marble"] = "Res/MeshData/Asylum/Pillar_Marble/Pillar_Marble.obj";
	filepathMap["PosterHuman"] = "Res/MeshData/Asylum/PosterHuman/PosterHuman.obj";
	filepathMap["Pot"] = "Res/MeshData/Asylum/Pot/Pot.obj";
	filepathMap["ProjectorCloth"] = "Res/MeshData/Asylum/ProjectorCloth/ProjectorCloth.obj";
	filepathMap["RestRoom_DoorFrame"]
		= "Res/MeshData/Asylum/RestRoom_DoorFrame/RestRoom_DoorFrame.obj";
	filepathMap["RestRoom_DoorFrame_Cervice"]
		= "Res/MeshData/Asylum/RestRoom_DoorFrame_Cervice/RestRoom_DoorFrame_Cervice.obj";
	filepathMap["SheetRack"] = "Res/MeshData/Asylum/SheetRack/SheetRack.obj";
	filepathMap["SheetRackCase"] = "Res/MeshData/Asylum/SheetRackCase/SheetRackCase.obj";
	filepathMap["Sink"] = "Res/MeshData/Asylum/Sink/Sink.obj";
	filepathMap["SmallWindow_Hall"] = "Res/MeshData/Asylum/SmallWindow_Hall/SmallWindow_Hall.obj";
	filepathMap["SmallWindow_Suspension"]
		= "Res/MeshData/Asylum/SmallWindow_Suspension/SmallWindow_Suspension.obj";
	filepathMap["Sofa_LeatherBottom"] = "Res/MeshData/Asylum/Sofa_LeatherBottom/Sofa_LeatherBottom.obj";
	filepathMap["Sofa_LeatherPillows"] = "Res/MeshData/Asylum/Sofa_LeatherPillows/Sofa_LeatherPillows.obj";
	filepathMap["Sofa_LeatherSide"] = "Res/MeshData/Asylum/Sofa_LeatherSide/Sofa_LeatherSide.obj";
	filepathMap["Sofa_SingleMain"] = "Res/MeshData/Asylum/Sofa_SingleMain/Sofa_SingleMain.obj";
	filepathMap["Sofa_SinglePillow"] = "Res/MeshData/Asylum/Sofa_SinglePillow/Sofa_SinglePillow.obj";
	filepathMap["Sofa_Small"] = "Res/MeshData/Asylum/Sofa_Small/Sofa_Small.obj";
	filepathMap["Stair"] = "Res/MeshData/Asylum/Stair/Stair.obj";
	filepathMap["Statue"] = "Res/MeshData/Asylum/Statue/Statue.obj";
	filepathMap["SurgeryChairConstruction"]
		= "Res/MeshData/Asylum/SurgeryChairConstruction/SurgeryChairConstruction.obj";
	filepathMap["SurgeryChairPillow"] = "Res/MeshData/Asylum/SurgeryChairPillow/SurgeryChairPillow.obj";
	filepathMap["SurgeryLamp"] = "Res/MeshData/Asylum/SurgeryLamp/SurgeryLamp.obj";
	filepathMap["SurgeryTable"] = "Res/MeshData/Asylum/SurgeryTable/SurgeryTable.obj";
	filepathMap["SurgeryWheelBad"] = "Res/MeshData/Asylum/SurgeryWheelBad/SurgeryWheelBad.obj";
	filepathMap["Table_Bed"] = "Res/MeshData/Asylum/Table_Bed/Table_Bed.obj";
	filepathMap["Table_Office"] = "Res/MeshData/Asylum/Table_Office/Table_Office.obj";
	filepathMap["Table_School"] = "Res/MeshData/Asylum/Table_School/Table_School.obj";
	filepathMap["Table_Small"] = "Res/MeshData/Asylum/Table_Small/Table_Small.obj";
	filepathMap["Table_White"] = "Res/MeshData/Asylum/Table_White/Table_White.obj";
	filepathMap["Table_WhiteKitchen"] = "Res/MeshData/Asylum/Table_WhiteKitchen/Table_WhiteKitchen.obj";
	filepathMap["TV_Room"] = "Res/MeshData/Asylum/TV_Room/TV_Room.obj";
	filepathMap["Wall_Floor"] = "Res/MeshData/Asylum/Wall_Floor/Wall_Floor.obj";
	filepathMap["Wall_Room"] = "Res/MeshData/Asylum/Wall_Room/Wall_Room.obj";
	filepathMap["WheelChair"] = "Res/MeshData/Asylum/WheelChair/WheelChair.obj";

	//ステージ生成
	engine->LoadGameObjectMap("Res/Json/AsylumMap.json", filepathMap);

}


void GameObjectSettingsJSON::RiderSceneMapLoad()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();
	// ゲームオブジェクト配置ファイルを読み込む
	Engine::FilepathMap filepathMap;

	filepathMap["Branch1"] = "Res/MeshData/DeathForest/Branch/Branch.obj";
	filepathMap["Bridge"] = "Res/MeshData/DeathForest/Bridge/Bridge.obj";
	filepathMap["RailingA"] = "Res/MeshData/DeathForest/Fence/Fence.obj";
	filepathMap["Fence_out"] = "Res/MeshData/DeathForest/Fence_out/Fence_out.obj";
	filepathMap["Ground"] = "Res/MeshData/DeathForest/Ground/Ground.obj";
	filepathMap["Rock_3"] = "Res/MeshData/a_piece_of_nature/Rock_3.obj";
	filepathMap["Pine_tree"] = "Res/MeshData/a_piece_of_nature/Pine_tree.obj";
	filepathMap["Road"] = "Res/MeshData/DeathForest/Road/Road.obj";
	filepathMap["Roof_house"] = "Res/MeshData/DeathForest/Roof_house/Roof_house.obj";
	filepathMap["Roof_house_small"] = "Res/MeshData/DeathForest/Roof_house_small/Roof_house_small.obj";
	filepathMap["Stair"] = "Res/MeshData/DeathForest/Stair/Stair.obj";
	filepathMap["Tunnel"] = "Res/MeshData/DeathForest/Tunnel/Tunnel.obj";
	filepathMap["Wall"] = "Res/MeshData/DeathForest/Wall/Wall.obj";
	filepathMap["Wall_house"] = "Res/MeshData/DeathForest/Wall_house/Wall_house.obj";

	//ステージ生成
	engine->LoadGameObjectMap("Res/Json/DeathForestMap.json", filepathMap);

}

void GameObjectSettingsJSON::SubMainGameSceneMapLoad()
{
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	// ゲームオブジェクト配置ファイルを読み込む
	Engine::FilepathMap filepathMap;

	filepathMap["Branch1"] = "Res/MeshData/DeathForest/Branch/Branch.obj";
	filepathMap["Bridge"] = "Res/MeshData/DeathForest/Bridge/Bridge.obj";
	filepathMap["RailingA"] = "Res/MeshData/DeathForest/Fence/Fence.obj";
	filepathMap["Fence_out"] = "Res/MeshData/DeathForest/Fence_out/Fence_out.obj";
	filepathMap["Ground"] = "Res/MeshData/DeathForest/Ground/Ground.obj";
	filepathMap["Rock_3"] = "Res/MeshData/a_piece_of_nature/Rock_3.obj";
	filepathMap["Pine_tree"] = "Res/MeshData/a_piece_of_nature/Pine_tree.obj";
	filepathMap["Road"] = "Res/MeshData/DeathForest/Road/Road.obj";
	filepathMap["Roof_house"] = "Res/MeshData/DeathForest/Roof_house/Roof_house.obj";
	filepathMap["Roof_house_small"] = "Res/MeshData/DeathForest/Roof_house_small/Roof_house_small.obj";
	filepathMap["Stair"] = "Res/MeshData/DeathForest/Stair/Stair.obj";
	filepathMap["Tunnel"] = "Res/MeshData/DeathForest/Tunnel/Tunnel.obj";
	filepathMap["Wall"] = "Res/MeshData/DeathForest/Wall/Wall.obj";
	filepathMap["Wall_house"] = "Res/MeshData/DeathForest/Wall_house/Wall_house.obj";

	//ステージ生成
	engine->LoadGameObjectMap("Res/Json/DeathForestMap2.json", filepathMap);

}

