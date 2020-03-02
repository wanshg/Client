#ifndef _COLOR_H_
#define _COLOR_H_

class Color
{
public:
	float                           r, g, b, a;
	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	Color(unsigned int uiColor) : r(float(uiColor & 0xFF) * 0.0039215686f), g(float((uiColor >> 8) & 0xFF) * 0.0039215686f), b(float((uiColor >> 16) & 0xFF) * 0.0039215686f), a(float((uiColor >> 24) & 0xFF) * 0.0039215686f) {}
	Color(float fR, float fG, float fB, float fA = 1.0f) : r(fR), g(fG), b(fB), a(fA) {}

	static Color					Lerp(Color from, Color to, float time);
	inline void                     Set(float fR, float fG, float fB, float fA = 1.0f) { r = fR; g = fG; b = fB; a = fA; }

	inline void                     Set(unsigned int uiColor) { r = float(uiColor & 0xFF) * 0.0039215686f; g = float((uiColor >> 8) & 0xFF) * 0.0039215686f; b = float((uiColor >> 16) & 0xFF) * 0.0039215686f; a = float((uiColor >> 24) & 0xFF) * 0.0039215686f; }

	//		inline void                     Set(const Color &rkColor) { fmemcpy(&r, &rkColor.r, sizeof(float)* 4); }

	inline                          operator float* () { return &r; }

	inline                          operator const float* () const { return &r; }

	inline Color					operator*(const Color& rhs) const { return Color(r*rhs.r, g*rhs.g, b*rhs.b, a*rhs.a); };
	inline Color&					operator*=(const Color& rhs) { r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a; return *this; }

	inline float                   &operator [] (int iComponent) { return ((float*)&r)[iComponent]; }

	inline const float             &operator [] (int iComponent) const { return ((const float*)&r)[iComponent]; }

	//		inline Color                   &operator = (const Color &rkColor) { fmemcpy(&r, &rkColor.r, sizeof(float)* 4); return(*this); }

	inline bool                     operator == (const Color &rkColor) const { return((r == rkColor.r) && (g == rkColor.g) && (b == rkColor.b) && (a == rkColor.a)); }

	inline bool                     operator != (const Color &rkColor) const { return(!(*this == rkColor)); }

	static const Color    BLACK;

	static const Color    WHITE;

	static const Color    RED;

	static const Color    GREEN;

	static const Color    BLUE;
#if 0

	static Color      BLACK;

	static Color      WHITE;

	static Color      RED;

	static Color      GREEN;

	static Color      BLUE;
#endif
};

class Color32
{
private:

	union Color32Union
	{
		unsigned int                  m_uiValue;
		unsigned char                 m_aucComponent[4];
	}                               m_uiColor;


public:

	static unsigned int             s_uiShift[4];

	static unsigned int             s_uiComponent[4];


	Color32() { m_uiColor.m_uiValue = 0xFF << s_uiShift[3]; }

	Color32(unsigned char ucR, unsigned char ucG, unsigned char ucB, unsigned char ucA = 0xFF) { m_uiColor.m_uiValue = (ucR << s_uiShift[0]) | (ucG << s_uiShift[1]) | (ucB << s_uiShift[2]) | (ucA << s_uiShift[3]); }

	Color32(unsigned char *pucComponents) { m_uiColor.m_uiValue = (pucComponents[0] << s_uiShift[0]) | (pucComponents[1] << s_uiShift[1]) | (pucComponents[2] << s_uiShift[2]) | (pucComponents[3] << s_uiShift[3]); }

	Color32(const Color &rkColor) { m_uiColor.m_uiValue = ((int(rkColor.r * 255.0f) & 0xFF) << s_uiShift[0]) | ((int(rkColor.g * 255.0f) & 0xFF) << s_uiShift[1]) | ((int(rkColor.b * 255.0f) & 0xFF) << s_uiShift[2]) | ((int(rkColor.a * 255.0f) & 0xFF) << s_uiShift[3]); }

	Color32(float *pfComponents) { m_uiColor.m_uiValue = ((int(pfComponents[0] * 255.0f) & 0xFF) << s_uiShift[0]) | ((int(pfComponents[1] * 255.0f) & 0xFF) << s_uiShift[1]) | ((int(pfComponents[2] * 255.0f) & 0xFF) << s_uiShift[2]) | ((int(pfComponents[3] * 255.0f) & 0xFF) << s_uiShift[3]); }

	Color32(float fR, float fG, float fB, float fA = 1.0f) { m_uiColor.m_uiValue = ((int(fR * 255.0f) & 0xFF) << s_uiShift[0]) | ((int(fG * 255.0f) & 0xFF) << s_uiShift[1]) | ((int(fB * 255.0f) & 0xFF) << s_uiShift[2]) | ((int(fA * 255.0f) & 0xFF) << s_uiShift[3]); }

	inline void                     Set(unsigned char ucR, unsigned char ucG, unsigned char ucB, unsigned char ucA = 0xFF) { m_uiColor.m_uiValue = (ucR << s_uiShift[0]) | (ucG << s_uiShift[1]) | (ucB << s_uiShift[2]) | (ucA << s_uiShift[3]); }

	inline void                     Set(unsigned char *pucComponents) { m_uiColor.m_uiValue = (pucComponents[0] << s_uiShift[0]) | (pucComponents[1] << s_uiShift[1]) | (pucComponents[2] << s_uiShift[2]) | (pucComponents[3] << s_uiShift[3]); }

	inline void                     Set(const Color &rkColor) { m_uiColor.m_uiValue = ((int(rkColor.r * 255.0f) & 0xFF) | ((int(rkColor.g * 255.0f) & 0xFF) << 8) | ((int(rkColor.b * 255.0f) & 0xFF) << 16) | ((int(rkColor.a * 255.0f) & 0xFF) << 24)); }

	inline void                     Set(float fR, float fG, float fB, float fA = 1.0f) { m_uiColor.m_uiValue = ((int(fR * 255.0f) & 0xFF) << s_uiShift[0]) | ((int(fG * 255.0f) & 0xFF) << s_uiShift[1]) | ((int(fB * 255.0f) & 0xFF) << s_uiShift[2]) | ((int(fA * 255.0f) & 0xFF) << s_uiShift[3]); }

	operator const unsigned int& () const { return m_uiColor.m_uiValue; }

	operator unsigned char* () { return m_uiColor.m_aucComponent; }

	unsigned char                  &operator [] (int iComponent) { return m_uiColor.m_aucComponent[s_uiComponent[iComponent]]; }

	const unsigned char            &operator [] (int iComponent) const { return m_uiColor.m_aucComponent[s_uiComponent[iComponent]]; }

	Color32                        &operator = (const Color32 &rkColor) { m_uiColor.m_uiValue = rkColor.m_uiColor.m_uiValue; return(*this); }

	Color32                        &operator = (const Color &rkColor) { Set(rkColor.r, rkColor.g, rkColor.b, rkColor.a); return(*this); }

	Color32                        &operator = (unsigned int uiColor) { Set((unsigned char)(uiColor & 0xFF), (unsigned char)((uiColor >> 8) & 0xFF), (unsigned char)((uiColor >> 16) & 0xFF), (unsigned char)((uiColor >> 24) & 0xFF)); return(*this); }
};

#endif