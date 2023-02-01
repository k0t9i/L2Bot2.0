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
		static int __fastcall __AddNetworkQueue_hook(NetworkHandler* This, int /*edx*/, L2::NetworkPacket* packet);;

		static void(__thiscall* __Init)(NetworkHandler*, float);
		static Item* (__thiscall* __GetNextItem)(NetworkHandler*, float, int);
		static User* (__thiscall* __GetNextCreature)(NetworkHandler*, float, int);
		static int(__thiscall* __AddNetworkQueue)(NetworkHandler*, L2::NetworkPacket*);
	private:
		static void* originalInitAddress;
		static NetworkHandler* _target;
	};
}
