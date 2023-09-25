#include "soundfile.hpp"
#include "soundfilewav.hpp"
#include "soundfileogg.hpp"

namespace
{
void ensureRegistration()
{
	static bool registered = false;
	if (!registered)
	{
		SoundFile::registerFile<SoundFileWav>();
		SoundFile::registerFile<SoundFileOgg>();
		registered = true;
	}
}
}

std::vector<std::function<SoundFile::Ptr()>>
SoundFile::mFactories;

SoundFile::Ptr
SoundFile::createFromFilename(const std::string &filename, Info &info)
{
	ensureRegistration();
	for (const auto &factory: mFactories)
	{
		auto ptr = factory();
		if (ptr->open(filename, info))
		{
			return ptr;
		}
	}
	return Ptr();
}

