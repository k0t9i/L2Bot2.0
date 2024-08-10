#include "pch.h"
#include "VersionAbstractFactory.h"
#include "Interlude/AbstractFactory.h"

const VersionAbstractFactory& VersionAbstractFactory::GetFactory(const Version version, const uint16_t creatureRadius, const uint16_t dropRadius)
{
	switch (version)
	{
	case Version::interlude:
		static Interlude::AbstractFactory interlude = Interlude::AbstractFactory(creatureRadius, dropRadius);
		return interlude;
	}
	
	//todo throw exception
}