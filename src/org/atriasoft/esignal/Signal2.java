package org.atriasoft.esignal;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.BiConsumer;

class BiConnectedElement<T, U> {
	protected final WeakReference<BiConsumer<T, U>> consumer;
	
	public BiConnectedElement(final BiConsumer<T, U> consumer) {
		this.consumer = new WeakReference<BiConsumer<T, U>>(consumer);
	}
	
	public BiConsumer<T, U> getConsumer() {
		return this.consumer.get();
	}

	public boolean isCompatibleWith(final Object elem) {
		Object out = this.consumer.get();
		if (out == elem) {
			return true;
		}
		return false;
	}

	public void disconnect() {
		
	}
}

class BiConnectedElementDynamic<T, U> extends BiConnectedElement<T, U> {
	protected final WeakReference<Object> linkedObject;
	
	public BiConnectedElementDynamic(final Object linkedObject, final BiConsumer<T, U> consumer) {
		super(consumer);
		this.linkedObject = new WeakReference<Object>(linkedObject);
	}
	
	@Override
	public BiConsumer<T, U> getConsumer() {
		if (this.linkedObject.get() == null) {
			return null;
		}
		return this.consumer.get();
	}
	
	@Override
	public boolean isCompatibleWith(final Object elem) {
		if (super.isCompatibleWith(elem)) {
			return true;
		}
		Object obj = this.linkedObject.get();
		if (obj == elem) {
			return true;
		}
		return false;
	}

	@Override
	public void disconnect() {
		Object obj = this.linkedObject.get();
		if (obj == null) {
			return;
		}
		if (obj instanceof Connection tmp) {
			tmp.connectionIsRemovedBySignal();
		}
	}
}

public class Signal2<T, U> implements ConnectionRemoveInterface {
	List<BiConnectedElement<T, U>> data = new ArrayList<>();
	
	public void clear() {
		List<BiConnectedElement<T, U>> data2 = this.data;
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
		final Iterator<BiConnectedElement<T, U>> iterator = data2.iterator();
		while (iterator.hasNext()) {
			final BiConnectedElement<T, U> elem = iterator.next();
			elem.disconnect();
		}
	}

	public void connect(final BiConsumer<T, U> function) {
		synchronized(this.data) {
			this.data.add(new BiConnectedElement<T, U>(function));
		}
	}
	public void disconnect(final BiConsumer<T, U> obj) {
		synchronized(this.data) {
			final Iterator<BiConnectedElement<T, U>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final BiConnectedElement<T, U> elem = iterator.next();
				if (elem.isCompatibleWith(obj)) {
					iterator.remove();
				}
			}
		}
	}
	public Connection connectDynamic(final BiConsumer<T, U> function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new BiConnectedElementDynamic<T, U>(out, function));
		}
		return out;
	}
	public void connectAutoRemoveObject(final Object reference, final BiConsumer<T, U> function) {
		synchronized(this.data) {
			this.data.add(new BiConnectedElementDynamic<T, U>(reference, function));
		}
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<BiConnectedElement<T, U>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final BiConnectedElement<T, U> elem = iterator.next();
				if (elem.isCompatibleWith(connection)) {
					elem.disconnect();
					iterator.remove();
				}
			}
		}
	}

	public void emit(final T valueT, final U valueU) {
		List<BiConnectedElement<T, U>> tmp;
		// clean the list:
		synchronized(this.data) {
			final Iterator<BiConnectedElement<T, U>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final BiConnectedElement<T, U> elem = iterator.next();
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
			final Iterator<BiConnectedElement<T, U>> iterator = tmp.iterator();
			while (iterator.hasNext()) {
				final BiConnectedElement<T, U> elem = iterator.next();
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
