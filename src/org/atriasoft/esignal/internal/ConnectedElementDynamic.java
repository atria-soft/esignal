package org.atriasoft.esignal.internal;

import java.lang.ref.WeakReference;

import org.atriasoft.esignal.Connection;

/**
 * Connected element with a dependency of an other object to auto remove the signals
 *
 * @param <T> Type Of the Signal
 */
public class ConnectedElementDynamic<T> extends ConnectedElement<T> {
	protected final WeakReference<Object> linkedObject;
	
	public ConnectedElementDynamic(final Object linkedObject, final T consumer) {
		super(consumer);
		this.linkedObject = new WeakReference<Object>(linkedObject);
	}
	
	@Override
	public T getConsumer() {
		if (this.linkedObject.get() == null) {
			return null;
		}
		return this.consumer.get();
	}
	
	@Override
	public boolean isCompatibleWith(final Object elem) {
		if (super.isCompatibleWith(elem)) {
			return true;
		}
		Object obj = this.linkedObject.get();
		if (obj == elem) {
			return true;
		}
		return false;
	}

	@Override
	public void disconnect() {
		Object obj = this.linkedObject.get();
		if (obj == null) {
			return;
		}
		if (obj instanceof Connection tmp) {
			tmp.connectionIsRemovedBySignal();
		}
	}
}