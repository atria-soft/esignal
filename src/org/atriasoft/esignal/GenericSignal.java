package org.atriasoft.esignal;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.atriasoft.esignal.internal.ConnectedElementInterface;
import org.atriasoft.esignal.internal.ConnectedElementTRefConsumerWeakConnection;
import org.atriasoft.esignal.internal.ConnectedElementTRefConsumerWeakObject;
import org.atriasoft.esignal.internal.ConnectedElementTWeakConsumer;
import org.atriasoft.esignal.internal.ConnectedElementURefConsumerWeakObject;
import org.atriasoft.esignal.internal.ConnectedElementURefConsumerWeakObjectWeakConnection;

import edu.umd.cs.findbugs.annotations.CheckReturnValue;


/**
 * Generic interface for a signaling model...
 *
 * @param <T> generic Runnable, Consumer, or BiConsumer template...
 */
public class GenericSignal<T, U> implements ConnectionRemoveInterface {
	// List of all connected Links
	protected List<ConnectedElementInterface<T, U>> data = new ArrayList<>();
	/**
	 * Clear all connection on this signal.
	 */
	public void clear() {
		List<ConnectedElementInterface<T, U>> data2 = this.data;
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
		final Iterator<ConnectedElementInterface<T, U>> iterator = data2.iterator();
		while (iterator.hasNext()) {
			final ConnectedElementInterface<T, U> elem = iterator.next();
			elem.disconnect();
		}
	}
	/**
	 * Connect a Function to this signal (the signal will keep only the weak pointer on the function)
	 * @param function Function to connect (Keep a WeakReference on it only)
	 * @apiNote Make some attention when you connect a global lamba, nothing can permit to remove it @see connectAutoRemoveObject()
	 * 
	 * {@code
	 *     // Create the connection
	 *     // (1) create connection (direct):
	 * 	   ConsumerXX tmp = this::onEventXXX;
	 *     eventXXX.connectWeak(tmp);
	 *     // (2) create connection (lambda):
	 * 	   ConsumerXX tmp = (www) -> { ... };
	 *     eventXXX.connectWeak(tmp);
	 *     
	 *     // Disconnect:
	 *     // (1) remove connection (abstract way):
	 *     tmp = null; // ==> the function scope will be removed only when the garbage collector is called.
	 *     // (2) remove the connection from signal:
	 *     eventXXX.disconnect(tmp);
	 * }
	 * @apiNote You can be called while the Garbage collected does not removed the reference on the function or the lambda... 
	 */
	public void connectWeak(final T function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementTWeakConsumer<T, U>(function));
		}
	}
	

	/**
	 * Connect to the signal with a @see Connection object that permit to remove the connection to the signal.
	 * @param function Function to connect (Keep a WeakReference on it only)
	 * @return The connection interface.
	 * {@code
	 *     // Create the connection
	 *     // (1) create connection (direct):
	 *     Connection tmp = eventXXX.connect(this::onEventXXX);
	 *     // (2) create connection (lambda):
	 *     Connection tmp = eventXXX.connect((www) -> { ... });
	 *     
	 *     // Disconnect:
	 *     // (1) remove connection (abstract way):
	 *     tmp = null; // ==> the function scope will be removed only when the garbage collector is called.
	 *     // (2) remove the connection from signal:
	 *     tmp.close(); // use close instead of disconnect ==> permit to detect error (Connection is AutoClosable)
	 *     // (3) remove the connection from signal:
	 *     eventXXX.disconnect(tmp);
	 * }
	 */
	@CheckReturnValue
	public Connection connect(final T function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementTRefConsumerWeakConnection<T, U>(out, function));
		}
		return out;
	}

	/**
	 * Connect to the signal with Input object as first parameter with a @see Connection object that permit to remove the connection to the signal.
	 * This permit to remove hidden capture of the "this" in the lambda...
	 * @param function Function to connect (Keep a WeakReference on it only)
	 * @return The connection interface.
	 * {@code
	 *     class MyClass {
	 *         Connection tmp = null;
	 *         MyClass() { 
	 *             // Create the connection
	 *             // (1) create connection (direct):
	 *             tmp = eventXXX.connect(this, this::onEventXXX);
	 *             // (2) create connection (lambda):
	 *             tmp = eventXXX.connect((self, www) -> { ... });
	 *         }
	 *         public static onEventXXX(Object self, ...) {
	 *             MyClass mySelf = (MyClass) self;
	 *             ...
	 *         }
	 *         // Disconnect:
	 *         public void disconnect() {
	 *             // (1) remove connection (abstract way):
	 *             tmp = null; // ==> the function scope will be removed only when the garbage collector is called.
	 *             // (2) remove the connection from signal:
	 *             tmp.close(); // use close instead of disconnect ==> permit to detect error (Connection is AutoClosable)
	 *             // (3) remove the connection from signal:
	 *             eventXXX.disconnect(tmp);
	 *             // OR
	 *             eventXXX.disconnect(this);
	 *         }
	 *     }
	 * }
	 */
	@CheckReturnValue
	public Connection connect(final Object object, final U function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementURefConsumerWeakObjectWeakConnection<T, U>(out, object, function));
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
			this.data.add(new ConnectedElementTRefConsumerWeakObject<T, U>(object, function));
		}
	}
	/**
	 * Connect to the signal and automatically disconnect when the object is removed
	 * @param object Object to check if remove to continue keeping the signal active (Keep a WeakReference on it only)
	 * @param function Function to connect (Keep a WeakReference on it only)
	 */
	public void connectAuto(final Object object, final U function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementURefConsumerWeakObject<T, U>(object, function));
		}
	}
	
	
	
	
	
	/**
	 * Disconnect all connection that have this Object/function in reference
	 * @param obj Object to check the compatibility.
	 * @apiNote if you add a direct connection like {code  connect(this::onEvent(...)) } you can not disconnect it
	 */
	public void disconnect(final T obj) {
		synchronized(this.data) {
			final Iterator<ConnectedElementInterface<T, U>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElementInterface<T, U> elem = iterator.next();
				if (elem.isCompatibleWith(obj)) {
					iterator.remove();
				}
			}
		}
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<ConnectedElementInterface<T, U>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElementInterface<T, U> elem = iterator.next();
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
	protected List<ConnectedElementInterface<T, U>> getACleanedList() {
		// first clean the list
		cleanedList();
		// get a copy of elements
		List<ConnectedElementInterface<T, U>> out = null;
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
			final Iterator<ConnectedElementInterface<T, U>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElementInterface<T, U> elem = iterator.next();
				if (elem.isObjectDependent() && elem.lockObjects() == null) {
					elem.disconnect();
					iterator.remove();
				} else if (elem.getConsumer() == null && elem.getConsumer2() == null) {
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
	
	/**
	 * Connect a class with his hunction name
	 * @param object
	 * @param function
	 * @return
	 */
//	@CheckReturnValue
//	public <U> Connection connect(final U object, final String function) {
//		Type typeParam = (Class) ((T) getClass());
//		Method[] listMethod = object.getClass().getMethods();
//		Method findElem = null;
//		for (int iii=0; iii<listMethod.length; iii++) {
//			Method elem = listMethod[iii];
//			if (elem.getName().equals(function)) {
//				if (elem.getParameterCount() != 2) {
//					
//				} else {
//					Parameter[] params = elem.getParameters();
//					Parameter param = params[0];
//					if (!param.getType().equals(object.getClass()) ) {
//						// error
//						
//					} else {
//						
//					}
//					param = params[1];
//					if (! T.class.isAssignableFrom(param.getType()) ) {
//						// error
//						
//					} else {
//						
//					}
//					
//				}
//				
//			}
//		}
//		Method out = object.getClass().getMethod(function, U.class, T.class );
//		return null;
//	}

	// public void connectWeak(final T function) {
	// public <U> void connectWeak(final U object, final T+1 function) {
	// public Connection connect(final T function) {
	// public <U> Connection connect(final U object, final T+1 function) {
	// public <U> Connection connect(final U object, final String functionName) {
	
}
