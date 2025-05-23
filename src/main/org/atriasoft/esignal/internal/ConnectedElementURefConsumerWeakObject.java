package org.atriasoft.esignal.internal;

import java.lang.ref.WeakReference;

/**
 * Connected element on a consumer to Manage event
 *
 * @param <T> Type Of the Signal
 * @param <U> Second type Of the Signal
 */
public class ConnectedElementURefConsumerWeakObject<T, U> implements ConnectedElementInterface<T, U> {
	protected final U consumer;
	protected final WeakReference<Object> linkedObject;
	
	public ConnectedElementURefConsumerWeakObject(final Object linkedObject, final U consumer) {
		this.consumer = consumer;
		this.linkedObject = new WeakReference<Object>(linkedObject);
	}
	@Override
	public boolean isObjectDependent() {
		return true;
	}
	
	@Override
	public U getConsumer2() {
		return this.consumer;
	}
	
	@Override
	public Object lockObjects() {
		return this.linkedObject.get();
	}
	
	@Override
	public Object getObject() {
		return this.linkedObject.get();
	}

	@Override
	public boolean isCompatibleWith(final Object elem) {
		Object out = this.consumer;
		if (out == elem) {
			return true;
		}
		Object obj = this.linkedObject.get();
		if (obj == elem) {
			return true;
		}
		return false;
	}
}
