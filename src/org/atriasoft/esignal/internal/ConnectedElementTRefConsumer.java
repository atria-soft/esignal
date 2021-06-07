package org.atriasoft.esignal.internal;

/**
 * Connected element on a consumer to Manage event
 *
 * @param <T> Type Of the Signal
 */
public class ConnectedElementTRefConsumer<T, U> implements ConnectedElementInterface<T, U> {
	protected final T consumer;
	
	public ConnectedElementTRefConsumer(final T consumer) {
		this.consumer = consumer;
	}
	
	@Override
	public T getConsumer() {
		return this.consumer;
	}

	@Override
	public boolean isCompatibleWith(final Object elem) {
		if (this.consumer == elem) {
			return true;
		}
		return false;
	}

	@Override
	public void disconnect() {
		
	}
}
