/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <esignal/debug.hpp>
#include <esignal/Connection.hpp>

esignal::Connection::Connection():
  m_data(null),
  m_uid(0) {
	
}

esignal::Connection::Connection(ememory::SharedPtr<esignal::BaseInternal> _ref, size_t _id):
  m_data(_ref),
  m_uid(_id) {
	
}

esignal::Connection::Connection(esignal::Connection&& _obj):
  m_data(_obj.m_data),
  m_uid(_obj.m_uid) {
	_obj.m_uid = 0;
	_obj.m_data.reset();
}

esignal::Connection& esignal::Connection::operator=(esignal::Connection&& _obj) {
	disconnect();
	m_data = _obj.m_data;
	m_uid = _obj.m_uid;
	_obj.m_uid = 0;
	_obj.m_data.reset();
	return *this;
}

esignal::Connection::~Connection() {
	ememory::SharedPtr<esignal::BaseInternal> ref = m_data.lock();
	if (ref == null) {
		return;
	}
	ref->disconnect(m_uid);
	m_data.reset();
	m_uid = 0;
}

void esignal::Connection::disconnect() {
	ememory::SharedPtr<esignal::BaseInternal> ref = m_data.lock();
	if (ref == null) {
		return;
	}
	ref->disconnect(m_uid);
	m_data.reset();
	m_uid = 0;
}

bool esignal::Connection::isConnected() {
	return    m_data.useCount() != 0
	       && m_uid != 0;
}

