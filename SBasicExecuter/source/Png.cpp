#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iterator>
#include "Format.hpp"
#include "Png.hpp"
#include "../library/lpng/pngstruct.h"
#include "../library/lpng/pnginfo.h"

#ifdef _DEBUG
#pragma comment(lib, "../../library/zlib/x64/debug/zlib.lib")
#pragma comment(lib, "../../library/lpng/x64/debug/lpng.lib")
#else
#pragma comment(lib, "../../library/zlib/x64/release/zlib.lib")
#pragma comment(lib, "../../library/lpng/x64/release/lpng.lib")
#endif

namespace dms
{
	Png::Png(void)
	:png_handle(NULL)
	,png_info_data(NULL)
	,width(0)
	,height(0)
	,buffer(NULL)
	,save_buffer(NULL)
	{
	}

	Png::~Png(void)
	{
		Release();
	}

	bool Png::Create(unsigned int width, unsigned int height)
	{
		Release();
		this->width = width;
		this->height = height;
		size_t size = static_cast<size_t>(width * height * 4);
		this->buffer = reinterpret_cast<unsigned char*>(malloc(size));
		memset(this->buffer, 0, size);
		return true;
	}

	bool Png::Load(void* buffer, size_t buffer_size)
	{
		Release();
		// シグネチャ確認
		if(png_sig_cmp(static_cast<png_bytep>(buffer), 0, buffer_size) != 0)
		{
			throw Format("Abort: %s(%u)", __FILE__, __LINE__);
		}
		// ハンドル取得
		this->png_handle = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(this->png_handle == NULL)
		{
			throw Format("Abort: %s(%u)", __FILE__, __LINE__);
		}
		// 情報取得
		this->png_info_data = png_create_info_struct(this->png_handle);
		if(this->png_info_data == NULL)
		{
			throw Format("Abort: %s(%u)", __FILE__, __LINE__);
		}
		png_set_read_fn(this->png_handle, static_cast<png_voidp>(&buffer), static_cast<png_rw_ptr>(ReadCallback));
		png_read_info(this->png_handle, this->png_info_data);
		png_uint_32 png_width;
		png_uint_32 png_height;
		int bpp;
		int color_type;
		png_get_IHDR(this->png_handle, this->png_info_data, &png_width, &png_height, &bpp, &color_type, NULL, NULL, NULL);
		// 展開フォーマット設定
		if(png_get_valid(this->png_handle, this->png_info_data, PNG_INFO_tRNS))
		{
			png_set_expand(this->png_handle);
		}
		if(color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_expand(this->png_handle);
		}
		if((color_type == PNG_COLOR_TYPE_GRAY) && (bpp < 8))
		{
			png_set_expand(this->png_handle);
		}
		if(bpp > 8)
		{
			png_set_strip_16(this->png_handle);
		}
		if(color_type == PNG_COLOR_TYPE_GRAY)
		{
			png_set_gray_to_rgb(this->png_handle);
		}
		// linesに展開バッファ位置設定
		unsigned char **lines;
		unsigned int* workbuffer;
		// workbufferを破棄せずにそのまま使う
		workbuffer = static_cast<unsigned int*>(malloc(static_cast<size_t>(png_width) * png_height * 4));
		this->buffer = reinterpret_cast<unsigned char*>(workbuffer);
		lines = static_cast<unsigned char **>(malloc(sizeof(unsigned char*) * png_height));
		for(unsigned int i = 0; i < png_height; i ++)
		{
			lines[i] = reinterpret_cast<unsigned char*>(workbuffer + png_width * i);
		}
		if(!(color_type & PNG_COLOR_MASK_ALPHA))
		{
			png_set_filler(this->png_handle, 0, 1);
		}
		// 展開フォーマット設定
		png_set_bgr(this->png_handle); // B8G8R8A8に変更する
		// bufferにPNG展開
		png_read_image(this->png_handle, lines);
		free(lines);
		// 24ビット時はアルファを0xFFにする
		if(this->png_info_data->pixel_depth == 24)
		{
			for(png_uint_32 y = 0; y < png_height; ++ y)
			{
				for(png_uint_32 x = 0; x < png_width; ++ x)
				{
					this->buffer[y * png_width * 4 + x * 4 + 3] = 0xFF;
				}
			}
		}
		this->width = png_width;
		this->height = png_height;
		return true;
	}

