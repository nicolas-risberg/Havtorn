// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <array>

namespace Havtorn
{
	class CRenderStateManager
	{
	public:
		enum class EBlendStates
		{
			Disable,
			AlphaBlend,
			AdditiveBlend,
			GBufferAlphaBlend,
			Count
		};

		enum class EDepthStencilStates
		{
			Default,
			OnlyRead,
			StencilWrite,
			StencilMask,
			DepthFirst,
			Count
		};

		enum class ERasterizerStates
		{
			Default,//Uses backface culling.
			Wireframe,
			FrontFaceCulling,
			NoFaceCulling,
			Count
		};

		enum class ESamplerStates
		{
			Trilinear,
			Point,
			Wrap,
			Count
		};

	public:
		CRenderStateManager();
		~CRenderStateManager();

		bool Init(class CGraphicsFramework* aFramework);

		void SetBlendState(EBlendStates aBlendstate);
		void SetDepthStencilState(EDepthStencilStates aDepthStencilState, UINT aStencilRef = 0);
		void SetRasterizerState(ERasterizerStates aRasterizerState);
		void SetSamplerState(ESamplerStates aSamplerState);
		void SetAllStates(EBlendStates aBlendState, EDepthStencilStates aDepthStencilState, ERasterizerStates aRasterizerState, ESamplerStates aSamplerState);
		void SetAllDefault();

		void Release();

	private:
		bool CreateBlendStates(ID3D11Device* aDevice);
		bool CreateDepthStencilStates(ID3D11Device* aDevice);
		bool CreateRasterizerStates(ID3D11Device* aDevice);
		bool CreateSamplerStates(ID3D11Device* aDevice);

	private:
		ID3D11DeviceContext* Context;
		std::array<ID3D11BlendState*, (size_t)EBlendStates::Count> myBlendStates;
		std::array<ID3D11DepthStencilState*, (size_t)EDepthStencilStates::Count> myDepthStencilStates;
		std::array<ID3D11RasterizerState*, (size_t)ERasterizerStates::Count> myRasterizerStates;
		std::array<ID3D11SamplerState*, (size_t)ESamplerStates::Count> mySamplerStates;
	};
}
