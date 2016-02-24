/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include <esignal/debug.h>
#include <esignal/Connection.h>

esignal::Connection::Connection():
  m_signalRefUnique(), m_uid(0) {
	
}

esignal::Connection::Connection(const esignal::LockSharedPtrRef<esignal::Base>& _ref, std::size_t _id):
  m_signalRefUnique(_ref),
  m_uid(_id) {
	
}

esignal::Connection::Connection(esignal::Connection&& _obj):
  m_signalRefUnique(_obj.m_signalRefUnique),
  m_uid(_obj.m_uid) {
	_obj.m_uid = 0;
}

esignal::Connection& esignal::Connection::operator=(esignal::Connection&& _obj) {
	disconnect();
	m_signalRefUnique = _obj.m_signalRefUnique;
	m_uid = _obj.m_uid;
	_obj.m_uid = 0;
	return *this;
}
esignal::Connection::~Connection() {
	m_signalRefUnique.disconnect(m_uid);
	m_uid = 0;
}

void esignal::Connection::disconnect() {
	m_signalRefUnique.disconnect(m_uid);
	m_uid = 0;
}

bool esignal::Connection::isConnected() {
	return m_signalRefUnique.isAlive();
}

