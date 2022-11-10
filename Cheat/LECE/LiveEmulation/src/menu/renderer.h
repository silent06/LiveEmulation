#pragma once

enum eJustify {
	JustifyLeft,
	JustifyCenter,
	JustifyRight
};

class Renderer {
public:
	Material* m_whiteMaterial;
	struct Font {
		Font_s* bigDevFont;
		Font_s* smallDevFont;
		Font_s* consoleFont;
		Font_s* hudSmallFont;
		Font_s* hudBigFont;
		Font_s* objectiveFont;
		Font_s* extraBigFont;
		Font_s* normalFont;
		Font_s* boldFont;
		Font_s* smallFont;
		Font_s* bigFont;
	} Font;

	struct Colors {
		Color headerBorderColor;
		Color headerBackgroundColor;

		Color topRightBorderColor;
		Color topRightBackgroundColor;
		Color topRightTextColor;

		Color titleColor;

		Color subHeaderBorderColor;
		Color subHeaderBackgroundColor;
		Color subHeaderSelectorColor;
		Color subHeaderSelectorCreviceColor;
		Color subHeaderTextColor;

		Color backgroundBorderColor;
		Color backgroundColor;

		Color scrollBarBorderColor;
		Color scrollBarBackgroundColor;
		Color scrollBarColor;

		Color optionScrollBarColor;

		Color footerBorderColor;
		Color footerBackgroundColor;

		Color tooltipColor;

		Color sliderBoxOutlineColor;
	} UIColors;

	struct Rainbows {
		Rainbow m_ui;
	} Rainbows;

	void Init() {
#ifdef DEVELOPER_BUILD
		LOG(false, "[Renderer] Initializing renderer");
#endif
		
		/*Rainbows*/
		Rainbows.m_ui = Rainbow(); // init rainbow with default of blue

		/*Materials*/
		m_whiteMaterial = Native::UI::R_RegisterMaterial("white", 0);

		/*Fonts*/
		Font.bigDevFont = Native::UI::R_RegisterFont("fonts/bigDevFont", 0);
		Font.smallDevFont = Native::UI::R_RegisterFont("fonts/smallDevFont", 0);
		Font.consoleFont = Native::UI::R_RegisterFont("fonts/consoleFont", 0);
		Font.hudSmallFont = Native::UI::R_RegisterFont("fonts/hudsmallfont", 0);
		Font.hudBigFont = Native::UI::R_RegisterFont("fonts/hudbigfont", 0);
		Font.objectiveFont = Native::UI::R_RegisterFont("fonts/objectivefont", 0);
		Font.extraBigFont = Native::UI::R_RegisterFont("fonts/extrabigfont", 0);
		Font.normalFont = Native::UI::R_RegisterFont("fonts/normalfont", 0);
		Font.boldFont = Native::UI::R_RegisterFont("fonts/boldfont", 0);
		Font.smallFont = Native::UI::R_RegisterFont("fonts/smallfont", 0);
		Font.bigFont = Native::UI::R_RegisterFont("fonts/bigfont", 0);
		
		m_baseColor = Color(0x00cc99FF); // 0xE34F59FF

		m_optionHeight = 24.f;
		m_tooltipLines = 2;
		m_selectedFont = Font.normalFont;

		UIColors.sliderBoxOutlineColor = m_baseColor;

		UIColors.titleColor = Color(0xFFFFFFFF);

		UIColors.headerBackgroundColor = Color(0, 0, 0, 150);
		UIColors.headerBorderColor = Color(m_baseColor);

		UIColors.subHeaderBorderColor = Color(m_baseColor);
		UIColors.subHeaderBackgroundColor = Color(57, 57, 57, 150);
		UIColors.subHeaderSelectorColor = m_baseColor.WithOpacity(0xB4);
		UIColors.subHeaderSelectorCreviceColor = Color(m_baseColor);
		UIColors.subHeaderTextColor = Color(0xFFFFFFFF);

		UIColors.topRightBorderColor = Color(m_baseColor);
		UIColors.topRightBackgroundColor = Color(0, 0, 0, 150);
		UIColors.topRightTextColor = Color(0xFFFFFFFF);

		UIColors.backgroundBorderColor = Color(m_baseColor);
		UIColors.backgroundColor = Color(0, 0, 0, 150);

		UIColors.scrollBarBorderColor = Color(m_baseColor);
		UIColors.scrollBarBackgroundColor = Color(0, 0, 0, 100);
		UIColors.scrollBarColor = Color(0xFFFFFFFF);

		UIColors.optionScrollBarColor = m_baseColor.WithOpacity(0xB4);

		UIColors.footerBorderColor = Color(m_baseColor);
		UIColors.footerBackgroundColor = Color(0, 0, 0, 150);

		UIColors.tooltipColor = Color(0xFFFFFFFF);
	}

	float CalculateXValueWithAlign(float x, const char* text, Font_s* font, float scale, eJustify justify);
	string WordWrap(string txt, Font_s* font, float fontScale, int maxX, int* lines);
	int Clamp(int val, int min, int max);

	void DrawOutlinedRect(float x, float y, float w, float h, float thickness, Color boxColor, Color borderColor, bool sides = false);
	void DrawString(const char* text, float x, float y, Font_s* font, float scale, Color color, bool outlined, bool shadow, eJustify justify);
	void DrawShader(float x, float y, float width, float height, Color color, Material* material);
	void DrawRect(float x, float y, float width, float height, eJustify justify, Color color);
	void DrawTopRight(const char* text, int pos);
	void DrawLine(float X1, float Y1, float X2, float Y2, float width, Color color, Material* Shader);
	void DrawTitle(const char* title);

	void Render();
	void RenderHeader();
	void RenderSubHeader();
	void RenderBackground();
	void RenderOptionHighlight();
	void RenderFooter();

	void SetToolip(string name) {
		m_tooltip = name;
	}

	Font_s* m_selectedFont;
	string m_tooltip;
	int m_tooltipLines;
	float m_renderOptions;
	float m_optionHeight;
	Color m_baseColor;
	float m_bgPosition;
	float m_bgYCoordTemp;
	float m_scrollPosition;
	float m_scrollYCoordTemp;
	float m_descriptionBoxYCoordTemp;
	float m_descriptionBoxPosition;
};

Renderer* GetRenderer();