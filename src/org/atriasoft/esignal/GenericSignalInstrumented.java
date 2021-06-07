package org.atriasoft.esignal;

import java.util.function.BiConsumer;

public class GenericSignalInstrumented<T, U> extends GenericSignal<T, U> {
	BiConsumer<Integer, Integer> callback = null;

	/**
	 * Notify to the callback that the number of connection change
	 * @param sizeStart Number of connection before call
	 * @param sizeEnd Number of connection after call
	 */
	protected synchronized void notifyChange(final int sizeStart, final int sizeEnd) {
		if (sizeEnd == sizeStart) {
			return;
		}
		if (this.callback == null) {
			return;
		}
		BiConsumer<Integer, Integer> cb = this.callback;
		if (cb == null) {
			this.callback = null;
			return;
		}
		// notify client
		cb.accept(sizeEnd, sizeEnd - sizeStart);
	}
	
	/**
	 * Specify the consumer that managed the dynamic connection handle change
	 * @param callback BiConsumer that receive the number of current connection and the delta of connection since the previous call.
	 */
	public synchronized void setCallBackNotification(final BiConsumer<Integer, Integer> callback) {
		this.callback = callback;
	}
	
	@Override
	public void clear() {
		int sizeStart = this.data.size();
		super.clear();
		notifyChange(sizeStart, this.data.size());
	}

	@Override
	public Connection connect(final T function) {
		int sizeStart = this.data.size();
		Connection tmp = super.connect(function);
		notifyChange(sizeStart, this.data.size());
		return tmp;
	}

	@Override
	public void disconnect(final T obj) {
		int sizeStart = this.data.size();
		super.disconnect(obj);
		notifyChange(sizeStart, this.data.size());
	}

	@Override
	public void connectWeak(final T function) {
		int sizeStart = this.data.size();
		super.connectWeak(function);
		notifyChange(sizeStart, this.data.size());
	}

	@Override
	public Connection connect(final Object object, final U function) {
		int sizeStart = this.data.size();
		Connection out = super.connect(object, function);
		notifyChange(sizeStart, this.data.size());
		return out;
	}

	@Override
	public void connectAuto(final Object object, final U function) {
		int sizeStart = this.data.size();
		super.connectAuto(object, function);
		notifyChange(sizeStart, this.data.size());
	}

	@Override
	public void connectAutoRemoveObject(final Object object, final T function) {
		int sizeStart = this.data.size();
		super.connectAutoRemoveObject(object, function);
		notifyChange(sizeStart, this.data.size());
	}

	@Override
	public void disconnect(final Connection connection) {
		int sizeStart = this.data.size();
		super.disconnect(connection);
		notifyChange(sizeStart, this.data.size());
	}

	@Override
	protected void cleanedList() {
		int sizeStart = this.data.size();
		super.cleanedList();
		notifyChange(sizeStart, this.data.size());
	}
	
}
