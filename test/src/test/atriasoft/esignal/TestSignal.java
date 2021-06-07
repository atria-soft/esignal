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
public class TestSignal {

	class EmiterSimple {
		public Signal<String> signalEvent = new Signal<String>();
		public void sendEvent(final String value) {
			this.signalEvent.emit(value);
		}
	}
	static class ReceiverSimple implements AutoCloseable {
		private String dataReceive = null;
		ReceiverSimple() {
		}

		// record consumer
		private Consumer<String> tmpConsumer1 = null;
		private Connection tmpConnect = null;
		@Override
		public void close() {
			this.tmpConsumer1 = null;
			if (this.tmpConnect != null) {
				this.tmpConnect.close();
				this.tmpConnect = null;
			}
			this.dataReceive = null;
		}
		public void disconnectConnection() {
			this.tmpConnect.close();
		}
		public boolean isConnected() {
			return this.tmpConnect.isConnected();
		}
		
		
		public void connect1(final EmiterSimple other) {
			WeakReference<ReceiverSimple> self = new WeakReference<ReceiverSimple>(this);
			this.tmpConnect = other.signalEvent.connect(data -> {
					self.get().onData(data);
				});
			System.gc();
		}
		public void connect2(final EmiterSimple other) {
			// the solo lambda will not depend on the object => the remove must be done manually... 
			this.tmpConnect = other.signalEvent.connect(data -> {
					Log.error("lambda receive: " + data);
				});
			System.gc();
		}
		public void connect3(final EmiterSimple other) {
			// we reference the local object, then the lambda is alive while the object is alive...
			this.tmpConnect =other.signalEvent.connect(data -> {
					Log.error("lambda receive: " + data);
					this.dataReceive = data;
				});
			System.gc();
		}
		public void connect4(final EmiterSimple other) {
			this.tmpConnect = other.signalEvent.connect(data -> {
					onData(data);
				});
			System.gc();
		}
		public void connect5(final EmiterSimple other) {
			this.tmpConsumer1 = this::onData;
			other.signalEvent.connectWeak(this.tmpConsumer1);
			System.gc();
		}
		
		public void disconnectConsumer1(final EmiterSimple other) {
			other.signalEvent.disconnect(this.tmpConsumer1);
			this.tmpConsumer1 = null;
		}

		public void connect6(final EmiterSimple other) {
			// the solo lambda will not depend on the object => the remove must be done manually... 
			other.signalEvent.connectAutoRemoveObject(this, data -> {
					Log.error("lambda receive: " + data);
				});
			System.gc();
		}
		
		public void connect7(final EmiterSimple other) {
			this.tmpConnect = other.signalEvent.connect(this::onData);
			System.gc();
		}

		public void disconnect7(final EmiterSimple other) {
			other.signalEvent.disconnect(this.tmpConnect);
		}

		public void onData(final String data) {
			Log.error("Retrive data : " + data);
			this.dataReceive = data; 
		}
		

		public void connect8(final EmiterSimple other) {
			this.tmpConnect = other.signalEvent.connect(this, ReceiverSimple::onDataStatic);
			System.gc();
		}

		public void connect9(final EmiterSimple other) {
			other.signalEvent.connectAuto(this, ReceiverSimple::onDataStatic);
			System.gc();
		}


		public static void onDataStatic(final Object local, final String data) {
			ReceiverSimple self = (ReceiverSimple)local;
			Log.error("Retrive data : " + data);
			self.dataReceive = data; 
		}


		public void connect10(final EmiterSimple other) {
			this.tmpConnect = other.signalEvent.connect(this, ReceiverSimple::onDataStatic2);
			System.gc();
		}

		public void connect11(final EmiterSimple other) {
			other.signalEvent.connectAuto(this, ReceiverSimple::onDataStatic2);
			System.gc();
		}
		
		public static void onDataStatic2(final ReceiverSimple self, final String data) {
			Log.error("Retrive data : " + data);
			self.dataReceive = data; 
		}
		
		public void connect12(final EmiterSimple other) {
			this.tmpConnect = other.signalEvent.connect(this, (final ReceiverSimple self, final String data) -> {
				Log.error("Retrive data : " + data);
				self.dataReceive = data; 
			});
			System.gc();
		}

