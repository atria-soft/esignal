package org.atriasoft.esignal.internal;

import java.lang.ref.WeakReference;

/**
 * Connected element on a consumer to Manage event
 *
 * @param <T> Type Of the Signal
 */
public class ConnectedElementTWeakConsumer<T, U> implements ConnectedElementInterface<T, U> {
	protected final WeakReference<T> consumer;
	
	public ConnectedElementTWeakConsumer(final T consumer) {
		this.consumer = new WeakReference<T>(consumer);
	}
	
	@Override
	public T getConsumer() {
		return this.consumer.get();
	}

	@Override
	public boolean isCompatibleWith(final Object elem) {
		Object out = this.consumer.get();
		if (out == elem) {
			return true;
		}
		return false;
	}

	@Override
	public void disconnect() {
		
	}
}
