/*******************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Contributors:
 *     Edouard DUPIN - initial API and implementation
 ******************************************************************************/
package test.atriasoft.esignal;

import java.lang.ref.WeakReference;
import java.util.function.Consumer;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;

import org.junit.Test;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.MethodOrderer.OrderAnnotation;
import org.junit.jupiter.api.Order;
//import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestMethodOrder;

@TestMethodOrder(OrderAnnotation.class)
public class TestSignalType {

	class EmiterSimple {
		public Signal<String> signalEvent = new Signal<String>();
		public void sendEvent(final String value) {
			this.signalEvent.emit(value);
		}
	}
	class ReceiverSimple {
		private String dataReceive = null;
		ReceiverSimple() {
		}
		public void connect1(final EmiterSimple other) {
			WeakReference<ReceiverSimple> tmpp = new WeakReference<ReceiverSimple>(this);
			other.signalEvent.connect(data -> {
					tmpp.get().onData(data);
				});
		}
		public void connect2(final EmiterSimple other) {
			// the solo lambda will not depend on the object => the remove must be done manually... 
			other.signalEvent.connect(data -> {
					Log.error("lambda receive: " + data);
				});
		}
		public void connect3(final EmiterSimple other) {
			// we reference the local object, then the lambda is alive while the object is alive...
			other.signalEvent.connect(data -> {
					Log.error("lambda receive: " + data);
					this.dataReceive = data;
				});
		}
		public void connect4(final EmiterSimple other) {
			other.signalEvent.connect(data -> {
					onData(data);
				});
		}
		// record consumer
		private Consumer<String> tmpConsumer = null;
		public void connect5(final EmiterSimple other) {
			this.tmpConsumer = this::onData;
			other.signalEvent.connect(this.tmpConsumer);
		}
		
		public void disconnect5(final EmiterSimple other) {
			other.signalEvent.disconnect(this.tmpConsumer);
			this.tmpConsumer = null;
		}

		public void connect6(final EmiterSimple other) {
			// the solo lambda will not depend on the object => the remove must be done manually... 
			other.signalEvent.connectAutoRemoveObject(this, data -> {
					Log.error("lambda receive: " + data);
				});
		}
		private Connection tmpConnect = null;
		
		public void connect7(final EmiterSimple other) {
			this.tmpConnect = other.signalEvent.connectDynamic(this::onData);
		}

		public void disconnect7(final EmiterSimple other) {
			other.signalEvent.disconnect(this.tmpConnect);
		}

		public void disconnect72() {
			this.tmpConnect.disconnect();
		}
		public boolean isConnected() {
			return this.tmpConnect.isConnected();
		}
		
		public void onData(final String data) {
			Log.error("Retrive data : " + data);
			this.dataReceive = data; 
		}
		public String getDataAndClean() {
			String tmp = this.dataReceive;
			this.dataReceive = null;
			return tmp;
		}
		
	}

	@Test
	@Order(1)
	public void testConnectAndTransmit1() {
		Log.warning("Test 1 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect1(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		receiver = null;
		Assertions.assertEquals(1, sender.signalEvent.size());
		System.gc();
		String testData2 = "MUST NOT receive this data...";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 1 [ END ]");
		
	}
	@Test
	@Order(2)
	public void testConnectAndTransmit2() {
		Log.warning("Test 2 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect2(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		// No data stored ... assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		receiver = null;
		System.gc();
		String testData2 = "Solo Lambda MUST receive this data...";
		sender.sendEvent(testData2);
		Assertions.assertEquals(1, sender.signalEvent.size());
		Log.warning("Test 2 [ END ]");
	}

	@Test
	@Order(3)
	public void testConnectAndTransmit3() {
		Log.warning("Test 3 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect3(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		receiver = null;
		System.gc();
		String testData2 = "MUST NOT receive this data...";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 3 [ END ]");
		
	}

	@Test
	@Order(4)
	public void testConnectAndTransmit4() {
		Log.warning("Test 4 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect4(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		receiver = null;
		System.gc();
		String testData2 = "MUST NOT receive this data...";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 4 [ END ]");
		
	}

	@Test
	@Order(5)
	public void testConnectAndTransmit5() {
		Log.warning("Test 5 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect5(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		// remove connection
		receiver.disconnect5(sender);
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		System.gc();
		String testData2 = "MUST NOT receive this data... 222";
		sender.sendEvent(testData2);
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect5(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData3 = "MUST receive this data... 333";
		sender.sendEvent(testData3);
		Assertions.assertEquals(testData3, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		// check auto remove...
		receiver = null;
		System.gc();
		String testData4 = "MUST NOT receive this data... 444";
		sender.sendEvent(testData4);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 5 [ END ]");
		
	}
	@Test
	@Order(6)
	public void testConnectAndTransmit6() {
		Log.warning("Test 6 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect6(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		//assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		receiver = null;
		System.gc();
		String testData2 = "MUST NOT receive this data...";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 6 [ END ]");
		
	}

	@Test
	@Order(7)
	public void testConnectAndTransmit7() {
		Log.warning("Test 7 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect7(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		Assertions.assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnect7(sender);
		Assertions.assertEquals(false, receiver.isConnected());
		System.gc();
		String testData2 = "MUST NOT receive this data... 222";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect7(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData3 = "MUST receive this data... 333";
		sender.sendEvent(testData3);
		Assertions.assertEquals(testData3, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		Assertions.assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnect72();
		Assertions.assertEquals(false, receiver.isConnected());
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		System.gc();
		String testData4 = "MUST NOT receive this data... 444";
		sender.sendEvent(testData4);
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect7(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData5 = "MUST receive this data... 555";
		sender.sendEvent(testData5);
		Assertions.assertEquals(testData5, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		// check auto remove...
		receiver = null;
		System.gc();
		String testData6 = "MUST NOT receive this data... 666";
		sender.sendEvent(testData6);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 7 [ END ]");
		
	}

	@Test
	public void testClearConnection() {
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect7(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		sender.signalEvent.clear();
	}
}
