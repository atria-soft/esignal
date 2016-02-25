/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esignal/debug.h>
#include <esignal/details/LockSharedPtrRef.hxx>
#include <esignal/Base.h>

// only one really needed ...
template class esignal::LockSharedPtrRef<esignal::Base>;

