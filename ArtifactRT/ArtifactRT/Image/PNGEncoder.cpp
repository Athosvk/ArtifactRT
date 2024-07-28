#include "PNGEncoder.h"

// Needed because SPNG is compiled in a separate project statically
#define SPNG_STATIC
#include <spng.h>
#include <cassert>

#include "Image.h"

OutputBuffer PNGEncoder::encode(const Image& image) const
{
    spng_ihdr header = {
		.width = image.Dimensions.Width,
		.height = image.Dimensions.Height,
		.bit_depth = 8,
		.color_type = spng_color_type::SPNG_COLOR_TYPE_TRUECOLOR
    };
    spng_ctx* png_encoder_context = spng_ctx_new(SPNG_CTX_ENCODER);

    spng_set_option(png_encoder_context, SPNG_ENCODE_TO_BUFFER, 1);
    auto return_value = spng_set_ihdr(png_encoder_context, &header);
    if (return_value)
    {
        std::cout << "Encoding error: " << spng_strerror(return_value) << "\n";
    }
    assert(return_value == 0 && "Could not set header");

    auto pixel_data = image.pixelsAsBytes();
    return_value = spng_encode_image(png_encoder_context, static_cast<const void*>(pixel_data.data()), pixel_data.size(),
        SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);

    if (return_value)
    {
        std::cout << "Encoding error: " << spng_strerror(return_value) << "\n";
    }
    assert(return_value == 0 && "Could not encode PNG");

    size_t output_buffer_size;
    char* output_buffer = static_cast<char*>(spng_get_png_buffer(png_encoder_context, &output_buffer_size, &return_value));

    if (return_value)
    {
        std::cout << "Error retrieving encoded PNG: " << spng_strerror(return_value) << "\n";
    }
    assert(return_value == 0 && "Could not retrieve encoded PNG");
    return OutputBuffer{ std::unique_ptr<char[]>(output_buffer), output_buffer_size };
}
