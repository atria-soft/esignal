package org.atriasoft.esignal.internal;

/**
 * Connected element on a consumer to Manage event
 *
 * @param <T> Type Of the Signal
 */
public interface ConnectedElementInterface<T, U> {
	default T getConsumer() { return null; }
	
	default boolean isObjectDependent() { return false; }
	default U getConsumer2() { return null; }
	/**
	 * Lock an Object that keep a reference on all object that need to be alive while the call is done
	 * @return An unusable Object (just keep a reference on it.
	 */
	default Object lockObjects() { return null; }
	default Object getObject() { return null; }

	boolean isCompatibleWith(final Object elem);

	/**
	 * Permit to remove connection on the associated signal.
	 */
	default void disconnect() {}
}