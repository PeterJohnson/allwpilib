/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.cscore;

/**
 * A source that represents a network video stream.
 */
public class NetworkSource extends VideoCamera {
  /**
   * Create a source for a network video stream.
   *
   * @param name Source name (arbitrary unique identifier)
   * @param url Camera URL (e.g. "http://10.x.y.11/video/stream.mjpg")
   */
  public NetworkSource(String name, String url) {
    super(CameraServerJNI.createNetworkSource(name, url));
  }

  /**
   * Create a source for a network video stream.
   *
   * @param name Source name (arbitrary unique identifier)
   * @param urls Array of Camera URLs
   */
  public NetworkSource(String name, String[] urls) {
    super(CameraServerJNI.createNetworkSourceMulti(name, urls));
  }

  /**
   * Change the URLs used to connect to the camera.
   */
  public void setUrls(String[] urls) {
    CameraServerJNI.setNetworkSourceUrls(m_handle, urls);
  }

  /**
   * Get the URLs used to connect to the camera.
   */
  public String[] getUrls() {
    return CameraServerJNI.getNetworkSourceUrls(m_handle);
  }
}
