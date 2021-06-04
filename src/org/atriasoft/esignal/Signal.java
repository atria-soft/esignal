package org.atriasoft.esignal;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;

import org.atriasoft.esignal.internal.ConnectedElement;
import org.atriasoft.esignal.internal.ConnectedElementDynamic;

/**
 * Simple interface to manage signal connection and disconnection
 * <pre>{@code
 * class EmiterSimple {
 *     public Signal<String> signalEvent = new Signal<>();
 * }
 *  
 * class ReceiverSimple {
 *     public void onEvent(String data) {
 *         Log.error("function receive: " + data);
 *     }
 *     public connectLambda(EmiterSimple other) {
 *         // Note : this lambda is reference a a global, then it will never removed in the connection list ==> refer the local class or @see connectAutoRemoveObject
 *         other.signalEvent.connect((data) -> {
 *             Log.error("lambda receive: " + data);
 *         });
 *     }
 * }
 * // use : 
 * EmiterSimple aaa = new EmiterSimple();
 * ReceiverSimple bbb = new ReceiverSimple();
 * // Emit a signal:
 * aaa.signalEvent.emit("My message ...");
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
 * @param <T> Type of the signal
 * 
 */
public class Signal<T> implements ConnectionRemoveInterface {
	List<ConnectedElement<Consumer<T>>> data = new ArrayList<>();
	
	public void clear() {
		List<ConnectedElement<Consumer<T>>> data2 = this.data;
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
		final Iterator<ConnectedElement<Consumer<T>>> iterator = data2.iterator();
		while (iterator.hasNext()) {
			final ConnectedElement<Consumer<T>> elem = iterator.next();
			elem.disconnect();
		}
	}

	public void connect(final Consumer<T> function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElement<Consumer<T>>(function));
		}
	}
	public void disconnect(final Consumer<T> obj) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<Consumer<T>>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<Consumer<T>> elem = iterator.next();
				if (elem.isCompatibleWith(obj)) {
					iterator.remove();
				}
			}
		}
	}
	public Connection connectDynamic(final Consumer<T> function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<Consumer<T>>(out, function));
		}
		return out;
	}
	public void connectAutoRemoveObject(final Object reference, final Consumer<T> function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<Consumer<T>>(reference, function));
		}
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<Consumer<T>>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<Consumer<T>> elem = iterator.next();
				if (elem.isCompatibleWith(connection)) {
					elem.disconnect();
					iterator.remove();
				}
			}
		}
	}

	public void emit(final T value) {
		List<ConnectedElement<Consumer<T>>> tmp;
		// clean the list:
		synchronized(this.data) {
			final Iterator<ConnectedElement<Consumer<T>>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<Consumer<T>> elem = iterator.next();
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
			final Iterator<ConnectedElement<Consumer<T>>> iterator = tmp.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<Consumer<T>> elem = iterator.next();
				Consumer<T> tmpObject = elem.getConsumer();
				if (tmpObject == null) {
					continue;
				}
				tmpObject.accept(value);
			}
		}
	}
	
	public int size() {
		return this.data.size();
	}

}
