#pragma once

#include "../../GameStructs/GameStructs.h"

namespace Interlude
{
	class User
	{
	public:
		char pad_0000[8]; //0x0000
		L2::UserType userType; //0x0008
		char pad_000C[4]; //0x000C
		int32_t isMob; //0x0010
		uint32_t npcId; //0x0014
		uint32_t objectId; //0x0018
		wchar_t nickname[24]; //0x001C
		L2::Race raceId; //0x004C
		L2::Gender gender; //0x0050
		int32_t classId; //0x0054
		uint32_t lvl; //0x0058
		int32_t exp; //0x005C
		char pad_0060[4]; //0x0060
		int32_t str; //0x0064
		int32_t dex; //0x0068
		int32_t con; //0x006C
		int32_t int_; //0x0070
		int32_t wit; //0x0074
		int32_t men; //0x0078
		int32_t maxHp; //0x007C
		int32_t hp; //0x0080
		int32_t maxMp; //0x0084
		int32_t mp; //0x0088
		int32_t maxWeight; //0x008C
		char pad_0090[8]; //0x0090
		class L2::UserWear wear; //0x0098
		char pad_010C[132]; //0x010C
		class L2::FColor titleColor; //0x0190
		int32_t pad_0194; //0x0194 pvp state: 0 - normal, 1 - pvp, 2 - blinking
		int32_t karma; //0x0198
		char pad_019C[104]; //0x019C
		class APawn* pawn; //0x0204
		char pad_0208[12]; //0x0208
		int32_t weight; //0x0214
		int32_t sp; //0x0218
		int32_t accuracy; //0x021C
		int32_t critRate; //0x0220
		int32_t pAttack; //0x0224
		int32_t attackSpeed; //0x0228
		int32_t pDefense; //0x022C
		int32_t evasion; //0x0230
		int32_t mAttack; //0x0234
		int32_t mDefense; //0x0238
		int32_t castingSpeed; //0x023C
		char pad_0240[20]; //0x0240
		wchar_t title[16]; //0x0254
		char pad_0274[32]; //0x0274
		int32_t pad_0294; //0x0294
		char pad_0298[16]; //0x0298
		int32_t hasDwarvenCraft; //0x02A8
		int32_t attackSpeed2; //0x02AC
		char pad_02B0[4]; //0x02B0
		int32_t pkKills; //0x02B4
		int32_t pvpKills; //0x02B8
		char pad_02BC[4]; //0x02BC
		int32_t activeClassId; //0x02C0
		int32_t maxCp; //0x02C4
		int32_t cp; //0x02C8
		char pad_02CC[20]; //0x02CC
		int16_t recRemaining; //0x02E0
		int16_t evalScore; //0x02E2
		int32_t invSlotCount; //0x02E4
		char pad_02E8[32]; //0x02E8
		class L2::FColor nicknameColor; //0x0308
		char pad_030C[164]; //0x030C
	}; //Size: 0x03B0

	class APawn
	{
	public:
		char pad_0000[8]; //0x0000
		void* uStaticMeshInstance; //0x0008
		void* fStateFrame; //0x000C
		char pad_0010[8]; //0x0010
		void* uPackage; //0x0018
		char pad_001C[32]; //0x001C
		class ALineagePlayerController* lineagePlayerController; //0x003C
		void* terrainInfo; //0x0040
		char pad_0044[28]; //0x0044
		int32_t ownerObjectId; //0x0060
		char pad_0064[344]; //0x0064
		class L2::FVector Location; //0x01BC
		class L2::FRotator Rotation; //0x01C8
		class L2::FVector Velocity; //0x01D4
		class L2::FVector Acceleration; //0x01E0
		char pad_01EC[336]; //0x01EC
		class L2::FVector Location2; //0x033C
		char pad_0348[1244]; //0x0348
	}; //Size: 0x0824

	class ALineagePlayerController
	{
	public:
		char pad_0000[444]; //0x0000
		class L2::FVector cameraPosition; //0x01BC
		class L2::FRotator cameraRotation; //0x01C8
		char pad_01D4[544]; //0x01D4
		class L2::FVector moveLocation; //0x03F4
		char pad_0400[16]; //0x0400
		int32_t isIdle; //0x0410
		uint32_t targetObjectId; //0x0414
		char pad_0418[28]; //0x0418
		int8_t isRunning; //0x0434
		int8_t isStanding; //0x0435
		char pad_0436[26]; //0x0436
	}; //Size: 0x0450

	struct Item
	{
		uint32_t objectId;
		unsigned int itemId;
		unsigned int isStackable; // ??
		unsigned int amount;
		APawn* pawn;
	};

