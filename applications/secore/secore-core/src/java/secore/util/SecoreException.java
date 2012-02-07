/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2012 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package secore.util;

import net.opengis.ows.v_1_0_0.ExceptionReport;
import net.opengis.ows.v_1_0_0.ExceptionType;

/**
 * This exception can return an error report, that can be marshalled into a
 * standard compliant XML structure describing the error that has happened.
 * 
 * @author Mihaela Rusu
 * @author Dimitar Misev
 */
public class SecoreException extends Exception {

  private ExceptionCode code;
  private ExceptionType type;
  private ExceptionReport report;

  private SecoreException() {
  }

  /**
   * Construct a new exception
   *
   * @param exceptionCode exception code, if it's null then UnknownError is used.
   */
  public SecoreException(ExceptionCode exceptionCode) {
    this(exceptionCode, null, null);
  }

  /**
   * Construct a new exception
   *
   * @param exceptionCode exception code, if it's null then UnknownError is used.
   * @param exceptionText exception message
   */
  public SecoreException(ExceptionCode exceptionCode, String exceptionText) {
    this(exceptionCode, exceptionText, null);
  }

  /**
   * Construct a new exception
   *
   * @param exceptionCode exception code, if it's null then UnknownError is used.
   * @param ex original exception
   */
  public SecoreException(ExceptionCode exceptionCode, Exception ex) {
    this(exceptionCode, ex.getLocalizedMessage(), ex);
  }

  /**
   * Construct a new exception
   *
   * @param exceptionCode exception code (must not be null)
   * @param exceptionText exception message
   * @param ex original exception
   */
  public SecoreException(ExceptionCode exceptionCode, String exceptionText, Exception ex) {
    this(exceptionCode, exceptionText, ex, Config.VERSION, Config.LANGUAGE);
  }

  /**
   * Construct a new exception
   *
   * @param exceptionCode exception code, if it's null then UnknownError is used.
   * @param exceptionText exception message
   * @param ex original exception
   * @param version
   * @param language
   */
  protected SecoreException(ExceptionCode exceptionCode, String exceptionText, Exception ex, String version, String language) {
    super(exceptionText, ex);
    if (exceptionCode == null) {
      exceptionCode = ExceptionCode.UnknownError;
    }
    code = exceptionCode;

    report = new ExceptionReport();
    report.setLanguage(language);
    report.setVersion(version);

    type = new ExceptionType();
    type.setExceptionCode(exceptionCode.getExceptionCode());
    type.setLocator(exceptionCode.getLocator());
    if (exceptionText != null) {
      type.getExceptionText().add(exceptionText);
    } else if (exceptionCode.getDescription() != null) {
      type.getExceptionText().add(exceptionCode.getDescription());
    }
    report.getException().add(type);
  }

  /**
   * @return Return the error code.
   */
  public ExceptionCode getExceptionCode() {
    return code;
  }

  /**
   * @return Return the detailed error message.
   */
  public String getExceptionText() {
    return StringUtil.ltos(type.getExceptionText(), Constants.NEW_LINE_CHAR);
  }

  /**
   * Retrieves a data structure that can be later marshalled into a XML
   * ExceptionReport" document.
   *
   * @return ExceptionReport object
   */
  public ExceptionReport getReport() {
    return report;
  }

  /**
   * Adds text to this exception's detail message.
   *
   * @param msg
   */
  public void appendExceptionText(String msg) {
    type.getExceptionText().add(msg);
  }

  @Override
  public String getMessage() {
    return getExceptionText();
  }

  @Override
  public String toString() {
    return type.getExceptionCode() + ": " + getMessage();
  }
}
