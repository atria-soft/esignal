/**
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

size_t esignal::s_uid = 0;

#undef __class__
#define __class__ "Signal<void>"
// void generic signal
template class esignal::Signal<>;
// std generic signal
template class esignal::Signal<bool>;
template class esignal::Signal<std::string>;
#if __CPP_VERSION__ >= 2011
	template class esignal::Signal<std::u32string>;
#endif

template class esignal::Signal<int64_t>;
template class esignal::Signal<int32_t>;
template class esignal::Signal<int16_t>;
template class esignal::Signal<int8_t>;

template class esignal::Signal<uint64_t>;
template class esignal::Signal<uint32_t>;
template class esignal::Signal<uint16_t>;
template class esignal::Signal<uint8_t>;

template class esignal::Signal<float>;
template class esignal::Signal<double>;
// etk generic vetor 2D
template class esignal::Signal<vec2>;
template class esignal::Signal<bvec2>;
template class esignal::Signal<ivec2>;
template class esignal::Signal<uivec2>;
// etk generic vetor 3D
template class esignal::Signal<vec3>;
template class esignal::Signal<bvec3>;
template class esignal::Signal<ivec3>;
template class esignal::Signal<uivec3>;
// etk generic color
template class esignal::Signal<etk::Color<unsigned char,4>>;
template class esignal::Signal<etk::Color<unsigned char,3>>;
template class esignal::Signal<etk::Color<float,4>>;
template class esignal::Signal<etk::Color<float,3>>;
