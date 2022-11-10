#include "stdafx.h"
static Renderer g_renderer;

void Renderer::DrawString(const char* text, float x, float y, Font_s* font, float scale, Color color, bool outlined, bool shadow, eJustify justify) {
	float defaultX = CalculateXValueWithAlign(x, text, font, scale, justify);

	Native::UI::R_AddCmdDrawText(text, 0x7FFFFFFF, font, defaultX, y, scale, scale, 0.f, color.GetFloatPtr(), 0);

	if (outlined) {
		Native::UI::R_AddCmdDrawText(text, 0x7FFFFFFF, font, defaultX, y, scale + 0.02f, scale + 0.02f, 0.f, Color(0, 0, 0, 255).GetFloatPtr(), 0);
	}

	if (shadow) {
		Native::UI::R_AddCmdDrawText(text, 0x7FFFFFFF, font, defaultX + 3.f, y + 3.f, scale, scale, 0.f, Color(0, 0, 0, 255).GetFloatPtr(), 0);
	}
}

void Renderer::DrawOutlinedRect(float x, float y, float w, float h, float thickness, Color boxColor, Color borderColor, bool sides) {
	Native::UI::R_AddCmdDrawStretchPic(x, y, w, h, 1.f, 1.f, 1.f, 1.f, boxColor.GetFloatPtr(), m_whiteMaterial); // bg 
	Native::UI::R_AddCmdDrawStretchPic(x, y - thickness, w, thickness, 1, 1, 1, 1, borderColor.GetFloatPtr(), m_whiteMaterial); // top bar
	Native::UI::R_AddCmdDrawStretchPic(x, y + h, w, thickness, 1, 1, 1, 1, borderColor.GetFloatPtr(), m_whiteMaterial); // bottom bar

	if (sides) {
		Native::UI::R_AddCmdDrawStretchPic(x - thickness, y - thickness, thickness, h + (thickness * 2), 1, 1, 1, 1, borderColor.GetFloatPtr(), m_whiteMaterial); // left bar
		Native::UI::R_AddCmdDrawStretchPic(x + w, y - thickness, thickness, h + (thickness * 2), 1, 1, 1, 1, borderColor.GetFloatPtr(), m_whiteMaterial); // right bar
	}
}

void Renderer::DrawShader(float x, float y, float width, float height, Color color, Material* material) {
	Native::UI::R_AddCmdDrawStretchPic(x, y, width, height, 1, 1, 1, 1, color.GetFloatPtr(), material);
}

void Renderer::DrawLine(float X1, float Y1, float X2, float Y2, float width, Color color, Material* Shader) {
	float X, Y, Angle, L1, L2, H1;
	H1 = Y2 - Y1;
	L1 = X2 - X1;
	L2 = sqrt(L1 * L1 + H1 * H1);
	X = X1 + ((L1 - L2) / 2);
	Y = Y1 + (H1 / 2);
	Angle = (float)atan(H1 / L1) * (180.f / 3.14159265358979323846f);
	Native::UI::CG_DrawRotatedPicPhysical(0x827322C0, X, Y, L2, width, Angle, color.GetFloatPtr(), Shader);
}

void Renderer::DrawRect(float x, float y, float width, float height, eJustify justify, Color color) {
	if (justify == JustifyLeft) {
		DrawShader(x, y, width, height, color, m_whiteMaterial);
	}

	if (justify == JustifyCenter) {
		DrawShader(x - (width / 2), y, width, height, color, m_whiteMaterial);
	}

	if (justify == JustifyRight) {
		DrawShader(x - width, y, width, height, color, m_whiteMaterial);
	}
}

void Renderer::DrawTopRight(const char* text, int pos) {
	int textWidth = Native::UI::R_TextWidth(text, m_selectedFont);
	DrawOutlinedRect(1260.f - (textWidth * 0.53f), (pos * 29.f), (textWidth * 0.53f) + 10.f, 22.f, 2, UIColors.topRightBackgroundColor, UIColors.topRightBorderColor, true);
	DrawString(text, CalculateXValueWithAlign(1265.f, text, m_selectedFont, 0.53f, JustifyLeft), 20.f + (pos * 29.f), m_selectedFont, 0.53f, UIColors.topRightTextColor, false, false, JustifyRight);
}

