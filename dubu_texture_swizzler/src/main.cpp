#include <dubu_texture_swizzler/DubuTextureSwizzler.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <dubu_texture_swizzler/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <dubu_texture_swizzler/stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

int main() {
	DubuTextureSwizzler app;
	app.Run();

	return 0;
}