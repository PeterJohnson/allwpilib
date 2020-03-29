/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.cscore;

/**
 * Server configuration.
 */
public class ServerConfig {
  public ServerConfig(int port) {
    this.port = port;
  }

  public ServerConfig(int port, VideoSource defaultSource) {
    this.port = port;
    this.defaultSource = defaultSource;
  }

  public ServerConfig(String address, int port) {
    this.address = address;
    this.port = port;
  }

  public ServerConfig(String address, int port, VideoSource defaultSource) {
    this.address = address;
    this.port = port;
    this.defaultSource = defaultSource;
  }

  public ServerConfig(String address, int port, VideoSource defaultSource, VideoSource[] onlySources) {
    this.address = address;
    this.port = port;
    this.defaultSource = defaultSource;
    this.onlySources = onlySources;
  }

  /**
   * Listen address.  If empty, serves on all addresses.
   */
  public String address;

  /**
   * Listen port for this server.
   */
  public int port;

  /**
   * If not null, the source to stream when no source is specified in request.
   */
  public VideoSource defaultSource;

  /**
   * If not empty, only allow specified sources to be streamed through this
   * particular server.
   */
  public VideoSource[] onlySources;
}