float Renderer::CalculateXValueWithAlign(float x, const char* text, Font_s* font, float scale, eJustify justify) {
	if (justify == JustifyLeft)
		return x;
	if (justify == JustifyCenter)
		return x - (Native::UI::R_TextWidth(text, font) / 2 * scale);
	if (justify == JustifyRight)
		return x - (Native::UI::R_TextWidth(text, font) * scale);

	return 0;
}

string Renderer::WordWrap(string text, Font_s* font, float fontScale, int maxX, int* lines) {
	bool first = false;

	*lines = 2;

	vector<int> newLines(1);

	for (size_t i = 0; i < text.length(); i++) {
		string newstr = text.substr(0, i);
		int pos = newstr.find_last_of("\n");
		if (pos > 1) {
			auto vit = find(newLines.begin(), newLines.end(), pos);
			if (vit == newLines.end()) {
				newLines.push_back(pos);
				if (first)
					*lines = *lines + 1;
				first = true;
			}
		}
	}

	int textWidth = (int)((float)Native::UI::R_TextWidth(text.c_str(), font) * fontScale);
	if (textWidth >= maxX) {
		for (size_t i = 0; i < text.length(); i++) {
			string newstr = text.substr(0, i);
			int twidth = (int)((float)Native::UI::R_TextWidth(newstr.c_str(), font) * fontScale);
			if (twidth >= maxX && newstr.find_last_of("\n") == newstr.npos) {
				int pos = newstr.find_last_of(" ");
				if (pos != newstr.npos) {
					text[pos] = '\n';
					if (first)
						*lines = *lines + 1;
					first = true;
				}
			}
		}
	}

	return text;
}

int Renderer::Clamp(int val, int min, int max) {
	return val < min ? min : val > max ? max : val;
}

void Renderer::Render() {
	m_renderOptions = (float)(GetSubmenuHandler()->GetTotalOptions() > GetMenu()->m_maxOptions ? GetMenu()->m_maxOptions : GetSubmenuHandler()->GetTotalOptions());

	m_bgYCoordTemp = m_renderOptions * m_optionHeight;
	Utils::Ease(m_bgPosition, m_bgYCoordTemp, 4.f);

	int Y_coord = Clamp(GetMenu()->m_currentOption - GetMenu()->m_scrollOffset > GetMenu()->m_maxOptions ? GetMenu()->m_maxOptions : GetMenu()->m_currentOption - GetMenu()->m_scrollOffset, 0, GetMenu()->m_maxOptions);
	m_scrollYCoordTemp = 177 + (Y_coord)* m_optionHeight;
	Utils::Ease(m_scrollPosition, m_scrollYCoordTemp, 4.f);

	m_descriptionBoxYCoordTemp = 178 + m_bgYCoordTemp;
	Utils::Ease(m_descriptionBoxPosition, m_descriptionBoxYCoordTemp, 4.f);

	RenderHeader();
	RenderSubHeader();
	RenderBackground();
	RenderOptionHighlight();
	RenderFooter();
}

void Renderer::DrawTitle(const char* title) {
	DrawString(title, GetMenu()->m_center.x, GetMenu()->m_center.y - 218 - 4, Font.smallFont, 1.0f, UIColors.titleColor, false, false, JustifyCenter);
}

void Renderer::RenderHeader() {
	float width = 425; // 415
	DrawOutlinedRect(GetMenu()->m_center.x - (width / 2), 102, width, 48, 1, UIColors.headerBackgroundColor, UIColors.headerBorderColor, true);
}

