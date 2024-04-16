// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.util.datalog;

/** Log entry base class. */
public class DataLogEntry {
  /**
   * Constructs a data log entry.
   *
   * @param log datalog
   * @param name name of the entry
   * @param type Data type
   * @param metadata metadata
   * @param timestamp entry creation timestamp (0=now)
   */
  protected DataLogEntry(DataLog log, String name, String type, String metadata, long timestamp) {
    m_log = log;
    m_entry = log.start(name, type, metadata, timestamp);
  }

  /**
   * Constructs a data log entry.
   *
   * @param log datalog
   * @param name name of the entry
   * @param type Data type
   * @param metadata metadata
   */
  protected DataLogEntry(DataLog log, String name, String type, String metadata) {
    this(log, name, type, metadata, 0);
  }

  /**
   * Constructs a data log entry.
   *
   * @param log datalog
   * @param name name of the entry
   * @param type Data type
   */
  protected DataLogEntry(DataLog log, String name, String type) {
    this(log, name, type, "");
  }

  /**
   * Updates the metadata for the entry.
   *
   * @param metadata New metadata for the entry
   * @param timestamp Time stamp (0 to indicate now)
   */
  public void setMetadata(String metadata, long timestamp) {
    m_log.setMetadata(m_entry, metadata, timestamp);
  }

  /**
   * Updates the metadata for the entry.
   *
   * @param metadata New metadata for the entry
   */
  public void setMetadata(String metadata) {
    setMetadata(metadata, 0);
  }

  /**
   * Sets duplicate detection behavior for an entry.  By default, this is
   * disabled, so every append() call results in output to the file.  With this
   * enabled, append() will only output to the file if the value is different
   * than the previous value.  Enabling duplicate prevention force-enables
   * saving of the last value.
   *
   * @param enable duplicate prevention enable (true) / disable (false)
   */
  public void preventDuplicates(boolean enable) {
    m_log.preventDuplicates(m_entry, enable);
  }

  /**
   * Sets last value saving behavior for an entry.  By default, this is
   * disabled.  Enabling this consumes a bit more time and memory but is
   * required for getLast() functions to work.
   *
   * @param enable last value saving enable (true) / disable (false)
   */
  public void saveLastValues(boolean enable) {
    m_log.saveLastValues(m_entry, enable);
  }

  /**
   * Finishes the entry.
   *
   * @param timestamp Time stamp (0 to indicate now)
   */
  public void finish(long timestamp) {
    m_log.finish(m_entry, timestamp);
  }

  /** Finishes the entry. */
  public void finish() {
    finish(0);
  }

  /** The data log instance associated with the entry. */
  protected final DataLog m_log;

  /** The data log entry index. */
  protected final int m_entry;
}
