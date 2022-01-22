// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Layer.h"

#pragma warning(disable : 4251)

namespace Havtorn
{
	class HAVTORN_API CLayerStack
	{
	public:
		CLayerStack();
		~CLayerStack();

		void PushLayer(CLayer* layer);
		void PushOverlay(CLayer* overlay);
		void PopLayer(CLayer* layer);
		void PopOverlay(CLayer* overlay);

		std::vector<CLayer*>::iterator begin() { return Layers.begin(); }
		std::vector<CLayer*>::iterator end() { return Layers.end(); }

	private:
		std::vector<CLayer*> Layers;
		std::vector<CLayer*>::iterator LayerInsert;
	};
}