void Renderer::RenderSubHeader() {
	float width = 386;
	DrawOutlinedRect(GetMenu()->m_center.x - (width / 2), GetMenu()->m_center.y - 209, width, 25, 1, UIColors.subHeaderBackgroundColor, UIColors.subHeaderBorderColor, true);

	float tabSpace = width / 5; // 5 tabs

	DrawRect(GetMenu()->m_center.x - (width / 2) + (tabSpace * GetMenu()->m_selectedTab), 151, tabSpace, 25, JustifyLeft, UIColors.subHeaderSelectorColor);
	DrawRect(GetMenu()->m_center.x - (width / 2) + (tabSpace * GetMenu()->m_selectedTab), 172, tabSpace, 4, JustifyLeft, UIColors.subHeaderSelectorCreviceColor);

	DrawString("Main", CalculateXValueWithAlign(408 + (tabSpace * 1), "Main", m_selectedFont, 0.55f, JustifyCenter), 172, m_selectedFont, 0.55f, UIColors.subHeaderTextColor, false, false, JustifyLeft);
	DrawString("Aimbot", CalculateXValueWithAlign(408 + (tabSpace * 2), "Aimbot", m_selectedFont, 0.55f, JustifyCenter), 172, m_selectedFont, 0.55f, UIColors.subHeaderTextColor, false, false, JustifyLeft);
	DrawString("Visuals", CalculateXValueWithAlign(408 + (tabSpace * 3), "Visuals", m_selectedFont, 0.55f, JustifyCenter), 172, m_selectedFont, 0.55f, UIColors.subHeaderTextColor, false, false, JustifyLeft);
	DrawString("Settings", CalculateXValueWithAlign(408 + (tabSpace * 4), "Settings", m_selectedFont, 0.55f, JustifyCenter), 172, m_selectedFont, 0.55f, UIColors.subHeaderTextColor, false, false, JustifyLeft);
	DrawString("Players", CalculateXValueWithAlign(408 + (tabSpace * 5), "Players", m_selectedFont, 0.55f, JustifyCenter), 172, m_selectedFont, 0.55f, UIColors.subHeaderTextColor, false, false, JustifyLeft);
}

void Renderer::RenderBackground() {
	float staticY = 155;

	float width = 386;
	DrawOutlinedRect(GetMenu()->m_center.x - (width / 2), 177, width, m_bgPosition, 1, UIColors.backgroundColor, UIColors.backgroundBorderColor, false);

	//float scrollBarPos = (GetMenu()->m_center.x + (width / 2)) + 5;
	//DrawOutlinedRect(scrollBarPos, staticY, 8, m_bgPosition + 18, 0.75, UIColors.scrollBarBackgroundColor, UIColors.scrollBarBorderColor, true);

	//float sliderPosition = (((float)(GetSubmenuHandler()->m_currentOptionsWithoutBreaks) / ((float)GetSubmenuHandler()->m_totalOptionsWithoutBreaks - 1)) * ((m_bgPosition + 18) - m_optionHeight)) + m_optionHeight / 2;
	//if (GetSubmenuHandler()->m_totalOptions < 2)sliderPosition = (m_bgPosition + 18) - m_optionHeight + m_optionHeight / 2;

	//static float easedSliderPosition = sliderPosition;
	//Utils::Ease(easedSliderPosition, sliderPosition, 2);

	//DrawRect(scrollBarPos, staticY + easedSliderPosition - 12, 8, m_optionHeight, JustifyLeft, UIColors.scrollBarColor);
}

void Renderer::RenderOptionHighlight() {
	float width = 386;
	DrawRect(GetMenu()->m_center.x - (width / 2), m_scrollPosition, width, 25, JustifyLeft, UIColors.optionScrollBarColor);
}

void Renderer::RenderFooter() {
	float width = 425;
	if (m_tooltip.length() == 0) {
		DrawOutlinedRect(GetMenu()->m_center.x - (width / 2), m_descriptionBoxPosition, width, 2 * m_optionHeight, 1, UIColors.footerBackgroundColor, UIColors.footerBorderColor, true);
	} else {
		string tip = WordWrap(m_tooltip, m_selectedFont, 0.58f, 380, &m_tooltipLines);
		DrawOutlinedRect(GetMenu()->m_center.x - (width / 2), m_descriptionBoxPosition, width, m_tooltipLines * m_optionHeight, 1, UIColors.footerBackgroundColor, UIColors.footerBorderColor, true);
		DrawString(tip.c_str(), GetMenu()->m_center.x - (width / 2) + 5, 200 + m_bgYCoordTemp, m_selectedFont, 0.58f, UIColors.tooltipColor, false, false, JustifyLeft);
	}
}

Renderer* GetRenderer() {
	return &g_renderer;
}