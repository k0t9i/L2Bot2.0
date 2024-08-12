#pragma once

#include <cstdint>
#include <map>
#include "../../GameStructs/NetworkHandlerInterface.h"
#include "GameStructs.h"
#include "../../GameStructs/FindObjectsTrait.h"
#include "L2ParamStack.h"

namespace Interlude
{
	class NetworkHandlerWrapper : public NetworkHandlerInterface, public FindObjectsTrait
	{
	public:
		class NetworkHandler {};
		NetworkHandlerWrapper() = default;
		virtual ~NetworkHandlerWrapper() = default;

		void Init(HMODULE hModule) override;
		void Restore() override;

		Item* GetNextItem(float_t radius, int prevId) const;
		User* GetNextCreature(float_t radius, int prevId) const;
		User* GetHero() const;
		User* GetUser(int objectId) const;
		Item* GetItem(int objectId) const;
		int RequestItemList() const;
		void MTL(APawn* self, L2::FVector dst, L2::FVector src, void* terrainInfo, int unk1) const;
		void Action(int objectId, L2::FVector objectLocation, int unk) const;
		void RequestMagicSkillUse(L2ParamStack& stack) const;
		int RequestUseItem(L2ParamStack& stack) const;
		void RequestAutoSoulShot(L2ParamStack& stack) const;
		void ChangeWaitType(int type) const;
		void RequestRestartPoint(L2ParamStack& stack) const;
	private:

		static void __fastcall __Init_hook(NetworkHandler* This, int /*edx*/, float unk);
		static int __fastcall __AddNetworkQueue_hook(NetworkHandler* This, int /*edx*/, L2::NetworkPacket* packet);

		static void(__thiscall* __Init)(NetworkHandler*, float);
		static Item* (__thiscall* __GetNextItem)(NetworkHandler*, float, int);
		static User* (__thiscall* __GetNextCreature)(NetworkHandler*, float, int);
		static int(__thiscall* __AddNetworkQueue)(NetworkHandler*, L2::NetworkPacket*);
		static int(__thiscall* __RequestItemList)(NetworkHandler*);
		static User* (__thiscall* __GetUser)(NetworkHandler*, int);
		static Item* (__thiscall* __GetItem)(NetworkHandler*, int);
		static void(__thiscall* __MTL)(NetworkHandler*, APawn*, L2::FVector, L2::FVector, void*, int);
		static void(__thiscall* __Action)(NetworkHandler*, int, L2::FVector, int);
		//stack skillId, isForce, isShiftPressed
		static void(__thiscall* __RequestMagicSkillUse)(NetworkHandler*, L2ParamStack&);
		static int(__thiscall* __RequestUseItem)(NetworkHandler*, L2ParamStack&);
		//stack itemId, on/off
		static void(__thiscall* __RequestAutoSoulShot)(NetworkHandler*, L2ParamStack&);
		//params objectId, unk
		static void(__thiscall* __ChangeWaitType)(NetworkHandler*, int);
		static void(__thiscall* __RequestRestartPoint)(NetworkHandler*, L2ParamStack&);

	private:
		static void* originalInitAddress;
		static NetworkHandler* _target;
	};
}
