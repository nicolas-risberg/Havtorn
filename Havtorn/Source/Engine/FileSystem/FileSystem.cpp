// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "FileSystem.h"

#include <iostream>
#include <rapidjson.h>

using std::fstream;


namespace Havtorn
{
	CFileSystem::CFileSystem()
		: AssetsPath("Assets/")
	{
	}

	bool CFileSystem::DoesFileExist(const std::string& fileName)
	{
		const std::ifstream infile(fileName);
		return infile.good();
	}

	void CFileSystem::OpenFile(const std::string& fileName, EFileMode mode)
	{
		bool openSuccess = false;

		switch (mode)
		{
		case EFileMode::Read: 
			InputStream.open(fileName.c_str(), fstream::in);

			if (InputStream)
				openSuccess = true;

			break;

		case EFileMode::Write: 
			OutputStream.open(fileName.c_str(), fstream::out);

			if (OutputStream)
				openSuccess = true;

			break;

		case EFileMode::BinaryRead: 
			InputStream.open(fileName.c_str(), fstream::in | fstream::binary);

			if (InputStream)
				openSuccess = true;

			break;
			
		case EFileMode::BinaryWrite: 
			OutputStream.open(fileName.c_str(), fstream::out | fstream::binary);

			if (OutputStream)
				openSuccess = true;

			break;
		}

		if (!openSuccess)
			HV_LOG_ERROR("FileSystem could not open file: %s", fileName.c_str());
	}

	void CFileSystem::CloseFile(EFileMode mode)
	{
		bool operationFailure = false;
		std::string streamName;

		switch (mode)
		{
		case EFileMode::Read:
		case EFileMode::BinaryRead:
			InputStream.close();

			if (InputStream.bad())
			{
				operationFailure = true;
				streamName = "InputStream";
			}
			break;

		case EFileMode::Write:
		case EFileMode::BinaryWrite:
			OutputStream.close();

			if (InputStream.bad())
			{
				operationFailure = true;
				streamName = "OutputStream";
			}
			break;
		}

		if (operationFailure)
			HV_LOG_ERROR("FileSystem encountered an operation error after closing the %s", streamName.c_str());
	}

	void CFileSystem::Serialize(const std::string& fileName, const char* data, U32 size)
	{
		OpenFile(fileName, EFileMode::BinaryWrite);
		OutputStream.write(data, size);
		CloseFile(EFileMode::BinaryWrite);
	}

	void CFileSystem::Deserialize(const std::string& fileName, char* data, U32 size)
	{
		OpenFile(fileName, EFileMode::BinaryRead);
		InputStream.read(data, size);
		CloseFile(EFileMode::BinaryRead);
	}

	U64 CFileSystem::GetFileSize(const std::string& fileName) const
	{
		return std::filesystem::file_size(fileName);
	}
	void CFileSystem::IterateThroughFiles(const std::string& root)
	{
		using DirectoryIterator = std::filesystem::recursive_directory_iterator;
		
		for (const auto& dirEntry : DirectoryIterator(root))
		{
			SFilePath filePath = dirEntry;

			HV_LOG_TRACE("Dir: %s", filePath.Directory().c_str());
			HV_LOG_TRACE("Filename: %s", filePath.Filename().c_str());
			HV_LOG_TRACE("Extension: %s", filePath.Extension().c_str());
			HV_LOG_TRACE("Path: %s", filePath.GetPath().c_str());
		}
	}
}

