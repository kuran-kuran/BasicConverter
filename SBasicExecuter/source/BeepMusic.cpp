#include "BeepMusic.hpp"
#include <Windows.h>
#include "Format.hpp"

BeepMusic::BeepMusic(void)
:baseFrequency(48000.0)
,volume(8000)
,tempo(4)
,toneList()
,wave(0)
,cycle(0.0)
,nextCycle(-1.0)
,cycleCount(0.0)
,lengthCount(0.0)
,duration(5)
,outOfData(false)
,tone()
{
	this->tempo7lengthTable = 
	{
		baseFrequency / 32.0,	// 32分音符
		baseFrequency / 16.0,	// 16分音符
		baseFrequency / 12.0,	// 付点16分音符
		baseFrequency / 8.0,	// 8分音符
		baseFrequency / 6.0,	// 付点8分音符
		baseFrequency / 4.0,	// 4分音符
		baseFrequency / 3.0,	// 付点4分音符
		baseFrequency / 2.0,	// 2分音符
		baseFrequency / 1.5,	// 付点2分音符
		baseFrequency / 1.0		// 1分音符
	};
}

BeepMusic::~BeepMusic(void)
{
}

void BeepMusic::MML(const dms::String& mml)
{
	int noteTable[] = {69, 71, 60, 62, 64, 65, 67};
	int phase = 0; // 0: +-#記号, 1: 音程, 2:音長
	int octave = 0;
	int addNote = 0;
	int note = 0;
	for(size_t i = 0; i < mml.size(); ++ i)
	{
		unsigned char byte = mml[i];
		if(byte == '+')
		{
			if(phase > 1)
			{
				MakeTone(note, this->duration);
				octave = 0;
				addNote = 0;
				note = 0;
			}
			octave = 1;
			phase = 1;
		}
		if(byte == '-')
		{
			if(phase > 1)
			{
				MakeTone(note, this->duration);
				octave = 0;
				addNote = 0;
				note = 0;
			}
			octave = -1;
			phase = 1;
		}
		if(byte == '#')
		{
			if(phase > 1)
			{
				MakeTone(note, this->duration);
				octave = 0;
				addNote = 0;
				note = 0;
			}
			addNote = 1;
			phase = 1;
		}
		if((byte >= 'A') && (byte <= 'G'))
		{
			if(phase >= 2)
			{
				MakeTone(note, this->duration);
				octave = 0;
				addNote = 0;
				note = 0;
			}
			int baseNote = noteTable[byte - 'A'];
			note = baseNote + 12 * octave + addNote;
			phase = 2;
		}
		if((byte == 'R'))
		{
			if(phase >= 2)
			{
				MakeTone(note, this->duration);
				octave = 0;
				addNote = 0;
				note = 0;
			}
			note = -1;
			phase = 2;
		}
		if((byte >= '0') && (byte <= '9'))
		{
			this->duration = byte - '0';
			phase = 0;
			MakeTone(note, duration);
			octave = 0;
			addNote = 0;
			note = 0;
		}
	}
	if(phase > 0)
	{
		MakeTone(note, this->duration);
	}
}

void BeepMusic::SetTempo(int tempo)
{
	this->tempo = tempo;
}

void BeepMusic::GetBuffer(std::vector<short>& buffer, size_t size)
{
	for(size_t i = 0; i < size; ++ i)
	{
		if(this->toneList.empty() == false)
		{
			this->outOfData = false;
		}
		if(this->outOfData == true)
		{
			DecayWave();
			buffer.push_back(this->wave);
			continue;
		}
		this->lengthCount -= 1.0;
		if(this->lengthCount < 0.0)
		{
			if(this->toneList.empty() == true)
			{
				this->outOfData = true;
				buffer.push_back(this->wave);
				continue;
			}
			this->tone = this->toneList.front();
			this->toneList.pop();
			this->nextCycle = this->tone.cycle;
			this->lengthCount = this->tone.length;
		}
		if(this->cycleCount < -99999.0)
		{
			DecayWave();
			if(this->nextCycle > 0.0)
			{
				this->cycleCount = this->nextCycle;
			}
		}
		else
		{
			this->cycleCount -= 1.0;
			if(this->cycleCount < 0.0)
			{
				if(this->wave <= 0)
				{
					this->wave = this->volume;
				}
				else
				{
					this->wave = -this->volume;
				}
				this->cycleCount = this->nextCycle;
			}
		}
		buffer.push_back(this->wave);
	}
}

bool BeepMusic::Playing(void)
{
	return !this->outOfData;
}

void BeepMusic::DecayWave(void)
{
	if(this->wave < 0)
	{
		this->wave += 1;
		if(this->wave > 0)
		{
			this->wave = 0;
		}
	}
	else
	{
		this->wave -= 1;
		if(this->wave < 0)
		{
			this->wave = 0;
		}
	}
}


void BeepMusic::MakeTone(int note, int noteLength)
{
	double noteFrequency = 0.0;
	double cycle = 0.0;
	if(note >= 0)
	{
		noteFrequency = 440.0 * pow(2.0, ((static_cast<double>( note ) - 69.0) * 100.0) / 1200.0);
		cycle = baseFrequency / noteFrequency / 2.0;
	}
	else
	{
		cycle = -100000.0;
	}
	double tempoSpeedRate = 8.0 - static_cast<double>(this->tempo);
	double length = this->tempo7lengthTable[noteLength];
	length *= tempoSpeedRate;
	Tone toneData = {cycle, length};
	this->toneList.push(toneData);
}
