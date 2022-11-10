#pragma once

struct RGBA { 
	int r;
	int g;
	int b;
	int a;

	RGBA() {
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	RGBA(int _r, int _g, int _b, int _a) { 
		r = _r; 
		g = _g;
		b = _b; 
		a = _a; 
	} 
};

struct HSV {
	float h;
	float s;
	float v;
};

class Color {
public:
	RGBA m_color;

	Color() {}

	Color(int r, int g, int b, int a = 255)
		: m_color(RGBA(r, g, b, a))
	{}

	Color(unsigned int hex) {
		m_color.r = (hex >> 24) & 0xFF;
		m_color.g = (hex >> 16) & 0xFF;
		m_color.b = (hex >> 8) & 0xFF;
		m_color.a = hex & 0xFF;
		if (m_color.a == 0)
			m_color.a = 255;
	}

	float* GetFloatPtr() {
		static float ptr[4];
		ptr[0] = float(m_color.r / 255.f);
		ptr[1] = float(m_color.g / 255.f);
		ptr[2] = float(m_color.b / 255.f);
		ptr[3] = float(m_color.a / 255.f);
		return ptr;
	}

	Color WithOpacity(int opacity) {
		static Color col;
		col = *this;
		col.m_color.a = opacity;
		return col;
	}


};

class Rainbow {
public:
	Rainbow() {
		m_pregameAccess = false;
		m_inc = 0;
	}

	Rainbow(bool pre) {
		m_pregameAccess = pre;
	}

	void Run() {
		if (m_enabled && (Utils::IsInGame() || m_pregameAccess)) {
			switch (m_inc) {
			case 0:
				m_color.r = 255;
				m_color.g = 0;
				m_color.b = 0;
				m_inc++;
				break;
			case 1:
				m_color.g++;
				if (m_color.g == 255)
					m_inc++;
				break;
			case 2:
				m_color.r--;
				if (m_color.r == 0)
					m_inc++;
				break;
			case 3:
				m_color.b++;
				if (m_color.b == 255)
					m_inc++;
				break;
			case 4:
				m_color.g--;
				if (m_color.g == 0)
					m_inc++;
				break;
			case 5:
				m_color.r++;
				if (m_color.r == 255)
					m_inc++;
				break;
			case 6:
				m_color.b--;
				if (m_color.b == 0)
					m_inc = 0;
				break;
			}
			Update();
		}
	}

	void Add(RGBA* color) {
		m_defaults.push_back(make_pair(*color, color));
		m_colors.push_back(color);
	}

	void Remove(RGBA* color) {
		auto exists = find(m_colors.begin(), m_colors.end(), color);
		if (exists != m_colors.end()) {
			color->r = GetDefault(color).r;
			color->g = GetDefault(color).g;
			color->b = GetDefault(color).b;

			m_colors.erase(remove(m_colors.begin(), m_colors.end(), color), m_colors.end());
			
			for (int i = 0; i < m_defaults.size(); i++) {
				if (m_defaults.at(i).second == color) { // compare the ptrs
					m_defaults.erase(m_defaults.begin() + i);
					break;
				}
			}
		}
	}

	void Stop() {
		m_enabled = false;

		for (int i = 0; i < m_colors.size(); i++) {
			m_colors[i]->r = GetDefault(m_colors[i]).r;
			m_colors[i]->g = GetDefault(m_colors[i]).g;
			m_colors[i]->b = GetDefault(m_colors[i]).b;
		}
	}

	void Start() {
		m_enabled = true;
	}

	bool Exists(RGBA* color) {
		auto exists = find(m_colors.begin(), m_colors.end(), color);
		return exists != m_colors.end();
	}

private:
	bool m_pregameAccess;
	RGBA m_color;
	vector<RGBA*> m_colors;
	vector<pair<RGBA, RGBA*>> m_defaults;
	bool m_enabled;
	int m_inc;

	RGBA GetDefault(RGBA* ptr) {
		for (int i = 0; i < m_defaults.size(); i++) {
			if (m_defaults.at(i).second == ptr) { // compare the ptrs
				return m_defaults.at(i).first;
			}
		}

		return RGBA();
	}

	void Update() {
		for (int i = 0; i < m_colors.size(); i++) {
			m_colors[i]->r = m_color.r;
			m_colors[i]->g = m_color.g;
			m_colors[i]->b = m_color.b;
		}
	}
};