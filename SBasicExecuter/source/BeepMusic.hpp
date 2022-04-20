#ifndef BEEPMUSIC_HPP
#define BEEPMUSIC_HPP

#include <queue>
#include <vector>
#include "String.hpp"

class BeepMusic
{
public:
	BeepMusic(void);
	~BeepMusic(void);
	void MML(const dms::String& mml);
	void SetTempo(int tempo);
	void GetBuffer(std::vector<short>& buffer, size_t size);
	bool Playing(void);
private:
	struct Tone
	{
		double cycle;
		double length;
	};
	void MakeTone(int note, int noteLength);
	void DecayWave(void);
	double baseFrequency;
	short volume;
	int tempo;
	std::queue<Tone> toneList;
	std::vector<double> tempo7lengthTable;
	short wave;
	double cycle;
	double nextCycle;
	double cycleCount;
	double lengthCount;
	int duration;
	bool outOfData;
	Tone tone;
};

#endif
