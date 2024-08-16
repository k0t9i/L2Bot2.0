#include "pch.h"
#include "../../../Common/apihook.h"
#include "NetworkHandlerWrapper.h"
#include "Domain/Events/SpoiledEvent.h"
#include "ProcessManipulation.h"
#include "Domain/Services/ServiceLocator.h"
#include "Domain/Exceptions.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	void* NetworkHandlerWrapper::originalInitAddress = 0;
	NetworkHandlerWrapper::NetworkHandler* NetworkHandlerWrapper::_target = 0;

	void(__thiscall* NetworkHandlerWrapper::__Init)(NetworkHandler*, float) = 0;
	Item* (__thiscall* NetworkHandlerWrapper::__GetNextItem)(NetworkHandler*, float, int) = 0;
	User* (__thiscall* NetworkHandlerWrapper::__GetNextCreature)(NetworkHandler*, float, int) = 0;
	int(__thiscall* NetworkHandlerWrapper::__AddNetworkQueue)(NetworkHandler*, L2::NetworkPacket*) = 0;
	int(__thiscall* NetworkHandlerWrapper::__RequestItemList)(NetworkHandler*) = 0;
	User* (__thiscall* NetworkHandlerWrapper::__GetUser)(NetworkHandler*, int) = 0;
	Item* (__thiscall* NetworkHandlerWrapper::__GetItem)(NetworkHandler*, int) = 0;
	void(__thiscall* NetworkHandlerWrapper::__Action)(NetworkHandler*, int, L2::FVector, int) = 0;
	void(__thiscall* NetworkHandlerWrapper::__MTL)(NetworkHandler*, APawn*, L2::FVector, L2::FVector, void*, int) = 0;
	void(__thiscall* NetworkHandlerWrapper::__RequestMagicSkillUse)(NetworkHandler*, L2ParamStack&) = 0;
	int(__thiscall* NetworkHandlerWrapper::__RequestUseItem)(NetworkHandler*, L2ParamStack&) = 0;
	void(__thiscall* NetworkHandlerWrapper::__RequestAutoSoulShot)(NetworkHandler*, L2ParamStack&) = 0;
	void(__thiscall* NetworkHandlerWrapper::__ChangeWaitType)(NetworkHandler*, int) = 0;
	void(__thiscall* NetworkHandlerWrapper::__RequestRestartPoint)(NetworkHandler*, L2ParamStack&) = 0;

	Item* NetworkHandlerWrapper::GetNextItem(float_t radius, int prevId) const
	{
		__try {
			if (__GetNextItem && _target) {
				return (*__GetNextItem)(_target, radius, prevId);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"UNetworkHandler::GetNextItem failed");
		}
	}

	User* NetworkHandlerWrapper::GetNextCreature(float_t radius, int prevId) const
	{
		__try {
			if (__GetNextCreature && _target) {
				return (*__GetNextCreature)(_target, radius, prevId);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::GetNextCreature failed");
		}
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

	User* NetworkHandlerWrapper::GetUser(int objectId) const
	{
		__try {
			if (__GetUser && _target) {
				return (*__GetUser)(_target, objectId);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::GetUser failed");
		}
	}

	Item* NetworkHandlerWrapper::GetItem(int objectId) const
	{
		__try {
			if (__GetItem && _target) {
				return (*__GetItem)(_target, objectId);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::GetItem failed");
		}
	}

	void NetworkHandlerWrapper::MTL(APawn* self, L2::FVector dst, L2::FVector src, void* terrainInfo, int unk1) const
	{
		__try {
			if (__MTL && _target) {
				(*__MTL)(_target, self, dst, src, terrainInfo, unk1);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::MTL failed");
		}
	}

	void NetworkHandlerWrapper::Action(int objectId, L2::FVector objectLocation, int unk) const
	{
		__try {
			if (__Action && _target) {
				(*__Action)(_target, objectId, objectLocation, unk);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::Action failed");
		}
	}

	void NetworkHandlerWrapper::RequestMagicSkillUse(L2ParamStack& stack) const
	{
		__try {
			if (__RequestMagicSkillUse && _target) {
				(*__RequestMagicSkillUse)(_target, stack);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::RequestMagicSkillUse failed");
		}
	}

	int NetworkHandlerWrapper::RequestUseItem(L2ParamStack& stack) const
	{
		__try {
			if (__RequestUseItem && _target) {
				return (*__RequestUseItem)(_target, stack);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::RequestUseItem failed");
		}
	}

	void NetworkHandlerWrapper::RequestAutoSoulShot(L2ParamStack& stack) const
	{
		__try {
			if (__RequestAutoSoulShot && _target) {
				(*__RequestAutoSoulShot)(_target, stack);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::RequestAutoSoulShot failed");
		}
	}

	void NetworkHandlerWrapper::ChangeWaitType(int type) const
	{
		__try {
			if (__ChangeWaitType && _target) {
				(*__ChangeWaitType)(_target, type);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::ChangeWaitType failed");
		}
	}

	int NetworkHandlerWrapper::RequestItemList() const
	{
		__try {
			if (__RequestItemList && _target) {
				return (*__RequestItemList)(_target);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::RequestItemList failed");
		}
	}

	void NetworkHandlerWrapper::RequestRestartPoint(L2ParamStack& stack) const
	{
		__try {
			if (__RequestRestartPoint && _target) {
				(*__RequestRestartPoint)(_target, stack);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw RuntimeException(L"UNetworkHandler::RequestRestartPoint failed");
		}
	}

	void NetworkHandlerWrapper::Init(HMODULE hModule)
	{
		void* initAddress = GetProcAddress(hModule, "?Tick@UNetworkHandler@@UAEXM@Z");
		originalInitAddress = splice(initAddress, __Init_hook);
		(FARPROC&)__Init = (FARPROC)initAddress;

		(FARPROC&)__GetNextItem = GetProcAddress(hModule, "?GetNextItem@UNetworkHandler@@UAEPAUItem@@MH@Z");
		(FARPROC&)__GetNextCreature = GetProcAddress(hModule, "?GetNextCreature@UNetworkHandler@@UAEPAUUser@@MH@Z");
		(FARPROC&)__RequestItemList = GetProcAddress(hModule, "?RequestItemList@UNetworkHandler@@UAEHXZ");
		(FARPROC&)__GetUser = GetProcAddress(hModule, "?GetUser@UNetworkHandler@@UAEPAUUser@@H@Z");
		(FARPROC&)__GetItem = GetProcAddress(hModule, "?GetItem@UNetworkHandler@@UAEPAUItem@@H@Z");
		(FARPROC&)__MTL = GetProcAddress(hModule, "?MTL@UNetworkHandler@@UAEXPAVAActor@@VFVector@@10H@Z");
		(FARPROC&)__Action = GetProcAddress(hModule, "?Action@UNetworkHandler@@UAEXHVFVector@@H@Z");
		(FARPROC&)__RequestMagicSkillUse = GetProcAddress(hModule, "?RequestMagicSkillUse@UNetworkHandler@@UAEXAAVL2ParamStack@@@Z");
		(FARPROC&)__RequestUseItem = GetProcAddress(hModule, "?RequestUseItem@UNetworkHandler@@UAEHAAVL2ParamStack@@@Z");
		(FARPROC&)__RequestAutoSoulShot = GetProcAddress(hModule, "?RequestAutoSoulShot@UNetworkHandler@@UAEXAAVL2ParamStack@@@Z");
		(FARPROC&)__ChangeWaitType = GetProcAddress(hModule, "?ChangeWaitType@UNetworkHandler@@UAEXH@Z");
		(FARPROC&)__RequestRestartPoint = GetProcAddress(hModule, "?RequestRestartPoint@UNetworkHandler@@UAEXAAVL2ParamStack@@@Z");
		
		(FARPROC&)__AddNetworkQueue = (FARPROC)splice(
			GetProcAddress(hModule, "?AddNetworkQueue@UNetworkHandler@@UAEHPAUNetworkPacket@@@Z"), __AddNetworkQueue_hook
		);
		Services::ServiceLocator::GetInstance().GetLogger()->Info(L"UNetworkHandler hooks initialized");
	}

	void NetworkHandlerWrapper::Restore()
	{
		restore(originalInitAddress);
		restore((void*&)__AddNetworkQueue);
		Services::ServiceLocator::GetInstance().GetLogger()->Info(L"UNetworkHandler hooks restored");
	}

	void __fastcall NetworkHandlerWrapper::__Init_hook(NetworkHandler* This, int /*edx*/, float unk)
	{
		if (_target == 0) {
			_target = This;
			Services::ServiceLocator::GetInstance().GetLogger()->Info(L"UNetworkHandler pointer {:#010x} obtained", (int)_target);

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
				Services::ServiceLocator::GetInstance().GetEventDispatcher()->Dispatch(Events::SpoiledEvent{});
			}
		}

		return (*__AddNetworkQueue)(This, packet);
	}
}
