package org.atriasoft.esignal;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

class ConnectedElementEmpty {
	protected final WeakReference<Runnable> runner;
	
	public ConnectedElementEmpty(final Runnable runner) {
		this.runner = new WeakReference<Runnable>(runner);
	}
	
	public Runnable getRunner() {
		return this.runner.get();
	}

	public boolean isCompatibleWith(final Object elem) {
		Object out = this.runner.get();
		if (out == elem) {
			return true;
		}
		return false;
	}

	public void disconnect() {
		
	}
}

class ConnectedElementDynamicEmpty extends ConnectedElementEmpty {
	protected final WeakReference<Object> linkedObject;
	
	public ConnectedElementDynamicEmpty(final Object linkedObject, final Runnable runner) {
		super(runner);
		this.linkedObject = new WeakReference<Object>(linkedObject);
	}
	
	@Override
	public Runnable getRunner() {
		if (this.linkedObject.get() == null) {
			return null;
		}
		return this.runner.get();
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

public class SignalEmpty implements ConnectionRemoveInterface {
	List<ConnectedElementEmpty> data = new ArrayList<>();
	
	public void clear() {
		List<ConnectedElementEmpty> data2 = this.data;
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
		final Iterator<ConnectedElementEmpty> iterator = data2.iterator();
		while (iterator.hasNext()) {
			final ConnectedElementEmpty elem = iterator.next();
			elem.disconnect();
		}
	}

	public void connect(final Runnable function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementEmpty(function));
		}
	}
	public void disconnect(final Runnable obj) {
		synchronized(this.data) {
			final Iterator<ConnectedElementEmpty> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElementEmpty elem = iterator.next();
				if (elem.isCompatibleWith(obj)) {
					iterator.remove();
				}
			}
		}
	}
	public Connection connectDynamic(final Runnable function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamicEmpty(out, function));
		}
		return out;
	}
	public void connectAutoRemoveObject(final Object reference, final Runnable function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamicEmpty(reference, function));
		}
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<ConnectedElementEmpty> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElementEmpty elem = iterator.next();
				if (elem.isCompatibleWith(connection)) {
					elem.disconnect();
					iterator.remove();
				}
			}
		}
	}
	
	public void emit() {
		List<ConnectedElementEmpty> tmp;
		// clean the list:
		synchronized(this.data) {
			final Iterator<ConnectedElementEmpty> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElementEmpty elem = iterator.next();
				Object tmpObject = elem.getRunner();
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
			final Iterator<ConnectedElementEmpty> iterator = tmp.iterator();
			while (iterator.hasNext()) {
				final ConnectedElementEmpty elem = iterator.next();
				Runnable tmpObject = elem.getRunner();
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
