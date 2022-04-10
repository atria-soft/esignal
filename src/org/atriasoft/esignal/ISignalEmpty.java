package org.atriasoft.esignal;

import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;

import org.atriasoft.esignal.internal.ConnectedElementInterface;

import edu.umd.cs.findbugs.annotations.CheckReturnValue;

/**
 * Simple interface to manage signal connection and disconnection with notification on connection change.
 * @see SignalEmpty documentation
 * <pre>{@code
 * class EmiterSimple {
 *     public final ISignalEmpty signalEvent = new ISignalEmpty();
 *     public EmiterSimple() {
 *         signalEvent.setCallBackNotification(this::onConnectionChange);
 *     }
 *     public void onConnectionChange(final int currentNumberConnection, final int deltaConnection) {
 *         Log.info("Number of connection Change : {} delta={}", currentNumberConnection, deltaConnection);
 *     }
 * }
 * }</pre>
 * 
 */
public class ISignalEmpty extends GenericSignalInstrumented<Runnable, Consumer<Object>> {
	@CheckReturnValue
	@SuppressWarnings("unchecked")
	public <V> Connection connect(final V object, final Consumer<V> function) {
		return connect(object, (final Object obj) -> {
			function.accept((V) obj);
		});
	}
	
	/**
	 * Connect to the signal and automatically disconnect when the object is removed
	 * @param object Object to check if remove to continue keeping the signal active (Keep a WeakReference on it only)
	 * @param function Function to connect (Keep a WeakReference on it only)
	 */
	@SuppressWarnings("unchecked")
	public <V> void connectAuto(final V object, final Consumer<V> function) {
		connectAuto(object, (final Object obj) -> {
			function.accept((V) obj);
		});
	}
	
	/**
	 * Emit a signal on all element connect (and clean the list of unlinked elements).
	 */
	public void emit() {
		final List<ConnectedElementInterface<Runnable, Consumer<Object>>> tmp = getACleanedList();
		if (tmp == null) {
			return;
		}
		// real call elements
		final Iterator<ConnectedElementInterface<Runnable, Consumer<Object>>> iterator = tmp.iterator();
		while (iterator.hasNext()) {
			final ConnectedElementInterface<Runnable, Consumer<Object>> elem = iterator.next();
			
			final Object remoteLockObject = elem.lockObjects();
			if (elem.isObjectDependent() && remoteLockObject == null) {
				continue;
			}
			final Runnable tmpConsumer = elem.getConsumer();
			if (tmpConsumer != null) {
				tmpConsumer.run();
				continue;
			}
			// Not a dead code, but very hard to simply test it.
			final Consumer<Object> tmpConsumer2 = elem.getConsumer2();
			if (tmpConsumer2 != null) {
				tmpConsumer2.accept(elem.getObject());
			}
		}
	}
	
}
