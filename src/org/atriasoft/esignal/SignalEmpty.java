package org.atriasoft.esignal;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.atriasoft.esignal.internal.ConnectedElement;
import org.atriasoft.esignal.internal.ConnectedElementDynamic;

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
public class SignalEmpty implements ConnectionRemoveInterface {
	List<ConnectedElement<Runnable>> data = new ArrayList<>();
	
	public void clear() {
		List<ConnectedElement<Runnable>> data2 = this.data;
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
		final Iterator<ConnectedElement<Runnable>> iterator = data2.iterator();
		while (iterator.hasNext()) {
			final ConnectedElement<Runnable> elem = iterator.next();
			elem.disconnect();
		}
	}

	public void connect(final Runnable function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElement<Runnable>(function));
		}
	}
	public void disconnect(final Runnable obj) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<Runnable>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<Runnable> elem = iterator.next();
				if (elem.isCompatibleWith(obj)) {
					iterator.remove();
				}
			}
		}
	}
	public Connection connectDynamic(final Runnable function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<Runnable>(out, function));
		}
		return out;
	}
	public void connectAutoRemoveObject(final Object reference, final Runnable function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<Runnable>(reference, function));
		}
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<Runnable>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<Runnable> elem = iterator.next();
				if (elem.isCompatibleWith(connection)) {
					elem.disconnect();
					iterator.remove();
				}
			}
		}
	}
	
	public void emit() {
		List<ConnectedElement<Runnable>> tmp;
		// clean the list:
		synchronized(this.data) {
			final Iterator<ConnectedElement<Runnable>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<Runnable> elem = iterator.next();
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
	
	public int size() {
		return this.data.size();
	}

}