	bool Png::Save(std::vector<unsigned char>& buffer)
	{
		png_bytepp image;
		image = reinterpret_cast<png_bytepp>(malloc(static_cast<size_t>(this->height * sizeof(png_bytep))));
		for(unsigned int y = 0; y < this->height; y ++)
		{
			image[y] = reinterpret_cast<png_bytep>(&this->buffer[this->width * 4 * y]);
		}
		png_structp png_ptr;
		png_infop info_ptr;
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(png_ptr == NULL)
		{
			throw Format("Abort: %s(%u)", __FILE__, __LINE__);
		}
		info_ptr = png_create_info_struct(png_ptr);
		if(info_ptr == NULL)
		{
			png_destroy_write_struct(&png_ptr, reinterpret_cast<png_infopp>(NULL));
			throw Format("Abort: %s(%u)", __FILE__, __LINE__);
		}
#if 0
		if(setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_write_struct(&png_ptr, &info_ptr);
			throw dms::Format("Abort: %s(%u)", __FILE__, __LINE__);
			return false;
		}
#endif
		this->save_buffer = &buffer;
		png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
		png_set_write_fn(png_ptr, static_cast<png_voidp>(this), static_cast<png_rw_ptr>(WriteCallback), static_cast<png_flush_ptr>(FlushCallback));
		png_set_IHDR(png_ptr, info_ptr, static_cast<png_uint_32>(this->width), static_cast<png_uint_32>(this->height), 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png_ptr, info_ptr);
		png_write_image(png_ptr, image);
		png_write_end(png_ptr, info_ptr);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		free(image);
		return true;
	}

	void Png::Release(void)
	{
		if((this->png_handle != NULL) && (this->png_info_data != NULL))
		{
			png_destroy_read_struct(&this->png_handle, &this->png_info_data, NULL);
		}
		this->png_handle = NULL;
		this->png_info_data = NULL;
		if(buffer != NULL)
		{
			free(this->buffer);
		}
		this->width = 0;
		this->height = 0;
		this->buffer = NULL;
		this->save_buffer = NULL;
	}

	unsigned int Png::GetWidth(void) const
	{
		return this->width;
	}

	unsigned int Png::GetHeight(void) const
	{
		return this->height;
	}

	unsigned char* Png::GetBuffer(void) const
	{
		return this->buffer;
	}

	Png::Color Png::GetPixel(int x, int y) const
	{
		Color color = {0.0f, 0.0f, 0.0f, 0.0f};
		color.red = static_cast<float>(this->buffer[y * this->width * 4 + x * 4]) / 255.0f;
		color.blue = static_cast<float>(this->buffer[y * this->width * 4 + x * 4 + 1]) / 255.0f;
		color.green = static_cast<float>(this->buffer[y * this->width * 4 + x * 4 + 2]) / 255.0f;
		color.alpha = static_cast<float>(this->buffer[y * this->width * 4 + x * 4 + 3]) / 255.0f;
		return color;
	}

	void Png::SetPixel(int x, int y, Png::Color& color)
	{
		this->buffer[y * this->width * 4 + x * 4] = static_cast<unsigned char>(color.red * 255.0f);
		this->buffer[y * this->width * 4 + x * 4 + 1] = static_cast<unsigned char>(color.blue * 255.0f);
		this->buffer[y * this->width * 4 + x * 4 + 2] = static_cast<unsigned char>(color.green * 255.0f);
		this->buffer[y * this->width * 4 + x * 4 + 3] = static_cast<unsigned char>(color.alpha * 255.0f);
	}

	void Png::SetPixelBuffer(void* buffer, size_t buffer_size)
	{
		size_t image_buffer_size = static_cast<size_t>(this->width) * this->height * 4;
		if(buffer_size > image_buffer_size)
		{
			buffer_size = image_buffer_size;
		}
		memcpy(this->buffer, buffer, buffer_size);
	}

	void Png::ReadCallback(png_struct* png_handle, png_bytep buffer, png_size_t size)
	{
		unsigned char** data = static_cast<unsigned char**>(png_get_io_ptr(png_handle));
		memcpy(buffer, *data, size);
		*data += static_cast<int>(size);
	}

	void Png::FlushCallback(png_struct* png_handle)
	{
#if 0
		Png* png = static_cast<Png*>(png_get_io_ptr(png_handle));
#else
		(void)png_handle;
#endif
	}

	void Png::WriteCallback(png_struct* png_handle, png_byte* buffer, png_size_t size)
	{
		Png* png = static_cast<Png*>(png_get_io_ptr(png_handle));
		// pngイメージをsave_bufferに転送
#if 0
		size_t buffer_base = png->save_buffer->size();
		size_t buffer_size = buffer_base + static_cast<size_t>(size);
		png->save_buffer->resize(buffer_size);
		memcpy(&(*png->save_buffer)[buffer_base], buffer, size);
#else
		std::copy(buffer, buffer + size, std::back_inserter(*png->save_buffer));
#endif
	}
};
