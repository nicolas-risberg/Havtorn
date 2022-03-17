// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include <filesystem>
#include <fstream>

namespace Havtorn
{
	enum class ESerializeMode
	{
		Binary,
		Readable
	};

	enum class EFileMode
	{
		Read,
		Write,
		BinaryRead,
		BinaryWrite
	};

	class CFileSystem
	{
	public:
		CFileSystem();
		~CFileSystem() = default;

		void OpenFile(const std::string& fileName, EFileMode mode);
		void CloseFile(EFileMode mode);

		void Serialize(const SVector& data, std::string fileName, ESerializeMode mode);
		void DeSerialize(const SVector& data, std::string fileName, ESerializeMode mode);

	private:
		std::ifstream InputStream;
		std::ofstream OutputStream;
		std::string AssetsPath;
	};
}
