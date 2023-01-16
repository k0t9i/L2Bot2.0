#include "pch.h"
#include "VersionAbstractFactory.h"
#include "Interlude/AbstractFactory.h"

const VersionAbstractFactory& VersionAbstractFactory::GetFactory(const Version version, const uint16_t radius)
{
	switch (version)
	{
	case Version::interlude:
		static Interlude::AbstractFactory interlude = Interlude::AbstractFactory(radius);
		return interlude;
	}
	
	//todo throw exception
}