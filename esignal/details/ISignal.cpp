/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include <esignal/details/ISignal.hxx>
#include <etk/types.h>
#include <etk/math/Vector2D.h>
#include <etk/math/Vector3D.h>
#include <etk/Color.h>

#undef __class__
#define __class__ "ISignal<void>"
// void generic signal
template class esignal::ISignal<>;
// std generic signal
template class esignal::ISignal<bool>;
template class esignal::ISignal<std::string>;
#if __CPP_VERSION__ >= 2011
	template class esignal::ISignal<std::u32string>;
#endif

template class esignal::ISignal<int64_t>;
template class esignal::ISignal<int32_t>;
template class esignal::ISignal<int16_t>;
template class esignal::ISignal<int8_t>;

template class esignal::ISignal<uint64_t>;
template class esignal::ISignal<uint32_t>;
template class esignal::ISignal<uint16_t>;
template class esignal::ISignal<uint8_t>;

template class esignal::ISignal<float>;
template class esignal::ISignal<double>;
// etk generic vetor 2D
template class esignal::ISignal<vec2>;
template class esignal::ISignal<bvec2>;
template class esignal::ISignal<ivec2>;
template class esignal::ISignal<uivec2>;
// etk generic vetor 3D
template class esignal::ISignal<vec3>;
template class esignal::ISignal<bvec3>;
template class esignal::ISignal<ivec3>;
template class esignal::ISignal<uivec3>;
// etk generic color
template class esignal::ISignal<etk::Color<unsigned char,4>>;
template class esignal::ISignal<etk::Color<unsigned char,3>>;
template class esignal::ISignal<etk::Color<float,4>>;
template class esignal::ISignal<etk::Color<float,3>>;
