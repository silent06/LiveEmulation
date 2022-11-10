#include "stdafx.h"
static MenuInput g_menuInput;
Color g_tempColor;

void MenuInput::Update() {
	if (!GetMenu()->IsOpen())return;
	for (int i = 0; i < m_functionQueue.size(); i++) m_functionQueue[i]();
	m_functionQueue.clear();
}

void MenuInput::Setup() {
	GetMenu()->m_disableAllInputThisFrame = true;
}

Color HSVToRGB(float h, float s, float v, int origAlpha = 255) {
	float r = 0, g = 0, b = 0;

	if (s == 0) {
		r = v;
		g = v;
		b = v;
	}
	else {
		int i;
		double f, p, q, t;

		if (h == 360)h = 0;
		else h = h / 60;

		i = (int)Utils::trunc(h);
		f = h - i;

		p = v * (1.0f - s);
		q = v * (1.0f - (s * f));
		t = v * (1.0f - (s * (1.0f - f)));

		switch (i) {
		case 0: r = v; g = t; b = p; break;
		case 1: r = q; g = v; b = p; break;
		case 2: r = p; g = v; b = t; break;
		case 3: r = p; g = q; b = v; break;
		case 4: r = t; g = p; b = v; break;
		default: r = v; g = p; b = q; break;
		}

	}

	return Color((int)(r * 255), (int)(g * 255), (int)(b * 255), origAlpha);
};

HSV RGBToHSV(RGBA in) {
	HSV out;
	double min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.v = max;
	delta = max - min;
	if (delta < 0.00001) {
		out.s = 0;
		out.h = 0;
		return out;
	}
	if (max > 0.0) {
		out.s = (delta / max);
	} else {
		out.s = 0.0;
		out.h = 0.0;
		return out;
	}
	if (in.r >= max)
		out.h = (in.g - in.b) / delta;
	else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;
		else
			out.h = 4.0 + (in.r - in.g) / delta;

	out.h *= 60.0;

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}

