#include <cassert>
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "audiodevice.hpp"

namespace
{
const float Attenuation = 8.f;
}

std::vector<std::string>
AudioDevice::enumerateDevices()
{
	std::vector<std::string> devices;
	const char *stringList = nullptr;
	if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT"))
	{
		stringList = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
	}
	else if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT"))
	{
		stringList = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
	}

	while (stringList && stringList[0])
	{
		std::string name(stringList);
		stringList += name.size() + 1;
		devices.push_back(std::move(name));
	}
	return devices;
}

AudioDevice::AudioDevice(const std::string &name)
	: mAudioDevice(nullptr)
	, mAudioContext(nullptr)
	, mVolume(75.f)
	, mPosition(0.f)
	, mDirection(0.f, 0.f, -1.f)
	, mUpVector(0.f, 1.f, 0.f)
{
	if (!open(name))
	{
		throw std::runtime_error("Cannot open the device");
	}
}

AudioDevice::~AudioDevice()
{
	close();
}

bool
AudioDevice::isOpen() const
{
	return mAudioDevice && mAudioContext;
}

bool
AudioDevice::open(const std::string &name)
{
	const char *deviceName = nullptr;
	if (!name.empty())
	{
		deviceName = name.c_str();
	}

	if (mAudioDevice && alcIsExtensionPresent(mAudioDevice, "ALC_SOFT_reopen_device"))
	{
		auto alcReopenDeviceSOFT = reinterpret_cast<ALCboolean (ALC_APIENTRY*)(ALCdevice *, const ALCchar *, const ALCint *)>(alcGetProcAddress(mAudioDevice, "alcReopenDeviceSOFT"));
		if (!alcReopenDeviceSOFT(mAudioDevice, deviceName, NULL))
		{
			return false;
		}
	}
	else
	{
		if (mAudioDevice)
		{
			close();
		}

		auto audioDevice = alcOpenDevice(deviceName);
		if (!audioDevice)
		{
			return false;
		}
		auto audioContext = alcCreateContext(audioDevice, nullptr);
		if (!audioContext)
		{
			alcCloseDevice(mAudioDevice);
			return false;
		}
		mAudioDevice = audioDevice;
		mAudioContext = audioContext;
		alcMakeContextCurrent(mAudioContext);
	}

	// apply current values
	alCheck(alListenerf(AL_GAIN, mVolume * 0.01f));
	alCheck(alListenerfv(AL_POSITION, glm::value_ptr(mPosition)));
	applyOrientation();
	return true;
}

void
AudioDevice::close()
{
	alcMakeContextCurrent(nullptr);
	if (mAudioContext)
	{
		alcDestroyContext(mAudioContext);
		mAudioContext = nullptr;
	}
	if (mAudioDevice)
	{
		alcCloseDevice(mAudioDevice);
		mAudioDevice = nullptr;
	}
}

bool
AudioDevice::isSupported(const std::string &extension) const
{
	assert(mAudioDevice && "AudioDevice not open");
	if (extension.rfind("ALC", 0) == 0)
	{
		return alcIsExtensionPresent(mAudioDevice, extension.c_str()) != AL_FALSE;
	}

	return alIsExtensionPresent(extension.c_str()) != AL_FALSE;
}

float
AudioDevice::getGlobalVolume() const
{
	return mVolume;
}

void
AudioDevice::setGlobalVolume(float volume)
{
	assert(0.f <= volume && volume <= 100.f && "Volume out of bounds");
	mVolume = volume;
	if (isOpen())
	{
		alCheck(alListenerf(AL_GAIN, volume * 0.01f));
	}
}

glm::vec3
AudioDevice::getPosition() const
{
	return mPosition;
}

void
AudioDevice::setPosition(glm::vec3 position)
{
	mPosition = position;
	if (isOpen())
	{
		alCheck(alListenerfv(AL_POSITION, glm::value_ptr(position)));
	}
}

glm::vec3
AudioDevice::getDirection() const
{
	return mDirection;
}

void
AudioDevice::setDirection(glm::vec3 direction)
{
	mDirection = direction;
	if (isOpen())
	{
		applyOrientation();
	}
}

glm::vec3
AudioDevice::getUpVector() const
{
	return mUpVector;
}

void
AudioDevice::setUpVector(glm::vec3 upVector)
{
	mUpVector = upVector;
	if (isOpen())
	{
		applyOrientation();
	}
}

void
AudioDevice::applyOrientation()
{
	assert(isOpen() && "Device not open");
	float orientation[] = {
		mDirection.x,
		mDirection.y,
		mDirection.z,
		mUpVector.x,
		mUpVector.y,
		mUpVector.z,
	};
	alCheck(alListenerfv(AL_ORIENTATION, orientation));
}
