package org.atriasoft.esignal.internal;

import java.lang.ref.WeakReference;

import org.atriasoft.esignal.Connection;

record LockObject(Object aaa, Object bbb) {};

/**
 * Connected element on a consumer to Manage event
 *
 * @param <T> Type Of the Signal
 * @param <U> Second type Of the Signal
 */
public class ConnectedElementURefConsumerWeakObjectWeakConnection<T, U> extends ConnectedElementURefConsumerWeakObject<T, U> {
	protected final WeakReference<Connection> linkedConnection;
	
	public ConnectedElementURefConsumerWeakObjectWeakConnection(final Connection linkedConnection, final Object linkedObject, final U consumer) {
		super(linkedObject, consumer);
		this.linkedConnection = new WeakReference<Connection>(linkedConnection);
	}
	
	@Override
	public Object lockObjects() {
		Object tmp1 = this.linkedObject.get();
		Object tmp2 = this.linkedConnection.get();
		if (tmp1 != null && tmp2 != null) {
			return new LockObject(tmp1, tmp2);
		}
		return null;
	}

	@Override
	public boolean isCompatibleWith(final Object elem) {
		if (super.isCompatibleWith(elem)) {
			return true;
		}
		if (this.linkedConnection.get() == elem) {
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
