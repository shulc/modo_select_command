#include "layer.h"

#include "mesh.h"
#include "iterator.h"

TLayers::TLayers()
{
	Service.BeginScan(LXf_LAYERSCAN_EDIT_POLYS, Scan);
}

TLayers::~TLayers()
{
	Scan.Apply();
}

TLayers::TIterator TLayers::begin()
{
	return TIterator(Scan);
}

TLayers::TIterator TLayers::end()
{
	TIterator it(Scan);
	it.LayerId = it.NumLayers;
	return it;
}

TLayers::TIterator::TIterator(CLxUser_LayerScan& scan)
	: Scan(scan)
	, NumLayers(Scan.NumLayers())
{
}



TLayers::TIterator& TLayers::TIterator::operator++()
{
	++this->LayerId;
	return *this;
}

bool TLayers::TIterator::operator==(const TIterator& rhs) const
{
	return LayerId == rhs.LayerId;
}

bool TLayers::TIterator::operator!=(const TIterator& rhs) const
{
	return !(this->operator==(rhs));
}

TMesh TLayers::TIterator::operator*()
{
	CLxUser_Mesh mesh;
	Scan.EditMeshByIndex(LayerId, mesh);
	return TMesh(mesh, Scan, LayerId);
}
