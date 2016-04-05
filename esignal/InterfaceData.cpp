/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <memory>
#include <esignal/debug.h>
#include <esignal/InterfaceData.h>
#include <esignal/Base.h>

esignal::InterfaceData::InterfaceData() {
	
}

esignal::InterfaceData::~InterfaceData() {
	m_list.clear();
}

// note this pointer is not allocated and not free at the end of the class
void esignal::InterfaceData::add(esignal::Base* _pointerOnSignal) {
	if (_pointerOnSignal == nullptr) {
		ESIGNAL_ERROR("Try to link a nullptr parameters");
		return;
	}
	m_list.push_back(_pointerOnSignal);
}

void esignal::InterfaceData::remove(esignal::Base* _pointerOnSignal) {
	auto it = m_list.begin();
	while (it != m_list.end()) {
		if (    *it == nullptr
		     || *it == _pointerOnSignal) {
			it = m_list.erase(it);
		} else {
			++it;
		}
	}
}

std::vector<std::string> esignal::InterfaceData::getAll() const {
	std::vector<std::string> out;
	for (auto &it : m_list) {
		if(it != nullptr) {
			out.push_back(it->getName());
		}
	}
	return out;
}

void esignal::InterfaceData::disconnect(const std::shared_ptr<void>& _object) {
	if (_object == nullptr) {
		ESIGNAL_ERROR("Input ERROR nullptr pointer Object ...");
		return;
	}
	for(auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		it->disconnectShared(_object);
	}
}


