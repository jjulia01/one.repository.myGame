struct ButtonState {
	bool isDown;
	bool changed;
};

enum {
	BUTTON_UP,		// <- 0
	BUTTON_DOWN,	// <- 1
	BUTTON_LEFT,	// <- 2
	BUTTON_RIGHT,	// <- 3

	BUTTON_COUNT	// <- 4
};

struct Input {
	ButtonState buttons[BUTTON_COUNT];
};