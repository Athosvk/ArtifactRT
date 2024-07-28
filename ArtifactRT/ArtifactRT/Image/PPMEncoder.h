#pragma once
#include "ImageEncoder.h"

class PPMEncoder : public ImageEncoder
{
public:
	// Inherited via ImageEncoder
	virtual OutputBuffer encode(const Image& image) const override;
};

