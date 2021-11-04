workspace "NewEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cgf.system}-%{cfg.architecture}"

project "Engine"
	location "Source/Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("Temp/" .. outputdir .. "/%{prj.name}") 

	files
	{
		"Source/%{prj.name}/**.h",
		"Source/%{prj.name}/**.cpp",
	}

	includedirs
	{
		"External/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"NE_PLATFORM_WINDOWS",
			"NE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../Bin/" .. outputdir)
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

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("Temp/" .. outputdir .. "/%{prj.name}") 

	files
	{
		"Source/%{prj.name}/**.h",
		"Source/%{prj.name}/**.cpp",
	}

	includedirs
	{
		"External/spdlog/include"
		"Source/Engine"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"NE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "NE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "NE_RELEASE"
		optimize "On"