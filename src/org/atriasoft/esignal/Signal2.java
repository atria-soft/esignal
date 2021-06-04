package org.atriasoft.esignal;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.BiConsumer;

import org.atriasoft.esignal.internal.ConnectedElement;
import org.atriasoft.esignal.internal.ConnectedElementDynamic;


/**
 * Simple interface to manage signal connection and disconnection
 * <pre>{@code
 * class EmiterSimple {
 *     public Signal2<String, Double> signalEvent = new Signal2<>();
 * }
 *  
 * class ReceiverSimple {
 *     public void onEvent(String data, Double data2) {
 *         Log.error("function receive: " + data + "  " + data2);
 *     }
 *     public connectLambda(EmiterSimple other) {
 *         // Note : this lambda is reference a a global, then it will never removed in the connection list ==> refer the local class or @see connectAutoRemoveObject
 *         other.signalEvent.connect((data, data2) -> {
 *             Log.error("lambda receive: " + data + "  " + data2);
 *         });
 *     }
 * }
 * // use : 
 * EmiterSimple aaa = new EmiterSimple();
 * ReceiverSimple bbb = new ReceiverSimple();
 * // Emit a signal:
 * aaa.signalEvent.emit("My message ...", 16516.541654);
 * // simple direct connection: 
 * aaa.signalEvent.connect(bbb::onEvent);
 * //removable connection (2 possibilities:)
 * // First solution (best way ==> does not need to lock a reference on the current object and the remote)
 * {
 *     Connection connect = aaa.signalEvent.connectDynamic(bbb::onEvent());
 *     // disconnect
 *     connect.disconnect();
 * }
 * // Second solution
 * {
 *     Consumer<?> connect = bbb::onEvent;
 *     aaa.signalEvent.connect(connect);
 *     // disconnect
 *     aaa.signalEvent.disconnect(connect);
 * }
 * }</pre>
 * 
 * @param <T> First type of the signal
 * @param <U> Second type of the signal
 * 
 */
public class Signal2<T, U> implements ConnectionRemoveInterface {
	List<ConnectedElement<BiConsumer<T, U>>> data = new ArrayList<>();
	
	public void clear() {
		List<ConnectedElement<BiConsumer<T, U>>> data2 = this.data;
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
		final Iterator<ConnectedElement<BiConsumer<T, U>>> iterator = data2.iterator();
		while (iterator.hasNext()) {
			final ConnectedElement<BiConsumer<T, U>> elem = iterator.next();
			elem.disconnect();
		}
	}

	public void connect(final BiConsumer<T, U> function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElement<BiConsumer<T, U>>(function));
		}
	}
	public void disconnect(final BiConsumer<T, U> obj) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<BiConsumer<T, U>>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<BiConsumer<T, U>> elem = iterator.next();
				if (elem.isCompatibleWith(obj)) {
					iterator.remove();
				}
			}
		}
	}
	public Connection connectDynamic(final BiConsumer<T, U> function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<BiConsumer<T, U>>(out, function));
		}
		return out;
	}
	public void connectAutoRemoveObject(final Object reference, final BiConsumer<T, U> function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<BiConsumer<T, U>>(reference, function));
		}
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<BiConsumer<T, U>>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<BiConsumer<T, U>> elem = iterator.next();
				if (elem.isCompatibleWith(connection)) {
					elem.disconnect();
					iterator.remove();
				}
			}
		}
	}

	public void emit(final T valueT, final U valueU) {
		List<ConnectedElement<BiConsumer<T, U>>> tmp;
		// clean the list:
		synchronized(this.data) {
			final Iterator<ConnectedElement<BiConsumer<T, U>>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<BiConsumer<T, U>> elem = iterator.next();
				Object tmpObject = elem.getConsumer();
				if (tmpObject == null) {
					elem.disconnect();
					iterator.remove();
				}
			}
			// simple optimization:
			if (this.data.isEmpty()) {
				return;
			}
			// clone the list to permit to have asynchronous remove call
			tmp = new ArrayList<>(this.data);
		}
		// real call elements
		{
			final Iterator<ConnectedElement<BiConsumer<T, U>>> iterator = tmp.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<BiConsumer<T, U>> elem = iterator.next();
				BiConsumer<T, U> tmpObject = elem.getConsumer();
				if (tmpObject == null) {
					continue;
				}
				tmpObject.accept(valueT, valueU);
			}
		}
	}
	
	public int size() {
		return this.data.size();
	}

}
