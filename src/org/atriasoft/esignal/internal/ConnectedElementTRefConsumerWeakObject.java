package org.atriasoft.esignal.internal;

import java.lang.ref.WeakReference;

/**
 * Connected element with a dependency of an other object to auto remove the signals
 *
 * @param <T> Type Of the Signal
 */
public class ConnectedElementTRefConsumerWeakObject<T, U> extends ConnectedElementTRefConsumer<T, U> {
	protected final WeakReference<Object> linkedObject;
	
	public ConnectedElementTRefConsumerWeakObject(final Object linkedObject, final T consumer) {
		super(consumer);
		this.linkedObject = new WeakReference<Object>(linkedObject);
	}

	@Override
	public boolean isObjectDependent() {
		return true;
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
		if (super.isCompatibleWith(elem)) {
			return true;
		}
		Object obj = this.linkedObject.get();
		if (obj == elem) {
			return true;
		}
		return false;
	}

}