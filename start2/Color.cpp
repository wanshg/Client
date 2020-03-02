#include "Color.h"

#include "Mathf.h"

//#if !defined( __MINGW32__ ) || defined( BUILD_STATIC )
const Color Color::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::RED(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const Color Color::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
/*#else
Color Color::BLACK( 0.0f, 0.0f, 0.0f, 1.0f );
Color Color::WHITE( 1.0f, 1.0f, 1.0f, 1.0f );
Color Color::RED(   1.0f, 0.0f, 0.0f, 1.0f );
Color Color::GREEN( 0.0f, 1.0f, 0.0f, 1.0f );
Color Color::BLUE(  0.0f, 0.0f, 1.0f, 1.0f );
#endif*/

//default to RGBA order as OpenGL wants it
unsigned int Color32::s_uiShift[4] = { 0, 8, 16, 24 };
unsigned int Color32::s_uiComponent[4] = { 0, 1, 2, 3 };


Color Color::Lerp(Color from, Color to, float time)
{
	float tr = Mathf::Lerp(from.r, to.r, time);
	float tg = Mathf::Lerp(from.r, to.r, time);
	float tb = Mathf::Lerp(from.r, to.r, time);
	float ta = Mathf::Lerp(from.r, to.r, time);
	return Color(tr, tg, tb, ta);
}