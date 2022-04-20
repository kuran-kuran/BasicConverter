#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <map>
#include <Windows.h>

class KeyBoard
{
public:
	static inline KeyBoard& GetInstance(void)
	{
		static KeyBoard instance;
		return instance;
	}
	void SetKeyUp(WPARAM wParam, LPARAM lParam);
	void SetKeyDown(WPARAM wParam, LPARAM lParam);
	void SetGrph(bool grph);
	void SetKana(bool kana);
	bool GetGrph(void);
	bool GetKana(void);
	unsigned char Get(void);
protected:
	KeyBoard(void);
	~KeyBoard(void);
private:
	struct Change
	{
		unsigned char normal;
		unsigned char shift;
		unsigned char grph;
		unsigned char shift_grph;
		unsigned char kana;
		unsigned char shift_kana;
	};
	WPARAM beforeCode;
	unsigned char code;
	bool kana;
	bool grph;
	bool shift;
	bool sftlock;
	std::map<unsigned char, Change> changeTable;
};

#endif
