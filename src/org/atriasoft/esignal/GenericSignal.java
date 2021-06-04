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
	
	protected List<ConnectedElement<T>> data = new ArrayList<>();
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

	public void connect(final T function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElement<T>(function));
		}
	}
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
	public Connection connectDynamic(final T function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<T>(out, function));
		}
		return out;
	}
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
	public int size() {
		return this.data.size();
	}
	public int sizeCleaned() {
		cleanedList();
		return this.data.size();
	}

}
