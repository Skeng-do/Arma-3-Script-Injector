#pragma once

#define World              0x255E9E0
#define LocalPlayer        0x26D0
#define NetworkManager     0x25154F0
#define PboManager         0x2599418
#define NetworkClient      0x48
#define BEScriptCallback   0x590
#define Deref              0x0
#define NearTable          0x16B0
#define FarTable           0x1778
#define FarFarTable        0x1840
#define FarFarFarTable     0x1908
#define CameraPointer      0x26D0
#define CameraOn           0x7D8
#define VIEWRIGHT          0x8
#define VIEWUP             0x14 
#define VIEWFORWARD        0x20
#define VIEWTRANSLATION    0x2C
#define VIEWPORTMAT        0x58
#define PROJD1             0xD0
#define PROJD2             0xDC
#define WorldSize          0x2EB4
#define EntityPtr          0x8
#define EntityLink         0x10
#define ObjectEntity       0x8
#define ObjectEntityType   0x150
#define ObjectDisplayName  0x11A0
#define VisualState        0x190
#define ScoreBoard         0x48
#define PlayerSize         0x290
#define PlayerName         0x120
#define PlayerId           0x8
#define EntityPlayerId     0x340
#define MuzzleTablePointer 0xCD8
#define MuzzleTable        0x68
#define Weapon             0x8
#define WeaponType         0x18
#define WeaponName         0x30
#define IsDead             0x504
#define HeadPos            0x168 
#define EntityType         0x150
#define EntityName         0x68     
#define XYZcoord           0x2C
#define Recoil             0x1058

namespace ArmaScripts
{
	uint64_t ScriptVm = 0x1400;
	uint64_t ScriptThreads = 0x1408;
	uint64_t ScriptContents = 0x4A8;
};

namespace EventHandler
{
	uint64_t Table = 0x608;
	uint64_t TableSize = 0x610;
	uint64_t TableMaxSize = 0x618;
	uint64_t Size = 0x28;
	uint64_t Type = 0x8;
	uint64_t Container = 0x10;
	uint64_t ContainerSize = 0x18;
	uint64_t ContainerMaxSize = 0x20;
	uint64_t Script = 0x10;
};

namespace Player
{
	uint64_t nightvision = 0x1038;
	uint64_t thermal = 0x1041;
	uint64_t recoil = 0x1058;
	uint64_t stamina = 0x1094;
	uint64_t breath = 0x10A4;
	uint64_t fatigue = 0x109C;
};