#if defined(_WIN32)

#include <stdlib.h>
#include <windows.h>
#include "WinMain.hpp"
#include "SoundStream.hpp"

#pragma comment(lib, "dsound.lib")

SoundStream* SoundStream::sound_stream;

SoundStream::SoundStream(int frequency)
:direct_sound(NULL)
,primary_buffer(NULL)
,secondry_buffer(NULL)
,frequency(frequency)
,volume(0)
,request(true)
,stream_buffer_size(SOUND_BUFFER_SIZE * 2)
,stream_buffer_size_half(SOUND_BUFFER_SIZE)
,preload(true)
,stream_buffer_toggle(0)
,directsound_playing(false)
,position_add(0)
,before_position(0)
,position(0)
{
	if(FAILED(DirectSoundCreate(NULL, &this->direct_sound, NULL)))
	{
		return;
	}
	HWND window_handle = GetWindowHandle();
	if(FAILED(this->direct_sound->SetCooperativeLevel(window_handle, DSSCL_PRIORITY)))
	{
		Release();
		return;
	}
	// プライマリバッファ
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	if(FAILED(this->direct_sound->CreateSoundBuffer(&dsbd, &this->primary_buffer, NULL)))
	{
		Release();
		return;
	}
	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(wfex));
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.nChannels = 2;
	wfex.nSamplesPerSec = 48000;
	wfex.wBitsPerSample = 16;
	wfex.nBlockAlign = (wfex.nChannels * wfex.wBitsPerSample) / 8;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
	if(FAILED(this->primary_buffer->SetFormat(&wfex)))
	{
		Release();
		return;
	}
	// セカンダリバッファ
	WAVEFORMATEX pcmwf;
	ZeroMemory(&pcmwf, sizeof(WAVEFORMATEX));
	pcmwf.wFormatTag = WAVE_FORMAT_PCM;
	pcmwf.nChannels = 1;
	pcmwf.nSamplesPerSec = frequency;
	pcmwf.wBitsPerSample = 16;
	pcmwf.nBlockAlign = (pcmwf.nChannels * pcmwf.wBitsPerSample) / 8;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_STICKYFOCUS | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = static_cast<DWORD>(stream_buffer_size);
	dsbd.lpwfxFormat = &pcmwf;
	if(FAILED(this->direct_sound->CreateSoundBuffer(&dsbd, &this->secondry_buffer, NULL)))
	{
		Release();
	}
	WriteData(0, NULL);
	WriteData(1, NULL);
}

SoundStream::~SoundStream(void)
{
	Release();
}

void SoundStream::Initialize(int frequency)
{
	SoundStream::sound_stream = new SoundStream(frequency);
}

void SoundStream::Finalize(void)
{
	if(SoundStream::sound_stream != NULL)
	{
		delete SoundStream::sound_stream;
		SoundStream::sound_stream = NULL;
	}
}

SoundStream& SoundStream::GetInstance(void)
{
	return *SoundStream::sound_stream;
}

void SoundStream::Release(void)
{
	if(this->primary_buffer != NULL)
	{
		this->primary_buffer->Release();
		this->primary_buffer = NULL;
	}
	if(this->direct_sound != NULL)
	{
		this->direct_sound->Release();
		this->direct_sound = NULL;
	}
	if(this->secondry_buffer != NULL)
	{
		this->secondry_buffer->Release();
		this->secondry_buffer = NULL;
	}
}

void SoundStream::Update(void)
{
	if(this->directsound_playing == true)
	{
		// 次のデータが必要か
		DWORD current_position;
		secondry_buffer->GetCurrentPosition(&current_position, 0);
		if(request == false)
		{
			if(current_position >= this->stream_buffer_size_half)
			{
				if(this->stream_buffer_toggle == 1)
				{
					this->request = true;
				}
			}
			else
			{
				if(this->stream_buffer_toggle == 0)
				{
					this->request = true;
				}
			}
		}
		// 再生位置取得
		if(current_position < this->before_position)
		{
			// 再生位置が先頭に戻った
			this->position_add = static_cast<unsigned int>(this->stream_buffer_size_half) - this->before_position + static_cast<unsigned int>(current_position);
		}
		else
		{
			// 再生位置が進んだ
			this->position_add = current_position - this->before_position;
		}
		this->position += this->position_add;
		this->before_position = current_position;
	}
}

// Samples
int SoundStream::GetWritableSize(void) const
{
	if(this->request == true)
	{
		return static_cast<int>(SOUND_BUFFER_SIZE / 2);
	}
	return 0;
}

bool SoundStream::Write(const short* buffer)
{
	if(this->request == false)
	{
		return false;
	}
	if(this->preload == true)
	{
		WriteData(0, buffer);
		this->secondry_buffer->Play(0, 0, DSBPLAY_LOOPING);
		this->directsound_playing = true;
		this->preload = false;
		return true;
	}
	int stream_buffer_toggle;
	DWORD current_position;
	this->secondry_buffer->GetCurrentPosition(&current_position, 0);
	if(current_position >= this->stream_buffer_size_half)
	{
		// 前半に書き込む
		stream_buffer_toggle = 0;
	}
	else
	{
		// 後半に書き込む
		stream_buffer_toggle = 1;
	}
	WriteData(stream_buffer_toggle, buffer);
	this->stream_buffer_toggle = stream_buffer_toggle;
	this->request = false;
	return true;
}

void SoundStream::WriteData(int stream_buffer_index, const void* buffer)
{
	// バッファ書き込み
	WORD* ptr1;
	WORD* ptr2;
	DWORD size1;
	DWORD size2;
	DWORD write_size = static_cast<DWORD>(this->stream_buffer_size_half);
	const char* writeBuffer = reinterpret_cast<const char*>(buffer);
	HRESULT result = this->secondry_buffer->Lock(static_cast<DWORD>(this->stream_buffer_size_half) * stream_buffer_index, static_cast<DWORD>(this->stream_buffer_size_half), reinterpret_cast<void**>(&ptr1), &size1, reinterpret_cast<void**>(&ptr2), &size2, 0);
	if(result == DSERR_BUFFERLOST)
	{
		this->secondry_buffer->Restore();
	}
	if(ptr1 != NULL)
	{
		if(writeBuffer == NULL)
		{
			ZeroMemory(ptr1, size1);
		}
		else
		{
			if(write_size > size1)
			{
				memcpy(ptr1, writeBuffer, size1);
				write_size -= size1;
			}
			else if(write_size > 0)
			{
				memcpy(ptr1, writeBuffer, write_size);
				write_size = 0;
			}
		}
	}
	if(ptr2 != NULL)
	{
		if(writeBuffer == NULL)
		{
			ZeroMemory(ptr2, size2);
		}
		else
		{
			if(write_size > size2)
			{
				memcpy(ptr2, &writeBuffer[size1 / 2], size2);
				write_size -= size2;
			}
			else if(write_size > 0)
			{
				memcpy(ptr2, &writeBuffer[size1 / 2], write_size);
				write_size = 0;
			}
		}
	}
	this->secondry_buffer->Unlock(ptr1, size1, ptr2, size2);
}

// volume = 0-10
void SoundStream::SetVolume(int volume)
{
	this->volume = volume;
}

int SoundStream::GetVolume(void)
{
	return this->volume;
}

#endif
