/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
*/
package org.odmg;

/**
* This interfaces provides the operations necessary to perform database transactions.
* All access, creation, and modification of persistent objects and their fields
* must be done within a transaction. Before performing any database operations,
* a thread must explicitly create a transaction object or associate itself with
* an existing transaction object (by calling <code>join</code>),
* and that transaction must be open (through a call to <code>begin</code>).
* All subsequent operations by the thread, including reads, writes, and lock
* acquisitions, are done under the thread's current transaction.
* <P>
* A thread may only operate on its current transaction. For example,
* a <code>TransactionNotInProgressException</code> is thrown if a thread attempts
* to begin, commit, checkpoint, or abort a transaction prior to joining itself
* to that transaction.
* <P>
* A transaction is either <i>open</i> or <i>closed</i>. A transaction is open if a call
* has been made to <code>begin</code>, but no call has been made to <code>commit</code> or
* <code>abort</code>. Once <code>commit</code> or <code>abort</code> is called,
* the transaction is closed. The method <code>isOpen</code> can be called to
* determine the state of the transaction.
* <P>
* Read locks are implicitly obtained on objects as they are accessed.
* Write locks are implicitly obtained as objects are modified.
* <code>Transaction</code> objects are transient, they cannot be stored in the database.
* @author	David Jordan (as Java Editor of the Object Data Management Group)
* @version ODMG 3.0
* @see TransactionNotInProgressException
*/

public interface Transaction {
/**
* Attach the caller's thread to this <code>Transaction</code> and detach the thread
* from any former <code>Transaction</code> the thread may have been associated with,
* <BR><I> not yet available in rasdaman. </BR></I>
*/
	public		void		join();

/**
* Detach the caller's thread from this <code>Transaction</code>, but do not attach
* the thread to another <code>Transaction</code>,
* <BR><I> not yet available in rasdaman. </BR></I>
*/
	public		void		leave();

/**
* Start a transaction.
* Calling <code>begin</code> multiple times on the same transaction object,
* without an intervening call to <code>commit</code> or <code>abort</code>,
* causes the exception <code>TransactionInProgressException</code> to be thrown
* on the second and subsequent calls. Operations executed before a transaction
* has been opened, or before reopening after a transaction is aborted or committed,
* have undefined results;
* these may throw a <code>TransactionNotInProgressException</code> exception.
*/
	public		void		begin();

/**
* Determine whether the transaction is open or not.
* A transaction is open if a call has been made to <code>begin</code>,
* but a subsequent call to either <code>commit</code> or <code>abort</code>
* has not been made.
* @return True if the transaction is open, otherwise false.
*/
	public		boolean		 isOpen();

/**
* Commit and close the transaction.
* Calling <code>commit</code> commits to the database all persistent object
* modifications within the transaction and releases any locks held by the transaction.
* A persistent object modification is an update of any field of an existing
* persistent object, or an update or creation of a new named object in the database.
* If a persistent object modification results in a reference from an existing
* persistent object to a transient object, the transient object is moved to the
* database, and all references to it updated accordingly. Note that the act of
* moving a transient object to the database may create still more persistent
* references to transient objects, so its referents must be examined and moved as well.
* This process continues until the database contains no references to transient objects,
* a condition that is guaranteed as part of transaction commit.
* Committing a transaction does not remove from memory transient objects created
* during the transaction
*/
	public		void		commit();

/**
* Abort and close the transaction.
* Calling abort abandons all persistent object modifications and releases the
* associated locks.
* Aborting a transaction does not restore the state of modified transient objects
*/
	public		void		abort();

/**
* Commit the transaction, but reopen the transaction, retaining all locks.
* Calling <code>checkpoint</code> commits persistent object modifications made
* within the transaction since the last checkpoint to the database.
* The transaction retains all locks it held on those objects at the time the
* checkpoint was invoked.
*/
	public		void		checkpoint();

/**
* Read lock mode.
*/
	public static final int  READ = 1;

/**
* Upgrade lock mode.
*/
	public static final int  UPGRADE = 2;

/**
* Write lock mode.
*/
	public static final int  WRITE = 4;

/**
* Upgrade the lock on the given object to the given lock mode,
* <BR><I> not yet available in rasdaman. </BR></I>
* The call has no effect if the object's current lock is already at or above
* that level of lock mode.
* @param	obj	The object to acquire a lock on.
* @param	lockMode	The lock mode to acquire. The lock modes are <code>READ</code>,
* <code>UPGRADE</code>, and <code>WRITE</code>.
* @exception LockNotGrantedException	Is thrown if the given lock mode could not be acquired.
*/
	public		void		lock(Object obj, int lockMode)
				 		throws LockNotGrantedException;
/**
* Upgrade the lock on the given object to the given lock mode,
* <BR><I> not yet available in rasdaman. </BR></I>
* Method <code>tryLock</code> is the same as <code>lock</code> except it returns
* a boolean indicating whether the lock was granted instead of generating an exception.
* @param	obj	The object to acquire a lock on.
* @param	lockMode	The lock mode to acquire. The lock modes are <code>READ</code>,
* <code>UPGRADE</code>, and <code>WRITE</code>.
* @return	True if the lock has been acquired, otherwise false.
*/
	public		boolean 		 tryLock(Object obj, int lockMode);

}

