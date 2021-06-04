package org.atriasoft.esignal;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.atriasoft.esignal.internal.ConnectedElement;
import org.atriasoft.esignal.internal.ConnectedElementDynamic;

/**
 * Generic interface for a signaling model...
 *
 * @param <T> generic Runnable, Consumer, or BiConsumer template...
 */
public class GenericSignal<T> implements ConnectionRemoveInterface {
	// List of all connected Links
	protected List<ConnectedElement<T>> data = new ArrayList<>();
	/**
	 * Clear all connection on this signal.
	 */
	public void clear() {
		List<ConnectedElement<T>> data2 = this.data;
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
		final Iterator<ConnectedElement<T>> iterator = data2.iterator();
		while (iterator.hasNext()) {
			final ConnectedElement<T> elem = iterator.next();
			elem.disconnect();
		}
	}
	/**
	 * Connect a Function to this signal
	 * @param function Function to connect (Keep a WeakReference on it only)
	 * @apiNote Make some attention when you connect a global lamba, nothing can pertmit to remove it @see connectAutoRemoveObject()
	 */
	public void connect(final T function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElement<T>(function));
		}
	}
	/**
	 * Disconnect all connection that have this Object/function in reference
	 * @param obj Object to check the compatibility.
	 * @apiNote if you add a direct connection like {code  connect(this::onEvent(...)) } you can not disconnect it
	 */
	public void disconnect(final T obj) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<T> elem = iterator.next();
				if (elem.isCompatibleWith(obj)) {
					iterator.remove();
				}
			}
		}
	}
	/**
	 * Connect to the signal with a @see Connection object that permit to remove the connection to the signal.
	 * @param function Function to connect (Keep a WeakReference on it only)
	 * @return The connection interface.
	 */
	public Connection connectDynamic(final T function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<T>(out, function));
		}
		return out;
	}
	/**
	 * Connect to the signal and automatically disconnect when the object is removed
	 * @param object Object to check if remove to continue keeping the signal active (Keep a WeakReference on it only)
	 * @param function Function to connect (Keep a WeakReference on it only)
	 */
	public void connectAutoRemoveObject(final Object object, final T function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<T>(object, function));
		}
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<T> elem = iterator.next();
				if (elem.isCompatibleWith(connection)) {
					elem.disconnect();
					iterator.remove();
				}
			}
		}
	}
	
	/**
	 * Get a copy on the List of current connection and remove all the deprecated connection.
	 * @return The copy of the available connection (Note: the connection can be removed when return)
	 */
	protected List<ConnectedElement<T>> getACleanedList() {
		// first clean the list
		cleanedList();
		// get a copy of elements
		List<ConnectedElement<T>> out = null;
		// clean the list:
		synchronized(this.data) {
			// simple optimization:
			if (this.data.isEmpty()) {
				return null;
			}
			// clone the list to permit to have asynchronous remove call
			out = new ArrayList<>(this.data);
		}
		return out;
	}
	/**
	 * Clean all the deprecated list of removed elements
	 */
	protected void cleanedList() {
		// clean the list:
		synchronized(this.data) {
			final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<T> elem = iterator.next();
				Object tmpObject = elem.getConsumer();
				if (tmpObject == null) {
					elem.disconnect();
					iterator.remove();
				}
			}
		}
	}
	/**
	 * Get the Number of current connection (clean is not done).
	 * @return Number of connection
	 */
	public int size() {
		return this.data.size();
	}
	/**
	 * Get the Number of current connection (clean is done).
	 * @return Number of connection
	 */
	public int sizeCleaned() {
		cleanedList();
		return this.data.size();
	}

}
