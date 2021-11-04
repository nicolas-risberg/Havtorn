workspace "NewEngine"
	architecture "x64"
	location "Source"

	configurations
	{ "Debug", "Release" }
	startproject "Launcher"

outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"
engineProj = "Engine"
engineSource = "Source/" .. engineProj .. "/"

project "Engine"
	location "Source/Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"

	targetname "%{prj.name}_%{cfg.buildcfg}"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("Temp/" .. outputdir .. "/%{prj.name}") 

	warnings "Extra"
	flags { "FatalWarnings" }

	files 
	{
		"Source/%{prj.name}/**.h",
		"Source/%{prj.name}/**.cpp",
		
		vpaths 
		{
			["Core"] = 
			{
				engineSource .. "Core.h",
				engineSource .. "CoreTypes.h",
				engineSource .. "EngineMath.h",
				engineSource .. "EngineMathSSE.h",
				engineSource .. "EngineTypes.h",
				engineSource .. "EntryPoint.h",
				engineSource .. "Matrix.h",
				engineSource .. "Quaternion.h",
				engineSource .. "Rotator.h",
				engineSource .. "Vector.h",
				engineSource .. "WindowsInclude.h"
			},
		}
	}

	includedirs
	{
		"External/spdlog/include"
	}

	filter "system:Windows"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"NE_PLATFORM_WINDOWS",
			"NE_BUILD_DLL"
		}

		postbuildcommands
		{
			"{MKDIR} ../../Bin/",
			"{COPY} %{cfg.buildtarget.relpath} ../../Bin/"
		}

	filter "configurations:Debug"
		defines "NE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "NE_RELEASE"
		optimize "On"

project "Launcher"
	location "Source/Launcher"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"

	targetname "%{prj.name}_%{cfg.buildcfg}"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("Temp/" .. outputdir .. "/%{prj.name}") 

	warnings "Extra"
	flags { "FatalWarnings" }

	files
	{
		"Source/%{prj.name}/**.h",
		"Source/%{prj.name}/**.cpp",
	}

	includedirs
	{
		"External/spdlog/include",
		"Source/Engine"
	}

	links
	{
		"Engine"
	}

	filter "system:Windows"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"NE_PLATFORM_WINDOWS"
		}
		
		postbuildcommands
		{
			"{MKDIR} ../../Bin/",
			"{COPY} %{cfg.buildtarget.relpath} ../../Bin/"
		}

	filter "configurations:Debug"
		defines "NE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "NE_RELEASE"
		optimize "On"