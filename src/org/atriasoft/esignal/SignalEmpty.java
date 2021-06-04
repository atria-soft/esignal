package org.atriasoft.esignal;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.atriasoft.esignal.internal.ConnectedElement;

/**
 * Simple interface to manage signal connection and disconnection
 * <pre>{@code
 * class EmiterSimple {
 *     public SignalEmpty signalEvent = new SignalEmpty();
 * }
 *  
 * class ReceiverSimple {
 *     public void onEvent() {
 *         Log.error("function receive event ...");
 *     }
 *     public connectLambda(EmiterSimple other) {
 *         // Note : this lambda is reference a a global, then it will never removed in the connection list ==> refer the local class or @see connectAutoRemoveObject
 *         other.signalEvent.connect(() -> {
 *             Log.error("lambda receive event");
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
public class SignalEmpty extends GenericSignal<Runnable> {
	
	public void emit() {
		List<ConnectedElement<Runnable>> tmp = getACleanedList();
		if (tmp == null) {
			return;
		}
		// real call elements
		final Iterator<ConnectedElement<Runnable>> iterator = tmp.iterator();
		while (iterator.hasNext()) {
			final ConnectedElement<Runnable> elem = iterator.next();
			Runnable tmpObject = elem.getConsumer();
			if (tmpObject == null) {
				continue;
			}
			tmpObject.run();
		}
	}
	
}
