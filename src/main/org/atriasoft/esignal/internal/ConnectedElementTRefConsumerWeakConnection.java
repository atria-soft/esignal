package org.atriasoft.esignal.internal;

import java.lang.ref.WeakReference;

import org.atriasoft.esignal.Connection;

/**
 * Connected element with a dependency of an other object to auto remove the signals
 *
 * @param <T> Type Of the Signal
 */
public class ConnectedElementTRefConsumerWeakConnection<T, U> extends ConnectedElementTRefConsumer<T, U> {
	protected final WeakReference<Connection> linkedConnection;
	
	public ConnectedElementTRefConsumerWeakConnection(final Connection linkedConnection, final T consumer) {
		super(consumer);
		this.linkedConnection = new WeakReference<Connection>(linkedConnection);
	}

	@Override
	public boolean isObjectDependent() {
		return true;
	}
	
	@Override
	public Object lockObjects() {
		return this.linkedConnection.get();
	}
	
	@Override
	public boolean isCompatibleWith(final Object elem) {
		if (super.isCompatibleWith(elem)) {
			return true;
		}
		Object obj = this.linkedConnection.get();
		if (obj == elem) {
			return true;
		}
		return false;
	}

	@Override
	public void disconnect() {
		Connection con = this.linkedConnection.get();
		if (con != null) {
			con.connectionIsRemovedBySignal();
		}
	}
}