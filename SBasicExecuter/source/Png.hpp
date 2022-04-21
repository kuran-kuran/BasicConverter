#ifndef PNG_HPP
#define PNG_HPP

#include <vector>
#include "../library/lpng/png.h"
#include "../library/zlib/zlib.h"

namespace dms
{
	// PNG読み込み、書き込み両方R8G8B8A8のみ対応
	class Png
	{
	public:
		struct Color
		{
			float red;
			float blue;
			float green;
			float alpha;
		};
		Png(void);
		~Png(void);
		bool Create(unsigned int width, unsigned int height);
		bool Load(void* buffer, size_t buffer_size);
		bool Save(std::vector<unsigned char>& buffer);
		void Release(void);
		unsigned int GetWidth(void) const;
		unsigned int GetHeight(void) const;
		unsigned char* GetBuffer(void) const;
		Color GetPixel(int x, int y) const;
		void SetPixel(int x, int y, Color& color);
		void SetPixelBuffer(void* buffer, size_t buffer_size);
		static void ReadCallback(png_struct* png_handle, png_bytep buffer, png_size_t size);
		static void FlushCallback(png_struct* png_handle);
		static void WriteCallback(png_struct* png_handle, png_bytep buffer, png_size_t size);
	private:
		png_struct* png_handle;
		png_infop png_info_data;
		unsigned int width;
		unsigned int height;
		unsigned char* buffer; // フォーマットはR8G8B8A8(DirectXはB8G8R8A8、OpenGLはR8G8B8A8)
		std::vector<unsigned char>* save_buffer; // Save時にPNG形式で格納される
	private:
		Png(Png&);
		Png& operator = (Png&);
	};
};

#endif