		public void connect13(final EmiterSimple other) {
			other.signalEvent.connectAuto(this, (final ReceiverSimple self, final String data) -> {
				Log.error("Retrive data : " + data);
				self.dataReceive = data; 
			});
			System.gc();
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
		// !!!! NO need to close lambda does not capture the THIS
		receiver = null;
		System.gc();
		String testData2 = "Solo Lambda MUST receive this data...";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size());
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
		// Need to close ==> capture Of this
		receiver.close();
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
		// Need to close ==> capture Of this
		receiver.close();
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
		receiver.disconnectConsumer1(sender);
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
		// No need to close ==> the capture seams does not increase the cumber of Reference ....
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
		receiver.disconnectConnection();
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
		// Need to close to prevent  miss removing of connection.
		receiver.close();
		// check auto remove...
		receiver = null;
		System.gc();
		String testData6 = "MUST NOT receive this data... 666";
		sender.sendEvent(testData6);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 7 [ END ]");
		
	}

	@Test
	@Order(8)
	public void testConnectAndTransmit8() {
		Log.warning("Test 8 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect8(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		Assertions.assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnectConnection();
		Assertions.assertEquals(false, receiver.isConnected());
		System.gc();
		String testData2 = "MUST NOT receive this data... 222";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect8(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData3 = "MUST receive this data... 333";
		sender.sendEvent(testData3);
		Assertions.assertEquals(testData3, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		Assertions.assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnectConnection();
		Assertions.assertEquals(false, receiver.isConnected());
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		System.gc();
		String testData4 = "MUST NOT receive this data... 444";
		sender.sendEvent(testData4);
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect8(sender);
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
		Log.warning("Test 8 [ END ]");
		
	}
	
	@Test
	@Order(9)
	public void testConnectAndTransmit9() {
		Log.warning("Test 9 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect9(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		// remove connection (check auto remove)...
		receiver = null;
		System.gc();
		String testData6 = "MUST NOT receive this data... 666";
		sender.sendEvent(testData6);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 9 [ END ]");
		
	}

	@Test
	@Order(10)
	public void testConnectAndTransmit10() {
		Log.warning("Test 10 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect10(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		Assertions.assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnectConnection();
		Assertions.assertEquals(false, receiver.isConnected());
		System.gc();
		String testData2 = "MUST NOT receive this data... 222";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect10(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData3 = "MUST receive this data... 333";
		sender.sendEvent(testData3);
		Assertions.assertEquals(testData3, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		Assertions.assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnectConnection();
		Assertions.assertEquals(false, receiver.isConnected());
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		System.gc();
		String testData4 = "MUST NOT receive this data... 444";
		sender.sendEvent(testData4);
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect10(sender);
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
		Log.warning("Test 10 [ END ]");
		
	}
	
	@Test
	@Order(11)
	public void testConnectAndTransmit11() {
		Log.warning("Test 11 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect11(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		// remove connection (check auto remove)...
		receiver = null;
		System.gc();
		String testData6 = "MUST NOT receive this data... 666";
		sender.sendEvent(testData6);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 11 [ END ]");
		
	}

	@Test
	@Order(12)
	public void testConnectAndTransmit12() {
		Log.warning("Test 12 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect12(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		Assertions.assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnectConnection();
		Assertions.assertEquals(false, receiver.isConnected());
		System.gc();
		String testData2 = "MUST NOT receive this data... 222";
		sender.sendEvent(testData2);
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect12(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData3 = "MUST receive this data... 333";
		sender.sendEvent(testData3);
		Assertions.assertEquals(testData3, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		Assertions.assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnectConnection();
		Assertions.assertEquals(false, receiver.isConnected());
		Assertions.assertEquals(0, sender.signalEvent.size()); 
		System.gc();
		String testData4 = "MUST NOT receive this data... 444";
		sender.sendEvent(testData4);
		Assertions.assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect12(sender);
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
		Log.warning("Test 12 [ END ]");
		
	}
	
	@Test
	@Order(13)
	public void testConnectAndTransmit13() {
		Log.warning("Test 13 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect13(sender);
		Assertions.assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 131";
		sender.sendEvent(testData1);
		Assertions.assertEquals(testData1, receiver.getDataAndClean());
		Assertions.assertEquals(1, sender.signalEvent.size());
		// remove connection (check auto remove)...
		receiver = null;
		System.gc();
		String testData6 = "MUST NOT receive this data... 666";
		sender.sendEvent(testData6);
		Assertions.assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 13 [ END ]");
		
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
