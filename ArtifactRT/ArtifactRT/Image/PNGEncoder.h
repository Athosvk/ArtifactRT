#pragma once
#include <vector>
#include <memory>

#include "../Vector3.h"
#include "ImageEncoder.h"


class PNGEncoder : public ImageEncoder
{
public:
	virtual OutputBuffer encode(const Image& image) const override;
};
