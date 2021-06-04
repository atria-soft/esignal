package org.atriasoft.esignal;

/**
 * Interface to permit to the connection to unlink itself.
 */
public interface ConnectionRemoveInterface {
	/**
	 * Request the removing on the specify connection
	 * @param connection Connection to removed.
	 */
	void disconnect(final Connection connection);
}