void MenuInput::Col() {
	static int hueSlider = 0;
	static bool sliderSelected = false;
	static bool alphaSelected = false;
	Color originalCol = *m_changingColor;
	static int sX = GRID_COUNT, sY = GRID_COUNT;
	
	GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - 160 - 15 - (7.5), GetMenu()->m_center.y - ((GRID_COUNT * GRID_ITEM_HEIGHT) / 2) - 50 - 1, 369 + 15, 40, 1, Color(0, 0, 0, 150), m_gridColor, true); // top bar
	GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - 160 - 15 - (7.5), 1 + GetMenu()->m_center.y - ((GRID_COUNT * GRID_ITEM_HEIGHT) / 2) - 10 + (GRID_COUNT * GRID_ITEM_HEIGHT) + 30, 369 + 15, 40, 1, Color(0, 0, 0, 150), m_gridColor, true); // bottom bar
	GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - ((GRID_COUNT * GRID_ITEM_WIDTH) / 2) - 10 - (7.5), GetMenu()->m_center.y - ((GRID_COUNT * GRID_ITEM_HEIGHT) / 2) - 10, (GRID_COUNT * GRID_ITEM_WIDTH) + 40 + 15, (GRID_COUNT * GRID_ITEM_HEIGHT) + 30, 1, Color(57, 57, 57, 100), m_gridColor, true); // bg
	GetRenderer()->DrawString(m_name, GetMenu()->m_center.x + 9, GetMenu()->m_center.y - 168, GetRenderer()->Font.smallFont, 0.90f, Color(255, 255, 255), false, false, JustifyCenter);

	for (int xx = 0; xx <= SELECTION_COUNT; xx++) {
		int defX = GetMenu()->m_center.x + (((GRID_COUNT + 1) * GRID_ITEM_WIDTH) / 2) + 25;
		GetRenderer()->DrawRect(defX - (7.5), GetMenu()->m_center.y + (xx * SELECTION_ITEM_HEIGHT) - ((SELECTION_COUNT * SELECTION_ITEM_HEIGHT) / 2), SELECTION_ITEM_WIDTH, SELECTION_ITEM_HEIGHT, JustifyLeft, HSVToRGB(xx * (360 / SELECTION_COUNT), 1, 1));

		int alpha = (((255 * xx) / SELECTION_COUNT));
		defX -= 15;
		GetRenderer()->DrawRect(defX - (7.5), GetMenu()->m_center.y + (xx * SELECTION_ITEM_HEIGHT) - ((SELECTION_COUNT * SELECTION_ITEM_HEIGHT) / 2), SELECTION_ITEM_WIDTH, SELECTION_ITEM_HEIGHT, JustifyLeft, HSVToRGB(hueSlider, sX * (1.0f / GRID_COUNT), sY * (1.0f / GRID_COUNT), alpha));
	}

	for (int xx = 0; xx <= GRID_COUNT; xx++) {
		for (int yy = 0; yy <= GRID_COUNT; yy++) {
			GetRenderer()->DrawRect(GetMenu()->m_center.x + (xx * GRID_ITEM_WIDTH) - ((GRID_COUNT * GRID_ITEM_WIDTH) / 2) - (7.5), GetMenu()->m_center.y + (yy * GRID_ITEM_HEIGHT) - ((GRID_COUNT * GRID_ITEM_HEIGHT) / 2), GRID_ITEM_WIDTH, GRID_ITEM_HEIGHT, JustifyLeft, HSVToRGB(hueSlider, xx * (1.0f / GRID_COUNT), yy * (1.0f / GRID_COUNT)));
		}
	}

	int sliderSelectedXPos = GetMenu()->m_center.x + (((GRID_COUNT + 1) * GRID_ITEM_WIDTH) / 2) + 25;
	float sliderSelectedYPos = GetMenu()->m_center.y + ((((SELECTION_COUNT * SELECTION_ITEM_HEIGHT) * hueSlider) / 360)) - ((SELECTION_COUNT * SELECTION_ITEM_HEIGHT) / 2);
	GetRenderer()->DrawOutlinedRect(sliderSelectedXPos - 1 - (7.5), sliderSelectedYPos - 1, SELECTION_ITEM_WIDTH + 2, SELECTION_ITEM_HEIGHT + 2, 1, HSVToRGB(hueSlider, 1, 1), sliderSelected ? Color(255, 255, 255) : Color(196, 196, 196), true);

	int alphaSelectedXPos = GetMenu()->m_center.x + (((GRID_COUNT + 1) * GRID_ITEM_WIDTH) / 2) + 10;
	float alphaSelectedYPos = GetMenu()->m_center.y + ((((SELECTION_COUNT * SELECTION_ITEM_HEIGHT) * alphaSlider) / 255)) - ((SELECTION_COUNT * SELECTION_ITEM_HEIGHT) / 2);
	GetRenderer()->DrawOutlinedRect(alphaSelectedXPos - 1 - (7.5), alphaSelectedYPos - 1, SELECTION_ITEM_WIDTH + 2, SELECTION_ITEM_HEIGHT + 2, 1, Color(0, 0, 0, 0), alphaSelected ? Color(255, 255, 255) : Color(196, 196, 196), true);

	int defX = GetMenu()->m_center.x + (sX * GRID_ITEM_WIDTH) - ((GRID_COUNT * GRID_ITEM_WIDTH) / 2);
	float y = GetMenu()->m_center.y + (sY * GRID_ITEM_HEIGHT) - ((GRID_COUNT * GRID_ITEM_HEIGHT) / 2);
	GetRenderer()->DrawOutlinedRect(defX - 1 - (7.5), y - 1, GRID_ITEM_WIDTH + 2, GRID_ITEM_HEIGHT + 2, 1, HSVToRGB(hueSlider, sX * (1.0f / GRID_COUNT), sY * (1.0f / GRID_COUNT)), (sliderSelected || alphaSelected) ? Color(196, 196, 196) : Color(255, 255, 255), true);

	static bool downDisabled = false;
	static bool leftDisabled = false;
	static bool rightDisabled = false;
	static int downTimer = 0;
	static int leftTimer = 0;
	static int rightTimer = 0;

	int delayCount = Utils::IsInGame() ? (sliderSelected || alphaSelected) ? 5 : 50 : 25;

	static bool upDisabled = false;
	static int upTimer = 0;

	*m_changingColor = HSVToRGB(hueSlider, sX * (1.0f / GRID_COUNT), sY * (1.0f / GRID_COUNT), alphaSlider);

	float previewY = 1 + GetMenu()->m_center.y - ((GRID_COUNT * GRID_ITEM_HEIGHT) / 2) - 5 + (GRID_COUNT * GRID_ITEM_HEIGHT) + 30;
	GetRenderer()->DrawOutlinedRect(GetMenu()->m_center.x - 160 - 10 - (7.5), previewY, 359 + 15, 30, 0, *m_changingColor, m_gridColor, true); // bottom bar

	GetRenderer()->DrawString("\x0005 - Jump Left | Jump Right - \x0006", GetMenu()->m_center.x + 10, previewY + 70, GetRenderer()->Font.smallFont, 0.80f, Color(255, 255, 255), false, false, JustifyCenter);

	if (IsJustPressed(RIGHT_SHOULDER)) {
		if (!sliderSelected && !alphaSelected) {
			alphaSelected = true;
		}
		else if (alphaSelected) {
			sliderSelected = true;
			alphaSelected = false;
		} 
	}

	if (IsJustPressed(LEFT_SHOULDER)) {
		if (sliderSelected) {
			alphaSelected = true;
			sliderSelected = false;
		}
		else if (alphaSelected) {
			alphaSelected = false;
		}
	}

	//Scroll up
	if (IsJustPressed(DPAD_UP)) {
		if (!upDisabled) {
			if (alphaSelected) {
				alphaSlider -= 1;
				if (alphaSlider < 0) alphaSlider = 255;
			} else if (sliderSelected) {
				hueSlider -= 1;
				if (hueSlider < 0) hueSlider = 360;
			} else {
				if (sY > 0) sY--;
			}
		}
		upDisabled = false;
		upTimer = 0;
	}
		
	// Hold up
	if (IsDown(DPAD_UP)) {
		if (++upTimer > 20) {
			upDisabled = true;

			static int TIMER;
			if ((Utils::timeGetTime() - TIMER) > delayCount) {
				if (alphaSelected) {
					alphaSlider -= 1;
					if (alphaSlider < 0) alphaSlider = 255;
				} else if (sliderSelected) {
					hueSlider -= 1;
					if (hueSlider < 0) hueSlider = 360;
				} else {
					if (sY > 0) sY--;
				}
				TIMER = Utils::timeGetTime();
			}
		}
	}
		
	// Scroll down
	if (IsJustPressed(DPAD_DOWN)) {
		if (!downDisabled) {
			if (alphaSelected) {
				alphaSlider += 1;
				if (alphaSlider > 255) alphaSlider = 0;
			} else if (sliderSelected) {
				hueSlider += 1;
				if (hueSlider > 360) hueSlider = 0;
			} else {
				if (sY < GRID_COUNT) sY++;
			}
		}
		downDisabled = false;
		downTimer = 0;
	}

	// Hold down
	if (IsDown(DPAD_DOWN)) {
		if (++downTimer > 20) {
			downDisabled = true;

			static int TIMER;
			if ((Utils::timeGetTime() - TIMER) > delayCount) {
				if (alphaSelected) {
					alphaSlider += 1;
					if (alphaSlider > 255) alphaSlider = 0;
				} else if (sliderSelected) {
					hueSlider += 1;
					if (hueSlider > 360) hueSlider = 0;
				} else {
					if (sY < GRID_COUNT) sY++;
				}
				TIMER = Utils::timeGetTime();
			}
		}
	}

	//Scroll left
	if (IsJustPressed(DPAD_LEFT)) {
		if (!leftDisabled) {
			if (sliderSelected) {
				sliderSelected = false;
				alphaSelected = true;
			} else if (alphaSelected) {
				alphaSelected = false;
			} else 
				if (sX > 0) sX--;
		}
		leftDisabled = false;
		leftTimer = 0;
	}
	
	// Scroll right
	if (IsJustPressed(DPAD_RIGHT)) {
		if (!rightDisabled) {
			if (sX < GRID_COUNT) sX++;
			else {
				if (!alphaSelected && !sliderSelected) {
					alphaSelected = true;
				} else {
					sliderSelected = true;
					alphaSelected = false;
				}
			}
		}
		rightDisabled = false;
		rightTimer = 0;
	}
		
	// Hold left
	if (IsDown(DPAD_LEFT)) {
		if (++leftTimer > 20) {
			leftDisabled = true;
			static int TIMER;
			if ((Utils::timeGetTime() - TIMER) > delayCount) {
				if (sX > 0) sX--;
				else {
					if (sliderSelected) {
						alphaSelected = true;
						sliderSelected = false;
					}
					else {
						if (alphaSelected) {
							alphaSelected = false;
						}
					}
				}
				TIMER = Utils::timeGetTime();
			}
		}
	}
		
	// Hold right
	if (IsDown(DPAD_RIGHT)) {
		if (++rightTimer > 20) {
			rightDisabled = true;
			static int TIMER;
			if ((Utils::timeGetTime() - TIMER) > delayCount) {
				if (sX < GRID_COUNT) sX++;
				else {
					if (!alphaSelected && !sliderSelected) {
						alphaSelected = true;
					}
					else {
						sliderSelected = true;
						alphaSelected = false;
					}
				}
				TIMER = Utils::timeGetTime();
			}
		}
	}

	if (IsJustPressed(A)) {
		m_colorCallback();
		ColorOptionSaveConfig(m_classPtr);
		m_accepted = true;
		GetMenu()->m_isShowingAnotherWindow = false;
	}

	if (m_accepted) *m_changingColor = HSVToRGB(hueSlider, sX * (1.0f / GRID_COUNT), sY *(1.0f / GRID_COUNT), m_changingColor->m_color.a);
	else *m_changingColor = originalCol;
}

MenuInput* GetMenuInput() {
	return &g_menuInput;
}
