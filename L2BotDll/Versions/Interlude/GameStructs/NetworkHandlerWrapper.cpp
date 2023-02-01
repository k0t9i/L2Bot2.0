#include "pch.h"
#include "../../../Common/apihook.h"
#include "NetworkHandlerWrapper.h"
#include "../../../Events/SpoiledEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "ProcessManipulation.h"

namespace Interlude
{
	void* NetworkHandlerWrapper::originalInitAddress = 0;
	NetworkHandlerWrapper::NetworkHandler* NetworkHandlerWrapper::_target = 0;

	void(__thiscall* NetworkHandlerWrapper::__Init)(NetworkHandler*, float) = 0;
	Item* (__thiscall* NetworkHandlerWrapper::__GetNextItem)(NetworkHandler*, float, int) = 0;
	User* (__thiscall* NetworkHandlerWrapper::__GetNextCreature)(NetworkHandler*, float, int) = 0;
	int(__thiscall* NetworkHandlerWrapper::__AddNetworkQueue)(NetworkHandler*, L2::NetworkPacket*) = 0;

	//todo exception
	Item* NetworkHandlerWrapper::GetNextItem(float_t radius, int prevId) const
	{
		if (__GetNextItem && _target) {
			return (*__GetNextItem)(_target, radius, prevId);
		}
		return 0;
	}

	//todo exception
	User* NetworkHandlerWrapper::GetNextCreature(float_t radius, int prevId) const
	{
		if (__GetNextCreature && _target) {
			return (*__GetNextCreature)(_target, radius, prevId);
		}
		return 0;
	}

	User* NetworkHandlerWrapper::GetHero() const
	{
		const auto creatures = FindAllObjects<User*>(0.1f, [this](float_t radius, int32_t prevId) {
			return GetNextCreature(radius, prevId);
		});

		for (const auto& kvp : creatures)
		{
			const auto& creature = static_cast<User*>(kvp.second);
			if (creature->userType == L2::UserType::USER && creature->lvl > 0)
			{
				return creature;
			}
		}
		return 0;
	}

	void NetworkHandlerWrapper::Init(HMODULE hModule)
	{
		void* initAddress = GetProcAddress(hModule, "?Tick@UNetworkHandler@@UAEXM@Z");
		originalInitAddress = splice(initAddress, __Init_hook);
		(FARPROC&)__Init = (FARPROC)initAddress;

		(FARPROC&)__GetNextItem = GetProcAddress(hModule, "?GetNextItem@UNetworkHandler@@UAEPAUItem@@MH@Z");
		(FARPROC&)__GetNextCreature = GetProcAddress(hModule, "?GetNextCreature@UNetworkHandler@@UAEPAUUser@@MH@Z");

		
		(FARPROC&)__AddNetworkQueue = (FARPROC)splice(
			GetProcAddress(hModule, "?AddNetworkQueue@UNetworkHandler@@UAEHPAUNetworkPacket@@@Z"), __AddNetworkQueue_hook
		);
	}

	void NetworkHandlerWrapper::Restore()
	{
		restore(originalInitAddress);
		restore((void*&)__AddNetworkQueue);
	}

	void __fastcall NetworkHandlerWrapper::__Init_hook(NetworkHandler* This, int /*edx*/, float unk)
	{
		if (_target == 0) {
			_target = This;

			InjectLibrary::StopCurrentProcess();
			restore(originalInitAddress);
			InjectLibrary::StartCurrentProcess();

			(*__Init)(This, unk);
		}
	}

	int __fastcall NetworkHandlerWrapper::__AddNetworkQueue_hook(NetworkHandler* This, int, L2::NetworkPacket* packet)
	{
		if (packet->id == static_cast<int>(L2::NetworkPacketId::SYSTEM_MESSAGE)) {
			L2::SystemMessagePacket* p = static_cast<L2::SystemMessagePacket*>(packet);
			if (
				p->GetMessageId() == static_cast<int>(L2::SystemMessagePacket::Type::SPOIL_SUCCESS) ||
				p->GetMessageId() == static_cast<int>(L2::SystemMessagePacket::Type::ALREADY_SPOILED)
				) {
				EventDispatcher::GetInstance().Dispatch(SpoiledEvent{});
			}
		}

		return (*__AddNetworkQueue)(This, packet);
	}
}
