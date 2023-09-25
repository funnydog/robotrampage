#pragma once

#include <al.h>
#include <alc.h>

#include <filesystem>
#include <string_view>

#ifndef NDEBUG
#define alCheck(expr) do { expr; alCheckError(__FILE__, __LINE__, #expr); } while (0)
#else
#define alCheck(expr) do { expr; } while (0)
#endif

void alCheckError(const std::filesystem::path &file, unsigned line, std::string_view expr);
