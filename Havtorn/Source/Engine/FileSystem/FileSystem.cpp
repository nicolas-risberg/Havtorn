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
		//OpenFile(AssetsPath + "ExampleAsset.hvasset", EFileMode::BinaryWrite);

		//std::vector<SVector> vectors;
		//vectors.emplace_back(SVector::Right);
		//vectors.emplace_back(SVector::Up);
		//vectors.emplace_back(SVector::Forward);

		//for (U8 i = 0; i < 3; i++)
		//	OutputStream.write((char*)&vectors[i], sizeof(SVector));

		//CloseFile(EFileMode::BinaryWrite);

		//vectors.clear();

		//OpenFile(AssetsPath + "ExampleAsset.hvasset", EFileMode::BinaryRead);

		//for (U8 i = 0; i < 3; i++)
		//{
		//	vectors.emplace_back();
		//	InputStream.read((char*)&vectors.back(), sizeof(SVector));
		//}

		//CloseFile(EFileMode::BinaryRead);
		//
		//std::cout << "Data:" << std::endl;
		//for (int i = 0; i < 3; i++) {
		//	std::cout << vectors[i].ToString() << std::endl;
		//	std::cout << std::endl;
		//}
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
}

