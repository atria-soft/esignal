package org.atriasoft.esignal;

import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;

import org.atriasoft.esignal.internal.ConnectedElementInterface;

import edu.umd.cs.findbugs.annotations.CheckReturnValue;

/**
 * Simple interface to manage signal connection and disconnection
 * <pre>{@code
 * class EmiterSimple {
 *     public SignalEmpty signalEvent = new SignalEmpty();
 * }
 *  
 * class ReceiverSimple {
 *     public void onEvent() {
 *         LOGGER.error("function receive event ...");
 *     }
 *     public connectLambda(EmiterSimple other) {
 *         // Note : this lambda is reference a a global, then it will never removed in the connection list ==> refer the local class or @see connectAutoRemoveObject
 *         other.signalEvent.connect(() -> {
 *             LOGGER.error("lambda receive event");
 *         });
 *     }
 * }
 * // use : 
 * EmiterSimple aaa = new EmiterSimple();
 * ReceiverSimple bbb = new ReceiverSimple();
 * // Emit a signal:
 * aaa.signalEvent.emit();
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
 */
public class SignalEmpty extends GenericSignal<Runnable, Consumer<Object>> {
	/**
	 * Emit a signal on all element connect (and clean the list of unlinked elements).
	 */
	public void emit() {
		List<ConnectedElementInterface<Runnable, Consumer<Object>>> tmp = getACleanedList();
		if (tmp == null) {
			return;
		}
		// real call elements
		final Iterator<ConnectedElementInterface<Runnable, Consumer<Object>>> iterator = tmp.iterator();
		while (iterator.hasNext()) {
			final ConnectedElementInterface<Runnable, Consumer<Object>> elem = iterator.next();
			
			Object remoteLockObject = elem.lockObjects();
			if (elem.isObjectDependent() && remoteLockObject == null) {
				continue;
			}
			Runnable tmpConsumer = elem.getConsumer();
			if (tmpConsumer != null) {
				tmpConsumer.run();
				continue;
			}
			// Not a dead code, but very hard to simply test it.
			Consumer<Object> tmpConsumer2 = elem.getConsumer2();
			if (tmpConsumer2 != null) {
				tmpConsumer2.accept(elem.getObject());
			}
		}
	}

	/**
	 * Connect to the signal and automatically disconnect when the object is removed
	 * @param object Object to check if remove to continue keeping the signal active (Keep a WeakReference on it only)
	 * @param function Function to connect (Keep a WeakReference on it only)
	 */
	@SuppressWarnings("unchecked")
	public <V> void connectAuto(final V object, final Consumer<V> function) {
		connectAuto(object,
				(final Object obj) -> {
					function.accept((V)obj);
				});
	}
	
	@CheckReturnValue
	@SuppressWarnings("unchecked")
	public <V> Connection connect(final V object, final Consumer<V> function) {
		return connect(object,
				(final Object obj) -> {
					function.accept((V)obj);
				});
	}
	
}
