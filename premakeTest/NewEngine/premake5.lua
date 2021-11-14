workspace "NewEngine"
	architecture "x86_64"
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
	architecture "x86_64"

	targetname "%{prj.name}_%{cfg.buildcfg}"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("Temp/" .. outputdir .. "/%{prj.name}") 

	warnings "Extra"
	flags { "FatalWarnings", "ShadowedVariables" }

	files 
	{
		"Source/%{prj.name}/**.h",
		"Source/%{prj.name}/**.cpp",
		
		vpaths 
		{
			["*"] = "Source/"
		}
	}

	includedirs
	{
		"External/spdlog/include"
	}

	floatingpoint "Fast"
	debugdir "Bin/"

	filter "system:Windows"
		staticruntime "On"
		systemversion "latest"
		vectorextensions "SSE4.1"

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
		flags { "LinkTimeOptimization" }

project "Launcher"
	location "Source/Launcher"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	architecture "x86_64"

	targetname "%{prj.name}_%{cfg.buildcfg}"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("Temp/" .. outputdir .. "/%{prj.name}") 

	warnings "Extra"
	flags { "FatalWarnings", "ShadowedVariables" }

	files
	{
		"Source/%{prj.name}/**.h",
		"Source/%{prj.name}/**.cpp",

		vpaths 
		{
			["*"] = "Source/"
		}
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

	floatingpoint "Fast"
	debugdir "Bin/"

	filter "system:Windows"
		staticruntime "On"
		systemversion "latest"
		vectorextensions "SSE4.1"

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
		flags { "LinkTimeOptimization" }