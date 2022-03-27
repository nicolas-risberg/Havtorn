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

		static bool DoesFileExist(const std::string& fileName);

		void OpenFile(const std::string& fileName, EFileMode mode);
		void CloseFile(EFileMode mode);

		void Serialize(const SVector& data, std::string fileName, ESerializeMode mode);
		void DeSerialize(const SVector& data, std::string fileName, ESerializeMode mode);

		void Serialize(const std::string& fileName, const char* data, U32 size);
		void Deserialize(const std::string& fileName, char* data, U32 size);
		U64 GetFileSize(const std::string& fileName) const;

	private:
		std::ifstream InputStream;
		std::ofstream OutputStream;
		std::string AssetsPath;
	};
}
