#pragma once

#include <lx_mesh.hpp>
#include <lx_layer.hpp>

// #include <lx_vmodel.hpp>


class TMesh;

class TLayers
{
public:
	TLayers();
	~TLayers();

	class TIterator {
	public:
		TIterator(CLxUser_LayerScan& scan);
		TIterator& operator++();

		bool operator==(const TIterator& rhs) const;
		bool operator!=(const TIterator& rhs) const;

		TMesh operator*();

	private:
		CLxUser_LayerScan& Scan;
		unsigned LayerId = 0;
		unsigned NumLayers;

		friend class TLayers;
	};

	TIterator begin();
	TIterator end();

private:
	CLxUser_LayerService Service;
	CLxUser_LayerScan Scan;
};

