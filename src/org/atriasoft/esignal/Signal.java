package org.atriasoft.esignal;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;

import org.atriasoft.esignal.internal.ConnectedElement;

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
public class Signal<T> extends GenericSignal<Consumer<T>> {
	public void emit(final T value) {
		List<ConnectedElement<Consumer<T>>> tmp = getACleanedList();
		if (tmp == null) {
			return;
		}
		// real call elements
		final Iterator<ConnectedElement<Consumer<T>>> iterator = tmp.iterator();
		while (iterator.hasNext()) {
			final ConnectedElement<Consumer<T>> elem = iterator.next();
			Consumer<T> tmpObject = elem.getConsumer();
			if (tmpObject == null) {
				// Not a dead code, but very hard to simply test it.
				continue;
			}
			tmpObject.accept(value);
		}
	}
}
