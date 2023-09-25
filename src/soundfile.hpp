#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class SoundFile
{
public:
	using Ptr = std::unique_ptr<SoundFile>;

	struct Info
	{
		unsigned    channelCount = 0;
		unsigned    sampleRate   = 0;
		std::size_t sampleCount  = 0;
	};

public:
	virtual ~SoundFile() = default;
	virtual bool open(const std::string &filename, Info &info) = 0;
	virtual void seek(std::uint64_t sampleOffset) = 0;
	virtual std::size_t read(std::int16_t *samples, std::size_t size) = 0;

	template <typename T>
	static void registerFile();
	static Ptr createFromFilename(const std::string &filename, Info &info);

private:
	static std::vector<std::function<Ptr()>> mFactories;
};

template <typename T>
void SoundFile::registerFile()
{
	mFactories.push_back([]() {
		return std::make_unique<T>();
	});
}
