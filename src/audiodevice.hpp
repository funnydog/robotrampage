#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "alcheck.hpp"

class AudioDevice
{
public:
	static std::vector<std::string> enumerateDevices();

public:
	explicit AudioDevice(const std::string &name = "");
	~AudioDevice();

	bool isOpen() const;
	bool open(const std::string &name);
	void close();
	void bind() const;

	bool isSupported(const std::string &extension) const;

	float getGlobalVolume() const;
	void setGlobalVolume(float volume);

	glm::vec3 getPosition() const;
	void setPosition(glm::vec3 position);

	glm::vec3 getDirection() const;
	void setDirection(glm::vec3 direction);

	glm::vec3 getUpVector() const;
	void setUpVector(glm::vec3 upVector);

private:
	void applyOrientation();

private:
	ALCdevice  *mAudioDevice;
	ALCcontext *mAudioContext;

	float mVolume;
	glm::vec3 mPosition;
	glm::vec3 mDirection;
	glm::vec3 mUpVector;
};
