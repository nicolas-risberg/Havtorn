// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "LayerStack.h"

namespace Havtorn
{
	CLayerStack::CLayerStack()
	{
		LayerInsert = Layers.begin();
	}

	CLayerStack::~CLayerStack()
	{
		for (CLayer* layer : Layers)
			delete layer;
	}

	void CLayerStack::PushLayer(CLayer* layer)
	{
		LayerInsert = Layers.emplace(LayerInsert, layer);
	}

	void CLayerStack::PushOverlay(CLayer* overlay)
	{
		Layers.emplace_back(overlay);
	}

	void CLayerStack::PopLayer(CLayer* layer)
	{
		auto it = std::find(Layers.begin(), Layers.end(), layer);
		if (it != Layers.end())
		{
			Layers.erase(it);
			LayerInsert--;
		}
	}

	void CLayerStack::PopOverlay(CLayer* overlay)
	{
		auto it = std::find(Layers.begin(), Layers.end(), overlay);
		if (it != Layers.end())
			Layers.erase(it);
	}
}