	class ItemInfo
	{
	public:
		L2::ItemType2 type2; //0x0000
		char pad_0002[2]; //0x0002
		uint32_t objectId; //0x0004
		uint32_t itemId; //0x0008
		uint32_t amount; //0x000C
		char pad_0010[8]; //0x0010
		L2::ItemSlot itemSlot; //0x0018
		uint16_t customType1; //0x001C
		uint16_t isEquipped; //0x001E
		uint16_t enchantLevel; //0x0020
		char pad_0022[2]; //0x0022
		uint16_t customType2; //0x0024
		char pad_0026[10]; //0x0026
		uint32_t augmentation1; //0x0030
		uint32_t augmentation2; //0x0034
		int32_t mana; //0x0038
	}; //Size: 0x003C

	class FL2ItemDataBase
	{
	public:
		char pad_0000[4]; //0x0000
		L2::ItemDataType dataType; //0x0004
		char pad_0008[4]; //0x0008
		int32_t itemId; //0x000C
		char pad_0010[20]; //0x0010
		int32_t dropItemsNameIndex; //0x0024
		char pad_0028[8]; //0x0028
		int32_t dropItemsTexNameIndex; //0x0030
		char pad_0034[8]; //0x0034
		int32_t iconNameIndex; //0x003C
		char pad_0040[16]; //0x0040
		int32_t nameIndex; //0x0050
		char pad_0054[4]; //0x0054
		wchar_t* description; //0x0058
		char pad_005C[12]; //0x005C
		wchar_t* setItem; //0x0068
		char pad_006C[8]; //0x006C
		wchar_t* setEffect; //0x0074
		char pad_0078[8]; //0x0078
		wchar_t* addSetItem; //0x0080
		char pad_0084[8]; //0x0084
		wchar_t* addSetEffect; //0x008C
		char pad_0090[36]; //0x0090
		wchar_t* enchantEffect; //0x00B4
		char pad_00B8[12]; //0x00B8
		int32_t weight; //0x00C4
	}; //Size: 0x0140

	class FL2EtcItemData : public FL2ItemDataBase
	{
	};

	class FL2ArmorItemData : public FL2ItemDataBase
	{
	public:
		char pad_00C8[1308]; //0x00C8
		L2::ArmorType armorType; //0x05E4
		L2::CrystalType crystalType; //0x05E8
		char pad_05EC[4]; //0x05EC
		int32_t pDef; //0x05F0
		int32_t mDef; //0x05F4
		char pad_05F8[8]; //0x05F8
	};

	class FL2WeaponItemData : public FL2ItemDataBase
	{
	public:
		char pad_00C8[24]; //0x00C8
		int32_t wtfNameIndex1; //0x00E0
		char pad_00E4[16]; //0x00E4
		int32_t wtfNameIndex2; //0x00F4
		int32_t wtfNameIndex3; //0x00F8
		char pad_00FC[12]; //0x00FC
		int32_t wtfNameIndex4; //0x0108
		int32_t wtfNameIndex5; //0x010C
		int32_t wtfNameIndex6; //0x0110
		int32_t wtfNameIndex7; //0x0114
		int32_t wtfNameIndex8; //0x0118
		int32_t wtfNameIndex9; //0x011C
		char pad_0120[4]; //0x0120
		int32_t rndDamage; //0x0124
		int32_t pAtk; //0x0128
		int32_t mAtk; //0x012C
		L2::WeaponType weaponType; //0x0130
		L2::CrystalType crystalType; //0x0134
		int32_t critical; //0x0138
		int32_t hitModify; //0x013C
		int32_t shieldEvasion; //0x0140
		int32_t shieldPdef; //0x0144
		int32_t shieldDefRate; //0x0148
		int32_t atkSpd; //0x014C
		int32_t mpConsume; //0x0150
		int32_t soulshotCount; //0x0154
		int32_t spiritshotCount; //0x0158
		char pad_015C[16]; //0x015C
		int32_t wtfNameIndex10; //0x016C
		char pad_0170[220]; //0x0170
		int32_t wtfNameIndex11; //0x024C
		char pad_0250[48]; //0x0250
	}; //Size: 0x0280

	class FNameEntry
	{
	public:
		char pad_0000[12]; //0x0000
		wchar_t value[36]; //0x000C
	};

	class FL2MagicSkillData
	{
	public:
		wchar_t* name; //0x0000
		char pad_0004[8]; //0x0004
		wchar_t* description; //0x000C
		char pad_0010[8]; //0x0010
		int32_t skillId; //0x0018
		int32_t lvl; //0x001C
		char pad_0020[4]; //0x0020
		int32_t mpCost; //0x0024
		char pad_0028[4]; //0x0028
		int32_t range; //0x002C
		char pad_0030[4]; //0x0030
		float hitTime; //0x0034
		char pad_0038[12]; //0x0038
		int32_t wtfNameIndex; //0x0044
		int32_t iconNameIndex; //0x0048
		char pad_004C[52]; //0x004C
	}; //Size: 0x0080
};
