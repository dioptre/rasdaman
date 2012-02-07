/*
 * Code that handles connection to the back-end database.
 * <p>
 * {@link Database} declares a generic database interface that is used 
 * throughout secore. Concrete classes can implement details for different 
 * databases (or the same database in different ways). {@link BaseX} for example
 * implements the interface with code specific to the BaseX database.
 * <p>
 * The {@link DbManager} is used to set the specific {@link Database} instance
 * that will be used.
 */
package secore.db;
