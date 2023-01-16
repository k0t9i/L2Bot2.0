#pragma once

#include <vector>

namespace Interlude
{
	class L2ParamStack
	{
		char padding[16] = "";
	public:
		L2ParamStack(int size);
		// Деструктор не должен быть виртуальным
		~L2ParamStack();
		int PushBack(void* val);
		void* Top();
		void** GetBuffer();
		int GetBufferSize();
		int GetTotalBufferSize();
		
		template <typename T>
		std::vector<T> GetBufferAsVector()
		{
			std::vector<T> result;

			auto buffer = GetBuffer();
			for (int i = 0; i < GetBufferSize(); i++) {
				result.push_back((T) buffer[i]);
			}

			return result;
		}

		template <typename T>
		std::vector<T> GetBufferAsVector(uint16_t objectSize)
		{
			std::vector<T> result;

			uint32_t* tmpBuffer = (uint32_t*)GetBuffer();
			uint32_t objectsCount = tmpBuffer[0];

			auto buffer = GetBuffer();

			for (size_t i = 0; i < objectsCount; i++) {
				for (size_t j = 0; j < objectSize; j++) {
					result.push_back((T)buffer[i * objectSize + j + 1]);
				}
			}

			return result;
		}
	private:
		void Init();
	private:
		static void(__thiscall* __Ctor)(L2ParamStack* This, int);
		static void(__thiscall* __Dtor)(L2ParamStack* This);
		static int(__thiscall* __PushBack)(L2ParamStack* This, void*);
		static void*(__thiscall* __Top)(L2ParamStack* This);
		static void**(__thiscall* __GetBuffer)(L2ParamStack* This);
		static int(__thiscall* __GetBufferSize)(L2ParamStack* This);
		static int(__thiscall* __GetTotalBufferSize)(L2ParamStack* This);
		static HMODULE _hModule;
	};
}