package org.atriasoft.esignal;

import java.util.Iterator;
import java.util.List;
import java.util.function.BiConsumer;
import java.util.function.Consumer;

import org.atriasoft.esignal.internal.ConnectedElementInterface;

import edu.umd.cs.findbugs.annotations.CheckReturnValue;

/**
 * Simple interface to manage signal connection and disconnection with notification on connection change.
 * @see Signal documentation
 * <pre>{@code
 * class EmiterSimple {
 *     public final ISignal<String> signalEvent = new ISignal<>();
 *     public EmiterSimple() {
 *         signalEvent.setCallBackNotification(this::onConnectionChange);
 *     }
 *     public void onConnectionChange(final int currentNumberConnection, final int deltaConnection) {
 *         LOGGER.info("Number of connection Change : {} dalta={}", currentNumberConnection, deltaConnection);
 *     }
 * }
 * }</pre>
 * 
 * @param <T> Type of the signal
 * 
 */
public class ISignal<T> extends GenericSignalInstrumented<Consumer<T>, BiConsumer<Object, T>> {
	@CheckReturnValue
	@SuppressWarnings("unchecked")
	public <V> Connection connect(final V object, final BiConsumer<V, T> function) {
		return connect(object, (final Object obj, final T value) -> {
			function.accept((V) obj, value);
		});
	}
	
	/**
	 * Connect to the signal and automatically disconnect when the object is removed
	 * @param object Object to check if remove to continue keeping the signal active (Keep a WeakReference on it only)
	 * @param function Function to connect (Keep a WeakReference on it only)
	 */
	@SuppressWarnings("unchecked")
	public <V> void connectAuto(final V object, final BiConsumer<V, T> function) {
		connectAuto(object, (final Object obj, final T value) -> {
			function.accept((V) obj, value);
		});
	}
	
	/**
	 * Emit a signal on all element connect (and clean the list of unlinked elements).
	 * @param value Value to set in parameter.
	 */
	public void emit(final T value) {
		final List<ConnectedElementInterface<Consumer<T>, BiConsumer<Object, T>>> tmp = getACleanedList();
		if (tmp == null) {
			return;
		}
		// real call elements
		final Iterator<ConnectedElementInterface<Consumer<T>, BiConsumer<Object, T>>> iterator = tmp.iterator();
		while (iterator.hasNext()) {
			final ConnectedElementInterface<Consumer<T>, BiConsumer<Object, T>> elem = iterator.next();
			
			final Object remoteLockObject = elem.lockObjects();
			if (elem.isObjectDependent() && remoteLockObject == null) {
				continue;
			}
			final Consumer<T> tmpConsumer = elem.getConsumer();
			if (tmpConsumer != null) {
				tmpConsumer.accept(value);
				continue;
			}
			// Not a dead code, but very hard to simply test it.
			final BiConsumer<Object, T> tmpConsumer2 = elem.getConsumer2();
			if (tmpConsumer2 != null) {
				tmpConsumer2.accept(elem.getObject(), value);
			}
		}
	}
}
