#pragma once

#include "pch.h"
#include "GameStructs.h"
#include "../../GameStructs/FNameInterface.h"

namespace Interlude
{
	class FName : public FNameInterface
	{
	public:
		FName() = default;
		virtual ~FName() = default;

		FNameEntry* GetEntry(int index) const;
		void Init(HMODULE hModule) override;
	private:
		static FNameEntry* (__cdecl* __GetEntry)(int);
	};

}
