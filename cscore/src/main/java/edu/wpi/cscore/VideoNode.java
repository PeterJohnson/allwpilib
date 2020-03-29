/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.cscore;

/**
 * Base class for nodes.
 */
public class VideoNode implements AutoCloseable {
  protected VideoNode(int handle) {
    m_handle = handle;
  }

  @Override
  public synchronized void close() {
    if (m_handle != 0) {
      CameraServerJNI.releaseNode(m_handle);
    }
    m_handle = 0;
  }

  public boolean isValid() {
    return m_handle != 0;
  }

  public int getHandle() {
    return m_handle;
  }

  @Override
  public boolean equals(Object other) {
    if (this == other) {
      return true;
    }
    if (other == null) {
      return false;
    }
    if (getClass() != other.getClass()) {
      return false;
    }
    VideoNode node = (VideoNode) other;
    return m_handle == node.m_handle;
  }

  @Override
  public int hashCode() {
    return m_handle;
  }

  /**
   * Gets enable status.  On sources, this is determined with a combination of
   * connection strategy and the number of sinks connected.
   *
   * @return True if enabled, false otherwise.
   */
  public boolean isEnabled() {
    return CameraServerJNI.isNodeEnabled(m_handle);
  }

  /**
   * Set settings and properties from a JSON configuration string.
   *
   * <p>The format of the JSON input for sources is:
   *
   * <pre>
   * {
   *     "pixel format": "MJPEG", "YUYV", etc
   *     "width": video mode width
   *     "height": video mode height
   *     "fps": video mode fps
   *     "brightness": percentage brightness
   *     "white balance": "auto", "hold", or value
   *     "exposure": "auto", "hold", or value
   *     "properties": [
   *         {
   *             "name": property name
   *             "value": property value
   *         }
   *     ]
   * }
   * </pre>
   *
   * @param config configuration
   * @return True if set successfully
   */
  public boolean setConfigJson(String config) {
    return CameraServerJNI.setNodeConfigJson(m_handle, config);
  }

  /**
   * Get a JSON configuration string.
   *
   * @return JSON configuration string
   */
  public String getConfigJson() {
    return CameraServerJNI.getNodeConfigJson(m_handle);
  }

  /**
   * Get a property.
   *
   * @param name Property name
   * @return Property contents (of kind Property::kNone if no property with
   *         the given name exists)
   */
  public VideoProperty getProperty(String name) {
    return new VideoProperty(CameraServerJNI.getNodeProperty(m_handle, name));
  }

  /**
   * Enumerate all properties.
   */
  @SuppressWarnings("PMD.AvoidInstantiatingObjectsInLoops")
  public VideoProperty[] enumerateProperties() {
    int[] handles = CameraServerJNI.enumerateNodeProperties(m_handle);
    VideoProperty[] rv = new VideoProperty[handles.length];
    for (int i = 0; i < handles.length; i++) {
      rv[i] = new VideoProperty(handles[i]);
    }
    return rv;
  }

  protected int m_handle;
}
