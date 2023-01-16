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
	private:

		static void __fastcall __Init_hook(NetworkHandler* This, int /*edx*/, float unk);
		static int __fastcall __AddNetworkQueue_hook(NetworkHandler* This, int /*edx*/, L2::NetworkPacket* packet);
		static int __fastcall __OnDie_hook(NetworkHandler* This, int /*edx*/, User* creature, L2ParamStack& stack);
		static float __fastcall __GetMaxTickRate_hook(NetworkHandler* This, int /*edx*/);

		static void(__thiscall* __Init)(NetworkHandler*, float);
		static Item* (__thiscall* __GetNextItem)(NetworkHandler*, float, int);
		static User* (__thiscall* __GetNextCreature)(NetworkHandler*, float, int);
		static float(__thiscall* __GetMaxTickRate)(NetworkHandler*);
		static int(__thiscall* __AddNetworkQueue)(NetworkHandler*, L2::NetworkPacket*);
		static int(__thiscall* __OnDie)(NetworkHandler*, User*, L2ParamStack&);
	private:
		static void* originalInitAddress;
		static NetworkHandler* _target;
	};
}
