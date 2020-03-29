/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.cscore;

/**
 * A network server for video.
 */
public class VideoServer extends VideoNode implements AutoCloseable {
  /**
   * Create a network video server.  By default, the server will be stopped
   * when this object is destroyed; call Detach() to keep the server running
   * in the background.
   *
   * @param config server configuration
   */
  public VideoServer(ServerConfig config) {
    super(CameraServerJNI.startServer(config));
  }

  /**
   * Create a network video server.  By default, the server will be stopped
   * when this object is destroyed; call Detach() to keep the server running
   * in the background.
   *
   * The format of the JSON input is:
   *
   * <pre>
   * {
   *     "address": "address",                   // optional
   *     "port": port number,
   *     "default source": "name",               // optional
   *     "only sources": ["name1", "name2"],     // optional
   *     "properties": [                         // optional
   *         {
   *             "name": "property name",
   *             "value": property value
   *         }
   *     ]
   * }
   * </pre>
   *
   * @param config JSON server configuration
   */
  public VideoServer(String config) {
    super(CameraServerJNI.startServerJson(config));
  }

  /**
   * Create a network video server.  By default, the server will be stopped
   * when this object is destroyed; call Detach() to keep the server running
   * in the background.  For more than one listen address, use the ServerConfig
   * constructor.
   *
   * @param listenAddress TCP listen address (empty string for all addresses)
   * @param port TCP port number
   */
  public VideoServer(String listenAddress, int port) {
    super(CameraServerJNI.startServer(new ServerConfig(listenAddress, port)));
  }

  /**
   * Create a network video server.  By default, the server will be stopped
   * when this object is destroyed; call Detach() to keep the server running
   * in the background.  For more than one listen address, use the ServerConfig
   * constructor.
   *
   * @param listenAddress TCP listen address (empty string for all addresses)
   * @param port TCP port number
   * @param defaultSource source to stream when no source is specified in
   *                      request
   */
  public VideoServer(String listenAddress, int port, String defaultSource) {
    super(CameraServerJNI.startServer(new ServerConfig(listenAddress, port, defaultSource)));
  }

  /**
   * Create a network video server.  By default, the server will be stopped
   * when this object is destroyed; call Detach() to keep the server running
   * in the background.
   *
   * @param port TCP port number
   */
  public VideoServer(int port) {
    super(CameraServerJNI.startServer(new ServerConfig(port)));
  }

  /**
   * Create a network video server.  By default, the server will be stopped
   * when this object is destroyed; call Detach() to keep the server running
   * in the background.
   *
   * @param port TCP port number
   * @param defaultSource source to stream when no source is specified in
   *                      request
   */
  public VideoServer(int port, String defaultSource) {
    super(CameraServerJNI.startServer(new ServerConfig(port, defaultSource)));
  }

  @Override
  public synchronized void close() {
    if (m_owned && m_handle != 0) {
      CameraServerJNI.stopServer(m_handle);
    }
    m_owned = false;
    m_handle = 0;
  }

  /**
   * Detaches the server so it keeps running in the background even if
   * this object is destroyed.
   */
  public void detach() {
    m_owned = false;
  }

  /**
   * Returns if this server instance is detached.
   */
  public boolean isDetached() {
    return !m_owned;
  }

  /**
   * Stops the server.
   */
  public void stop() {
    CameraServerJNI.stopServer(m_handle);
  }

  /**
   * Stops all servers.
   */
  public static void stopAll() {
    CameraServerJNI.stopAllServers();
  }

  /**
   * Gets server configuration.
   */
  public ServerConfig getConfig() {
    return CameraServerJNI.getServerConfig(m_handle);
  }

  /**
   * Get the listen address of the server.
   */
  public String getListenAddress() {
    return getConfig().address;
  }

  /**
   * Get the port number of the server.
   */
  public int getPort() {
    return getConfig().port;
  }

  /**
   * Set the stream resolution for clients that don't specify it.
   *
   * <p>Setting this different than the source resolution will result in
   * increased CPU usage.
   *
   * @param source source name (if empty, applies to all sources)
   * @param width width, 0 for unspecified
   * @param height height, 0 for unspecified
   */
  public void setResolution(VideoSource source, int width, int height) {
    CameraServerJNI.setServerResolution(m_handle, source.getHandle(), width, height);
  }

  /**
   * Set the stream frames per second (FPS) for clients that don't specify it.
   *
   * <p>It is not necessary to set this if it is the same as the source FPS.
   *
   * @param source source name (if empty, applies to all sources)
   * @param fps FPS, 0 for unspecified
   */
  public void setFPS(VideoSource source, int fps) {
    CameraServerJNI.setServerFPS(m_handle, source.getHandle(), fps);
  }

  /**
   * Set the JPEG compression for clients that don't specify it.
   *
   * <p>Setting this will result in increased CPU usage for MJPEG source cameras
   * as it will decompress and recompress the image instead of using the
   * camera's MJPEG image directly.
   *
   * @param source source name (if empty, applies to all sources)
   * @param quality JPEG compression quality (0-100), -1 for unspecified
   */
  public void setJpegQuality(VideoSource source, int quality) {
    CameraServerJNI.setServerJpegQuality(m_handle, source.getHandle(), quality);
  }

  /**
   * Set the default compression used for non-MJPEG sources.  If not set,
   * 80 is used.  This function has no effect on MJPEG source cameras; use
   * SetCompression() instead to force recompression of MJPEG source images.
   *
   * @param source source name (if empty, applies to all sources)
   * @param quality JPEG compression quality (0-100)
   */
  public void setDefaultJpegQuality(VideoSource source, int quality) {
    CameraServerJNI.setServerDefaultJpegQuality(m_handle, source.getHandle(), quality);
  }

  private boolean m_owned = true;
}
