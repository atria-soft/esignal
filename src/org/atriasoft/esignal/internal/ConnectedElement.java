package org.atriasoft.esignal.internal;

import java.lang.ref.WeakReference;

/**
 * Connected element on a consumer to Manage event
 *
 * @param <T> Type Of the Signal
 */
public class ConnectedElement<T> {
	protected final WeakReference<T> consumer;
	
	public ConnectedElement(final T consumer) {
		this.consumer = new WeakReference<T>(consumer);
	}
	
	public T getConsumer() {
		return this.consumer.get();
	}

	public boolean isCompatibleWith(final Object elem) {
		Object out = this.consumer.get();
		if (out == elem) {
			return true;
		}
		return false;
	}

	public void disconnect() {
		
	}
}