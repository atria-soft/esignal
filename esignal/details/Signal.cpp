/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include <esignal/details/Signal.hxx>
#include <etk/types.h>
#include <etk/math/Vector2D.h>
#include <etk/math/Vector3D.h>
#include <etk/Color.h>

// void generic signal
ESIGNAL_DECLARE_SIGNAL();
// std generic signal
ESIGNAL_DECLARE_SIGNAL(bool);
ESIGNAL_DECLARE_SIGNAL(std::string);
#if __CPP_VERSION__ >= 2011
	ESIGNAL_DECLARE_SIGNAL(std::u32string);
#endif

ESIGNAL_DECLARE_SIGNAL(int64_t);
ESIGNAL_DECLARE_SIGNAL(int32_t);
ESIGNAL_DECLARE_SIGNAL(int16_t);
ESIGNAL_DECLARE_SIGNAL(int8_t);

ESIGNAL_DECLARE_SIGNAL(uint64_t);
ESIGNAL_DECLARE_SIGNAL(uint32_t);
ESIGNAL_DECLARE_SIGNAL(uint16_t);
ESIGNAL_DECLARE_SIGNAL(uint8_t);

ESIGNAL_DECLARE_SIGNAL(float);
ESIGNAL_DECLARE_SIGNAL(double);
// etk generic vetor 2D
ESIGNAL_DECLARE_SIGNAL(vec2);
ESIGNAL_DECLARE_SIGNAL(bvec2);
ESIGNAL_DECLARE_SIGNAL(ivec2);
ESIGNAL_DECLARE_SIGNAL(uivec2);
// etk generic vetor 3D
ESIGNAL_DECLARE_SIGNAL(vec3);
ESIGNAL_DECLARE_SIGNAL(bvec3);
ESIGNAL_DECLARE_SIGNAL(ivec3);
ESIGNAL_DECLARE_SIGNAL(uivec3);
// etk generic color
ESIGNAL_DECLARE_SIGNAL(etk::Color<unsigned char,4>);
ESIGNAL_DECLARE_SIGNAL(etk::Color<unsigned char,3>);
ESIGNAL_DECLARE_SIGNAL(etk::Color<float,4>);
ESIGNAL_DECLARE_SIGNAL(etk::Color<float,3>);

