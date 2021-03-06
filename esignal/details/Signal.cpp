/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <esignal/details/Signal.hxx>
#include <etk/types.hpp>
#include <etk/math/Vector2D.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Quaternion.hpp>
#include <etk/math/Transform3D.hpp>
#include <etk/Color.hpp>
#include <etk/path/Path.hpp>
#include <etk/uri/Uri.hpp>

// void generic signal
ESIGNAL_DECLARE_SIGNAL();
// std generic signal
ESIGNAL_DECLARE_SIGNAL(bool);
ESIGNAL_DECLARE_SIGNAL(etk::String);
#if __CPP_VERSION__ >= 2011
	ESIGNAL_DECLARE_SIGNAL(etk::UString);
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
// etk generic vector 2D
ESIGNAL_DECLARE_SIGNAL(vec2);
ESIGNAL_DECLARE_SIGNAL(bvec2);
ESIGNAL_DECLARE_SIGNAL(ivec2);
ESIGNAL_DECLARE_SIGNAL(uivec2);
// etk generic vector 3D
ESIGNAL_DECLARE_SIGNAL(vec3);
ESIGNAL_DECLARE_SIGNAL(bvec3);
ESIGNAL_DECLARE_SIGNAL(ivec3);
ESIGNAL_DECLARE_SIGNAL(uivec3);
// etk generic 3D elements
ESIGNAL_DECLARE_SIGNAL(etk::Quaternion);
ESIGNAL_DECLARE_SIGNAL(etk::Transform3D);
// etk generic color
ESIGNAL_DECLARE_SIGNAL(etk::Color<unsigned char,4>);
ESIGNAL_DECLARE_SIGNAL(etk::Color<unsigned char,3>);
ESIGNAL_DECLARE_SIGNAL(etk::Color<float,4>);
ESIGNAL_DECLARE_SIGNAL(etk::Color<float,3>);

ESIGNAL_DECLARE_SIGNAL(etk::Path);
ESIGNAL_DECLARE_SIGNAL(etk::Uri);

#ifdef ESIGNAL_BUILD_WITH_ECHRONO
	#include <echrono/Clock.hpp>
	#include <echrono/Duration.hpp>
	#include <echrono/Steady.hpp>
	#include <echrono/Time.hpp>
	ESIGNAL_DECLARE_SIGNAL(echrono::Clock);
	ESIGNAL_DECLARE_SIGNAL(echrono::Duration);
	ESIGNAL_DECLARE_SIGNAL(echrono::Steady);
	ESIGNAL_DECLARE_SIGNAL(echrono::Time);
#endif
